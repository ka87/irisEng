#include "../sound.h"
#include "../win/winsound.h"
#include "../ai.h"
#include "ai_mathfunctions.h"
#include "ai_soundfunctions.h"

tAINode* AI_EvalFunc_SNDSetGlobalPan(tAINode* a1)
{
    int v1;

    v1 = AI_GetIntFromStack();
    SND_SetGlobalPan(v1);
    return a1 + 1;
}

tAINode* AI_EvalFunc_SNDGetGlobalPan(tAINode* a1)
{
    int v1;

    v1 = SND_l_GetGlobalPan();
    AI_PutIntToStack(v1);
    return a1 + 1;
}

tAINode* AI_EvalFunc_SNDSetGlobalVol(tAINode* a1)
{
    float v2;

    v2 = AI_GetFloatFromStack();
    SND_SetGlobalVol(v2);
    return a1 + 1;
}

tAINode* AI_EvalFunc_SNDGetGlobalVol(tAINode* a1)
{
    float v2;

    v2 = SND_f_GetGlobalVol();
    AI_PutFloatToStack(v2);
    return a1 + 1;
}

tAINode* AI_EvalFunc_SNDGetSound(tAINode* a1)
{
    int v1;
    int v2;

    v1 = AI_GetIntFromStack();
    v2 = SND_l_GetSoundIndex(v1);
    AI_PutIntToStack(v2);
    return a1 + 1;
}

int AI_EvalFunc_SNDRequest_C(struct sGameObjectMain* gao, int a2, int a3)
{
    int idx;
    Mtx4x4* v5;

    if (!SND_gst_Params.soundEnabled)
        return -1;
    QueryPerformanceCounter(&PerfCounterStruct[39]);
    v5 = gao->matrices;
    idx = SND_l_Request(gao, a2);
    if (idx != -1)
    {
        SND_SetFlags(gao, idx, a3);
        SND_SetPos(gao, idx, v5);
        if ((a3 & 0x800) != 0)
            SND_PlaySound(idx);
    }
    PRO_QPC_RecordTimeDecRef(39);
    return idx;
}

tAINode* AI_EvalFunc_SNDRequest(tAINode* a1)
{
    int v1;
    int v2;
    tGameObjectMain* v4;
    int a3;

    a3 = AI_GetIntFromStack();
    v4 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v1 = AI_GetIntFromStack();
    v2 = AI_EvalFunc_SNDRequest_C(v4, v1, a3);
    AI_PutIntToStack(v2);
    return a1 + 1;
}

int AI_EvalFunc_SNDRequestPlay_C(struct sGameObjectMain* gao, int a2, float a3)
{
    int a2a;

    if (!SND_gst_Params.soundEnabled)
        return -1;
    a2a = AI_EvalFunc_SNDRequest_C(gao, a2, 0x18000080);
    QueryPerformanceCounter(&PerfCounterStruct[39]);
    if (a3 != -1.0)
        SND_SetVol(a2a, a3);
    SND_PlaySound(a2a);
    PRO_QPC_RecordTimeDecRef(39);
    return a2a;
}

int AI_EvalFunc_SNDRequestPlay_C_CURRENT(int a1, float a2)
{
    return AI_EvalFunc_SNDRequestPlay_C(AI_gpst_CurrentGameObject, a1, a2);
}

tAINode* AI_EvalFunc_SNDRequestPlay(tAINode* a1)
{
    int v1;
    tGameObjectMain* v3;
    int a2;
    float a3;

    a3 = AI_GetFloatFromStack();
    a2 = AI_GetIntFromStack();
    v3 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v1 = AI_EvalFunc_SNDRequestPlay_C(v3, a2, a3);
    AI_PutIntToStack(v1);
    return a1 + 1;
}

int AI_EvalFunc_SNDRequestOnTrack_C(tGameObjectMain* a1, int a2, int a3)
{
    int idx;
    Mtx4x4* v5;

    if (!SND_gst_Params.soundEnabled)
        return -1;
    QueryPerformanceCounter(&PerfCounterStruct[39]);
    v5 = a1->matrices;
    idx = SND_l_Request(a1, a2);
    if (idx != -1)
    {
        SND_SetFlags(a1, idx, a3);
        SND_SetPos(a1, idx, v5);
        if ((a3 & 0x800) != 0)
            SND_PlaySoundOnTrack(idx);
    }
    PRO_QPC_RecordTimeDecRef(39);
    return idx;
}

tAINode* AI_EvalFunc_SNDRequestOnTrack(tAINode* a1)
{
    int v1;
    int v2;
    tGameObjectMain* v4;
    int v5;

    v5 = AI_GetIntFromStack();
    v4 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v1 = AI_GetIntFromStack();
    v2 = AI_EvalFunc_SNDRequestOnTrack_C(v4, v1, v5);
    AI_PutIntToStack(v2);
    return a1 + 1;
}

int AI_EvalFunc_SNDRequestPlayOnTrack_C(tGameObjectMain* a1, int a2, float a3)
{
    int v4;

    if (!SND_gst_Params.soundEnabled)
        return -1;
    v4 = AI_EvalFunc_SNDRequestOnTrack_C(a1, a2, 0x18000080);
    QueryPerformanceCounter(&PerfCounterStruct[39]);
    SND_SetVol(v4, a3);
    SND_PlaySoundOnTrack(v4);
    PRO_QPC_RecordTimeDecRef(39);
    return v4;
}

tAINode* AI_EvalFunc_SNDRequestPlayOnTrack(tAINode* a1)
{
    int v1;
    tGameObjectMain* v3;
    int a2;
    float a3;

    a3 = AI_GetFloatFromStack();
    a2 = AI_GetIntFromStack();
    v3 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v1 = AI_EvalFunc_SNDRequestPlayOnTrack_C(v3, a2, a3);
    AI_PutIntToStack(v1);
    return a1 + 1;
}

tAINode* AI_EvalFunc_SNDDestroy(tAINode* a1)
{
    int v1;

    v1 = AI_GetIntFromStack();
    SND_Release(v1);
    return a1 + 1;
}

void SND_SetNearFarS(int idx, float a2, float a3)
{
    QueryPerformanceCounter(&PerfCounterStruct[39]);
    SND_SetNearFar(idx, 4096, a2, a3);
    PRO_QPC_RecordTimeDecRef(39);
}

tAINode* AI_EvalFunc_SNDSetNearFar(tAINode* a1)
{
    int v1;
    float v3;
    float v4;

    v4 = AI_GetFloatFromStack();
    v3 = AI_GetFloatFromStack();
    v1 = AI_GetIntFromStack();
    SND_SetNearFarS(v1, v3, v4);
    return a1 + 1;
}

