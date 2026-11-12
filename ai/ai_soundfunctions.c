#include "../sound.h"
#include "../win/winsound.h"
#include "../ai.h"
#include "ai_mathfunctions.h"
#include "ai_soundfunctions.h"

// Six builtins start a sound instance, and all six check the same four
// things first. The order matters: a negative index passes the count
// test and is caught by the next one, so SND_i_IsPlaying only ever sees
// a valid index.
static BOOL AI_l_CanStartInstance(int instanceIdx)
{
    return SND_gst_Params.soundEnabled
           && SND_gst_Params.soundInstanceNum > instanceIdx
           && instanceIdx >= 0
           && !SND_i_IsPlaying(instanceIdx);
}

// Hands the instance to a game object, so the sound follows it.
static void AI_l_AttachInstanceToGAO(struct sGameObjectMain* gao, int instanceIdx)
{
    SND_gst_Params.soundInstances[instanceIdx].gao = gao;
    SND_SetPos(gao, instanceIdx, gao->matrices);
}

// The kind of a sound is a single bit of its flag word, and no bit at
// all means a plain sample. Both the instance and the bank accessor
// decode it this way.
static int AI_l_SoundTypeFromFlags(unsigned int flags)
{
    switch (flags & 0x86008)
    {
    case 0:
        return 1;
    case 8:
        return 5;
    case 0x2000:
        return 3;
    case 0x4000:
        return 2;
    case 0x80000:
        return 4;
    default:
        return -1;
    }
}

// Five builtins walk an object's sound modifiers and do one thing to
// each, where a modifier id of -1 means every one of them. The caller
// has already checked that the object has a modifier chain.
static void AI_l_ForEachSoundModifier(struct sGameObjectMain* gao, int modifierID,
                                      void (*apply)(tSoundMdfStruct*))
{
    tSoundMdfStruct* sound;
    tModifier* mdf;

    QueryPerformanceCounter(&PerfCounterStruct[39]);
    for (mdf = gao->extended->mdf; mdf; mdf = mdf->nextElem)
    {
        if (mdf->modifierIntf->mdfId != 8)
            continue;

        sound = (tSoundMdfStruct*)mdf->mdfPrivateDataPtr;
        if (modifierID == -1 || sound->modifierID == modifierID)
            apply(sound);
    }
    PRO_QPC_RecordTimeDecRef(39);
}

tAINode* AI_EvalFunc_SNDSetGlobalPan(tAINode* node)
{
    int pan;

    pan = AI_GetIntFromStack();
    SND_SetGlobalPan(pan);
    return node + 1;
}

tAINode* AI_EvalFunc_SNDGetGlobalPan(tAINode* node)
{
    AI_PutIntToStack(SND_l_GetGlobalPan());
    return node + 1;
}

tAINode* AI_EvalFunc_SNDSetGlobalVol(tAINode* node)
{
    float volume;

    volume = AI_GetFloatFromStack();
    SND_SetGlobalVol(volume);
    return node + 1;
}

tAINode* AI_EvalFunc_SNDGetGlobalVol(tAINode* node)
{
    AI_PutFloatToStack(SND_f_GetGlobalVol());
    return node + 1;
}

tAINode* AI_EvalFunc_SNDGetSound(tAINode* node)
{
    int soundID;

    soundID = AI_GetIntFromStack();
    AI_PutIntToStack(SND_l_GetSoundIndex(soundID));
    return node + 1;
}

// Takes an instance for a sound and positions it on the object. Bit
// 0x800 of the flags asks for it to start playing straight away.
int AI_EvalFunc_SNDRequest_C(struct sGameObjectMain* gao, int soundID, int flags)
{
    Mtx4x4* matrices;
    int instanceIdx;

    if (!SND_gst_Params.soundEnabled)
        return -1;

    QueryPerformanceCounter(&PerfCounterStruct[39]);
    matrices = gao->matrices;
    instanceIdx = SND_l_Request(gao, soundID);
    if (instanceIdx != -1)
    {
        SND_SetFlags(gao, instanceIdx, flags);
        SND_SetPos(gao, instanceIdx, matrices);
        if ((flags & 0x800) != 0)
            SND_PlaySound(instanceIdx);
    }
    PRO_QPC_RecordTimeDecRef(39);
    return instanceIdx;
}

tAINode* AI_EvalFunc_SNDRequest(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    int soundID;
    int flags;

    flags = AI_GetIntFromStack();
    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    soundID = AI_GetIntFromStack();
    AI_PutIntToStack(AI_EvalFunc_SNDRequest_C(gaoUltra, soundID, flags));
    return node + 1;
}

// A volume of -1 leaves the instance's own volume alone.
int AI_EvalFunc_SNDRequestPlay_C(struct sGameObjectMain* gao, int soundID, float volume)
{
    int instanceIdx;

    if (!SND_gst_Params.soundEnabled)
        return -1;

    instanceIdx = AI_EvalFunc_SNDRequest_C(gao, soundID, 0x18000080);
    QueryPerformanceCounter(&PerfCounterStruct[39]);
    if (volume != -1.0)
        SND_SetVol(instanceIdx, volume);
    SND_PlaySound(instanceIdx);
    PRO_QPC_RecordTimeDecRef(39);
    return instanceIdx;
}

int AI_EvalFunc_SNDRequestPlay_C_CURRENT(int soundID, float volume)
{
    return AI_EvalFunc_SNDRequestPlay_C(AI_gpst_CurrentGameObject, soundID, volume);
}

tAINode* AI_EvalFunc_SNDRequestPlay(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    int soundID;
    float volume;

    volume = AI_GetFloatFromStack();
    soundID = AI_GetIntFromStack();
    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_PutIntToStack(AI_EvalFunc_SNDRequestPlay_C(gaoUltra, soundID, volume));
    return node + 1;
}

