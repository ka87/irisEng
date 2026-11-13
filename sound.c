#include "common.h"
#include "basic_array.h"
#include "loading.h"
#include "bigfiles.h"
#include "memory.h"
#include "modifiers/gensoundmodifier.h"
#include "modifiers/loadingsoundmodifier.h"
#include "textget.h"
#include "dbg.h"
#include "strings.h"  
#include "time.h"
#include "sound.h"
#include "win/winsound.h"

float SND_gf_AutoVolumeOff = 1.0f;

float SND_gf_FadeOut[] = {
    1.0f, 0.99607843f, 0.99215686f, 0.98823529f, 0.98431373f, 0.98039216f, 0.97647059f, 0.97254902f, 0.96862745f, 0.96470588f,
    0.96078432f, 0.95686275f, 0.95294118f, 0.94901961f, 0.94509804f, 0.94117647f, 0.93725491f, 0.93333334f, 0.92941177f, 0.9254902f,
    0.92156863f, 0.91764706f, 0.9137255f, 0.90980393f, 0.90588236f, 0.90196079f, 0.89803922f, 0.89411765f, 0.89019608f, 0.88627452f,
    0.88235295f, 0.87843138f, 0.87450981f, 0.87058824f, 0.86666667f, 0.86274511f, 0.85882354f, 0.85490197f, 0.8509804f, 0.84705883f,
    0.84313726f, 0.8392157f, 0.83529413f, 0.83137256f, 0.82745099f, 0.82352942f, 0.81960785f, 0.81568629f, 0.81176472f, 0.80784315f,
    0.80392158f, 0.80000001f, 0.79607844f, 0.79215688f, 0.78823531f, 0.78431374f, 0.78039217f, 0.7764706f, 0.77254903f, 0.76862746f,
    0.7647059f, 0.76078433f, 0.75686276f, 0.75294119f, 0.74901962f, 0.74509805f, 0.74117649f, 0.73725492f, 0.73333335f, 0.72941178f,
    0.72549021f, 0.72156864f, 0.71764708f, 0.71372551f, 0.70980394f, 0.70588237f, 0.7019608f, 0.69803923f, 0.69411767f, 0.6901961f,
    0.68627453f, 0.68235296f, 0.67843139f, 0.67450982f, 0.67058825f, 0.66666669f, 0.66274512f, 0.65882355f, 0.65490198f, 0.65098041f,
    0.64705884f, 0.64313728f, 0.63921571f, 0.63529414f, 0.63137257f, 0.627451f, 0.62352943f, 0.61960787f, 0.6156863f, 0.61176473f,
    0.60784316f, 0.60392159f, 0.60000002f, 0.59607846f, 0.59215689f, 0.58823532f, 0.58431375f, 0.58039218f, 0.57647061f, 0.57254905f,
    0.56862748f, 0.56470591f, 0.56078434f, 0.55686277f, 0.5529412f, 0.54901963f, 0.54509807f, 0.5411765f, 0.53725493f, 0.53333336f,
    0.52941179f, 0.52549022f, 0.52156866f, 0.51764709f, 0.51372552f, 0.50980395f, 0.50588238f, 0.50196075f, 0.49803922f, 0.49411765f,
    0.49019608f, 0.48627451f, 0.48235294f, 0.47843137f, 0.47450981f, 0.47058824f, 0.46666667f, 0.4627451f, 0.45882353f, 0.45490196f,
    0.4509804f, 0.44705883f, 0.44313726f, 0.43921569f, 0.43529412f, 0.43137255f, 0.42745098f, 0.42352942f, 0.41960785f, 0.41568628f,
    0.41176471f, 0.40784314f, 0.40392157f, 0.40000001f, 0.39607844f, 0.39215687f, 0.3882353f, 0.38431373f, 0.38039216f, 0.3764706f,
    0.37254903f, 0.36862746f, 0.36470589f, 0.36078432f, 0.35686275f, 0.35294119f, 0.34901962f, 0.34509805f, 0.34117648f, 0.33725491f,
    0.33333334f, 0.32941177f, 0.32549021f, 0.32156864f, 0.31764707f, 0.3137255f, 0.30980393f, 0.30588236f, 0.3019608f, 0.29803923f,
    0.29411766f, 0.29019609f, 0.28627452f, 0.28235295f, 0.27843139f, 0.27450982f, 0.27058825f, 0.26666668f, 0.26274511f, 0.25882354f,
    0.25490198f, 0.25098038f, 0.24705882f, 0.24313726f, 0.23921569f, 0.23529412f, 0.23137255f, 0.22745098f, 0.22352941f, 0.21960784f,
    0.21568628f, 0.21176471f, 0.20784314f, 0.20392157f, 0.2f, 0.19607843f, 0.19215687f, 0.1882353f, 0.18431373f, 0.18039216f,
    0.17647059f, 0.17254902f, 0.16862746f, 0.16470589f, 0.16078432f, 0.15686275f, 0.15294118f, 0.14901961f, 0.14509805f, 0.14117648f,
    0.13725491f, 0.13333334f, 0.12941177f, 0.12549019f, 0.12156863f, 0.11764706f, 0.11372549f, 0.10980392f, 0.10588235f, 0.10196079f,
    0.098039217f, 0.094117649f, 0.090196081f, 0.086274512f, 0.082352944f, 0.078431375f, 0.074509807f, 0.070588239f,  0.06666667f,
    0.062745094f, 0.05882353f, 0.054901961f, 0.050980393f, 0.047058824f, 0.043137256f, 0.039215688f, 0.035294119f,  0.031372547f,
    0.027450981f, 0.023529412f, 0.019607844f, 0.015686275f, 0.011764706f, 0.0078431368f, 0.0039215689f,  0.0f
};

int i_init_521 = 0; // FIXME: game doesn't set it explicitly


BOOL SND_b_IsLoadingInstance(int instanceIdx)
{
    BOOL result;

    if (instanceIdx >= 0 && instanceIdx < SND_gst_Params.soundInstanceNum)
        result = instanceIdx == SND_gst_LSGlobal.soundInstanceIndex;
    else
        result = 0;
    return result;
}

void SND_DetachLoadingInstance()
{
    tSoundInstanceElem* inst;
    int flags;

    if (SND_gst_LSGlobal.soundInstanceIndex >= 0 && SND_gst_LSGlobal.soundInstanceIndex < SND_gst_Params.soundInstanceNum)
    {
        inst = &SND_gst_Params.soundInstances[SND_gst_LSGlobal.soundInstanceIndex];
        flags = inst->flags;
        if ((flags & 0x400) != 0)
        {
            if ((flags & 0x1F000000) != 0)
                inst->flags = flags & ~0x1F000000u;
            inst->flags &= ~0x1000u;
            inst->gao = 0;
            inst->sndPosMtxSetInSetPos = 0;
            inst->sndPositionSetInSetPosPtr = 0;
            SND_gst_LSGlobal.userCallBackFunction = 0;
            SND_gst_LSGlobal.userCallBackFunctionParam = 0;
        }
    }
}

static void SND_TrackReleaseResource(int trackIdx)
{
    SND_gast_TrackResources[SND_gax_TrackPlayingList[trackIdx].trackResourceNum - 1].isBusy = 0;
    SND_gax_TrackPlayingList[trackIdx].trackResourceNum = 0;
    SND_gax_TrackPlayingList[trackIdx].sndBufferElem = 0;
    SND_gax_TrackPlayingList[trackIdx].trackResource = 0;
}

static void SND_TrackSwitchToMono(int trackIdx)
{
    tTrackListElem* track;
    tSndSoundBufferElem* buffer;
    int resourceIdx;

    track = &SND_gax_TrackPlayingList[trackIdx];
    buffer = track->sndBufferElem;
    track->sndBufferElem = track->trackResource;
    track->trackResource = buffer;
    track->flags &= ~4u;
    if (track->stereoToMono_sub499C20)
    {
        resourceIdx = track->stereoToMono_sub499C20 - 1;
        track->stereoToMono_sub499C20 = 0;
        SND_gast_TrackResources[resourceIdx].isBusy = 0;
        SND_gpst_Interface.win.StreamGetStereoToMono(&track->trackResource, &SND_gast_TrackResources[resourceIdx].stereoSoundBuffer);
    }
}

void SND_TrackDeleteInstanceFromList(int id)
{
    int flags;
    tSoundInstanceElem* inst;
    int trackIdx;

    if (id >= 0 && id < SND_gst_Params.soundInstanceNum)
    {
        flags = SND_gst_Params.soundInstances[id].flags;
        inst = &SND_gst_Params.soundInstances[id];
        if ((flags & 0x400) != 0)
        {
            inst->flags = flags & ~0x20010u;
            trackIdx = BAS_bsearch(inst->trackID, &SND_gst_TrackId);
            if (trackIdx != -1)
            {
                if (SND_gax_TrackPlayingList[trackIdx].trackID == id)
                {
                    SND_gpst_Interface.win.Stop(SND_gax_TrackPlayingList[trackIdx].sndBufferElem);
                    SND_gax_TrackPlayingList[trackIdx].trackID = -1;
                    SND_gax_TrackPlayingList[trackIdx].flags &= ~2u;
                    inst->soundIntf = SND_gax_TrackPlayingList[trackIdx].soundIntf;
                    SND_gax_TrackPlayingList[trackIdx].soundIntf = 0;
                    BAS_bdelete((unsigned int)inst->soundBufElemFromModel, &SND_gst_TrackLookUpTable); // TODO FIXME, biztos?????
                    if (SND_gax_TrackPlayingList[trackIdx].stereoToMono_sub499C20)
                        SND_TrackSwitchToMono(trackIdx);
                    SND_TrackReleaseResource(trackIdx);
                }
            }
        }
    }
}

void SND_TrackForceFreeResources(int keepIdx)
{
    tTrackListElem* track;
    int idx;

    if (SND_gi_TrackNb <= 0)
        return;
    for (idx = 0, track = SND_gax_TrackPlayingList; ; ++idx, ++track)
    {
        if (track->flags != 1 && idx != keepIdx)
        {
            if (track->stereoToMono_sub499C20)
                break;
            if (track->trackResourceNum)
            {
                if ((SND_gax_TrackPlayingList[idx].flags & 2) != 0)
                {
                    SND_gpst_Interface.win.Stop(SND_gax_TrackPlayingList[idx].sndBufferElem);
                    SND_TrackDeleteInstanceFromList(SND_gax_TrackPlayingList[idx].trackID);
                }
                SND_TrackReleaseResource(idx);
                return;
            }
        }
        if (idx + 1 >= SND_gi_TrackNb)
            return;
    }
    if ((SND_gax_TrackPlayingList[idx].flags & 2) != 0)
    {
        SND_gpst_Interface.win.Stop(SND_gax_TrackPlayingList[idx].sndBufferElem);
        SND_TrackDeleteInstanceFromList(SND_gax_TrackPlayingList[idx].trackID);
    }
}

static void SND_TrackAcquireResource(int trackIdx)
{
    tTrackResourceElem* resource;
    int resourceIdx;

    for (;;)
    {
        resourceIdx = 0;
        resource = SND_gast_TrackResources;
        while (resource->isBusy && resource < &SND_gast_TrackResources[3])
        {
            ++resource;
            ++resourceIdx;
        }
        if (!resource->isBusy)
            break;
        SND_TrackForceFreeResources(trackIdx);
    }
    SND_gax_TrackPlayingList[trackIdx].sndBufferElem = SND_gast_TrackResources[resourceIdx].stereoSoundBuffer;
    SND_gax_TrackPlayingList[trackIdx].trackResource = SND_gast_TrackResources[resourceIdx].monoSoundBuffer;
    SND_gax_TrackPlayingList[trackIdx].trackResourceNum = resourceIdx + 1;
    SND_gast_TrackResources[resourceIdx].isBusy = 1;
}

void SND_FreeInstance(int instanceIdx)
{
    tSoundInstanceElem* inst;

    inst = &SND_gst_Params.soundInstances[instanceIdx];
    if ((SND_gst_Params.allocatedSounds[inst->allocatedSoundsIdx].flags & 0x80000000) == 0)
        --SND_AllocatedSoundsNumWithhoutFlag80000000;
    else
        --SND_AllocatedSoundsNumWithFlag80000000;
    if ((inst->flags & 0x20000) != 0)
        SND_TrackDeleteInstanceFromList(instanceIdx);
    if ((inst->flags & 0x40) != 0)
    {
        inst->soundIntf->Stop(inst->soundBufElemFromModel);
        inst->soundIntf->Release(inst->soundBufElemFromModel);
    }
    else
    {
        inst->soundIntf->Stop(inst->soundBufElemFromModel);
        SND_gst_Params.allocatedSounds[inst->allocatedSoundsIdx].flags &= ~0x40u;
    }
    SND_SModifierDeleteAll(&inst->modifierInstances);
    inst->allocatedSoundsIdx = -1;
    inst->trackID = -1;
    inst->flags = 0;
    inst->muteFactorFlags = 0;
    inst->familyBitmask = 0;
    inst->freq = 0;
    inst->volume = 0.0;
    inst->front = 0;
    inst->pan = 0;
    inst->minPan = 0;
    inst->nearArr[0] = 0.0;
    inst->nearArr[1] = 0.0;
    inst->nearArr[2] = 0.0;
    inst->farArr[0] = 0.0;
    inst->farArr[1] = 0.0;
    inst->farArr[2] = 0.0;
    inst->dtFar = 0.0;
    inst->centerArr[0] = 0.0;
    inst->centerArr[1] = 0.0;
    inst->centerArr[2] = 0.0;
    inst->farCoefficient = 0;
    inst->middleCoefficient = 0;
    inst->sndPosMtxSetInSetPos = 0;
    inst->sndPositionSetInSetPosPtr = 0;
    inst->sndInstPosVecPrevious.x = 0.0f;
    inst->sndInstPosVecPrevious.y = 0.0f;
    inst->sndInstPosVecPrevious.z = 0.0f;
    inst->sndInstPosVec.x = 0.0;
    inst->sndInstPosVec.y = 0.0;
    inst->sndInstPosVec.z = 0.0;
    inst->modifierInstances = 0;
    inst->soundBufElemFromModel = 0;
    inst->requestTimeStamp = 0.0;
    inst->gao = 0;
    inst->soundIntf = 0;
    inst->unkMuteSetupSeeInitVolTable = 0;
    inst->timeRemainTillStart = 0.0;
    inst->timeFromDefault = 0.0;
}

void SND_DetachSound(int soundIdx)
{
    int idx;
    tSoundInstanceElem* inst;

    if (SND_gst_Params.soundEnabled)
    {
        for (idx = 0; idx < SND_gst_Params.soundInstanceNum; ++idx)
        {
            inst = &SND_gst_Params.soundInstances[idx];
            if ((inst->flags & 0x400) != 0 && inst->allocatedSoundsIdx == soundIdx)
            {
                if (SND_b_IsLoadingInstance(idx))
                {
                    SND_DetachLoadingInstance();
                }
                else
                {
                    inst->soundIntf->Stop(inst->soundBufElemFromModel);
                    SND_FreeInstance(idx);
                }
            }
        }
    }
}

BOOL SND_b_IsLoadingSound(int soundIdx)
{
    BOOL result;

    if (soundIdx >= 0 && soundIdx < SND_gst_Params.allocatedSoundsMaxNum)
        result = soundIdx == SND_gst_LSGlobal.soundElemIndex;
    else
        result = 0;
    return result;
}

void SND_DeleteByIndex(int soundIdx)
{
    tSndSoundElem* sound;

    if (SND_gst_Params.allocatedSounds)
    {
        if (soundIdx >= 0 && soundIdx < SND_gst_Params.allocatedSoundsMaxNum)
        {
            sound = &SND_gst_Params.allocatedSounds[soundIdx];
            if ((sound->flags & 0x400) != 0 && sound->usageCnt-- == 1)
            {
                SND_DetachSound(soundIdx);
                if (!SND_b_IsLoadingSound(soundIdx))
                {
                    if (sound->soundModifier)
                    {
                        SND_SModifierDeleteAllLinkedSound(sound->soundModifier);
                        SND_SModifierDeleteAll(&sound->soundModifier);
                    }
                    else if ((sound->flags & 8) == 0)
                    {
                        sound->sndInterface->Stop(sound->sndSndBufElem);
                        sound->sndInterface->Release(sound->sndSndBufElem);
                    }
                    sound->soundKey = -1;
                    sound->flags = 0;
                    sound->usageCnt = 0;
                    sound->sndSndBufElem = 0;
                    sound->freq = 0;
                    sound->numChannels = 0;
                    sound->bitsPerSample = 0;
                    sound->sndInterface = 0;
                    sound->soundModifier = 0;
                    sound->startDataPos = 0;
                    sound->fullsoundLen = 0;
                    sound->byteRate = 0;
                    sound->blockAlign = 0;
                    sound->extraDataSize = 0;
                    sound->format = 0;
                }
            }
        }
    }
}

static void SND_MetaBankUserRemove(tSndBank* bank)
{
    unsigned int idx;

    for (idx = 0; idx < SND_gul_MetaBankUserNb; ++idx)
    {
        if (SND_gax_MetaBankUserList[idx].bank == bank)
        {
            SND_gax_MetaBankUserList[idx].bank = 0;
            SND_gax_MetaBankUserList[idx].offset = 0;
            return;
        }
    }
}

void SND_FreeSndBank(struct sSndBank** bankPtr)
{
    tSndBank* bank;
    int i;
    int soundIdx;
    int* indices;

    if (*bankPtr)
    {
        --(*bankPtr)->usageCount;
        bank = *bankPtr;
        if (!(*bankPtr)->usageCount)
        {
            if (bank->soundIndicesV2)
            {
                for (i = 0; i < (*bankPtr)->elemNum; ++i)
                {
                    soundIdx = (*bankPtr)->soundIndices[i];
                    if (soundIdx != (*bankPtr)->soundIndicesV2[i] && soundIdx != -1)
                        SND_DeleteByIndex((*bankPtr)->soundIndices[i]);
                }
                MEM_Free((*bankPtr)->soundIndices);
                (*bankPtr)->soundIndices = (*bankPtr)->soundIndicesV2;
                (*bankPtr)->soundIndicesV2 = 0;
            }
            indices = (*bankPtr)->soundIndices;
            if (indices)
            {
                for (i = 0; i < (*bankPtr)->elemNum; ++i)
                {
                    if ((*bankPtr)->soundIndices[i] != -1)
                        SND_DeleteByIndex((*bankPtr)->soundIndices[i]);
                }
                MEM_Free((*bankPtr)->soundIndices);
            }
            SND_MetaBankUserRemove(*bankPtr);
            if (indices)
                LOA_DeleteAddress((int)*bankPtr);
            MEM_Free(*bankPtr);
            *bankPtr = 0;
        }
    }
}

void SND_FreeMainStruct(struct sGameObjectMain* gao)
{
    tGameObjectExtended* extended;

    if ((gao->flags & 0x8000000) != 0)
    {
        extended = gao->extended;
        if (extended)
        {
            if (extended->snd)
                SND_FreeSndBank(&extended->snd);
        }
    }
}

void SND_Duplicate(struct sGameObjectMain* src, struct sGameObjectMain* dest)
{
    if (src && src->extended && dest)
    {
        if ((dest->flags & 0x8000000) == 0)
            dest->flags |= 0x8000000u;
        OBJ_GameObject_CreateExtendedIfNot(dest);
        dest->extended->snd = src->extended->snd;
        ++src->extended->snd->usageCount;
    }
}

void SND_ResolveRefsForMdF(struct sModifier* mdf)
{
    tModifier* i;
    tSoundMdfStruct* sndMdf;
    tSndBank* bank;

    for (i = mdf; i; i = i->nextElem)
    {
        if (i->modifierIntf->mdfId == 8)          // GEN_ModifierSound
        {
            sndMdf = (tSoundMdfStruct*)(i->mdfPrivateDataPtr);
            if ((sndMdf->flags & 0x800) != 0)
            {
                bank = sndMdf->soundBank;
                if (bank)
                {
                    if (bank->soundIndices)
                        ++bank->usageCount;
                }
            }
        }
    }
}

void SND_ResolveRefs(struct sGameObjectMain* gao)
{
    tGameObjectExtended* extended;
    tSndBank* bank;

    extended = gao->extended;
    if (extended)
    {
        if ((gao->flags & 0x8000000) != 0)
        {
            bank = extended->snd;
            if (bank)
            {
                if (bank->soundIndices)
                    ++bank->usageCount;
            }
        }
    }
}

void SND_RestoreBank(struct sGameObjectMain* gao)
{
    tGameObjectExtended* extended;
    tSndBank* bank;
    int i;
    int soundIdx;

    if (gao)
    {
        if ((gao->flags & 0x8000000) != 0)
        {
            extended = gao->extended;
            if (extended)
            {
                bank = extended->snd;
                if (bank)
                {
                    if (bank->soundIndices && bank->soundIndicesV2)
                    {
                        for (i = 0; i < bank->elemNum; ++i)
                        {
                            soundIdx = bank->soundIndices[i];
                            if (soundIdx != bank->soundIndicesV2[i] && soundIdx != -1)
                                SND_DeleteByIndex(bank->soundIndices[i]);
                        }
                        MEM_Free(bank->soundIndices);
                        bank->soundIndices = bank->soundIndicesV2;
                        bank->soundIndicesV2 = 0;
                    }
                }
            }
        }
    }
}

void SND_DetachObject(struct sGameObjectMain* gao)
{
    int idx;
    tSoundInstanceElem* instances;
    tGameObjectExtended* extended;
    tSndBank* bank;

    if (SND_gst_Params.soundEnabled && (gao->flags & 0x8000000) != 0)
    {
        instances = SND_gst_Params.soundInstances;
        for (idx = 0; idx < SND_gst_Params.soundInstanceNum; ++idx)
        {
            if ((instances[idx].flags & 0x400) != 0 && instances[idx].gao == gao)
            {
                if ((gao->flags & 0x8000000) == 0)
                    return;
                extended = gao->extended;
                if (!extended || !extended->snd)
                    return;
                instances[idx].soundIntf->Stop(instances[idx].soundBufElemFromModel);
                SND_FreeInstance(idx);
                instances = SND_gst_Params.soundInstances;
            }
        }
        extended = gao->extended;
        if (extended)
        {
            bank = extended->snd;
            if (bank)
            {
                if (bank->soundIndices)
                {
                    if (bank->usageCount == 1)
                        SND_RestoreBank(gao);
                }
            }
        }
    }
}

void SND_Release(int soundInstanceID)
{
    tSoundInstanceElem* inst;

    if (soundInstanceID >= 0 && soundInstanceID < SND_gst_Params.soundInstanceNum)
    {
        inst = &SND_gst_Params.soundInstances[soundInstanceID];
        if ((inst->flags & 0x400) != 0 && !SND_b_IsLoadingInstance(soundInstanceID))
        {
            inst->soundIntf->Stop(inst->soundBufElemFromModel);
            SND_FreeInstance(soundInstanceID);
        }
    }
}

void SND_DestroyInstanceOfGao(struct sGameObjectMain* gao)
{
    tGameObjectExtended* extended;
    tModifier* i;
    int mdfId;
    int idx;
    tSoundInstanceElem* instances;

    if (gao && SND_gst_Params.soundEnabled)
    {
        extended = gao->extended;
        if (extended)
        {
            for (i = extended->mdf; i; i = i->nextElem)
            {
                mdfId = i->modifierIntf->mdfId;
                if (mdfId == 8 || mdfId == 15 || mdfId == 19) // 8 = gensound, 15 = gensoundfx, 19 = loadingsound
                    i->modifierIntf->ReInit(i);
            }
        }
        instances = SND_gst_Params.soundInstances;
        for (idx = 0; idx < SND_gst_Params.soundInstanceNum; ++idx)
        {
            if ((instances[idx].flags & 0x400) != 0)
            {
                if (instances[idx].gao)
                {
                    if (instances[idx].gao == gao)
                    {
                        SND_Release(idx);
                        instances = SND_gst_Params.soundInstances;
                    }
                }
            }
        }
    }

}

float SND_f_MuteComputeFactor(tSoundInstanceElem* inst)
{
    int flags;
    double result;
    tMutesSetupStruct* fader;
    tGameObjectMain* gao;
    tGameObjectExtended* extended;

    flags = inst->flags;
    if ((flags & 0x400) == 0)
        return 1.0;
    fader = inst->unkMuteSetupSeeInitVolTable;
    if (fader->muteEnabled)
        result = fader->muteSetUp2;
    else
        result = fader->muteSetUp1;
    if (SND_Mutes_B30600_fadergroups[10].muteEnabled)
    {
        if ((inst->muteFactorFlags & 0x200) != 0)
            result = result * SND_Mutes_B30600_fadergroups[10].muteSetUp2;
        else
            result = result * SND_Mutes_B30600_fadergroups[10].muteSetUp1;
    }
    if (SND_Mutes_B30600_fadergroups[11].muteEnabled)
    {
        if ((inst->muteFactorFlags & 0x400) != 0)
            result = result * SND_Mutes_B30600_fadergroups[11].muteSetUp2;
        else
            result = result * SND_Mutes_B30600_fadergroups[11].muteSetUp1;
    }
    if (SND_Mutes_B30600_fadergroups[0].muteEnabled)
    {
        gao = inst->gao;
        if (gao)
        {
            if ((gao->flags & 0x8000000) != 0)
            {
                extended = gao->extended;
                if (extended)
                {
                    if (extended->snd)
                    {
                        if ((gao->flags2 & 0x4000006) == 2)
                            result = result * SND_Mutes_B30600_fadergroups[0].muteSetUp1;
                        else
                            result = result * SND_Mutes_B30600_fadergroups[0].muteSetUp2;
                    }
                }
            }
        }
    }
    if (SND_Mutes_B30600_fadergroups[1].muteEnabled)
    {
        if ((inst->muteFactorFlags & 0x40) != 0)
            result = result * SND_Mutes_B30600_fadergroups[1].muteSetUp1;
        else
            result = result * SND_Mutes_B30600_fadergroups[1].muteSetUp2;
    }
    if (SND_Mutes_B30600_fadergroups[4].muteEnabled)
    {
        if ((inst->muteFactorFlags & 0x80) == 0)
            result = result * SND_Mutes_B30600_fadergroups[4].muteSetUp1;
        else
            result = result * SND_Mutes_B30600_fadergroups[4].muteSetUp2;
    }
    if (SND_Mutes_B30600_fadergroups[2].muteEnabled)
    {
        if ((inst->muteFactorFlags & 0x100) != 0)
            result = result * SND_Mutes_B30600_fadergroups[2].muteSetUp2;
        else
            result = result * SND_Mutes_B30600_fadergroups[2].muteSetUp1;
    }
    if (SND_Mutes_B30600_fadergroups[3].muteEnabled)
    {
        if ((inst->muteFactorFlags & 0x1C0) != 0 || (flags & 0x4000) == 0)
            result = result * SND_Mutes_B30600_fadergroups[3].muteSetUp1;
        else
            result = result * SND_Mutes_B30600_fadergroups[3].muteSetUp2;
    }
    if (SND_Mutes_B30600_fadergroups[6].muteEnabled)
    {
        if ((flags & 8) != 0)
            result = result * SND_Mutes_B30600_fadergroups[6].muteSetUp2;
        else
            result = result * SND_Mutes_B30600_fadergroups[6].muteSetUp1;
    }
    return (float)result;
}

float SND_f_GetInstVolume(tSoundInstanceElem* inst)
{
    float vol3d;

    if ((SND_gst_Params.flags & 0x40000) != 0)
        return inst->volume;
    vol3d = inst->soundIntf->Compute3DVol(inst);
    return SND_f_MuteComputeFactor(inst) * vol3d * SND_gf_AutoVolumeOff;
}

float SND_f_GetVolFromAtt(int att)
{
    int lo;
    int hi;
    double attHi;

    lo = 0;
    hi = 255;
    if (att >= SND_al_AttTable[0])
        return 1.0;
    if (att <= SND_al_AttTable[255])
        return 0.0;
    do
    {
        if (SND_al_AttTable[(lo + hi) / 2] >= att)
            lo = (lo + hi) / 2;
        else
            hi = (lo + hi) / 2;
    } while (hi - lo > 1);
    attHi = (double)SND_al_AttTable[hi];
    return (float)(SND_af_VolTable[hi] + (SND_af_VolTable[hi] - SND_af_VolTable[lo]) / (attHi - (double)SND_al_AttTable[lo]) * ((double)att - attHi));
}