tAINode* AI_EvalFunc_SNDSetPan(tAINode* a1)
{
    int v1;
    int v3;
    int a3;

    a3 = AI_GetIntFromStack();
    v3 = AI_GetIntFromStack();
    v1 = AI_GetIntFromStack();
    SND_SetPan(v1, v3, a3);
    return a1 + 1;
}

tAINode* AI_EvalFunc_SNDGetPan(tAINode* a1)
{
    int v1;
    int v2;

    v1 = AI_GetIntFromStack();
    v2 = SND_l_GetPan(v1);
    AI_PutIntToStack(v2);
    return a1 + 1;
}

tAINode* AI_EvalFunc_SNDGetFront(tAINode* a1)
{
    int v1;
    int v2;

    v1 = AI_GetIntFromStack();
    v2 = SND_l_GetFront(v1);
    AI_PutIntToStack(v2);
    return a1 + 1;
}

tAINode* AI_EvalFunc_SNDSetFreq(tAINode* arg0)
{
    int a2;
    int a1;

    a2 = AI_GetIntFromStack();
    a1 = AI_GetIntFromStack();
    QueryPerformanceCounter(&PerfCounterStruct[39]);
    SND_SetFreq(a1, a2);
    PRO_QPC_RecordTimeDecRef(39);
    return arg0 + 1;
}

tAINode* AI_EvalFunc_SNDGetFreq(tAINode* a1)
{
    int v1;
    int v2;

    v1 = AI_GetIntFromStack();
    v2 = SND_l_GetFreq(v1);
    AI_PutIntToStack(v2);
    return a1 + 1;
}

tAINode* AI_EvalFunc_SNDSetVol(tAINode* a1)
{
    int v1;
    float v3;

    v3 = AI_GetFloatFromStack();
    v1 = AI_GetIntFromStack();
    SND_SetVol(v1, v3);
    return a1 + 1;
}

tAINode* AI_EvalFunc_SNDGetVol(tAINode* a1)
{
    int v1;
    float v3;

    v1 = AI_GetIntFromStack();
    v3 = SND_f_GetVol(v1);
    AI_PutFloatToStack(v3);
    return a1 + 1;
}

void AI_EvalFunc_SNDUltraPlaySoundLooped_C(struct sGameObjectMain* a1, int a2)
{
    if (SND_gst_Params.soundEnabled && SND_gst_Params.soundInstanceNum > a2 && a2 >= 0 && !SND_i_IsPlaying(a2))
    {
        QueryPerformanceCounter(&PerfCounterStruct[39]);
        SND_i_CheckOrInitGAO(a1);
        SND_gst_Params.soundInstances[a2].gao = a1;
        SND_SetPos(a1, a2, a1->matrices);
        SND_PlaySoundLooping(a2);
        PRO_QPC_RecordTimeDecRef(39);
    }
}

tAINode* AI_EvalFunc_SNDUltraPlaySoundLooped(tAINode* a1)
{
    tGameObjectMain* v2;
    int a2;

    v2 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    a2 = AI_GetIntFromStack();
    AI_EvalFunc_SNDUltraPlaySoundLooped_C(v2, a2);
    return a1 + 1;
}

void AI_EvalFunc_SNDUltraPlaySound_C(struct sGameObjectMain* a1, int a2)
{
    if (SND_gst_Params.soundEnabled && SND_gst_Params.soundInstanceNum > a2 && a2 >= 0 && !SND_i_IsPlaying(a2))
    {
        QueryPerformanceCounter(&PerfCounterStruct[39]);
        SND_i_CheckOrInitGAO(a1);
        SND_gst_Params.soundInstances[a2].gao = a1;
        SND_SetPos(a1, a2, a1->matrices);
        SND_PlaySound(a2);
        PRO_QPC_RecordTimeDecRef(39);
    }
}

tAINode* AI_EvalFunc_SNDUltraPlaySound(tAINode* a1)
{
    tGameObjectMain* v2;
    int a2;

    v2 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    a2 = AI_GetIntFromStack();
    AI_EvalFunc_SNDUltraPlaySound_C(v2, a2);
    return a1 + 1;
}

void AI_EvalFunc_SNDPlaySoundOnTrack_C(struct sGameObjectMain* a1, int a2)
{
    if (SND_gst_Params.soundEnabled && SND_gst_Params.soundInstanceNum > a2 && a2 >= 0 && !SND_i_IsPlaying(a2))
    {
        QueryPerformanceCounter(&PerfCounterStruct[39]);
        SND_i_CheckOrInitGAO(a1);
        if (a1)
        {
            SND_gst_Params.soundInstances[a2].gao = a1;
            SND_SetPos(a1, a2, a1->matrices);
        }
        SND_PlaySoundOnTrack(a2);
        PRO_QPC_RecordTimeDecRef(39);
    }
}

tAINode* AI_EvalFunc_SNDPlaySoundOnTrack(tAINode* a1)
{
    tGameObjectMain* v2;
    int v3;

    v2 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v3 = AI_GetIntFromStack();
    AI_EvalFunc_SNDPlaySoundOnTrack_C(v2, v3);
    return a1 + 1;
}

void AI_EvalFunc_SNDPlaySoundLoopedOnTrack_C(tGameObjectMain* a1, int a2)
{
    if (SND_gst_Params.soundEnabled && SND_gst_Params.soundInstanceNum > a2 && a2 >= 0 && !SND_i_IsPlaying(a2))
    {
        QueryPerformanceCounter(&PerfCounterStruct[39]);
        SND_i_CheckOrInitGAO(a1);
        SND_gst_Params.soundInstances[a2].gao = a1;
        SND_SetPos(a1, a2, a1->matrices);
        SND_PlaySoundLoopingOnTrack(a2);
        PRO_QPC_RecordTimeDecRef(39);
    }
}

tAINode* AI_EvalFunc_SNDPlaySoundLoopedOnTrack(tAINode* a1)
{
    tGameObjectMain* v2;
    int a2;

    v2 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    a2 = AI_GetIntFromStack();
    AI_EvalFunc_SNDPlaySoundLoopedOnTrack_C(v2, a2);
    return a1 + 1;
}

tAINode* AI_EvalFunc_SNDSetDoppler(tAINode* a1)
{
    int v1;
    int v3;

    v3 = AI_GetIntFromStack();
    v1 = AI_GetIntFromStack();
    SND_SetDoppler(v1, v3);
    return a1 + 1;
}