int AI_EvalFunc_SNDRequestOnTrack_C(tGameObjectMain* gao, int soundID, int flags)
{
    Mtx4x4* matrices;
    int instanceIdx;

    if (!SND_gst_Params.soundEnabled)
        return -1;

    QueryPerformanceCounter(&PerfCounterStruct[39]);
    matrices = gao->matrices;
    instanceIdx = SND_l_Request(gao, soundID);
    if (instanceIdx != -1)
    {
        SND_SetFlags(gao, instanceIdx, flags);
        SND_SetPos(gao, instanceIdx, matrices);
        if ((flags & 0x800) != 0)
            SND_PlaySoundOnTrack(instanceIdx);
    }
    PRO_QPC_RecordTimeDecRef(39);
    return instanceIdx;
}

tAINode* AI_EvalFunc_SNDRequestOnTrack(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    int soundID;
    int flags;

    flags = AI_GetIntFromStack();
    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    soundID = AI_GetIntFromStack();
    AI_PutIntToStack(AI_EvalFunc_SNDRequestOnTrack_C(gaoUltra, soundID, flags));
    return node + 1;
}

// Unlike SNDRequestPlay, this one sets the volume whatever it is.
int AI_EvalFunc_SNDRequestPlayOnTrack_C(tGameObjectMain* gao, int soundID, float volume)
{
    int instanceIdx;

    if (!SND_gst_Params.soundEnabled)
        return -1;

    instanceIdx = AI_EvalFunc_SNDRequestOnTrack_C(gao, soundID, 0x18000080);
    QueryPerformanceCounter(&PerfCounterStruct[39]);
    SND_SetVol(instanceIdx, volume);
    SND_PlaySoundOnTrack(instanceIdx);
    PRO_QPC_RecordTimeDecRef(39);
    return instanceIdx;
}

tAINode* AI_EvalFunc_SNDRequestPlayOnTrack(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    int soundID;
    float volume;

    volume = AI_GetFloatFromStack();
    soundID = AI_GetIntFromStack();
    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_PutIntToStack(AI_EvalFunc_SNDRequestPlayOnTrack_C(gaoUltra, soundID, volume));
    return node + 1;
}

tAINode* AI_EvalFunc_SNDDestroy(tAINode* node)
{
    int instanceIdx;

    instanceIdx = AI_GetIntFromStack();
    SND_Release(instanceIdx);
    return node + 1;
}

void SND_SetNearFarS(int instanceIdx, float nearDist, float farDist)
{
    QueryPerformanceCounter(&PerfCounterStruct[39]);
    SND_SetNearFar(instanceIdx, 4096, nearDist, farDist);
    PRO_QPC_RecordTimeDecRef(39);
}

tAINode* AI_EvalFunc_SNDSetNearFar(tAINode* node)
{
    int instanceIdx;
    float nearDist;
    float farDist;

    farDist = AI_GetFloatFromStack();
    nearDist = AI_GetFloatFromStack();
    instanceIdx = AI_GetIntFromStack();
    SND_SetNearFarS(instanceIdx, nearDist, farDist);
    return node + 1;
}

tAINode* AI_EvalFunc_SNDSetPan(tAINode* node)
{
    int instanceIdx;
    int pan;
    int front;

    front = AI_GetIntFromStack();
    pan = AI_GetIntFromStack();
    instanceIdx = AI_GetIntFromStack();
    SND_SetPan(instanceIdx, pan, front);
    return node + 1;
}

tAINode* AI_EvalFunc_SNDGetPan(tAINode* node)
{
    int instanceIdx;

    instanceIdx = AI_GetIntFromStack();
    AI_PutIntToStack(SND_l_GetPan(instanceIdx));
    return node + 1;
}

tAINode* AI_EvalFunc_SNDGetFront(tAINode* node)
{
    int instanceIdx;

    instanceIdx = AI_GetIntFromStack();
    AI_PutIntToStack(SND_l_GetFront(instanceIdx));
    return node + 1;
}

tAINode* AI_EvalFunc_SNDSetFreq(tAINode* node)
{
    int instanceIdx;
    int freq;

    freq = AI_GetIntFromStack();
    instanceIdx = AI_GetIntFromStack();
    QueryPerformanceCounter(&PerfCounterStruct[39]);
    SND_SetFreq(instanceIdx, freq);
    PRO_QPC_RecordTimeDecRef(39);
    return node + 1;
}

tAINode* AI_EvalFunc_SNDGetFreq(tAINode* node)
{
    int instanceIdx;

    instanceIdx = AI_GetIntFromStack();
    AI_PutIntToStack(SND_l_GetFreq(instanceIdx));
    return node + 1;
}

tAINode* AI_EvalFunc_SNDSetVol(tAINode* node)
{
    int instanceIdx;
    float volume;

    volume = AI_GetFloatFromStack();
    instanceIdx = AI_GetIntFromStack();
    SND_SetVol(instanceIdx, volume);
    return node + 1;
}

tAINode* AI_EvalFunc_SNDGetVol(tAINode* node)
{
    int instanceIdx;

    instanceIdx = AI_GetIntFromStack();
    AI_PutFloatToStack(SND_f_GetVol(instanceIdx));
    return node + 1;
}

void AI_EvalFunc_SNDUltraPlaySoundLooped_C(struct sGameObjectMain* gao, int instanceIdx)
{
    if (!AI_l_CanStartInstance(instanceIdx))
        return;

    QueryPerformanceCounter(&PerfCounterStruct[39]);
    SND_i_CheckOrInitGAO(gao);
    AI_l_AttachInstanceToGAO(gao, instanceIdx);
    SND_PlaySoundLooping(instanceIdx);
    PRO_QPC_RecordTimeDecRef(39);
}

tAINode* AI_EvalFunc_SNDUltraPlaySoundLooped(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    int instanceIdx;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    instanceIdx = AI_GetIntFromStack();
    AI_EvalFunc_SNDUltraPlaySoundLooped_C(gaoUltra, instanceIdx);
    return node + 1;
}

void AI_EvalFunc_SNDUltraPlaySound_C(struct sGameObjectMain* gao, int instanceIdx)
{
    if (!AI_l_CanStartInstance(instanceIdx))
        return;

    QueryPerformanceCounter(&PerfCounterStruct[39]);
    SND_i_CheckOrInitGAO(gao);
    AI_l_AttachInstanceToGAO(gao, instanceIdx);
    SND_PlaySound(instanceIdx);
    PRO_QPC_RecordTimeDecRef(39);
}