int SND_l_GetAttFromVol(float vol)
{
    int lo;
    int hi;
    double attHi;
    double rounded;

    lo = 0;
    hi = 255;
    if (vol >= (double)SND_af_VolTable[0])
        return 0;
    if (vol <= (double)SND_af_VolTable[255])
        return -8000;
    do
    {
        if (SND_af_VolTable[(lo + hi) / 2] >= (double)vol)
            lo = (lo + hi) / 2;
        else
            hi = (lo + hi) / 2;
    } while (hi - lo > 1);
    attHi = (double)SND_al_AttTable[hi];
    rounded = (attHi - (double)SND_al_AttTable[lo]) / (SND_af_VolTable[hi] - SND_af_VolTable[lo]) * (vol - SND_af_VolTable[hi]) + attHi + MATH_gd_magicDoubleForRound;
    return *((int32_t *)(&rounded));
}

void SND_SetInstVolume(tSoundInstanceElem* inst)
{
    tSndInterfaceStruct* intf;
    int att;
    float vol;

    vol = SND_f_GetInstVolume(inst);
    intf = inst->soundIntf;
    att = SND_l_GetAttFromVol(vol);
    intf->Setvolume(inst->soundBufElemFromModel, att);
}

void SND_RegisterInactiveGAO(struct sGameObjectMain* gao)
{
    int idx;
    tSoundInstanceElem* inst;
    float vol;

    if (gao)
    {
        if (SND_gst_Params.soundEnabled)
        {
            for (idx = 0; idx < SND_gst_Params.soundInstanceNum; ++idx)
            {
                inst = &SND_gst_Params.soundInstances[idx];
                if ((inst->flags & 0x400) != 0)
                {
                    if (inst->gao)
                    {
                        if (inst->gao == gao)
                        {
                            vol = inst->volume;
                            inst->volume = 0.0;
                            SND_SetInstVolume(inst);
                            inst->volume = vol;
                        }
                    }
                }
            }
        }
    }
}

void SND_l_InitVolumeTable()
{
    int i;
    tMutesSetupStruct* fader;
    double rounded;

    for (i = 0; i < 256; ++i)
    {
        rounded = MATH_gd_magicDoubleForRound - (double)i * 31.372549;
        SND_al_AttTable[i] = *((int32_t*)&rounded);
        SND_af_VolTable[i] = (float) (pow(10.0, (double)(*((int32_t *)&rounded)) * 0.0005));
    }
    fader = SND_Mutes_B30600_fadergroups;
    do
    {
        fader->muteEnabled = 0;
        fader->muteSetUp1 = 1.0;
        fader->muteSetUp2 = 0.5;
        fader->flags = 0;
        fader->elapsedTime = 0.0;
        fader++;
    } while (fader <= &SND_Mutes_B30600_fadergroups[11]);
    SND_Mutes_B30600_fadergroups[7].muteEnabled = 0;
    SND_Mutes_B30600_fadergroups[8].muteEnabled = 0;
    SND_Mutes_B30600_fadergroups[9].muteEnabled = 0;
    SND_Mutes_B30600_fadergroups[10].muteEnabled = 0;
    SND_Mutes_B30600_fadergroups[11].muteEnabled = 0;
    SND_Mutes_B30600_fadergroups[5].muteEnabled = 0;
    SND_Mutes_B30600_fadergroups[7].muteSetUp1 = 1.0;
    SND_Mutes_B30600_fadergroups[7].muteSetUp2 = 1.0;
    SND_Mutes_B30600_fadergroups[8].muteSetUp1 = 1.0;
    SND_Mutes_B30600_fadergroups[8].muteSetUp2 = 1.0;
    SND_Mutes_B30600_fadergroups[9].muteSetUp1 = 1.0;
    SND_Mutes_B30600_fadergroups[9].muteSetUp2 = 1.0;
    SND_Mutes_B30600_fadergroups[10].muteSetUp1 = 1.0;
    SND_Mutes_B30600_fadergroups[10].muteSetUp2 = 1.0;
    SND_Mutes_B30600_fadergroups[11].muteSetUp1 = 1.0;
    SND_Mutes_B30600_fadergroups[11].muteSetUp2 = 1.0;
    SND_Mutes_B30600_fadergroups[5].muteSetUp1 = 1.0;
    SND_Mutes_B30600_fadergroups[5].muteSetUp2 = 1.0;
}

void SND_StreamStopAll()
{
    int idx;
    int instanceIdx;

    if (SND_gst_Params.soundEnabled)
    {
        SND_gpst_Interface.win.ShutdownStreams();
        for (idx = 0; idx < SND_gi_TrackNb; ++idx)  // SND_TrackResetAllMute inline
        {
            SND_gax_TrackPlayingList[idx].unk1 = 0;
            if (SND_gax_TrackPlayingList[idx].flags != 1)
            {
                instanceIdx = SND_gax_TrackPlayingList[idx].trackID;
                if (instanceIdx >= 0)
                    SND_SetInstVolume(&SND_gst_Params.soundInstances[instanceIdx]);
            }
        }
    }
}

void SND_NotifyNewLoading()
{
    SND_gst_LSGlobal.isNewLoading = 1;
}

void SND_StreamResolveSyncPrefetch()
{
    int idx;
    int slot;
    int slotNum;
    tSndPrefetchElem* elem;
    tSndSoundElem* sound;
    unsigned int flags;
    int16_t numChannels;
    int owner;
    int positions[16];
    int sizes[8];

    if (SND_gst_Params.soundEnabled)
    {
        memset(positions, 0, 32u);
        memset(sizes, 0, sizeof(sizes));
        slotNum = 0;
        for (idx = 0; idx < SND_gst_PrefetchManager.num; ++idx)
        {
            elem = &(SND_gst_PrefetchManager.ptr[idx]);
            if (elem->type == 1)
            {
                sound = &SND_gst_Params.allocatedSounds[elem->sndElemIdx];
                flags = sound->flags;
                if ((flags & 0x200000) == 0)
                {
                    if ((flags & 0x4000) == 0 && (flags & 0x2000) == 0 && (flags & 0x80000) == 0
                        || SND_l_AddStreamLoad(sound->soundKey) < 0)
                    {
                        return;
                    }
                }
                --SND_gst_PrefetchManager.sumChannelNums;
                numChannels = sound->numChannels;
                if (SND_gst_PrefetchManager.sumChannelNums + numChannels <= 8)
                {
                    positions[slotNum + 8] = idx;
                    elem->sndIntanceIdx = sound->startDataPos;
                    if (numChannels == 1)
                    {
                        elem->sndInstanceIdx2 = 0;
                        sizes[slotNum] = sound->fullsoundLen;
                        positions[slotNum] = elem->sndIntanceIdx;
                    }
                    else
                    {
                        elem->sndInstanceIdx2 = sound->startDataPos  + (sound->fullsoundLen >> 1) + ((uint16_t)sound->extraDataSize >> 1);
                        sizes[slotNum] = sound->fullsoundLen >> 1;
                        positions[slotNum++] = elem->sndIntanceIdx;
                        sizes[slotNum] = sound->fullsoundLen >> 1;
                        positions[slotNum] = elem->sndInstanceIdx2;
                        positions[slotNum + 8] = idx;
                    }
                    ++slotNum;
                    elem->type = 3;
                    SND_gst_PrefetchManager.sumChannelNums += sound->numChannels;
                }
                else
                {
                    elem->type = 0;
                    elem->sndInstanceIdx2 = 0;
                    elem->sndIntanceIdx = 0;
                    elem->sndElemIdx = -1;
                }
            }
        }
        if (slotNum)
        {
            SND_gpst_Interface.win.StreamPrefetchArray(positions, sizes);
            for (slot = 0; slot < slotNum; ++slot)
            {
                owner = positions[slot + 8];
                elem = &SND_gst_PrefetchManager.ptr[owner];
                if (!positions[slot])
                    continue;
                --SND_gst_PrefetchManager.sumChannelNums;
                elem->type = 0;
                if (elem->sndInstanceIdx2)
                {
                    --SND_gst_PrefetchManager.sumChannelNums;
                    if (!slot)
                    {
                        if (!positions[1])
                            SND_gpst_Interface.win.StreamFlush(elem->sndInstanceIdx2);
                        slot = 1;
                    }
                    else if (slot == slotNum - 1)
                    {
                        if (!positions[slot - 1])
                            SND_gpst_Interface.win.StreamFlush(elem->sndIntanceIdx);
                    }
                    else if (positions[slot + 9] == owner)
                    {
                        if (!positions[slot + 1])
                            SND_gpst_Interface.win.StreamFlush(elem->sndInstanceIdx2);
                        ++slot;
                    }
                    else if (positions[slot + 7] == owner && !positions[slot - 1])
                    {
                        SND_gpst_Interface.win.StreamFlush(elem->sndIntanceIdx);
                    }
                }
                elem->sndInstanceIdx2 = 0;
                elem->sndIntanceIdx = 0;
                elem->sndElemIdx = -1;
            }
        }
    }
}

int SND_l_GetSoundIndex(int soundKEy)
{
    tSndSoundElem* sound;
    int idx;

    sound = SND_gst_Params.allocatedSounds;
    for (idx = 0; idx < SND_gst_Params.allocatedSoundsMaxNum; ++idx, ++sound)
    {
        if ((sound->flags & 0x400) != 0 && sound->soundKey == soundKEy)
            return idx;
    }
    return -1;
}

int SND_i_AllocSound()
{
    int idx;
    tSndSoundElem* grown;
    int oldMaxNum;
    size_t size;

    for (idx = 0; idx < SND_gst_Params.allocatedSoundsMaxNum; ++idx)
    {
        if ((SND_gst_Params.allocatedSounds[idx].flags & 0x400) == 0)
            return idx;
    }
    size = sizeof(tSndSoundElem) * (SND_gst_Params.allocatedSoundsMaxNum + 1000);
    if (SND_gst_Params.allocatedSoundsMaxNum)
        grown = (tSndSoundElem*)MEM_p_Realloc(SND_gst_Params.allocatedSounds, size);
    else
        grown = (tSndSoundElem*)MEM_p_Alloc(size);
    oldMaxNum = SND_gst_Params.allocatedSoundsMaxNum;
    SND_gst_Params.allocatedSounds = grown;
    memset(&grown[SND_gst_Params.allocatedSoundsMaxNum], 0, sizeof(tSndSoundElem) * 1000);
    SND_gst_Params.allocatedSoundsMaxNum += 1000;
    return oldMaxNum;
}

int SND_l_AddSound(int soundKey)
{
    int soundIdx;
    tSndSoundElem* sound;

    if (!SND_gst_Params.soundEnabled)
        return -1;
    if (!soundKey || soundKey == -1)
        return -1;
    soundIdx = SND_l_GetSoundIndex(soundKey);
    if (soundIdx == -1)
    {
        soundIdx = SND_i_AllocSound();
        sound = &SND_gst_Params.allocatedSounds[soundIdx];
        sound->soundKey = soundKey;
        sound->startDataPos = 0;
        sound->fullsoundLen = 0;
        sound->freq = 0;
        sound->numChannels = 0;
        sound->bitsPerSample = 0;
        sound->format = 0;
        sound->byteRate = 0;
        sound->blockAlign = 0;
        sound->extraDataSize = 0;
        sound->sndSndBufElem = 0;
        sound->soundModifier = 0;
        sound->flags = 0x400;
        sound->muteFacorFlags = 64;
        sound->usageCnt = 1;
        sound->sndInterface = (tSndInterfaceStruct*)&SND_gpst_Interface; // TODO FIXME ez jó itt?
        SND_gb_UnloadedFile = 1;
    }
    else
    {
        ++SND_gst_Params.allocatedSounds[soundIdx].usageCnt;
    }
    return soundIdx;
}

int SND_l_AddLoadingSound(int soundKey)
{
    int soundIdx;

    soundIdx = SND_l_AddSound(soundKey);
    if (soundIdx != -1)
        SND_gst_Params.allocatedSounds[soundIdx].flags |= 8u;
    return soundIdx;
}

void SND_StreamPrefetchOneSound(int soundIdx, int mode)
{
    unsigned int flags;
    tSndSoundElem* sound;
    tSndPrefetchElem* slot;
    int idx;
    tSndPrefetchElem* elem;

    if (SND_gst_Params.soundEnabled)
    {
        if (soundIdx >= 0 && soundIdx < SND_gst_Params.allocatedSoundsMaxNum)
        {
            flags = SND_gst_Params.allocatedSounds[soundIdx].flags;
            sound = &SND_gst_Params.allocatedSounds[soundIdx];
            if ((flags & 0x400) != 0 && SND_gst_PrefetchManager.sumChannelNums < 8)
            {
                slot = SND_gst_PrefetchManager.ptr;
                elem = SND_gst_PrefetchManager.ptr;
                for (idx = 0; idx < SND_gst_PrefetchManager.num; ++idx, ++elem)
                {
                    if (elem->type)
                    {
                        if (elem->sndElemIdx == soundIdx)
                            return;
                        flags = SND_gst_Params.allocatedSounds[soundIdx].flags;
                    }
                }
                if (SND_gst_PrefetchManager.num > 0)
                {
                    for (idx = 0; slot->type; ++idx, ++slot)
                    {
                        if (idx + 1 >= SND_gst_PrefetchManager.num)
                            return;
                    }
                    if (mode)
                    {
                        if (mode == 1
                            && (sound->startDataPos != -1
                                || ((flags & 0x2000) != 0 || (flags & 0x4000) != 0 || (flags & 0x80000) != 0)
                                && SND_l_AddStreamLoad(sound->soundKey) >= 0))
                        {
                            if (sound->numChannels == 1)
                            {
                                SND_gpst_Interface.win.StreamPrefetch(sound->startDataPos, sound->fullsoundLen);
                                slot->sndIntanceIdx = sound->startDataPos;
                                slot->sndInstanceIdx2 = 0;
                                ++SND_gst_PrefetchManager.sumChannelNums;
                            }
                            else
                            {
                                SND_gpst_Interface.win.StreamPrefetch(sound->startDataPos, sound->fullsoundLen >> 1);
                                SND_gpst_Interface.win.StreamPrefetch((sound->fullsoundLen >> 1) + sound->startDataPos, sound->fullsoundLen >> 1);
                                slot->sndIntanceIdx = sound->startDataPos;
                                slot->sndInstanceIdx2 = sound->startDataPos + (sound->fullsoundLen >> 1) + ((uint16_t)sound->extraDataSize >> 1);
                                SND_gst_PrefetchManager.sumChannelNums += 2;
                            }
                            slot->type = 2;
                            slot->sndElemIdx = soundIdx;
                        }
                    }
                    else
                    {
                        ++SND_gst_PrefetchManager.sumChannelNums;
                        slot->type = 1;
                        slot->sndElemIdx = soundIdx;
                    }
                }
            }
        }
    }
}

int SND_ResolveStaticPrefetchForSpecialMdF()
{
    int result;
    unsigned int i;
    tSoundMdfStruct* sndMdf;
    int soundIdx;

    result = SND_gst_Params.soundEnabled;
    if (SND_gst_Params.soundEnabled)
    {
        result = SND_gst_BankMdFUserList;
        for (i = 0; i < SND_gst_BankMdFUserList; ++i)
        {
            sndMdf = sndMdfBankUser[i];
            if (sndMdf && (sndMdf->flags & 0x1800) == 0x1800)
            {
                MDF_SndGetSoundIndexFromBank(sndMdfBankUser[i]);
                soundIdx = sndMdf->idx;
                if (soundIdx >= 0)
                    SND_StreamPrefetchOneSound(soundIdx, 0);
            }
            result = SND_gst_BankMdFUserList;
        }
    }
    return result;
}

int SND_SoundListMake(tSndList** list, int* count)
{
    int result;
    int idx;
    tSndSoundElem* sound;
    unsigned int flags;
    tSndList* grown;

    result = SND_gst_Params.allocatedSoundsMaxNum;
    for (idx = 0; idx < SND_gst_Params.allocatedSoundsMaxNum; ++idx)
    {
        sound = &(SND_gst_Params.allocatedSounds[idx]);
        flags = sound->flags;
        if ((flags & 0x400) != 0 && (flags & 1) == 0 && (flags & 0x96000) == 0 && sound->soundKey && sound->soundKey != -1)
        {
            if (*list)
                grown = (tSndList*)MEM_p_Realloc(*list, 8 * *count + 8);
            else
                grown = (tSndList*)MEM_p_Alloc(8u);
            *list = grown;
            (*list)[*count].sndkey = sound->soundKey;
            (*list)[(*count)++].flags = sound->flags & 0x96008;
        }
        result = idx + 1;
    }
    return result;
}

int SND_l_LoadData(int soundKEy)
{
    int soundIdx;
    tSndSoundElem* soundElem;
    tSndSoundBufferElem* buffer;
    tSndWavHdrStruct wavHdr;

    if (!SND_gst_Params.soundEnabled)
        return -1;
    soundIdx = SND_l_GetSoundIndex(soundKEy);
    if (soundIdx == -1)
        return -1;
    soundElem = &SND_gst_Params.allocatedSounds[soundIdx];
    if ((soundElem->flags & 1) == 0)
    {
        wavHdr.wfex.wFormatTag = soundElem->format;
        wavHdr.wfex.nChannels = soundElem->numChannels;
        wavHdr.wfex.nSamplesPerSec = soundElem->freq;
        wavHdr.wfex.nAvgBytesPerSec = soundElem->byteRate;
        wavHdr.wfex.nBlockAlign = soundElem->blockAlign;
        wavHdr.wfex.wBitsPerSample = soundElem->bitsPerSample;
        wavHdr.wfex.cbSize = soundElem->extraDataSize;
        wavHdr.fullSoundSize = soundElem->fullsoundLen;
        wavHdr.dataStartPos = soundElem->startDataPos;
        SND_gst_Params.handles->soundElemFlagsOffset20isHas1 = (soundElem->flags >> 20) & 1;
        SND_gi_RasterCurrentFileId = 0;
        buffer = SND_gpst_Interface.win.Create(SND_gst_Params.handles, &wavHdr, soundElem->startDataPos);
        if (!buffer)
            return -1;
        soundElem->flags = 0x200401;
        soundElem->sndSndBufElem = buffer;
    }
    return soundIdx;
}

int SND_l_LoadHeader(int soundKey)
{
    int soundIdx;
    tSndSoundElem* sndElem;
    unsigned int binPos;
    tSndWavHdrStruct wavHdr;

    memset(&wavHdr, 0, sizeof(tSndWavHdrStruct)); // Ez nincs a játékban

    if (!SND_gst_Params.soundEnabled)
        return -1;
    soundIdx = SND_l_GetSoundIndex(soundKey);
    if (soundIdx != -1)
    {
        sndElem = &SND_gst_Params.allocatedSounds[soundIdx];
        if ((sndElem->flags & 0x200000) != 0)
            return soundIdx;
        if (soundKey)
        {
            if (soundKey != -1)
            {
                binPos = BIG_ul_SearchKeyToPos(soundKey);
                if (SND_i_GetWaveInfo(binPos + 4, &wavHdr, 5) != -1)
                {
                    sndElem->flags |= 0x200000u;
                    sndElem->startDataPos = wavHdr.dataStartPos;
                    sndElem->fullsoundLen = wavHdr.fullSoundSize;
                    sndElem->freq = wavHdr.wfex.nSamplesPerSec;
                    sndElem->numChannels = wavHdr.wfex.nChannels;
                    sndElem->bitsPerSample = wavHdr.wfex.wBitsPerSample;
                    sndElem->format = wavHdr.wfex.wFormatTag;
                    sndElem->byteRate = wavHdr.wfex.nAvgBytesPerSec;
                    sndElem->blockAlign = wavHdr.wfex.nBlockAlign;
                    sndElem->extraDataSize = wavHdr.wfex.cbSize;
                    return soundIdx;
                }
            }
        }
    }
    return -1;
}

int SND_l_LoadLoadingSoundHeader(int soundKey)
{
    int soundIdx;

    soundIdx = SND_l_LoadHeader(soundKey);
    if (soundIdx != -1)
        SND_gst_Params.allocatedSounds[soundIdx].flags |= 8u;
    return soundIdx;
}

void SND_LoadAllSounds()
{
    tSndList* list;
    unsigned int binPos;
    int i;
    int entryFlags;
    int soundIdx;
    unsigned int* flagsPtr;
    int count;
    tSndList* madeList;
    char* cursor;

    if (!SND_gb_AllSoundsLoaded)
    {
        list = 0;
        madeList = 0;
        count = 0;
        if (!LOA_gb_SpeedMode
            || (binPos = BIG_ul_SearchKeyToPos(LOA_ul_BinKey | 0xFF400000), binPos != (LOA_ul_BinKey | 0xFF400000)) && binPos != -1)
        {
            LOA_BeginSpeedMode(0xFF400000);
            if (!LOA_gb_SpeedMode || BIG_gi_ReadMode != 2)
            {
                SND_SoundListMake(&madeList, &count);
                list = madeList;
            }
            if (LOA_gb_SpeedMode)
            {
                if (BIG_gi_ReadMode == 2)
                {
                    cursor = LOA_FetchBuffer(4);
                    count = LOA_ReadLong(&cursor, 0, 1);
                    list = (tSndList*)MEM_p_Alloc(8 * count);
                    memset(list, 0xFFu, 8 * count);
                    cursor = LOA_FetchBuffer(8 * count);
                    for (i = 0; i < count; ++i)
                    {
                        list[i].sndkey = LOA_ReadLong(&cursor, 0, 1);
                        list[i].flags = LOA_ReadLong(&cursor, 0, 1);
                    }
                }
            }
            for (i = 0; i < SND_gst_Params.allocatedSoundsMaxNum; ++i)
            {
                flagsPtr = &SND_gst_Params.allocatedSounds[i].flags;
                if ((*flagsPtr & 0x400) != 0)
                    *flagsPtr &= ~0x200000u;
            }
            for (i = 0; i < count; ++i)
            {
                entryFlags = list[i].flags;
                if (entryFlags == 8)
                    soundIdx = SND_l_AddLoadingSound(list[i].sndkey);
                else if (entryFlags)
                    continue;
                else
                    soundIdx = SND_l_AddSound(list[i].sndkey);
                if (soundIdx != -1)
                {
                    entryFlags = list[i].flags;
                    if (entryFlags)
                    {
                        if (entryFlags == 8)
                            SND_l_LoadLoadingSoundHeader(list[i].sndkey);
                    }
                    else
                    {
                        SND_l_LoadHeader(list[i].sndkey);
                    }
                    --SND_gst_Params.allocatedSounds[soundIdx].usageCnt;
                }
            }
            if (LOA_gb_SpeedMode)
                SND_gul_WavBinSeek = BIG_gul_ReadPos + BIG_gi_ReadSeek + 4;
            for (i = 0; i < count; ++i)
            {
                if (!list[i].flags)
                    SND_l_LoadData(list[i].sndkey);
            }
            if (list)
                MEM_Free(list);
            if (LOA_gb_SpeedMode)
                SND_gul_WavBinSeek = 0;
            LOA_EndSpeedMode();
            SND_gb_AllSoundsLoaded = 1;
        }
    }
}

void SND_FreeHeaderArray(BASStruct* array)
{
    int i;

    for (i = 0; i < array->actElemNum; ++i)
    {
        if (array->keyValPtr[i].val != -1)
            MEM_Free((void*)array->keyValPtr[i].val);
    }
    BAS_bfree(array);
    array->keyValPtr = 0;
    array->actElemNum = 0;
    array->maxElemNum = 0;
    array->elemNumToAddWhenFull = 0;
}

void SND_ReadRIFFHeader(char** cursor, void* hdrDest)
{
    int fmtSize;

    tSoundHeaderDataCase0* thdrDest = (tSoundHeaderDataCase0*)hdrDest;
    tSoundHeaderDataCase1* thdrDest1 = (tSoundHeaderDataCase1*)hdrDest;
    tSoundHeaderDataCase2* thdrDest2 = (tSoundHeaderDataCase2*)hdrDest;

    // itt midnegy mire castolom, mert az összes eleje ugyanaz
    thdrDest->i1 = LOA_ReadLong(cursor, 0, 1);
    thdrDest->i2 = LOA_ReadLong(cursor, 0, 1);
    thdrDest->i3 = LOA_ReadLong(cursor, 0, 1);
    thdrDest->i4 = LOA_ReadLong(cursor, 0, 1);
    fmtSize = LOA_ReadLong(cursor, 0, 1);
    thdrDest->i5 = fmtSize;
    switch (fmtSize)
    {
    case 14:
        thdrDest->is1 = LOA_ReadShort(cursor, 0, 1);
        thdrDest->is2 = LOA_ReadShort(cursor, 0, 1);
        thdrDest->ii1 = LOA_ReadLong(cursor, 0, 1);
        thdrDest->ii2 = LOA_ReadLong(cursor, 0, 1);
        thdrDest->is3 = LOA_ReadShort(cursor, 0, 1);
        thdrDest->ii3 = LOA_ReadLong(cursor, 0, 1);
        thdrDest->ii4 = LOA_ReadLong(cursor, 0, 1);
        break;
    case 16:
        thdrDest1->is1 = LOA_ReadShort(cursor, 0, 1);
        thdrDest1->is2 = LOA_ReadShort(cursor, 0, 1);
        thdrDest1->ii1 = LOA_ReadLong(cursor, 0, 1);
        thdrDest1->ii2 = LOA_ReadLong(cursor, 0, 1);
        thdrDest1->is3 = LOA_ReadShort(cursor, 0, 1);
        thdrDest1->is4 = LOA_ReadShort(cursor, 0, 1);
        thdrDest1->ii3 = LOA_ReadLong(cursor, 0, 1);
        thdrDest1->ii4 = LOA_ReadLong(cursor, 0, 1);
        break;
    case 18:
        thdrDest2->is1 = LOA_ReadShort(cursor, 0, 1);
        thdrDest2->is2 = LOA_ReadShort(cursor, 0, 1);
        thdrDest2->ii1 = LOA_ReadLong(cursor, 0, 1);
        thdrDest2->ii2 = LOA_ReadLong(cursor, 0, 1);
        thdrDest2->is3 = LOA_ReadShort(cursor, 0, 1);
        thdrDest2->is4 = LOA_ReadShort(cursor, 0, 1);
        thdrDest2->is5 = LOA_ReadShort(cursor, 0, 1);
        thdrDest2->ii3 = LOA_ReadLong(cursor, 0, 1);
        thdrDest2->ii4 = LOA_ReadLong(cursor, 0, 1);
        break;
    default:
        break;
    }
}

int SND_ReadSNDHeaderElem(char** cursor, tSoundHeaderGeneric* header)
{
    int result;
    header->soundKey = LOA_ReadLong(cursor, 0, 1);
    SND_ReadRIFFHeader(cursor, header->data);
    result = LOA_ReadLong(cursor, 0, 1);
    header->soundKey1 = result;
    return result;
}

int SND_i_GetWaveInfo(int Offset, tSndWavHdrStruct* wavHdrStructDest, int readMode)
{
    int fileStartOffset;
    int wawHdrRaw;
    int result;
    int headerLen;
    char* fetched;
    char DstBuf[46];

    memset(wavHdrStructDest, 0, sizeof(tSndWavHdrStruct));

    switch (readMode)
    {
    case 1:
    case 5:
        fileStartOffset = Offset;
        wawHdrRaw = (int)DstBuf;
        if (!LOA_gb_SpeedMode || BIG_gi_ReadMode != 2)
            BIG_ReadNoSeek(Offset, DstBuf, 46u);
        if (LOA_gb_SpeedMode && BIG_gi_ReadMode == 2)
        {
            fetched = LOA_FetchBuffer(46);
            SND_ReadRIFFHeader(&fetched, DstBuf);
        }
        break;
    case 4:
        wawHdrRaw = Offset + 4;
        fileStartOffset = BIG_ul_SearchKeyToPos(*(int32_t*)(Offset + 50)) + 4;
        break;
    default:
        return -1;
    }

    tSoundHeaderDataCase0* thdrDest = (tSoundHeaderDataCase0*)wawHdrRaw;
    tSoundHeaderDataCase1* thdrDest1 = (tSoundHeaderDataCase1*)wawHdrRaw;
    tSoundHeaderDataCase2* thdrDest2 = (tSoundHeaderDataCase2*)wawHdrRaw;

    if (memcmp(&(thdrDest->i1), "RIFF", 4)
        || memcmp(&(thdrDest->i4), "fmt ", 4)
        || memcmp(&(thdrDest->i3), "WAVE", 4)
        || thdrDest->i5 < 14u)      // 4 subchunksize
    {
        return -1;
    }
    wavHdrStructDest->wfex.nAvgBytesPerSec = thdrDest->ii2;
    wavHdrStructDest->wfex.nBlockAlign = thdrDest->is3;
    wavHdrStructDest->wfex.nChannels = thdrDest->is2;
    wavHdrStructDest->wfex.nSamplesPerSec = thdrDest->ii1;
    wavHdrStructDest->wfex.wFormatTag = thdrDest->is1;
    if (thdrDest->i5 == 14)
    {
        wavHdrStructDest->wfex.cbSize = 0;
        wavHdrStructDest->wfex.wBitsPerSample = 8 * (thdrDest->ii2 / (unsigned int)thdrDest->is3 / thdrDest->is2);
        if (memcmp(&(thdrDest->ii3), "data", 4))
            return -1;
        wavHdrStructDest->fullSoundSize = thdrDest->ii4;
        headerLen = 42;
    }
    else if (thdrDest1->i5 == 16)
    {
        wavHdrStructDest->wfex.cbSize = 0;
        wavHdrStructDest->wfex.wBitsPerSample = thdrDest1->is4;
        if (memcmp(&(thdrDest->ii3), "data", 4))
            return -1;
        wavHdrStructDest->fullSoundSize = thdrDest1->ii4;
        headerLen = 44;
    }
    else
    {
        if (thdrDest2->i5 != 18)
            DBG_assert(0, __FILE__, __LINE__, "0", "Wave Header format unknown", 0, 1);
        wavHdrStructDest->wfex.cbSize = thdrDest2->is5;
        wavHdrStructDest->wfex.wBitsPerSample = thdrDest2->is4;
        if (memcmp(&(thdrDest2->ii3), "data", 4)) // if (memcmp(&(thdrDest->ii3), "data", 4))
            return -1;
        wavHdrStructDest->fullSoundSize = thdrDest2->ii4;
        headerLen = 46;
    }
    result = fileStartOffset + headerLen;
    if (readMode == 1 || readMode == 4 || readMode == 5)
        wavHdrStructDest->dataStartPos = result;
    return result;
}