tAINode* AI_EvalFunc_SNDEnableMute(tAINode* a1)
{
    int v1;
    int v3;

    v3 = AI_GetIntFromStack();
    v1 = AI_GetIntFromStack();
    SND_MuteEnable(v1, v3);
    return a1 + 1;
}

tAINode* AI_EvalFunc_SNDMuteIsEnable(tAINode* a1)
{
    int v1;
    int v2;

    v1 = AI_GetIntFromStack();
    v2 = SND_i_MuteIsEnable(v1);
    AI_PutIntToStack(v2);
    return a1 + 1;
}

tAINode* AI_EvalFunc_SNDSetUpMute(tAINode* a1)
{
    int v1;
    float a2;
    float a3;

    a3 = AI_GetFloatFromStack();
    a2 = AI_GetFloatFromStack();
    v1 = AI_GetIntFromStack();
    SND_MuteSetUp(v1, a2, a3);
    return a1 + 1;
}

tAINode* AI_EvalFunc_SNDGetMute(tAINode* a1)
{
    int v1;
    int v3;
    float v4;

    v3 = AI_GetIntFromStack();
    v1 = AI_GetIntFromStack();
    v4 = SND_f_MuteGet(v1, v3);
    AI_PutFloatToStack(v4);
    return a1 + 1;
}

tAINode* AI_EvalFunc_SNDStopSound(tAINode* a1)
{
    int v1;

    v1 = AI_GetIntFromStack();
    SND_StopRq(v1);
    return a1 + 1;
}

tAINode* AI_EvalFunc_SNDPauseSound(tAINode* a1)
{
    int v1;

    v1 = AI_GetIntFromStack();
    SND_Pause(v1);
    return a1 + 1;
}

tAINode* AI_EvalFunc_SNDIsPlaying(tAINode* a1)
{
    int v1;
    int v2;

    v1 = AI_GetIntFromStack();
    v2 = SND_i_IsPlaying(v1);
    AI_PutIntToStack(v2);
    return a1 + 1;
}

tAINode* AI_EvalFunc_SNDCurPosRatio(tAINode* a1)
{
    int v1;
    float v3;

    v1 = AI_GetIntFromStack();
    v3 = SND_f_CurrentPosRatio(v1);
    AI_PutFloatToStack(v3);
    return a1 + 1;
}

tAINode* AI_EvalFunc_SNDSetSndTrack(tAINode* a1)
{
    int v1;
    int v3;

    v3 = AI_GetIntFromStack();
    v1 = AI_GetIntFromStack();
    SND_SetTrack(v1, v3);
    return a1 + 1;
}

tAINode* AI_EvalFunc_SNDGetSndTrack(tAINode* a1)
{
    int v1;
    int v3;

    v3 = AI_GetIntFromStack();
    v1 = SND_i_GetTrack(v3);
    AI_PutIntToStack(v1);
    return a1 + 1;
}

tAINode* AI_EvalFunc_SNDFxSeti(tAINode* arg0)
{
    int v1;
    int a1;
    int a2;

    a2 = AI_GetIntFromStack();
    a1 = AI_GetIntFromStack();
    v1 = SND_i_FxSeti(a1, a2);
    AI_PutIntToStack(v1);
    return arg0 + 1;
}

tAINode* AI_EvalFunc_SNDSeti(tAINode* a1)
{
    int v1;
    int v3;
    int v4;

    v4 = AI_GetIntFromStack();
    v3 = AI_GetIntFromStack();
    v1 = SND_i_Seti(v3, v4);
    AI_PutIntToStack(v1);
    return a1 + 1;
}

tAINode* AI_EvalFunc_SNDFxSetf(tAINode* arg0)
{
    int v1;
    int a1;
    float a2;

    a2 = AI_GetFloatFromStack();
    a1 = AI_GetIntFromStack();
    v1 = SND_i_FxSetf(a1, a2);
    AI_PutIntToStack(v1);
    return arg0 + 1;
}

tAINode* AI_EvalFunc_SNDFxGeti(tAINode* a1)
{
    int v1;
    int v3;

    v3 = AI_GetIntFromStack();
    v1 = SND_i_FxGeti(v3);
    AI_PutIntToStack(v1);
    return a1 + 1;
}

tAINode* AI_EvalFunc_SNDGeti(tAINode* a1)
{
    int v1;
    int v3;

    v3 = AI_GetIntFromStack();
    v1 = SND_i_Geti(v3);
    AI_PutIntToStack(v1);
    return a1 + 1;
}

tAINode* AI_EvalFunc_SNDFxGetf(tAINode* a1)
{
    float v2;
    int v3;

    v3 = AI_GetIntFromStack();
    v2 = SND_f_FxGetf(v3);
    AI_PutFloatToStack(v2);
    return a1 + 1;
}

tAINode* AI_EvalFunc_SNDGetSoundFromInst(tAINode* a1)
{
    int v1;
    int v3;

    v3 = AI_GetIntFromStack();
    v1 = SND_l_GetSoundFromInstance(v3);
    AI_PutIntToStack(v1);
    return a1 + 1;
}

tAINode* AI_EvalFunc_SNDSetf(tAINode* a1)
{
    int v1;
    int v3;
    float a2;

    a2 = AI_GetFloatFromStack();
    v3 = AI_GetIntFromStack();
    v1 = SND_i_Setf(v3, a2);
    AI_PutIntToStack(v1);
    return a1 + 1;
}

tAINode* AI_EvalFunc_SNDGetf(tAINode* a1)
{
    float v2;
    int v3;

    v3 = AI_GetIntFromStack();
    v2 = SND_f_Getf(v3);
    AI_PutFloatToStack(v2);
    return a1 + 1;
}

int AI_EvalFunc_SNDGetInstType_C(int a1)
{
    unsigned int v2;
    int v3;

    if (!SND_gst_Params.soundEnabled)
        return -1;
    if (SND_gst_Params.soundInstanceNum <= a1)
        return -1;
    if (a1 < 0)
        return -1;
    if ((SND_gst_Params.soundInstances[a1].flags & 0x400) == 0)
        return -1;
    v2 = SND_gst_Params.soundInstances[a1].flags & 0x86008;
    if (v2 > 0x2000)
    {
        if (v2 == 0x4000)
        {
            v3 = 2;
        }
        else
        {
            if (v2 != 0x80000)
                return -1;
            v3 = 4;
        }
    }
    else if (v2 == 0x2000)
    {
        v3 = 3;
    }
    else if (v2)
    {
        if (v2 != 8)
            return -1;
        v3 = 5;
    }
    else
    {
        v3 = 1;
    }
    return v3;
}