tAINode* AI_EvalFunc_SNDUltraPlaySound(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    int instanceIdx;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    instanceIdx = AI_GetIntFromStack();
    AI_EvalFunc_SNDUltraPlaySound_C(gaoUltra, instanceIdx);
    return node + 1;
}

// This one and SNDPlaySoundOnTrackFromTo guard the attach with a null
// check that their four siblings do not have.
void AI_EvalFunc_SNDPlaySoundOnTrack_C(struct sGameObjectMain* gao, int instanceIdx)
{
    if (!AI_l_CanStartInstance(instanceIdx))
        return;

    QueryPerformanceCounter(&PerfCounterStruct[39]);
    SND_i_CheckOrInitGAO(gao);
    if (gao)
        AI_l_AttachInstanceToGAO(gao, instanceIdx);
    SND_PlaySoundOnTrack(instanceIdx);
    PRO_QPC_RecordTimeDecRef(39);
}

tAINode* AI_EvalFunc_SNDPlaySoundOnTrack(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    int instanceIdx;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    instanceIdx = AI_GetIntFromStack();
    AI_EvalFunc_SNDPlaySoundOnTrack_C(gaoUltra, instanceIdx);
    return node + 1;
}

void AI_EvalFunc_SNDPlaySoundLoopedOnTrack_C(tGameObjectMain* gao, int instanceIdx)
{
    if (!AI_l_CanStartInstance(instanceIdx))
        return;

    QueryPerformanceCounter(&PerfCounterStruct[39]);
    SND_i_CheckOrInitGAO(gao);
    AI_l_AttachInstanceToGAO(gao, instanceIdx);
    SND_PlaySoundLoopingOnTrack(instanceIdx);
    PRO_QPC_RecordTimeDecRef(39);
}

tAINode* AI_EvalFunc_SNDPlaySoundLoopedOnTrack(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    int instanceIdx;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    instanceIdx = AI_GetIntFromStack();
    AI_EvalFunc_SNDPlaySoundLoopedOnTrack_C(gaoUltra, instanceIdx);
    return node + 1;
}

tAINode* AI_EvalFunc_SNDSetDoppler(tAINode* node)
{
    int instanceIdx;
    int enable;

    enable = AI_GetIntFromStack();
    instanceIdx = AI_GetIntFromStack();
    SND_SetDoppler(instanceIdx, enable);
    return node + 1;
}

tAINode* AI_EvalFunc_SNDEnableMute(tAINode* node)
{
    int group;
    int enable;

    enable = AI_GetIntFromStack();
    group = AI_GetIntFromStack();
    SND_MuteEnable(group, enable);
    return node + 1;
}

tAINode* AI_EvalFunc_SNDMuteIsEnable(tAINode* node)
{
    int group;

    group = AI_GetIntFromStack();
    AI_PutIntToStack(SND_i_MuteIsEnable(group));
    return node + 1;
}

tAINode* AI_EvalFunc_SNDSetUpMute(tAINode* node)
{
    int group;
    float target;
    float duration;

    duration = AI_GetFloatFromStack();
    target = AI_GetFloatFromStack();
    group = AI_GetIntFromStack();
    SND_MuteSetUp(group, target, duration);
    return node + 1;
}

tAINode* AI_EvalFunc_SNDGetMute(tAINode* node)
{
    int group;
    int which;

    which = AI_GetIntFromStack();
    group = AI_GetIntFromStack();
    AI_PutFloatToStack(SND_f_MuteGet(group, which));
    return node + 1;
}

tAINode* AI_EvalFunc_SNDStopSound(tAINode* node)
{
    int instanceIdx;

    instanceIdx = AI_GetIntFromStack();
    SND_StopRq(instanceIdx);
    return node + 1;
}

tAINode* AI_EvalFunc_SNDPauseSound(tAINode* node)
{
    int instanceIdx;

    instanceIdx = AI_GetIntFromStack();
    SND_Pause(instanceIdx);
    return node + 1;
}

tAINode* AI_EvalFunc_SNDIsPlaying(tAINode* node)
{
    int instanceIdx;

    instanceIdx = AI_GetIntFromStack();
    AI_PutIntToStack(SND_i_IsPlaying(instanceIdx));
    return node + 1;
}

tAINode* AI_EvalFunc_SNDCurPosRatio(tAINode* node)
{
    int instanceIdx;

    instanceIdx = AI_GetIntFromStack();
    AI_PutFloatToStack(SND_f_CurrentPosRatio(instanceIdx));
    return node + 1;
}

tAINode* AI_EvalFunc_SNDSetSndTrack(tAINode* node)
{
    int instanceIdx;
    int trackID;

    trackID = AI_GetIntFromStack();
    instanceIdx = AI_GetIntFromStack();
    SND_SetTrack(instanceIdx, trackID);
    return node + 1;
}

tAINode* AI_EvalFunc_SNDGetSndTrack(tAINode* node)
{
    int instanceIdx;

    instanceIdx = AI_GetIntFromStack();
    AI_PutIntToStack(SND_i_GetTrack(instanceIdx));
    return node + 1;
}

tAINode* AI_EvalFunc_SNDFxSeti(tAINode* node)
{
    int field;
    int value;

    value = AI_GetIntFromStack();
    field = AI_GetIntFromStack();
    AI_PutIntToStack(SND_i_FxSeti(field, value));
    return node + 1;
}

tAINode* AI_EvalFunc_SNDSeti(tAINode* node)
{
    int field;
    int value;

    value = AI_GetIntFromStack();
    field = AI_GetIntFromStack();
    AI_PutIntToStack(SND_i_Seti(field, value));
    return node + 1;
}

tAINode* AI_EvalFunc_SNDFxSetf(tAINode* node)
{
    int field;
    float value;

    value = AI_GetFloatFromStack();
    field = AI_GetIntFromStack();
    AI_PutIntToStack(SND_i_FxSetf(field, value));
    return node + 1;
}

tAINode* AI_EvalFunc_SNDFxGeti(tAINode* node)
{
    int field;

    field = AI_GetIntFromStack();
    AI_PutIntToStack(SND_i_FxGeti(field));
    return node + 1;
}