int SND_i_LoadStreamedFile(unsigned int soundKey, tSndWavHdrStruct* wavHdrStructDest)
{
    int headerPos;
    unsigned int binPos;

    if (!soundKey || soundKey == -1)
        return -1;
    headerPos = BAS_bsearch(soundKey, &SND_gst_HeaderArray);
    if (headerPos != -1)
        return SND_i_GetWaveInfo(headerPos, wavHdrStructDest, 4);
    binPos = BIG_ul_SearchKeyToPos(soundKey);
    if (binPos == -1 || binPos == soundKey)
        return -1;
    return SND_i_GetWaveInfo(binPos + 4, wavHdrStructDest, 1);
}

int SND_i_StreamAlloc()
{
    tSndStreamList* list;
    tSndStreamList* grown;
    int idx;

    list = SND_gast_StreamList;
    for (idx = 0; idx < SND_gi_StreamListSize; ++idx)
    {
        if (!list[idx].num)
            break;
    }
    if (idx == SND_gi_StreamListSize)
    {
        if (SND_gi_StreamListSize)
        {
            SND_gast_StreamList = (tSndStreamList*)MEM_p_Realloc(SND_gast_StreamList, sizeof(tSndStreamList) * (SND_gi_StreamListSize + 200));
            grown = &SND_gast_StreamList[SND_gi_StreamListSize];
        }
        else
        {
            grown = (tSndStreamList*)MEM_p_Alloc(200 * sizeof(tSndStreamList));
            SND_gast_StreamList = grown;
        }
        memset(grown, 0, (200 * sizeof(tSndStreamList)));
        list = SND_gast_StreamList;
        idx = SND_gi_StreamListSize;
        SND_gi_StreamListSize += 200;
    }
    list[idx].num = 1024;
    return idx + 1;
}

static int SND_l_StreamLoadSound(int soundKey)
{
    int result = 0; // ez sincs egyenlõvé téve 0-val
    int soundIdx;
    tSndSoundElem* sound;
    int dataPos;
    int streamIdx;
    tSndWavHdrStruct wavHdr;

    if (!SND_gst_Params.soundEnabled)
        return -1;
    soundIdx = SND_l_GetSoundIndex(soundKey);
    if (soundIdx == -1)
        return -1;
    sound = &SND_gst_Params.allocatedSounds[soundIdx];
    result = soundIdx;
    if ((sound->flags & 1) != 0)
        return result;
    dataPos = SND_i_LoadStreamedFile(sound->soundKey, &wavHdr);
    sound->startDataPos = dataPos;
    // preserved: two of SND_i_LoadStreamedFile's failure paths return without
    // touching wavHdr, so this reads it uninitialised
    if (wavHdr.wfex.wFormatTag != 2)
        return -1;
    if (dataPos == -1)
        return -1;
    sound->flags |= 1u;
    sound->freq = wavHdr.wfex.nSamplesPerSec;
    sound->numChannels = wavHdr.wfex.nChannels;
    sound->bitsPerSample = wavHdr.wfex.wBitsPerSample;
    sound->format = wavHdr.wfex.wFormatTag;
    sound->byteRate = wavHdr.wfex.nAvgBytesPerSec;
    sound->blockAlign = wavHdr.wfex.nBlockAlign;
    sound->extraDataSize = wavHdr.wfex.cbSize;
    streamIdx = SND_i_StreamAlloc();
    sound->sndSndBufElem = (tSndSoundBufferElem*)streamIdx; // TODO FIXME ez jó típus itt??, sztem jó mert az összes stream snd fv castolt intekkel operál
    sound->fullsoundLen = wavHdr.fullSoundSize;
    if (streamIdx <= 0)
        result = -1;
    return result;
}

int SND_l_AddStreamLoad(int soundKey)
{
    return SND_l_StreamLoadSound(soundKey);
}



void SND_HeaderMakeArray(BASStruct* array)
{
    tSndSoundElem* sounds;
    tSndSoundElem* sound;
    unsigned int flags;
    tSoundHeaderGeneric* header;
    unsigned int binPos;

    SND_FreeHeaderArray(array);
    BAS_binit(array, 200);
    sounds = SND_gst_Params.allocatedSounds;
    sound = SND_gst_Params.allocatedSounds;
    BAS_bsortmode = 0;
    if (SND_gst_Params.allocatedSounds < &SND_gst_Params.allocatedSounds[SND_gst_Params.allocatedSoundsMaxNum])
    {
        do
        {
            flags = sound->flags;
            if ((flags & 0x400) != 0 && sound->soundKey && sound->soundKey != -1 && (flags & 0x86000) != 0)
            {
                header = (tSoundHeaderGeneric*)MEM_p_Alloc(sizeof(tSoundHeaderGeneric));
                memset(header, 0, sizeof(tSoundHeaderGeneric));
                binPos = BIG_ul_SearchKeyToPos(sound->soundKey);
                if (binPos != -1 && binPos != sound->soundKey)
                {
                    BIG_ReadNoSeek(binPos + 4, header->data, 46u);
                    header->soundKey = sound->soundKey;
                    header->soundKey1 = sound->soundKey;
                }
                BAS_binsert(sound->soundKey, (int)header, array);
                sounds = SND_gst_Params.allocatedSounds;
            }
            ++sound;
        } while (sound < &sounds[SND_gst_Params.allocatedSoundsMaxNum]);
    }
    BAS_bsortmode = 1;
    BAS_bsort(array);
}

void SND_HeaderLoadArray(BASStruct* array, int binKey)
{
    int binPos;
    char* data;
    unsigned int i;
    tSoundHeaderGeneric* header;
    unsigned int headerNum;
    char* cursor;

    binPos = BIG_ul_SearchKeyToPos(binKey);
    if (binPos == -1 || binPos == binKey) {
        SND_HeaderMakeArray(array);
        return;
    }
    data = BIG_pc_ReadFileTmp(binPos, &headerNum);
    headerNum /= sizeof(tSoundHeaderGeneric);
    SND_FreeHeaderArray(array);
    BAS_bsortmode = 0;
    BAS_binit(array, 200);
    for (i = 0; i < headerNum; ++i)
    {
        header = (tSoundHeaderGeneric*)MEM_p_Alloc(54u);
        cursor = data;
        SND_ReadSNDHeaderElem(&cursor, header);
        BAS_binsert(header->soundKey, (int)header, array);
        data += sizeof(tSoundHeaderGeneric);
    }
    BAS_bsortmode = 1;
    BAS_bsort(array);
}

void SND_StreamFlushOneSound(tSndPrefetchElem* elem)
{
    int remaining;
    int type;

    if (SND_gst_Params.soundEnabled && SND_gst_PrefetchManager.sumChannelNums && elem->type)
    {
        remaining = --SND_gst_PrefetchManager.sumChannelNums;
        if (elem->sndInstanceIdx2)
            SND_gst_PrefetchManager.sumChannelNums = remaining - 1;
        type = elem->type;
        if (type >= 2 && type <= 3)
        {
            SND_gpst_Interface.win.StreamFlush(elem->sndIntanceIdx);
            if (elem->sndInstanceIdx2)
                SND_gpst_Interface.win.StreamFlush(elem->sndInstanceIdx2);
        }
        elem->type = 0;
        elem->sndInstanceIdx2 = 0;
        elem->sndIntanceIdx = 0;
        elem->sndElemIdx = -1;
    }
}

void SND_StreamPrefetchFlushAll()
{
    int idx;

    if (SND_gst_Params.soundEnabled)
    {
        for (idx = 0; idx < SND_gst_PrefetchManager.num; ++idx)
        {
            if (SND_gst_PrefetchManager.ptr[idx].type)
                SND_StreamFlushOneSound(&SND_gst_PrefetchManager.ptr[idx]);
        }
    }
}

void SND_StreamFlushOneSound_0(int soundIdx, int doFlush)
{
    int idx;
    tSndPrefetchElem* elem;
    int remaining;
    int type;

    if (SND_gst_Params.soundEnabled)
    {
        if (SND_gst_PrefetchManager.sumChannelNums)
        {
            if (soundIdx >= 0
                && soundIdx < SND_gst_Params.allocatedSoundsMaxNum
                && (SND_gst_Params.allocatedSounds[soundIdx].flags & 0x400) != 0)
            {
                elem = SND_gst_PrefetchManager.ptr;
                for (idx = 0; idx < SND_gst_PrefetchManager.num; ++idx, ++elem)
                {
                    if (elem->type && elem->sndElemIdx == soundIdx)
                        break;
                }
                if (idx >= SND_gst_PrefetchManager.num)
                    return;
                remaining = --SND_gst_PrefetchManager.sumChannelNums;
                if (elem->sndInstanceIdx2)
                    SND_gst_PrefetchManager.sumChannelNums = remaining - 1;
                if (doFlush)
                {
                    type = elem->type;
                    if (type >= 2 && type <= 3)
                    {
                        SND_gpst_Interface.win.StreamFlush(elem->sndIntanceIdx);
                        if (elem->sndInstanceIdx2)
                            SND_gpst_Interface.win.StreamFlush(elem->sndInstanceIdx2);
                    }
                }
                elem->type = 0;
                elem->sndInstanceIdx2 = 0;
                elem->sndIntanceIdx = 0;
                elem->sndElemIdx = -1;
            }
        }
    }
}

int SND_SignalChangeLanguageToMdF()
{
    int result;
    unsigned int i;
    tSoundMdfStruct* sndMdf;

    result = SND_gst_Params.soundEnabled;
    if (SND_gst_Params.soundEnabled)
    {
        result = SND_gst_BankMdFUserList;
        for (i = 0; i < SND_gst_BankMdFUserList; ++i)
        {
            sndMdf = sndMdfBankUser[i];
            if (sndMdf)
            {
                SND_StreamFlushOneSound_0(sndMdf->idx, 1);
                if ((sndMdf->flags & 0x800) != 0)
                {
                    sndMdf->sndInstanceID = -1;
                    sndMdf->idx = -1;
                }
            }
            result = SND_gst_BankMdFUserList;
        }
    }
    return result;
}

tSndBank* SND_ul_CallbackLoadMetaBank(int Offset)
{
    char* data;
    char* dataEnd;
    int bankKey;
    unsigned int bankLangSpecificAlternativeKey;
    int key;
    int bankPos;
    tSndBank* bank;
    unsigned int idx;
    tSndMetaBank* slots;
    int langIdx;
    char* cursor;
    int size;
    int lang;

    lang = TEXT_i_GetLang();
    data = BIG_pc_ReadFileTmp(Offset, &size);
    dataEnd = &data[size];
    bankKey = 0;
    bankLangSpecificAlternativeKey = 0;
    cursor = data;
    langIdx = 0;
    if (data < dataEnd)
    {
        do
        {
            key = LOA_ReadLong(&cursor, 0, 1);
            LOA_ReadLong(&cursor, 0, 2);
            if (key && key != -1)
            {
                if (!bankKey)
                    bankKey = key;
                if (langIdx == lang)
                    bankLangSpecificAlternativeKey = key;
                ++langIdx;
            }
        } while (cursor < dataEnd);
    }
    if (!bankLangSpecificAlternativeKey)
        bankLangSpecificAlternativeKey = bankKey;
    bankPos = BIG_ul_SearchKeyToPos(bankLangSpecificAlternativeKey);
    bank = SND_ul_CallbackLoadBank(bankPos);
    if (!SND_gst_Params.soundEnabled)
        return bank;
    for (idx = 0; idx < SND_gul_MetaBankUserNb; ++idx)
    {
        if (SND_gax_MetaBankUserList[idx].bank == bank)
            return bank;
    }
    for (idx = 0; idx < SND_gul_MetaBankUserNb; ++idx)
    {
        if (!SND_gax_MetaBankUserList[idx].bank)
        {
            SND_gax_MetaBankUserList[idx].bank = bank;
            SND_gax_MetaBankUserList[idx].offset = Offset;
            return bank;
        }
    }
    if (!SND_gul_MetaBankUserNb)
    {
        slots = (tSndMetaBank*)MEM_p_Alloc(128u);
        SND_gax_MetaBankUserList = slots;
    }
    else
    {
        SND_gax_MetaBankUserList = (tSndMetaBank*)MEM_p_Realloc(
            SND_gax_MetaBankUserList,
            sizeof(tSndMetaBank) * SND_gul_MetaBankUserNb + (16 * sizeof(tSndMetaBank)));
        slots = &SND_gax_MetaBankUserList[SND_gul_MetaBankUserNb];
    }
    memset(slots, 0, (16 * sizeof(tSndMetaBank)));
    SND_gax_MetaBankUserList[SND_gul_MetaBankUserNb].bank = bank;
    SND_gax_MetaBankUserList[SND_gul_MetaBankUserNb].offset = Offset;
    SND_gul_MetaBankUserNb += 16;
    return bank;
}

unsigned int SND_UpdateLang()
{
    unsigned int result;
    int soundIdx;
    tSndSoundElem* sound;
    unsigned int flags;
    unsigned int idx;
    tSndBank* bank;
    int i;
    tSndBank* reloaded;

    SND_StreamPrefetchFlushAll();
    SND_SignalChangeLanguageToMdF();
    result = SND_gst_Params.soundEnabled;         // SND_MetaBankReloadAll inline vegig
    if (SND_gst_Params.soundEnabled)
    {
        result = SND_gul_MetaBankUserNb;
        if (SND_gul_MetaBankUserNb)
        {
            if (LOA_gb_SpeedMode)
            {
                LOA_BeginSpeedMode(LOA_ul_BinKey & 0xFFFFF | ((TEXT_gst_Global.language + 1) << 20) | 0xFE000000);
                SND_HeaderLoadArray(&SND_gst_HeaderArray, LOA_ul_BinKey);
                LOA_EndSpeedMode();
                result = SND_gst_Params.allocatedSoundsMaxNum;
                for (soundIdx = 0; soundIdx < SND_gst_Params.allocatedSoundsMaxNum; ++soundIdx)
                {
                    sound = &(SND_gst_Params.allocatedSounds[soundIdx]);
                    flags = sound->flags;
                    if ((flags & 0x400) != 0 && (flags & 0x4000) != 0 && (flags & 1) != 0)
                    {
                        SND_DetachSound(soundIdx);
                        if (sound->soundModifier)
                        {
                            SND_SModifierDeleteAllLinkedSound(sound->soundModifier);
                            SND_SModifierDeleteAll(&sound->soundModifier);
                        }
                        else
                        {
                            sound->sndInterface->Stop(sound->sndSndBufElem);
                            sound->sndInterface->Release(sound->sndSndBufElem);
                        }
                        flags = sound->flags & ~1u;
                        sound->sndInterface = &SND_gpst_Interface.dialog;
                        sound->sndSndBufElem = 0;
                        sound->flags = flags;
                    }
                    result = SND_gst_Params.allocatedSoundsMaxNum;
                }
            }
            else
            {
                for (idx = 0; idx < result; ++idx)
                {
                    bank = SND_gax_MetaBankUserList[idx].bank;
                    if (bank)
                    {
                        if (bank->soundIndices)
                        {
                            for (i = 0; i < bank->elemNum; ++i)
                            {
                                if (bank->soundIndices[i] != -1)
                                    SND_DeleteByIndex(bank->soundIndices[i]);
                            }
                            MEM_Free(bank->soundIndices);
                        }
                        reloaded = SND_ul_CallbackLoadMetaBank(SND_gax_MetaBankUserList[idx].offset);
                        bank->elemNum = reloaded->elemNum;
                        bank->soundIndices = reloaded->soundIndices;
                        SND_MetaBankUserRemove(reloaded);
                        MEM_Free(reloaded);
                        result = SND_gul_MetaBankUserNb;
                    }
                }
            }
        }
    }
    return result;
}

int SND_l_AddStreamNoLoad(int soundKey)
{
    int soundIdx;
    tSndSoundElem* sound;

    if (!SND_gst_Params.soundEnabled)
        return -1;
    soundIdx = SND_l_GetSoundIndex(soundKey);
    if (soundIdx == -1)
    {
        soundIdx = SND_i_AllocSound();
        sound = &SND_gst_Params.allocatedSounds[soundIdx];
        sound->soundKey = soundKey;
        sound->muteFacorFlags = 0;
        sound->freq = 0;
        sound->fullsoundLen = 0;
        sound->numChannels = 0;
        sound->bitsPerSample = 0;
        sound->sndSndBufElem = 0;
        sound->soundModifier = 0;
        sound->startDataPos = 0;
        sound->flags = 0x80020400;
        sound->usageCnt = 1;
        sound->sndInterface = &SND_gpst_Interface.stream;
    }
    else
    {
        ++SND_gst_Params.allocatedSounds[soundIdx].usageCnt;
    }
    return soundIdx;
}

int SND_l_AddDialogNoLoad(int soundKey)
{
    int soundIdx;
    tSndSoundElem* sound;

    soundIdx = SND_l_AddStreamNoLoad(soundKey);
    if (soundIdx != -1)
    {
        sound = &SND_gst_Params.allocatedSounds[soundIdx];
        if (sound->usageCnt == 1)
        {
            sound->flags |= 0x4000u;
            sound->sndInterface = &SND_gpst_Interface.dialog;
        }
    }
    return soundIdx;
}

int SND_l_AddMusicNoLoad(int soundKey)
{
    int soundIdx;
    tSndSoundElem* sound;

    soundIdx = SND_l_AddStreamNoLoad(soundKey);
    if (soundIdx != -1)
    {
        sound = &SND_gst_Params.allocatedSounds[soundIdx];
        if (sound->usageCnt == 1)
        {
            sound->flags |= 0x2000u;
            sound->muteFacorFlags |= 0x100u;
            sound->sndInterface = &SND_gpst_Interface.music;
        }
    }
    return soundIdx;
}

int SND_l_AddAmbienceNoLoad(int soundKey)
{
    int soundIdx;
    tSndSoundElem* sound;

    soundIdx = SND_l_AddStreamNoLoad(soundKey);
    if (soundIdx != -1)
    {
        sound = &SND_gst_Params.allocatedSounds[soundIdx];
        if (sound->usageCnt == 1)
        {
            sound->muteFacorFlags |= 0x80u;
            sound->flags |= 0x80000u;
            sound->sndInterface = &SND_gpst_Interface.ambience;
        }
    }
    return soundIdx;
}

tSndBank* SND_ul_CallbackLoadBank(int Offset)
{
    char* data;
    unsigned int elemNum;
    tSndBank* bank;
    int* indices;
    unsigned int elemIdx;
    int soundKey;
    int soundIdx;
    unsigned int i;
    char String1[4];
    char* cursor;
    int size;

    data = BIG_pc_ReadFileTmp(Offset, &size);
    elemNum = (unsigned int)size >> 3;
    cursor = data;
    if (!elemNum)
        return 0;
    bank = (tSndBank*)MEM_p_Alloc(sizeof(tSndBank));
    bank->usageCount = 0;
    bank->elemNum = 0;
    bank->bankKey = 0;
    bank->soundIndices = 0;
    bank->soundIndicesV2 = 0;
    bank->usageCount = 0;
    if (LOA_gb_SpeedMode && BIG_gi_ReadMode == 2)
        elemNum = LOA_ReadLong(&cursor, 0, 1);
    bank->elemNum = elemNum;
    bank->bankKey = LOA_ul_GetCurrentKey();
    indices = (int*)MEM_p_Alloc(4 * elemNum);
    bank->soundIndices = indices;
    if (indices)
        memset(indices, 0xFFu, 4 * elemNum);
    for (elemIdx = 0; elemIdx < elemNum; ++elemIdx)
    {
        soundKey = LOA_ReadLong(&cursor, 0, 1);
        LOA_ReadCharArray(&cursor, String1, 4u, 1);
        if (!soundKey || soundKey == -1)
            continue;
        if (!_strnicmp(String1, ".wad", 4u))
            soundIdx = SND_l_AddDialogNoLoad(soundKey);
        else if (!_strnicmp(String1, ".wam", 4u))
            soundIdx = SND_l_AddMusicNoLoad(soundKey);
        else if (!_strnicmp(String1, ".waa", 4u))
            soundIdx = SND_l_AddAmbienceNoLoad(soundKey);
        else if (!_strnicmp(String1, ".smd", 4u))
            soundIdx = SND_l_AddSModifierNoLoad(soundKey, &cursor);
        else if (!_strnicmp(String1, ".wac", 4u))
            soundIdx = SND_l_AddLoadingSound(soundKey);
        else if (!_strnicmp(String1, ".wav", 4u))
            soundIdx = SND_l_AddSound(soundKey);
        else
        {
            for (i = 0; i < elemIdx; ++i)
                SND_DeleteByIndex(bank->soundIndices[i]);
            MEM_Free(bank);
            return 0;
        }
        bank->soundIndices[elemIdx] = soundIdx;
    }
    return bank;
}

void SND_DelMdFBankUser(struct sSoundMdfStruct* sndMdf)
{
    unsigned int idx;

    if (SND_gst_Params.soundEnabled)
    {
        if (sndMdf)
        {
            for (idx = 0; idx < SND_gst_BankMdFUserList; ++idx)
            {
                if (sndMdfBankUser[idx] == sndMdf)
                    sndMdfBankUser[idx] = 0;
            }
        }
    }
}

static void SND_FamillyInnerRelease(tSoundFamilyInnerStruct* inner)
{
    if (inner->i1 == 1)
        MEM_Free(inner);
}

void SND_LoadingSoundCloseModule()
{
    int instanceIdx;

    if (SND_gst_LSGlobal.soundInstanceIndex != -1)
    {
        instanceIdx = SND_gst_LSGlobal.soundInstanceIndex;
        SND_gst_LSGlobal.soundInstanceIndex = -1;
        SND_Release(instanceIdx);
    }
    if (SND_gst_LSGlobal.soundElemIndex != -1)
    {
        SND_gst_LSGlobal.soundElemIndex = -1;
        // preserved: the index is cleared first, so this always deletes nothing
        SND_DeleteByIndex(-1);
    }
    SND_gpst_Interface.win.Release(SND_gst_LSGlobal.soundElemElem);
    SND_gst_LSGlobal.soundElemElem = 0;
}

void SND_RasterCloseModule()
{
    BAS_bfree(&rasterbas1);
    BAS_bfree(&rasterbas2);
    memset(&sndRasterStat, 0, sizeof(sndRasterStat));
    SND_AllocatedSoundsNumWithhoutFlag80000000 = 0;
    SND_AllocatedSoundsNumWithFlag80000000 = 0;
    SND_gi_RasterCurrentFileId = 0;
    SND_AllocatedSoundsNumWithhoutFlag80000000PeakUsage = 0;
    SND_AllocatedSoundsNumWithFlag80000000PeakUsage = 0;
}

void SND_SModifierCloseModule()
{
    MEM_Free(SND_gs_SoundModifiers[0].linkedSoundData);
    MEM_Free(SND_gs_SoundModifiers[0].sndModifierModelIntf);
    MEM_Free(SND_gs_SoundModifiers[1].linkedSoundData);
    MEM_Free(SND_gs_SoundModifiers[1].sndModifierModelIntf);
    MEM_Free(SND_gs_SoundModifiers[3].sndModifierModelIntf);
    MEM_Free(SND_gs_SoundModifiers[4].linkedSoundData);
    MEM_Free(SND_gs_SoundModifiers[4].sndModifierModelIntf);
    MEM_Free(SND_gs_SoundModifiers[2].linkedSoundData);
    MEM_Free(SND_gs_SoundModifiers[2].sndModifierModelIntf);
    MEM_Free(SND_gs_SoundModifiers[5].linkedSoundData);
    MEM_Free(SND_gs_SoundModifiers[5].sndModifierModelIntf);
    MEM_Free(SND_gs_SoundModifiers[6].linkedSoundData);
    MEM_Free(SND_gs_SoundModifiers[6].sndModifierModelIntf);
    memset(SND_gs_SoundModifiers, 0, sizeof(SND_gs_SoundModifiers));
}

void SND_FxCloseModule()
{
    if (SND_gst_Params.soundEnabled)
    {
        SND_gpst_Interface.win.FxClose();
        memset(&SND_gst_FxParam, 0, sizeof(SND_gst_FxParam));
        memset(&SND_gst_FxDefaultParam, 0, sizeof(SND_gst_FxDefaultParam));
        SND_gst_FxParam.mode = 0;
        SND_gst_FxDefaultParam.mode = 0;
    }
}

void SND_FamillyCloseModule()
{
    int i;
    tSoundFamilyStruct* familly;
    int j;

    if (SND_gast_AndFamillyList)
    {
        for (i = 0; i < SND_gi_AndFamillyNb; ++i)
        {
            familly = &SND_gast_AndFamillyList[i];
            for (j = 0; j < familly->innerNum; ++j)
                SND_FamillyInnerRelease(familly->unkIntArrPtr[j]);
            MEM_Free(familly->unkIntArrPtr);
        }
        MEM_Free(SND_gast_AndFamillyList);
    }
    SND_gast_AndFamillyList = 0;
    SND_gi_AndFamillyListSize = 0;
    SND_gi_AndFamillyNb = 0;
}

void SND_CloseMdFModule()
{
    if (SND_gst_Params.soundEnabled)
    {
        if (sndMdfBankUser)
            MEM_Free(sndMdfBankUser);
        sndMdfBankUser = 0;
    }
}

void SND_MetaBankFreeList()
{
    SND_gul_MetaBankUserNb = 0;
    if (SND_gax_MetaBankUserList)
    {
        MEM_Free(SND_gax_MetaBankUserList);
        SND_gax_MetaBankUserList = 0;
    }
}

void SND_TrackCloseModule()
{
    tTrackResourceElem* resource;

    BAS_bfree(&SND_gst_TrackId);
    SND_gst_TrackId.keyValPtr = 0;
    BAS_bfree(&SND_gst_TrackLookUpTable);
    SND_gst_TrackLookUpTable.keyValPtr = 0;
    resource = SND_gast_TrackResources;
    do
    {
        SND_gpst_Interface.win.ReleaseTrack(resource->stereoSoundBuffer, resource->monoSoundBuffer);
        ++resource;
    } while (resource <= &SND_gast_TrackResources[3]);
}

void SND_StreamCloseModule()
{
    if (SND_gast_StreamList)
        MEM_Free(SND_gast_StreamList);
    SND_gast_StreamList = 0;
    SND_gi_StreamListSize = 0;
    if (SND_gst_PrefetchManager.ptr)
        MEM_Free(SND_gst_PrefetchManager.ptr);
    SND_gst_PrefetchManager.ptr = 0;
    SND_gst_PrefetchManager.num = 0;
    SND_gst_PrefetchManager.sumChannelNums = 0;
}

void SND_CloseModule()
{
    if (SND_gst_Params.soundEnabled)
    {
        SND_FreeHeaderArray(&SND_gst_HeaderArray);
        SND_LoadingSoundCloseModule();
        SND_RasterCloseModule();
        SND_SModifierCloseModule();
        SND_FxCloseModule();
        SND_FamillyCloseModule();
        SND_CloseMdFModule();
        SND_MetaBankFreeList();
        if (SND_gst_Params.handles)
        {
            SND_TrackCloseModule();
            SND_StreamCloseModule();
            SND_gpst_Interface.win.Close(SND_gst_Params.handles);
        }
        SND_gst_Params.soundEnabled = 0;
    }
    if (SND_gst_Params.handles)
        MEM_Free(SND_gst_Params.handles);
    SND_gst_Params.handles = 0;
    if (SND_gst_Params.soundInstances)
    {
        MEM_Free(SND_gst_Params.soundInstances);
        SND_gst_Params.soundInstances = 0;
        SND_gst_Params.soundInstanceNum = 0;
        SND_gst_Params.soundInstanceMaxNum = 0;
    }
    if (SND_gst_Params.allocatedSounds)
    {
        MEM_Free(SND_gst_Params.allocatedSounds);
        SND_gst_Params.allocatedSounds = 0;
        SND_gst_Params.allocatedSoundsMaxNum = 0;
    }
    memset(&SND_gpst_Interface, 0, sizeof(SND_gpst_Interface));
}