tAINode* AI_EvalFunc_SNDGetInstType(tAINode* a1)
{
    int v1;
    int v3;

    v3 = AI_GetIntFromStack();
    v1 = AI_EvalFunc_SNDGetInstType_C(v3);
    AI_PutIntToStack(v1);
    return a1 + 1;
}

int AI_EvalFunc_SNDGetSndType_C(tGameObjectMain* a1, int a2)
{
    unsigned int v3;
    int v4;

    if (!SND_gst_Params.soundEnabled)
        return -1;
    if ((a1->flags & 0x8000000) == 0)
        return -1;
    if (!a1->extended)
        return -1;
    if (!a1->extended->snd)
        return -1;
    if (!a1->extended->snd->soundIndices)
        return -1;
    if (a2 < 0 || a2 >= a1->extended->snd->elemNum)
        return -1;
    if (a2 >= SND_gst_Params.allocatedSoundsMaxNum)
        return -1;
    v3 = SND_gst_Params.allocatedSounds[a1->extended->snd->soundIndices[a2]].flags & 0x86008;
    if (v3 > 0x2000)
    {
        if (v3 == 0x4000)
        {
            v4 = 2;
        }
        else
        {
            if (v3 != 0x80000)
                return -1;
            v4 = 4;
        }
    }
    else if (v3 == 0x2000)
    {
        v4 = 3;
    }
    else if (v3)
    {
        if (v3 != 8)
            return -1;
        v4 = 5;
    }
    else
    {
        v4 = 1;
    }
    return v4;
}

tAINode* AI_EvalFunc_SNDGetSndType(tAINode* a1)
{
    int v1;
    tGameObjectMain* v3;
    int a2;

    a2 = AI_GetIntFromStack();
    v3 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v1 = AI_EvalFunc_SNDGetSndType_C(v3, a2);
    AI_PutIntToStack(v1);
    return a1 + 1;
}

tAINode* AI_EvalFunc_SNDFlagSet(tAINode* a1)
{
    tGameObjectMain* v2;
    int a3;
    int idx;

    a3 = AI_GetIntFromStack();
    idx = AI_GetIntFromStack();
    v2 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    SND_SetFlags(v2, idx, a3);
    return a1 + 1;
}

tAINode* AI_EvalFunc_SNDFlagReset(tAINode* a1)
{
    tGameObjectMain* v2;
    int a3;
    int a2;

    a3 = AI_GetIntFromStack();
    a2 = AI_GetIntFromStack();
    v2 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    SND_ResetFlags(v2, a2, a3);
    return a1 + 1;
}

int AI_EvalFunc_SNDGetInstanceModifier_C(struct sGameObjectMain* a1, int a2)
{
    tSoundMdfStruct* v3;
    tModifier* i;

    if (a2 == -10)
        return -1;
    if (!SND_gst_Params.soundEnabled)
        return -1;
    if (!a1)
        return -1;
    if (!a1->extended)
        return -1;
    if (!a1->extended->mdf)
        return -1;
    if (a2 < 0)
        return -1;
    QueryPerformanceCounter(&PerfCounterStruct[39]);
    for (i = a1->extended->mdf; i; i = i->nextElem)
    {
        if (i->modifierIntf->mdfId == 8)
        {
            v3 = (tSoundMdfStruct*)i->mdfPrivateDataPtr;
            if (v3->modifierID == a2)
                return v3->sndInstanceID;
        }
    }
    PRO_QPC_RecordTimeDecRef(39);
    return -1;
}

tAINode* AI_EvalFunc_SNDGetInstanceModifier(tAINode* a1)
{
    int v1;
    tGameObjectMain* v3;
    int a2;

    a2 = AI_GetIntFromStack();
    v3 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v1 = AI_EvalFunc_SNDGetInstanceModifier_C(v3, a2);
    AI_PutIntToStack(v1);
    return a1 + 1;
}

void AI_EvalFunc_SNDPlayModifier_C(struct sGameObjectMain* a1, int a2)
{
    tSoundMdfStruct* a1a;
    tModifier* v3;

    if (SND_gst_Params.soundEnabled && a1 && a1->extended && a1->extended->mdf && a2 != -10)
    {
        QueryPerformanceCounter(&PerfCounterStruct[39]);
        v3 = a1->extended->mdf;
        if (a2 == -1)
        {
            while (v3)
            {
                if (v3->modifierIntf->mdfId == 8)
                    SND_PlayMdF((tSoundMdfStruct*)v3->mdfPrivateDataPtr);
                v3 = v3->nextElem;
            }
        }
        else
        {
            while (v3)
            {
                if (v3->modifierIntf->mdfId == 8)
                {
                    a1a = (tSoundMdfStruct*)v3->mdfPrivateDataPtr;
                    if (a1a->modifierID == a2)
                        SND_PlayMdF(a1a);
                }
                v3 = v3->nextElem;
            }
        }
        PRO_QPC_RecordTimeDecRef(39);
    }
}

tAINode* AI_EvalFunc_SNDPlayModifier(tAINode* a1)
{
    int v1;
    tGameObjectMain* v3;

    v1 = AI_GetIntFromStack();
    v3 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_EvalFunc_SNDPlayModifier_C(v3, v1);
    return a1 + 1;
}

void AI_EvalFunc_SNDPauseModifier_C(tGameObjectMain* a1, int a2)
{
    tSoundMdfStruct* v2;
    tModifier* v3;

    if (SND_gst_Params.soundEnabled && a1 && a1->extended && a1->extended->mdf && a2 != -10)
    {
        QueryPerformanceCounter(&PerfCounterStruct[39]);
        v3 = a1->extended->mdf;
        if (a2 == -1)
        {
            while (v3)
            {
                if (v3->modifierIntf->mdfId == 8)
                    SND_PauseMdF((tSoundMdfStruct*)(v3->mdfPrivateDataPtr));
                v3 = v3->nextElem;
            }
        }
        else
        {
            while (v3)
            {
                if (v3->modifierIntf->mdfId == 8)
                {
                    v2 = (tSoundMdfStruct*)v3->mdfPrivateDataPtr;
                    if (v2->modifierID == a2)
                        SND_PauseMdF(v2);
                }
                v3 = v3->nextElem;
            }
        }
        PRO_QPC_RecordTimeDecRef(39);
    }
}

tAINode* AI_EvalFunc_SNDPauseModifier(tAINode* a1)
{
    int v1;
    tGameObjectMain* v3;

    v1 = AI_GetIntFromStack();
    v3 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_EvalFunc_SNDPauseModifier_C(v3, v1);
    return a1 + 1;
}