tAINode* AI_EvalFunc_SNDGeti(tAINode* node)
{
    int field;

    field = AI_GetIntFromStack();
    AI_PutIntToStack(SND_i_Geti(field));
    return node + 1;
}

tAINode* AI_EvalFunc_SNDFxGetf(tAINode* node)
{
    int field;

    field = AI_GetIntFromStack();
    AI_PutFloatToStack(SND_f_FxGetf(field));
    return node + 1;
}

tAINode* AI_EvalFunc_SNDGetSoundFromInst(tAINode* node)
{
    int instanceIdx;

    instanceIdx = AI_GetIntFromStack();
    AI_PutIntToStack(SND_l_GetSoundFromInstance(instanceIdx));
    return node + 1;
}

tAINode* AI_EvalFunc_SNDSetf(tAINode* node)
{
    int field;
    float value;

    value = AI_GetFloatFromStack();
    field = AI_GetIntFromStack();
    AI_PutIntToStack(SND_i_Setf(field, value));
    return node + 1;
}

tAINode* AI_EvalFunc_SNDGetf(tAINode* node)
{
    int field;

    field = AI_GetIntFromStack();
    AI_PutFloatToStack(SND_f_Getf(field));
    return node + 1;
}

int AI_EvalFunc_SNDGetInstType_C(int instanceIdx)
{
    if (!SND_gst_Params.soundEnabled)
        return -1;
    if (SND_gst_Params.soundInstanceNum <= instanceIdx)
        return -1;
    if (instanceIdx < 0)
        return -1;
    if ((SND_gst_Params.soundInstances[instanceIdx].flags & 0x400) == 0)
        return -1;

    return AI_l_SoundTypeFromFlags(SND_gst_Params.soundInstances[instanceIdx].flags);
}

tAINode* AI_EvalFunc_SNDGetInstType(tAINode* node)
{
    int instanceIdx;

    instanceIdx = AI_GetIntFromStack();
    AI_PutIntToStack(AI_EvalFunc_SNDGetInstType_C(instanceIdx));
    return node + 1;
}

// The same decode, for a sound of the object's own bank rather than a
// live instance.
int AI_EvalFunc_SNDGetSndType_C(tGameObjectMain* gao, int bankIdx)
{
    if (!SND_gst_Params.soundEnabled)
        return -1;
    if ((gao->flags & 0x8000000) == 0)
        return -1;
    if (!gao->extended)
        return -1;
    if (!gao->extended->snd)
        return -1;
    if (!gao->extended->snd->soundIndices)
        return -1;
    if (bankIdx < 0 || bankIdx >= gao->extended->snd->elemNum)
        return -1;
    if (bankIdx >= SND_gst_Params.allocatedSoundsMaxNum)
        return -1;

    return AI_l_SoundTypeFromFlags(
        SND_gst_Params.allocatedSounds[gao->extended->snd->soundIndices[bankIdx]].flags);
}

tAINode* AI_EvalFunc_SNDGetSndType(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    int bankIdx;

    bankIdx = AI_GetIntFromStack();
    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_PutIntToStack(AI_EvalFunc_SNDGetSndType_C(gaoUltra, bankIdx));
    return node + 1;
}

tAINode* AI_EvalFunc_SNDFlagSet(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    int instanceIdx;
    int flags;

    flags = AI_GetIntFromStack();
    instanceIdx = AI_GetIntFromStack();
    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    SND_SetFlags(gaoUltra, instanceIdx, flags);
    return node + 1;
}

tAINode* AI_EvalFunc_SNDFlagReset(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    int instanceIdx;
    int flags;

    flags = AI_GetIntFromStack();
    instanceIdx = AI_GetIntFromStack();
    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    SND_ResetFlags(gaoUltra, instanceIdx, flags);
    return node + 1;
}

// The instance a sound modifier is using. Note that the timer this
// opens is only closed on the not-found path: a match returns from
// inside the loop without it.
int AI_EvalFunc_SNDGetInstanceModifier_C(struct sGameObjectMain* gao, int modifierID)
{
    tSoundMdfStruct* sound;
    tModifier* mdf;

    if (modifierID == -10)
        return -1;
    if (!SND_gst_Params.soundEnabled)
        return -1;
    if (!gao)
        return -1;
    if (!gao->extended)
        return -1;
    if (!gao->extended->mdf)
        return -1;
    if (modifierID < 0)
        return -1;

    QueryPerformanceCounter(&PerfCounterStruct[39]);
    for (mdf = gao->extended->mdf; mdf; mdf = mdf->nextElem)
    {
        if (mdf->modifierIntf->mdfId == 8)
        {
            sound = (tSoundMdfStruct*)mdf->mdfPrivateDataPtr;
            if (sound->modifierID == modifierID)
                return sound->sndInstanceID;
        }
    }
    PRO_QPC_RecordTimeDecRef(39);
    return -1;
}

tAINode* AI_EvalFunc_SNDGetInstanceModifier(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    int modifierID;

    modifierID = AI_GetIntFromStack();
    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_PutIntToStack(AI_EvalFunc_SNDGetInstanceModifier_C(gaoUltra, modifierID));
    return node + 1;
}

void AI_EvalFunc_SNDPlayModifier_C(struct sGameObjectMain* gao, int modifierID)
{
    if (SND_gst_Params.soundEnabled && gao && gao->extended && gao->extended->mdf
        && modifierID != -10)
    {
        AI_l_ForEachSoundModifier(gao, modifierID, SND_PlayMdF);
    }
}

tAINode* AI_EvalFunc_SNDPlayModifier(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    int modifierID;

    modifierID = AI_GetIntFromStack();
    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_EvalFunc_SNDPlayModifier_C(gaoUltra, modifierID);
    return node + 1;
}

void AI_EvalFunc_SNDPauseModifier_C(tGameObjectMain* gao, int modifierID)
{
    if (SND_gst_Params.soundEnabled && gao && gao->extended && gao->extended->mdf
        && modifierID != -10)
    {
        AI_l_ForEachSoundModifier(gao, modifierID, SND_PauseMdF);
    }
}