int SND_InitSoundSystem()
{
    if (SND_gpst_Interface.win.Init(SND_gst_Params.handles))
        return 0;
    SND_CloseModule();
    return -1;
}

int SND_i_TrackNew()
{
    tTrackListElem* grown;
    int idx;
    int oldTrackNb;

    for (idx = 0; idx < SND_gi_TrackNb; ++idx)
    {
        if (SND_gax_TrackPlayingList[idx].flags == 1)
            break;
    }
    if (idx == SND_gi_TrackNb)
    {
        if (SND_gi_TrackNb)
        {
            SND_gax_TrackPlayingList = (tTrackListElem*)MEM_p_Realloc(SND_gax_TrackPlayingList, sizeof(tTrackListElem) * (SND_gi_TrackNb + 16));
            grown = &SND_gax_TrackPlayingList[SND_gi_TrackNb];
        }
        else
        {
            grown = (tTrackListElem*)MEM_p_Alloc(sizeof(tTrackListElem) * 16);
            SND_gax_TrackPlayingList = grown;
        }
        memset(grown, 0, sizeof(tTrackListElem) * 16);
        oldTrackNb = SND_gi_TrackNb;
        for (idx = oldTrackNb; idx < oldTrackNb + 16; ++idx) // __OFSUB__
            SND_gax_TrackPlayingList[idx].flags = 1;
        idx = oldTrackNb;
        SND_gi_TrackNb = oldTrackNb + 16;
    }
    SND_gax_TrackPlayingList[idx].trackID = -1;
    SND_gax_TrackPlayingList[idx].soundIntf = 0;
    SND_gax_TrackPlayingList[idx].flags = 1;
    SND_gax_TrackPlayingList[idx].sndBufferElem = 0;
    SND_gax_TrackPlayingList[idx].trackResource = 0;
    SND_gax_TrackPlayingList[idx].trackResourceNum = 0;
    SND_gax_TrackPlayingList[idx].stereoToMono_sub499C20 = 0;
    SND_gax_TrackPlayingList[idx].unk1 = 0;
    SND_gax_TrackPlayingList[idx].f2 = 1.0f;
    SND_gax_TrackPlayingList[idx].f1 = 0.5f;
    return idx;
}

static void SND_TrackAcquireStereoResource(int trackIdx)
{
    tTrackListElem* track;
    tTrackResourceElem* resource;
    int resourceIdx;

    track = &SND_gax_TrackPlayingList[trackIdx];
    for (;;)
    {
        resourceIdx = 0;
        resource = SND_gast_TrackResources;
        while (resource->isBusy && resource < &SND_gast_TrackResources[3])
        {
            ++resource;
            ++resourceIdx;
        }
        if (!resource->isBusy)
            break;
        SND_TrackForceFreeResources(trackIdx);
    }
    track->stereoToMono_sub499C20 = resourceIdx + 1;
    SND_gast_TrackResources[resourceIdx].isBusy = 1;
    SND_gpst_Interface.win.StreamGetMonoToStereo(&track->trackResource, &SND_gast_TrackResources[resourceIdx].stereoSoundBuffer);
}

static void SND_TrackSwitchToStereo(int trackIdx)
{
    tTrackListElem* track;
    tSndSoundBufferElem* buffer;

    track = &SND_gax_TrackPlayingList[trackIdx];
    SND_TrackAcquireStereoResource(trackIdx);
    if (track->stereoToMono_sub499C20)
    {
        buffer = track->trackResource;
        track->trackResource = track->sndBufferElem;
        track->sndBufferElem = buffer;
        track->flags |= 4;
    }
}

void SND_Track_Merge(int trackIdx, tSoundInstanceElem* previous, tSoundInstanceElem* inst, int flags, int from, int to)
{
    tTrackListElem* trackPlayingList;
    int16_t numChannels;
    tSndSoundElem* sound;
    int att;
    float vol;
    int status;

    if (previous)
    {
        SND_gpst_Interface.win.GetStatus(SND_gax_TrackPlayingList[trackIdx].sndBufferElem, &status);
        if ((status & 1) != 0)
            SND_gpst_Interface.win.Stop(SND_gax_TrackPlayingList[trackIdx].sndBufferElem);
    }
    trackPlayingList = SND_gax_TrackPlayingList;
    if (!SND_gax_TrackPlayingList[trackIdx].trackResourceNum)
    {
        SND_TrackAcquireResource(trackIdx);
        trackPlayingList = SND_gax_TrackPlayingList;
    }
    numChannels = SND_gst_Params.allocatedSounds[inst->allocatedSoundsIdx].numChannels;
    if (numChannels != 2 || (trackPlayingList[trackIdx].flags & 4) != 0)
    {
        if (numChannels == 1 && (trackPlayingList[trackIdx].flags & 4) != 0)
            SND_TrackSwitchToMono(trackIdx);
    }
    else
    {
        SND_TrackSwitchToStereo(trackIdx);
    }
    SND_StreamFlushOneSound_0(inst->allocatedSoundsIdx, 0);
    sound = &SND_gst_Params.allocatedSounds[inst->allocatedSoundsIdx];
    vol = SND_f_GetInstVolume(inst);
    att = SND_l_GetAttFromVol(vol);
    SND_gpst_Interface.win.StreamReinitAndPlay(SND_gax_TrackPlayingList[trackIdx].sndBufferElem, sound->fullsoundLen, sound->startDataPos, from,
                                                to, flags, inst->freq, inst->pan, att);
}

void SND_TrackRegisterAndPlayInstance(int idx, int flags, int from, int to)
{
    int instFlags;
    tSoundInstanceElem* sndInst;
    tSoundInstanceElem* previous;
    int trackIdx;
    int previousIdx;
    unsigned int newFlags;

    if (idx >= 0 && idx < SND_gst_Params.soundInstanceNum)
    {
        instFlags = SND_gst_Params.soundInstances[idx].flags;
        sndInst = &SND_gst_Params.soundInstances[idx];
        if ((instFlags & 0x400) != 0)
        {
            previous = 0;
            if ((instFlags & 0x86000) != 0)
            {
                trackIdx = BAS_bsearch(sndInst->trackID, &SND_gst_TrackId);
                if (trackIdx == -1)
                {
                    trackIdx = SND_i_TrackNew();
                    SND_gax_TrackPlayingList[trackIdx].flags = 0;
                    sndInst->trackID = SND_gi_TrackLastVirtualId++;
                    BAS_binsert(sndInst->trackID, trackIdx, &SND_gst_TrackId);
                }
                else if ((SND_gax_TrackPlayingList[trackIdx].flags & 2) != 0)
                {
                    previousIdx = SND_gax_TrackPlayingList[trackIdx].trackID;
                    previous = &SND_gst_Params.soundInstances[previousIdx];
                    previous->flags = SND_gst_Params.soundInstances[previousIdx].flags & ~0x808010u;
                    previous->soundIntf = SND_gax_TrackPlayingList[trackIdx].soundIntf;
                    BAS_bdelete((unsigned int)previous->soundBufElemFromModel, &SND_gst_TrackLookUpTable);
                }
                SND_gax_TrackPlayingList[trackIdx].trackID = idx;
                SND_gax_TrackPlayingList[trackIdx].soundIntf = sndInst->soundIntf;
                SND_gax_TrackPlayingList[trackIdx].flags |= 2u;
                BAS_binsert((unsigned int)sndInst->soundBufElemFromModel, trackIdx, &SND_gst_TrackLookUpTable);
                SND_Track_Merge(trackIdx, previous, sndInst, flags, from, to);
                newFlags = sndInst->flags & ~0x828010u;
                sndInst->soundIntf = &SND_gpst_Interface.track;
                if ((flags & 1) != 0)
                    sndInst->flags = newFlags | 0x820000;
                else
                    sndInst->flags = newFlags | 0x28000;
            }
        }
    }
}

void SND_SModifierNotifyPlayInstance(tSoundInstanceElem* inst)
{
    tSndModifierElem* i;
    void(*notify)(tSndModifierElem*, tSoundInstanceElem*);

    for (i = inst->modifierInstances; i; i = i->nextModifier)
    {
        notify = i->sndModifierModelIntf->NotifyPlay;
        if (notify)
            notify(i, inst);
    }
}

void SND_PlayRq(unsigned int from, tSoundInstanceElem* sie, int flags, int to)
{
    int muteFlags;
    int freq;
    int soundIdx;
    double totalBytes;
    tSndSoundElem* sound;
    BOOL negativeRate;
    double bytesPerSecond;
    tSoundInstanceElem* victim;
    double offsetBytes;
    int instFlags;
    tSndInterfaceStruct* intf;
    int idx;
    tSoundInstanceElem* inst;
    int wasOnTrack;
    int started;
    double rounded;
    int status;
    float duration;

    if ((SND_gst_Params.flags & 0x10000) == 0)
    {
        muteFlags = sie->muteFactorFlags & ~0x1000u;
        sie->muteFactorFlags = muteFlags;
        if ((muteFlags & 0x800) != 0)
        {
            freq = sie->freq;
            if (freq)
            {
                soundIdx = sie->allocatedSoundsIdx;
                totalBytes = (double)SND_gst_Params.allocatedSounds[soundIdx].fullsoundLen;
                sound = &SND_gst_Params.allocatedSounds[soundIdx];
                negativeRate = ((freq * sound->numChannels) & 0x40000000) != 0;
                bytesPerSecond = (double)(2 * freq * sound->numChannels);
            }
            else
            {
                soundIdx = sie->allocatedSoundsIdx;
                totalBytes = (double)SND_gst_Params.allocatedSounds[soundIdx].fullsoundLen;
                bytesPerSecond = (double)(int)SND_gst_Params.allocatedSounds[soundIdx].byteRate;
                negativeRate = (SND_gst_Params.allocatedSounds[soundIdx].byteRate & 0x80000000) != 0;
            }
            if (negativeRate)
                bytesPerSecond = bytesPerSecond + 4294967300.0;
            duration = (float)(totalBytes / bytesPerSecond);
            sie->timeFromDefault = (float)((double)rand() * 0.000030518509 * duration);
        }
        victim = 0;
        if (!from && sie->timeFromDefault != 0.0)
        {
            soundIdx = sie->allocatedSoundsIdx;
            offsetBytes = (double)(2 * sie->freq * SND_gst_Params.allocatedSounds[soundIdx].numChannels);
            if (((sie->freq * SND_gst_Params.allocatedSounds[soundIdx].numChannels) & 0x40000000) != 0)
                offsetBytes = offsetBytes + 4294967300.0;
            rounded = offsetBytes * sie->timeFromDefault + MATH_gd_magicDoubleForRound;
            from = (*(uint32_t*)(&rounded)) & ~3u;
        }
        if ((sie->flags & 0x100000) != 0)
            flags |= 0x100000u;
        SND_SModifierNotifyPlayInstance(sie);
        if ((flags & 0x20000000) != 0)
        {
            sie->timeRemainTillStart = 0.0;
        }
        else if (sie->timeRemainTillStart != 0.0)
        {
            return;
        }
        if ((flags & 0x8000000) != 0 || (instFlags = sie->flags, (instFlags & 0x86000) != 0))
        {
            sie->requestTimeStamp = TIM_gf_MainClock;
            SND_TrackRegisterAndPlayInstance(((unsigned int)((char*)sie - (char*)SND_gst_Params.soundInstances) / sizeof(tSoundInstanceElem)),         // elsõ az idx
                flags,
                from,
                to);
        }
        else
        {
            intf = sie->soundIntf;
            sie->flags = instFlags & ~0x20000u;
            sie->requestTimeStamp = TIM_gf_MainClock;
            started = intf->Play(sie->soundBufElemFromModel, flags) >= 0;
            if (!started)
            {
                for (idx = 0; idx < SND_gst_Params.soundInstanceNum; ++idx)
                {
                    instFlags = SND_gst_Params.soundInstances[idx].flags;
                    inst = &SND_gst_Params.soundInstances[idx];
                    if ((instFlags & 0x400) != 0 && (instFlags & 0x82000) == 0)
                    {
                        inst->soundIntf->GetStatus(inst->soundBufElemFromModel, &status);
                        if ((status & 4) == 0
                            && (status & 1) != 0
                            && (!victim || inst->requestTimeStamp < (double)victim->requestTimeStamp))
                        {
                            victim = inst;
                        }
                    }
                }
                if (!victim)
                {
                    for (idx = 0; idx < SND_gst_Params.soundInstanceNum; ++idx)
                    {
                        instFlags = SND_gst_Params.soundInstances[idx].flags;
                        inst = &SND_gst_Params.soundInstances[idx];
                        if ((instFlags & 0x400) != 0 && (instFlags & 0x82000) == 0)
                        {
                            inst->soundIntf->GetStatus(inst->soundBufElemFromModel, &status);
                            if ((status & 1) != 0 && (!victim || inst->requestTimeStamp < (double)victim->requestTimeStamp))
                                victim = inst;
                        }
                    }
                }
                if (victim)
                {
                    victim->soundIntf->Stop(victim->soundBufElemFromModel);
                    wasOnTrack = (victim->flags & 0x20000) != 0;
                    victim->flags &= ~0x808000u;
                    if (wasOnTrack)
                        SND_TrackDeleteInstanceFromList((unsigned int)((char*)victim - (char*)SND_gst_Params.soundInstances) / sizeof(tSoundInstanceElem));
                    intf = sie->soundIntf;
                    sie->requestTimeStamp = TIM_gf_MainClock;
                    started = intf->Play(sie->soundBufElemFromModel, flags) >= 0;
                }
            }
            if (started)
            {
                if ((flags & 1) != 0)
                    sie->flags |= 0x800000;
                else
                    sie->flags |= 0x8000;
            }
        }
    }
}

int SND_b_SndIsInActiveVolume(int instanceIdx)
{
    tSoundInstanceElem* inst;
    int flags;
    Mtx4x4* mtx;
    int axis;
    int axisFlag;
    double projection;
    double dotYZ;
    double dotX;
    Vec3* micPos;
    Vec3* sndPos;
    Vec3 dest;
    int axisState[3];

    if (!SND_gst_Params.sndMatrixRef)
        return 1;
    if (instanceIdx >= 0 && instanceIdx < SND_gst_Params.soundInstanceNum)
    {
        inst = &SND_gst_Params.soundInstances[instanceIdx];
        flags = inst->flags;
        if ((flags & 0x400) != 0 && (flags & 0x17000000) != 0)
        {
            if (inst->sndPositionSetInSetPosPtr)
            {
                mtx = inst->sndPosMtxSetInSetPos;
                if (mtx || (flags & 0x7000000) == 0)
                {
                    sndPos = inst->sndPositionSetInSetPosPtr;
                    micPos = &SND_gst_Params.sndMatrixRef->Pos;
                    if ((flags & 0x10000000) != 0)
                        return inst->dtFar + inst->farArr[0] > MATH_DistanceOfTwoPoints(micPos, sndPos);
                    axisState[0] = 0x1000000;
                    axisState[1] = 0x2000000;
                    axisState[2] = 0x4000000;
                    MATH_SubstractVec(&dest, micPos, sndPos);
                    for (axis = 0; axis < 3; ++axis)
                    {
                        axisFlag = axisState[axis];
                        if ((axisFlag & flags) == 0)
                        {
                            axisState[axis] = 2;
                            continue;
                        }
                        if (axis == 1)
                        {
                            dotYZ = mtx->v2.z * dest.z + mtx->v2.y * dest.y;
                            dotX = dest.x * mtx->v2.x;
                            projection = fabs(dotYZ + dotX);
                        }
                        else if (axis == 2)
                        {
                            dotYZ = mtx->v3.z * dest.z + mtx->v3.y * dest.y;
                            dotX = dest.x * mtx->v3.x;
                            projection = fabs(dotYZ + dotX);
                        }
                        else
                        {
                            dotYZ = mtx->v1.z * dest.z + mtx->v1.y * dest.y;
                            dotX = dest.x * mtx->v1.x;
                            projection = fabs(dotYZ + dotX);
                        }
                        if (projection >= inst->farArr[axis] + inst->dtFar)
                            axisState[axis] = 0;
                        else
                            axisState[axis] = axisFlag | 2;
                    }
                    return (uint8_t)(axisState[2] & axisState[1] & (axisState[0] & 0xff) & 2) >> 1; //return (uint8_t)(v14[2] & v14[1] & LOBYTE(v14[0]) & 2) >> 1;
                }
            }
        }
    }
    return 0;
}

void SND_PlaySound(int instanceIdx)
{
    tSoundInstanceElem* sndInst;
    double remaining;
    int status;

    if (instanceIdx >= 0 && instanceIdx < SND_gst_Params.soundInstanceNum)
    {
        sndInst = &SND_gst_Params.soundInstances[instanceIdx];
        if ((sndInst->flags & 0x400) != 0)
        {
            if ((sndInst->muteFactorFlags & 0x1000) != 0
                || (sndInst->soundIntf->GetStatus(sndInst->soundBufElemFromModel, &status),
                    (status & 1) == 0))
            {
                remaining = sndInst->timeRemainTillStart;
                sndInst->flags = sndInst->flags & ~4u | 0x200;
                if (remaining == 0.0)
                {
                    if ((sndInst->flags & 0x17000000) == 0 || SND_b_SndIsInActiveVolume(instanceIdx))
                    {
                        SND_SetInstVolume(sndInst);
                        SND_PlayRq(0, sndInst, 0, -1);
                    }
                }
            }
        }
    }
}

void SND_PlaySoundLooping(int instanceIdx)
{
    int flags;
    tSoundInstanceElem* inst;
    int status;

    if (instanceIdx >= 0 && instanceIdx < SND_gst_Params.soundInstanceNum)
    {
        flags = SND_gst_Params.soundInstances[instanceIdx].flags;
        inst = &SND_gst_Params.soundInstances[instanceIdx];
        if ((flags & 0x400) != 0)
        {
            inst->flags = flags & ~0x80u | 0x204;
            if ((inst->muteFactorFlags & 0x1000) != 0
                || (inst->soundIntf->GetStatus(inst->soundBufElemFromModel, &status),
                    (status & 1) == 0))
            {
                SND_SetInstVolume(inst);
                SND_PlayRq(0, inst, 1, -1);
            }
        }
    }
}

void SND_PlaySoundOnTrack(int instanceIdx)
{
    tSoundInstanceElem* inst;
    double remaining;
    int status;

    if (instanceIdx >= 0 && instanceIdx < SND_gst_Params.soundInstanceNum)
    {
        inst = &SND_gst_Params.soundInstances[instanceIdx];
        if ((inst->flags & 0x400) != 0)
        {
            if ((inst->muteFactorFlags & 0x1000) != 0
                || (inst->soundIntf->GetStatus(inst->soundBufElemFromModel, &status),
                    (status & 1) == 0))
            {
                remaining = inst->timeRemainTillStart;
                inst->flags = inst->flags & 0xFFFFFFFB | 0x200;
                if (remaining == 0.0)
                {
                    SND_SetInstVolume(inst);
                    SND_PlayRq(0, inst, 0x8000000, -1);
                }
            }
        }
    }
}

void SND_PlaySoundLoopingOnTrack(int instanceIdx)
{
    tSoundInstanceElem* inst;
    int flags;
    int status;

    if (instanceIdx >= 0 && instanceIdx < SND_gst_Params.soundInstanceNum)
    {
        inst = &SND_gst_Params.soundInstances[instanceIdx];
        flags = inst->flags;
        if ((flags & 0x400) != 0)
        {
            inst->flags = flags | 0x204;
            if ((inst->muteFactorFlags & 0x1000) != 0
                || (inst->soundIntf->GetStatus(inst->soundBufElemFromModel, &status),
                    (status & 1) == 0))
            {
                SND_SetInstVolume(inst);
                SND_PlayRq(0, inst, 0x8000001, -1);
            }
        }
    }
}

int SND_i_InstanceSetf(int instanceIdx, int type, float value)
{
    tSoundInstanceElem* inst;
    int result;

    if (instanceIdx < 0)
        return -1;
    if (instanceIdx >= SND_gst_Params.soundInstanceNum)
        return -1;
    inst = &SND_gst_Params.soundInstances[instanceIdx];
    if ((inst->flags & 0x400) == 0)
        return -1;
    switch (type)
    {
    case 4099:
        inst->centerArr[0] = value;
        result = 0;
        break;
    case 4100:
        inst->centerArr[1] = value;
        result = 0;
        break;
    case 4101:
        inst->centerArr[2] = value;
        result = 0;
        break;
    case 4102:
        inst->farCoefficient = value;
        result = 0;
        break;
    case 4103:
        inst->middleCoefficient = value;
        result = 0;
        break;
    case 4104:
        inst->dtFar = value;
        result = 0;
        break;
    case 4105:
        inst->minPan = value;
        result = 0;
        break;
    case 4106:
        inst->nearArr[0] = value;
        result = 0;
        break;
    case 4107:
        inst->farArr[0] = value;
        result = 0;
        break;
    case 4108:
        inst->nearArr[1] = value;
        result = 0;
        break;
    case 4109:
        inst->farArr[1] = value;
        result = 0;
        break;
    case 4110:
        inst->nearArr[2] = value;
        result = 0;
        break;
    case 4111:
        inst->farArr[2] = value;
        result = 0;
        break;
    case 4114:
        inst->timeRemainTillStart = value;
        result = 0;
        break;
    case 4117:
        inst->timeFromDefault = value;
        result = 0;
        break;
    default:
        return -1;
    }
    return result;
}

void SND_SetFlags(struct sGameObjectMain* gao, int idx, int flags)
{
    tSoundInstanceElem* inst;
    int instFlags;
    int muteFlags;

    if (idx >= 0 && idx < SND_gst_Params.soundInstanceNum)
    {
        inst = &SND_gst_Params.soundInstances[idx];
        instFlags = inst->flags;
        if ((instFlags & 0x400) != 0)
        {
            if ((flags & 0x40000000) != 0)
            {
                if ((flags & 0x1C0) != 0)
                    inst->muteFactorFlags &= ~0x1C0u;
                muteFlags = flags & 0xFCB | inst->muteFactorFlags;
                inst->muteFactorFlags = muteFlags;
                if ((muteFlags & 0x40) != 0)
                {
                    inst->unkMuteSetupSeeInitVolTable = &SND_Mutes_B30600_fadergroups[7];
                }
                else if ((muteFlags & 0x80u) == 0)
                {
                    if ((muteFlags & 0x100) != 0)
                        inst->unkMuteSetupSeeInitVolTable = &SND_Mutes_B30600_fadergroups[9];
                    else
                        inst->unkMuteSetupSeeInitVolTable = &SND_Mutes_B30600_fadergroups[5];
                }
                else
                {
                    inst->unkMuteSetupSeeInitVolTable = &SND_Mutes_B30600_fadergroups[8];
                }
            }
            else
            {
                inst->flags = flags | instFlags;
            }
        }
    }
}

int SND_i_CheckOrInitGAO(struct sGameObjectMain* gao)
{
    tSndBank* bank;

    if (!gao || ((uint8_t)gao & 3) != 0 || MEM_b_CheckBlockHasBeenFreed(gao) || (gao->flags & 0x8000000) != 0)
        return 1;
    OBJ_GameObject_CreateExtendedIfNot(gao);
    bank = (tSndBank*)MEM_p_Alloc(sizeof(tSndBank));
    bank->usageCount = 0;
    bank->elemNum = 0;
    bank->bankKey = 0;
    bank->soundIndices = 0;
    bank->soundIndicesV2 = 0;
    bank->usageCount = 1;
    bank->bankKey = -1;
    bank->elemNum = 0;
    bank->soundIndices = 0;
    bank->soundIndicesV2 = 0;
    gao->extended->snd = bank;
    gao->flags |= 0x8000000u;
    return 0;
}

void SND_SetPos(struct sGameObjectMain* gao, int instanceIdx, Mtx4x4* mtx)
{
    tSoundInstanceElem* inst;

    if (instanceIdx >= 0 && instanceIdx < SND_gst_Params.soundInstanceNum)
    {
        inst = &SND_gst_Params.soundInstances[instanceIdx];
        if ((inst->flags & 0x400) != 0)
        {
            inst->sndPosMtxSetInSetPos = mtx;
            inst->sndPositionSetInSetPosPtr = &mtx->Pos;
        }
    }
}

void SND_SetPan(int idx, int pan, int front)
{
    tSoundInstanceElem* ie;

    if (idx >= 0 && idx < SND_gst_Params.soundInstanceNum)
    {
        ie = &SND_gst_Params.soundInstances[idx];
        if ((ie->flags & 0x400) != 0)
        {
            ie->soundIntf->SetPan(ie->soundBufElemFromModel, pan, front);
            ie->front = front;
            ie->pan = pan;
        }
    }
}

void SND_SetVol(int instanceIdx, float vol)
{
    tSoundInstanceElem* inst;

    if (vol >= 0.0000001)
    {
        if (vol > 1.0)
            vol = 1.0;
    }
    else
    {
        vol = 0.0;
    }
    if (instanceIdx >= 0 && instanceIdx < SND_gst_Params.soundInstanceNum)
    {
        inst = &SND_gst_Params.soundInstances[instanceIdx];
        if ((inst->flags & 0x400) != 0)
        {
            inst->volume = vol;
            SND_SetInstVolume(inst);
        }
    }
}

void SND_SetNearFar(int idx, int type, float near1, float far1)
{
    tSoundInstanceElem* inst;
    BOOL centerIsDefault; // zf

    if (idx >= 0 && idx < SND_gst_Params.soundInstanceNum)
    {
        inst = &SND_gst_Params.soundInstances[idx];
        if ((inst->flags & 0x400) != 0)
        {
            switch (type)
            {
            case 4096:
                inst->nearArr[0] = near1;
                centerIsDefault = (*((uint32_t*)(&inst->centerArr[0]))) == 0x42340000;
                inst->farArr[0] = far1;
                if (centerIsDefault)
                    inst->centerArr[0] = (float)((near1 + far1) * 0.5);
                break;
            case 4097:
                inst->nearArr[1] = near1;
                centerIsDefault = (*((uint32_t*)(&inst->centerArr[1]))) == 0x42340000;
                inst->farArr[1] = far1;
                if (centerIsDefault)
                    inst->centerArr[1] = (float)((near1 + far1) * 0.5);
                break;
            case 4098:
                inst->nearArr[2] = near1;
                centerIsDefault = (*((uint32_t*)(&inst->centerArr[2]))) == 0x42340000;
                inst->farArr[2] = far1;
                if (centerIsDefault)
                    inst->centerArr[2] = (float)((near1 + far1) * 0.5);
                break;
            }
        }
    }
}

void SND_SModifierNotifyPauseInstance(tSoundInstanceElem* inst)
{
    tSndModifierElem* i;
    void(__cdecl * notify)(tSndModifierElem*, tSoundInstanceElem*);

    for (i = inst->modifierInstances; i; i = i->nextModifier)
    {
        notify = i->sndModifierModelIntf->NotifyPause;
        if (notify)
            notify(i, inst);
    }
}

void SND_Pause(int instanceIdx)
{
    tSoundInstanceElem* inst;

    if (instanceIdx >= 0 && instanceIdx < SND_gst_Params.soundInstanceNum)
    {
        inst = &SND_gst_Params.soundInstances[instanceIdx];
        if ((inst->flags & 0x400) != 0)
        {
            SND_SModifierNotifyPauseInstance(inst);
            inst->soundIntf->Pause(inst->soundBufElemFromModel);
        }
    }
}

void SND_SetFreq(int instanceIdx, int freq)
{
    tSoundInstanceElem* inst;

    if ((SND_gst_Params.flags & 0x80000) == 0 && instanceIdx >= 0 && instanceIdx < SND_gst_Params.soundInstanceNum)
    {
        inst = &SND_gst_Params.soundInstances[instanceIdx];
        if ((inst->flags & 0x400) != 0)
        {
            inst->soundIntf->SetFrequency(inst->soundBufElemFromModel, freq);
            inst->freq = freq;
        }
    }
}