void AI_EvalFunc_SNDStopModifier_C(struct sGameObjectMain* a1, int a2)
{
    tSoundMdfStruct* v2;
    tModifier* v3;

    if (SND_gst_Params.soundEnabled && a1 && a1->extended && a1->extended->mdf && a2 != -10)
    {
        QueryPerformanceCounter(&PerfCounterStruct[39]);
        v3 = a1->extended->mdf;
        if (a2 == -1)
        {
            while (v3)
            {
                if (v3->modifierIntf->mdfId == 8)
                    SND_StopMdF((tSoundMdfStruct*)v3->mdfPrivateDataPtr);
                v3 = v3->nextElem;
            }
        }
        else
        {
            while (v3)
            {
                if (v3->modifierIntf->mdfId == 8)
                {
                    v2 = (tSoundMdfStruct*)v3->mdfPrivateDataPtr;
                    if (v2->modifierID == a2)
                        SND_StopMdF(v2);
                }
                v3 = v3->nextElem;
            }
        }
        PRO_QPC_RecordTimeDecRef(39);
    }
}

tAINode* AI_EvalFunc_SNDStopModifier(tAINode* a1)
{
    int v1;
    tGameObjectMain* v3;

    v1 = AI_GetIntFromStack();
    v3 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_EvalFunc_SNDStopModifier_C(v3, v1);
    return a1 + 1;
}

int AI_EvalFunc_SNDGetStatusModifier_C(struct sGameObjectMain* a1, int a2)
{
    tSoundMdfStruct* v3;
    int v4;
    int v5;
    tModifier* v6;

    if (a2 == -10)
        return 0;
    if (!SND_gst_Params.soundEnabled)
        return 0;
    if (!a1)
        return 0;
    if (!a1->extended)
        return 0;
    if (!a1->extended->mdf)
        return 0;
    QueryPerformanceCounter(&PerfCounterStruct[39]);
    v6 = a1->extended->mdf;
    v5 = 0;
    v4 = 0;
    if (a2 == -1)
    {
        while (v6)
        {
            if (v6->modifierIntf->mdfId == 8)
            {
                v5 |= ((tSoundMdfStruct*)(v6->mdfPrivateDataPtr))->flags;
                v4 = 1;
            }
            v6 = v6->nextElem;
        }
        if (v4)
        {
            PRO_QPC_RecordTimeDecRef(39);
            return v5;
        }
    }
    else
    {
        while (v6)
        {
            if (v6->modifierIntf->mdfId == 8)
            {
                v3 = (tSoundMdfStruct*)(v6->mdfPrivateDataPtr);
                PRO_QPC_RecordTimeDecRef(39);
                if (v3->modifierID == a2)
                    return v3->flags;
            }
            v6 = v6->nextElem;
        }
    }
    PRO_QPC_RecordTimeDecRef(39);
    return 0;
}

tAINode* AI_EvalFunc_SNDGetStatusModifier(tAINode* a1)
{
    int v1;
    tGameObjectMain* v3;
    int a2;

    a2 = AI_GetIntFromStack();
    v3 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v1 = AI_EvalFunc_SNDGetStatusModifier_C(v3, a2);
    AI_PutIntToStack(v1);
    return a1 + 1;
}

void AI_EvalFunc_SNDSetFlagModifier_C(tGameObjectMain* a1, int a2, int a3)
{
    tModifier* i;
    int idx;

    if (a2 != -10 && SND_gst_Params.soundEnabled && a1 && a1->extended && a1->extended->mdf)
    {
        QueryPerformanceCounter(&PerfCounterStruct[39]);
        if (a2 == -1)
        {
            for (i = a1->extended->mdf; i; i = i->nextElem)
            {
                if (i->modifierIntf->mdfId == 8)
                    SND_SetFlags(0, ((tSoundMdfStruct*)(i->mdfPrivateDataPtr))->sndInstanceID, a3);
            }
        }
        else
        {
            idx = AI_EvalFunc_SNDGetInstanceModifier_C(a1, a2);
            SND_SetFlags(0, idx, a3);
        }
        PRO_QPC_RecordTimeDecRef(39);
    }
}

tAINode* AI_EvalFunc_SNDSetFlagModifier(tAINode* a1)
{
    tGameObjectMain* v2;
    int v3;
    int v4;

    v3 = AI_GetIntFromStack();
    v4 = AI_GetIntFromStack();
    v2 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_EvalFunc_SNDSetFlagModifier_C(v2, v4, v3);
    return a1 + 1;
}

void AI_EvalFunc_SNDResetFlagModifier_C(tGameObjectMain* a1, int arg4, int a3)
{
    tModifier* i;
    int a2; 

    if (arg4 != -10 && SND_gst_Params.soundEnabled && a1 && a1->extended && a1->extended->mdf)
    {
        QueryPerformanceCounter(&PerfCounterStruct[39]);
        if (arg4 == -1)
        {
            for (i = a1->extended->mdf; i; i = i->nextElem)
            {
                if (i->modifierIntf->mdfId == 8)
                    SND_ResetFlags(0, ((tSoundMdfStruct*)(i->mdfPrivateDataPtr))->sndInstanceID, a3);
            }
        }
        else
        {
            a2 = AI_EvalFunc_SNDGetInstanceModifier_C(a1, arg4);
            SND_ResetFlags(0, a2, a3);
        }
        PRO_QPC_RecordTimeDecRef(39);
    }
}

tAINode* AI_EvalFunc_SNDResetFlagModifier(tAINode* a1)
{
    tGameObjectMain* v2;
    int v3;
    int v4;

    v3 = AI_GetIntFromStack();
    v4 = AI_GetIntFromStack();
    v2 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_EvalFunc_SNDResetFlagModifier_C(v2, v4, v3);
    return a1 + 1;
}

tAINode* AI_EvalFunc_SNDFamillyPlay(tAINode* a1)
{
    int v1;
    int v3;

    v3 = AI_GetIntFromStack();
    v1 = AI_GetIntFromStack();
    SND_PlayFamilly(v1, v3);
    return a1 + 1;
}

tAINode* AI_EvalFunc_SNDFamillyStop(tAINode* a1)
{
    int v1;
    int v3;

    v3 = AI_GetIntFromStack();
    v1 = AI_GetIntFromStack();
    SND_StopFamilly(v1, v3);
    return a1 + 1;
}

tAINode* AI_EvalFunc_SNDFamillyMuteOn(tAINode* a1)
{
    int v1;
    int a2;

    a2 = AI_GetIntFromStack();
    v1 = AI_GetIntFromStack();
    SND_MuteOnFamilly(v1, a2);
    return a1 + 1;
}