tAINode* AI_EvalFunc_SNDPauseModifier(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    int modifierID;

    modifierID = AI_GetIntFromStack();
    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_EvalFunc_SNDPauseModifier_C(gaoUltra, modifierID);
    return node + 1;
}

void AI_EvalFunc_SNDStopModifier_C(struct sGameObjectMain* gao, int modifierID)
{
    if (SND_gst_Params.soundEnabled && gao && gao->extended && gao->extended->mdf
        && modifierID != -10)
    {
        AI_l_ForEachSoundModifier(gao, modifierID, SND_StopMdF);
    }
}

tAINode* AI_EvalFunc_SNDStopModifier(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    int modifierID;

    modifierID = AI_GetIntFromStack();
    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_EvalFunc_SNDStopModifier_C(gaoUltra, modifierID);
    return node + 1;
}

/* Not written with AI_l_ForEachSoundModifier, because it is not that
   shape: the all-modifiers arm ORs the flags together and reports them
   only if it saw one, and the by-id arm closes the profiler timer once
   per sound modifier it steps over rather than once for the call. Both
   are preserved. */
int AI_EvalFunc_SNDGetStatusModifier_C(struct sGameObjectMain* gao, int modifierID)
{
    tSoundMdfStruct* sound;
    tModifier* mdf;
    int flags;
    int found;

    if (modifierID == -10)
        return 0;
    if (!SND_gst_Params.soundEnabled)
        return 0;
    if (!gao)
        return 0;
    if (!gao->extended)
        return 0;
    if (!gao->extended->mdf)
        return 0;

    QueryPerformanceCounter(&PerfCounterStruct[39]);
    flags = 0;
    found = 0;

    if (modifierID == -1)
    {
        for (mdf = gao->extended->mdf; mdf; mdf = mdf->nextElem)
        {
            if (mdf->modifierIntf->mdfId == 8)
            {
                flags |= ((tSoundMdfStruct*)(mdf->mdfPrivateDataPtr))->flags;
                found = 1;
            }
        }
        if (found)
        {
            PRO_QPC_RecordTimeDecRef(39);
            return flags;
        }
    }
    else
    {
        for (mdf = gao->extended->mdf; mdf; mdf = mdf->nextElem)
        {
            if (mdf->modifierIntf->mdfId == 8)
            {
                sound = (tSoundMdfStruct*)(mdf->mdfPrivateDataPtr);
                PRO_QPC_RecordTimeDecRef(39);
                if (sound->modifierID == modifierID)
                    return sound->flags;
            }
        }
    }

    PRO_QPC_RecordTimeDecRef(39);
    return 0;
}

tAINode* AI_EvalFunc_SNDGetStatusModifier(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    int modifierID;

    modifierID = AI_GetIntFromStack();
    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_PutIntToStack(AI_EvalFunc_SNDGetStatusModifier_C(gaoUltra, modifierID));
    return node + 1;
}

// Unlike its siblings, the by-id arm goes through
// SNDGetInstanceModifier rather than walking the chain itself, so it
// sets the flags of one instance and not of every matching modifier.
void AI_EvalFunc_SNDSetFlagModifier_C(tGameObjectMain* gao, int modifierID, int flags)
{
    tModifier* mdf;
    int instanceIdx;

    if (modifierID == -10 || !SND_gst_Params.soundEnabled || !gao || !gao->extended
        || !gao->extended->mdf)
    {
        return;
    }

    QueryPerformanceCounter(&PerfCounterStruct[39]);
    if (modifierID == -1)
    {
        for (mdf = gao->extended->mdf; mdf; mdf = mdf->nextElem)
        {
            if (mdf->modifierIntf->mdfId == 8)
            {
                SND_SetFlags(0,
                             ((tSoundMdfStruct*)(mdf->mdfPrivateDataPtr))->sndInstanceID,
                             flags);
            }
        }
    }
    else
    {
        instanceIdx = AI_EvalFunc_SNDGetInstanceModifier_C(gao, modifierID);
        SND_SetFlags(0, instanceIdx, flags);
    }
    PRO_QPC_RecordTimeDecRef(39);
}

tAINode* AI_EvalFunc_SNDSetFlagModifier(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    int modifierID;
    int flags;

    flags = AI_GetIntFromStack();
    modifierID = AI_GetIntFromStack();
    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_EvalFunc_SNDSetFlagModifier_C(gaoUltra, modifierID, flags);
    return node + 1;
}

void AI_EvalFunc_SNDResetFlagModifier_C(tGameObjectMain* gao, int modifierID, int flags)
{
    tModifier* mdf;
    int instanceIdx;

    if (modifierID == -10 || !SND_gst_Params.soundEnabled || !gao || !gao->extended
        || !gao->extended->mdf)
    {
        return;
    }

    QueryPerformanceCounter(&PerfCounterStruct[39]);
    if (modifierID == -1)
    {
        for (mdf = gao->extended->mdf; mdf; mdf = mdf->nextElem)
        {
            if (mdf->modifierIntf->mdfId == 8)
            {
                SND_ResetFlags(0,
                               ((tSoundMdfStruct*)(mdf->mdfPrivateDataPtr))->sndInstanceID,
                               flags);
            }
        }
    }
    else
    {
        instanceIdx = AI_EvalFunc_SNDGetInstanceModifier_C(gao, modifierID);
        SND_ResetFlags(0, instanceIdx, flags);
    }
    PRO_QPC_RecordTimeDecRef(39);
}

tAINode* AI_EvalFunc_SNDResetFlagModifier(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    int modifierID;
    int flags;

    flags = AI_GetIntFromStack();
    modifierID = AI_GetIntFromStack();
    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_EvalFunc_SNDResetFlagModifier_C(gaoUltra, modifierID, flags);
    return node + 1;
}
/* The family builtins select instances by two bitmasks: an instance
   takes part when it has ALL of requiredMask and ANY of anyMask. The
   link and unlink pair are not that shape -- they take one mask and one
   instance index -- which is why their arguments are not reversed. */