void SND_EngineSyncMdF(tSoundMdfStruct* sndMdf, int sync)
{
    int instanceIdx;
    tSoundInstanceElem* inst;
    tSoundInstanceElem* instAlias;
    int soundIdx;
    double bytesPerSecond;
    double rounded;
    int totalPos;
    int currPos;
    int32_t offsetBytes;
    double freqRounded;

    if (SND_gst_Params.soundEnabled)
    {
        instanceIdx = sndMdf->sndInstanceID;
        if (instanceIdx >= 0 && (sndMdf->flags & 0x10000) != 0 && ENG_gb_EVERunning && EVE_gpst_CurrentParam->currTime <= 1.1)
        {
            if (EVE_gpst_CurrentParam->currTime <= 0.80000001)
            {
                if (instanceIdx < SND_gst_Params.soundInstanceNum)
                {
                    inst = &SND_gst_Params.soundInstances[instanceIdx];
                    instAlias = inst;
                    if ((inst->flags & 0x400) != 0)
                    {
                        if (sync)
                        {
                            inst->soundIntf->GetCurrPos(inst->soundBufElemFromModel, &currPos, &totalPos);
                            if (currPos)
                            {
                                soundIdx = instAlias->allocatedSoundsIdx;
                                bytesPerSecond = (double)(2 * SND_gst_Params.allocatedSounds[soundIdx].numChannels);
                                if ((SND_gst_Params.allocatedSounds[soundIdx].numChannels & 0x40000000) != 0)
                                    bytesPerSecond = bytesPerSecond + 4294967300.0;
                                rounded = bytesPerSecond * sndMdf->freq * EVE_gpst_CurrentParam->currTime + MATH_gd_magicDoubleForRound; // TODO FIXME ezek biztos jók?
                                offsetBytes = *((int32_t*)&rounded);
                                freqRounded = (double)offsetBytes / (double)currPos * sndMdf->freq + MATH_gd_magicDoubleForRound;
                                SND_SetFreq(sndMdf->sndInstanceID, *((int32_t*)&freqRounded));
                                SND_SetVol(sndMdf->sndInstanceID, 0.0);
                            }
                        }
                        else
                        {
                            sndMdf->freq = (float)inst->freq;
                        }
                    }
                }
            }
            else
            {
                freqRounded = sndMdf->freq + MATH_gd_magicDoubleForRound;
                SND_SetFreq(instanceIdx, *((int32_t*)&freqRounded));
                SND_SetVol(sndMdf->sndInstanceID, sndMdf->volume);
            }
        }
    }
}

void SND_PlayMdF(struct sSoundMdfStruct* sndMdf)
{
    int flags;
    int newFlags;
    int flags2;

    if (SND_gst_Params.soundEnabled && sndMdf->sndInstanceID >= 0)
    {
        flags = sndMdf->flags;
        switch (flags & 0x84)
        {
        case 4:
            if ((flags & 0x10000) != 0)
                SND_EngineSyncMdF(sndMdf, 1);
            break;
        case 0x84:
            SND_Pause(sndMdf->sndInstanceID);
            sndMdf->flags &= ~0x80u;
            break;
        case 0:
        case 0x80:
            if ((flags & 0x84) == 0x80)
                sndMdf->flags = flags & ~0x80u;
            newFlags = sndMdf->flags | 4;
            sndMdf->flags = newFlags;
            if ((newFlags & 0x10000) != 0)
                SND_EngineSyncMdF(sndMdf, 0);
            if (sndMdf->ii2 == 0.0)
                SND_SetVol(sndMdf->sndInstanceID, sndMdf->volume);
            if (sndMdf->timeRemainTillStart != 0.0)
                SND_i_InstanceSetf(sndMdf->sndInstanceID, 4114, sndMdf->timeRemainTillStart);
            flags2 = sndMdf->flags2;
            if ((flags2 & 0x20000) != 0)
            {
                if ((flags2 & 4) != 0)
                    SND_PlaySoundLoopingOnTrack(sndMdf->sndInstanceID);
                else
                    SND_PlaySoundOnTrack(sndMdf->sndInstanceID);
            }
            else if ((flags2 & 4) != 0)
            {
                SND_PlaySoundLooping(sndMdf->sndInstanceID);
            }
            else
            {
                SND_PlaySound(sndMdf->sndInstanceID);
            }
            break;
        default:
            return;
        }
    }
}

void SND_ReinitLoadingInstance()
{
    int instanceIdx;

    if (SND_gst_LSGlobal.soundInstanceIndex != -1)
    {
        instanceIdx = SND_gst_LSGlobal.soundInstanceIndex;
        SND_gst_LSGlobal.soundInstanceIndex = -1;
        if (SND_gst_LSGlobal.userCallBackFunction)
            SND_gst_LSGlobal.userCallBackFunction(SND_gst_LSGlobal.userCallBackFunctionParam);
        SND_gst_LSGlobal.userCallBackFunction = 0;
        SND_gst_LSGlobal.userCallBackFunctionParam = 0;
        SND_Release(instanceIdx);
    }
}

int SND_l_LoadLoadingSoundData(int soundKEy)
{
    int soundIdx;
    tSndSoundElem* sound;
    int oldSoundIdx;
    tSndSoundBufferElem* buffer;
    int oldSoundIdxAlias;
    tSndWavHdrStruct wavHdr;

    if (!SND_gst_Params.soundEnabled)
        return -1;
    soundIdx = SND_l_GetSoundIndex(soundKEy);
    if (soundIdx == -1)
        return -1;
    if (SND_gst_LSGlobal.soundElemIndex != soundIdx)
    {
        sound = &SND_gst_Params.allocatedSounds[soundIdx];
        if ((sound->flags & 1) == 0)
        {
            if (SND_gst_LSGlobal.soundElemIndex != -1)
            {
                SND_ReinitLoadingInstance();
                oldSoundIdx = SND_gst_LSGlobal.soundElemIndex;
                oldSoundIdxAlias = SND_gst_LSGlobal.soundElemIndex;
                SND_gst_LSGlobal.soundElemIndex = -1;
                SND_DetachSound(oldSoundIdxAlias);
                SND_gst_Params.allocatedSounds[oldSoundIdx].flags &= ~0x41u;
            }
            wavHdr.wfex.wFormatTag = sound->format;
            wavHdr.wfex.nChannels = sound->numChannels;
            wavHdr.wfex.nSamplesPerSec = sound->freq;
            wavHdr.wfex.nAvgBytesPerSec = sound->byteRate;
            wavHdr.wfex.nBlockAlign = sound->blockAlign;
            wavHdr.wfex.wBitsPerSample = sound->bitsPerSample;
            wavHdr.wfex.cbSize = sound->extraDataSize;
            wavHdr.fullSoundSize = sound->fullsoundLen;
            wavHdr.dataStartPos = sound->startDataPos;
            SND_gi_RasterCurrentFileId = 8;
            buffer = SND_gpst_Interface.win.Reload(SND_gst_Params.handles, SND_gst_LSGlobal.soundElemElem, &wavHdr, sound->startDataPos);
            sound->sndSndBufElem = buffer;
            if (!buffer)
                return -1;
            SND_gst_LSGlobal.soundElemElem = buffer;
            SND_gst_LSGlobal.soundElemIndex = soundIdx;
            if (sound->format != 2)
                return -1;
            sound->flags = 0x200409;
        }
    }
    return soundIdx;
}

void SND_l_AddSModifierLoad(int soundKey)
{
    int soundIdx; // eax
    tSndSoundElem* sound; // ebx
    tSndModifierElem* i; // edi
    tLinkedSoundFade* linked; // esi
    int linkedSoundIdx; // ecx
    int linkedFlags; // eax
    int loadResult; // eax
    int soundIdxSaved; // [esp+4h] [ebp-4h]

    if (SND_gst_Params.soundEnabled)
    {
        if (soundKey)
        {
            if (soundKey != -1)
            {
                soundIdx = SND_l_GetSoundIndex(soundKey);
                if (soundIdx != -1)
                {
                    soundIdxSaved = soundIdx;
                    sound = &SND_gst_Params.allocatedSounds[soundIdx];
                    for (i = sound->soundModifier; i; i = i->nextModifier)
                    {
                        linked = (tLinkedSoundFade*)i->linkedSoundData;
                        linkedSoundIdx = linked->sndIndexInAllocatedSounds;
                        if (linkedSoundIdx != -1)
                        {
                            if ((SND_gst_Params.allocatedSounds[linkedSoundIdx].flags & 1) == 0)
                            {
                                linkedFlags = linked->flags;                 // 4000 = dialog, 2000 = music, 80000 = ambience
                                if ((linkedFlags & 0x4000) != 0 || (linkedFlags & 0x2000) != 0 || (linkedFlags & 0x80000) != 0)
                                {
                                    loadResult = SND_l_AddStreamLoad(linked->sndKey);
                                }
                                else if ((linkedFlags & 8) != 0)
                                {
                                    SND_l_LoadLoadingSoundHeader(linked->sndKey);
                                    loadResult = SND_l_LoadLoadingSoundData(linked->sndKey);
                                }
                                else
                                {
                                    SND_l_LoadHeader(linked->sndKey);
                                    loadResult = SND_l_LoadData(linked->sndKey);
                                }
                                if (loadResult == -1)
                                {
                                    SND_DeleteByIndex(linked->sndIndexInAllocatedSounds);
                                    linked->sndIndexInAllocatedSounds = -1;
                                }
                                sound = &SND_gst_Params.allocatedSounds[soundIdxSaved];
                            }
                            sound->sndSndBufElem = (tSndSoundBufferElem*)linked->sndIndexInAllocatedSounds;
                            SND_gs_SoundModifiers[i->sndModifierModelIntf->Type].sndModifierModelIntf->PlayApplyOnSound(i, i->linkedSoundData, i->linkedSoundDataSize, sound);
                        }
                    }
                    sound->flags |= 1u;
                }
            }
        }
    }
}

int SND_l_GetUnusedInstance()
{
    int instanceNum;
    int idx;
    tSoundInstanceElem* grown;
    int oldMaxNum;

    for (idx = 0; idx < SND_gst_Params.soundInstanceNum; ++idx)
    {
        if ((SND_gst_Params.soundInstances[idx].flags & 0x400) == 0)
            return idx;
    }
    instanceNum = SND_gst_Params.soundInstanceNum;
    oldMaxNum = SND_gst_Params.soundInstanceMaxNum;
    if (SND_gst_Params.soundInstanceNum >= SND_gst_Params.soundInstanceMaxNum)
    {
        if (SND_gst_Params.soundInstanceMaxNum)
        {
            SND_gst_Params.soundInstanceMaxNum += 200;
            grown = (tSoundInstanceElem*)MEM_p_Realloc(SND_gst_Params.soundInstances, sizeof(tSoundInstanceElem) * (oldMaxNum + 200));
        }
        else
        {
            SND_gst_Params.soundInstanceMaxNum = 200;
            grown = (tSoundInstanceElem*)MEM_p_Alloc(sizeof(tSoundInstanceElem) * 200);
        }
        SND_gst_Params.soundInstances = grown;
        memset(&grown[SND_gst_Params.soundInstanceMaxNum - 200], 0, sizeof(tSoundInstanceElem) * 200);
        instanceNum = SND_gst_Params.soundInstanceNum;
    }
    SND_gst_Params.soundInstanceNum = instanceNum + 1;
    return instanceNum;
}

void SND_SetTrack(int instanceIdx, unsigned int trackID)
{
    int flags;
    tSoundInstanceElem* inst;
    int newFlags;
    int trackIdx;

    if (instanceIdx >= 0 && instanceIdx < SND_gst_Params.soundInstanceNum)
    {
        flags = SND_gst_Params.soundInstances[instanceIdx].flags;
        inst = &SND_gst_Params.soundInstances[instanceIdx];
        if ((flags & 0x400) != 0 && trackID != -1 && trackID != inst->trackID)
        {
            if ((flags & 0x20000) != 0)
                SND_TrackDeleteInstanceFromList(instanceIdx);
            newFlags = inst->flags | 0x20000;
            inst->trackID = trackID;
            inst->flags = newFlags;
            if (BAS_bsearch(trackID, &SND_gst_TrackId) == -1)
            {
                trackIdx = SND_i_TrackNew();
                SND_gax_TrackPlayingList[trackIdx].flags = 0;
                BAS_binsert(trackID, trackIdx, &SND_gst_TrackId);
            }
        }
    }
}

int SND_l_Request(struct sGameObjectMain* gao, int soundIdx)
{
    tGameObjectExtended* extended;
    tSndBank* bank;
    int* indices;
    unsigned int flags;
    tSndSoundElem* sound; 
    unsigned int loadedFlags; 
    int modifierInstanceIdx;
    tSoundInstanceElem* modifierInst;
    int newInstanceIdx;
    int withFlagCount;
    BOOL isNewPeak;
    int withoutFlagCount; 
    tSoundInstanceElem* inst;
    int muteFlags;
    int instanceIdx;

    if (SND_gc_NoSound)
        return -1;
    if ((SND_gst_Params.flags & 0x20000) != 0)
        return -1;
    if (gao)
    {
        if ((gao->flags & 0x8000000) == 0)
            return -1;
        extended = gao->extended;
        if (!extended)
            return -1;
        bank = extended->snd;
        if (!bank)
            return -1;
        indices = bank->soundIndices;
        if (!indices || !SND_gst_Params.soundEnabled || soundIdx < 0 || soundIdx >= bank->elemNum)
            return -1;
        soundIdx = indices[soundIdx];
    }
    if (soundIdx == -1 || soundIdx < 0 || soundIdx >= SND_gst_Params.allocatedSoundsMaxNum)
        return -1;
    flags = SND_gst_Params.allocatedSounds[soundIdx].flags;
    sound = &SND_gst_Params.allocatedSounds[soundIdx];
    if ((flags & 1) == 0)
    {
        if ((flags & 0x10000) != 0)
        {
            SND_l_AddSModifierLoad(sound->soundKey);
        }
        else if ((flags & 0x4000) != 0 || (flags & 0x2000) != 0 || (flags & 0x80000) != 0)
        {
            SND_l_AddStreamLoad(sound->soundKey);
        }
        else if ((flags & 8) != 0)
        {
            SND_l_LoadLoadingSoundHeader(sound->soundKey);
            SND_l_LoadLoadingSoundData(sound->soundKey);
        }
        else
        {
            SND_l_LoadHeader(sound->soundKey);
            SND_l_LoadData(sound->soundKey);
        }
    }
    loadedFlags = sound->flags;
    if ((loadedFlags & 0x400) == 0 || (loadedFlags & 1) == 0)
        return -1;
    if (sound->soundModifier)
    {
        if (sound->sndSndBufElem != (tSndSoundBufferElem*)-1)
        {
            modifierInstanceIdx = SND_l_Request(0, (int)sound->sndSndBufElem);
            if (modifierInstanceIdx != -1)
            {
                modifierInst = &SND_gst_Params.soundInstances[modifierInstanceIdx];
                modifierInst->gao = gao;
                SND_SModifierInstanciate(sound->soundModifier, modifierInst);
                SND_SetInstVolume(modifierInst);
                modifierInst->soundIntf->SetFrequency(modifierInst->soundBufElemFromModel, modifierInst->freq);
            }
            return modifierInstanceIdx;
        }
        return -1;
    }
    newInstanceIdx = SND_l_GetUnusedInstance();
    instanceIdx = newInstanceIdx;
    if (newInstanceIdx == -1)
        return -1;
    if ((sound->flags & 0x80000000) == 0)
    {
        withoutFlagCount = SND_AllocatedSoundsNumWithhoutFlag80000000 + 1;
        isNewPeak = SND_AllocatedSoundsNumWithhoutFlag80000000PeakUsage < ++SND_AllocatedSoundsNumWithhoutFlag80000000;
        if (isNewPeak)
            SND_AllocatedSoundsNumWithhoutFlag80000000PeakUsage = withoutFlagCount;
    }
    else
    {
        withFlagCount = SND_AllocatedSoundsNumWithFlag80000000 + 1;
        isNewPeak = SND_AllocatedSoundsNumWithFlag80000000PeakUsage < ++SND_AllocatedSoundsNumWithFlag80000000;
        if (isNewPeak)
            SND_AllocatedSoundsNumWithFlag80000000PeakUsage = withFlagCount;
    }
    inst = &SND_gst_Params.soundInstances[newInstanceIdx];
    inst->allocatedSoundsIdx = soundIdx;
    inst->nearArr[0] = 10.0;
    inst->nearArr[1] = 10.0;
    inst->nearArr[2] = 10.0;
    inst->dtFar = 2.0;
    inst->middleCoefficient = 0.5;
    inst->farCoefficient = 0.0;
    inst->sndPosMtxSetInSetPos = 0;
    inst->sndPositionSetInSetPosPtr = 0;
    inst->farArr[0] = 80.0;
    inst->farArr[1] = 80.0;
    inst->farArr[2] = 80.0;
    inst->centerArr[0] = 45.0;
    inst->centerArr[1] = 45.0;
    inst->centerArr[2] = 45.0;
    inst->sndInstPosVecPrevious.x = 0.0f;
    inst->sndInstPosVecPrevious.y = 0.0f;
    inst->sndInstPosVecPrevious.z = 0.0f;
    inst->sndInstPosVec.x = 0.0;
    inst->sndInstPosVec.y = 0.0;
    inst->sndInstPosVec.z = 0.0;
    inst->flags = sound->flags & 0x80186408 | 0x400;
    inst->muteFactorFlags = sound->muteFacorFlags & 0xFCB;
    inst->gao = gao;
    inst->freq = sound->freq;
    inst->soundIntf = sound->sndInterface;
    inst->trackID = -1;
    inst->requestTimeStamp = 0.0;
    inst->volume = 1.0;
    inst->front = 0;
    inst->pan = 0;
    inst->minPan = 0.0;
    inst->modifierInstances = 0;
    inst->familyBitmask = 0;
    inst->timeRemainTillStart = 0.0;
    inst->timeFromDefault = 0.0;
    muteFlags = sound->muteFacorFlags;
    if ((muteFlags & 0x40) != 0)
    {
        inst->unkMuteSetupSeeInitVolTable = &SND_Mutes_B30600_fadergroups[7];
    }
    else if ((muteFlags & 0x80u) == 0)
    {
        if ((muteFlags & 0x100) != 0)
            inst->unkMuteSetupSeeInitVolTable = &SND_Mutes_B30600_fadergroups[9];
        else
            inst->unkMuteSetupSeeInitVolTable = &SND_Mutes_B30600_fadergroups[5];
    }
    else
    {
        inst->unkMuteSetupSeeInitVolTable = &SND_Mutes_B30600_fadergroups[8];
    }
    if ((sound->flags & 0x40) != 0)
    {
        SND_gst_Params.handles->soundElem = sound;
        sound->sndInterface->Duplicate(SND_gst_Params.handles, sound->sndSndBufElem, &inst->soundBufElemFromModel);
        if (!inst->soundBufElemFromModel)
        {
            SND_FreeInstance(instanceIdx);
            return -1;
        }
        inst->flags |= 0x40u;
        SND_SetInstVolume(inst);
    }
    else
    {
        inst->soundBufElemFromModel = sound->sndSndBufElem;
        sound->flags |= 0x40u;
        inst->soundIntf->SetCurrPos(inst->soundBufElemFromModel, 0);
        SND_SetInstVolume(inst);
        inst->soundIntf->SetFrequency(inst->soundBufElemFromModel, inst->freq);
        inst->soundIntf->SetPan(inst->soundBufElemFromModel, 0, 0);
    }
    if ((sound->flags & 0x4000) != 0)
        SND_SetTrack(instanceIdx, 1040u);
    return instanceIdx;
}

void SND_SModifierAddInInstance(int soundInstanceID, int sndModifierID, void* linkedSound, int dataSize)
{
    tSoundInstanceElem* inst;
    tSndModifierElem* modifier;

    if (soundInstanceID >= 0 && soundInstanceID < SND_gst_Params.soundInstanceNum)
    {
        inst = &SND_gst_Params.soundInstances[soundInstanceID];
        if ((inst->flags & 0x400) != 0)
        {
            modifier = SND_pst_SModifierInsertOneSModifier(&inst->modifierInstances, sndModifierID, linkedSound, dataSize);
            modifier->sndModifierModelIntf->PlayApplyOnInstance(modifier, modifier->linkedSoundData, modifier->linkedSoundDataSize, inst);
        }
    }
}

void SND_LinkToFamilly(int mask, int instanceIdx)
{
    tSoundInstanceElem* inst;

    if (instanceIdx >= 0 && instanceIdx < SND_gst_Params.soundInstanceNum)
    {
        inst = &SND_gst_Params.soundInstances[instanceIdx];
        if ((inst->flags & 0x400) != 0)
            inst->familyBitmask |= mask;
    }
}

int SND_i_IsTrackPlaying(unsigned int trackKey)
{
    int trackIdx;
    tTrackListElem* track;
    int result;
    int status;

    if (!SND_gst_Params.soundEnabled
        || (trackIdx = BAS_bsearch(trackKey, &SND_gst_TrackId), trackIdx == -1)
        || (track = &SND_gax_TrackPlayingList[trackIdx], track->trackID < 0)
        || (SND_gpst_Interface.win.GetStatus(track->sndBufferElem, &status),
            result = 1,
            (status & 1) == 0))
    {
        result = 0;
    }
    return result;
}

int SND_i_IsPlaying(int instanceIdx)
{
    tSoundInstanceElem* inst;
    int status;
    int flags;

    if (instanceIdx < 0)
        return 0;
    if (instanceIdx >= SND_gst_Params.soundInstanceNum)
        return 0;
    inst = &SND_gst_Params.soundInstances[instanceIdx];
    flags = inst->flags;
    if ((flags & 0x400) == 0)
        return 0;
    if (inst->timeRemainTillStart != 0.0)
    {
        inst->flags = flags | 0x200;
        return 1;
    }
    inst->soundIntf->GetStatus(inst->soundBufElemFromModel, &status);
    if ((status & 1) == 0)
        return 0;
    return ((inst->flags & 0x800000) != 0) + 1;
}

void SND_Stop(int instanceIdx)
{
    tSoundInstanceElem* inst;
    BOOL notOnTrack;

    if (instanceIdx >= 0 && instanceIdx < SND_gst_Params.soundInstanceNum)
    {
        inst = &SND_gst_Params.soundInstances[instanceIdx];
        if ((inst->flags & 0x400) != 0)
        {
            inst->soundIntf->Stop(inst->soundBufElemFromModel);
            notOnTrack = (inst->flags & 0x20000) == 0;
            inst->flags &= ~0x808000u;
            if (!notOnTrack)
                SND_TrackDeleteInstanceFromList(instanceIdx);
            inst->soundIntf->SetCurrPos(inst->soundBufElemFromModel, 0);
            inst->timeRemainTillStart = 0.0;
        }
    }
}

struct sSndBank* SND_ul_CallbackLoad(uint32_t Offset)
{
    tSndBank* result; 
    char String1[4];
    char* offset;
    int DstBuf[2]; // 2nd is type

    if (!LOA_gb_SpeedMode || BIG_gi_ReadMode != 2)
    {
        BIG_ReadNoSeek(Offset + 4, DstBuf, 8u);
        *(uint32_t*)String1 = DstBuf[1];
    }
    if (LOA_gb_SpeedMode && BIG_gi_ReadMode == 2)
    {
        offset = LOA_FetchBuffer(4);
        LOA_ReadCharArray(&offset, String1, 4u, 1);
    }
    if (!_strnicmp(String1, ".snk", 4u))
        result = SND_ul_CallbackLoadMetaBank(Offset);
    else
        result = SND_ul_CallbackLoadBank(Offset);
    return result;
}

void SND_AddMdFBankUser(struct sSoundMdfStruct* sndMdf)
{
    unsigned int idx;
    tSoundMdfStruct** grown;

    if (SND_gst_Params.soundEnabled && sndMdf)
    {
        for (idx = 0; idx < SND_gst_BankMdFUserList; ++idx)
        {
            if (!sndMdfBankUser[idx])
            {
                sndMdfBankUser[idx] = sndMdf;
                return;
            }
        }
        if (sndMdfBankUser)
            grown = (tSoundMdfStruct**)MEM_p_Realloc(sndMdfBankUser, 4 * SND_gst_BankMdFUserList + 16 * 4);
        else
            grown = (tSoundMdfStruct**)MEM_p_Alloc(4 * SND_gst_BankMdFUserList + 16 * 4);
        sndMdfBankUser = grown;
        memset(&grown[SND_gst_BankMdFUserList], 0, 16 * 4);
        idx = SND_gst_BankMdFUserList;
        SND_gst_BankMdFUserList += 16;
        sndMdfBankUser[idx] = sndMdf;
    }
}

int SND_i_FxSeti(int type, int value)
{
    int result;
    int att;
    int defaultAtt;
    int mode;
    int defaultMode;

    if (!SND_gst_Params.soundEnabled)
        return -1;
    result = 0;
    switch (type)
    {
    case 8193:
        if (value)
        {
            SND_gpst_Interface.win.FxDisable(&SND_gst_FxDefaultParam);
            SND_gst_FxDefaultParam.flags &= ~1u;
            SND_gpst_Interface.win.FxSetMode(&SND_gst_FxParam, SND_gst_FxParam.mode);
            SND_gpst_Interface.win.FxSetDelay(&SND_gst_FxParam, SND_gst_FxParam.delay);
            SND_gpst_Interface.win.FxSetFeedback(&SND_gst_FxParam, SND_gst_FxParam.feedback);
            SND_gpst_Interface.win.FxSetWetPan(&SND_gst_FxParam, SND_gst_FxParam.wetPan);
            att = SND_l_GetAttFromVol(SND_gst_FxParam.vol);
            SND_gpst_Interface.win.FxSetWetVolume(&SND_gst_FxParam, att);
            result = SND_gpst_Interface.win.FxEnable(&SND_gst_FxParam);
            SND_gst_FxParam.flags |= 1u;
        }
        else if ((SND_gst_FxParam.flags & 1) != 0)
        {
            result = SND_gpst_Interface.win.FxDisable(&SND_gst_FxParam);
            SND_gst_FxParam.flags &= ~1u;
        }
        break;
    case 8194:
        mode = value;
        if (value == 7)
        {
            SND_gst_FxParam.mode = 10;
        }
        else
        {
            if (value == 8)
                mode = 11;
            SND_gst_FxParam.mode = mode;
        }
        break;
    case 8195:
        SND_gst_FxParam.delay = value;
        break;
    case 8196:
        SND_gst_FxParam.feedback = value;
        break;
    case 8198:
        SND_gst_FxParam.wetPan = value;
        break;
    case 8199:
        if (value)
        {
            SND_gpst_Interface.win.FxDisable(&SND_gst_FxParam);
            SND_gst_FxParam.flags &= ~1u;
            SND_gpst_Interface.win.FxSetMode(&SND_gst_FxDefaultParam, SND_gst_FxDefaultParam.mode);
            SND_gpst_Interface.win.FxSetDelay(&SND_gst_FxDefaultParam, SND_gst_FxDefaultParam.delay);
            SND_gpst_Interface.win.FxSetFeedback(&SND_gst_FxDefaultParam, SND_gst_FxDefaultParam.feedback);
            SND_gpst_Interface.win.FxSetWetPan(&SND_gst_FxDefaultParam, SND_gst_FxDefaultParam.wetPan);
            defaultAtt = SND_l_GetAttFromVol(SND_gst_FxDefaultParam.vol);
            SND_gpst_Interface.win.FxSetWetVolume(&SND_gst_FxDefaultParam, defaultAtt);
            result = SND_gpst_Interface.win.FxEnable(&SND_gst_FxDefaultParam);
            SND_gst_FxDefaultParam.flags |= 1u;
        }
        else if ((SND_gst_FxDefaultParam.flags & 1) != 0)
        {
            result = SND_gpst_Interface.win.FxDisable(&SND_gst_FxDefaultParam);
            SND_gst_FxDefaultParam.flags &= ~1u;
        }
        break;
    case 8200:
        defaultMode = value;
        if (value == 7)
        {
            SND_gst_FxDefaultParam.mode = 10;
        }
        else
        {
            if (value == 8)
                defaultMode = 11;
            SND_gst_FxDefaultParam.mode = defaultMode;
        }
        break;
    case 8201:
        SND_gst_FxDefaultParam.delay = value;
        break;
    case 8202:
        SND_gst_FxDefaultParam.feedback = value;
        break;
    case 8204:
        SND_gst_FxDefaultParam.wetPan = value;
        break;
    default:
        return -1;
    }
    return result;
}

int SND_i_FxSetf(int type, float value)
{
    int result;

    if (!SND_gst_Params.soundEnabled)
        return -1;
    result = 0;
    if (type == 8197)
    {
        SND_gst_FxParam.vol = value;
    }
    else
    {
        if (type != 8203)
            return -1;
        SND_gst_FxDefaultParam.vol = value;
    }
    return result;
}