tAINode* AI_EvalFunc_SNDFamillyMuteOff(tAINode* a1)
{
    int v1;
    int a2;

    a2 = AI_GetIntFromStack();
    v1 = AI_GetIntFromStack();
    SND_MuteOffFamilly(v1, a2);
    return a1 + 1;
}

tAINode* AI_EvalFunc_SNDFamillySetf(tAINode* a1)
{
    int a2;
    float a4;
    int v4;
    int a3;

    a4 = AI_GetFloatFromStack();
    a3 = AI_GetIntFromStack();
    a2 = AI_GetIntFromStack();
    v4 = AI_GetIntFromStack();
    SND_SetfFamilly(v4, a2, a3, a4);
    return a1 + 1;
}

tAINode* AI_EvalFunc_SNDFamillyGetf(tAINode* a1)
{
    float a1a;
    int a2;
    int v4;
    int a3;

    a3 = AI_GetIntFromStack();
    a2 = AI_GetIntFromStack();
    v4 = AI_GetIntFromStack();
    a1a = SND_f_GetfFamilly(v4, a2, a3);
    AI_PutFloatToStack(a1a);
    return a1 + 1;
}

tAINode* AI_EvalFunc_SNDFamillyGetStatus(tAINode* a1)
{
    int v1;
    int v3;
    int v4;

    v3 = AI_GetIntFromStack();
    v4 = AI_GetIntFromStack();
    v1 = SND_i_GetStatusFromFamilly(v4, v3);
    AI_PutIntToStack(v1);
    return a1 + 1;
}

tAINode* AI_EvalFunc_SNDFamillyFadeIn(tAINode* a1)
{
    int v1;
    int a2;

    a2 = AI_GetIntFromStack();
    v1 = AI_GetIntFromStack();
    SND_FadeInFamilly(v1, a2);
    return a1 + 1;
}

tAINode* AI_EvalFunc_SNDFamillyFadeOut(tAINode* a1)
{
    int v1;
    int v3;

    v3 = AI_GetIntFromStack();
    v1 = AI_GetIntFromStack();
    SND_FadeOutFamilly(v1, v3);
    return a1 + 1;
}

tAINode* AI_EvalFunc_SNDFamillyAdd(tAINode* arg0)
{
    int a2;
    int a1;

    a1 = AI_GetIntFromStack();
    a2 = AI_GetIntFromStack();
    SND_LinkToFamilly(a1, a2);
    return arg0 + 1;
}

tAINode* AI_EvalFunc_SNDFamillySub(tAINode* a1)
{
    int a2;
    int v3;

    v3 = AI_GetIntFromStack();
    a2 = AI_GetIntFromStack();
    SND_UnlinkFromFamilly(v3, a2);
    return a1 + 1;
}

tAINode* AI_EvalFunc_SNDIsTrackPlaying(tAINode* a1)
{
    int v1;
    int v3;

    v3 = AI_GetIntFromStack();
    v1 = SND_i_IsTrackPlaying(v3);
    AI_PutIntToStack(v1);
    return a1 + 1;
}

tAINode* AI_EvalFunc_SNDTrackSeti(tAINode* a1)
{
    int v1;
    int v3;
    int v4;
    int v5;

    v4 = AI_GetIntFromStack();
    v5 = AI_GetIntFromStack();
    v3 = AI_GetIntFromStack();
    v1 = SND_i_TrackSeti(v3, v5, v4);
    AI_PutIntToStack(v1);
    return a1 + 1;
}

tAINode* AI_EvalFunc_SNDTrackGeti(tAINode* a1)
{
    int v1;
    int v3;
    int v4;

    v4 = AI_GetIntFromStack();
    v3 = AI_GetIntFromStack();
    v1 = SND_i_TrackGeti(v3, v4);
    AI_PutIntToStack(v1);
    return a1 + 1;
}

tAINode* AI_EvalFunc_SNDTrackSetf(tAINode* a1)
{
    int v1;
    int v3;
    float v4;
    int v5;

    v4 = AI_GetFloatFromStack();
    v5 = AI_GetIntFromStack();
    v3 = AI_GetIntFromStack();
    v1 = SND_i_TrackSetf(v3, v5, v4);
    AI_PutIntToStack(v1);
    return a1 + 1;
}

tAINode* AI_EvalFunc_SNDTrackGetf(tAINode* a1)
{
    float v2;
    int v3;
    int v4;

    v4 = AI_GetIntFromStack();
    v3 = AI_GetIntFromStack();
    v2 = SND_f_TrackGetf(v3, v4);
    AI_PutFloatToStack(v2);
    return a1 + 1;
}


void AI_EvalFunc_SNDMicroPosSet_C(struct sGameObjectMain* a1)
{
    if (a1->matrices)
        SND_gpst_RqMicroPosFromAI = a1;
}

tAINode* AI_EvalFunc_SNDMicroPosSet(tAINode* a1)
{
    tGameObjectMain* v2;

    v2 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_EvalFunc_SNDMicroPosSet_C(v2);
    return a1 + 1;
}

void AI_EvalFunc_SNDFaderGroupSet_C(int a1, float a2)
{
    SND_MuteSetUp(a1, a2, 1.0f);
}

tAINode* AI_EvalFunc_SNDFaderGroupSet(tAINode* a1)
{
    int v1;
    float a2;

    a2 = AI_GetFloatFromStack();
    v1 = AI_GetIntFromStack();
    AI_EvalFunc_SNDFaderGroupSet_C(v1, a2);
    return a1 + 1;
}

float AI_EvalFunc_SNDFaderGroupGet_C(int a1)
{
    return SND_f_MuteGet(a1, 1);
}

tAINode* AI_EvalFunc_SNDFaderGroupGet(tAINode* a2)
{
    int v1;
    float a1a;

    v1 = AI_GetIntFromStack();
    a1a = AI_EvalFunc_SNDFaderGroupGet_C(v1);
    AI_PutFloatToStack(a1a);
    return a2 + 1;
}

tAINode* AI_EvalFunc_SNDBankMerge(tAINode* a1)
{
    tGameObjectMain* v2;

    v2 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_gul_SizeGlobalStack -= 32;
    --AI_gul_SizeGlobalVarStack;
    SND_MergeBankNoOver(v2, *(tGameObjectMain**)&AI_gac_GlobalStack[AI_gul_SizeGlobalStack]);
    return a1 + 1;
}