tAINode* AI_EvalFunc_SNDFamillyPlay(tAINode* node)
{
    int requiredMask;
    int anyMask;

    anyMask = AI_GetIntFromStack();
    requiredMask = AI_GetIntFromStack();
    SND_PlayFamilly(requiredMask, anyMask);
    return node + 1;
}

tAINode* AI_EvalFunc_SNDFamillyStop(tAINode* node)
{
    int requiredMask;
    int anyMask;

    anyMask = AI_GetIntFromStack();
    requiredMask = AI_GetIntFromStack();
    SND_StopFamilly(requiredMask, anyMask);
    return node + 1;
}

tAINode* AI_EvalFunc_SNDFamillyMuteOn(tAINode* node)
{
    int requiredMask;
    int anyMask;

    anyMask = AI_GetIntFromStack();
    requiredMask = AI_GetIntFromStack();
    SND_MuteOnFamilly(requiredMask, anyMask);
    return node + 1;
}

tAINode* AI_EvalFunc_SNDFamillyMuteOff(tAINode* node)
{
    int requiredMask;
    int anyMask;

    anyMask = AI_GetIntFromStack();
    requiredMask = AI_GetIntFromStack();
    SND_MuteOffFamilly(requiredMask, anyMask);
    return node + 1;
}

tAINode* AI_EvalFunc_SNDFamillySetf(tAINode* node)
{
    int requiredMask;
    int anyMask;
    int field;
    float value;

    value = AI_GetFloatFromStack();
    field = AI_GetIntFromStack();
    anyMask = AI_GetIntFromStack();
    requiredMask = AI_GetIntFromStack();
    SND_SetfFamilly(requiredMask, anyMask, field, value);
    return node + 1;
}

tAINode* AI_EvalFunc_SNDFamillyGetf(tAINode* node)
{
    int requiredMask;
    int anyMask;
    int field;

    field = AI_GetIntFromStack();
    anyMask = AI_GetIntFromStack();
    requiredMask = AI_GetIntFromStack();
    AI_PutFloatToStack(SND_f_GetfFamilly(requiredMask, anyMask, field));
    return node + 1;
}

tAINode* AI_EvalFunc_SNDFamillyGetStatus(tAINode* node)
{
    int requiredMask;
    int anyMask;

    anyMask = AI_GetIntFromStack();
    requiredMask = AI_GetIntFromStack();
    AI_PutIntToStack(SND_i_GetStatusFromFamilly(requiredMask, anyMask));
    return node + 1;
}

tAINode* AI_EvalFunc_SNDFamillyFadeIn(tAINode* node)
{
    int requiredMask;
    int anyMask;

    anyMask = AI_GetIntFromStack();
    requiredMask = AI_GetIntFromStack();
    SND_FadeInFamilly(requiredMask, anyMask);
    return node + 1;
}

tAINode* AI_EvalFunc_SNDFamillyFadeOut(tAINode* node)
{
    int requiredMask;
    int anyMask;

    anyMask = AI_GetIntFromStack();
    requiredMask = AI_GetIntFromStack();
    SND_FadeOutFamilly(requiredMask, anyMask);
    return node + 1;
}

tAINode* AI_EvalFunc_SNDFamillyAdd(tAINode* node)
{
    int famillyMask;
    int instanceIdx;

    famillyMask = AI_GetIntFromStack();
    instanceIdx = AI_GetIntFromStack();
    SND_LinkToFamilly(famillyMask, instanceIdx);
    return node + 1;
}

tAINode* AI_EvalFunc_SNDFamillySub(tAINode* node)
{
    int famillyMask;
    int instanceIdx;

    famillyMask = AI_GetIntFromStack();
    instanceIdx = AI_GetIntFromStack();
    SND_UnlinkFromFamilly(famillyMask, instanceIdx);
    return node + 1;
}

tAINode* AI_EvalFunc_SNDIsTrackPlaying(tAINode* node)
{
    int trackID;

    trackID = AI_GetIntFromStack();
    AI_PutIntToStack(SND_i_IsTrackPlaying(trackID));
    return node + 1;
}

tAINode* AI_EvalFunc_SNDTrackSeti(tAINode* node)
{
    int trackID;
    int field;
    int value;

    value = AI_GetIntFromStack();
    field = AI_GetIntFromStack();
    trackID = AI_GetIntFromStack();
    AI_PutIntToStack(SND_i_TrackSeti(trackID, field, value));
    return node + 1;
}

tAINode* AI_EvalFunc_SNDTrackGeti(tAINode* node)
{
    int trackID;
    int field;

    field = AI_GetIntFromStack();
    trackID = AI_GetIntFromStack();
    AI_PutIntToStack(SND_i_TrackGeti(trackID, field));
    return node + 1;
}

tAINode* AI_EvalFunc_SNDTrackSetf(tAINode* node)
{
    int trackID;
    int field;
    float value;

    value = AI_GetFloatFromStack();
    field = AI_GetIntFromStack();
    trackID = AI_GetIntFromStack();
    AI_PutIntToStack(SND_i_TrackSetf(trackID, field, value));
    return node + 1;
}

tAINode* AI_EvalFunc_SNDTrackGetf(tAINode* node)
{
    int trackID;
    int field;

    field = AI_GetIntFromStack();
    trackID = AI_GetIntFromStack();
    AI_PutFloatToStack(SND_f_TrackGetf(trackID, field));
    return node + 1;
}

void AI_EvalFunc_SNDMicroPosSet_C(struct sGameObjectMain* gao)
{
    if (gao->matrices)
        SND_gpst_RqMicroPosFromAI = gao;
}

tAINode* AI_EvalFunc_SNDMicroPosSet(tAINode* node)
{
    tGameObjectMain* gaoUltra;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_EvalFunc_SNDMicroPosSet_C(gaoUltra);
    return node + 1;
}

// A fader group is a mute group reached with a fixed duration of one.
void AI_EvalFunc_SNDFaderGroupSet_C(int group, float target)
{
    SND_MuteSetUp(group, target, 1.0f);
}

tAINode* AI_EvalFunc_SNDFaderGroupSet(tAINode* node)
{
    int group;
    float target;

    target = AI_GetFloatFromStack();
    group = AI_GetIntFromStack();
    AI_EvalFunc_SNDFaderGroupSet_C(group, target);
    return node + 1;
}