void SND_LoadingSoundRegisterInstanceUserCallback(int index, void (*callback)(void*), struct sLoadingSoundMdfStruct* param) 
{
    if (index >= 0
        && index < SND_gst_Params.soundInstanceNum
        && (SND_gst_Params.soundInstances[index].flags & 0x400) != 0
        && SND_gst_LSGlobal.soundInstanceIndex != index)
    {
        if (SND_gst_LSGlobal.soundInstanceIndex != -1
            && SND_gst_Params.soundInstances[SND_gst_LSGlobal.soundInstanceIndex].allocatedSoundsIdx != SND_gst_Params.soundInstances[index].allocatedSoundsIdx)
        {
            SND_ReinitLoadingInstance();
        }
        if (SND_gst_LSGlobal.userCallBackFunction)
            SND_gst_LSGlobal.userCallBackFunction(SND_gst_LSGlobal.userCallBackFunctionParam);
        SND_gst_LSGlobal.userCallBackFunction = callback;
        SND_gst_LSGlobal.userCallBackFunctionParam = (void*)param;
        SND_gst_LSGlobal.soundInstanceIndex = index;
    }
}

int SND_l_GetLoadingInstance()
{
    return SND_gst_LSGlobal.soundInstanceIndex;
}

void SND_TrackDestroy(unsigned int trackKey)
{
    int trackIdx;
    tTrackListElem* trackList;
    unsigned int trackIdxAlias;
    int instIdx;
    tSoundInstanceElem* inst;
    unsigned int newFlags;
    unsigned int buffer;
    int trackIdxSaved;

    trackIdx = BAS_bsearch(trackKey, &SND_gst_TrackId);
    trackIdxSaved = trackIdx;
    if (trackIdx != -1)
    {
        trackList = SND_gax_TrackPlayingList;
        trackIdxAlias = trackIdx;
        if (SND_gax_TrackPlayingList[trackIdx].flags != 1)
        {
            instIdx = 0;
            if (SND_gst_Params.soundInstanceNum > 0)
            {
                inst = SND_gst_Params.soundInstances;
                do
                {
                    if ((inst->flags & 0x400) != 0 && inst->trackID == trackKey)
                    {
                        if (trackList[trackIdxAlias].trackID == instIdx)
                        {
                            buffer = (unsigned int)inst->soundBufElemFromModel;
                            inst->soundIntf = trackList[trackIdxAlias].soundIntf;
                            BAS_bdelete(buffer, &SND_gst_TrackLookUpTable);
                        }
                        newFlags = inst->flags & ~0x20010u;
                        inst->trackID = -1;
                        inst->flags = newFlags;
                        trackList = SND_gax_TrackPlayingList;
                    }
                    ++instIdx;
                    inst++;
                } while (instIdx < SND_gst_Params.soundInstanceNum);
                trackIdx = trackIdxSaved;
            }
            if (trackList[trackIdxAlias].stereoToMono_sub499C20)
            {
                SND_TrackSwitchToMono(trackIdx);
                trackList = SND_gax_TrackPlayingList;
            }
            if (trackList[trackIdxAlias].trackResourceNum)
            {
                SND_TrackReleaseResource(trackIdx);
                trackList = SND_gax_TrackPlayingList;
            }
            trackList[trackIdxAlias].sndBufferElem = 0;
            SND_gax_TrackPlayingList[trackIdxAlias].trackResource = 0;
            SND_gax_TrackPlayingList[trackIdxAlias].trackID = -1;
            SND_gax_TrackPlayingList[trackIdxAlias].soundIntf = 0;
            SND_gax_TrackPlayingList[trackIdxAlias].flags = 1;
            BAS_bdelete(trackKey, &SND_gst_TrackId);
        }
    }
}

void SND_TrackFreeList()
{
    while (SND_gst_TrackId.actElemNum)
        SND_TrackDestroy(SND_gst_TrackId.keyValPtr->key);
    if (SND_gax_TrackPlayingList)
        MEM_Free(SND_gax_TrackPlayingList);
    SND_gax_TrackPlayingList = 0;
    SND_gi_TrackNb = 0;
    SND_gst_TrackId.actElemNum = 0;
    SND_gi_TrackLastVirtualId = 10000;
}

void SND_StopAll(int mode)
{
    int i;

    if (SND_gst_Params.soundEnabled && mode != 20)
    {
        SND_i_FxSeti(8193, 0);
        SND_i_FxSeti(8199, 0);
        SND_ReinitLoadingInstance();
        for (i = 0; i < SND_gst_Params.soundInstanceNum; ++i)
            SND_Release(i);
        SND_TrackFreeList();
        SND_AllocatedSoundsNumWithhoutFlag80000000 = 0;
        SND_AllocatedSoundsNumWithhoutFlag80000000PeakUsage = 0;
        SND_AllocatedSoundsNumWithFlag80000000 = 0;
        SND_AllocatedSoundsNumWithFlag80000000PeakUsage = 0;
    }
}

int SND_MuteSetUp(int group, float setUp2, float setUp1)
{
    int result;

    result = SND_gst_Params.soundEnabled;
    if (SND_gst_Params.soundEnabled)
    {
        result = group;
        switch (group)
        {
        case 0:
        case 1:
        case 2:
        case 3:
        case 4:
        case 6:
        case 7:
        case 8:
        case 9:
        case 10:
        case 11:
            result = group;
            SND_Mutes_B30600_fadergroups[result].muteSetUp1 = setUp1;
            SND_Mutes_B30600_fadergroups[result].muteSetUp2 = setUp2;
            break;
        default:
            return result;
        }
    }
    return result;
}

float SND_f_MuteGet(int group, int which)
{
    double result;

    if (!SND_gst_Params.soundEnabled)
        return 0.0;
    switch (group)
    {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 6:
    case 7:
    case 8:
    case 9:
    case 10:
    case 11:
        if (which)
            result = SND_Mutes_B30600_fadergroups[group].muteSetUp2;
        else
            result = SND_Mutes_B30600_fadergroups[group].muteSetUp1;
        break;
    default:
        return 0.0;
    }
    return (float)result;
}

void SND_FadeUpdate()
{
    tMutesSetupStruct* fader;
    int flags;
    double elapsed;
    int step;
    double start;
    double level;
    double rounded;

    for (fader = SND_Mutes_B30600_fadergroups; fader <= &SND_Mutes_B30600_fadergroups[11]; ++fader)
    {
        flags = fader->flags;
        if (!flags)
            continue;
        if ((flags & 1) != 0)
        {
            elapsed = TIM_gf_dt + fader->elapsedTime;
            fader->elapsedTime = (float)elapsed;
            rounded = elapsed * fader->i6 + MATH_gd_magicDoubleForRound;
            step = (*(uint32_t *)&rounded);
            if ((*(int32_t*)&rounded) >= 0)
            {
                if ((*(int32_t*)&rounded) > 255)
                {
                    fader->muteSetUp2 = fader->muteSetUp2ToSetWithThis;
                    fader->muteEnabled = 0;
                    fader->flags = 0;
                    continue;
                }
            }
            else
            {
                step = 0;
            }
            start = fader->i7;
            fader->muteSetUp1 = 1.0;
            level = (start - fader->muteSetUp2ToSetWithThis) * SND_gf_FadeOut[step] + fader->muteSetUp2ToSetWithThis;
            fader->muteSetUp2 = (float)level;
            if (level >= fader->muteSetUp2ToSetWithThis)
                fader->flags = 0;
            if (level >= 1.0)
                fader->muteEnabled = 0;
            continue;
        }
        if ((flags & 2) == 0)
            continue;
        elapsed = TIM_gf_dt + fader->elapsedTime;
        fader->elapsedTime = (float)elapsed;
        rounded = elapsed * fader->i6 + MATH_gd_magicDoubleForRound;
        step = *(uint32_t *)(&rounded);
        if ((*(int32_t *)&rounded) >= 0)
        {
            if ((*(int32_t *)&rounded) > 255)
            {
                fader->muteSetUp2 = fader->muteSetUp2ToSetWithThis;
                fader->muteSetUp1 = 1.0;
                fader->flags = 0;
                continue;
            }
        }
        else
        {
            step = 0;
        }
        start = fader->i7;
        fader->muteSetUp1 = 1.0;
        level = (start - fader->muteSetUp2ToSetWithThis) * SND_gf_FadeOut[step] + fader->muteSetUp2ToSetWithThis;
        fader->muteSetUp2 = (float)level;
        if (level <= fader->muteSetUp2ToSetWithThis)
            fader->flags = 0;
    }
}

void SND_FamillyUpdate()
{
    int idx;
    tSoundFamilyStruct* familly;
    int i;
    tSoundFamilyInnerStruct* inner;
    tSoundFamilyInnerStruct* inner2;
    int j;
    tSoundFamilyInnerStruct* inner3;
    tSoundFamilyInnerStruct* inner4;
    tSoundFamilyInnerStruct* inner5;
    tSoundFamilyInnerStruct* inner6;
    BOOL more;
    int count;
    int scan;

    idx = 0;
    count = 0;
    if (SND_gi_AndFamillyNb > 0)
    {
        scan = 0;
        do
        {
            familly = &SND_gast_AndFamillyList[idx];
            for (i = 0; i < familly->innerNum; ++i)
            {
                inner = familly->unkIntArrPtr[i];
                if (inner->muteSetup2.muteEnabled == 10)
                    inner->muteSetup2.muteEnabled = 1;
                inner2 = familly->unkIntArrPtr[i];
                if (inner2->muteSetup2.muteEnabled == 20)
                    inner2->muteSetup2.muteEnabled = 2;
            }
            for (j = 0; j < familly->innerNum; ++j)
            {
                inner3 = familly->unkIntArrPtr[j];
                if (inner3->muteSetup2.muteEnabled == 1)
                {
                    inner3->muteSetup2.muteSetUp2 = TIM_gf_dt * inner3->muteSetup1TimeMultiplier + inner3->muteSetup2.muteSetUp2;
                    inner4 = familly->unkIntArrPtr[j];
                    if (inner4->muteSetup2.muteSetUp2 < 1.0)
                    {
                        inner4->muteSetup2.muteEnabled = 10;
                    }
                    else
                    {
                        inner4->muteSetup2.muteSetUp2 = 1.0;
                        familly->unkIntArrPtr[j]->muteSetup2.muteEnabled = 100;
                    }
                }
                inner5 = familly->unkIntArrPtr[j];
                if (inner5->muteSetup2.muteEnabled == 2)
                {
                    inner5->muteSetup2.muteSetUp2 = TIM_gf_dt * inner5->muteSetup2TimeMultiplier + inner5->muteSetup2.muteSetUp2;
                    inner6 = familly->unkIntArrPtr[j];
                    if (inner6->muteSetup2.muteSetUp2 > 0.0)
                    {
                        inner6->muteSetup2.muteEnabled = 20;
                    }
                    else
                    {
                        inner6->muteSetup2.muteSetUp2 = 0.0;
                        familly->unkIntArrPtr[j]->muteSetup2.muteEnabled = 200;
                    }
                }
            }
            idx = scan + 1;
            more = ++count < SND_gi_AndFamillyNb;
            scan++;
        } while (more);
    }
}

void SND_DeleteOldLoadingSound()
{
    int stillUsed;
    int idx;
    tSndSoundElem* sounds;
    int scan;
    unsigned int flags;

    if (SND_gst_LSGlobal.isNewLoading)
    {
        stillUsed = 0;
        idx = 0;
        if (SND_gst_Params.allocatedSoundsMaxNum > 0)
        {
            sounds = SND_gst_Params.allocatedSounds;
            scan = 0;
            do
            {
                flags = sounds[scan].flags;
                if ((flags & 0x400) != 0 && (flags & 8) != 0)
                {
                    ++stillUsed;
                    if (sounds[scan].usageCnt <= 0)
                    {
                        --stillUsed;
                        sounds[scan].usageCnt = 1;
                        SND_DeleteByIndex(idx);
                        sounds = SND_gst_Params.allocatedSounds;
                    }
                }
                ++idx;
                ++scan;
            } while (idx < SND_gst_Params.allocatedSoundsMaxNum);
        }
        SND_gst_LSGlobal.isNewLoading = 0;
        SND_gst_LSGlobal.isSoundStillUsedElsewhere = stillUsed <= 0;
    }
}

void SND_MuteEnable(int group, int enabled)
{
    switch (group)
    {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 6:
    case 7:
    case 8:
    case 9:
    case 10:
    case 11:
        SND_Mutes_B30600_fadergroups[group].muteEnabled = enabled;
        break;
    default:
        return;
    }
}

void SND_LoadingSoundUpdate()
{
    int soundIdx;
    tSndSoundElem* sound;
    unsigned int flags;
    double faded;
    float inverse;
    float level;

    SND_DeleteOldLoadingSound();
    if (SND_gst_LoadingEffect.mode == 1)
    {
        if (SND_gst_LoadingEffect.i5 == 0.0)
            return;
        faded = SND_gst_LoadingEffect.i5 - SND_gst_LoadingEffect.i4 * 0.02;
        SND_gst_LoadingEffect.i5 = (float)faded;
        if (faded < 0.0)
            SND_gst_LoadingEffect.i5 = 0.0;
        level = SND_gst_LoadingEffect.i5;
        inverse = (float)(1.0 - SND_gst_LoadingEffect.i5);
        SND_MuteSetUp(6, inverse, level);
        return;
    }
    if (SND_gst_LoadingEffect.mode == 2)
    {
        if (SND_gst_LoadingEffect.i5 < 1.0)
        {
            faded = SND_gst_LoadingEffect.i4 * 0.02 + SND_gst_LoadingEffect.i5;
            SND_gst_LoadingEffect.i5 = (float)faded;
            if (faded > 1.0)
                SND_gst_LoadingEffect.i5 = 1.0;
            level = SND_gst_LoadingEffect.i5;
            inverse = (float)(1.0 - SND_gst_LoadingEffect.i5);
            SND_MuteSetUp(6, inverse, level);
            return;
        }
        SND_gst_LoadingEffect.mode = 0;
        SND_MuteEnable(6, 0);
        if (!SND_gst_LSGlobal.isSoundStillUsedElsewhere)
        {
            SND_Stop(SND_gst_LSGlobal.soundInstanceIndex);
            return;
        }
    }
    else if (SND_gst_LoadingEffect.mode)
    {
        SND_gst_LoadingEffect.mode = 0;
        SND_MuteEnable(6, 0);
        return;
    }
    else if (!SND_gst_LSGlobal.isSoundStillUsedElsewhere)
    {
        return;
    }
    SND_ReinitLoadingInstance();
    soundIdx = SND_gst_LSGlobal.soundElemIndex;
    if (SND_gst_LSGlobal.soundElemIndex >= 0)
    {
        sound = &SND_gst_Params.allocatedSounds[SND_gst_LSGlobal.soundElemIndex];
        flags = sound->flags;
        if ((flags & 0x400) != 0 && (flags & 8) != 0)
        {
            sound->usageCnt = 1;
            SND_gst_LSGlobal.soundElemIndex = -1;
            SND_DeleteByIndex(soundIdx);
        }
    }
}

void SND_SetMicroPos(Mtx4x4* this)
{
    if (SND_gpst_RqMicroPosFromAI && SND_gpst_RqMicroPosFromAI->matrices)
    {
        SND_gst_Params.sndMatrixRef = SND_gpst_RqMicroPosFromAI->matrices;
        SND_gst_Params.flags &= ~0x1000u;
    }
    else if ((SND_gst_Params.flags & 0x1000) != 0)
    {
        SND_gst_Params.sndMatrixRef = &SND_gst_Params.sndMatrix2;
    }
    else if (*AI_MainActor)
    {
        SND_gst_Params.sndMatrixRef = (*AI_MainActor)->matrices;
    }
    else
    {
        SND_gst_Params.sndMatrixRef = this;
    }
    SND_gpst_RqMicroPosFromAI = 0;
    SND_gst_Params.microPosMtxPtr = &SND_gst_Params.microPosMtx;
    if ((SND_gst_Params.flags & 0x2000) == 0)
        SND_gst_Params.microPosMtxPtr = this;
}

void SND_SModifierUpdateInstance(tSoundInstanceElem* inst)
{
    tSndModifierElem* i;
    void(*update)(tSndModifierElem*, tSoundInstanceElem*, int);

    for (i = inst->modifierInstances; i; i = i->nextModifier)
    {
        update = i->sndModifierModelIntf->Update;
        if (update)
            update(i, inst, 4);
    }
}

void SND_Update(Mtx4x4* mtx)
{
    double invDt;
    float micZ;
    int idx;
    tSoundInstanceElem* inst;
    int flags;
    double remaining;
    tGameObjectMain* gao;
    int instFlags;
    int status;
    Vec3 distance;

    if (!SND_gst_Params.soundEnabled)
        return;
    SND_SetMicroPos(mtx);
    if (SND_gst_Params.sndMatrixRef && i_init_521)
    {
        distance.x = SND_gst_Params.sndMatrixRef->Pos.x - t_last_mic_pos_522.x;
        distance.y = SND_gst_Params.sndMatrixRef->Pos.y - t_last_mic_pos_522.y;
        distance.z = SND_gst_Params.sndMatrixRef->Pos.z - t_last_mic_pos_522.z;
        invDt = 1.0 / TIM_gf_dt;
        t_last_mic_pos_522 = SND_gst_Params.sndMatrixRef->Pos;
        distance.x = (float)(distance.x * invDt);
        distance.y = (float)(distance.y * invDt);
        distance.z = (float)(invDt * distance.z);
    }
    else
    {
        if (SND_gst_Params.sndMatrixRef)
        {
            t_last_mic_pos_522.x = SND_gst_Params.sndMatrixRef->Pos.x;
            t_last_mic_pos_522.y = SND_gst_Params.sndMatrixRef->Pos.y;
            micZ = SND_gst_Params.sndMatrixRef->Pos.z;
            i_init_521 = 1;
            t_last_mic_pos_522.z = micZ;
        }
        distance.x = 0.0;
        distance.y = 0.0;
        distance.z = 0.0;
    }
    SND_LoadingSoundUpdate();
    SND_FamillyUpdate();
    for (idx = 0; idx < SND_gst_Params.soundInstanceNum; ++idx)
    {
        inst = &SND_gst_Params.soundInstances[idx];
        flags = inst->flags;
        if ((flags & 0x400) == 0)
            continue;
        if (inst->timeRemainTillStart != 0.0)
        {
            remaining = inst->timeRemainTillStart - TIM_gf_dt;
            inst->timeRemainTillStart = (float)remaining;
            if (remaining <= 0.0)
            {
                inst->timeRemainTillStart = 0.0;
                SND_PlayRq(0, inst, flags & 0x20000 | ((flags & 4 | 0x80000003) >> 2), -1);
            }
        }
        else if ((flags & 0x80u) != 0 && (flags & 0x200) != 0 && (inst->muteFactorFlags & 4) == 0)
        {
            inst->soundIntf->GetStatus(inst->soundBufElemFromModel, &status);
            if ((status & 1) == 0)
            {
                SND_FreeInstance(idx);
                continue;
            }
        }
        gao = inst->gao;
        if (!gao || (gao->flags2 & 0x400000) == 0)
        {
            if (inst->modifierInstances)
                SND_SModifierUpdateInstance(inst);
            SND_SetInstVolume(inst);
            instFlags = inst->flags;
            if ((instFlags & 0x2000) == 0 && (instFlags & 0x1000) != 0)
                inst->soundIntf->ComputeDoppler(&distance, &SND_gst_Params.sndMatrixRef->Pos, inst);
        }
    }
    SND_FadeUpdate();
    SND_gpst_Interface.win.SNDNull(&distance);
}

float SND_f_SphericComputing(Vec3* micPos, struct sSoundInstanceElem* inst, int* state)
{
    Vec3* sndPos;
    double dy;
    double dz;
    double dx;
    double dist;
    double lowCoeff;
    float nearDist;
    double farDist;
    float highCoeff;
    float scaled;

    *state = 0;
    sndPos = inst->sndPositionSetInSetPosPtr;
    dy = micPos->y - sndPos->y;
    dz = micPos->z - sndPos->z;
    dx = micPos->x - sndPos->x;
    dist = sqrt(dz * dz + dx * dx + dy * dy);
    if (dist >= inst->farArr[0])
    {
        if ((inst->muteFactorFlags & 5) == 1
            && dist > inst->dtFar + inst->dtFar + inst->farArr[0])
        {
            *state = 1;
        }
        if ((inst->muteFactorFlags & 6) == 6 && dist < inst->dtFar + inst->farArr[0])
            *state |= 2u;
    }
    scaled = (float)(SND_gst_Params.f_rolloff * dist);
    if (scaled <= (double)inst->nearArr[0])
        return 1.0;
    if (scaled <= (double)inst->centerArr[0])
    {
        lowCoeff = inst->middleCoefficient;
        nearDist = inst->nearArr[0];
        farDist = inst->centerArr[0];
        highCoeff = 1.0;
        return (float)(lowCoeff + (1.0 / (scaled * scaled) - 1.0 / (farDist * farDist)) * (nearDist * nearDist * (farDist * farDist)) / (farDist * farDist - nearDist * nearDist) * (highCoeff - lowCoeff));
    }
    if (scaled < (double)inst->farArr[0])
    {
        lowCoeff = inst->farCoefficient;
        farDist = inst->farArr[0];
        nearDist = inst->centerArr[0];
        highCoeff = inst->middleCoefficient;
        return (float)(lowCoeff + (1.0 / (scaled * scaled) - 1.0 / (farDist * farDist)) * (nearDist * nearDist * (farDist * farDist)) / (farDist * farDist - nearDist * nearDist) * (highCoeff - lowCoeff));
    }
    return inst->farCoefficient;
}

void SND_SModifierNotifyStopInstance(tSoundInstanceElem* inst)
{
    tSndModifierElem* i;
    void( *notify)(tSndModifierElem*, tSoundInstanceElem*);

    for (i = inst->modifierInstances; i; i = i->nextModifier)
    {
        notify = i->sndModifierModelIntf->NotifyStop;
        if (notify)
            notify(i, inst);
    }
}

struct sSndModifierElem* SND_pst_SModifierGet(struct sSndModifierElem* list, int type)
{
    tSndModifierElem* result;

    if (type >= 7 || type < 0)
        return NULL;
    for (result = list; result; result = result->nextModifier)
    {
        if (!result->sndModifierModelIntf)
            break;
        if (result->sndModifierModelIntf->Type == type)
            return result;
    }
    return NULL;
}

void SND_StopRq(int instanceIdx)
{
    tSoundInstanceElem* inst;

    if (instanceIdx >= 0 && instanceIdx < SND_gst_Params.soundInstanceNum)
    {
        inst = &SND_gst_Params.soundInstances[instanceIdx];
        if ((inst->flags & 0x400) != 0 && (inst->muteFactorFlags & 0x1000) == 0)
        {
            SND_SModifierNotifyStopInstance(&SND_gst_Params.soundInstances[instanceIdx]);
            if (SND_pst_SModifierGet(inst->modifierInstances, 4))
                inst->muteFactorFlags |= 0x1000u;
            else
                SND_Stop(instanceIdx);
        }
    }
}

void SND_AutoStop(struct sSoundInstanceElem* inst)
{
    int muteFlags;
    int status;
    int trackFlag;

    if ((inst->muteFactorFlags & 4) == 0)
    {
        inst->soundIntf->GetStatus(inst->soundBufElemFromModel, &status);
        if ((status & 1) != 0)
        {
            trackFlag = (inst->flags & 0x20000);
            SND_StopRq((((char*)inst - (char*)SND_gst_Params.soundInstances) / sizeof(tSoundInstanceElem)));
            muteFlags = inst->muteFactorFlags | 4;
            inst->flags |= trackFlag; // fixed: was (unsigned int)a1
            inst->muteFactorFlags = muteFlags;
        }
    }
}

float SND_f_AxisComputing(Vec3* micPos, struct sSoundInstanceElem* inst, int* state, int axis)
{
    Vec3* sndPos;
    double dx;
    double dy;
    double dz;
    Vec3* axisVec;
    double projection;
    double lowCoeff;
    double farDist;
    float highCoeff;
    float nearDist;
    float scaled;

    *state = 0;
    sndPos = inst->sndPositionSetInSetPosPtr;
    dx = micPos->x - sndPos->x;
    dy = micPos->y - sndPos->y;
    dz = micPos->z - sndPos->z;
    if (axis)
    {
        if (axis == 1)
        {
            axisVec = &inst->sndPosMtxSetInSetPos->v2;
        }
        else
        {
            if (axis != 2)
                return 0.0;
            axisVec = &inst->sndPosMtxSetInSetPos->v3;
        }
    }
    else
    {
        axisVec = &inst->sndPosMtxSetInSetPos->v1;
    }
    projection = fabs(axisVec->z * dz + axisVec->y * dy + dx * axisVec->x);
    if ((inst->muteFactorFlags & 5) == 1
        && projection > inst->dtFar + inst->dtFar + inst->farArr[axis])
    {
        *state = 1;
    }
    if ((inst->muteFactorFlags & 6) == 6 && projection < inst->farArr[axis] + inst->dtFar)
        *state |= 2u;
    scaled = (float)(SND_gst_Params.f_rolloff * projection);
    if (scaled <= (double)inst->nearArr[axis])
        return 1.0;
    if (scaled <= (double)inst->centerArr[axis])
    {
        highCoeff = 1.0;
        nearDist = inst->nearArr[axis];
        lowCoeff = inst->middleCoefficient;
        farDist = inst->centerArr[axis];
        return (float)(lowCoeff + (1.0 / (scaled * scaled) - 1.0 / (farDist * farDist)) * (nearDist * nearDist * (farDist * farDist)) / (farDist * farDist - nearDist * nearDist) * (highCoeff - lowCoeff));
    }
    if (scaled < (double)inst->farArr[axis])
    {
        lowCoeff = inst->farCoefficient;
        farDist = inst->farArr[axis];
        highCoeff = inst->middleCoefficient;
        nearDist = inst->centerArr[axis];
        return (float)(lowCoeff + (1.0 / (scaled * scaled) - 1.0 / (farDist * farDist)) * (nearDist * nearDist * (farDist * farDist)) / (farDist * farDist - nearDist * nearDist) * (highCoeff - lowCoeff));
    }
    return inst->farCoefficient;
}

void SND_AutoPlay(struct sSoundInstanceElem* inst)
{
    int muteFlags;

    muteFlags = inst->muteFactorFlags;
    if ((muteFlags & 4) != 0)
    {
        inst->muteFactorFlags = muteFlags & ~4u;
        SND_PlayRq(0, inst, inst->flags & 0x20000 | ((unsigned int)inst->flags >> 2) & 1, -1);
    }
}

float SND_f_GetGlobalVol()
{
    int att;

    if (!SND_gst_Params.soundEnabled)
        return 0.0;
    att = SND_gpst_Interface.win.GetGlobalVol();
    return SND_f_GetVolFromAtt(att);
}

int SND_TrackInitModule()
{
    tTrackResourceElem *resource;

    SND_gst_TrackId.keyValPtr = 0;
    SND_gst_TrackId.actElemNum = 0;
    SND_gst_TrackId.maxElemNum = 0;
    BAS_binit(&SND_gst_TrackId, 100);
    SND_gst_TrackLookUpTable.keyValPtr = 0;
    SND_gst_TrackLookUpTable.actElemNum = 0;
    SND_gst_TrackLookUpTable.maxElemNum = 0;
    BAS_binit(&SND_gst_TrackLookUpTable, 200);
    SND_gi_TrackLastVirtualId = 10000;
    SND_gi_RasterCurrentFileId = 0x2000;
    resource = SND_gast_TrackResources;
    do
    {
        SND_gpst_Interface.win.CreateTrack(&(resource->stereoSoundBuffer), &(resource->monoSoundBuffer));
        resource->isBusy = 0;
        resource++;
    } while (resource <= &SND_gast_TrackResources[3]);
    return SND_i_TrackNew();
}

int SND_i_ChangeRenderMode(int mode)
{
    int result;

    if (mode == -1)
        return SND_gst_Params.soundRenderMode;
    if ((mode & SND_gst_Params.soundRenderModeMask) != mode)
        return -1;
    result = SND_gst_Params.soundRenderMode;
    if (mode == SND_gst_Params.soundRenderMode)
        return 0;
    switch (mode)
    {
    case -1:
        return result;
    case 1:
    case 2:
    case 4:
    case 8:
    case 16:
    case 32:
    case 64:
        result = SND_gpst_Interface.win.ChangeRenderMode(mode);
        break;
    default:
        result = -2;
        break;
    }
    return result;
}

int ret_negaitive_1()
{
    return -1;
}

int ConsoleSpecificOption(int option)
{
    int result;
    int state;
    int renderMode;

    switch (option)
    {
    case 1:
        renderMode = SND_i_ChangeRenderMode(-1);
        if (renderMode == -1)
            state = -1;
        else
            state = renderMode != 1;
        result = state;
        break;
    case 2:
        result = SND_i_ChangeRenderMode(1) != 0;
        break;
    case 3:
        result = SND_i_ChangeRenderMode(2) == 0;
        break;
    case 4:
        result = 0;
        break;
    case 16:
    case 17:
    case 18:
        result = ret_negaitive_1();
        break;
    default:
        result = -1;
        break;
    }
    return result;
}