tAINode* AI_EvalFunc_SNDBankMergeOver(tAINode* a1)
{
    tGameObjectMain* v2;

    v2 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_gul_SizeGlobalStack -= 32;
    --AI_gul_SizeGlobalVarStack;
    SND_MergeBankOver(v2, *(tGameObjectMain**)&AI_gac_GlobalStack[AI_gul_SizeGlobalStack]);
    return a1 + 1;
}

tAINode* AI_EvalFunc_SNDBankRestore(tAINode* a1)
{
    tGameObjectMain* v2;

    v2 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    SND_RestoreBank(v2);
    return a1 + 1;
}

void AI_EvalFunc_SNDInstPosVSet_C(int a1, Vec3* a2)
{
    tSoundInstanceElem* v2;

    if (a1 >= 0 && a1 < SND_gst_Params.soundInstanceNum)
    {
        v2 = &SND_gst_Params.soundInstances[a1];
        if ((v2->flags & 0x400) != 0)
        {
            if (a2)
            {
                v2->sndInstPosVec = *a2;
                v2->sndPositionSetInSetPosPtr = &v2->sndInstPosVec;
                v2->sndPosMtxSetInSetPos = 0;
                v2->flags &= ~0x7000000u;
            }
        }
    }
}

tAINode* AI_EvalFunc_SNDInstPosVSet(tAINode* a1)
{
    int v1;
    Vec3* v3;

    AI_gul_SizeGlobalStack -= 32;
    --AI_gul_SizeGlobalVarStack;
    v3 = (Vec3*)&AI_gac_GlobalStack[AI_gul_SizeGlobalStack];
    v1 = AI_GetIntFromStack();
    AI_EvalFunc_SNDInstPosVSet_C(v1, v3);
    return a1 + 1;
}

void AI_EvalFunc_SNDPrefetchModifier_C(struct sGameObjectMain* a1, int a2)
{
    tSoundMdfStruct* a1a;
    tModifier* v3;

    if (a2 != -10 && SND_gst_Params.soundEnabled && a1->extended && a1->extended->mdf)
    {
        QueryPerformanceCounter(&PerfCounterStruct[39]);
        v3 = a1->extended->mdf;
        if (a2 == -1)
        {
            while (v3)
            {
                if (v3->modifierIntf->mdfId == 8)
                    SND_PrefetchMdF((tSoundMdfStruct*)v3->mdfPrivateDataPtr);
                v3 = v3->nextElem;
            }
        }
        else
        {
            while (v3)
            {
                if (v3->modifierIntf->mdfId == 8)
                {
                    a1a = (tSoundMdfStruct*)v3->mdfPrivateDataPtr;
                    if (a1a->modifierID == a2)
                        SND_PrefetchMdF(a1a);
                }
                v3 = v3->nextElem;
            }
        }
        PRO_QPC_RecordTimeDecRef(39);
    }
}

tAINode* AI_EvalFunc_SNDPrefetchModifier(tAINode* a1)
{
    int v1;
    tGameObjectMain* v3;

    v1 = AI_GetIntFromStack();
    v3 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_EvalFunc_SNDPrefetchModifier_C(v3, v1);
    return a1 + 1;
}

tAINode* AI_EvalFunc_SNDPrefetch(tAINode* a1)
{
    int v2;

    v2 = AI_GetIntFromStack();
    SND_StreamPrefetch(v2);
    return a1 + 1;
}

void AI_EvalFunc_SNDFlushModifier_C(tGameObjectMain* a1, int a2)
{
    tSoundMdfStruct* v2;
    tModifier* v3;

    if (a2 != -10 && SND_gst_Params.soundEnabled && a1->extended && a1->extended->mdf)
    {
        QueryPerformanceCounter(&PerfCounterStruct[39]);
        v3 = a1->extended->mdf;
        if (a2 == -1)
        {
            while (v3)
            {
                if (v3->modifierIntf->mdfId == 8)
                    SND_FlushMdF((tSoundMdfStruct*)v3->mdfPrivateDataPtr);
                v3 = v3->nextElem;
            }
        }
        else
        {
            while (v3)
            {
                if (v3->modifierIntf->mdfId == 8)
                {
                    v2 = (tSoundMdfStruct*)v3->mdfPrivateDataPtr;
                    if (v2->modifierID == a2)
                        SND_FlushMdF(v2);
                }
                v3 = v3->nextElem;
            }
        }
        PRO_QPC_RecordTimeDecRef(39);
    }
}

tAINode* AI_EvalFunc_SNDFlushModifier(tAINode* a1)
{
    int v1;
    tGameObjectMain* v3;

    v1 = AI_GetIntFromStack();
    v3 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_EvalFunc_SNDFlushModifier_C(v3, v1);
    return a1 + 1;
}

tAINode* AI_EvalFunc_SNDFlush(tAINode* a1)
{
    int instanceIdx;

    instanceIdx = AI_GetIntFromStack();
    SND_StreamFlush(instanceIdx);
    return a1 + 1;
}

tAINode* AI_EvalFunc_SNDInstanceSeti(tAINode* a1)
{
    int v1;
    int i2;
    int i3;
    int i1;

    i3 = AI_GetIntFromStack();
    i2 = AI_GetIntFromStack();
    i1 = AI_GetIntFromStack();
    v1 = SND_i_InstanceSeti_return_negative_1(i1, i2, i3);
    AI_PutIntToStack(v1);
    return a1 + 1;
}

tAINode* AI_EvalFunc_SNDInstanceGeti(tAINode* a1)
{
    int v1;
    int v3;
    int v4;

    v3 = AI_GetIntFromStack();
    v4 = AI_GetIntFromStack();
    v1 = SND_i_InstanceGeti(v4, v3);
    AI_PutIntToStack(v1);
    return a1 + 1;
}

tAINode* AI_EvalFunc_SNDInstanceSetf(tAINode* a1)
{
    int v1;
    int a2;
    float a3;
    int v5;

    a3 = AI_GetFloatFromStack();
    a2 = AI_GetIntFromStack();
    v5 = AI_GetIntFromStack();
    v1 = SND_i_InstanceSetf(v5, a2, a3);
    AI_PutIntToStack(v1);
    return a1 + 1;
}

tAINode* AI_EvalFunc_SNDInstanceGetf(tAINode* a1)
{
    float v2;
    int v3;
    int v4;

    v3 = AI_GetIntFromStack();
    v4 = AI_GetIntFromStack();
    v2 = SND_f_InstanceGetf(v4, v3);
    AI_PutFloatToStack(v2);
    return a1 + 1;
}