float AI_EvalFunc_SNDFaderGroupGet_C(int group)
{
    return SND_f_MuteGet(group, 1);
}

tAINode* AI_EvalFunc_SNDFaderGroupGet(tAINode* node)
{
    int group;

    group = AI_GetIntFromStack();
    AI_PutFloatToStack(AI_EvalFunc_SNDFaderGroupGet_C(group));
    return node + 1;
}

tAINode* AI_EvalFunc_SNDBankMerge(tAINode* node)
{
    tGameObjectMain* gaoUltra;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    SND_MergeBankNoOver(gaoUltra, (tGameObjectMain*)AI_GetObjectFromStack());
    return node + 1;
}

tAINode* AI_EvalFunc_SNDBankMergeOver(tAINode* node)
{
    tGameObjectMain* gaoUltra;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    SND_MergeBankOver(gaoUltra, (tGameObjectMain*)AI_GetObjectFromStack());
    return node + 1;
}

tAINode* AI_EvalFunc_SNDBankRestore(tAINode* node)
{
    tGameObjectMain* gaoUltra;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    SND_RestoreBank(gaoUltra);
    return node + 1;
}

// Pins the instance to a fixed point instead of to an object, which
// also clears the three position-source bits.
void AI_EvalFunc_SNDInstPosVSet_C(int instanceIdx, Vec3* pos)
{
    tSoundInstanceElem* instance;

    if (instanceIdx < 0 || instanceIdx >= SND_gst_Params.soundInstanceNum)
        return;

    instance = &SND_gst_Params.soundInstances[instanceIdx];
    if ((instance->flags & 0x400) == 0)
        return;
    if (!pos)
        return;

    instance->sndInstPosVec = *pos;
    instance->sndPositionSetInSetPosPtr = &instance->sndInstPosVec;
    instance->sndPosMtxSetInSetPos = 0;
    instance->flags &= ~0x7000000u;
}

tAINode* AI_EvalFunc_SNDInstPosVSet(tAINode* node)
{
    Vec3* pos;
    int instanceIdx;

    // the position is read where the stack keeps it rather than copied out
    pos = (Vec3*)AI_PopStackSlot();
    instanceIdx = AI_GetIntFromStack();
    AI_EvalFunc_SNDInstPosVSet_C(instanceIdx, pos);
    return node + 1;
}

// These two do not null-check the object before reaching its extended
// block, where their Play, Pause and Stop siblings do.
void AI_EvalFunc_SNDPrefetchModifier_C(struct sGameObjectMain* gao, int modifierID)
{
    if (modifierID != -10 && SND_gst_Params.soundEnabled && gao->extended
        && gao->extended->mdf)
    {
        AI_l_ForEachSoundModifier(gao, modifierID, SND_PrefetchMdF);
    }
}

tAINode* AI_EvalFunc_SNDPrefetchModifier(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    int modifierID;

    modifierID = AI_GetIntFromStack();
    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_EvalFunc_SNDPrefetchModifier_C(gaoUltra, modifierID);
    return node + 1;
}

tAINode* AI_EvalFunc_SNDPrefetch(tAINode* node)
{
    int instanceIdx;

    instanceIdx = AI_GetIntFromStack();
    SND_StreamPrefetch(instanceIdx);
    return node + 1;
}

void AI_EvalFunc_SNDFlushModifier_C(tGameObjectMain* gao, int modifierID)
{
    if (modifierID != -10 && SND_gst_Params.soundEnabled && gao->extended
        && gao->extended->mdf)
    {
        AI_l_ForEachSoundModifier(gao, modifierID, SND_FlushMdF);
    }
}

tAINode* AI_EvalFunc_SNDFlushModifier(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    int modifierID;

    modifierID = AI_GetIntFromStack();
    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_EvalFunc_SNDFlushModifier_C(gaoUltra, modifierID);
    return node + 1;
}

tAINode* AI_EvalFunc_SNDFlush(tAINode* node)
{
    int instanceIdx;

    instanceIdx = AI_GetIntFromStack();
    SND_StreamFlush(instanceIdx);
    return node + 1;
}

tAINode* AI_EvalFunc_SNDInstanceSeti(tAINode* node)
{
    int instanceIdx;
    int field;
    int value;

    value = AI_GetIntFromStack();
    field = AI_GetIntFromStack();
    instanceIdx = AI_GetIntFromStack();
    AI_PutIntToStack(SND_i_InstanceSeti_return_negative_1(instanceIdx, field, value));
    return node + 1;
}

tAINode* AI_EvalFunc_SNDInstanceGeti(tAINode* node)
{
    int instanceIdx;
    int field;

    field = AI_GetIntFromStack();
    instanceIdx = AI_GetIntFromStack();
    AI_PutIntToStack(SND_i_InstanceGeti(instanceIdx, field));
    return node + 1;
}

tAINode* AI_EvalFunc_SNDInstanceSetf(tAINode* node)
{
    int instanceIdx;
    int field;
    float value;

    value = AI_GetFloatFromStack();
    field = AI_GetIntFromStack();
    instanceIdx = AI_GetIntFromStack();
    AI_PutIntToStack(SND_i_InstanceSetf(instanceIdx, field, value));
    return node + 1;
}

tAINode* AI_EvalFunc_SNDInstanceGetf(tAINode* node)
{
    int instanceIdx;
    int field;

    field = AI_GetIntFromStack();
    instanceIdx = AI_GetIntFromStack();
    AI_PutFloatToStack(SND_f_InstanceGetf(instanceIdx, field));
    return node + 1;
}