int SND_StreamInitModule()
{
    int result;

    SND_gast_StreamList = 0;
    SND_gi_StreamListSize = 0;
    // preserved: SND_i_StreamAlloc assigns SND_gast_StreamList, and which of the
    // base and the index is evaluated first is unspecified
    SND_gast_StreamList[SND_i_StreamAlloc()].num = 0;
    SND_gst_PrefetchManager.ptr = 0;
    SND_gst_PrefetchManager.num = 0;
    SND_gst_PrefetchManager.sumChannelNums = 0;
    SND_gst_PrefetchManager.ptr = (tSndPrefetchElem*)MEM_p_Alloc(8 * sizeof(tSndPrefetchElem));
    result = 0;
    memset(SND_gst_PrefetchManager.ptr, 0, 8 * sizeof(tSndPrefetchElem));
    SND_gst_PrefetchManager.num = 8;
    return result;
}

void SND_LoadingSoundInitModule()
{
    tSndWavHdrStruct wavHdr;

    memset(&SND_gst_LSGlobal, 0, sizeof(SND_gst_LSGlobal));
    SND_gst_LSGlobal.soundInstanceIndex = -1;
    SND_gst_LSGlobal.soundElemIndex = -1;
    wavHdr.wfex.wFormatTag = 2;
    wavHdr.wfex.nBlockAlign = 2;
    SND_gst_LoadingEffect.mode = 0;
    SND_gst_LoadingEffect.f_fadeOutTime = 0.2f;
    SND_gst_LoadingEffect.f_fadeInTime = 1.0;
    SND_gst_LoadingEffect.i6 = 1.0;
    SND_gst_LoadingEffect.i4 = 1.0;
    SND_gst_LoadingEffect.i5 = 1.0;
    wavHdr.wfex.nChannels = 1;
    wavHdr.wfex.nSamplesPerSec = 48000;
    wavHdr.wfex.nAvgBytesPerSec = 96000;
    wavHdr.wfex.wBitsPerSample = 16;
    wavHdr.wfex.cbSize = 0;
    wavHdr.fullSoundSize = 153600;
    wavHdr.dataStartPos = 0;
    SND_gi_RasterCurrentFileId = 8;
    SND_gst_LSGlobal.soundElemElem = SND_gpst_Interface.win.Create(SND_gst_Params.handles, &wavHdr, 0);
}

int SND_i_FxInitModule()
{
    if (!SND_gst_Params.soundEnabled)
        return -1;
    memset(&SND_gst_FxParam, 0, sizeof(SND_gst_FxParam));
    memset(&SND_gst_FxDefaultParam, 0, sizeof(SND_gst_FxDefaultParam));
    SND_gst_FxParam.mode = 0;
    SND_gst_FxDefaultParam.mode = 0;
    SND_gpst_Interface.win.FxInit();
    SND_i_FxSeti(8200, 0);
    SND_i_FxSeti(8201, 0);
    SND_i_FxSeti(8202, 0);
    SND_i_FxSetf(8203, 0.0);
    SND_i_FxSeti(8204, 0);
    SND_i_FxSeti(8199, 1);
    return 0;
}

void SND_RasterInitModule()
{
    memset(&sndRasterStat, 0, sizeof(sndRasterStat));
    SND_AllocatedSoundsNumWithhoutFlag80000000 = 0;
    SND_AllocatedSoundsNumWithFlag80000000 = 0;
    SND_gi_RasterCurrentFileId = 0;
    SND_AllocatedSoundsNumWithhoutFlag80000000PeakUsage = 0;
    SND_AllocatedSoundsNumWithFlag80000000PeakUsage = 0;
    BAS_binit(&rasterbas1, 200);
    BAS_binit(&rasterbas2, 200);
}

void SND_InitModule()
{
    SND_l_InitVolumeTable();
    SND_RasterInitModule();
    SND_gul_WavBinSeek = 0;
    SND_gpst_RqMicroPosFromAI = 0;
    SND_gb_UnloadedFile = 0;
    SND_gb_AllSoundsLoaded = 0;
    memset(&SND_gst_Params, 0, sizeof(SND_gst_Params));
    SND_gst_Params.handles = (tSndHandles*)MEM_p_Alloc(sizeof(tSndHandles));
    SND_gst_HeaderArray.keyValPtr = 0;
    SND_gst_HeaderArray.actElemNum = 0;
    SND_gst_HeaderArray.maxElemNum = 0;
    SND_gst_HeaderArray.elemNumToAddWhenFull = 0;
    SND_gst_Params.f_surround = 1.2f;
    SND_gst_Params.f_rolloff = 1.0;
    SND_gst_Params.fx_doppler = 1.0;
    SND_gst_Params.fx_DopplerSpeed = 300.0f;
    SND_gst_Params.flags |= 0x500u;
    SND_gst_Params.soundInstanceMaxNum = 200;
    SND_gst_Params.soundInstanceNum = 200;
    SND_gst_Params.soundInstances = (tSoundInstanceElem*)MEM_p_Alloc(sizeof(tSoundInstanceElem) * 200);
    memset(SND_gst_Params.soundInstances, 0, sizeof(tSoundInstanceElem) * SND_gst_Params.soundInstanceMaxNum);
    SND_gst_Params.allocatedSoundsMaxNum = 1000;
    SND_gst_Params.allocatedSounds = (tSndSoundElem*)MEM_p_Alloc(sizeof(tSndSoundElem) * 1000);
    memset(SND_gst_Params.allocatedSounds, 0, sizeof(tSndSoundElem) * SND_gst_Params.allocatedSoundsMaxNum);
    memset(&SND_gpst_Interface, 0, sizeof(SND_gpst_Interface));
    if (!SND_i_SoftInit())
    {
        SND_i_ChangeRenderMode(64);
        if (!SND_InitSoundSystem() && !SND_gc_NoSound)
        {
            SND_gst_Params.soundEnabled = 1;
            SND_InitMdFModule();
            SND_TrackInitModule();
            SND_StreamInitModule();
            SND_LoadingSoundInitModule();
            SND_i_FxInitModule();
            SND_SModifierInitModule();
            SND_gast_AndFamillyList = 0;
            SND_gi_AndFamillyListSize = 0;
            SND_gi_AndFamillyNb = 0;
        }
    }
}

void SND_SetGlobalPan(int pan)
{
    SND_gpst_Interface.win.SetGlobalPan(pan);
}

int SND_l_GetGlobalPan()
{
    return SND_gpst_Interface.win.GetGlobalPan();
}

void SND_SetGlobalVol(float vol)
{
    int att;

    if (SND_gst_Params.soundEnabled)
    {
        att = SND_l_GetAttFromVol(vol);
        SND_gpst_Interface.win.SetGlobalVol(att);
    }
}

int SND_l_GetPan(int instanceIdx)
{
    int pan;
    int front;

    if (instanceIdx < 0 || instanceIdx >= SND_gst_Params.soundInstanceNum || (SND_gst_Params.soundInstances[instanceIdx].flags & 0x400) == 0)
        return 0;
    SND_gst_Params.soundInstances[instanceIdx].soundIntf->GetPan(SND_gst_Params.soundInstances[instanceIdx].soundBufElemFromModel, &pan, &front);
    return pan;
}

int SND_l_GetFront(int instanceIdx)
{
    int pan;
    int front;

    if (instanceIdx < 0 || instanceIdx >= SND_gst_Params.soundInstanceNum || (SND_gst_Params.soundInstances[instanceIdx].flags & 0x400) == 0)
        return 0;
    SND_gst_Params.soundInstances[instanceIdx].soundIntf->GetPan(SND_gst_Params.soundInstances[instanceIdx].soundBufElemFromModel, &pan, &front);
    return front;
}

int SND_l_GetFreq(int instanceIdx)
{
    tSoundInstanceElem* inst;
    int result;

    if (instanceIdx >= 0 && instanceIdx < SND_gst_Params.soundInstanceNum && (inst = &SND_gst_Params.soundInstances[instanceIdx], (inst->flags & 0x400) != 0))
    {
        result = inst->freq;
    }
    else
    {
        result = 0;
    }
    return result;
}

float SND_f_GetVol(int instanceIdx)
{
    tSoundInstanceElem* inst;
    float result;

    if (instanceIdx >= 0
        && instanceIdx < SND_gst_Params.soundInstanceNum
        && (inst = &SND_gst_Params.soundInstances[instanceIdx], (inst->flags & 0x400) != 0))
    {
        result = inst->volume;
    }
    else
    {
        result = 0.0;
    }
    return result;
}

void SND_SetDoppler(int instanceIdx, int enabled)
{
    tSoundInstanceElem* inst;
    int flags;

    if (instanceIdx >= 0 && instanceIdx < SND_gst_Params.soundInstanceNum)
    {
        inst = &SND_gst_Params.soundInstances[instanceIdx];
        flags = inst->flags;
        if ((flags & 0x400) != 0)
        {
            if (enabled)
                inst->flags = flags | 0x1000;
            else
                inst->flags = flags & ~0x1000;
        }
    }
}

int SND_i_MuteIsEnable(int group)
{
    int result;

    if (!SND_gst_Params.soundEnabled)
        return 0;
    switch (group)
    {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 6:
    case 7:
    case 8:
    case 9:
    case 10:
    case 11:
        result = SND_Mutes_B30600_fadergroups[group].muteEnabled;
        break;
    default:
        return 0;
    }
    return result;
}

float SND_f_CurrentPosRatio(int instanceIdx)
{
    tSoundInstanceElem* inst;
    int currPos;
    int totalPos;

    if (instanceIdx < 0)
        return 0.0;
    if (instanceIdx >= SND_gst_Params.soundInstanceNum)
        return 0.0;
    inst = &SND_gst_Params.soundInstances[instanceIdx];
    if ((inst->flags & 0x400) == 0)
        return 0.0;
    inst->soundIntf->GetCurrPos(inst->soundBufElemFromModel, &currPos, &totalPos);
    return (float)((double)currPos / (double)SND_gst_Params.allocatedSounds[inst->allocatedSoundsIdx].fullsoundLen);
}

int SND_i_GetTrack(int instanceIdx)
{
    tSoundInstanceElem* inst;
    int result;

    if (instanceIdx >= 0
        && instanceIdx < SND_gst_Params.soundInstanceNum
        && (inst = &SND_gst_Params.soundInstances[instanceIdx], (inst->flags & 0x400) != 0))
    {
        result = inst->trackID;
    }
    else
    {
        result = -1;
    }
    return result;
}

void SND_LoadingSoundNotifyEndLoading()
{
    if (SND_gst_LoadingEffect.mode)
    {
        if (SND_gst_LoadingEffect.mode == 1)
        {
            if (SND_gst_LSGlobal.soundInstanceIndex >= 0)
            {
                SND_gst_LoadingEffect.i4 = (float)((1.0 - SND_gst_LoadingEffect.i5) / SND_gst_LoadingEffect.f_fadeInTime);
                SND_MuteSetUp(6, 1.0, SND_gst_LoadingEffect.i5);
                SND_MuteEnable(6, 1);
                SND_gst_LoadingEffect.mode = 2;
                return;
            }
            SND_gst_LoadingEffect.i4 = 1.0;
        }
        SND_gst_LoadingEffect.mode = 0;
        SND_MuteEnable(6, 0);
    }
}

void SND_LoadingSoundNotifyBeginLoading()
{
    SND_gst_LoadingEffect.mode = 1;
    SND_gst_LoadingEffect.i5 = 1.0;
    SND_gst_LoadingEffect.i4 = (float)(1.0 / SND_gst_LoadingEffect.f_fadeOutTime);
    SND_MuteSetUp(6, 1.0, 1.0);
    SND_MuteEnable(6, 1);
    SND_PlaySoundLooping(SND_gst_LSGlobal.soundInstanceIndex);
}

int SND_i_Seti(int type, int value)
{
    if (!SND_gst_Params.soundEnabled)
        return -1;
    switch (type)
    {
    case -2:
    case -1:
        return 0;
    case 7:
        if (value)
        {
            if ((SND_gst_Params.flags & 0x1000) == 0 && SND_gst_Params.sndMatrixRef)
            {
                SND_gst_Params.flags |= 0x1000;
                MATH_CopyMatrix(&SND_gst_Params.sndMatrix2, SND_gst_Params.sndMatrixRef);
                SND_gst_Params.sndMatrixRefBackup = SND_gst_Params.sndMatrixRef;
                SND_gst_Params.sndMatrixRef = &SND_gst_Params.sndMatrix2;
            }
            return 0;
        }
        if ((SND_gst_Params.flags & 0x1000) != 0)
        {
            SND_gst_Params.flags &= ~0x1000u;
            SND_gst_Params.sndMatrixRef = SND_gst_Params.sndMatrixRefBackup;
        }
        return 0;
    case 8:
        if (value)
        {
            if ((SND_gst_Params.flags & 0x2000) == 0 && SND_gst_Params.microPosMtxPtr)
            {
                SND_gst_Params.flags |= 0x2000;
                MATH_CopyMatrix(&SND_gst_Params.microPosMtx, SND_gst_Params.microPosMtxPtr);
                SND_gst_Params.microPosMtxPtrBackup = SND_gst_Params.microPosMtxPtr;
                SND_gst_Params.microPosMtxPtr = &SND_gst_Params.microPosMtx;
            }
        }
        else if ((SND_gst_Params.flags & 0x2000) != 0)
        {
            SND_gst_Params.flags &= ~0x2000;
            SND_gst_Params.microPosMtxPtr = SND_gst_Params.microPosMtxPtrBackup;
        }
        return 0;
    case 9:
        if (value)
            SND_LoadingSoundNotifyBeginLoading();
        return 0;
    case 10:
        if (value)
            SND_LoadingSoundNotifyEndLoading();
        return 0;
    case 14:
        return SND_i_ChangeRenderMode(value);
    default:
        return -1;
    }
}

int SND_i_FxGeti(int type)
{
    int result;

    if (!SND_gst_Params.soundEnabled)
        return -1;
    switch (type)
    {
    case 8193:
        result = SND_gst_FxParam.flags & 1;
        break;
    case 8194:
        result = SND_gst_FxParam.mode;
        break;
    case 8195:
        result = SND_gst_FxParam.delay;
        break;
    case 8196:
        result = SND_gst_FxParam.feedback;
        break;
    case 8198:
        result = SND_gst_FxParam.wetPan;
        break;
    case 8199:
        result = SND_gst_FxDefaultParam.flags & 1;
        break;
    case 8200:
        result = SND_gst_FxDefaultParam.mode;
        break;
    case 8201:
        result = SND_gst_FxDefaultParam.delay;
        break;
    case 8202:
        result = SND_gst_FxDefaultParam.feedback;
        break;
    case 8204:
        result = SND_gst_FxDefaultParam.wetPan;
        break;
    default:
        return -1;
    }
    return result;
}

int SND_i_Geti(int type)
{
    int result;

    if (!SND_gst_Params.soundEnabled)
        return -1;
    switch (type)
    {
    case 8:
        result = ((unsigned int)SND_gst_Params.flags >> 13) & 1;
        break;
    case 9:
    case 10:
        result = 0;
        break;
    case 14:
        result = SND_gst_Params.soundRenderMode;
        break;
    case 15:
        result = SND_gst_Params.soundRenderModeMask;
        break;
    default:
        return -1;
    }
    return result;
}

float SND_f_FxGetf(int type)
{
    if (!SND_gst_Params.soundEnabled)
        return -1.0;
    if (type == 8197)
        return SND_gst_FxParam.vol;
    if (type == 8203)
        return SND_gst_FxDefaultParam.vol;
    return -1.0;
}

int SND_l_GetSoundFromInstance(int instanceIdx)
{
    tSoundInstanceElem* inst;
    int result;

    if (instanceIdx >= 0
        && instanceIdx < SND_gst_Params.soundInstanceNum
        && (inst = &SND_gst_Params.soundInstances[instanceIdx], (inst->flags & 0x400) != 0))
    {
        result = inst->allocatedSoundsIdx;
    }
    else
    {
        result = -1;
    }
    return result;
}

int SND_i_Setf(int id, float val)
{
    int result;

    if (!SND_gst_Params.soundEnabled)
        return -1;

    switch (id)
    {
    case 0:
        SND_gst_Params.fx_doppler = val;
        result = 0;
        break;
    case 1:
        SND_gst_Params.f_rolloff = val;
        result = 0;
        break;
    case 2:
        SND_gst_Params.f_surround = val;
        result = 0;
        break;
    case 11:
        SND_gst_LoadingEffect.f_fadeInTime = val;
        result = 0;
        break;
    case 12:
        SND_gst_LoadingEffect.f_fadeOutTime = val;
        result = 0;
        break;
    case 13:
        SND_gst_Params.fx_DopplerSpeed = val;
        result = 0;
        break;
    default:
        return -1;
    }
    return result;
}

float SND_f_Getf(int type)
{
    double result;

    if (!SND_gst_Params.soundEnabled)
        return -1.0;
    switch (type)
    {
    case 0:
        result = SND_gst_Params.fx_doppler;
        break;
    case 1:
        result = SND_gst_Params.f_rolloff;
        break;
    case 2:
        result = SND_gst_Params.f_surround;
        break;
    case 11:
        result = SND_gst_LoadingEffect.f_fadeInTime;
        break;
    case 12:
        result = SND_gst_LoadingEffect.f_fadeOutTime;
        break;
    case 13:
        result = SND_gst_Params.fx_DopplerSpeed;
        break;
    default:
        result = -1.0;
        break;
    }
    return (float)result;
}

void SND_ResetFlags(struct sGameObjectMain* gao, int instanceIdx, int flags)
{
    tSoundInstanceElem* inst;
    int instFlags;

    if (instanceIdx >= 0 && instanceIdx < SND_gst_Params.soundInstanceNum)
    {
        inst = &SND_gst_Params.soundInstances[instanceIdx];
        instFlags = inst->flags;
        if ((instFlags & 0x400) != 0)
        {
            if ((flags & 0x40000000) != 0)
                inst->muteFactorFlags &= ~(flags & 0xFCB);
            else
                inst->flags = instFlags & ~flags;
        }
    }
}

void SND_PauseMdF(struct sSoundMdfStruct* sndMdf)
{
    int flags;

    if (SND_gst_Params.soundEnabled && sndMdf->sndInstanceID >= 0)
    {
        flags = sndMdf->flags;
        if ((flags & 0x84) != 0)
        {
            if ((sndMdf->flags & 0x84) != 4)
                return;
            SND_Pause(sndMdf->sndInstanceID);
            flags = sndMdf->flags;
        }
        sndMdf->flags = flags | 0x80;
    }
}

void SND_StopMdF(struct sSoundMdfStruct* sndMdf)
{
    int instanceIdx;

    if (SND_gst_Params.soundEnabled)
    {
        instanceIdx = sndMdf->sndInstanceID;
        if (instanceIdx >= 0)
        {
            if ((sndMdf->flags & 0x84) == 4)
            {
                sndMdf->flags &= ~4u;
                SND_StopRq(instanceIdx);
                return;
            }
            if ((sndMdf->flags & 0x84) != 0x80)
            {
                if ((sndMdf->flags & 0x84) != 0x84)
                    return;
                SND_Stop(sndMdf->sndInstanceID);
            }
            sndMdf->flags &= ~0x84u;
        }
    }
}

void SND_PlayFamilly(int mask, int bits)
{
    int idx;
    int scan;
    int* flagsPtr;
    int familyBitmask;

    if (SND_gst_Params.soundEnabled)
    {
        idx = 0;
        if (SND_gst_Params.soundInstanceNum > 0)
        {
            scan = 0;
            do
            {
                flagsPtr = &SND_gst_Params.soundInstances[scan].flags;
                if ((*flagsPtr & 0x400) != 0)
                {
                    familyBitmask = SND_gst_Params.soundInstances[scan].familyBitmask;
                    if ((mask & familyBitmask) == mask && (familyBitmask & bits) != 0 && !SND_i_IsPlaying(idx))
                    {
                        if ((*flagsPtr & 4) != 0)
                            SND_PlaySoundLooping(idx);
                        else
                            SND_PlaySound(idx);
                    }
                }
                ++idx;
                ++scan;
            } while (idx < SND_gst_Params.soundInstanceNum);
        }
    }
}

void SND_StopFamilly(int mask, int bits)
{
    int idx;
    tSoundInstanceElem* instances;
    int scan;

    if (SND_gst_Params.soundEnabled)
    {
        idx = 0;
        if (SND_gst_Params.soundInstanceNum > 0)
        {
            instances = SND_gst_Params.soundInstances;
            scan = 0;
            do
            {
                if ((instances[scan].flags & 0x400) != 0 && (mask & instances[scan].familyBitmask) == mask && (instances[scan].familyBitmask & bits) != 0)
                {
                    SND_Stop(idx);
                    instances = SND_gst_Params.soundInstances;
                }
                ++idx;
                ++scan;
            } while (idx < SND_gst_Params.soundInstanceNum);
        }
    }
}

static tSoundFamilyInnerStruct* SND_pst_FamillyInnerNew()
{
    tSoundFamilyInnerStruct* result;

    result = (tSoundFamilyInnerStruct*)MEM_p_Alloc(sizeof(tSoundFamilyInnerStruct));
    if (result)
    {
        result->i1 = 0;
        result->muteSetup1.muteEnabled = 0;
        result->muteSetup1.muteSetUp1 = 1.0f;
        result->muteSetup1.muteSetUp2 = 0.5f;
        result->muteSetup2.muteEnabled = 0;
        result->muteSetup2.muteSetUp1 = 1.0f;
        result->muteSetup2.muteSetUp2 = 1.0f;
        result->muteSetup2TimeMultiplier = -10000.0f;
        result->muteSetup1TimeMultiplier = 10000.0f;
    }
    return result;
}

tSoundFamilyStruct* SND_pst_GetOrArray(int mask, int bits)
{
    int famillyNb;
    int innerIdx;
    int idx;
    tSoundFamilyStruct* list;
    tSoundFamilyStruct* scan;
    tSoundFamilyStruct* familly;
    size_t size;
    tSoundFamilyStruct* grown;
    int remaining;
    tSoundFamilyInnerStruct* shared;
    int* bitsPtr;
    int entryBits;
    int keptBits;
    tSoundFamilyInnerStruct* inner;
    tSoundFamilyInnerStruct** inners;
    int newInnerNum;
    tSoundFamilyInnerStruct* added;
    tSoundFamilyInnerStruct* newInner;
    int oldInnerNum;
    size_t innerSize;
    tSoundFamilyInnerStruct** grownInners;
    int innerNumBefore;
    tSoundFamilyInnerStruct* linked;
    int wanted;
    int* bitsPtrSaved;

    famillyNb = SND_gi_AndFamillyNb;
    innerIdx = 0;
    idx = 0;
    list = SND_gast_AndFamillyList;
    if (SND_gi_AndFamillyNb <= 0)
    {
        familly = (tSoundFamilyStruct*)bits; // TODO FIXME: ez mi, jó ez így? kidebuggolni
    }
    else
    {
        scan = SND_gast_AndFamillyList;
        do
        {
            familly = scan;
            if (mask == scan->i1)
                break;
            ++idx;
            ++scan;
        } while (idx < SND_gi_AndFamillyNb);
    }
    if (idx == SND_gi_AndFamillyNb)
    {
        if (SND_gi_AndFamillyNb >= SND_gi_AndFamillyListSize)
        {
            size = 16 * (9 * SND_gi_AndFamillyListSize + sizeof(tSoundFamilyStruct));
            if (SND_gi_AndFamillyListSize)
                grown = (tSoundFamilyStruct*)MEM_p_Realloc(SND_gast_AndFamillyList, size);
            else
                grown = (tSoundFamilyStruct*)MEM_p_Alloc(size);
            SND_gast_AndFamillyList = grown;
            memset(&grown[SND_gi_AndFamillyNb], 0, size);
            list = SND_gast_AndFamillyList;
            SND_gi_AndFamillyListSize += 16;
            famillyNb = SND_gi_AndFamillyNb;
            innerIdx = 0;
        }
        familly = &list[famillyNb];
        SND_gi_AndFamillyNb = famillyNb + 1;
        familly->i1 = mask;
        familly->innerNum = 0;
        familly->i2 = 0;
        familly->i4[0] = bits;
        familly->unkIntArrPtr = 0;
    }
    if ((bits & familly->i2) == 0)
    {
        newInner = SND_pst_FamillyInnerNew();
        oldInnerNum = familly->innerNum;
        innerSize = 4 * oldInnerNum + 4;
        if (oldInnerNum)
            grownInners = (tSoundFamilyInnerStruct**)MEM_p_Realloc(familly->unkIntArrPtr, innerSize);
        else
            grownInners = (tSoundFamilyInnerStruct**)MEM_p_Alloc(innerSize);
        innerNumBefore = familly->innerNum;
        familly->unkIntArrPtr = grownInners;
        memset(&familly->unkIntArrPtr[innerNumBefore], 0, innerSize);
        familly->unkIntArrPtr[familly->innerNum] = newInner;
        linked = familly->unkIntArrPtr[familly->innerNum];
        ++linked->i1;
        familly->i4[familly->innerNum++] = bits;
        familly->i2 |= bits;
        return familly;
    }
    remaining = bits;
    shared = 0;
    wanted = bits;
    if (familly->innerNum > 0)
    {
        bitsPtr = familly->i4;
        bitsPtrSaved = familly->i4;
        while (1)
        {
            entryBits = *bitsPtr;
            if ((*bitsPtr & remaining) != 0)
            {
                keptBits = entryBits & ~remaining;
                if (keptBits)
                {
                    if (shared)
                    {
                        inner = familly->unkIntArrPtr[innerIdx];
                        if (inner != shared)
                        {
                            if (inner->i1 <= 1)
                            {
                                SND_FamillyInnerRelease(familly->unkIntArrPtr[innerIdx]);
                                bitsPtr = bitsPtrSaved;
                                ++shared->i1;
                                familly->unkIntArrPtr[innerIdx] = shared;
                                *bitsPtrSaved = keptBits;
                            }
                            else
                            {
                                --inner->i1;
                                familly->unkIntArrPtr[innerIdx] = shared;
                                *bitsPtr = keptBits;
                            }
                        }
                    }
                    else
                    {
                        shared = familly->unkIntArrPtr[innerIdx];
                    }
                }
                else
                {
                    wanted &= ~entryBits;
                    if (!wanted)
                    {
                        familly->i2 |= bits;
                        return familly;
                    }
                }
            }
            remaining = wanted;
            ++innerIdx;
            bitsPtrSaved = ++bitsPtr;
            if (innerIdx >= familly->innerNum)
                break;
        }
    }
    if (!remaining)
    {
        familly->i2 |= bits;
        return familly;
    }
    if (!shared)
        shared = SND_pst_FamillyInnerNew();
    familly->unkIntArrPtr = (tSoundFamilyInnerStruct**)MEM_p_Realloc(familly->unkIntArrPtr, 4 * familly->innerNum + 4);
    familly->i4[familly->innerNum] = remaining;
    inners = familly->unkIntArrPtr;
    newInnerNum = familly->innerNum + 1;
    familly->innerNum = newInnerNum;
    // preserved: the realloc above sizes the array for innerNum+1 pointers, so this
    // stores one past its end and leaves the slot i4[] was filled for uninitialised
    inners[newInnerNum] = shared;
    added = familly->unkIntArrPtr[familly->innerNum];
    ++added->i1;
    familly->i2 |= bits;
    return familly;
}

void SND_MuteOnFamilly(int mask, int x)
{
    tSoundFamilyStruct* familly;
    tMutesSetupStruct* fader = NULL; // TODO FIXME: ez biztos jó? a játék nem inicializálja ezt
    int idx;
    int* bitsPtr;
    int instIdx;
    tSoundInstanceElem* instances;
    int scan;

    if (SND_gst_Params.soundEnabled)
    {
        familly = SND_pst_GetOrArray(mask, x);
        idx = 0;
        if (familly->innerNum > 0)
        {
            bitsPtr = familly->i4;
            do
            {
                if ((x & *bitsPtr) != 0)
                {
                    fader = &familly->unkIntArrPtr[idx]->muteSetup1;
                    fader->muteEnabled = 1;
                }
                ++idx;
                ++bitsPtr;
            } while (idx < familly->innerNum);
        }
        instIdx = 0;
        if (SND_gst_Params.soundInstanceNum > 0)
        {
            instances = SND_gst_Params.soundInstances;
            scan = 0;
            do
            {
                if ((instances[scan].flags & 0x400) != 0 && (mask & instances[scan].familyBitmask) == mask && (instances[scan].familyBitmask & x) != 0)
                {
                    instances[scan].unkMuteSetupSeeInitVolTable = fader;
                    instances = SND_gst_Params.soundInstances;
                }
                ++instIdx;
                ++scan;
            } while (instIdx < SND_gst_Params.soundInstanceNum);
        }
    }
}