int AI_EvalFunc_SNDRequestPlayRandomFreq_C(struct sGameObjectMain* gao, int a2, float a3, float a4, float a5)
{
    double v6;
    float v7;
    float v8;
    tSoundInstanceElem* v9;
    int a1;
    double tmp;


    a1 = AI_EvalFunc_SNDRequestPlay_C(gao, a2, a3);
    if (a1 < 0 || a1 >= SND_gst_Params.soundInstanceNum)
        return -1;
    v9 = &SND_gst_Params.soundInstances[a1];
    if ((v9->flags & 0x400) == 0)
        return -1;
    v6 = AI_EvalFunc_MATHRandFloat_C(a4, a5);
    v7 = (float)v9->freq;
    v8 = (float)(v6 * v7);
    tmp = v8 + MATH_gd_magicDoubleForRound;
    SND_SetFreq(a1, *(uint32_t *)&tmp);
    return a1;
}

tAINode* AI_EvalFunc_SNDRequestPlayRandomFreq(tAINode* a1)
{
    int v1;
    tGameObjectMain* v3;
    float a5;
    int a2;
    float a4;
    float a3;

    a5 = AI_GetFloatFromStack();
    a4 = AI_GetFloatFromStack();
    a3 = AI_GetFloatFromStack();
    a2 = AI_GetIntFromStack();
    v3 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v1 = AI_EvalFunc_SNDRequestPlayRandomFreq_C(v3, a2, a3, a4, a5);
    AI_PutIntToStack(v1);
    return a1 + 1;
}

int AI_EvalFunc_SND_SetObject_C(struct sGameObjectMain* a1, int a2)
{
    tSoundInstanceElem* v3;

    if (a2 < 0 || a2 >= SND_gst_Params.soundInstanceNum)
        return -1;
    v3 = &SND_gst_Params.soundInstances[a2];
    if ((v3->flags & 0x400) == 0)
        return -1;
    if (SND_i_CheckOrInitGAO(a1))
        return -1;
    v3->gao = a1;
    SND_SetPos(a1, a2, a1->matrices);
    return 0;
}

tAINode* AI_EvalFunc_SND_SetObject(tAINode* a1)
{
    int v1;
    int v2;
    tGameObjectMain* v4;

    v4 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    v1 = AI_GetIntFromStack();
    v2 = AI_EvalFunc_SND_SetObject_C(v4, v1);
    AI_PutIntToStack(v2);
    return a1 + 1;
}

tAINode* AI_EvalFunc_SND_Fade(tAINode* a1)
{
    float v2;
    float v3;
    float v4;
    int v5;

    v2 = AI_GetFloatFromStack();
    v4 = AI_GetFloatFromStack();
    v3 = AI_GetFloatFromStack();
    v5 = AI_GetIntFromStack();
    SND_Fade(v5, v3, v4, v2);
    return a1 + 1;
}

void AI_EvalFunc_SNDPlaySoundOnTrackFromTo_C(tGameObjectMain* a1, int a2, float a3, float a4)
{
    if (SND_gst_Params.soundEnabled && SND_gst_Params.soundInstanceNum > a2 && a2 >= 0 && !SND_i_IsPlaying(a2))
    {
        QueryPerformanceCounter(&PerfCounterStruct[39]);
        SND_i_CheckOrInitGAO(a1);
        if (a1)
        {
            SND_gst_Params.soundInstances[a2].gao = a1;
            SND_SetPos(a1, a2, a1->matrices);
        }
        SND_PlaySoundOnTrackFromTo(a2, a3, a4);
        PRO_QPC_RecordTimeDecRef(39);
    }
}

void AI_EvalFunc_SNDPlaySoundLoopedOnTrackFrom_C(tGameObjectMain* a1, int a2, float a3)
{
    if (SND_gst_Params.soundEnabled && SND_gst_Params.soundInstanceNum > a2 && a2 >= 0 && !SND_i_IsPlaying(a2))
    {
        QueryPerformanceCounter(&PerfCounterStruct[39]);
        SND_i_CheckOrInitGAO(a1);
        SND_gst_Params.soundInstances[a2].gao = a1;
        SND_SetPos(a1, a2, a1->matrices);
        SND_PlaySoundLoopingOnTrackFrom(a2, a3);
        PRO_QPC_RecordTimeDecRef(39);
    }
}

int AI_EvalFunc_SND_ExtPlay_C(struct sGameObjectMain* a1, int a2, int a3, float a4, float a5)
{
    int result; 

    switch (a3 & 0xF)
    {
    case 0:
    case 4:
    case 8:
    case 12:
        AI_EvalFunc_SNDUltraPlaySound_C(a1, a2);
        goto LABEL_11;
    case 1:
    case 5:
    case 9:
    case 13:
        AI_EvalFunc_SNDUltraPlaySoundLooped_C(a1, a2);
        goto LABEL_11;
    case 2:
        AI_EvalFunc_SNDPlaySoundOnTrack_C(a1, a2);
        goto LABEL_11;
    case 3:
    case 11:
        AI_EvalFunc_SNDPlaySoundLoopedOnTrack_C(a1, a2);
        goto LABEL_11;
    case 6:
        AI_EvalFunc_SNDPlaySoundOnTrackFromTo_C(a1, a2, a4, -1.0);
        goto LABEL_11;
    case 7:
    case 15:
        AI_EvalFunc_SNDPlaySoundLoopedOnTrackFrom_C(a1, a2, a4);
        goto LABEL_11;
    case 10:
        AI_EvalFunc_SNDPlaySoundOnTrackFromTo_C(a1, a2, 0.0, a5);
        goto LABEL_11;
    case 14:
        AI_EvalFunc_SNDPlaySoundOnTrackFromTo_C(a1, a2, a4, a5);
    LABEL_11:
        result = 0;
        break;
    default:
        result = -1;
        break;
    }
    return result;
}

tAINode* AI_EvalFunc_SND_ExtPlay(tAINode* a1)
{
    int v1;
    tGameObjectMain* v3;
    float a4;
    int a3;
    int a2;
    float a5;

    v3 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    a5 = AI_GetFloatFromStack();
    a4 = AI_GetFloatFromStack();
    a3 = AI_GetIntFromStack();
    a2 = AI_GetIntFromStack();
    v1 = AI_EvalFunc_SND_ExtPlay_C(v3, a2, a3, a4, a5);
    AI_PutIntToStack(v1);
    return a1 + 1;
}

tAINode* AI_EvalFunc_SND_SoundDurationGet(tAINode* a1)
{
    float v2;
    tGameObjectMain* v3;
    int sndIdx;

    v3 = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    sndIdx = AI_GetIntFromStack();
    v2 = SND_SoundDurationGet(v3, sndIdx);
    AI_PutFloatToStack(v2);
    return a1 + 1;
}