// Plays the sound with its frequency scaled by a random factor. The
// scaled frequency is rounded through the magic double, which puts the
// integer in the low word of the sum.
int AI_EvalFunc_SNDRequestPlayRandomFreq_C(struct sGameObjectMain* gao, int soundID, float volume, float minScale, float maxScale)
{
    tSoundInstanceElem* instance;
    double scale;
    double rounded;
    float baseFreq;
    float scaled;
    int instanceIdx;

    instanceIdx = AI_EvalFunc_SNDRequestPlay_C(gao, soundID, volume);
    if (instanceIdx < 0 || instanceIdx >= SND_gst_Params.soundInstanceNum)
        return -1;

    instance = &SND_gst_Params.soundInstances[instanceIdx];
    if ((instance->flags & 0x400) == 0)
        return -1;

    scale = AI_EvalFunc_MATHRandFloat_C(minScale, maxScale);
    baseFreq = (float)instance->freq;
    scaled = (float)(scale * baseFreq);
    rounded = scaled + MATH_gd_magicDoubleForRound;
    SND_SetFreq(instanceIdx, *(uint32_t*)&rounded);
    return instanceIdx;
}

tAINode* AI_EvalFunc_SNDRequestPlayRandomFreq(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    int soundID;
    float volume;
    float minScale;
    float maxScale;

    maxScale = AI_GetFloatFromStack();
    minScale = AI_GetFloatFromStack();
    volume = AI_GetFloatFromStack();
    soundID = AI_GetIntFromStack();
    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_PutIntToStack(AI_EvalFunc_SNDRequestPlayRandomFreq_C(gaoUltra, soundID, volume,
                                                            minScale, maxScale));
    return node + 1;
}

int AI_EvalFunc_SND_SetObject_C(struct sGameObjectMain* gao, int instanceIdx)
{
    tSoundInstanceElem* instance;

    if (instanceIdx < 0 || instanceIdx >= SND_gst_Params.soundInstanceNum)
        return -1;

    instance = &SND_gst_Params.soundInstances[instanceIdx];
    if ((instance->flags & 0x400) == 0)
        return -1;
    if (SND_i_CheckOrInitGAO(gao))
        return -1;

    instance->gao = gao;
    SND_SetPos(gao, instanceIdx, gao->matrices);
    return 0;
}

tAINode* AI_EvalFunc_SND_SetObject(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    int instanceIdx;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    instanceIdx = AI_GetIntFromStack();
    AI_PutIntToStack(AI_EvalFunc_SND_SetObject_C(gaoUltra, instanceIdx));
    return node + 1;
}

tAINode* AI_EvalFunc_SND_Fade(tAINode* node)
{
    int instanceIdx;
    float from;
    float to;
    float duration;

    duration = AI_GetFloatFromStack();
    to = AI_GetFloatFromStack();
    from = AI_GetFloatFromStack();
    instanceIdx = AI_GetIntFromStack();
    SND_Fade(instanceIdx, from, to, duration);
    return node + 1;
}

void AI_EvalFunc_SNDPlaySoundOnTrackFromTo_C(tGameObjectMain* gao, int instanceIdx, float from, float to)
{
    if (!AI_l_CanStartInstance(instanceIdx))
        return;

    QueryPerformanceCounter(&PerfCounterStruct[39]);
    SND_i_CheckOrInitGAO(gao);
    if (gao)
        AI_l_AttachInstanceToGAO(gao, instanceIdx);
    SND_PlaySoundOnTrackFromTo(instanceIdx, from, to);
    PRO_QPC_RecordTimeDecRef(39);
}

void AI_EvalFunc_SNDPlaySoundLoopedOnTrackFrom_C(tGameObjectMain* gao, int instanceIdx, float from)
{
    if (!AI_l_CanStartInstance(instanceIdx))
        return;

    QueryPerformanceCounter(&PerfCounterStruct[39]);
    SND_i_CheckOrInitGAO(gao);
    AI_l_AttachInstanceToGAO(gao, instanceIdx);
    SND_PlaySoundLoopingOnTrackFrom(instanceIdx, from);
    PRO_QPC_RecordTimeDecRef(39);
}

/* One builtin for all eight ways of starting a sound, chosen by the low
   four bits of the mode: bit 0 loops, bit 1 puts it on a track, and
   bits 2 and 3 say which of the from and to times are given. Every one
   of the sixteen values is handled, so the default arm cannot be
   reached; it is kept as the original has it. */
int AI_EvalFunc_SND_ExtPlay_C(struct sGameObjectMain* gao, int instanceIdx, int mode, float from, float to)
{
    switch (mode & 0xF)
    {
    case 0:
    case 4:
    case 8:
    case 12:
        AI_EvalFunc_SNDUltraPlaySound_C(gao, instanceIdx);
        return 0;
    case 1:
    case 5:
    case 9:
    case 13:
        AI_EvalFunc_SNDUltraPlaySoundLooped_C(gao, instanceIdx);
        return 0;
    case 2:
        AI_EvalFunc_SNDPlaySoundOnTrack_C(gao, instanceIdx);
        return 0;
    case 3:
    case 11:
        AI_EvalFunc_SNDPlaySoundLoopedOnTrack_C(gao, instanceIdx);
        return 0;
    case 6:
        AI_EvalFunc_SNDPlaySoundOnTrackFromTo_C(gao, instanceIdx, from, -1.0);
        return 0;
    case 7:
    case 15:
        AI_EvalFunc_SNDPlaySoundLoopedOnTrackFrom_C(gao, instanceIdx, from);
        return 0;
    case 10:
        AI_EvalFunc_SNDPlaySoundOnTrackFromTo_C(gao, instanceIdx, 0.0, to);
        return 0;
    case 14:
        AI_EvalFunc_SNDPlaySoundOnTrackFromTo_C(gao, instanceIdx, from, to);
        return 0;
    default:
        return -1;
    }
}

tAINode* AI_EvalFunc_SND_ExtPlay(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    int instanceIdx;
    int mode;
    float from;
    float to;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    to = AI_GetFloatFromStack();
    from = AI_GetFloatFromStack();
    mode = AI_GetIntFromStack();
    instanceIdx = AI_GetIntFromStack();
    AI_PutIntToStack(AI_EvalFunc_SND_ExtPlay_C(gaoUltra, instanceIdx, mode, from, to));
    return node + 1;
}

tAINode* AI_EvalFunc_SND_SoundDurationGet(tAINode* node)
{
    tGameObjectMain* gaoUltra;
    int sndIdx;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    sndIdx = AI_GetIntFromStack();
    AI_PutFloatToStack(SND_SoundDurationGet(gaoUltra, sndIdx));
    return node + 1;
}