void SND_MuteOffFamilly(int mask, int bits)
{
    tSoundFamilyStruct* familly;
    tMutesSetupStruct* fader = NULL; // TODO FIXME: ez biztos jó? a játék nem inicializálja ezt
    int idx;
    int* bitsPtr; 
    int instIdx; 
    tSoundInstanceElem* instances;
    int scan;

    if (SND_gst_Params.soundEnabled)
    {
        familly = SND_pst_GetOrArray(mask, bits);
        idx = 0;
        if (familly->innerNum > 0)
        {
            bitsPtr = familly->i4;
            do
            {
                if ((bits & *bitsPtr) != 0)
                {
                    fader = &familly->unkIntArrPtr[idx]->muteSetup1;
                    fader->muteEnabled = 0;
                }
                ++idx;
                ++bitsPtr;
            } while (idx < familly->innerNum);
        }
        instIdx = 0;
        if (SND_gst_Params.soundInstanceNum > 0)
        {
            instances = SND_gst_Params.soundInstances;
            scan = 0;
            do
            {
                if ((instances[scan].flags & 0x400) != 0 && (mask & instances[scan].familyBitmask) == mask && (instances[scan].familyBitmask & bits) != 0)
                {
                    instances[scan].unkMuteSetupSeeInitVolTable = fader;
                    instances = SND_gst_Params.soundInstances;
                }
                ++instIdx;
                ++scan;
            } while (instIdx < SND_gst_Params.soundInstanceNum);
        }
    }
}

void SND_SetfFamilly(int mask, int bits, int type, float value)
{
    long double magnitude;
    tSoundFamilyStruct* familly;
    int idx;

    if (!SND_gst_Params.soundEnabled)
        return;
    switch (type)
    {
    case 3:
    case 4:
        magnitude = fabs(value);
        value = (float)magnitude;
        break;
    case 5:
        magnitude = fabs(value);
        if (magnitude == 0.0)
        {
            value = 100.0f;
        }
        else
        {
            magnitude = 1.0 / fabs(magnitude);
            value = (float)magnitude;
        }
        break;
    case 6:
        break;
    default:
        return;
    }
    familly = SND_pst_GetOrArray(mask, bits);
    for (idx = 0; idx < familly->innerNum; ++idx)
    {
        if ((bits & familly->i4[idx]) == 0)
            continue;
        switch (type)
        {
        case 3:
            familly->unkIntArrPtr[idx]->muteSetup1.muteSetUp2 = value;
            break;
        case 4:
            familly->unkIntArrPtr[idx]->muteSetup1.muteSetUp1 = value;
            break;
        case 5:
            familly->unkIntArrPtr[idx]->muteSetup1TimeMultiplier = value;
            break;
        case 6:
            familly->unkIntArrPtr[idx]->muteSetup2TimeMultiplier = -value;
            break;
        default:
            return;
        }
    }
}

float SND_f_GetfFamilly(int mask, int bits, int type)
{
    double result;
    tSoundFamilyStruct* familly;
    int innerNum;
    int i;

    if (!SND_gst_Params.soundEnabled)
        return -1.0;
    result = -1.0;
    switch (type)
    {
    case 3:
    case 4:
    case 5:
    case 6:
        familly = SND_pst_GetOrArray(mask, bits);
        result = (float)-1.0;
        innerNum = familly->innerNum;
        for (i = 0; i < innerNum; ++i)
        {
            if ((bits & familly->i4[i]) != 0)
            {
                switch (type)
                {
                case 3:
                    result = familly->unkIntArrPtr[i]->muteSetup1.muteSetUp2;
                    break;
                case 4:
                    result = familly->unkIntArrPtr[i]->muteSetup1.muteSetUp1;
                    break;
                case 5:
                    result = 1.0 / familly->unkIntArrPtr[i]->muteSetup1TimeMultiplier;
                    break;
                case 6:
                    result = -(1.0 / familly->unkIntArrPtr[i]->muteSetup2TimeMultiplier);
                    break;
                default:
                    break;
                }
                i = familly->innerNum;
            }
        }
        break;
    default:
        return (float)result;
    }
    return (float)result;
}

int SND_i_GetStatusFromFamilly(int mask, int bits)
{
    tSoundInstanceElem* instances;
    int idx;

    if (!SND_gst_Params.soundEnabled)
        return 0;
    instances = SND_gst_Params.soundInstances;
    for (idx = 0; idx < SND_gst_Params.soundInstanceNum; ++idx)
    {
        if ((instances[idx].flags & 0x400) != 0
            && (mask & instances[idx].familyBitmask) == mask
            && (instances[idx].familyBitmask & bits) != 0)
        {
            if (SND_i_IsPlaying(idx))
                return 1;
            instances = SND_gst_Params.soundInstances;
        }
    }
    return 0;
}

void SND_FadeInFamilly(int mask, int bits)
{
    tSoundFamilyStruct* familly;
    tMutesSetupStruct* fader = NULL;  // TODO FIXME: ez biztos jó? a játék nem inicializálja ezt
    int idx;
    int* bitsPtr;
    int instIdx;
    tSoundInstanceElem* instances;
    int scan;

    if (SND_gst_Params.soundEnabled)
    {
        familly = SND_pst_GetOrArray(mask, bits);
        idx = 0;
        if (familly->innerNum > 0)
        {
            bitsPtr = familly->i4;
            do
            {
                if ((bits & *bitsPtr) != 0)
                {
                    fader = &familly->unkIntArrPtr[idx]->muteSetup2;
                    fader->muteEnabled = 1;
                    familly->unkIntArrPtr[idx]->muteSetup2.muteSetUp2 = 0.0;
                }
                ++idx;
                ++bitsPtr;
            } while (idx < familly->innerNum);
        }
        instIdx = 0;
        if (SND_gst_Params.soundInstanceNum > 0)
        {
            instances = SND_gst_Params.soundInstances;
            scan = 0;
            do
            {
                if ((instances[scan].flags & 0x400) != 0 && (mask & instances[scan].familyBitmask) == mask && (instances[scan].familyBitmask & bits) != 0)
                {
                    instances[scan].unkMuteSetupSeeInitVolTable = fader;
                    instances = SND_gst_Params.soundInstances;
                }
                ++instIdx;
                ++scan;
            } while (instIdx < SND_gst_Params.soundInstanceNum);
        }
    }
}

void SND_FadeOutFamilly(int mask, int bits)
{
    int maskAlias;
    tSoundFamilyStruct* familly;
    tMutesSetupStruct* fader = NULL; // TODO FIXME: ez biztos jó? a játék nem inicializálja ezt
    int idx;
    int* bitsPtr;
    int instIdx;
    tSoundInstanceElem* instances;
    int scan;

    if (SND_gst_Params.soundEnabled)
    {
        maskAlias = mask;
        familly = SND_pst_GetOrArray(mask, bits);
        idx = 0;
        if (familly->innerNum > 0)
        {
            bitsPtr = familly->i4;
            do
            {
                if ((bits & *bitsPtr) != 0)
                {
                    fader = &familly->unkIntArrPtr[idx]->muteSetup2;
                    fader->muteEnabled = 2;
                    familly->unkIntArrPtr[idx]->muteSetup2.muteSetUp2 = 1.0;
                }
                ++idx;
                ++bitsPtr;
            } while (idx < familly->innerNum);
            maskAlias = mask;
        }
        instIdx = 0;
        if (SND_gst_Params.soundInstanceNum > 0)
        {
            instances = SND_gst_Params.soundInstances;
            scan = 0;
            do
            {
                if ((instances[scan].flags & 0x400) != 0 && (maskAlias & instances[scan].familyBitmask) == maskAlias && (instances[scan].familyBitmask & bits) != 0)
                {
                    instances[scan].unkMuteSetupSeeInitVolTable = fader;
                    instances = SND_gst_Params.soundInstances;
                }
                ++instIdx;
                ++scan;
            } while (instIdx < SND_gst_Params.soundInstanceNum);
        }
    }
}

void SND_UnlinkFromFamilly(int mask, int instanceIdx)
{
    tSoundInstanceElem* inst;

    if (instanceIdx >= 0 && instanceIdx < SND_gst_Params.soundInstanceNum)
    {
        inst = &SND_gst_Params.soundInstances[instanceIdx];
        if ((inst->flags & 0x400) != 0)
            inst->familyBitmask &= ~mask;
    }
}

static void SND_TrackStopRq(unsigned int trackKey)
{
    int trackIdx;
    int instanceIdx;

    if (SND_gst_Params.soundEnabled)
    {
        trackIdx = BAS_bsearch(trackKey, &SND_gst_TrackId);
        if (trackIdx != -1)
        {
            instanceIdx = SND_gax_TrackPlayingList[trackIdx].trackID;
            if (instanceIdx >= 0)
                SND_StopRq(instanceIdx);
        }
    }
}

int SND_i_TrackSeti(unsigned int trackKey, int type, int value)
{
    int trackIdx;
    tTrackListElem* track;
    int muted;
    int instanceIdx;

    if (!SND_gst_Params.soundEnabled)
        return -1;
    trackIdx = BAS_bsearch(trackKey, &SND_gst_TrackId);
    if (trackIdx == -1)
        return -1;
    track = &SND_gax_TrackPlayingList[trackIdx];
    if (type == 1)
    {
        if (value)
            track->flags |= 0x20;
        else
            track->flags &= ~0x20u;
        return 0;
    }
    if (type == 2)
    {
        muted = track->unk1;
        if (value && !muted)
            track->unk1 = 1;
        else if (!value && muted)
            track->unk1 = 0;
        else
            return 0;
        instanceIdx = track->trackID;
        if (instanceIdx >= 0)
            SND_SetInstVolume(&SND_gst_Params.soundInstances[instanceIdx]);
        return 0;
    }
    if (type != 4)
        return -1;
    SND_TrackStopRq(trackKey);
    return 0;
}

int SND_i_TrackGeti(unsigned int trackKey, int type)
{
    int trackIdx;
    tTrackListElem* track;

    if (!SND_gst_Params.soundEnabled)
        return -1;
    trackIdx = BAS_bsearch(trackKey, &SND_gst_TrackId);
    if (trackIdx == -1)
        return -1;
    track = &SND_gax_TrackPlayingList[trackIdx];
    if (type == 1)
        return ((unsigned int)track->flags >> 5) & 1;
    if (type == 2)
        return track->unk1 != 0;
    if (type != 5)
        return -1;
    return track->trackID;
}

void SND_TrackSetFadeOut(int key, float duration)
{
    int trackIdx;
    int* trackIDPtr;
    int instanceIdx;
    int instanceIdxAlias;
    int flags;
    tSoundInstanceElem* inst;
    tSoundModifierDataFade fade;

    if (SND_gst_Params.soundEnabled)
    {
        if (duration > 0.0)
        {
            trackIdx = BAS_bsearch(key, &SND_gst_TrackId);
            if (trackIdx != -1)
            {
                trackIDPtr = &SND_gax_TrackPlayingList[trackIdx].trackID;
                instanceIdx = *trackIDPtr;
                if (*trackIDPtr >= 0 && instanceIdx < SND_gst_Params.soundInstanceNum)
                {
                    instanceIdxAlias = instanceIdx;
                    flags = SND_gst_Params.soundInstances[instanceIdxAlias].flags;
                    inst = &SND_gst_Params.soundInstances[instanceIdxAlias];
                    if ((flags & 0x400) != 0)
                    {
                        fade.i3 = duration;
                        fade.i2 = 0.0;
                        fade.i1 = inst->volume;
                        fade.i5 = 0x80;
                        fade.gen.flags = 0;
                        fade.gen.sndKey = SND_gst_Params.allocatedSounds[inst->allocatedSoundsIdx].soundKey;
                        fade.gen.sndIndexInAllocatedSounds = inst->allocatedSoundsIdx;
                        SND_SModifierAddInInstance(*trackIDPtr, 4, &fade, 36);
                    }
                }
            }
        }
    }
}

int SND_i_TrackSetf(int trackKey, int type, float value)
{
    int trackIdx;
    tTrackListElem* track;
    int instanceIdx;

    if (!SND_gst_Params.soundEnabled)
        return -1;
    trackIdx = BAS_bsearch(trackKey, &SND_gst_TrackId);
    if (trackIdx == -1)
        return -1;
    track = &SND_gax_TrackPlayingList[trackIdx];
    if (type == 1)
        return 0;
    if (type == 3)
    {
        track->f1 = value;
        instanceIdx = track->trackID;
        if (instanceIdx >= 0)
            SND_SetInstVolume(&SND_gst_Params.soundInstances[instanceIdx]);
        return 0;
    }
    if (type != 6)
        return -1;
    SND_TrackSetFadeOut(trackKey, value);
    return 0;
}

float SND_f_TrackGetf(unsigned int trackKey, int type)
{
    int trackIdx;
    tTrackListElem* track;
    double result;
    int instanceIdx;

    if (!SND_gst_Params.soundEnabled)
        return -1.0f;
    trackIdx = BAS_bsearch(trackKey, &SND_gst_TrackId);
    if (trackIdx == -1)
        return -1.0f;
    track = &SND_gax_TrackPlayingList[trackIdx];
    if (type == 1)
    {
        // preserved: both arms assign the same value, so the whole test is dead
        if ((track->flags & 0x20) == 0
            || (instanceIdx = track->trackID, instanceIdx >= 0)
            && instanceIdx < SND_gst_Params.soundInstanceNum
            && (SND_gst_Params.soundInstances[instanceIdx].flags & 0x400) != 0)
        {
            result = 0.0;
        }
        else
        {
            result = 0.0;
        }
    }
    else
    {
        if (type != 3)
            return -1.0f;
        result = track->f1;
    }
    return (float)result;
}

void SND_MergeBank(tGameObjectMain* dest, tGameObjectMain* src, int firstIdx, int lastIdx, int overwrite)
{
    int idx;
    tGameObjectExtended* srcExtended;
    tSndBank* srcBank;
    tGameObjectExtended* destExtended;
    tSndBank* destBank;
    int last;
    int elemNum;
    int* copy;
    int* original;
    unsigned int size;
    int destSoundIdx;
    int srcSoundIdx;
    tSndSoundElem* sound;
    int bound;

    idx = firstIdx;
    if (firstIdx < 0)
        return;
    if (!src || (src->flags & 0x8000000) == 0)
        return;
    srcExtended = src->extended;
    if (!srcExtended)
        return;
    srcBank = srcExtended->snd;
    if (!srcBank || !srcBank->soundIndices)
        return;
    if (!dest || (dest->flags & 0x8000000) == 0)
        return;
    destExtended = dest->extended;
    if (!destExtended)
        return;
    destBank = destExtended->snd;
    if (!destBank || !destBank->soundIndices)
        return;
    last = lastIdx;
    if (lastIdx == -1)
    {
        bound = destBank->elemNum;
        if (srcBank->elemNum <= bound)
            bound = srcBank->elemNum;
        lastIdx = bound - 1;
        last = lastIdx;
    }
    if (last >= srcBank->elemNum)
        return;
    elemNum = destBank->elemNum;
    if (last >= elemNum)
        return;
    if (!destBank->soundIndicesV2)
    {
        destBank->soundIndicesV2 = destBank->soundIndices;
        copy = (int*)MEM_p_Alloc(4 * elemNum);
        original = destBank->soundIndicesV2;
        last = lastIdx;
        size = 4 * destBank->elemNum;
        destBank->soundIndices = copy;
        memcpy(copy, original, size);
        idx = firstIdx;
    }
    for (; idx <= last; ++idx)
    {
        destSoundIdx = destBank->soundIndices[idx];
        srcSoundIdx = srcBank->soundIndices[idx];
        if (destSoundIdx != srcSoundIdx && srcSoundIdx != -1 && (destSoundIdx == -1 || overwrite))
        {
            sound = &SND_gst_Params.allocatedSounds[srcSoundIdx];
            if ((sound->flags & 0x400) != 0)
                ++sound->usageCnt;
            if (destBank->soundIndices[idx] != destBank->soundIndicesV2[idx])
            {
                SND_DeleteByIndex(destBank->soundIndices[idx]);
                last = lastIdx;
            }
            destBank->soundIndices[idx] = srcBank->soundIndices[idx];
        }
    }
}

void SND_MergeBankOver(struct sGameObjectMain* src, struct sGameObjectMain* dest)
{
    SND_MergeBank(dest, src, 0, -1, 1);
}

void SND_MergeBankNoOver(struct sGameObjectMain* src, struct sGameObjectMain* dest)
{
    SND_MergeBank(dest, src, 0, -1, 0);
}

void SND_PrefetchMdF(struct sSoundMdfStruct* sndMdf)
{
    int soundIdx;

    if (SND_gst_Params.soundEnabled)
    {
        MDF_SndGetSoundIndexFromBank(sndMdf);
        soundIdx = sndMdf->idx;
        if (soundIdx >= 0)
            SND_StreamPrefetchOneSound(soundIdx, 1);
    }
}

void SND_StreamPrefetch(int instanceIdx)
{
    tSoundInstanceElem* inst;
    int soundIdx;
    tSndSoundElem* sound;
    int startPos;
    unsigned int len;

    if (instanceIdx >= 0 && instanceIdx < SND_gst_Params.soundInstanceNum)
    {
        inst = &SND_gst_Params.soundInstances[instanceIdx];
        if ((inst->flags & 0x400) != 0)
        {
            soundIdx = inst->allocatedSoundsIdx;
            sound = &SND_gst_Params.allocatedSounds[inst->allocatedSoundsIdx];
            if ((sound->flags & 0x86000) != 0)
            {
                startPos = sound->startDataPos;
                if (startPos)
                {
                    if (startPos != -1)
                    {
                        len = sound->fullsoundLen;
                        if (len)
                        {
                            if (len >= 0x25800)
                                SND_StreamPrefetchOneSound(soundIdx, 1);
                        }
                    }
                }
            }
        }
    }
}

void SND_FlushMdF(tSoundMdfStruct* sndMdf)
{
    int soundIdx;

    if (SND_gst_Params.soundEnabled)
    {
        MDF_SndGetSoundIndexFromBank(sndMdf);
        soundIdx = sndMdf->idx;
        if (soundIdx >= 0)
            SND_StreamFlushOneSound_0(soundIdx, 1);
    }
}

void SND_StreamFlush(int instanceIdx)
{
    tSoundInstanceElem* inst;
    int soundIdx;
    tSndSoundElem* sound;
    int startPos;

    if (instanceIdx >= 0 && instanceIdx < SND_gst_Params.soundInstanceNum)
    {
        inst = &SND_gst_Params.soundInstances[instanceIdx];
        if ((inst->flags & 0x400) != 0)
        {
            soundIdx = inst->allocatedSoundsIdx;
            sound = &SND_gst_Params.allocatedSounds[inst->allocatedSoundsIdx];
            if ((sound->flags & 0x86000) != 0)
            {
                startPos = sound->startDataPos;
                if (startPos)
                {
                    if (startPos != -1)
                        SND_StreamFlushOneSound_0(soundIdx, 1);
                }
            }
        }
    }
}

int SND_i_InstanceSeti_return_negative_1(int i1, int i2, int i3)
{
    return -1;
}

int SND_i_InstanceGeti(int instanceIdx, int type)
{
    tSoundInstanceElem* inst;
    int status;

    if (instanceIdx >= 0 && instanceIdx < SND_gst_Params.soundInstanceNum)
    {
        inst = &SND_gst_Params.soundInstances[instanceIdx];
        if ((inst->flags & 0x400) != 0)
        {
            if (type == 4112)
            {
                inst->soundIntf->GetStatus(inst->soundBufElemFromModel, &status);
                return ((unsigned int)status >> 4) & 1;
            }
            if (type == 4113)
                return inst->familyBitmask;
        }
    }
    return -1;
}

float SND_f_InstanceGetf(int instanceIdx, int type)
{
    tSoundInstanceElem* ie;
    double result;
    tSndSoundElem* se;

    if (instanceIdx < 0)
        return -1.0f;
    if (instanceIdx >= SND_gst_Params.soundInstanceNum)
        return -1.0f;

    ie = &SND_gst_Params.soundInstances[instanceIdx];

    if ((ie->flags & 0x400) == 0)
        return -1.0f;
    switch (type)
    {
    case 4099:
        result = ie->centerArr[0];
        break;
    case 4100:
        result = ie->centerArr[1];
        break;
    case 4101:
        result = ie->centerArr[2];
        break;
    case 4102:
        result = ie->farCoefficient;
        break;
    case 4103:
        result = ie->middleCoefficient;
        break;
    case 4104:
        result = ie->dtFar;
        break;
    case 4105:
        result = ie->minPan;
        break;
    case 4106:
        result = ie->nearArr[0];
        break;
    case 4107:
        result = ie->farArr[0];
        break;
    case 4108:
        result = ie->nearArr[1];
        break;
    case 4109:
        result = ie->farArr[1];
        break;
    case 4110:
        result = ie->nearArr[2];
        break;
    case 4111:
        result = ie->farArr[2];
        break;
    case 4114:
        result = ie->timeRemainTillStart;
        break;
    case 4116:
        se = &SND_gst_Params.allocatedSounds[ie->allocatedSoundsIdx];
        if ((se->flags & 0x400) != 0)
            result = (double)se->fullsoundLen / (double)se->byteRate;
        else
            result = 0.0f;
        break;
    case 4117:
        result = ie->timeFromDefault;
        break;
    default:
        return -1.0f;
    }
    return (float)result;
}

void SND_Fade(int group, float from, float to, float duration)
{
    tMutesSetupStruct* fg;

    switch (group)
    {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 6:
    case 7:
    case 8:
    case 9:
    case 10:
    case 11:
        fg = &SND_Mutes_B30600_fadergroups[group];
        if (duration <= 0.0 || from == to)
        {
            fg->elapsedTime = 0.0;
            fg->flags = 0;
            fg->muteEnabled = 0;
        }
        else
        {
            fg->muteSetUp2ToSetWithThis = to;
            fg->elapsedTime = 0.0;
            fg->i7 = from;
            fg->i6 = (256.0f / duration);
            if (to <= from)
                fg->flags = 2;
            else
                fg->flags = 1;
            fg->muteSetUp2 = from;
            fg->muteSetUp1 = 1.0f;
            fg->muteEnabled = 1;
        }
        break;
    default:
        return;
    }
}

void SND_PlaySoundOnTrackFromTo(int instanceIdx, float fromTime, float toTime)
{
    tSoundInstanceElem* inst;
    int soundIdx;
    double bytesPerSecond;
    double fromRounded;
    double toRounded;
    int status;

    if (instanceIdx >= 0 && instanceIdx < SND_gst_Params.soundInstanceNum)
    {
        inst = &SND_gst_Params.soundInstances[instanceIdx];
        if ((inst->flags & 0x400) != 0)
        {
            if ((inst->muteFactorFlags & 0x1000) != 0
                || (inst->soundIntf->GetStatus(inst->soundBufElemFromModel, &status),
                    (status & 1) == 0))
            {
                soundIdx = inst->allocatedSoundsIdx;
                inst->flags = inst->flags & ~4u | 0x200;
                bytesPerSecond = (double)(2 * inst->freq * SND_gst_Params.allocatedSounds[soundIdx].numChannels);
                if (((inst->freq * SND_gst_Params.allocatedSounds[soundIdx].numChannels) & 0x40000000) != 0)
                    bytesPerSecond = bytesPerSecond + 4294967300.0; // TODO FIXME ....?????!
                fromRounded = fromTime * bytesPerSecond + MATH_gd_magicDoubleForRound;
                toRounded = bytesPerSecond * toTime + MATH_gd_magicDoubleForRound;
                SND_SetInstVolume(inst);
                SND_PlayRq((*(uint32_t *)&fromRounded) & 0xFFFFFFFC, inst, 0x8000000, (*(uint32_t *)&toRounded) & 0xFFFFFFFC);
            }
        }
    }
}

void SND_PlaySoundLoopingOnTrackFrom(int instanceIdx, float fromTime)
{
    tSoundInstanceElem* inst;
    int soundIdx;
    double bytesPerSecond;
    double fromRounded;
    int status;

    if (instanceIdx >= 0 && instanceIdx < SND_gst_Params.soundInstanceNum)
    {
        inst = &SND_gst_Params.soundInstances[instanceIdx];
        if ((inst->flags & 0x400) != 0)
        {
            if ((inst->muteFactorFlags & 0x1000) != 0
                || (inst->soundIntf->GetStatus(inst->soundBufElemFromModel, &status),
                    (status & 1) == 0))
            {
                soundIdx = inst->allocatedSoundsIdx;
                inst->flags = inst->flags & ~4u | 0x200;
                bytesPerSecond = (double)(2 * inst->freq * SND_gst_Params.allocatedSounds[soundIdx].numChannels);
                if (((inst->freq * SND_gst_Params.allocatedSounds[soundIdx].numChannels) & 0x40000000) != 0)
                    bytesPerSecond = bytesPerSecond + 4294967300.0; // TODO FIXME ....?????!
                fromRounded = bytesPerSecond * fromTime + MATH_gd_magicDoubleForRound;
                SND_SetInstVolume(inst);
                SND_PlayRq((*(uint32_t *)&fromRounded) & 0xFFFFFFFC, inst, 0x8000001, -1);
            }
        }
    }
}

float SND_SoundDurationGet(struct sGameObjectMain* gao, int sndIdx)
{
    float byteRate;
    float len;
    tSndSoundElem* sound;

    if (!SND_gst_Params.soundEnabled)
        return 0.0;
    if ((gao->flags & 0x8000000) == 0)
        return 0.0;
    if (!gao->extended)
        return 0.0;
    if (!gao->extended->snd)
        return 0.0;
    if (!gao->extended->snd->soundIndices)
        return 0.0;
    if (sndIdx < 0 || sndIdx >= gao->extended->snd->elemNum)
        return 0.0;
    if (sndIdx >= SND_gst_Params.allocatedSoundsMaxNum)
        return 0.0;
    sound = &SND_gst_Params.allocatedSounds[gao->extended->snd->soundIndices[sndIdx]];
    if ((sound->flags & 0x400) == 0)
        return 0.0;
    if ((sound->flags & 0x200000) == 0)
    {
        if ((sound->flags & 0x4000) != 0 || (sound->flags & 0x2000) != 0 || (sound->flags & 0x80000) != 0)
        {
            SND_l_AddStreamLoad(sound->soundKey);
        }
        else if ((sound->flags & 8) != 0)
        {
            SND_l_LoadLoadingSoundHeader(sound->soundKey);
        }
        else
        {
            SND_l_LoadHeader(sound->soundKey);
        }
    }
    len = (float)sound->fullsoundLen;
    byteRate = (float)sound->byteRate;
    return len / byteRate;
}

void SND_ResetAllMute()
{
    tMutesSetupStruct* fader;

    fader = SND_Mutes_B30600_fadergroups;
    do
    {
        if (fader != &SND_Mutes_B30600_fadergroups[6])
        {
            fader->muteEnabled = 0;
            fader->muteSetUp1 = 1.0f;
            fader->muteSetUp2 = 0.5f;
            fader->flags = 0;
            fader->elapsedTime = 0.0f;
        }
        ++fader;
    } while (fader <= &SND_Mutes_B30600_fadergroups[11]);

    SND_Mutes_B30600_fadergroups[7].muteEnabled = 0;
    SND_Mutes_B30600_fadergroups[7].muteSetUp1 = 1.0;
    SND_Mutes_B30600_fadergroups[7].muteSetUp2 = 1.0;
    SND_Mutes_B30600_fadergroups[8].muteEnabled = 0;
    SND_Mutes_B30600_fadergroups[8].muteSetUp1 = 1.0;
    SND_Mutes_B30600_fadergroups[8].muteSetUp2 = 1.0;
    SND_Mutes_B30600_fadergroups[9].muteEnabled = 0;
    SND_Mutes_B30600_fadergroups[9].muteSetUp1 = 1.0;
    SND_Mutes_B30600_fadergroups[9].muteSetUp2 = 1.0;
    SND_Mutes_B30600_fadergroups[10].muteEnabled = 0;
    SND_Mutes_B30600_fadergroups[10].muteSetUp1 = 1.0;
    SND_Mutes_B30600_fadergroups[10].muteSetUp2 = 1.0;
    SND_Mutes_B30600_fadergroups[11].muteEnabled = 0;
    SND_Mutes_B30600_fadergroups[11].muteSetUp1 = 1.0;
    SND_Mutes_B30600_fadergroups[11].muteSetUp2 = 1.0;
    SND_Mutes_B30600_fadergroups[5].muteEnabled = 0;
    SND_Mutes_B30600_fadergroups[5].muteSetUp1 = 1.0;
    SND_Mutes_B30600_fadergroups[5].muteSetUp2 = 1.0;
}