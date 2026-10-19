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


BOOL SND_b_IsLoadingInstance(int a1)
{
    BOOL result;

    if (a1 >= 0 && a1 < SND_gst_Params.soundInstanceNum)
        result = a1 == SND_gst_LSGlobal.soundInstanceIndex;
    else
        result = 0;
    return result;
}

void SND_DetachLoadingInstance()
{
    tSoundInstanceElem* v0;
    int v1;

    if (SND_gst_LSGlobal.soundInstanceIndex >= 0 && SND_gst_LSGlobal.soundInstanceIndex < SND_gst_Params.soundInstanceNum)
    {
        v0 = &SND_gst_Params.soundInstances[SND_gst_LSGlobal.soundInstanceIndex];
        v1 = v0->flags;
        if ((v1 & 0x400) != 0)
        {
            if ((v1 & 0x1F000000) != 0)
                v0->flags = v1 & ~0x1F000000u;
            v0->flags &= ~0x1000u;
            v0->gao = 0;
            v0->sndPosMtxSetInSetPos = 0;
            v0->sndPositionSetInSetPosPtr = 0;
            SND_gst_LSGlobal.userCallBackFunction = 0;
            SND_gst_LSGlobal.userCallBackFunctionParam = 0;
        }
    }
}

void SND_TrackForceFreeRessources_inner1_sub_499590(int a1)
{
    tTrackListElem* v1;
    int v2;

    v1 = SND_gax_TrackPlayingList;
    v2 = a1;
    SND_gast_TrackResources[SND_gax_TrackPlayingList[a1].trackResourceNum - 1].isBusy = 0;
    v1[v2].trackResourceNum = 0;
    SND_gax_TrackPlayingList[v2].sndBufferElem = 0;
    SND_gax_TrackPlayingList[v2].trackResource = 0;
}

void snd_track_merge_inner_type_1_sub_499C20(int a1)
{
    tTrackListElem* v1;
    __int16 v2;
    int v3;
    int v4;
    tSndSoundBufferElem** v5;
    tSndSoundBufferElem* v6;

    v4 = a1;
    v5 = &SND_gax_TrackPlayingList[a1].sndBufferElem;
    v6 = *v5;
    *v5 = SND_gax_TrackPlayingList[a1].trackResource;
    SND_gax_TrackPlayingList[v4].trackResource = v6;
    SND_gax_TrackPlayingList[v4].flags &= ~4u;
    v1 = &SND_gax_TrackPlayingList[a1];
    v2 = v1->stereoToMono_sub499C20;
    if (v2)
    {
        v3 = v2 - 1;
        v1->stereoToMono_sub499C20 = 0;
        SND_gast_TrackResources[v3].isBusy = 0;
        SND_gpst_Interface.win.StreamGetStereoToMono(&v1->trackResource, &SND_gast_TrackResources[v3].stereoSoundBuffer);
    }
}

void SND_TrackDeleteInstanceFromList(int id)
{
    int v1;
    tSoundInstanceElem* v2;
    int v3;
    int v4;

    if (id >= 0 && id < SND_gst_Params.soundInstanceNum)
    {
        v1 = SND_gst_Params.soundInstances[id].flags;
        v2 = &SND_gst_Params.soundInstances[id];
        if ((v1 & 0x400) != 0)
        {
            v2->flags = v1 & ~0x20010u;
            v3 = BAS_bsearch(v2->trackID, &SND_gst_TrackId);
            if (v3 != -1)
            {
                v4 = v3;
                if (SND_gax_TrackPlayingList[v3].trackID == id)
                {
                    SND_gpst_Interface.win.Stop(SND_gax_TrackPlayingList[v4].sndBufferElem);
                    SND_gax_TrackPlayingList[v4].trackID = -1;
                    SND_gax_TrackPlayingList[v4].flags &= ~2u;
                    v2->soundIntf = SND_gax_TrackPlayingList[v4].soundIntf;
                    SND_gax_TrackPlayingList[v4].soundIntf = 0;
                    BAS_bdelete((unsigned int)v2->soundBufElemFromModel, &SND_gst_TrackLookUpTable); // TODO FIXME, biztos?????
                    if (SND_gax_TrackPlayingList[v4].stereoToMono_sub499C20)
                        snd_track_merge_inner_type_1_sub_499C20(v3);
                    SND_TrackForceFreeRessources_inner1_sub_499590(v3);
                }
            }
        }
    }
}

void SND_TrackForceFreeResources(int idx)
{
    int v1;
    tTrackListElem *i;
    int v3;
    int v4;

    v1 = 0;
    if (SND_gi_TrackNb <= 0)
        return;
    for (i = SND_gax_TrackPlayingList; ; i++)
    {
        if (i->flags == 1 || v1 == idx)
            goto LABEL_7;
        if (i->stereoToMono_sub499C20)
            break;
        if (i->trackResourceNum)
        {
            v4 = v1;
            if ((SND_gax_TrackPlayingList[v1].flags & 2) != 0)
            {
                SND_gpst_Interface.win.Stop(SND_gax_TrackPlayingList[v4].sndBufferElem);
                SND_TrackDeleteInstanceFromList(SND_gax_TrackPlayingList[v4].trackID);
            }
            SND_TrackForceFreeRessources_inner1_sub_499590(v1);
            return;
        }
    LABEL_7:
        if (++v1 >= SND_gi_TrackNb)
            return;
    }
    v3 = v1;
    if ((SND_gax_TrackPlayingList[v3].flags & 2) != 0)
    {
        SND_gpst_Interface.win.Stop(SND_gax_TrackPlayingList[v3].sndBufferElem);
        SND_TrackDeleteInstanceFromList(SND_gax_TrackPlayingList[v3].trackID);
    }
}

void calls_SND_TrackForceFreeResourcesAllTrackOnList_plus_setsomething_first_sub_49A140(int a1)
{
    int v1;
    tTrackResourceElem* v2;
    int v3;

LABEL_1:
    v1 = 0;
    v2 = SND_gast_TrackResources;
    while (v2->isBusy)
    {
        ++v2;
        ++v1;
        if ((int)v2 > (int)&SND_gast_TrackResources[3])
        {
            SND_TrackForceFreeResources(a1);
            goto LABEL_1;
        }
    }
    v3 = a1;
    SND_gax_TrackPlayingList[v3].sndBufferElem = SND_gast_TrackResources[v1].stereoSoundBuffer;
    SND_gax_TrackPlayingList[v3].trackResource = SND_gast_TrackResources[v1].monoSoundBuffer;
    SND_gax_TrackPlayingList[v3].trackResourceNum = v1 + 1;
    SND_gast_TrackResources[v1].isBusy = 1;
}

void SND_FreeInstance(int a1)
{
    tSoundInstanceElem* v1;

    v1 = &SND_gst_Params.soundInstances[a1];
    if ((SND_gst_Params.allocatedSounds[v1->allocatedSoundsIdx].flags & 0x80000000) == 0)
        --SND_AllocatedSoundsNumWithhoutFlag80000000;
    else
        --SND_AllocatedSoundsNumWithFlag80000000;
    if ((v1->flags & 0x20000) != 0)
        SND_TrackDeleteInstanceFromList(a1);
    if ((v1->flags & 0x40) != 0)
    {
        v1->soundIntf->Stop(v1->soundBufElemFromModel);
        v1->soundIntf->Release(v1->soundBufElemFromModel);
    }
    else
    {
        v1->soundIntf->Stop(v1->soundBufElemFromModel);
        SND_gst_Params.allocatedSounds[v1->allocatedSoundsIdx].flags &= ~0x40u;
    }
    SND_SModifierDeleteAll(&v1->modifierInstances);
    v1->allocatedSoundsIdx = -1;
    v1->trackID = -1;
    v1->flags = 0;
    v1->muteFactorFlags = 0;
    v1->familyBitmask = 0;
    v1->freq = 0;
    v1->volume = 0.0;
    v1->front = 0;
    v1->pan = 0;
    v1->minPan = 0;
    v1->nearArr[0] = 0.0;
    v1->nearArr[1] = 0.0;
    v1->nearArr[2] = 0.0;
    v1->farArr[0] = 0.0;
    v1->farArr[1] = 0.0;
    v1->farArr[2] = 0.0;
    v1->dtFar = 0.0;
    v1->centerArr[0] = 0.0;
    v1->centerArr[1] = 0.0;
    v1->centerArr[2] = 0.0;
    v1->farCoefficient = 0;
    v1->middleCoefficient = 0;
    v1->sndPosMtxSetInSetPos = 0;
    v1->sndPositionSetInSetPosPtr = 0;
    v1->sndInstPosVecPrevious.x = 0.0f;
    v1->sndInstPosVecPrevious.y = 0.0f;
    v1->sndInstPosVecPrevious.z = 0.0f;
    v1->sndInstPosVec.x = 0.0;
    v1->sndInstPosVec.y = 0.0;
    v1->sndInstPosVec.z = 0.0;
    v1->modifierInstances = 0;
    v1->soundBufElemFromModel = 0;
    v1->requestTimeStamp = 0.0;
    v1->gao = 0;
    v1->soundIntf = 0;
    v1->unkMuteSetupSeeInitVolTable = 0;
    v1->timeRemainTillStart = 0.0;
    v1->timeFromDefault = 0.0;
}

void SND_DetachSound(int a1)
{
    int v1;
    int v2;
    tSoundInstanceElem* v3;

    if (SND_gst_Params.soundEnabled)
    {
        v1 = 0;
        if (SND_gst_Params.soundInstanceNum > 0)
        {
            v2 = 0;
            do
            {
                v3 = &SND_gst_Params.soundInstances[v2];
                if ((SND_gst_Params.soundInstances[v2].flags & 0x400) != 0 && v3->allocatedSoundsIdx == a1)
                {
                    if (SND_b_IsLoadingInstance(v1))
                    {
                        SND_DetachLoadingInstance();
                    }
                    else
                    {
                        v3->soundIntf->Stop(v3->soundBufElemFromModel);
                        SND_FreeInstance(v1);
                    }
                }
                ++v1;
                ++v2;
            } while (v1 < SND_gst_Params.soundInstanceNum);
        }
    }
}

BOOL SND_b_IsLoadingSound(int a1)
{
    BOOL result;

    if (a1 >= 0 && a1 < SND_gst_Params.allocatedSoundsMaxNum)
        result = a1 == SND_gst_LSGlobal.soundElemIndex;
    else
        result = 0;
    return result;
}

void SND_DeleteByIndex(int a1)
{
    tSndSoundElem* v1;

    if (SND_gst_Params.allocatedSounds)
    {
        if (a1 >= 0 && a1 < SND_gst_Params.allocatedSoundsMaxNum)
        {
            v1 = &SND_gst_Params.allocatedSounds[a1];
            if ((v1->flags & 0x400) != 0 && v1->usageCnt-- == 1)
            {
                SND_DetachSound(a1);
                if (!SND_b_IsLoadingSound(a1))
                {
                    if (v1->soundModifier)
                    {
                        SND_SModifierDeleteAllLinkedSound(v1->soundModifier);
                        SND_SModifierDeleteAll(&v1->soundModifier);
                    }
                    else if ((v1->flags & 8) == 0)
                    {
                        v1->sndInterface->Stop(v1->sndSndBufElem);
                        v1->sndInterface->Release(v1->sndSndBufElem);
                    }
                    v1->soundKey = -1;
                    v1->flags = 0;
                    v1->usageCnt = 0;
                    v1->sndSndBufElem = 0;
                    v1->freq = 0;
                    v1->numChannels = 0;
                    v1->bitsPerSample = 0;
                    v1->sndInterface = 0;
                    v1->soundModifier = 0;
                    v1->startDataPos = 0;
                    v1->fullsoundLen = 0;
                    v1->byteRate = 0;
                    v1->blockAlign = 0;
                    v1->extraDataSize = 0;
                    v1->format = 0;
                }
            }
        }
    }
}

void snd_metabankuserlistfree_sub_495790(tSndBank* a1)
{
    unsigned int v1;

    if (SND_gul_MetaBankUserNb)
    {
        v1 = 0;
        if (SND_gul_MetaBankUserNb)
        {
            while (SND_gax_MetaBankUserList[v1].bank != a1)
            {
                if (++v1 >= (unsigned int)SND_gul_MetaBankUserNb)
                    return;
            }
            SND_gax_MetaBankUserList[v1].bank = 0;
            SND_gax_MetaBankUserList[v1].offset = 0;
        }
    }
}

void SND_FreeSndBank(struct sSndBank** a1)
{
    tSndBank* v1;
    int v2;
    int v3;
    int* v4;
    int i;
    int* v6;

    if (*a1)
    {
        --(*a1)->usageCount;
        v1 = *a1;
        if (!(*a1)->usageCount)
        {
            if (v1->soundIndicesV2)
            {
                v2 = 0;
                if (v1->elemNum > 0)
                {
                    do
                    {
                        v3 = (*a1)->soundIndices[v2];
                        if (v3 != (*a1)->soundIndicesV2[v2] && v3 != -1)
                            SND_DeleteByIndex((*a1)->soundIndices[v2]);
                        ++v2;
                    } while (v2 < (*a1)->elemNum);
                }
                MEM_Free((*a1)->soundIndices);
                (*a1)->soundIndices = (*a1)->soundIndicesV2;
                (*a1)->soundIndicesV2 = 0;
            }
            v4 = (*a1)->soundIndices;
            if (v4)
            {
                for (i = 0; i < (*a1)->elemNum; ++i)
                {
                    v6 = (*a1)->soundIndices;
                    if (v6[i] != -1)
                        SND_DeleteByIndex(v6[i]);
                }
                MEM_Free((*a1)->soundIndices);
            }
            snd_metabankuserlistfree_sub_495790(*a1);
            if (v4)
                LOA_DeleteAddress((int)*a1);
            MEM_Free(*a1);
            *a1 = 0;
        }
    }
}

void SND_FreeMainStruct(struct sGameObjectMain* a1)
{
    tGameObjectExtended* v1;
    tSndBank* v2;
    tSndBank** v3;

    if ((a1->flags & 0x8000000) != 0)
    {
        v1 = a1->extended;
        if (v1)
        {
            v2 = v1->snd;
            v3 = &v1->snd;
            if (v2)
                SND_FreeSndBank(v3);
        }
    }
}

void SND_Duplicate(struct sGameObjectMain* a1, struct sGameObjectMain* a2)
{
    if (a1 && a1->extended && a2)
    {
        if ((a2->flags & 0x8000000) == 0)
            a2->flags |= 0x8000000u;
        OBJ_GameObject_CreateExtendedIfNot(a2);
        a2->extended->snd = a1->extended->snd;
        ++a1->extended->snd->usageCount;
    }
}

void SND_ResolveRefsForMdF(struct sModifier* a1)
{
    tModifier* i;
    tSoundMdfStruct* v2;
    tSndBank* v3;

    for (i = a1; i; i = i->nextElem)
    {
        if (i->modifierIntf->mdfId == 8)          // GEN_ModifierSound
        {
            v2 = (tSoundMdfStruct*)(i->mdfPrivateDataPtr);
            if ((v2->flags & 0x800) != 0)
            {
                v3 = v2->soundBank;
                if (v3)
                {
                    if (v3->soundIndices)
                        ++v3->usageCount;
                }
            }
        }
    }
}

void SND_ResolveRefs(struct sGameObjectMain* a1)
{
    tGameObjectExtended* v1;
    tSndBank* v2;

    v1 = a1->extended;
    if (v1)
    {
        if ((a1->flags & 0x8000000) != 0)
        {
            v2 = v1->snd;
            if (v2)
            {
                if (v2->soundIndices)
                    ++v2->usageCount;
            }
        }
    }
}

void SND_RestoreBank(struct sGameObjectMain* a1)
{
    tGameObjectExtended* v1;
    tSndBank* v2;
    int i;
    int v4;

    if (a1)
    {
        if ((a1->flags & 0x8000000) != 0)
        {
            v1 = a1->extended;
            if (v1)
            {
                v2 = v1->snd;
                if (v2)
                {
                    if (v2->soundIndices && v2->soundIndicesV2)
                    {
                        for (i = 0; i < v2->elemNum; ++i)
                        {
                            v4 = v2->soundIndices[i];
                            if (v4 != v2->soundIndicesV2[i] && v4 != -1)
                                SND_DeleteByIndex(v2->soundIndices[i]);
                        }
                        MEM_Free(v2->soundIndices);
                        v2->soundIndices = v2->soundIndicesV2;
                        v2->soundIndicesV2 = 0;
                    }
                }
            }
        }
    }
}

void SND_DetachObject(struct sGameObjectMain* a1)
{
    int v1;
    tSoundInstanceElem* v2;
    int v3;
    tGameObjectExtended* v4;
    tGameObjectExtended* v5;
    tSndBank* v6;

    if (SND_gst_Params.soundEnabled && (a1->flags & 0x8000000) != 0)
    {
        v1 = 0;
        if (SND_gst_Params.soundInstanceNum > 0)
        {
            v2 = SND_gst_Params.soundInstances;
            v3 = 0;
            do
            {
                if ((v2[v3].flags & 0x400) != 0 && v2[v3].gao == a1)
                {
                    if ((a1->flags & 0x8000000) == 0)
                        return;
                    v4 = a1->extended;
                    if (!v4 || !v4->snd)
                        return;
                    v2[v3].soundIntf->Stop(v2[v3].soundBufElemFromModel);
                    SND_FreeInstance(v1);
                    v2 = SND_gst_Params.soundInstances;
                }
                ++v1;
                ++v3;
            } while (v1 < SND_gst_Params.soundInstanceNum);
        }
        v5 = a1->extended;
        if (v5)
        {
            v6 = v5->snd;
            if (v6)
            {
                if (v6->soundIndices)
                {
                    if (v6->usageCount == 1)
                        SND_RestoreBank(a1);
                }
            }
        }
    }
}

void SND_Release(int soundInstanceID)
{
    tSoundInstanceElem* v1;

    if (soundInstanceID >= 0 && soundInstanceID < SND_gst_Params.soundInstanceNum)
    {
        v1 = &SND_gst_Params.soundInstances[soundInstanceID];
        if ((v1->flags & 0x400) != 0 && !SND_b_IsLoadingInstance(soundInstanceID))
        {
            v1->soundIntf->Stop(v1->soundBufElemFromModel);
            SND_FreeInstance(soundInstanceID);
        }
    }
}

void SND_DestroyInstanceOfGao(struct sGameObjectMain* a1)
{
    tGameObjectExtended* v1;
    tModifier* i;
    int v3;
    int v4;
    tSoundInstanceElem* v5;
    int v6;
    tGameObjectMain* v7;

    if (a1 && SND_gst_Params.soundEnabled)
    {
        v1 = a1->extended;
        if (v1)
        {
            for (i = v1->mdf; i; i = i->nextElem)
            {
                v3 = i->modifierIntf->mdfId;
                if (v3 == 8 || v3 == 15 || v3 == 19) // 8 = gensound, 15 = gensoundfx, 19 = loadingsound
                    i->modifierIntf->ReInit(i);
            }
        }
        v4 = 0;
        if (SND_gst_Params.soundInstanceNum > 0)
        {
            v5 = SND_gst_Params.soundInstances;
            v6 = 0;
            do
            {
                if ((v5[v6].flags & 0x400) != 0)
                {
                    v7 = v5[v6].gao;
                    if (v7)
                    {
                        if (v7 == a1)
                        {
                            SND_Release(v4);
                            v5 = SND_gst_Params.soundInstances;
                        }
                    }
                }
                ++v4;
                ++v6;
            } while (v4 < SND_gst_Params.soundInstanceNum);
        }
    }

}

float SND_f_MuteComputeFactor(tSoundInstanceElem* a2)
{
    int v2;
    double result;
    tMutesSetupStruct* v4;
    tGameObjectMain* v5;
    tGameObjectExtended* v6;

    v2 = a2->flags;
    if ((v2 & 0x400) == 0)
        return 1.0;
    v4 = a2->unkMuteSetupSeeInitVolTable;
    if (v4->muteEnabled)
        result = v4->muteSetUp2;
    else
        result = v4->muteSetUp1;
    if (SND_Mutes_B30600_fadergroups[10].muteEnabled)
    {
        if ((a2->muteFactorFlags & 0x200) != 0)
            result = result * SND_Mutes_B30600_fadergroups[10].muteSetUp2;
        else
            result = result * SND_Mutes_B30600_fadergroups[10].muteSetUp1;
    }
    if (SND_Mutes_B30600_fadergroups[11].muteEnabled)
    {
        if ((a2->muteFactorFlags & 0x400) != 0)
            result = result * SND_Mutes_B30600_fadergroups[11].muteSetUp2;
        else
            result = result * SND_Mutes_B30600_fadergroups[11].muteSetUp1;
    }
    if (SND_Mutes_B30600_fadergroups[0].muteEnabled)
    {
        v5 = a2->gao;
        if (v5)
        {
            if ((v5->flags & 0x8000000) != 0)
            {
                v6 = v5->extended;
                if (v6)
                {
                    if (v6->snd)
                    {
                        if ((v5->flags2 & 0x4000006) == 2)
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
        if ((a2->muteFactorFlags & 0x40) != 0)
            result = result * SND_Mutes_B30600_fadergroups[1].muteSetUp1;
        else
            result = result * SND_Mutes_B30600_fadergroups[1].muteSetUp2;
    }
    if (SND_Mutes_B30600_fadergroups[4].muteEnabled)
    {
        if ((a2->muteFactorFlags & 0x80) == 0)
            result = result * SND_Mutes_B30600_fadergroups[4].muteSetUp1;
        else
            result = result * SND_Mutes_B30600_fadergroups[4].muteSetUp2;
    }
    if (SND_Mutes_B30600_fadergroups[2].muteEnabled)
    {
        if ((a2->muteFactorFlags & 0x100) != 0)
            result = result * SND_Mutes_B30600_fadergroups[2].muteSetUp2;
        else
            result = result * SND_Mutes_B30600_fadergroups[2].muteSetUp1;
    }
    if (SND_Mutes_B30600_fadergroups[3].muteEnabled)
    {
        if ((a2->muteFactorFlags & 0x1C0) != 0 || (v2 & 0x4000) == 0)
            result = result * SND_Mutes_B30600_fadergroups[3].muteSetUp1;
        else
            result = result * SND_Mutes_B30600_fadergroups[3].muteSetUp2;
    }
    if (SND_Mutes_B30600_fadergroups[6].muteEnabled)
    {
        if ((v2 & 8) != 0)
            result = result * SND_Mutes_B30600_fadergroups[6].muteSetUp2;
        else
            result = result * SND_Mutes_B30600_fadergroups[6].muteSetUp1;
    }
    return (float)result;
}

float SND_f_GetInstVolume(tSoundInstanceElem* a1)
{
    float v3;

    if ((SND_gst_Params.flags & 0x40000) != 0)
        return a1->volume;
    v3 = a1->soundIntf->Compute3DVol(a1);
    return SND_f_MuteComputeFactor(a1) * v3 * SND_gf_AutoVolumeOff;
}

float SND_f_GetVolFromAtt(int a1)
{
    int v1;
    int v2;
    double v4;

    v1 = 0;
    v2 = 255;
    if (a1 >= SND_al_AttTable[0])
        return 1.0;
    if (a1 <= SND_al_AttTable[255])
        return 0.0;
    do
    {
        if (SND_al_AttTable[(v1 + v2) / 2] >= a1)
            v1 = (v1 + v2) / 2;
        else
            v2 = (v1 + v2) / 2;
    } while (v2 - v1 > 1);
    v4 = (double)SND_al_AttTable[v2];
    return (float)(SND_af_VolTable[v2] + (SND_af_VolTable[v2] - SND_af_VolTable[v1]) / (v4 - (double)SND_al_AttTable[v1]) * ((double)a1 - v4));
}

int SND_l_GetAttFromVol(float a1)
{
    int v1;
    int v2;
    double v4;
    double v5;

    v1 = 0;
    v2 = 255;
    if (a1 >= (double)SND_af_VolTable[0])
        return 0;
    if (a1 <= (double)SND_af_VolTable[255])
        return -8000;
    do
    {
        if (SND_af_VolTable[(v1 + v2) / 2] >= (double)a1)
            v1 = (v1 + v2) / 2;
        else
            v2 = (v1 + v2) / 2;
    } while (v2 - v1 > 1);
    v4 = (double)SND_al_AttTable[v2];
    v5 = (v4 - (double)SND_al_AttTable[v1]) / (SND_af_VolTable[v2] - SND_af_VolTable[v1]) * (a1 - SND_af_VolTable[v2]) + v4 + MATH_gd_magicDoubleForRound;
    return *((int32_t *)(&v5));
}

void SND_SetInstVolume(tSoundInstanceElem* a1)
{
    tSndInterfaceStruct* v2;
    int v3;
    float v5;

    v5 = SND_f_GetInstVolume(a1);
    v2 = a1->soundIntf;
    v3 = SND_l_GetAttFromVol(v5);
    v2->Setvolume(a1->soundBufElemFromModel, v3);
}

void SND_RegisterInactiveGAO(struct sGameObjectMain* a1)
{
    int v1;
    int v2;
    tSoundInstanceElem* v3;
    tGameObjectMain* v4;
    tSoundInstanceElem* v5;
    float v6;

    if (a1)
    {
        if (SND_gst_Params.soundEnabled)
        {
            v1 = 0;
            if (SND_gst_Params.soundInstanceNum > 0)
            {
                v2 = 0;
                do
                {
                    v3 = &SND_gst_Params.soundInstances[v2];
                    if ((SND_gst_Params.soundInstances[v2].flags & 0x400) != 0)
                    {
                        v4 = v3->gao;
                        if (v4)
                        {
                            if (v4 == a1)
                            {
                                v5 = &SND_gst_Params.soundInstances[v2];
                                v6 = v3->volume;
                                v3->volume = 0.0;
                                SND_SetInstVolume(v5);
                                v3->volume = v6;
                            }
                        }
                    }
                    ++v1;
                    ++v2;
                } while (v1 < SND_gst_Params.soundInstanceNum);
            }
        }
    }
}

void SND_l_InitVolumeTable()
{
    int v0;
    tMutesSetupStruct* v1;
    int v2;
    double v3;

    v0 = 0;
    v2 = 0;
    do
    {
        v3 = MATH_gd_magicDoubleForRound - (double)v2 * 31.372549;
        SND_al_AttTable[v0] = *((int32_t*)&v3);
        SND_af_VolTable[v0++] = (float) (pow(10.0, (double)(*((int32_t *)&v3)) * 0.0005));
        v2 = v0;
    } while (v0 < 256);
    v1 = SND_Mutes_B30600_fadergroups;
    do
    {
        v1->muteEnabled = 0;
        v1->muteSetUp1 = 1.0;
        v1->muteSetUp2 = 0.5;
        v1->flags = 0;
        v1->elapsedTime = 0.0;
        v1++;
    } while ((int)v1 <= (int)&SND_Mutes_B30600_fadergroups[11]);
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
    int v0;
    tTrackListElem* trackPlayingList;
    int v2;
    int v3;

    if (SND_gst_Params.soundEnabled)
    {
        SND_gpst_Interface.win.ShutdownStreams();
        v0 = 0;
        if (SND_gi_TrackNb > 0)                   // SND_TrackResetAllMute inline
        {
            trackPlayingList = SND_gax_TrackPlayingList;
            v2 = 0;
            do
            {
                trackPlayingList[v2].unk1 = 0;
                trackPlayingList = SND_gax_TrackPlayingList;
                if (SND_gax_TrackPlayingList[v2].flags != 1)
                {
                    v3 = SND_gax_TrackPlayingList[v2].trackID;
                    if (v3 >= 0)
                    {
                        SND_SetInstVolume(&SND_gst_Params.soundInstances[v3]);
                        trackPlayingList = SND_gax_TrackPlayingList;
                    }
                }
                ++v0;
                ++v2;
            } while (v0 < SND_gi_TrackNb);
        }
    }
}

void SND_NotifyNewLoading()
{
    SND_gst_LSGlobal.isNewLoading = 1;
}

void SND_StreamResolveSyncPrefetch()
{
    int v0;
    int v1;
    int v2;
    tSndPrefetchElem* v3;
    tSndSoundElem* v4;
    unsigned int v5;
    __int16 v6;
    int v7;
    int v8;
    tSndPrefetchElem* v9;
    BOOL v10;
    int v11;
    int v12;
    int v13[16];
    int v14[8];

    if (SND_gst_Params.soundEnabled)
    {
        memset(v13, 0, 32u);
        memset(v14, 0, sizeof(v14));
        v0 = 0;
        v1 = 0;
        v11 = 0;
        if (SND_gst_PrefetchManager.num > 0)
        {
            v2 = 0;
            v12 = 0;
            do
            {
                v3 = &(SND_gst_PrefetchManager.ptr[v2]);
                if (v3->type == 1)
                {
                    v4 = &SND_gst_Params.allocatedSounds[v3->sndElemIdx];
                    v5 = v4->flags;
                    if ((v5 & 0x200000) == 0)
                    {
                        if ((v5 & 0x4000) == 0 && (v5 & 0x2000) == 0 && (v5 & 0x80000) == 0
                            || SND_l_AddStreamLoad(v4->soundKey) < 0)
                        {
                            return;
                        }
                        v0 = v11;
                    }
                    --SND_gst_PrefetchManager.sumChannelNums;
                    v6 = v4->numChannels;
                    if (SND_gst_PrefetchManager.sumChannelNums + v6 <= 8)
                    {
                        v13[v1 + 8] = v0;
                        v3->sndIntanceIdx = v4->startDataPos;
                        if (v6 == 1)
                        {
                            v3->sndInstanceIdx2 = 0;
                            v14[v1] = v4->fullsoundLen;
                            v13[v1] = v3->sndIntanceIdx;
                        }
                        else
                        {
                            v3->sndInstanceIdx2 = v4->startDataPos  + (v4->fullsoundLen >> 1) + ((uint16_t)v4->extraDataSize >> 1);
                            v14[v1] = v4->fullsoundLen >> 1;
                            v13[v1++] = v3->sndIntanceIdx;
                            v14[v1] = v4->fullsoundLen >> 1;
                            v13[v1] = v3->sndInstanceIdx2;
                            v13[v1 + 8] = v0;
                        }
                        ++v1;
                        v3->type = 3;
                        SND_gst_PrefetchManager.sumChannelNums += v4->numChannels;
                    }
                    else
                    {
                        v3->type = 0;
                        v3->sndInstanceIdx2 = 0;
                        v3->sndIntanceIdx = 0;
                        v3->sndElemIdx = -1;
                    }
                }
                ++v0;
                ++v2;
                v11 = v0;
                v12 += 16;
            } while (v0 < SND_gst_PrefetchManager.num);
            if (v1)
            {
                SND_gpst_Interface.win.StreamPrefetchArray(v13, v14);
                v7 = 0;
                if (v1 > 0)
                {
                    while (1)
                    {
                        v8 = v13[v7 + 8];
                        v9 = &SND_gst_PrefetchManager.ptr[v8];
                        if (v13[v7])
                            break;
                    LABEL_38:
                        if (++v7 >= v1)
                            return;
                    }
                    --SND_gst_PrefetchManager.sumChannelNums;
                    v10 = v9->sndInstanceIdx2 == 0;
                    v9->type = 0;
                    if (!v10)
                    {
                        --SND_gst_PrefetchManager.sumChannelNums;
                        if (!v7)
                        {
                            if (!v13[1])
                                SND_gpst_Interface.win.StreamFlush(v9->sndInstanceIdx2);
                            v7 = 1;
                            goto LABEL_37;
                        }
                        if (v7 == v1 - 1)
                        {
                            if (!v13[v7 - 1])
                            {
                            LABEL_36:
                                SND_gpst_Interface.win.StreamFlush(v9->sndIntanceIdx);
                                goto LABEL_37;
                            }
                        }
                        else
                        {
                            if (v13[v7 + 9] == v8)
                            {
                                if (!v13[v7 + 1])
                                    SND_gpst_Interface.win.StreamFlush(v9->sndInstanceIdx2);
                                ++v7;
                                goto LABEL_37;
                            }
                            if (v13[v7 + 7] == v8 && !v13[v7 - 1])
                                goto LABEL_36;
                        }
                    }
                LABEL_37:
                    v9->sndInstanceIdx2 = 0;
                    v9->sndIntanceIdx = 0;
                    v9->sndElemIdx = -1;
                    goto LABEL_38;
                }
            }
        }
    }
}

int SND_l_GetSoundIndex(int soundKEy)
{
    tSndSoundElem* v1;
    tSndSoundElem* v2;
    int result;

    v1 = SND_gst_Params.allocatedSounds;
    v2 = &SND_gst_Params.allocatedSounds[SND_gst_Params.allocatedSoundsMaxNum];
    result = 0;
    if (SND_gst_Params.allocatedSounds >= v2)
        return -1;
    while ((v1->flags & 0x400) == 0 || v1->soundKey != soundKEy)
    {
        ++v1;
        ++result;
        if (v1 >= v2)
            return -1;
    }
    return result;
}

int SND_i_AllocSound()
{
    int result;
    unsigned int* v1;
    tSndSoundElem* v2;
    int v3;
    size_t v4;

    result = 0;
    if (SND_gst_Params.allocatedSoundsMaxNum <= 0)
        goto LABEL_7;
    v1 = &SND_gst_Params.allocatedSounds->flags;
    while ((*v1 & 0x400) != 0)
    {
        ++result;
        v1 += 14;
        if (result >= SND_gst_Params.allocatedSoundsMaxNum)
            goto LABEL_7;
    }
    if (result >= SND_gst_Params.allocatedSoundsMaxNum)
    {
    LABEL_7:
        v4 = 56 * (SND_gst_Params.allocatedSoundsMaxNum + 1000);
        if (SND_gst_Params.allocatedSoundsMaxNum)
            v2 = (tSndSoundElem*)MEM_p_Realloc(SND_gst_Params.allocatedSounds, v4);
        else
            v2 = (tSndSoundElem*)MEM_p_Alloc(v4);
        v3 = SND_gst_Params.allocatedSoundsMaxNum;
        SND_gst_Params.allocatedSounds = v2;
        memset(&v2[SND_gst_Params.allocatedSoundsMaxNum], 0, sizeof(tSndSoundElem) * 1000);
        SND_gst_Params.allocatedSoundsMaxNum += 1000;
        result = v3;
    }
    return result;
}

int SND_l_AddSound(int soundKey)
{
    int result;
    tSndSoundElem* v2;

    if (!SND_gst_Params.soundEnabled)
        return -1;
    if (!soundKey || soundKey == -1)
        return -1;
    result = SND_l_GetSoundIndex(soundKey);
    if (result == -1)
    {
        result = SND_i_AllocSound();
        v2 = &SND_gst_Params.allocatedSounds[result];
        v2->soundKey = soundKey;
        v2->startDataPos = 0;
        v2->fullsoundLen = 0;
        v2->freq = 0;
        v2->numChannels = 0;
        v2->bitsPerSample = 0;
        v2->format = 0;
        v2->byteRate = 0;
        v2->blockAlign = 0;
        v2->extraDataSize = 0;
        v2->sndSndBufElem = 0;
        v2->soundModifier = 0;
        v2->flags = 0x400;
        v2->muteFacorFlags = 64;
        v2->usageCnt = 1;
        v2->sndInterface = (tSndInterfaceStruct*)&SND_gpst_Interface; // TODO FIXME ez jó itt?
        SND_gb_UnloadedFile = 1;
    }
    else
    {
        ++SND_gst_Params.allocatedSounds[result].usageCnt;
    }
    return result;
}

int SND_l_AddLoadingSound(int a1)
{
    int result;

    result = SND_l_AddSound(a1);
    if (result != -1)
        SND_gst_Params.allocatedSounds[result].flags |= 8u;
    return result;
}

void SND_StreamPrefetchOneSound(int a1, int a2)
{
    unsigned int v2;
    tSndSoundElem* v3; 
    tSndPrefetchElem* v4; 
    int v5;
    tSndPrefetchElem* v6;
    int v7;
    int v8;

    if (SND_gst_Params.soundEnabled)
    {
        if (a1 >= 0 && a1 < SND_gst_Params.allocatedSoundsMaxNum)
        {
            v2 = SND_gst_Params.allocatedSounds[a1].flags;
            v3 = &SND_gst_Params.allocatedSounds[a1];
            if ((v2 & 0x400) != 0 && SND_gst_PrefetchManager.sumChannelNums < 8)
            {
                v4 = SND_gst_PrefetchManager.ptr;
                v5 = 0;
                if (SND_gst_PrefetchManager.num > 0)
                {
                    v6 = SND_gst_PrefetchManager.ptr;
                    do
                    {
                        if (v6->type)
                        {
                            if (v6->sndElemIdx == a1)
                                return;
                            v2 = SND_gst_Params.allocatedSounds[a1].flags;
                        }
                        ++v5;
                        ++v6;
                    } while (v5 < SND_gst_PrefetchManager.num);
                }
                v7 = 0;
                if (SND_gst_PrefetchManager.num > 0)
                {
                    while (v4->type)
                    {
                        ++v7;
                        ++v4;
                        if (v7 >= SND_gst_PrefetchManager.num)
                            return;
                    }
                    if (v7 < SND_gst_PrefetchManager.num)
                    {
                        if (a2)
                        {
                            if (a2 == 1
                                && (v3->startDataPos != -1
                                    || ((v2 & 0x2000) != 0 || (v2 & 0x4000) != 0 || (v2 & 0x80000) != 0)
                                    && SND_l_AddStreamLoad(v3->soundKey) >= 0))
                            {
                                v8 = v3->startDataPos;
                                if (v3->numChannels == 1)
                                {
                                    SND_gpst_Interface.win.StreamPrefetch(v8, v3->fullsoundLen);
                                    v4->sndIntanceIdx = v3->startDataPos;
                                    v4->sndInstanceIdx2 = 0;
                                    ++SND_gst_PrefetchManager.sumChannelNums;
                                }
                                else
                                {
                                    SND_gpst_Interface.win.StreamPrefetch(v8, v3->fullsoundLen >> 1);
                                    SND_gpst_Interface.win.StreamPrefetch((v3->fullsoundLen >> 1) + v3->startDataPos, v3->fullsoundLen >> 1);
                                    v4->sndIntanceIdx = v3->startDataPos;
                                    v4->sndInstanceIdx2 = v3->startDataPos + (v3->fullsoundLen >> 1) + ((uint16_t)v3->extraDataSize >> 1);
                                    SND_gst_PrefetchManager.sumChannelNums += 2;
                                }
                                v4->type = 2;
                                v4->sndElemIdx = a1;
                            }
                        }
                        else
                        {
                            ++SND_gst_PrefetchManager.sumChannelNums;
                            v4->type = 1;
                            v4->sndElemIdx = a1;
                        }
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
    tSoundMdfStruct* v2;
    int v3;

    result = SND_gst_Params.soundEnabled;
    if (SND_gst_Params.soundEnabled)
    {
        result = SND_gst_BankMdFUserList;
        for (i = 0; i < SND_gst_BankMdFUserList; ++i)
        {
            v2 = sndMdfBankUser[i];
            if (v2 && (v2->flags & 0x1800) == 0x1800)
            {
                MDF_SndGetSoundIndexFromBank(sndMdfBankUser[i]);
                v3 = v2->idx;
                if (v3 >= 0)
                    SND_StreamPrefetchOneSound(v3, 0);
            }
            result = SND_gst_BankMdFUserList;
        }
    }
    return result;
}

int SND_SoundListMake(tSndList** a1, int* a2)
{
    int result;
    int v3;
    tSndSoundElem* v4;
    unsigned int v5;
    tSndList* v6;
    BOOL v7;
    int v8;
    int v9;

    result = SND_gst_Params.allocatedSoundsMaxNum;
    v3 = 0;
    v9 = 0;
    if (SND_gst_Params.allocatedSoundsMaxNum > 0)
    {
        v8 = 0;
        do
        {
            v4 = &(SND_gst_Params.allocatedSounds[v3]);
            v5 = v4->flags;
            if ((v5 & 0x400) != 0 && (v5 & 1) == 0 && (v5 & 0x96000) == 0 && v4->soundKey && v4->soundKey != -1)
            {
                if (*a1)
                    v6 = (tSndList*)MEM_p_Realloc(*a1, 8 * *a2 + 8);
                else
                    v6 = (tSndList*)MEM_p_Alloc(8u);
                *a1 = v6;
                (*a1)[*a2].sndkey = v4->soundKey;
                (*a1)[(*a2)++].flags = v4->flags & 0x96008;
            }
            result = v9 + 1;
            v3 = v8 + 1;
            v7 = ++v9 < SND_gst_Params.allocatedSoundsMaxNum;
            v8++;
        } while (v7);
    }
    return result;
}

int SND_l_LoadData(int soundKEy)
{
    int v2;
    int v3;
    tSndSoundElem* soundElem;
    tSndSoundBufferElem* v5; 
    tSndWavHdrStruct wavHdr;

    if (!SND_gst_Params.soundEnabled)
        return -1;
    v2 = SND_l_GetSoundIndex(soundKEy);
    v3 = v2;
    if (v2 == -1)
        return -1;
    soundElem = &SND_gst_Params.allocatedSounds[v2];
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
        v5 = SND_gpst_Interface.win.Create(SND_gst_Params.handles, &wavHdr, soundElem->startDataPos);
        if (!v5)
            return -1;
        soundElem->flags = 0x200401;
        soundElem->sndSndBufElem = v5;
    }
    return v3;
}

int SND_l_LoadHeader(int a1)
{
    int v2;
    int v3;
    tSndSoundElem* sndElem;
    unsigned int v5;
    tSndWavHdrStruct wavHdr;

    memset(&wavHdr, 0, sizeof(tSndWavHdrStruct)); // Ez nincs a játékban

    if (!SND_gst_Params.soundEnabled)
        return -1;
    v2 = SND_l_GetSoundIndex(a1);
    v3 = v2;
    if (v2 != -1)
    {
        sndElem = &SND_gst_Params.allocatedSounds[v2];
        if ((sndElem->flags & 0x200000) != 0)
            return v3;
        if (a1)
        {
            if (a1 != -1)
            {
                v5 = BIG_ul_SearchKeyToPos(a1);
                if (SND_i_GetWaveInfo(v5 + 4, &wavHdr, 5) != -1)
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
                    return v3;
                }
            }
        }
    }
    return -1;
}

int SND_l_LoadLoadingSoundHeader(int a1)
{
    int result;

    result = SND_l_LoadHeader(a1);
    if (result != -1)
        SND_gst_Params.allocatedSounds[result].flags |= 8u;
    return result;
}

void SND_LoadAllSounds()
{
    tSndList* v0;
    unsigned int v1;
    int v2;
    int v3;
    char* v4;
    int v5;
    int v6;
    int v7;
    int v8;
    unsigned int* v9;
    int v10;
    int v11;
    int v12;
    int v13;
    int v14;
    int v15;
    int j;
    int a2;
    tSndList* a1;
    char* v19;
    char* i;

    if (!SND_gb_AllSoundsLoaded)
    {
        v0 = 0;
        a1 = 0;
        a2 = 0;
        if (!LOA_gb_SpeedMode
            || (v1 = BIG_ul_SearchKeyToPos(LOA_ul_BinKey | 0xFF400000), v1 != (LOA_ul_BinKey | 0xFF400000)) && v1 != -1)
        {
            LOA_BeginSpeedMode(0xFF400000);
            if (!LOA_gb_SpeedMode || BIG_gi_ReadMode != 2)
            {
                SND_SoundListMake(&a1, &a2);
                v0 = a1;
            }
            if (LOA_gb_SpeedMode)
            {
                if (BIG_gi_ReadMode == 2)
                {
                    v19 = LOA_FetchBuffer(4);
                    v2 = LOA_ReadLong(&v19, 0, 1);
                    v3 = 8 * v2;
                    a2 = v2;
                    v0 = (tSndList*)MEM_p_Alloc(8 * v2);
                    memset(v0, 0xFFu, v3);
                    v4 = LOA_FetchBuffer(v3);
                    v5 = a2;
                    v6 = 0;
                    for (i = v4; v6 < v5; ++v6)
                    {
                        v0[v6].sndkey = LOA_ReadLong(&i, 0, 1);
                        v0[v6].flags = LOA_ReadLong(&i, 0, 1);
                    }
                }
            }
            v7 = 0;
            if (SND_gst_Params.allocatedSoundsMaxNum > 0)
            {
                v8 = 0;
                do
                {
                    v9 = &SND_gst_Params.allocatedSounds[v8].flags;
                    if ((*v9 & 0x400) != 0)
                        *v9 &= ~0x200000u;
                    ++v7;
                    ++v8;
                } while (v7 < SND_gst_Params.allocatedSoundsMaxNum);
            }
            v10 = a2;
            v11 = 0;
            if (a2 > 0)
            {
                while (1)
                {
                    v12 = v0[v11].flags;
                    if (!v12)
                        break;
                    if (v12 == 8)
                    {
                        v13 = SND_l_AddLoadingSound(v0[v11].sndkey);
                        goto LABEL_22;
                    }
                LABEL_28:
                    v10 = a2;
                    if (++v11 >= a2)
                        goto LABEL_29;
                }
                v13 = SND_l_AddSound(v0[v11].sndkey);
            LABEL_22:
                v14 = v13;
                if (v13 != -1)
                {
                    v15 = v0[v11].flags;
                    if (v15)
                    {
                        if (v15 == 8)
                            SND_l_LoadLoadingSoundHeader(v0[v11].sndkey);
                    }
                    else
                    {
                        SND_l_LoadHeader(v0[v11].sndkey);
                    }
                    --SND_gst_Params.allocatedSounds[v14].usageCnt;
                }
                goto LABEL_28;
            }
        LABEL_29:
            if (LOA_gb_SpeedMode)
                SND_gul_WavBinSeek = BIG_gul_ReadPos + BIG_gi_ReadSeek + 4;
            for (j = 0; j < v10; ++j)
            {
                if (!v0[j].flags)
                    SND_l_LoadData(v0[j].sndkey);
            }
            if (v0)
                MEM_Free(v0);
            if (LOA_gb_SpeedMode)
                SND_gul_WavBinSeek = 0;
            LOA_EndSpeedMode();
            SND_gb_AllSoundsLoaded = 1;
        }
    }
}

void SND_FreeHeaderArray(BASStruct* a1)
{
    int i;

    for (i = 0; i < a1->actElemNum; ++i)
    {
        if (a1->keyValPtr[i].val != -1)
            MEM_Free((void*)a1->keyValPtr[i].val);
    }
    BAS_bfree(a1);
    a1->keyValPtr = 0;
    a1->actElemNum = 0;
    a1->maxElemNum = 0;
    a1->elemNumToAddWhenFull = 0;
}

void SND_ReadRIFFHeader(char** a1, void* hdrDest)
{
    int v2;
    int v3;
    int v4;

    tSoundHeaderDataCase0* thdrDest = (tSoundHeaderDataCase0*)hdrDest;
    tSoundHeaderDataCase1* thdrDest1 = (tSoundHeaderDataCase1*)hdrDest;
    tSoundHeaderDataCase2* thdrDest2 = (tSoundHeaderDataCase2*)hdrDest;

    // itt midnegy mire castolom, mert az összes eleje ugyanaz
    thdrDest->i1 = LOA_ReadLong(a1, 0, 1);
    thdrDest->i2 = LOA_ReadLong(a1, 0, 1);
    thdrDest->i3 = LOA_ReadLong(a1, 0, 1);
    thdrDest->i4 = LOA_ReadLong(a1, 0, 1);
    v2 = LOA_ReadLong(a1, 0, 1);
    thdrDest->i5 = v2;
    v3 = v2 - 14;
    if (v3)
    {
        v4 = v3 - 2;
        if (v4)
        {
            if (v4 == 2)
            {
                thdrDest2->is1 = LOA_ReadShort(a1, 0, 1);
                thdrDest2->is2 = LOA_ReadShort(a1, 0, 1);
                thdrDest2->ii1 = LOA_ReadLong(a1, 0, 1);
                thdrDest2->ii2 = LOA_ReadLong(a1, 0, 1);
                thdrDest2->is3 = LOA_ReadShort(a1, 0, 1);
                thdrDest2->is4 = LOA_ReadShort(a1, 0, 1);
                thdrDest2->is5 = LOA_ReadShort(a1, 0, 1);
                thdrDest2->ii3 = LOA_ReadLong(a1, 0, 1);
                thdrDest2->ii4 = LOA_ReadLong(a1, 0, 1);
            }
        }
        else
        {
            thdrDest1->is1 = LOA_ReadShort(a1, 0, 1);
            thdrDest1->is2 = LOA_ReadShort(a1, 0, 1);
            thdrDest1->ii1 = LOA_ReadLong(a1, 0, 1);
            thdrDest1->ii2 = LOA_ReadLong(a1, 0, 1);
            thdrDest1->is3 = LOA_ReadShort(a1, 0, 1);
            thdrDest1->is4 = LOA_ReadShort(a1, 0, 1);
            thdrDest1->ii3 = LOA_ReadLong(a1, 0, 1);
            thdrDest1->ii4 = LOA_ReadLong(a1, 0, 1);
        }
    }
    else
    {
        thdrDest->is1 = LOA_ReadShort(a1, 0, 1);
        thdrDest->is2 = LOA_ReadShort(a1, 0, 1);
        thdrDest->ii1 = LOA_ReadLong(a1, 0, 1);
        thdrDest->ii2 = LOA_ReadLong(a1, 0, 1);
        thdrDest->is3 = LOA_ReadShort(a1, 0, 1);
        thdrDest->ii3 = LOA_ReadLong(a1, 0, 1);
        thdrDest->ii4 = LOA_ReadLong(a1, 0, 1);
    }
}

int SND_ReadSNDHeaderElem(char** a1, tSoundHeaderGeneric* a2)
{
    int result;
    a2->soundKey = LOA_ReadLong(a1, 0, 1);
    SND_ReadRIFFHeader(a1, a2->data);
    result = LOA_ReadLong(a1, 0, 1);
    a2->soundKey1 = result;
    return result;
}

int SND_i_GetWaveInfo(int Offset, tSndWavHdrStruct* wavHdrStructDest, int readMode)
{
    tSndWavHdrStruct* v3;
    int fileStartOffset; 
    int wawHdrRaw;
    int result;
    int headerLen;
    char DstBuf[46];

    v3 = wavHdrStructDest;
    
    memset(v3, 0, sizeof(tSndWavHdrStruct));

    switch (readMode)
    {
    case 1:
        fileStartOffset = Offset;
        wawHdrRaw = (int)DstBuf;
        if (!LOA_gb_SpeedMode || BIG_gi_ReadMode != 2)
            BIG_ReadNoSeek(Offset, DstBuf, 46u);
        if (LOA_gb_SpeedMode && BIG_gi_ReadMode == 2)
        {
        LABEL_16:
            wavHdrStructDest = (tSndWavHdrStruct*)LOA_FetchBuffer(46);
            SND_ReadRIFFHeader((char**)&wavHdrStructDest, DstBuf);
            break;
        }
        break;
    case 4:
        wawHdrRaw = Offset + 4;
        fileStartOffset = BIG_ul_SearchKeyToPos(*(int32_t*)(Offset + 50)) + 4;
        break;
    case 5:
        fileStartOffset = Offset;
        wawHdrRaw = (int)DstBuf;
        if (!LOA_gb_SpeedMode || BIG_gi_ReadMode != 2)
            BIG_ReadNoSeek(Offset, DstBuf, 46u);
        if (LOA_gb_SpeedMode && BIG_gi_ReadMode == 2)
            goto LABEL_16;
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
    v3->wfex.nAvgBytesPerSec = thdrDest->ii2;
    v3->wfex.nBlockAlign = thdrDest->is3;
    v3->wfex.nChannels = thdrDest->is2;
    v3->wfex.nSamplesPerSec = thdrDest->ii1;
    v3->wfex.wFormatTag = thdrDest->is1;
    if (thdrDest->i5 == 14)
    {
        v3->wfex.cbSize = 0;
        v3->wfex.wBitsPerSample = 8 * (thdrDest->ii2 / (unsigned int)thdrDest->is3 / thdrDest->is2);
        if (!memcmp(&(thdrDest->ii3), "data", 4))
        {
            v3->fullSoundSize = thdrDest->ii4;
            headerLen = 42;
            goto LABEL_33;
        }
        return -1;
    }
    if (thdrDest1->i5 == 16)
    {
        v3->wfex.cbSize = 0;
        v3->wfex.wBitsPerSample = thdrDest1->is4;
        if (!memcmp(&(thdrDest->ii3), "data", 4))
        {
            v3->fullSoundSize = thdrDest1->ii4;
            headerLen = 44;
            goto LABEL_33;
        }
        return -1;
    }
    if (thdrDest2->i5 != 18)
        DBG_assert(0, __FILE__, __LINE__, "0", "Wave Header format unknown", 0, 1);
    v3->wfex.cbSize = thdrDest2->is5;
    v3->wfex.wBitsPerSample = thdrDest2->is4;
    if (memcmp(&(thdrDest2->ii3), "data", 4)) // if (memcmp(&(thdrDest->ii3), "data", 4))
        return -1;
    v3->fullSoundSize = thdrDest2->ii4;
    headerLen = 46;
LABEL_33:
    result = fileStartOffset + headerLen;
    if (readMode == 1 || readMode == 4 || readMode == 5)
        v3->dataStartPos = result;
    return result;
}

int SND_i_LoadStreamedFile(unsigned int a1, tSndWavHdrStruct* wavHdrStructDest)
{
    int v2;
    int result = 0;
    unsigned int v4;

    if (!a1 || a1 == -1)
        goto LABEL_8;
    v2 = BAS_bsearch(a1, &SND_gst_HeaderArray);
    if (v2 != -1)
        return SND_i_GetWaveInfo(v2, wavHdrStructDest, 4);
    v4 = BIG_ul_SearchKeyToPos(a1);
    if (v4 == -1 || v4 == a1)
        LABEL_8:
    result = -1;
    else
        result = SND_i_GetWaveInfo(v4 + 4, wavHdrStructDest, 1);
    return result;
}

int SND_i_StreamAlloc()
{
    tSndStreamList* v0;
    int v1;
    tSndStreamList* v2;
    tSndStreamList* v3;

    v0 = SND_gast_StreamList;
    v1 = 0;
    if (SND_gi_StreamListSize > 0)
    {
        v2 = SND_gast_StreamList;
        do
        {
            if (!v2->num)
                break;
            ++v1;
            ++v2;
        } while (v1 < SND_gi_StreamListSize);
    }
    if (v1 == SND_gi_StreamListSize)
    {
        if (SND_gi_StreamListSize)
        {
            SND_gast_StreamList = (tSndStreamList*)MEM_p_Realloc(SND_gast_StreamList, sizeof(tSndStreamList) * (SND_gi_StreamListSize + 200));
            v3 = &SND_gast_StreamList[SND_gi_StreamListSize];
        }
        else
        {
            v3 = (tSndStreamList*)MEM_p_Alloc(200 * sizeof(tSndStreamList));
            SND_gast_StreamList = v3;
        }
        memset(v3, 0, (200 * sizeof(tSndStreamList)));
        v0 = SND_gast_StreamList;
        v1 = SND_gi_StreamListSize;
        SND_gi_StreamListSize += 200;
    }
    v0[v1].num = 1024;
    return v1 + 1;
}

int SND_l_AddStreamLoad_inner_sub_496680(int a1)
{
    int result = 0; // ez sincs egyenlõvé téve 0-val
    int v2;
    int v3;
    tSndSoundElem* v4;
    int v5;
    int v6;
    tSndWavHdrStruct v7;

    if (!SND_gst_Params.soundEnabled)
        return -1;
    v2 = SND_l_GetSoundIndex(a1);
    v3 = v2;
    if (v2 == -1)
        goto LABEL_4;
    v4 = &SND_gst_Params.allocatedSounds[v2];
    if ((v4->flags & 1) != 0)
        goto LABEL_9;
    v5 = SND_i_LoadStreamedFile(v4->soundKey, &v7);
    v4->startDataPos = v5;
    if (v7.wfex.wFormatTag != 2)
        goto LABEL_4;
    if (v5 == -1)
        goto LABEL_4;
    v4->flags |= 1u;
    v4->freq = v7.wfex.nSamplesPerSec;
    v4->numChannels = v7.wfex.nChannels;
    v4->bitsPerSample = v7.wfex.wBitsPerSample;
    v4->format = v7.wfex.wFormatTag;
    v4->byteRate = v7.wfex.nAvgBytesPerSec;
    v4->blockAlign = v7.wfex.nBlockAlign;
    v4->extraDataSize = v7.wfex.cbSize;
    v6 = SND_i_StreamAlloc();
    v4->sndSndBufElem = (tSndSoundBufferElem*)v6; // TODO FIXME ez jó típus itt??, sztem jó mert az összes stream snd fv castolt intekkel operál
    v4->fullsoundLen = v7.fullSoundSize;
    if (v6 <= 0)
        LABEL_4:
    result = -1;
    else
        LABEL_9:
    result = v3;
    return result;
}

int SND_l_AddStreamLoad(int a1)
{
    return SND_l_AddStreamLoad_inner_sub_496680(a1);
}



void SND_HeaderMakeArray(BASStruct* a1)
{
    BASStruct* v1;
    tSndSoundElem* v2;
    tSndSoundElem* v3;
    unsigned int v4;
    tSoundHeaderGeneric* v5;
    unsigned int v6;

    v1 = a1;
    SND_FreeHeaderArray(a1);
    BAS_binit(a1, 200);
    v2 = SND_gst_Params.allocatedSounds;
    v3 = SND_gst_Params.allocatedSounds;
    BAS_bsortmode = 0;
    if (SND_gst_Params.allocatedSounds < &SND_gst_Params.allocatedSounds[SND_gst_Params.allocatedSoundsMaxNum])
    {
        do
        {
            v4 = v3->flags;
            if ((v4 & 0x400) != 0 && v3->soundKey && v3->soundKey != -1 && (v4 & 0x86000) != 0)
            {
                v5 = (tSoundHeaderGeneric*)MEM_p_Alloc(sizeof(tSoundHeaderGeneric));
                memset(v5, 0, sizeof(tSoundHeaderGeneric));
                v6 = BIG_ul_SearchKeyToPos(v3->soundKey);
                if (v6 != -1 && v6 != v3->soundKey)
                {
                    BIG_ReadNoSeek(v6 + 4, v5->data, 46u);
                    v5->soundKey = v3->soundKey;
                    v5->soundKey1 = v3->soundKey;
                }
                BAS_binsert(v3->soundKey, (int)v5, a1);
                v2 = SND_gst_Params.allocatedSounds;
            }
            ++v3;
        } while (v3 < &v2[SND_gst_Params.allocatedSoundsMaxNum]);
        v1 = a1;
    }
    BAS_bsortmode = 1;
    BAS_bsort(v1);
}

void SND_HeaderLoadArray(BASStruct* a1, int a2)
{
    int v2;
    char* v3;
    unsigned int v4;
    tSoundHeaderGeneric* v5;
    unsigned int a2a;
    char* v8;

    v2 = BIG_ul_SearchKeyToPos(a2);
    if (v2 == -1 || v2 == a2) {
        SND_HeaderMakeArray(a1);
        return;
    }
    v3 = BIG_pc_ReadFileTmp(v2, &a2a);
    a2a /= sizeof(tSoundHeaderGeneric);
    SND_FreeHeaderArray(a1);
    v4 = 0;
    BAS_bsortmode = 0;
    BAS_binit(a1, 200);
    if (a2a)
    {
        do
        {
            v5 = (tSoundHeaderGeneric*)MEM_p_Alloc(54u);
            v8 = v3;
            SND_ReadSNDHeaderElem(&v8, v5);
            BAS_binsert(v5->soundKey, (int)v5, a1);
            ++v4;
            v3 += sizeof(tSoundHeaderGeneric);
        } while (v4 < a2a);
    }
    BAS_bsortmode = 1;
    BAS_bsort(a1);
}

void SND_StreamFlushOneSound(tSndPrefetchElem* a1)
{
    int v1;
    int v2;

    if (SND_gst_Params.soundEnabled && SND_gst_PrefetchManager.sumChannelNums && a1->type)
    {
        v1 = --SND_gst_PrefetchManager.sumChannelNums;
        if (a1->sndInstanceIdx2)
            SND_gst_PrefetchManager.sumChannelNums = v1 - 1;
        v2 = a1->type;
        if (v2 >= 2 && v2 <= 3)
        {
            SND_gpst_Interface.win.StreamFlush(a1->sndIntanceIdx);
            if (a1->sndInstanceIdx2)
                SND_gpst_Interface.win.StreamFlush(a1->sndInstanceIdx2);
        }
        a1->type = 0;
        a1->sndInstanceIdx2 = 0;
        a1->sndIntanceIdx = 0;
        a1->sndElemIdx = -1;
    }
}

void SND_StreamPrefetchFlushAll()
{
    int v0;
    int v1;

    if (SND_gst_Params.soundEnabled)
    {
        v0 = 0;
        if (SND_gst_PrefetchManager.num > 0)
        {
            v1 = 0;
            do
            {
                if (SND_gst_PrefetchManager.ptr[v1].type)
                    SND_StreamFlushOneSound(&SND_gst_PrefetchManager.ptr[v1]);
                ++v0;
                ++v1;
            } while (v0 < SND_gst_PrefetchManager.num);
        }
    }
}

void SND_StreamFlushOneSound_0(int a1, int a2)
{
    int v2;
    tSndPrefetchElem* i;
    int v4;
    int v5;

    if (SND_gst_Params.soundEnabled)
    {
        if (SND_gst_PrefetchManager.sumChannelNums)
        {
            if (a1 >= 0
                && a1 < SND_gst_Params.allocatedSoundsMaxNum
                && (SND_gst_Params.allocatedSounds[a1].flags & 0x400) != 0)
            {
                v2 = 0;
                if (SND_gst_PrefetchManager.num > 0)
                {
                    for (i = SND_gst_PrefetchManager.ptr; !i->type || i->sndElemIdx != a1; ++i)
                    {
                        if (++v2 >= SND_gst_PrefetchManager.num)
                            return;
                    }
                    v4 = --SND_gst_PrefetchManager.sumChannelNums;
                    if (i->sndInstanceIdx2)
                        SND_gst_PrefetchManager.sumChannelNums = v4 - 1;
                    if (a2)
                    {
                        v5 = i->type;
                        if (v5 >= 2 && v5 <= 3)
                        {
                            SND_gpst_Interface.win.StreamFlush(i->sndIntanceIdx);
                            if (i->sndInstanceIdx2)
                                SND_gpst_Interface.win.StreamFlush(i->sndInstanceIdx2);
                        }
                    }
                    i->type = 0;
                    i->sndInstanceIdx2 = 0;
                    i->sndIntanceIdx = 0;
                    i->sndElemIdx = -1;
                }
            }
        }
    }
}

int SND_SignalChangeLanguageToMdF()
{
    int result;
    unsigned int i;
    tSoundMdfStruct* v2;

    result = SND_gst_Params.soundEnabled;
    if (SND_gst_Params.soundEnabled)
    {
        result = SND_gst_BankMdFUserList;
        for (i = 0; i < SND_gst_BankMdFUserList; ++i)
        {
            v2 = sndMdfBankUser[i];
            if (v2)
            {
                SND_StreamFlushOneSound_0(v2->idx, 1);
                if ((v2->flags & 0x800) != 0)
                {
                    v2->sndInstanceID = -1;
                    v2->idx = -1;
                }
            }
            result = SND_gst_BankMdFUserList;
        }
    }
    return result;
}

tSndBank* SND_ul_CallbackLoadMetaBank(int Offset)
{
    char* v1;
    char* v2;
    int bankKey;
    unsigned int bankLangSpecificAlternativeKey;
    int v5;
    int v6;
    tSndBank* v7;
    unsigned int v8;
    unsigned int v9;
    BOOL v10;
    tSndMetaBank* v11;
    int v13;
    char* a1;
    int size;
    int lang;

    lang = TEXT_i_GetLang();
    v1 = BIG_pc_ReadFileTmp(Offset, &size);
    v2 = &v1[size];
    bankKey = 0;
    bankLangSpecificAlternativeKey = 0;
    a1 = v1;
    v13 = 0;
    if (v1 >= &v1[size])
        goto LABEL_11;
    do
    {
        v5 = LOA_ReadLong(&a1, 0, 1);
        LOA_ReadLong(&a1, 0, 2);
        if (v5 && v5 != -1)
        {
            if (!bankKey)
                bankKey = v5;
            if (v13 == lang)
                bankLangSpecificAlternativeKey = v5;
            ++v13;
        }
    } while (a1 < v2);
    if (!bankLangSpecificAlternativeKey)
        LABEL_11:
    bankLangSpecificAlternativeKey = bankKey;
    v6 = BIG_ul_SearchKeyToPos(bankLangSpecificAlternativeKey);
    v7 = SND_ul_CallbackLoadBank(v6);
    if (!SND_gst_Params.soundEnabled)
        return v7;
    v8 = 0;
    if (SND_gul_MetaBankUserNb)
    {
        while (SND_gax_MetaBankUserList[v8].bank != v7)
        {
            if (++v8 >= SND_gul_MetaBankUserNb)
                goto LABEL_16;
        }
        return v7;
    }
LABEL_16:
    v9 = 0;
    v10 = SND_gul_MetaBankUserNb == 0;
    if (!SND_gul_MetaBankUserNb)
    {
    LABEL_20:
        if (v10)
        {
            v11 = (tSndMetaBank*)MEM_p_Alloc(128u);
            SND_gax_MetaBankUserList = v11;
        }
        else
        {
            SND_gax_MetaBankUserList = (tSndMetaBank*)MEM_p_Realloc(
                SND_gax_MetaBankUserList,
                sizeof(tSndMetaBank) * SND_gul_MetaBankUserNb + (16 * sizeof(tSndMetaBank)));
            v11 = &SND_gax_MetaBankUserList[SND_gul_MetaBankUserNb];
        }
        memset(v11, 0, (16 * sizeof(tSndMetaBank)));
        SND_gax_MetaBankUserList[SND_gul_MetaBankUserNb].bank = v7;
        SND_gax_MetaBankUserList[SND_gul_MetaBankUserNb].offset = Offset;
        SND_gul_MetaBankUserNb += 16;
        return v7;
    }
    while (SND_gax_MetaBankUserList[v9].bank)
    {
        if (++v9 >= SND_gul_MetaBankUserNb)
        {
            v10 = SND_gul_MetaBankUserNb == 0;
            goto LABEL_20;
        }
    }
    SND_gax_MetaBankUserList[v9].bank = v7;
    SND_gax_MetaBankUserList[v9].offset = Offset;
    return v7;
}

unsigned int SND_UpdateLang()
{
    unsigned int result;
    int v1;
    int v2;
    tSndSoundElem* v3;
    unsigned int v4;
    unsigned int v5;
    unsigned int v6;
    tSndBank* v7;
    int i;
    tSndBank* v9;
    int v10;

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
                v1 = 0;
                if (SND_gst_Params.allocatedSoundsMaxNum > 0)
                {
                    v2 = 0;
                    v10 = 0;
                    do
                    {
                        v3 = &(SND_gst_Params.allocatedSounds[v2]);
                        v4 = v3->flags;
                        if ((v4 & 0x400) != 0 && (v4 & 0x4000) != 0 && (v4 & 1) != 0)
                        {
                            SND_DetachSound(v1);
                            if (v3->soundModifier)
                            {
                                SND_SModifierDeleteAllLinkedSound(v3->soundModifier);
                                SND_SModifierDeleteAll(&v3->soundModifier);
                            }
                            else
                            {
                                v3->sndInterface->Stop(v3->sndSndBufElem);
                                v3->sndInterface->Release(v3->sndSndBufElem);
                            }
                            v5 = v3->flags & ~1u;
                            v3->sndInterface = &SND_gpst_Interface.dialog;
                            v3->sndSndBufElem = 0;
                            v3->flags = v5;
                        }
                        result = SND_gst_Params.allocatedSoundsMaxNum;
                        ++v1;
                        v2 = v10 + 1;
                        ++v10;
                    } while (v1 < SND_gst_Params.allocatedSoundsMaxNum);
                }
            }
            else
            {
                v6 = 0;
                if (SND_gul_MetaBankUserNb)
                {
                    do
                    {
                        v7 = SND_gax_MetaBankUserList[v6].bank;
                        if (v7)
                        {
                            if (v7->soundIndices)
                            {
                                for (i = 0; i < v7->elemNum; ++i)
                                {
                                    if (v7->soundIndices[i] != -1)
                                        SND_DeleteByIndex(v7->soundIndices[i]);
                                }
                                MEM_Free(v7->soundIndices);
                            }
                            v9 = SND_ul_CallbackLoadMetaBank(SND_gax_MetaBankUserList[v6].offset);
                            v7->elemNum = v9->elemNum;
                            v7->soundIndices = v9->soundIndices;
                            snd_metabankuserlistfree_sub_495790(v9);
                            MEM_Free(v9);
                            result = SND_gul_MetaBankUserNb;
                        }
                        ++v6;
                    } while (v6 < result);
                }
            }
        }
    }
    return result;
}

int SND_l_AddStreamNoLoad(int a1)
{
    int result;
    tSndSoundElem* v2;

    if (!SND_gst_Params.soundEnabled)
        return -1;
    result = SND_l_GetSoundIndex(a1);
    if (result == -1)
    {
        result = SND_i_AllocSound();
        v2 = &SND_gst_Params.allocatedSounds[result];
        v2->soundKey = a1;
        v2->muteFacorFlags = 0;
        v2->freq = 0;
        v2->fullsoundLen = 0;
        v2->numChannels = 0;
        v2->bitsPerSample = 0;
        v2->sndSndBufElem = 0;
        v2->soundModifier = 0;
        v2->startDataPos = 0;
        v2->flags = 0x80020400;
        v2->usageCnt = 1;
        v2->sndInterface = &SND_gpst_Interface.stream;
    }
    else
    {
        ++SND_gst_Params.allocatedSounds[result].usageCnt;
    }
    return result;
}

int SND_l_AddDialogNoLoad(int a1)
{
    int result;
    tSndSoundElem* v2;

    result = SND_l_AddStreamNoLoad(a1);
    if (result != -1)
    {
        v2 = &SND_gst_Params.allocatedSounds[result];
        if (v2->usageCnt == 1)
        {
            v2->flags |= 0x4000u;
            v2->sndInterface = &SND_gpst_Interface.dialog;
        }
    }
    return result;
}

int SND_l_AddMusicNoLoad(int a1)
{
    int result;
    tSndSoundElem* v2;

    result = SND_l_AddStreamNoLoad(a1);
    if (result != -1)
    {
        v2 = &SND_gst_Params.allocatedSounds[result];
        if (v2->usageCnt == 1)
        {
            v2->flags |= 0x2000u;
            v2->muteFacorFlags |= 0x100u;
            v2->sndInterface = &SND_gpst_Interface.music;
        }
    }
    return result;
}

int SND_l_AddAmbienceNoLoad(int a1)
{
    int result;
    tSndSoundElem* v2;

    result = SND_l_AddStreamNoLoad(a1);
    if (result != -1)
    {
        v2 = &SND_gst_Params.allocatedSounds[result];
        if (v2->usageCnt == 1)
        {
            v2->muteFacorFlags |= 0x80u;
            v2->flags |= 0x80000u;
            v2->sndInterface = &SND_gpst_Interface.ambience;
        }
    }
    return result;
}

tSndBank* SND_ul_CallbackLoadBank(int Offset)
{
    char* v1;
    unsigned int v2;
    tSndBank* v4;
    int* v5;
    unsigned int v6;
    int v7;
    int v8;
    unsigned int i;
    char String1[4];
    char* a1;
    int size;

    v1 = BIG_pc_ReadFileTmp(Offset, &size);
    v2 = (unsigned int)size >> 3;
    a1 = v1;
    if (!((unsigned int)size >> 3))
        return 0;
    v4 = (tSndBank*)MEM_p_Alloc(sizeof(tSndBank));
    v4->usageCount = 0;
    v4->elemNum = 0;
    v4->bankKey = 0;
    v4->soundIndices = 0;
    v4->soundIndicesV2 = 0;
    v4->usageCount = 0;
    if (LOA_gb_SpeedMode && BIG_gi_ReadMode == 2)
        v2 = LOA_ReadLong(&a1, 0, 1);
    v4->elemNum = v2;
    v4->bankKey = LOA_ul_GetCurrentKey();
    v5 = (int*)MEM_p_Alloc(4 * v2);
    v4->soundIndices = v5;
    if (v5)
        memset(v5, 0xFFu, 4 * v2);
    v6 = 0;
    if (!v2)
        return v4;
    while (1)
    {
        v7 = LOA_ReadLong(&a1, 0, 1);
        LOA_ReadCharArray(&a1, String1, 4u, 1);
        if (!v7 || v7 == -1)
            goto LABEL_24;
        if (_strnicmp(String1, ".wad", 4u))
            break;
        v8 = SND_l_AddDialogNoLoad(v7);
    LABEL_23:
        v4->soundIndices[v6] = v8;
    LABEL_24:
        if (++v6 >= v2)
            return v4;
    }
    if (!_strnicmp(String1, ".wam", 4u))
    {
        v8 = SND_l_AddMusicNoLoad(v7);
        goto LABEL_23;
    }
    if (!_strnicmp(String1, ".waa", 4u))
    {
        v8 = SND_l_AddAmbienceNoLoad(v7);
        goto LABEL_23;
    }
    if (!_strnicmp(String1, ".smd", 4u))
    {
        v8 = SND_l_AddSModifierNoLoad(v7, &a1);
        goto LABEL_23;
    }
    if (!_strnicmp(String1, ".wac", 4u))
    {
        v8 = SND_l_AddLoadingSound(v7);
        goto LABEL_23;
    }
    if (!_strnicmp(String1, ".wav", 4u))
    {
        v8 = SND_l_AddSound(v7);
        goto LABEL_23;
    }
    for (i = 0; i < v6; ++i)
        SND_DeleteByIndex(v4->soundIndices[i]);
    MEM_Free(v4);
    return 0;
}

void SND_DelMdFBankUser(struct sSoundMdfStruct* a1)
{
    unsigned int v1;
    tSoundMdfStruct** v2;

    if (SND_gst_Params.soundEnabled)
    {
        if (a1)
        {
            v1 = 0;
            if (SND_gst_BankMdFUserList)
            {
                v2 = sndMdfBankUser;
                do
                {
                    if (v2[v1] == a1)
                    {
                        v2[v1] = 0;
                        v2 = sndMdfBankUser;
                    }
                    ++v1;
                } while (v1 < SND_gst_BankMdFUserList);
            }
        }
    }
}

void unknown_snd_familylist_check_And_free_sub_49A520(tSoundFamilyInnerStruct* a1)
{
    if (a1->i1 == 1)
        MEM_Free(a1);
}

void SND_LoadingSoundCloseModule()
{
    int v0;

    if (SND_gst_LSGlobal.soundInstanceIndex != -1)
    {
        v0 = SND_gst_LSGlobal.soundInstanceIndex;
        SND_gst_LSGlobal.soundInstanceIndex = -1;
        SND_Release(v0);
    }
    if (SND_gst_LSGlobal.soundElemIndex != -1)
    {
        SND_gst_LSGlobal.soundElemIndex = -1;
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
    int v0;
    int i;
    tSoundFamilyStruct* v2;
    int j; 

    v0 = 0;
    if (SND_gast_AndFamillyList)
    {
        for (i = 0; i < SND_gi_AndFamillyNb; ++v0)
        {
            v2 = &SND_gast_AndFamillyList[v0];
            for (j = 0; j < v2->innerNum; ++j)
                unknown_snd_familylist_check_And_free_sub_49A520(v2->unkIntArrPtr[j]);
            MEM_Free(v2->unkIntArrPtr);
            ++i;
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
    tTrackResourceElem* v0;

    BAS_bfree(&SND_gst_TrackId);
    SND_gst_TrackId.keyValPtr = 0;
    BAS_bfree(&SND_gst_TrackLookUpTable);
    SND_gst_TrackLookUpTable.keyValPtr = 0;
    v0 = SND_gast_TrackResources;
    do
    {
        SND_gpst_Interface.win.ReleaseTrack(v0->stereoSoundBuffer, v0->monoSoundBuffer);
        ++v0;
    } while ((int)v0 <= (int)&SND_gast_TrackResources[3]);
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
    tTrackListElem* v0;
    int result;
    tTrackListElem* v2;
    tTrackListElem* v3;
    int v4;
    int v5;
    int v6;
    int v7;

    v0 = SND_gax_TrackPlayingList;
    result = 0;
    if (SND_gi_TrackNb > 0)
    {
        v2 = SND_gax_TrackPlayingList;
        do
        {
            if (v2->flags == 1)
                break;
            ++result;
            ++v2;
        } while (result < SND_gi_TrackNb);
    }
    if (result == SND_gi_TrackNb)
    {
        if (SND_gi_TrackNb)
        {
            SND_gax_TrackPlayingList = (tTrackListElem*)MEM_p_Realloc(SND_gax_TrackPlayingList, sizeof(tTrackListElem) * (SND_gi_TrackNb + 16));
            v3 = &SND_gax_TrackPlayingList[SND_gi_TrackNb];
        }
        else
        {
            v3 = (tTrackListElem*)MEM_p_Alloc(sizeof(tTrackListElem) * 16);
            SND_gax_TrackPlayingList = v3;
        }
        memset(v3, 0, sizeof(tTrackListElem) * 16);
        v4 = SND_gi_TrackNb;
        v5 = SND_gi_TrackNb;
        if (SND_gi_TrackNb < (SND_gi_TrackNb + 16)) // __OFSUB__
        {
            v6 = SND_gi_TrackNb;
            do
            {
                SND_gax_TrackPlayingList[v6].flags = 1;
                v4 = SND_gi_TrackNb;
                ++v5;
                ++v6;
            } while (v5 < SND_gi_TrackNb + 16);
        }
        v0 = SND_gax_TrackPlayingList;
        result = v4;
        SND_gi_TrackNb = v4 + 16;
    }
    v7 = result;
    v0[v7].trackID = -1;
    SND_gax_TrackPlayingList[v7].soundIntf = 0;
    SND_gax_TrackPlayingList[v7].flags = 1;
    SND_gax_TrackPlayingList[v7].sndBufferElem = 0;
    SND_gax_TrackPlayingList[v7].trackResource = 0;
    SND_gax_TrackPlayingList[v7].trackResourceNum = 0;
    SND_gax_TrackPlayingList[v7].stereoToMono_sub499C20 = 0;
    SND_gax_TrackPlayingList[v7].unk1 = 0;
    SND_gax_TrackPlayingList[v7].f2 = 1.0f;
    SND_gax_TrackPlayingList[v7].f1 = 0.5f;
    return result;
}

void SND_TrackForceFreeRessourcesAllTrackOnList_sub_49A0D0(int idx)
{
    tTrackListElem* v1;
    int v2;
    tTrackResourceElem* v3;

    v1 = &SND_gax_TrackPlayingList[idx];
LABEL_2:
    v2 = 0;
    v3 = SND_gast_TrackResources;
    while (v3->isBusy)
    {
        ++v3;
        ++v2;
        if ((int)v3 > (int)&SND_gast_TrackResources[3])
        {
            SND_TrackForceFreeResources(idx);
            goto LABEL_2;
        }
    }
    v1->stereoToMono_sub499C20 = v2 + 1;
    SND_gast_TrackResources[v2].isBusy = 1;
    SND_gpst_Interface.win.StreamGetMonoToStereo(&v1->trackResource, &SND_gast_TrackResources[v2].stereoSoundBuffer);
}

void calls_SND_TrackForceFreeRessourcesAllTrackOnList_plus_setsomething_sub_49A1F0(int id)
{
    tTrackListElem* v1;
    tSndSoundBufferElem* v2;
    int v3;

    v1 = &SND_gax_TrackPlayingList[id];
    SND_TrackForceFreeRessourcesAllTrackOnList_sub_49A0D0(id);
    if (v1->stereoToMono_sub499C20)
    {
        v2 = v1->trackResource;
        v1->trackResource = v1->sndBufferElem;
        v3 = v1->flags | 4;
        v1->sndBufferElem = v2;
        v1->flags = v3;
    }
}

void SND_Track_Merge(int iid, tSoundInstanceElem* a2a, tSoundInstanceElem* a3, int a4, int a5, int a6)
{
    tTrackListElem* trackPlayingList;
    unsigned int id;
    tSoundInstanceElem* v8;
    __int16 v9;
    tSndSoundElem* v10;
    int v11;
    float v12;
    int a2;

    if (a2a)
    {
        SND_gpst_Interface.win.GetStatus(SND_gax_TrackPlayingList[iid].sndBufferElem, &a2);
        if ((a2 & 1) != 0)
            SND_gpst_Interface.win.Stop(SND_gax_TrackPlayingList[iid].sndBufferElem);
    }
    trackPlayingList = SND_gax_TrackPlayingList;
    id = iid;
    if (!SND_gax_TrackPlayingList[iid].trackResourceNum)
    {
        calls_SND_TrackForceFreeResourcesAllTrackOnList_plus_setsomething_first_sub_49A140(iid);
        trackPlayingList = SND_gax_TrackPlayingList;
    }
    v8 = a3;
    v9 = SND_gst_Params.allocatedSounds[a3->allocatedSoundsIdx].numChannels;
    if (v9 != 2 || (trackPlayingList[id].flags & 4) != 0)
    {
        if (v9 == 1 && (trackPlayingList[id].flags & 4) != 0)
            snd_track_merge_inner_type_1_sub_499C20(iid);
    }
    else
    {
        calls_SND_TrackForceFreeRessourcesAllTrackOnList_plus_setsomething_sub_49A1F0(iid);
    }
    SND_StreamFlushOneSound_0(v8->allocatedSoundsIdx, 0);
    v10 = &SND_gst_Params.allocatedSounds[v8->allocatedSoundsIdx];
    v12 = SND_f_GetInstVolume(v8);
    v11 = SND_l_GetAttFromVol(v12);
    SND_gpst_Interface.win.StreamReinitAndPlay(SND_gax_TrackPlayingList[id].sndBufferElem, v10->fullsoundLen, v10->startDataPos, a5,
                                                a6, a4, v8->freq, v8->pan, v11);
}

void SND_TrackRegisterAndPlayInstance(int idx, int a2, int from, int to)
{
    int v4;
    tSoundInstanceElem* sndInst;
    tSoundInstanceElem* v6;
    int newTrackIdx;
    int v8;
    int v9;
    int v10;
    unsigned int v11;
    tSndSoundBufferElem* v12;

    if (idx >= 0 && idx < SND_gst_Params.soundInstanceNum)
    {
        v4 = SND_gst_Params.soundInstances[idx].flags;
        sndInst = &SND_gst_Params.soundInstances[idx];
        if ((v4 & 0x400) != 0)
        {
            v6 = 0;
            if ((v4 & 0x86000) != 0)
            {
                newTrackIdx = BAS_bsearch(sndInst->trackID, &SND_gst_TrackId);
                if (newTrackIdx == -1)
                {
                    newTrackIdx = SND_i_TrackNew();
                    v8 = newTrackIdx;
                    SND_gax_TrackPlayingList[newTrackIdx].flags = 0;
                    sndInst->trackID = SND_gi_TrackLastVirtualId++;
                    BAS_binsert(sndInst->trackID, newTrackIdx, &SND_gst_TrackId);
                }
                else
                {
                    v8 = newTrackIdx;
                    if ((SND_gax_TrackPlayingList[newTrackIdx].flags & 2) != 0)
                    {
                        v9 = SND_gax_TrackPlayingList[v8].trackID;
                        v10 = SND_gst_Params.soundInstances[v9].flags;
                        v6 = &SND_gst_Params.soundInstances[v9];
                        v6->flags = v10 & ~0x808010u;
                        v12 = v6->soundBufElemFromModel;
                        v6->soundIntf = SND_gax_TrackPlayingList[v8].soundIntf;
                        BAS_bdelete((unsigned int)v12, &SND_gst_TrackLookUpTable);
                    }
                }
                SND_gax_TrackPlayingList[v8].trackID = idx;
                SND_gax_TrackPlayingList[v8].soundIntf = sndInst->soundIntf;
                SND_gax_TrackPlayingList[v8].flags |= 2u;
                BAS_binsert((unsigned int)sndInst->soundBufElemFromModel, newTrackIdx, &SND_gst_TrackLookUpTable);
                SND_Track_Merge(newTrackIdx, v6, sndInst, a2, from, to);
                v11 = sndInst->flags & ~0x828010u;
                sndInst->soundIntf = &SND_gpst_Interface.track;
                sndInst->flags = v11;
                if ((a2 & 1) != 0)
                    sndInst->flags = v11 | 0x820000;
                else
                    sndInst->flags = v11 | 0x28000;
            }
        }
    }
}

void SND_SModifierNotifyPlayInstance(tSoundInstanceElem* a1)
{
    tSndModifierElem* i;
    void(*v2)(tSndModifierElem*, tSoundInstanceElem*);

    for (i = a1->modifierInstances; i; i = i->nextModifier)
    {
        v2 = i->sndModifierModelIntf->NotifyPlay;
        if (v2)
            v2(i, a1);
    }
}

void SND_PlayRq(unsigned int from, tSoundInstanceElem* sie, int flags, int to)
{
    int v6;
    int v7;
    int v8;
    double v9;
    tSndSoundElem* v10;
    BOOL v11;
    double v12;
    int v13;
    tSoundInstanceElem* v14;
    int v15;
    double v16;
    int v17;
    tSndInterfaceStruct* v18;
    int v19;
    int v20;
    int v21;
    tSoundInstanceElem* v22;
    BOOL v23;
    int v24;
    int v25;
    tSoundInstanceElem* v26;
    BOOL v27;
    tSndInterfaceStruct* v28;
    int v29;
    double v30;
    int v31;
    int v32;
    int status;
    float v34;
    int v35;

    if ((SND_gst_Params.flags & 0x10000) == 0)
    {
        v6 = sie->muteFactorFlags & ~0x1000u;
        sie->muteFactorFlags = v6;
        if ((v6 & 0x800) != 0)
        {
            v7 = sie->freq;
            if (v7)
            {
                v8 = sie->allocatedSoundsIdx;
                v9 = (double)SND_gst_Params.allocatedSounds[v8].fullsoundLen;
                v10 = &SND_gst_Params.allocatedSounds[v8];
                v11 = ((v7 * v10->numChannels) & 0x40000000) != 0;
                v12 = (double)(2 * v7 * v10->numChannels);
            }
            else
            {
                v13 = sie->allocatedSoundsIdx;
                v9 = (double)SND_gst_Params.allocatedSounds[v13].fullsoundLen;
                v12 = (double)(int)SND_gst_Params.allocatedSounds[v13].byteRate;
                v11 = (SND_gst_Params.allocatedSounds[v13].byteRate & 0x80000000) != 0;
            }
            if (v11)
                v12 = v12 + 4294967300.0;
            v34 = (float)(v9 / v12);
            sie->timeFromDefault = (float)((double)rand() * 0.000030518509 * v34);
        }
        v14 = 0;
        if (!from && sie->timeFromDefault != 0.0)
        {
            v15 = sie->allocatedSoundsIdx;
            v16 = (double)(2 * sie->freq * SND_gst_Params.allocatedSounds[v15].numChannels);
            if (((sie->freq * SND_gst_Params.allocatedSounds[v15].numChannels) & 0x40000000) != 0)
                v16 = v16 + 4294967300.0;
            v30 = v16 * sie->timeFromDefault + MATH_gd_magicDoubleForRound;
            from = (*(uint32_t*)(&v30)) & ~3u;
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
        if ((flags & 0x8000000) != 0 || (v17 = sie->flags, (v17 & 0x86000) != 0))
        {
            sie->requestTimeStamp = TIM_gf_MainClock;
            SND_TrackRegisterAndPlayInstance(((unsigned int)((char*)sie - (char*)SND_gst_Params.soundInstances) / sizeof(tSoundInstanceElem)),         // elsõ az idx
                flags,
                from,
                to);
        }
        else
        {
            v18 = sie->soundIntf;
            sie->flags = v17 & ~0x20000u;
            sie->requestTimeStamp = TIM_gf_MainClock;
            if (v18->Play(sie->soundBufElemFromModel, flags) >= 0)
                goto LABEL_46;
            v19 = SND_gst_Params.soundInstanceNum;
            v20 = 0;
            v35 = 0;
            if (SND_gst_Params.soundInstanceNum > 0)
            {
                v31 = 0;
                do
                {
                    v21 = SND_gst_Params.soundInstances[v20].flags;
                    v22 = &SND_gst_Params.soundInstances[v20];
                    if ((v21 & 0x400) != 0 && (v21 & 0x82000) == 0)
                    {
                        v22->soundIntf->GetStatus(v22->soundBufElemFromModel, &status);
                        if ((status & 4) == 0
                            && (status & 1) != 0
                            && (!v14 || v22->requestTimeStamp < (double)v14->requestTimeStamp))
                        {
                            v14 = v22;
                        }
                    }
                    v19 = SND_gst_Params.soundInstanceNum;
                    ++v20;
                    v23 = ++v35 < SND_gst_Params.soundInstanceNum;
                    ++v31;
                } while (v23);
                if (v14)
                    goto LABEL_52;
            }
            v24 = 0;
            v35 = 0;
            if (v19 > 0)
            {
                v32 = 0;
                do
                {
                    v25 = SND_gst_Params.soundInstances[v24].flags;
                    v26 = &SND_gst_Params.soundInstances[v24];
                    if ((v25 & 0x400) != 0 && (v25 & 0x82000) == 0)
                    {
                        v26->soundIntf->GetStatus(v26->soundBufElemFromModel, &status);
                        if ((status & 1) != 0 && (!v14 || v26->requestTimeStamp < (double)v14->requestTimeStamp))
                            v14 = v26;
                    }
                    v24 = v32 + 1;
                    v23 = ++v35 < SND_gst_Params.soundInstanceNum;
                    ++v32;
                } while (v23);
                if (v14)
                {
                LABEL_52:
                    v14->soundIntf->Stop(v14->soundBufElemFromModel);
                    v27 = (v14->flags & 0x20000) == 0;
                    v14->flags &= ~0x808000u;
                    if (!v27)
                        SND_TrackDeleteInstanceFromList((unsigned int)((char*)v14 - (char*)SND_gst_Params.soundInstances) / sizeof(tSoundInstanceElem));
                    v28 = sie->soundIntf;
                    sie->requestTimeStamp = TIM_gf_MainClock;
                    if (v28->Play(sie->soundBufElemFromModel, flags) >= 0)
                    {
                    LABEL_46:
                        v29 = sie->flags;
                        if ((flags & 1) != 0)
                            sie->flags = v29 | 0x800000;
                        else
                            sie->flags = v29 | 0x8000;
                    }
                }
            }
        }
    }
}

int SND_b_SndIsInActiveVolume(int a1)
{
    tSoundInstanceElem* v1;
    tSoundInstanceElem* v2;
    int v3;
    Mtx4x4* v4;
    int v6;
    int v7;
    double v8;
    double v9;
    double v10;
    Vec3* v11;
    Vec3* v12;
    Vec3 dest;
    int v14[3];

    if (!SND_gst_Params.sndMatrixRef)
        return 1;
    if (a1 >= 0 && a1 < SND_gst_Params.soundInstanceNum)
    {
        v1 = &SND_gst_Params.soundInstances[a1];
        v2 = v1;
        v3 = v1->flags;
        if ((v3 & 0x400) != 0 && (v3 & 0x17000000) != 0)
        {
            if (v1->sndPositionSetInSetPosPtr)
            {
                v4 = v1->sndPosMtxSetInSetPos;
                if (v4 || (v3 & 0x7000000) == 0)
                {
                    v12 = v1->sndPositionSetInSetPosPtr;
                    v11 = &SND_gst_Params.sndMatrixRef->Pos;
                    if ((v3 & 0x10000000) != 0)
                        return v1->dtFar + v1->farArr[0] > MATH_DistanceOfTwoPoints(v11, v12);
                    v14[0] = 0x1000000;
                    v14[1] = 0x2000000;
                    v14[2] = 0x4000000;
                    MATH_SubstractVec(&dest, v11, v12);
                    v6 = 0;
                    while (1)
                    {
                        v7 = v14[v6];
                        if ((v7 & v3) != 0)
                            break;
                        v14[v6] = 2;
                    LABEL_26:
                        if (++v6 >= 3)
                            return (uint8_t)(v14[2] & v14[1] & (v14[0] & 0xff) & 2) >> 1; //return (uint8_t)(v14[2] & v14[1] & LOBYTE(v14[0]) & 2) >> 1;
                    }
                    if (v6)
                    {
                        if (v6 == 1)
                        {
                            v9 = v4->v2.z * dest.z + v4->v2.y * dest.y;
                            v10 = dest.x * v4->v2.x;
                        }
                        else
                        {
                            if (v6 != 2)
                            {
                                v8 = 0.0;
                                goto LABEL_22;
                            }
                            v9 = v4->v3.z * dest.z + v4->v3.y * dest.y;
                            v10 = dest.x * v4->v3.x;
                        }
                    }
                    else
                    {
                        v9 = v4->v1.z * dest.z + v4->v1.y * dest.y;
                        v10 = dest.x * v4->v1.x;
                    }
                    v8 = fabs(v9 + v10);
                LABEL_22:
                    if (v8 >= v2->farArr[v6] + v2->dtFar)
                        v14[v6] = 0;
                    else
                        v14[v6] = v7 | 2;
                    goto LABEL_26;
                }
            }
        }
    }
    return 0;
}

void SND_PlaySound(int a1)
{
    int v1;
    tSoundInstanceElem* sndInst;
    double v3;

    v1 = a1;
    if (a1 >= 0 && a1 < SND_gst_Params.soundInstanceNum)
    {
        sndInst = &SND_gst_Params.soundInstances[a1];
        if ((sndInst->flags & 0x400) != 0)
        {
            if ((sndInst->muteFactorFlags & 0x1000) != 0
                || (sndInst->soundIntf->GetStatus(sndInst->soundBufElemFromModel, &a1),
                    (a1 & 1) == 0))
            {
                v3 = sndInst->timeRemainTillStart;
                sndInst->flags = sndInst->flags & ~4u | 0x200;
                if (v3 == 0.0)
                {
                    a1 = 1;
                    if ((sndInst->flags & 0x17000000) == 0 || SND_b_SndIsInActiveVolume(v1) && a1)
                    {
                        SND_SetInstVolume(sndInst);
                        SND_PlayRq(0, sndInst, 0, -1);
                    }
                }
            }
        }
    }
}

void SND_PlaySoundLooping(int a1)
{
    int v1;
    tSoundInstanceElem* v2;

    if (a1 >= 0 && a1 < SND_gst_Params.soundInstanceNum)
    {
        v1 = SND_gst_Params.soundInstances[a1].flags;
        v2 = &SND_gst_Params.soundInstances[a1];
        if ((v1 & 0x400) != 0)
        {
            v2->flags = v1 & ~0x80u | 0x204;
            if ((v2->muteFactorFlags & 0x1000) != 0
                || (v2->soundIntf->GetStatus(v2->soundBufElemFromModel, &a1),
                    (a1 & 1) == 0))
            {
                SND_SetInstVolume(v2);
                SND_PlayRq(0, v2, 1, -1);
            }
        }
    }
}

void SND_PlaySoundOnTrack(int a1)
{
    tSoundInstanceElem* v1;
    double v2;

    if (a1 >= 0 && a1 < SND_gst_Params.soundInstanceNum)
    {
        v1 = &SND_gst_Params.soundInstances[a1];
        if ((v1->flags & 0x400) != 0)
        {
            if ((v1->muteFactorFlags & 0x1000) != 0
                || (v1->soundIntf->GetStatus(v1->soundBufElemFromModel, &a1),
                    (a1 & 1) == 0))
            {
                v2 = v1->timeRemainTillStart;
                v1->flags = v1->flags & 0xFFFFFFFB | 0x200;
                if (v2 == 0.0)
                {
                    SND_SetInstVolume(v1);
                    SND_PlayRq(0, v1, 0x8000000, -1);
                }
            }
        }
    }
}

void SND_PlaySoundLoopingOnTrack(int a1)
{
    tSoundInstanceElem* v1;
    int v2;

    if (a1 >= 0 && a1 < SND_gst_Params.soundInstanceNum)
    {
        v1 = &SND_gst_Params.soundInstances[a1];
        v2 = v1->flags;
        if ((v2 & 0x400) != 0)
        {
            v1->flags = v2 | 0x204;
            if ((v1->muteFactorFlags & 0x1000) != 0
                || (v1->soundIntf->GetStatus(v1->soundBufElemFromModel, &a1),
                    (a1 & 1) == 0))
            {
                SND_SetInstVolume(v1);
                SND_PlayRq(0, v1, 0x8000001, -1);
            }
        }
    }
}

int SND_i_InstanceSetf(int a1, int a2, float a3)
{
    tSoundInstanceElem* v3;
    int result;

    if (a1 < 0)
        return -1;
    if (a1 >= SND_gst_Params.soundInstanceNum)
        return -1;
    v3 = &SND_gst_Params.soundInstances[a1];
    if ((v3->flags & 0x400) == 0)
        return -1;
    switch (a2)
    {
    case 4099:
        v3->centerArr[0] = a3;
        result = 0;
        break;
    case 4100:
        v3->centerArr[1] = a3;
        result = 0;
        break;
    case 4101:
        v3->centerArr[2] = a3;
        result = 0;
        break;
    case 4102:
        v3->farCoefficient = a3;
        result = 0;
        break;
    case 4103:
        v3->middleCoefficient = a3;
        result = 0;
        break;
    case 4104:
        v3->dtFar = a3;
        result = 0;
        break;
    case 4105:
        v3->minPan = a3;
        result = 0;
        break;
    case 4106:
        v3->nearArr[0] = a3;
        result = 0;
        break;
    case 4107:
        v3->farArr[0] = a3;
        result = 0;
        break;
    case 4108:
        v3->nearArr[1] = a3;
        result = 0;
        break;
    case 4109:
        v3->farArr[1] = a3;
        result = 0;
        break;
    case 4110:
        v3->nearArr[2] = a3;
        result = 0;
        break;
    case 4111:
        v3->farArr[2] = a3;
        result = 0;
        break;
    case 4114:
        v3->timeRemainTillStart = a3;
        result = 0;
        break;
    case 4117:
        v3->timeFromDefault = a3;
        result = 0;
        break;
    default:
        return -1;
    }
    return result;
}

void SND_SetFlags(struct sGameObjectMain* a1, int idx, int a3)
{
    tSoundInstanceElem* v3;
    int v4;
    int v5;

    if (idx >= 0 && idx < SND_gst_Params.soundInstanceNum)
    {
        v3 = &SND_gst_Params.soundInstances[idx];
        v4 = v3->flags;
        if ((v4 & 0x400) != 0)
        {
            if ((a3 & 0x40000000) != 0)
            {
                if ((a3 & 0x1C0) != 0)
                    v3->muteFactorFlags &= ~0x1C0u;
                v5 = a3 & 0xFCB | v3->muteFactorFlags;
                v3->muteFactorFlags = v5;
                if ((v5 & 0x40) != 0)
                {
                    v3->unkMuteSetupSeeInitVolTable = &SND_Mutes_B30600_fadergroups[7];
                }
                else if ((v5 & 0x80u) == 0)
                {
                    if ((v5 & 0x100) != 0)
                        v3->unkMuteSetupSeeInitVolTable = &SND_Mutes_B30600_fadergroups[9];
                    else
                        v3->unkMuteSetupSeeInitVolTable = &SND_Mutes_B30600_fadergroups[5];
                }
                else
                {
                    v3->unkMuteSetupSeeInitVolTable = &SND_Mutes_B30600_fadergroups[8];
                }
            }
            else
            {
                v3->flags = a3 | v4;
            }
        }
    }
}

int SND_i_CheckOrInitGAO(struct sGameObjectMain* a1)
{
    tSndBank* v2;

    if (!a1 || ((uint8_t)a1 & 3) != 0 || MEM_b_CheckBlockHasBeenFreed(a1) || (a1->flags & 0x8000000) != 0)
        return 1;
    OBJ_GameObject_CreateExtendedIfNot(a1);
    v2 = (tSndBank*)MEM_p_Alloc(sizeof(tSndBank));
    v2->usageCount = 0;
    v2->elemNum = 0;
    v2->bankKey = 0;
    v2->soundIndices = 0;
    v2->soundIndicesV2 = 0;
    v2->usageCount = 1;
    v2->bankKey = -1;
    v2->elemNum = 0;
    v2->soundIndices = 0;
    v2->soundIndicesV2 = 0;
    a1->extended->snd = v2;
    a1->flags |= 0x8000000u;
    return 0;
}

void SND_SetPos(struct sGameObjectMain* a1, int a2, Mtx4x4* a3)
{
    tSoundInstanceElem* v3;

    if (a2 >= 0 && a2 < SND_gst_Params.soundInstanceNum)
    {
        v3 = &SND_gst_Params.soundInstances[a2];
        if ((v3->flags & 0x400) != 0)
        {
            v3->sndPosMtxSetInSetPos = a3;
            v3->sndPositionSetInSetPosPtr = &a3->Pos;
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

void SND_SetVol(int a1, float a2)
{
    tSoundInstanceElem* v2;

    if (a2 >= 0.0000001)
    {
        if (a2 > 1.0)
            a2 = 1.0;
    }
    else
    {
        a2 = 0.0;
    }
    if (a1 >= 0 && a1 < SND_gst_Params.soundInstanceNum)
    {
        v2 = &SND_gst_Params.soundInstances[a1];
        if ((v2->flags & 0x400) != 0)
        {
            v2->volume = a2;
            SND_SetInstVolume(v2);
        }
    }
}

void SND_SetNearFar(int idx, int type, float near1, float far1)
{
    tSoundInstanceElem* v4;
    BOOL v5; // zf

    if (idx >= 0 && idx < SND_gst_Params.soundInstanceNum)
    {
        v4 = &SND_gst_Params.soundInstances[idx];
        if ((v4->flags & 0x400) != 0)
        {
            switch (type)
            {
            case 4096:
                v4->nearArr[0] = near1;
                v5 = (*((uint32_t*)(&v4->centerArr[0]))) == 0x42340000;
                v4->farArr[0] = far1;
                if (v5)
                    v4->centerArr[0] = (float)((near1 + far1) * 0.5);
                break;
            case 4097:
                v4->nearArr[1] = near1;
                v5 = (*((uint32_t*)(&v4->centerArr[1]))) == 0x42340000;
                v4->farArr[1] = far1;
                if (v5)
                    v4->centerArr[1] = (float)((near1 + far1) * 0.5);
                break;
            case 4098:
                v4->nearArr[2] = near1;
                v5 = (*((uint32_t*)(&v4->centerArr[2]))) == 0x42340000;
                v4->farArr[2] = far1;
                if (v5)
                    v4->centerArr[2] = (float)((near1 + far1) * 0.5);
                break;
            }
        }
    }
}

void SND_SModifierNotifyPauseInstance(tSoundInstanceElem* a1)
{
    tSndModifierElem* i;
    void(__cdecl * v2)(tSndModifierElem*, tSoundInstanceElem*);

    for (i = a1->modifierInstances; i; i = i->nextModifier)
    {
        v2 = i->sndModifierModelIntf->NotifyPause;
        if (v2)
            v2(i, a1);
    }
}

void SND_Pause(int a1)
{
    int result;
    tSoundInstanceElem* v2;

    result = a1;
    if (a1 >= 0 && a1 < SND_gst_Params.soundInstanceNum)
    {
        v2 = &SND_gst_Params.soundInstances[a1];
        result = v2->flags;
        if ((result & 0x400) != 0)
        {
            SND_SModifierNotifyPauseInstance(v2);
            v2->soundIntf->Pause(v2->soundBufElemFromModel);
        }
    }
}

void SND_SetFreq(int a1, int a2)
{
    tSoundInstanceElem* v2;

    if ((SND_gst_Params.flags & 0x80000) == 0 && a1 >= 0 && a1 < SND_gst_Params.soundInstanceNum)
    {
        v2 = &SND_gst_Params.soundInstances[a1];
        if ((v2->flags & 0x400) != 0)
        {
            v2->soundIntf->SetFrequency(v2->soundBufElemFromModel, a2);
            v2->freq = a2;
        }
    }
}

void SND_EngineSyncMdF(tSoundMdfStruct* a1, int a2)
{
    int v2;
    tSoundInstanceElem* v3;
    tSoundInstanceElem* v4;
    int v5;
    double v6;
    double v7;
    int v8;
    int v9;
    int32_t v7a;
    double v7b;

    if (SND_gst_Params.soundEnabled)
    {
        v2 = a1->sndInstanceID;
        if (v2 >= 0 && (a1->flags & 0x10000) != 0 && ENG_gb_EVERunning && EVE_gpst_CurrentParam->currTime <= 1.1)
        {
            if (EVE_gpst_CurrentParam->currTime <= 0.80000001)
            {
                if (v2 < SND_gst_Params.soundInstanceNum)
                {
                    v3 = &SND_gst_Params.soundInstances[v2];
                    v4 = v3;
                    if ((v3->flags & 0x400) != 0)
                    {
                        if (a2)
                        {
                            v3->soundIntf->GetCurrPos(v3->soundBufElemFromModel, &v9, &v8);
                            if (v9)
                            {
                                v5 = v4->allocatedSoundsIdx;
                                v6 = (double)(2 * SND_gst_Params.allocatedSounds[v5].numChannels);
                                if ((SND_gst_Params.allocatedSounds[v5].numChannels & 0x40000000) != 0)
                                    v6 = v6 + 4294967300.0;
                                v7 = v6 * a1->freq * EVE_gpst_CurrentParam->currTime + MATH_gd_magicDoubleForRound; // TODO FIXME ezek biztos jók?
                                v7a = *((int32_t*)&v7);
                                v7b = (double)v7a / (double)v9 * a1->freq + MATH_gd_magicDoubleForRound;
                                SND_SetFreq(a1->sndInstanceID, *((int32_t*)&v7b));
                                SND_SetVol(a1->sndInstanceID, 0.0);
                            }
                        }
                        else
                        {
                            a1->freq = (float)v3->freq;
                        }
                    }
                }
            }
            else
            {
                v7b = a1->freq + MATH_gd_magicDoubleForRound;
                SND_SetFreq(v2, *((int32_t*)&v7b));
                SND_SetVol(a1->sndInstanceID, a1->volume);
            }
        }
    }
}

void SND_PlayMdF(struct sSoundMdfStruct* a1)
{
    int v1;
    int v2;
    int v3;

    if (SND_gst_Params.soundEnabled && a1->sndInstanceID >= 0)
    {
        v1 = a1->flags;
        switch (v1 & 0x84)
        {
        case 0:
            goto LABEL_5;
        case 4:
            if ((v1 & 0x10000) != 0)
                SND_EngineSyncMdF(a1, 1);
            break;
        case 0x80:
            a1->flags = v1 & ~0x80u;
        LABEL_5:
            v2 = a1->flags | 4;
            a1->flags = v2;
            if ((v2 & 0x10000) != 0)
                SND_EngineSyncMdF(a1, 0);
            if (a1->ii2 == 0.0)
                SND_SetVol(a1->sndInstanceID, a1->volume);
            if (a1->timeRemainTillStart != 0.0)
                SND_i_InstanceSetf(a1->sndInstanceID, 4114, a1->timeRemainTillStart);
            v3 = a1->flags2;
            if ((v3 & 0x20000) != 0)
            {
                if ((v3 & 4) != 0)
                    SND_PlaySoundLoopingOnTrack(a1->sndInstanceID);
                else
                    SND_PlaySoundOnTrack(a1->sndInstanceID);
            }
            else if ((v3 & 4) != 0)
            {
                SND_PlaySoundLooping(a1->sndInstanceID);
            }
            else
            {
                SND_PlaySound(a1->sndInstanceID);
            }
            break;
        case 0x84:
            SND_Pause(a1->sndInstanceID);
            a1->flags &= ~0x80u;
            break;
        default:
            return;
        }
    }
}

void SND_ReinitLoadingInstance()
{
    int v0;

    if (SND_gst_LSGlobal.soundInstanceIndex != -1)
    {
        v0 = SND_gst_LSGlobal.soundInstanceIndex;
        SND_gst_LSGlobal.soundInstanceIndex = -1;
        if (SND_gst_LSGlobal.userCallBackFunction)
            SND_gst_LSGlobal.userCallBackFunction(SND_gst_LSGlobal.userCallBackFunctionParam);
        SND_gst_LSGlobal.userCallBackFunction = 0;
        SND_gst_LSGlobal.userCallBackFunctionParam = 0;
        SND_Release(v0);
    }
}

int SND_l_LoadLoadingSoundData(int soundKEy)
{
    int v2;
    tSndSoundElem* v3;
    int v4;
    tSndSoundBufferElem* v5;
    int v6;
    tSndWavHdrStruct v7;

    if (!SND_gst_Params.soundEnabled)
        return -1;
    v2 = SND_l_GetSoundIndex(soundKEy);
    if (v2 == -1)
        return -1;
    if (SND_gst_LSGlobal.soundElemIndex != v2)
    {
        v3 = &SND_gst_Params.allocatedSounds[v2];
        if ((v3->flags & 1) == 0)
        {
            if (SND_gst_LSGlobal.soundElemIndex != -1)
            {
                SND_ReinitLoadingInstance();
                v4 = SND_gst_LSGlobal.soundElemIndex;
                v6 = SND_gst_LSGlobal.soundElemIndex;
                SND_gst_LSGlobal.soundElemIndex = -1;
                SND_DetachSound(v6);
                SND_gst_Params.allocatedSounds[v4].flags &= ~0x41u;
            }
            v7.wfex.wFormatTag = v3->format;
            v7.wfex.nChannels = v3->numChannels;
            v7.wfex.nSamplesPerSec = v3->freq;
            v7.wfex.nAvgBytesPerSec = v3->byteRate;
            v7.wfex.nBlockAlign = v3->blockAlign;
            v7.wfex.wBitsPerSample = v3->bitsPerSample;
            v7.wfex.cbSize = v3->extraDataSize;
            v7.fullSoundSize = v3->fullsoundLen;
            v7.dataStartPos = v3->startDataPos;
            SND_gi_RasterCurrentFileId = 8;
            v5 = SND_gpst_Interface.win.Reload(SND_gst_Params.handles, SND_gst_LSGlobal.soundElemElem, &v7, v3->startDataPos);
            v3->sndSndBufElem = v5;
            if (!v5)
                return -1;
            SND_gst_LSGlobal.soundElemElem = v5;
            SND_gst_LSGlobal.soundElemIndex = v2;
            if (v3->format != 2)
                return -1;
            v3->flags = 0x200409;
        }
    }
    return v2;
}

void SND_l_AddSModifierLoad(int a1)
{
    int v1; // eax
    tSndSoundElem* v2; // ebx
    tSndModifierElem* i; // edi
    tLinkedSoundFade* v4; // esi
    int v5; // ecx
    int v6; // eax
    int v7; // eax
    int v8; // [esp+4h] [ebp-4h]

    if (SND_gst_Params.soundEnabled)
    {
        if (a1)
        {
            if (a1 != -1)
            {
                v1 = SND_l_GetSoundIndex(a1);
                if (v1 != -1)
                {
                    v8 = v1;
                    v2 = &SND_gst_Params.allocatedSounds[v1];
                    for (i = v2->soundModifier; i; i = i->nextModifier)
                    {
                        v4 = (tLinkedSoundFade*)i->linkedSoundData;
                        v5 = v4->sndIndexInAllocatedSounds;
                        if (v5 != -1)
                        {
                            if ((SND_gst_Params.allocatedSounds[v5].flags & 1) == 0)
                            {
                                v6 = v4->flags;                 // 4000 = dialog, 2000 = music, 80000 = ambience
                                if ((v6 & 0x4000) != 0 || (v6 & 0x2000) != 0 || (v6 & 0x80000) != 0)
                                {
                                    v7 = SND_l_AddStreamLoad(v4->sndKey);
                                }
                                else if ((v6 & 8) != 0)
                                {
                                    SND_l_LoadLoadingSoundHeader(v4->sndKey);
                                    v7 = SND_l_LoadLoadingSoundData(v4->sndKey);
                                }
                                else
                                {
                                    SND_l_LoadHeader(v4->sndKey);
                                    v7 = SND_l_LoadData(v4->sndKey);
                                }
                                if (v7 == -1)
                                {
                                    SND_DeleteByIndex(v4->sndIndexInAllocatedSounds);
                                    v4->sndIndexInAllocatedSounds = -1;
                                }
                                v2 = &SND_gst_Params.allocatedSounds[v8];
                            }
                            v2->sndSndBufElem = (tSndSoundBufferElem*)v4->sndIndexInAllocatedSounds;
                            SND_gs_SoundModifiers[i->sndModifierModelIntf->Type].sndModifierModelIntf->PlayApplyOnSound(i, i->linkedSoundData, i->linkedSoundDataSize, v2);
                        }
                    }
                    v2->flags |= 1u;
                }
            }
        }
    }
}

int SND_l_GetUnusedInstance()
{
    int v0;
    int result;
    tSoundInstanceElem* v2;
    int v3;
    tSoundInstanceElem* v4;

    v0 = SND_gst_Params.soundInstanceNum;
    result = 0;
    if (SND_gst_Params.soundInstanceNum <= 0)
    {
    LABEL_5:
        v3 = SND_gst_Params.soundInstanceMaxNum;
        if (SND_gst_Params.soundInstanceNum >= SND_gst_Params.soundInstanceMaxNum)
        {
            if (SND_gst_Params.soundInstanceMaxNum)
            {
                SND_gst_Params.soundInstanceMaxNum += 200;
                v4 = (tSoundInstanceElem*)MEM_p_Realloc(SND_gst_Params.soundInstances, sizeof(tSoundInstanceElem) * (v3 + 200));
            }
            else
            {
                SND_gst_Params.soundInstanceMaxNum = 200;
                v4 = (tSoundInstanceElem*)MEM_p_Alloc(sizeof(tSoundInstanceElem) * 200);
            }
            SND_gst_Params.soundInstances = v4;
            memset(&v4[SND_gst_Params.soundInstanceMaxNum - 200], 0, sizeof(tSoundInstanceElem) * 200);
            v0 = SND_gst_Params.soundInstanceNum;
        }
        result = v0;
        SND_gst_Params.soundInstanceNum = v0 + 1;
    }
    else
    {
        v2 = SND_gst_Params.soundInstances;
        while ((v2->flags & 0x400) != 0)
        {
            ++result;
            ++v2;
            if (result >= SND_gst_Params.soundInstanceNum)
                goto LABEL_5;
        }
    }
    return result;
}

void SND_SetTrack(int a1, unsigned int a2)
{
    int v2;
    tSoundInstanceElem* v3;
    int v4;
    int v5;

    if (a1 >= 0 && a1 < SND_gst_Params.soundInstanceNum)
    {
        v2 = SND_gst_Params.soundInstances[a1].flags;
        v3 = &SND_gst_Params.soundInstances[a1];
        if ((v2 & 0x400) != 0 && a2 != -1 && a2 != v3->trackID)
        {
            if ((v2 & 0x20000) != 0)
                SND_TrackDeleteInstanceFromList(a1);
            v4 = v3->flags | 0x20000;
            v3->trackID = a2;
            v3->flags = v4;
            if (BAS_bsearch(a2, &SND_gst_TrackId) == -1)
            {
                v5 = SND_i_TrackNew();
                SND_gax_TrackPlayingList[v5].flags = 0;
                BAS_binsert(a2, v5, &SND_gst_TrackId);
            }
        }
    }
}

int SND_l_Request(struct sGameObjectMain* gao, int a2)
{
    tGameObjectExtended* v3;
    tSndBank* v4;
    int* v5;
    int v6; 
    int v7; 
    unsigned int v8;
    tSndSoundElem* v9; 
    unsigned int v10; 
    int v11;
    int v12;
    tSoundInstanceElem* v13;
    int v14;
    int v15;
    BOOL v16;
    int v17; 
    tSoundInstanceElem* v18;
    int v19;
    int a1;

    if (SND_gc_NoSound)
        return -1;
    if ((SND_gst_Params.flags & 0x20000) != 0)
        return -1;
    if (gao)
    {
        if ((gao->flags & 0x8000000) == 0)
            return -1;
        v3 = gao->extended;
        if (!v3)
            return -1;
        v4 = v3->snd;
        if (!v4)
            return -1;
        v5 = v4->soundIndices;
        if (!v5 || !SND_gst_Params.soundEnabled || a2 < 0 || a2 >= v4->elemNum)
            return -1;
        v6 = v5[a2];
        a2 = v6;
    }
    else
    {
        v6 = a2;
    }
    if (v6 == -1 || v6 < 0 || v6 >= SND_gst_Params.allocatedSoundsMaxNum)
        return -1;
    v7 = v6;
    v8 = SND_gst_Params.allocatedSounds[v7].flags;
    v9 = &SND_gst_Params.allocatedSounds[v7];
    if ((v8 & 1) == 0)
    {
        if ((v8 & 0x10000) != 0)
        {
            SND_l_AddSModifierLoad(v9->soundKey);
        }
        else if ((v8 & 0x4000) != 0 || (v8 & 0x2000) != 0 || (v8 & 0x80000) != 0)
        {
            SND_l_AddStreamLoad(v9->soundKey);
        }
        else if ((v8 & 8) != 0)
        {
            SND_l_LoadLoadingSoundHeader(v9->soundKey);
            SND_l_LoadLoadingSoundData(v9->soundKey);
        }
        else
        {
            SND_l_LoadHeader(v9->soundKey);
            SND_l_LoadData(v9->soundKey);
        }
    }
    v10 = v9->flags;
    if ((v10 & 0x400) == 0 || (v10 & 1) == 0)
        return -1;
    if (v9->soundModifier)
    {
        if (v9->sndSndBufElem != (tSndSoundBufferElem*)-1)
        {
            v11 = SND_l_Request(0, (int)v9->sndSndBufElem);
            v12 = v11;
            if (v11 != -1)
            {
                v13 = &SND_gst_Params.soundInstances[v11];
                v13->gao = gao;
                SND_SModifierInstanciate(v9->soundModifier, v13);
                SND_SetInstVolume(v13);
                v13->soundIntf->SetFrequency(v13->soundBufElemFromModel, v13->freq);
            }
            return v12;
        }
        return -1;
    }
    v14 = SND_l_GetUnusedInstance();
    a1 = v14;
    if (v14 == -1)
        return -1;
    if ((v9->flags & 0x80000000) == 0)
    {
        v17 = SND_AllocatedSoundsNumWithhoutFlag80000000 + 1;
        v16 = SND_AllocatedSoundsNumWithhoutFlag80000000PeakUsage < ++SND_AllocatedSoundsNumWithhoutFlag80000000;
        if (v16)
            SND_AllocatedSoundsNumWithhoutFlag80000000PeakUsage = v17;
    }
    else
    {
        v15 = SND_AllocatedSoundsNumWithFlag80000000 + 1;
        v16 = SND_AllocatedSoundsNumWithFlag80000000PeakUsage < ++SND_AllocatedSoundsNumWithFlag80000000;
        if (v16)
            SND_AllocatedSoundsNumWithFlag80000000PeakUsage = v15;
    }
    v18 = &SND_gst_Params.soundInstances[v14];
    v18->allocatedSoundsIdx = a2;
    v18->nearArr[0] = 10.0;
    v18->nearArr[1] = 10.0;
    v18->nearArr[2] = 10.0;
    v18->dtFar = 2.0;
    v18->middleCoefficient = 0.5;
    v18->farCoefficient = 0.0;
    v18->sndPosMtxSetInSetPos = 0;
    v18->sndPositionSetInSetPosPtr = 0;
    v18->farArr[0] = 80.0;
    v18->farArr[1] = 80.0;
    v18->farArr[2] = 80.0;
    v18->centerArr[0] = 45.0;
    v18->centerArr[1] = 45.0;
    v18->centerArr[2] = 45.0;
    v18->sndInstPosVecPrevious.x = 0.0f;
    v18->sndInstPosVecPrevious.y = 0.0f;
    v18->sndInstPosVecPrevious.z = 0.0f;
    v18->sndInstPosVec.x = 0.0;
    v18->sndInstPosVec.y = 0.0;
    v18->sndInstPosVec.z = 0.0;
    v18->flags = v9->flags & 0x80186408 | 0x400;
    v18->muteFactorFlags = v9->muteFacorFlags & 0xFCB;
    v18->gao = gao;
    v18->freq = v9->freq;
    v18->soundIntf = v9->sndInterface;
    v18->trackID = -1;
    v18->requestTimeStamp = 0.0;
    v18->volume = 1.0;
    v18->front = 0;
    v18->pan = 0;
    v18->minPan = 0.0;
    v18->modifierInstances = 0;
    v18->familyBitmask = 0;
    v18->timeRemainTillStart = 0.0;
    v18->timeFromDefault = 0.0;
    v19 = v9->muteFacorFlags;
    if ((v19 & 0x40) != 0)
    {
        v18->unkMuteSetupSeeInitVolTable = &SND_Mutes_B30600_fadergroups[7];
    }
    else if ((v19 & 0x80u) == 0)
    {
        if ((v19 & 0x100) != 0)
            v18->unkMuteSetupSeeInitVolTable = &SND_Mutes_B30600_fadergroups[9];
        else
            v18->unkMuteSetupSeeInitVolTable = &SND_Mutes_B30600_fadergroups[5];
    }
    else
    {
        v18->unkMuteSetupSeeInitVolTable = &SND_Mutes_B30600_fadergroups[8];
    }
    if ((v9->flags & 0x40) != 0)
    {
        SND_gst_Params.handles->soundElem = v9;
        v9->sndInterface->Duplicate(SND_gst_Params.handles, v9->sndSndBufElem, &v18->soundBufElemFromModel);
        if (!v18->soundBufElemFromModel)
        {
            SND_FreeInstance(a1);
            return -1;
        }
        v18->flags |= 0x40u;
        SND_SetInstVolume(v18);
    }
    else
    {
        v18->soundBufElemFromModel = v9->sndSndBufElem;
        v9->flags |= 0x40u;
        v18->soundIntf->SetCurrPos(v18->soundBufElemFromModel, 0);
        SND_SetInstVolume(v18);
        v18->soundIntf->SetFrequency(v18->soundBufElemFromModel, v18->freq);
        v18->soundIntf->SetPan(v18->soundBufElemFromModel, 0, 0);
    }
    if ((v9->flags & 0x4000) != 0)
        SND_SetTrack(a1, 1040u);
    return a1;
}

void SND_SModifierAddInInstance(int soundInstanceID, int sndModifierID, void* a3, int a4)
{
    tSoundInstanceElem* v4;
    tSndModifierElem* v5;

    if (soundInstanceID >= 0 && soundInstanceID < SND_gst_Params.soundInstanceNum)
    {
        v4 = &SND_gst_Params.soundInstances[soundInstanceID];
        if ((v4->flags & 0x400) != 0)
        {
            v5 = SND_pst_SModifierInsertOneSModifier(&v4->modifierInstances, sndModifierID, a3, a4);
            v5->sndModifierModelIntf->PlayApplyOnInstance(v5, v5->linkedSoundData, v5->linkedSoundDataSize, v4);
        }
    }
}

void SND_LinkToFamilly(int a1, int a2)
{
    tSoundInstanceElem* v2;

    if (a2 >= 0 && a2 < SND_gst_Params.soundInstanceNum)
    {
        v2 = &SND_gst_Params.soundInstances[a2];
        if ((v2->flags & 0x400) != 0)
            v2->familyBitmask |= a1;
    }
}

int SND_i_IsTrackPlaying(unsigned int a1)
{
    int v1;
    tTrackListElem* v2;
    int result;
    int v4;

    if (!SND_gst_Params.soundEnabled
        || (v1 = BAS_bsearch(a1, &SND_gst_TrackId), v1 == -1)
        || (v2 = &SND_gax_TrackPlayingList[v1], v2->trackID < 0)
        || (SND_gpst_Interface.win.GetStatus(v2->sndBufferElem, &v4),
            result = 1,
            (v4 & 1) == 0))
    {
        result = 0;
    }
    return result;
}

int SND_i_IsPlaying(int a1)
{
    tSoundInstanceElem* v1;
    tSoundInstanceElem* v2;
    int rr;
    int v3;
    int result;

    if (a1 < 0)
        goto LABEL_8;
    if (a1 >= SND_gst_Params.soundInstanceNum)
        goto LABEL_8;
    v1 = &SND_gst_Params.soundInstances[a1];
    v2 = v1;
    v3 = v1->flags;
    if ((v3 & 0x400) == 0)
        goto LABEL_8;
    if (v1->timeRemainTillStart != 0.0)
    {
        v1->flags = v3 | 0x200;
        return 1;
    }
    v1->soundIntf->GetStatus(v1->soundBufElemFromModel, &rr);
    if ((rr & 1) != 0) {
        result = ((v2->flags & 0x800000) != 0) + 1;
    }
    else {
    LABEL_8:
        result = 0;
    }
    return result;
}

void SND_Stop(int a1)
{
    tSoundInstanceElem* v1;
    BOOL v2;

    if (a1 >= 0 && a1 < SND_gst_Params.soundInstanceNum)
    {
        v1 = &SND_gst_Params.soundInstances[a1];
        if ((v1->flags & 0x400) != 0)
        {
            v1->soundIntf->Stop(v1->soundBufElemFromModel);
            v2 = (v1->flags & 0x20000) == 0;
            v1->flags &= ~0x808000u;
            if (!v2)
                SND_TrackDeleteInstanceFromList(a1);
            v1->soundIntf->SetCurrPos(v1->soundBufElemFromModel, 0);
            v1->timeRemainTillStart = 0.0;
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

void SND_AddMdFBankUser(struct sSoundMdfStruct* a1)
{
    tSoundMdfStruct** v1;
    unsigned int v2;
    tSoundMdfStruct** v3;

    if (SND_gst_Params.soundEnabled && a1)
    {
        v1 = sndMdfBankUser;
        v2 = 0;
        if (!SND_gst_BankMdFUserList)
            goto LABEL_8;
        while (sndMdfBankUser[v2])
        {
            if (++v2 >= SND_gst_BankMdFUserList)
                goto LABEL_8;
        }
        if (v2 >= SND_gst_BankMdFUserList)
        {
        LABEL_8:
            if (sndMdfBankUser)
                v3 = (tSoundMdfStruct**)MEM_p_Realloc(sndMdfBankUser, 4 * SND_gst_BankMdFUserList + 16 * 4);
            else
                v3 = (tSoundMdfStruct**)MEM_p_Alloc(4 * SND_gst_BankMdFUserList + 16 * 4);
            sndMdfBankUser = v3;
            memset(&v3[SND_gst_BankMdFUserList], 0, 16 * 4);
            v2 = SND_gst_BankMdFUserList;
            SND_gst_BankMdFUserList += 16;
            v1 = sndMdfBankUser;
        }
        v1[v2] = a1;
    }
}

int SND_i_FxSeti(int a1, int a2)
{
    int result;
    int v3;
    int v4;
    int v5;
    int v6;

    if (!SND_gst_Params.soundEnabled)
        return -1;
    result = 0;
    switch (a1)
    {
    case 8193:
        if (a2)
        {
            SND_gpst_Interface.win.FxDisable(&SND_gst_FxDefaultParam);
            SND_gst_FxDefaultParam.flags &= ~1u;
            SND_gpst_Interface.win.FxSetMode(&SND_gst_FxParam, SND_gst_FxParam.mode);
            SND_gpst_Interface.win.FxSetDelay(&SND_gst_FxParam, SND_gst_FxParam.delay);
            SND_gpst_Interface.win.FxSetFeedback(&SND_gst_FxParam, SND_gst_FxParam.feedback);
            SND_gpst_Interface.win.FxSetWetPan(&SND_gst_FxParam, SND_gst_FxParam.wetPan);
            v3 = SND_l_GetAttFromVol(SND_gst_FxParam.vol);
            SND_gpst_Interface.win.FxSetWetVolume(&SND_gst_FxParam, v3);
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
        v5 = a2;
        if (a2 == 7)
        {
            SND_gst_FxParam.mode = 10;
        }
        else
        {
            if (a2 == 8)
                v5 = 11;
            SND_gst_FxParam.mode = v5;
        }
        break;
    case 8195:
        SND_gst_FxParam.delay = a2;
        break;
    case 8196:
        SND_gst_FxParam.feedback = a2;
        break;
    case 8198:
        SND_gst_FxParam.wetPan = a2;
        break;
    case 8199:
        if (a2)
        {
            SND_gpst_Interface.win.FxDisable(&SND_gst_FxParam);
            SND_gst_FxParam.flags &= ~1u;
            SND_gpst_Interface.win.FxSetMode(&SND_gst_FxDefaultParam, SND_gst_FxDefaultParam.mode);
            SND_gpst_Interface.win.FxSetDelay(&SND_gst_FxDefaultParam, SND_gst_FxDefaultParam.delay);
            SND_gpst_Interface.win.FxSetFeedback(&SND_gst_FxDefaultParam, SND_gst_FxDefaultParam.feedback);
            SND_gpst_Interface.win.FxSetWetPan(&SND_gst_FxDefaultParam, SND_gst_FxDefaultParam.wetPan);
            v4 = SND_l_GetAttFromVol(SND_gst_FxDefaultParam.vol);
            SND_gpst_Interface.win.FxSetWetVolume(&SND_gst_FxDefaultParam, v4);
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
        v6 = a2;
        if (a2 == 7)
        {
            SND_gst_FxDefaultParam.mode = 10;
        }
        else
        {
            if (a2 == 8)
                v6 = 11;
            SND_gst_FxDefaultParam.mode = v6;
        }
        break;
    case 8201:
        SND_gst_FxDefaultParam.delay = a2;
        break;
    case 8202:
        SND_gst_FxDefaultParam.feedback = a2;
        break;
    case 8204:
        SND_gst_FxDefaultParam.wetPan = a2;
        break;
    default:
        return -1;
    }
    return result;
}

int SND_i_FxSetf(int a1, float a2)
{
    int result;

    if (!SND_gst_Params.soundEnabled)
        return -1;
    result = 0;
    if (a1 == 8197)
    {
        SND_gst_FxParam.vol = a2;
    }
    else
    {
        if (a1 != 8203)
            return -1;
        SND_gst_FxDefaultParam.vol = a2;
    }
    return result;
}

void SND_LoadingSoundRegisterInstanceUserCallback(int index, void (*a2)(void*), struct sLoadingSoundMdfStruct* a3) 
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
        SND_gst_LSGlobal.userCallBackFunction = a2;
        SND_gst_LSGlobal.userCallBackFunctionParam = (void*)a3;
        SND_gst_LSGlobal.soundInstanceIndex = index;
    }
}

int SND_l_GetLoadingInstance()
{
    return SND_gst_LSGlobal.soundInstanceIndex;
}

void SND_TrackDestroy(unsigned int a1)
{
    int v1;
    tTrackListElem* v2;
    unsigned int v3;
    int v4;
    tSoundInstanceElem* v5;
    unsigned int v6;
    unsigned int v7;
    int v8;

    v1 = BAS_bsearch(a1, &SND_gst_TrackId);
    v8 = v1;
    if (v1 != -1)
    {
        v2 = SND_gax_TrackPlayingList;
        v3 = v1;
        if (SND_gax_TrackPlayingList[v1].flags != 1)
        {
            v4 = 0;
            if (SND_gst_Params.soundInstanceNum > 0)
            {
                v5 = SND_gst_Params.soundInstances;
                do
                {
                    if ((v5->flags & 0x400) != 0 && v5->trackID == a1)
                    {
                        if (v2[v3].trackID == v4)
                        {
                            v7 = (unsigned int)v5->soundBufElemFromModel;
                            v5->soundIntf = v2[v3].soundIntf;
                            BAS_bdelete(v7, &SND_gst_TrackLookUpTable);
                        }
                        v6 = v5->flags & ~0x20010u;
                        v5->trackID = -1;
                        v5->flags = v6;
                        v2 = SND_gax_TrackPlayingList;
                    }
                    ++v4;
                    v5++;
                } while (v4 < SND_gst_Params.soundInstanceNum);
                v1 = v8;
            }
            if (v2[v3].stereoToMono_sub499C20)
            {
                snd_track_merge_inner_type_1_sub_499C20(v1);
                v2 = SND_gax_TrackPlayingList;
            }
            if (v2[v3].trackResourceNum)
            {
                SND_TrackForceFreeRessources_inner1_sub_499590(v1);
                v2 = SND_gax_TrackPlayingList;
            }
            v2[v3].sndBufferElem = 0;
            SND_gax_TrackPlayingList[v3].trackResource = 0;
            SND_gax_TrackPlayingList[v3].trackID = -1;
            SND_gax_TrackPlayingList[v3].soundIntf = 0;
            SND_gax_TrackPlayingList[v3].flags = 1;
            BAS_bdelete(a1, &SND_gst_TrackId);
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

void SND_StopAll(int a1)
{
    int i;

    if (SND_gst_Params.soundEnabled && a1 != 20)
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

int SND_MuteSetUp(int a1, float a2, float a3)
{
    int result;

    result = SND_gst_Params.soundEnabled;
    if (SND_gst_Params.soundEnabled)
    {
        result = a1;
        switch (a1)
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
            result = a1;
            SND_Mutes_B30600_fadergroups[result].muteSetUp1 = a3;
            SND_Mutes_B30600_fadergroups[result].muteSetUp2 = a2;
            break;
        default:
            return result;
        }
    }
    return result;
}

float SND_f_MuteGet(int a1, int a2)
{
    double result;

    if (!SND_gst_Params.soundEnabled)
        return 0.0;
    switch (a1)
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
        if (a2)
            result = SND_Mutes_B30600_fadergroups[a1].muteSetUp2;
        else
            result = SND_Mutes_B30600_fadergroups[a1].muteSetUp1;
        break;
    default:
        return 0.0;
    }
    return (float)result;
}

void SND_FadeUpdate()
{
    tMutesSetupStruct* v0;
    int v1;
    double v2;
    int v3;
    double v4;
    double v5;
    double v8;
    int v9;
    double v10;
    double v11;
    double v12;
    double v13;

    v0 = SND_Mutes_B30600_fadergroups;
    do
    {
        v1 = v0->flags;
        if (!v1)
            goto LABEL_17;
        if ((v1 & 1) == 0)
        {
            if ((v1 & 2) == 0)
                goto LABEL_17;
            v8 = TIM_gf_dt + v0->elapsedTime;
            v0->elapsedTime = (float)v8;
            v13 = v8 * v0->i6 + MATH_gd_magicDoubleForRound;
            v9 = *(uint32_t *)(&v13);
            if ((*(int32_t *)&v13) >= 0)
            {
                if ((*(int32_t *)&v13) > 255)
                {
                    v0->muteSetUp2 = v0->muteSetUp2ToSetWithThis;
                    v0->muteSetUp1 = 1.0;
                    goto LABEL_16;
                }
            }
            else
            {
                v9 = 0;
            }
            v10 = v0->i7;
            v0->muteSetUp1 = 1.0;
            v11 = (v10 - v0->muteSetUp2ToSetWithThis) * SND_gf_FadeOut[v9] + v0->muteSetUp2ToSetWithThis;
            v0->muteSetUp2 = (float)v11;
            if (v11 > v0->muteSetUp2ToSetWithThis)
                goto LABEL_17;
            goto LABEL_16;
        }
        v2 = TIM_gf_dt + v0->elapsedTime;
        v0->elapsedTime = (float)v2;
        v12 = v2 * v0->i6 + MATH_gd_magicDoubleForRound;
        v3 = (*(uint32_t *)&v12);
        if ((*(int32_t*)&v12) >= 0)
        {
            if ((*(int32_t*)&v12) <= 255)
                goto LABEL_6;
            v0->muteSetUp2 = v0->muteSetUp2ToSetWithThis;
            v0->muteEnabled = 0;
        LABEL_16:
            v0->flags = 0;
            goto LABEL_17;
        }
        v3 = 0;
    LABEL_6:
        v4 = v0->i7;
        v0->muteSetUp1 = 1.0;
        v5 = (v4 - v0->muteSetUp2ToSetWithThis) * SND_gf_FadeOut[v3] + v0->muteSetUp2ToSetWithThis;
        v0->muteSetUp2 = (float)v5;
        if (v5 >= v0->muteSetUp2ToSetWithThis)
            v0->flags = 0;
        if (v5 >= 1.0)
            v0->muteEnabled = 0;
    LABEL_17:
        ++v0;
    } while (v0 <= &SND_Mutes_B30600_fadergroups[11]);
}

void SND_FamillyUpdate()
{
    int v0;
    tSoundFamilyStruct* v1;
    int i;
    tSoundFamilyInnerStruct* v3;
    tSoundFamilyInnerStruct* v4;
    int j;
    tSoundFamilyInnerStruct* v6;
    tSoundFamilyInnerStruct* v7;
    tSoundFamilyInnerStruct* v8;
    tSoundFamilyInnerStruct* v9;
    BOOL v10;
    int v11;
    int v12;

    v0 = 0;
    v11 = 0;
    if (SND_gi_AndFamillyNb > 0)
    {
        v12 = 0;
        do
        {
            v1 = &SND_gast_AndFamillyList[v0];
            for (i = 0; i < v1->innerNum; ++i)
            {
                v3 = v1->unkIntArrPtr[i];
                if (v3->muteSetup2.muteEnabled == 10)
                    v3->muteSetup2.muteEnabled = 1;
                v4 = v1->unkIntArrPtr[i];
                if (v4->muteSetup2.muteEnabled == 20)
                    v4->muteSetup2.muteEnabled = 2;
            }
            for (j = 0; j < v1->innerNum; ++j)
            {
                v6 = v1->unkIntArrPtr[j];
                if (v6->muteSetup2.muteEnabled == 1)
                {
                    v6->muteSetup2.muteSetUp2 = TIM_gf_dt * v6->muteSetup1TimeMultiplier + v6->muteSetup2.muteSetUp2;
                    v7 = v1->unkIntArrPtr[j];
                    if (v7->muteSetup2.muteSetUp2 < 1.0)
                    {
                        v7->muteSetup2.muteEnabled = 10;
                    }
                    else
                    {
                        v7->muteSetup2.muteSetUp2 = 1.0;
                        v1->unkIntArrPtr[j]->muteSetup2.muteEnabled = 100;
                    }
                }
                v8 = v1->unkIntArrPtr[j];
                if (v8->muteSetup2.muteEnabled == 2)
                {
                    v8->muteSetup2.muteSetUp2 = TIM_gf_dt * v8->muteSetup2TimeMultiplier + v8->muteSetup2.muteSetUp2;
                    v9 = v1->unkIntArrPtr[j];
                    if (v9->muteSetup2.muteSetUp2 > 0.0)
                    {
                        v9->muteSetup2.muteEnabled = 20;
                    }
                    else
                    {
                        v9->muteSetup2.muteSetUp2 = 0.0;
                        v1->unkIntArrPtr[j]->muteSetup2.muteEnabled = 200;
                    }
                }
            }
            v0 = v12 + 1;
            v10 = ++v11 < SND_gi_AndFamillyNb;
            v12++;
        } while (v10);
    }
}

void SND_DeleteOldLoadingSound()
{
    int v0;
    int v1;
    tSndSoundElem* v2;
    int v3;
    unsigned int v4;

    if (SND_gst_LSGlobal.isNewLoading)
    {
        v0 = 0;
        v1 = 0;
        if (SND_gst_Params.allocatedSoundsMaxNum > 0)
        {
            v2 = SND_gst_Params.allocatedSounds;
            v3 = 0;
            do
            {
                v4 = v2[v3].flags;
                if ((v4 & 0x400) != 0 && (v4 & 8) != 0)
                {
                    ++v0;
                    if (v2[v3].usageCnt <= 0)
                    {
                        --v0;
                        v2[v3].usageCnt = 1;
                        SND_DeleteByIndex(v1);
                        v2 = SND_gst_Params.allocatedSounds;
                    }
                }
                ++v1;
                ++v3;
            } while (v1 < SND_gst_Params.allocatedSoundsMaxNum);
        }
        SND_gst_LSGlobal.isNewLoading = 0;
        SND_gst_LSGlobal.isSoundStillUsedElsewhere = v0 <= 0;
    }
}

void SND_MuteEnable(int a1, int a2)
{
    switch (a1)
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
        SND_Mutes_B30600_fadergroups[a1].muteEnabled = a2;
        break;
    default:
        return;
    }
}

void SND_LoadingSoundUpdate()
{
    int v0;
    tSndSoundElem* v1;
    unsigned int v2;
    double v3;
    double v4;
    float v5; 
    float v6;

    SND_DeleteOldLoadingSound();
    if (!SND_gst_LoadingEffect.mode)
    {
        if (!SND_gst_LSGlobal.isSoundStillUsedElsewhere)
            return;
        goto LABEL_18;
    }
    if (SND_gst_LoadingEffect.mode == 1)
    {
        if (SND_gst_LoadingEffect.i5 == 0.0)
            return;
        v4 = SND_gst_LoadingEffect.i5 - SND_gst_LoadingEffect.i4 * 0.02;
        SND_gst_LoadingEffect.i5 = (float)v4;
        if (v4 < 0.0)
            SND_gst_LoadingEffect.i5 = 0.0;
        v6 = SND_gst_LoadingEffect.i5;
    LABEL_12:
        v5 = (float)(1.0 - SND_gst_LoadingEffect.i5);
        SND_MuteSetUp(6, v5, v6);
        return;
    }
    if (SND_gst_LoadingEffect.mode != 2)
    {
        SND_gst_LoadingEffect.mode = 0;
        SND_MuteEnable(6, 0);
        return;
    }
    if (SND_gst_LoadingEffect.i5 < 1.0)
    {
        v3 = SND_gst_LoadingEffect.i4 * 0.02 + SND_gst_LoadingEffect.i5;
        SND_gst_LoadingEffect.i5 = (float)v3;
        if (v3 > 1.0)
            SND_gst_LoadingEffect.i5 = 1.0;
        v6 = SND_gst_LoadingEffect.i5;
        goto LABEL_12;
    }
    SND_gst_LoadingEffect.mode = 0;
    SND_MuteEnable(6, 0);
    if (!SND_gst_LSGlobal.isSoundStillUsedElsewhere)
    {
        SND_Stop(SND_gst_LSGlobal.soundInstanceIndex);
        return;
    }
LABEL_18:
    SND_ReinitLoadingInstance();
    v0 = SND_gst_LSGlobal.soundElemIndex;
    if (SND_gst_LSGlobal.soundElemIndex >= 0)
    {
        v1 = &SND_gst_Params.allocatedSounds[SND_gst_LSGlobal.soundElemIndex];
        v2 = v1->flags;
        if ((v2 & 0x400) != 0 && (v2 & 8) != 0)
        {
            v1->usageCnt = 1;
            SND_gst_LSGlobal.soundElemIndex = -1;
            SND_DeleteByIndex(v0);
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

void SND_SModifierUpdateInstance(tSoundInstanceElem* a1)
{
    tSndModifierElem* i;
    void(*v2)(tSndModifierElem*, tSoundInstanceElem*, int);

    for (i = a1->modifierInstances; i; i = i->nextModifier)
    {
        v2 = i->sndModifierModelIntf->Update;
        if (v2)
            v2(i, a1, 4);
    }
}

void SND_Update(Mtx4x4* mtx)
{
    double v1;
    float v2;
    int v3;
    int v4;
    tSoundInstanceElem* sie;
    int v6;
    double v7;
    tGameObjectMain* v8;
    int v9;
    int v10;
    Vec3 distance;

    if (!SND_gst_Params.soundEnabled)
        return;
    SND_SetMicroPos(mtx);
    if (!SND_gst_Params.sndMatrixRef)
        goto LABEL_6;
    if (!i_init_521)
    {
        t_last_mic_pos_522.x = SND_gst_Params.sndMatrixRef->Pos.x;
        t_last_mic_pos_522.y = SND_gst_Params.sndMatrixRef->Pos.y;
        v2 = SND_gst_Params.sndMatrixRef->Pos.z;
        i_init_521 = 1;
        t_last_mic_pos_522.z = v2;
    LABEL_6:
        distance.x = 0.0;
        distance.y = 0.0;
        distance.z = 0.0;
        goto LABEL_7;
    }
    distance.x = SND_gst_Params.sndMatrixRef->Pos.x - t_last_mic_pos_522.x;
    distance.y = SND_gst_Params.sndMatrixRef->Pos.y - t_last_mic_pos_522.y;
    distance.z = SND_gst_Params.sndMatrixRef->Pos.z - t_last_mic_pos_522.z;
    v1 = 1.0 / TIM_gf_dt;
    t_last_mic_pos_522 = SND_gst_Params.sndMatrixRef->Pos;
    distance.x = (float)(distance.x * v1);
    distance.y = (float)(distance.y * v1);
    distance.z = (float)(v1 * distance.z);
LABEL_7:
    SND_LoadingSoundUpdate();
    SND_FamillyUpdate();
    v3 = 0;
    if (SND_gst_Params.soundInstanceNum > 0)
    {
        v4 = 0;
        do
        {
            sie = &SND_gst_Params.soundInstances[v4];
            v6 = SND_gst_Params.soundInstances[v4].flags;
            if ((v6 & 0x400) != 0)
            {
                if (sie->timeRemainTillStart != 0.0)
                {
                    v7 = sie->timeRemainTillStart - TIM_gf_dt;
                    sie->timeRemainTillStart = (float)v7;
                    if (v7 <= 0.0)
                    {
                        sie->timeRemainTillStart = 0.0;
                        SND_PlayRq(0, sie, v6 & 0x20000 | ((v6 & 4 | 0x80000003) >> 2), -1);
                    }
                LABEL_13:
                    v8 = sie->gao;
                    if (!v8 || (v8->flags2 & 0x400000) == 0)
                    {
                        if (sie->modifierInstances)
                            SND_SModifierUpdateInstance(sie);
                        SND_SetInstVolume(sie);
                        v9 = sie->flags;
                        if ((v9 & 0x2000) == 0 && (v9 & 0x1000) != 0)
                            sie->soundIntf->ComputeDoppler(&distance, &SND_gst_Params.sndMatrixRef->Pos, sie);
                    }
                    goto LABEL_20;
                }
                if ((v6 & 0x80u) == 0)
                    goto LABEL_13;
                if ((v6 & 0x200) == 0)
                    goto LABEL_13;
                if ((sie->muteFactorFlags & 4) != 0)
                    goto LABEL_13;
                sie->soundIntf->GetStatus(sie->soundBufElemFromModel, &v10);
                if ((v10 & 1) != 0)
                    goto LABEL_13;
                SND_FreeInstance(v3);
            }
        LABEL_20:
            ++v3;
            ++v4;
        } while (v3 < SND_gst_Params.soundInstanceNum);
    }
    SND_FadeUpdate();
    SND_gpst_Interface.win.SNDNull(&distance);
}

float SND_f_SphericComputing(Vec3* a1, struct sSoundInstanceElem* a2, int* a3)
{
    Vec3* v3;
    double v4;
    double v5;
    double v6;
    double v7;
    double v9;
    float v10;
    double v11;
    float v12;
    float v13;

    *a3 = 0;
    v3 = a2->sndPositionSetInSetPosPtr;
    v4 = a1->y - v3->y;
    v5 = a1->z - v3->z;
    v6 = a1->x - v3->x;
    v7 = sqrt(v5 * v5 + v6 * v6 + v4 * v4);
    if (v7 >= a2->farArr[0])
    {
        if ((a2->muteFactorFlags & 5) == 1
            && v7 > a2->dtFar + a2->dtFar + a2->farArr[0])
        {
            *a3 = 1;
        }
        if ((a2->muteFactorFlags & 6) == 6 && v7 < a2->dtFar + a2->farArr[0])
            *a3 |= 2u;
    }
    v13 = (float)(SND_gst_Params.f_rolloff * v7);
    if (v13 <= (double)a2->nearArr[0])
        return 1.0;
    if (v13 <= (double)a2->centerArr[0])
    {
        v9 = a2->middleCoefficient;
        v10 = a2->nearArr[0];
        v11 = a2->centerArr[0];
        v12 = 1.0;
        return (float)(v9 + (1.0 / (v13 * v13) - 1.0 / (v11 * v11)) * (v10 * v10 * (v11 * v11)) / (v11 * v11 - v10 * v10) * (v12 - v9));
    }
    if (v13 < (double)a2->farArr[0])
    {
        v9 = a2->farCoefficient;
        v11 = a2->farArr[0];
        v10 = a2->centerArr[0];
        v12 = a2->middleCoefficient;
        return (float)(v9 + (1.0 / (v13 * v13) - 1.0 / (v11 * v11)) * (v10 * v10 * (v11 * v11)) / (v11 * v11 - v10 * v10) * (v12 - v9));
    }
    return a2->farCoefficient;
}

void SND_SModifierNotifyStopInstance(tSoundInstanceElem* a1)
{
    tSndModifierElem* i;
    void( *v2)(tSndModifierElem*, tSoundInstanceElem*);

    for (i = a1->modifierInstances; i; i = i->nextModifier)
    {
        v2 = i->sndModifierModelIntf->NotifyStop;
        if (v2)
            v2(i, a1);
    }
}

struct sSndModifierElem* SND_pst_SModifierGet(struct sSndModifierElem* a1, int type)
{
    tSndModifierElem* result;

    if (type >= 7 || type < 0)
        return NULL;
    for (result = a1; result; result = result->nextModifier)
    {
        if (!result->sndModifierModelIntf)
            break;
        if (result->sndModifierModelIntf->Type == type)
            return result;
    }
    return NULL;
}

void SND_StopRq(int a1)
{
    tSoundInstanceElem* v1;

    if (a1 >= 0 && a1 < SND_gst_Params.soundInstanceNum)
    {
        v1 = &SND_gst_Params.soundInstances[a1];
        if ((v1->flags & 0x400) != 0 && (v1->muteFactorFlags & 0x1000) == 0)
        {
            SND_SModifierNotifyStopInstance(&SND_gst_Params.soundInstances[a1]);
            if (SND_pst_SModifierGet(v1->modifierInstances, 4))
                v1->muteFactorFlags |= 0x1000u;
            else
                SND_Stop(a1);
        }
    }
}

void SND_AutoStop(struct sSoundInstanceElem* a1)
{
    tSoundInstanceElem* v1;
    int v2;
    int a1a;

    v1 = a1;
    if ((a1->muteFactorFlags & 4) == 0)
    {
        a1->soundIntf->GetStatus(a1->soundBufElemFromModel, &a1a);
        if ((a1a & 1) != 0)
        {
            a1a = (v1->flags & 0x20000);
            SND_StopRq((((char*)v1 - (char*)SND_gst_Params.soundInstances) / sizeof(tSoundInstanceElem)));
            v2 = v1->muteFactorFlags | 4;
            v1->flags |= a1a; // fixed: was (unsigned int)a1
            v1->muteFactorFlags = v2;
        }
    }
}

float SND_f_AxisComputing(Vec3* a1, struct sSoundInstanceElem* a2, int* a3, int a4)
{
    Vec3* v4;
    double v5;
    double v6;
    double v7;
    Vec3* v9;
    double v10;
    double v11;
    double v12;
    float v13;
    float v14;
    float v15;

    *a3 = 0;
    v4 = a2->sndPositionSetInSetPosPtr;
    v5 = a1->x - v4->x;
    v6 = a1->y - v4->y;
    v7 = a1->z - v4->z;
    if (a4)
    {
        if (a4 == 1)
        {
            v9 = &a2->sndPosMtxSetInSetPos->v2;
        }
        else
        {
            if (a4 != 2)
                return 0.0;
            v9 = &a2->sndPosMtxSetInSetPos->v3;
        }
    }
    else
    {
        v9 = &a2->sndPosMtxSetInSetPos->v1;
    }
    v10 = fabs(v9->z * v7 + v9->y * v6 + v5 * v9->x);
    if ((a2->muteFactorFlags & 5) == 1
        && v10 > a2->dtFar + a2->dtFar + a2->farArr[a4])
    {
        *a3 = 1;
    }
    if ((a2->muteFactorFlags & 6) == 6 && v10 < a2->farArr[a4] + a2->dtFar)
        *a3 |= 2u;
    v15 = (float)(SND_gst_Params.f_rolloff * v10);
    if (v15 <= (double)a2->nearArr[a4])
        return 1.0;
    if (v15 <= (double)a2->centerArr[a4])
    {
        v13 = 1.0;
        v14 = a2->nearArr[a4];
        v11 = a2->middleCoefficient;
        v12 = a2->centerArr[a4];
        return (float)(v11 + (1.0 / (v15 * v15) - 1.0 / (v12 * v12)) * (v14 * v14 * (v12 * v12)) / (v12 * v12 - v14 * v14) * (v13 - v11));
    }
    if (v15 < (double)a2->farArr[a4])
    {
        v11 = a2->farCoefficient;
        v12 = a2->farArr[a4];
        v13 = a2->middleCoefficient;
        v14 = a2->centerArr[a4];
        return (float)(v11 + (1.0 / (v15 * v15) - 1.0 / (v12 * v12)) * (v14 * v14 * (v12 * v12)) / (v12 * v12 - v14 * v14) * (v13 - v11));
    }
    return a2->farCoefficient;
}

void SND_AutoPlay(struct sSoundInstanceElem* a1)
{
    int v1;

    v1 = a1->muteFactorFlags;
    if ((v1 & 4) != 0)
    {
        a1->muteFactorFlags = v1 & ~4u;
        SND_PlayRq(0, a1, a1->flags & 0x20000 | ((unsigned int)a1->flags >> 2) & 1, -1);
    }
}

float SND_f_GetGlobalVol()
{
    int v1;

    if (!SND_gst_Params.soundEnabled)
        return 0.0;
    v1 = SND_gpst_Interface.win.GetGlobalVol();
    return SND_f_GetVolFromAtt(v1);
}

int SND_TrackInitModule()
{
    tTrackResourceElem *v0;

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
    v0 = SND_gast_TrackResources;
    do
    {
        SND_gpst_Interface.win.CreateTrack(&(v0->stereoSoundBuffer), &(v0->monoSoundBuffer));
        v0->isBusy = 0;
        v0++;
    } while (v0 <= &SND_gast_TrackResources[3]);
    return SND_i_TrackNew();
}

int SND_i_ChangeRenderMode(int a1)
{
    int result;

    if (a1 == -1)
        return SND_gst_Params.soundRenderMode;
    if ((a1 & SND_gst_Params.soundRenderModeMask) != a1)
        return -1;
    result = SND_gst_Params.soundRenderMode;
    if (a1 == SND_gst_Params.soundRenderMode)
        return 0;
    switch (a1)
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
        result = SND_gpst_Interface.win.ChangeRenderMode(a1);
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

int ConsoleSpecificOption(int a1)
{
    int result;
    int v2;
    int v3;

    switch (a1)
    {
    case 1:
        v3 = SND_i_ChangeRenderMode(-1);
        if (v3 == -1)
            v2 = -1;
        else
            v2 = v3 != 1;
        result = v2;
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
    tSndWavHdrStruct v0;

    memset(&SND_gst_LSGlobal, 0, sizeof(SND_gst_LSGlobal));
    SND_gst_LSGlobal.soundInstanceIndex = -1;
    SND_gst_LSGlobal.soundElemIndex = -1;
    v0.wfex.wFormatTag = 2;
    v0.wfex.nBlockAlign = 2;
    SND_gst_LoadingEffect.mode = 0;
    SND_gst_LoadingEffect.f_fadeOutTime = 0.2f;
    SND_gst_LoadingEffect.f_fadeInTime = 1.0;
    SND_gst_LoadingEffect.i6 = 1.0;
    SND_gst_LoadingEffect.i4 = 1.0;
    SND_gst_LoadingEffect.i5 = 1.0;
    v0.wfex.nChannels = 1;
    v0.wfex.nSamplesPerSec = 48000;
    v0.wfex.nAvgBytesPerSec = 96000;
    v0.wfex.wBitsPerSample = 16;
    v0.wfex.cbSize = 0;
    v0.fullSoundSize = 153600;
    v0.dataStartPos = 0;
    SND_gi_RasterCurrentFileId = 8;
    SND_gst_LSGlobal.soundElemElem = SND_gpst_Interface.win.Create(SND_gst_Params.handles, &v0, 0);
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

void SND_SetGlobalPan(int a1)
{
    SND_gpst_Interface.win.SetGlobalPan(a1);
}

int SND_l_GetGlobalPan()
{
    return SND_gpst_Interface.win.GetGlobalPan();
}

void SND_SetGlobalVol(float a1)
{
    int v1;

    if (SND_gst_Params.soundEnabled)
    {
        v1 = SND_l_GetAttFromVol(a1);
        SND_gpst_Interface.win.SetGlobalVol(v1);
    }
}

int SND_l_GetPan(int a1)
{
    int v3;

    if (a1 < 0 || a1 >= SND_gst_Params.soundInstanceNum || (SND_gst_Params.soundInstances[a1].flags & 0x400) == 0)
        return 0;
    SND_gst_Params.soundInstances[a1].soundIntf->GetPan(SND_gst_Params.soundInstances[a1].soundBufElemFromModel, &a1, &v3);
    return a1;
}

int SND_l_GetFront(int a1)
{
    int v3;

    if (a1 < 0 || a1 >= SND_gst_Params.soundInstanceNum || (SND_gst_Params.soundInstances[a1].flags & 0x400) == 0)
        return 0;
    SND_gst_Params.soundInstances[a1].soundIntf->GetPan(SND_gst_Params.soundInstances[a1].soundBufElemFromModel, &v3, &a1);
    return a1;
}

int SND_l_GetFreq(int a1)
{
    tSoundInstanceElem* v1;
    int result;

    if (a1 >= 0 && a1 < SND_gst_Params.soundInstanceNum && (v1 = &SND_gst_Params.soundInstances[a1], (v1->flags & 0x400) != 0))
    {
        result = v1->freq;
    }
    else
    {
        result = 0;
    }
    return result;
}

float SND_f_GetVol(int a1)
{
    tSoundInstanceElem* v1;
    float result;

    if (a1 >= 0
        && a1 < SND_gst_Params.soundInstanceNum
        && (v1 = &SND_gst_Params.soundInstances[a1], (v1->flags & 0x400) != 0))
    {
        result = v1->volume;
    }
    else
    {
        result = 0.0;
    }
    return result;
}

void SND_SetDoppler(int a1, int a2)
{
    tSoundInstanceElem* v2;
    int v3;

    if (a1 >= 0 && a1 < SND_gst_Params.soundInstanceNum)
    {
        v2 = &SND_gst_Params.soundInstances[a1];
        v3 = v2->flags;
        if ((v3 & 0x400) != 0)
        {
            if (a2)
                v2->flags = v3 | 0x1000;
            else
                v2->flags = v3 & ~0x1000;
        }
    }
}

int SND_i_MuteIsEnable(int a1)
{
    int result;

    if (!SND_gst_Params.soundEnabled)
        return 0;
    switch (a1)
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
        result = SND_Mutes_B30600_fadergroups[a1].muteEnabled;
        break;
    default:
        return 0;
    }
    return result;
}

float SND_f_CurrentPosRatio(int a1)
{
    tSoundInstanceElem* v1;
    int v3;

    if (a1 < 0)
        return 0.0;
    if (a1 >= SND_gst_Params.soundInstanceNum)
        return 0.0;
    v1 = &SND_gst_Params.soundInstances[a1];
    if ((v1->flags & 0x400) == 0)
        return 0.0;
    v1->soundIntf->GetCurrPos(v1->soundBufElemFromModel, &a1, &v3);
    return (float)((double)a1 / (double)SND_gst_Params.allocatedSounds[v1->allocatedSoundsIdx].fullsoundLen);
}

int SND_i_GetTrack(int a1)
{
    tSoundInstanceElem* v1;
    int result;

    if (a1 >= 0
        && a1 < SND_gst_Params.soundInstanceNum
        && (v1 = &SND_gst_Params.soundInstances[a1], (v1->flags & 0x400) != 0))
    {
        result = v1->trackID;
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
        if (SND_gst_LoadingEffect.mode != 1)
        {
        LABEL_5:
            SND_gst_LoadingEffect.mode = 0;
            SND_MuteEnable(6, 0);
            return;
        }
        if (SND_gst_LSGlobal.soundInstanceIndex < 0)
        {
            SND_gst_LoadingEffect.i4 = 1.0;
            goto LABEL_5;
        }
        SND_gst_LoadingEffect.i4 = (float)((1.0 - SND_gst_LoadingEffect.i5) / SND_gst_LoadingEffect.f_fadeInTime);
        SND_MuteSetUp(6, 1.0, SND_gst_LoadingEffect.i5);
        SND_MuteEnable(6, 1);
        SND_gst_LoadingEffect.mode = 2;
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

int SND_i_Seti(int a1, int a2)
{
    int result;

    if (!SND_gst_Params.soundEnabled)
        return -1;
    switch (a1)
    {
    case -2:
    case -1:
        goto LABEL_19;
    case 7:
        if (a2)
        {
            if ((SND_gst_Params.flags & 0x1000) == 0 && SND_gst_Params.sndMatrixRef)
            {
                SND_gst_Params.flags |= 0x1000;
                MATH_CopyMatrix(&SND_gst_Params.sndMatrix2, SND_gst_Params.sndMatrixRef);
                SND_gst_Params.sndMatrixRefBackup = SND_gst_Params.sndMatrixRef;
                SND_gst_Params.sndMatrixRef = &SND_gst_Params.sndMatrix2;
                return 0;
            }
            goto LABEL_19;
        }
        if ((SND_gst_Params.flags & 0x1000) == 0)
            goto LABEL_19;
        SND_gst_Params.flags &= ~0x1000u;
        SND_gst_Params.sndMatrixRef = SND_gst_Params.sndMatrixRefBackup;
        return 0;
    case 8:
        if (a2)
        {
            if ((SND_gst_Params.flags & 0x2000) == 0 && SND_gst_Params.microPosMtxPtr)
            {
                SND_gst_Params.flags |= 0x2000;
                MATH_CopyMatrix(&SND_gst_Params.microPosMtx, SND_gst_Params.microPosMtxPtr);
                SND_gst_Params.microPosMtxPtrBackup = SND_gst_Params.microPosMtxPtr;
                SND_gst_Params.microPosMtxPtr = &SND_gst_Params.microPosMtx;
                return 0;
            }
        }
        else if ((SND_gst_Params.flags & 0x2000) != 0)
        {
            SND_gst_Params.flags &= ~0x2000;
            SND_gst_Params.microPosMtxPtr = SND_gst_Params.microPosMtxPtrBackup;
            return 0;
        }
    LABEL_19:
        result = 0;
        break;
    case 9:
        if (!a2)
            goto LABEL_19;
        SND_LoadingSoundNotifyBeginLoading();
        return 0;
    case 10:
        if (a2)
            SND_LoadingSoundNotifyEndLoading();
        goto LABEL_19;
    case 14:
        return SND_i_ChangeRenderMode(a2);
    default:
        return -1;
    }
    return result;
}

int SND_i_FxGeti(int a1)
{
    int result;

    if (!SND_gst_Params.soundEnabled)
        return -1;
    switch (a1)
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

int SND_i_Geti(int a1)
{
    int result;

    if (!SND_gst_Params.soundEnabled)
        return -1;
    switch (a1)
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

float SND_f_FxGetf(int a1)
{
    if (!SND_gst_Params.soundEnabled)
        return -1.0;
    if (a1 == 8197)
        return SND_gst_FxParam.vol;
    if (a1 == 8203)
        return SND_gst_FxDefaultParam.vol;
    return -1.0;
}

int SND_l_GetSoundFromInstance(int a1)
{
    tSoundInstanceElem* v1;
    int result;

    if (a1 >= 0
        && a1 < SND_gst_Params.soundInstanceNum
        && (v1 = &SND_gst_Params.soundInstances[a1], (v1->flags & 0x400) != 0))
    {
        result = v1->allocatedSoundsIdx;
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

float SND_f_Getf(int a1)
{
    double result;

    if (!SND_gst_Params.soundEnabled)
        return -1.0;
    switch (a1)
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

void SND_ResetFlags(struct sGameObjectMain* a1, int a2, int a3)
{
    tSoundInstanceElem* v3;
    int v4;

    if (a2 >= 0 && a2 < SND_gst_Params.soundInstanceNum)
    {
        v3 = &SND_gst_Params.soundInstances[a2];
        v4 = v3->flags;
        if ((v4 & 0x400) != 0)
        {
            if ((a3 & 0x40000000) != 0)
                v3->muteFactorFlags &= ~(a3 & 0xFCB);
            else
                v3->flags = v4 & ~a3;
        }
    }
}

void SND_PauseMdF(struct sSoundMdfStruct* a1)
{
    int v1;

    if (SND_gst_Params.soundEnabled && a1->sndInstanceID >= 0)
    {
        v1 = a1->flags;
        if ((v1 & 0x84) != 0)
        {
            if ((a1->flags & 0x84) != 4)
                return;
            SND_Pause(a1->sndInstanceID);
            v1 = a1->flags;
        }
        a1->flags = v1 | 0x80;
    }
}

void SND_StopMdF(struct sSoundMdfStruct* a1)
{
    int v1;

    if (SND_gst_Params.soundEnabled)
    {
        v1 = a1->sndInstanceID;
        if (v1 >= 0)
        {
            if ((a1->flags & 0x84) == 4)
            {
                a1->flags &= ~4u;
                SND_StopRq(v1);
                return;
            }
            if ((a1->flags & 0x84) != 0x80)
            {
                if ((a1->flags & 0x84) != 0x84)
                    return;
                SND_Stop(a1->sndInstanceID);
            }
            a1->flags &= ~0x84u;
        }
    }
}

void SND_PlayFamilly(int a1, int a2)
{
    int v2;
    int v3;
    int* v4;
    int v5;

    if (SND_gst_Params.soundEnabled)
    {
        v2 = 0;
        if (SND_gst_Params.soundInstanceNum > 0)
        {
            v3 = 0;
            do
            {
                v4 = &SND_gst_Params.soundInstances[v3].flags;
                if ((*v4 & 0x400) != 0)
                {
                    v5 = SND_gst_Params.soundInstances[v3].familyBitmask;
                    if ((a1 & v5) == a1 && (v5 & a2) != 0 && !SND_i_IsPlaying(v2))
                    {
                        if ((*v4 & 4) != 0)
                            SND_PlaySoundLooping(v2);
                        else
                            SND_PlaySound(v2);
                    }
                }
                ++v2;
                ++v3;
            } while (v2 < SND_gst_Params.soundInstanceNum);
        }
    }
}

void SND_StopFamilly(int a1, int a2)
{
    int v2;
    tSoundInstanceElem* v3;
    int v4;

    if (SND_gst_Params.soundEnabled)
    {
        v2 = 0;
        if (SND_gst_Params.soundInstanceNum > 0)
        {
            v3 = SND_gst_Params.soundInstances;
            v4 = 0;
            do
            {
                if ((v3[v4].flags & 0x400) != 0 && (a1 & v3[v4].familyBitmask) == a1 && (v3[v4].familyBitmask & a2) != 0)
                {
                    SND_Stop(v2);
                    v3 = SND_gst_Params.soundInstances;
                }
                ++v2;
                ++v4;
            } while (v2 < SND_gst_Params.soundInstanceNum);
        }
    }
}

tSoundFamilyInnerStruct* probably_sndFamillyAlloc_and_Init_sub_49A4E0()
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

tSoundFamilyStruct* SND_pst_GetOrArray(int a1, int a2)
{
    int v2;
    int v3;
    int v4;
    tSoundFamilyStruct* v5;
    tSoundFamilyStruct* v6;
    tSoundFamilyStruct* v7;
    size_t v8;
    tSoundFamilyStruct* v9;
    int v10;
    int v11;
    tSoundFamilyInnerStruct* v12;
    int* v13;
    int v14;
    int v15;
    tSoundFamilyInnerStruct* v16;
    tSoundFamilyInnerStruct** v17;
    int v18;
    tSoundFamilyInnerStruct* v19;
    tSoundFamilyInnerStruct* v21;
    int v22;
    size_t v23;
    tSoundFamilyInnerStruct** v24;
    int v25;
    tSoundFamilyInnerStruct* v26;
    int v27;
    int* v28;

    v2 = SND_gi_AndFamillyNb;
    v3 = 0;
    v4 = 0;
    v5 = SND_gast_AndFamillyList;
    if (SND_gi_AndFamillyNb <= 0)
    {
        v7 = (tSoundFamilyStruct*)a2; // TODO FIXME: ez mi, jó ez így? kidebuggolni
    }
    else
    {
        v6 = SND_gast_AndFamillyList;
        do
        {
            v7 = v6;
            if (a1 == v6->i1)
                break;
            ++v4;
            ++v6;
        } while (v4 < SND_gi_AndFamillyNb);
    }
    if (v4 == SND_gi_AndFamillyNb)
    {
        if (SND_gi_AndFamillyNb >= SND_gi_AndFamillyListSize)
        {
            v8 = 16 * (9 * SND_gi_AndFamillyListSize + sizeof(tSoundFamilyStruct));
            if (SND_gi_AndFamillyListSize)
                v9 = (tSoundFamilyStruct*)MEM_p_Realloc(SND_gast_AndFamillyList, v8);
            else
                v9 = (tSoundFamilyStruct*)MEM_p_Alloc(v8);
            SND_gast_AndFamillyList = v9;
            memset(&v9[SND_gi_AndFamillyNb], 0, v8);
            v5 = SND_gast_AndFamillyList;
            SND_gi_AndFamillyListSize += 16;
            v2 = SND_gi_AndFamillyNb;
            v3 = 0;
        }
        v7 = &v5[v2];
        SND_gi_AndFamillyNb = v2 + 1;
        v7->i1 = a1;
        v7->innerNum = 0;
        v7->i2 = 0;
        v7->i4[0] = a2;
        v7->unkIntArrPtr = 0;
    }
    v10 = a2;
    if ((a2 & v7->i2) == 0)
    {
        v21 = probably_sndFamillyAlloc_and_Init_sub_49A4E0();
        v22 = v7->innerNum;
        v23 = 4 * v22 + 4;
        if (v22)
            v24 = (tSoundFamilyInnerStruct**)MEM_p_Realloc(v7->unkIntArrPtr, v23);
        else
            v24 = (tSoundFamilyInnerStruct**)MEM_p_Alloc(v23);
        v25 = v7->innerNum;
        v7->unkIntArrPtr = v24;
        memset(&v7->unkIntArrPtr[v25], 0, v23);
        v7->unkIntArrPtr[v7->innerNum] = v21;
        v26 = v7->unkIntArrPtr[v7->innerNum];
        ++v26->i1;
        v10 = a2;
        v7->i4[v7->innerNum++] = a2;
        goto LABEL_36;
    }
    v11 = a2;
    v12 = 0;
    v27 = a2;
    if (v7->innerNum > 0)
    {
        v13 = v7->i4;
        v28 = v7->i4;
        while (1)
        {
            v14 = *v13;
            if ((*v13 & v11) != 0)
            {
                v15 = v14 & ~v11;
                if (v15)
                {
                    if (v12)
                    {
                        v16 = v7->unkIntArrPtr[v3];
                        if (v16 != v12)
                        {
                            if (v16->i1 <= 1)
                            {
                                unknown_snd_familylist_check_And_free_sub_49A520(v7->unkIntArrPtr[v3]);
                                v13 = v28;
                                ++v12->i1;
                                v7->unkIntArrPtr[v3] = v12;
                                *v28 = v15;
                            }
                            else
                            {
                                --v16->i1;
                                v7->unkIntArrPtr[v3] = v12;
                                *v13 = v15;
                            }
                        }
                    }
                    else
                    {
                        v12 = v7->unkIntArrPtr[v3];
                    }
                }
                else
                {
                    v27 &= ~v14;
                    if (!v27)
                    {
                        v7->i2 |= a2;
                        return v7;
                    }
                }
            }
            v11 = v27;
            ++v3;
            v28 = ++v13;
            if (v3 >= v7->innerNum)
            {
                v10 = a2;
                break;
            }
        }
    }
    if (!v11)
    {
    LABEL_36:
        v7->i2 |= v10;
        return v7;
    }
    if (!v12)
        v12 = probably_sndFamillyAlloc_and_Init_sub_49A4E0();
    v7->unkIntArrPtr = (tSoundFamilyInnerStruct**)MEM_p_Realloc(v7->unkIntArrPtr, 4 * v7->innerNum + 4);
    v7->i4[v7->innerNum] = v11;
    v17 = v7->unkIntArrPtr;
    v18 = v7->innerNum + 1;
    v7->innerNum = v18;
    v17[v18] = v12;
    v19 = v7->unkIntArrPtr[v7->innerNum];
    ++v19->i1;
    v7->i2 |= a2;
    return v7;
}

void SND_MuteOnFamilly(int a2, int x)
{
    tSoundFamilyStruct* v3;
    tMutesSetupStruct* v4 = NULL; // TODO FIXME: ez biztos jó? a játék nem inicializálja ezt
    int v5;
    int* v6;
    int v7;
    tSoundInstanceElem* v8;
    int v9;

    if (SND_gst_Params.soundEnabled)
    {
        v3 = SND_pst_GetOrArray(a2, x);
        v5 = 0;
        if (v3->innerNum > 0)
        {
            v6 = v3->i4;
            do
            {
                if ((x & *v6) != 0)
                {
                    v4 = &v3->unkIntArrPtr[v5]->muteSetup1;
                    v4->muteEnabled = 1;
                }
                ++v5;
                ++v6;
            } while (v5 < v3->innerNum);
        }
        v7 = 0;
        if (SND_gst_Params.soundInstanceNum > 0)
        {
            v8 = SND_gst_Params.soundInstances;
            v9 = 0;
            do
            {
                if ((v8[v9].flags & 0x400) != 0 && (a2 & v8[v9].familyBitmask) == a2 && (v8[v9].familyBitmask & x) != 0)
                {
                    v8[v9].unkMuteSetupSeeInitVolTable = v4;
                    v8 = SND_gst_Params.soundInstances;
                }
                ++v7;
                ++v9;
            } while (v7 < SND_gst_Params.soundInstanceNum);
        }
    }
}

void SND_MuteOffFamilly(int a1, int a2)
{
    tSoundFamilyStruct* v3;
    tMutesSetupStruct* v4 = NULL; // TODO FIXME: ez biztos jó? a játék nem inicializálja ezt
    int v5;
    int* v6; 
    int v7; 
    tSoundInstanceElem* v8;
    int v9;

    if (SND_gst_Params.soundEnabled)
    {
        v3 = SND_pst_GetOrArray(a1, a2);
        v5 = 0;
        if (v3->innerNum > 0)
        {
            v6 = v3->i4;
            do
            {
                if ((a2 & *v6) != 0)
                {
                    v4 = &v3->unkIntArrPtr[v5]->muteSetup1;
                    v4->muteEnabled = 0;
                }
                ++v5;
                ++v6;
            } while (v5 < v3->innerNum);
        }
        v7 = 0;
        if (SND_gst_Params.soundInstanceNum > 0)
        {
            v8 = SND_gst_Params.soundInstances;
            v9 = 0;
            do
            {
                if ((v8[v9].flags & 0x400) != 0 && (a1 & v8[v9].familyBitmask) == a1 && (v8[v9].familyBitmask & a2) != 0)
                {
                    v8[v9].unkMuteSetupSeeInitVolTable = v4;
                    v8 = SND_gst_Params.soundInstances;
                }
                ++v7;
                ++v9;
            } while (v7 < SND_gst_Params.soundInstanceNum);
        }
    }
}

void SND_SetfFamilly(int a1, int a2, int a3, float a4)
{
    long double v4;
    long double v5;
    tSoundFamilyStruct* v6;
    int v7;
    int* v8;

    if (SND_gst_Params.soundEnabled)
    {
        switch (a3)
        {
        case 3:
        case 4:
            v4 = fabs(a4);
            goto LABEL_7;
        case 5:
            v5 = fabs(a4);
            if (v5 == 0.0)
            {
                a4 = 100.0f;
            }
            else
            {
                v4 = 1.0 / fabs(v5);
            LABEL_7:
                a4 = (float)v4;
            }
            goto LABEL_8;
        case 6:
        LABEL_8:
            v6 = SND_pst_GetOrArray(a1, a2);
            v7 = 0;
            if (v6->innerNum <= 0)
                return;
            v8 = v6->i4;
            break;
        default:
            return;
        }
        while ((a2 & *v8) == 0)
        {
        LABEL_16:
            ++v7;
            ++v8;
            if (v7 >= v6->innerNum)
                return;
        }
        switch (a3)
        {
        case 3:
            v6->unkIntArrPtr[v7]->muteSetup1.muteSetUp2 = a4;
            goto LABEL_16;
        case 4:
            v6->unkIntArrPtr[v7]->muteSetup1.muteSetUp1 = a4;
            goto LABEL_16;
        case 5:
            v6->unkIntArrPtr[v7]->muteSetup1TimeMultiplier = a4;
            goto LABEL_16;
        case 6:
            v6->unkIntArrPtr[v7]->muteSetup2TimeMultiplier = -a4;
            goto LABEL_16;
        default:
            return;
        }
    }
}

float SND_f_GetfFamilly(int a1, int a2, int a3)
{
    double result;
    tSoundFamilyStruct* v4;
    int v5;
    int i;

    if (!SND_gst_Params.soundEnabled)
        return -1.0;
    result = -1.0;
    switch (a3)
    {
    case 3:
    case 4:
    case 5:
    case 6:
        v4 = SND_pst_GetOrArray(a1, a2);
        result = (float)-1.0;
        v5 = v4->innerNum;
        for (i = 0; i < v5; ++i)
        {
            if ((a2 & v4->i4[i]) != 0)
            {
                switch (a3)
                {
                case 3:
                    result = v4->unkIntArrPtr[i]->muteSetup1.muteSetUp2;
                    break;
                case 4:
                    result = v4->unkIntArrPtr[i]->muteSetup1.muteSetUp1;
                    break;
                case 5:
                    result = 1.0 / v4->unkIntArrPtr[i]->muteSetup1TimeMultiplier;
                    break;
                case 6:
                    result = -(1.0 / v4->unkIntArrPtr[i]->muteSetup2TimeMultiplier);
                    break;
                default:
                    break;
                }
                i = v4->innerNum;
            }
        }
        break;
    default:
        return (float)result;
    }
    return (float)result;
}

int SND_i_GetStatusFromFamilly(int a1, int a2)
{
    int v2;
    tSoundInstanceElem* v4;
    int i; 
    int v6;

    v2 = 0;
    v6 = 0;
    if (!SND_gst_Params.soundEnabled)
        return 0;
    if (SND_gst_Params.soundInstanceNum <= 0)
        return v6;
    v4 = SND_gst_Params.soundInstances;
    for (i = 0; ; ++i)
    {
        if ((v4[i].flags & 0x400) == 0 || (a1 & v4[i].familyBitmask) != a1 || (v4[i].familyBitmask & a2) == 0)
            goto LABEL_10;
        if (SND_i_IsPlaying(v2))
            break;
        v4 = SND_gst_Params.soundInstances;
    LABEL_10:
        if (++v2 >= SND_gst_Params.soundInstanceNum)
            return 0;
    }
    return 1;
}

void SND_FadeInFamilly(int a1, int a2)
{
    tSoundFamilyStruct* v2;
    tMutesSetupStruct* v3 = NULL;  // TODO FIXME: ez biztos jó? a játék nem inicializálja ezt
    int v4;
    int* v5;
    int v6;
    tSoundInstanceElem* v7;
    int v8;

    if (SND_gst_Params.soundEnabled)
    {
        v2 = SND_pst_GetOrArray(a1, a2);
        v4 = 0;
        if (v2->innerNum > 0)
        {
            v5 = v2->i4;
            do
            {
                if ((a2 & *v5) != 0)
                {
                    v3 = &v2->unkIntArrPtr[v4]->muteSetup2;
                    v3->muteEnabled = 1;
                    v2->unkIntArrPtr[v4]->muteSetup2.muteSetUp2 = 0.0;
                }
                ++v4;
                ++v5;
            } while (v4 < v2->innerNum);
        }
        v6 = 0;
        if (SND_gst_Params.soundInstanceNum > 0)
        {
            v7 = SND_gst_Params.soundInstances;
            v8 = 0;
            do
            {
                if ((v7[v8].flags & 0x400) != 0 && (a1 & v7[v8].familyBitmask) == a1 && (v7[v8].familyBitmask & a2) != 0)
                {
                    v7[v8].unkMuteSetupSeeInitVolTable = v3;
                    v7 = SND_gst_Params.soundInstances;
                }
                ++v6;
                ++v8;
            } while (v6 < SND_gst_Params.soundInstanceNum);
        }
    }
}

void SND_FadeOutFamilly(int a1, int a2)
{
    int v3;
    tSoundFamilyStruct* v4;
    tMutesSetupStruct* v5 = NULL; // TODO FIXME: ez biztos jó? a játék nem inicializálja ezt
    int v6;
    int* v7;
    int v8;
    tSoundInstanceElem* v9;
    int v10;

    if (SND_gst_Params.soundEnabled)
    {
        v3 = a1;
        v4 = SND_pst_GetOrArray(a1, a2);
        v6 = 0;
        if (v4->innerNum > 0)
        {
            v7 = v4->i4;
            do
            {
                if ((a2 & *v7) != 0)
                {
                    v5 = &v4->unkIntArrPtr[v6]->muteSetup2;
                    v5->muteEnabled = 2;
                    v4->unkIntArrPtr[v6]->muteSetup2.muteSetUp2 = 1.0;
                }
                ++v6;
                ++v7;
            } while (v6 < v4->innerNum);
            v3 = a1;
        }
        v8 = 0;
        if (SND_gst_Params.soundInstanceNum > 0)
        {
            v9 = SND_gst_Params.soundInstances;
            v10 = 0;
            do
            {
                if ((v9[v10].flags & 0x400) != 0 && (v3 & v9[v10].familyBitmask) == v3 && (v9[v10].familyBitmask & a2) != 0)
                {
                    v9[v10].unkMuteSetupSeeInitVolTable = v5;
                    v9 = SND_gst_Params.soundInstances;
                }
                ++v8;
                ++v10;
            } while (v8 < SND_gst_Params.soundInstanceNum);
        }
    }
}

void SND_UnlinkFromFamilly(int a1, int a2)
{
    tSoundInstanceElem* v2;

    if (a2 >= 0 && a2 < SND_gst_Params.soundInstanceNum)
    {
        v2 = &SND_gst_Params.soundInstances[a2];
        if ((v2->flags & 0x400) != 0)
            v2->familyBitmask &= ~a1;
    }
}

void check_in_trackIDlist_and_calls_SND_StopRq_sub_4994D0(unsigned int a1)
{
    int v1;
    int v2;

    if (SND_gst_Params.soundEnabled)
    {
        v1 = BAS_bsearch(a1, &SND_gst_TrackId);
        if (v1 != -1)
        {
            v2 = SND_gax_TrackPlayingList[v1].trackID;
            if (v2 >= 0)
                SND_StopRq(v2);
        }
    }
}

int SND_i_TrackSeti(unsigned int a1, int a2, int a3)
{
    int v4;
    tTrackListElem* v5;
    int v6;
    int v7;
    int v8;

    if (!SND_gst_Params.soundEnabled)
        return -1;
    v4 = BAS_bsearch(a1, &SND_gst_TrackId);
    if (v4 == -1)
        return -1;
    v5 = &SND_gax_TrackPlayingList[v4];
    if (a2 == 1)
    {
        v8 = v5->flags;
        if (a3)
        {
            v5->flags = v8 | 0x20;
            return 0;
        }
        v5->flags = v8 & ~0x20u;
        return 0;
    }
    if (a2 == 2)
    {
        v6 = v5->unk1;
        if (a3)
        {
            if (!v6)
            {
                v5->unk1 = 1;
                goto LABEL_14;
            }
        }
        else if (v6)
        {
            v5->unk1 = 0;
        LABEL_14:
            v7 = v5->trackID;
            if (v7 >= 0)
            {
                SND_SetInstVolume(&SND_gst_Params.soundInstances[v7]);
                return 0;
            }
            return 0;
        }
        return 0;
    }
    if (a2 != 4)
        return -1;
    check_in_trackIDlist_and_calls_SND_StopRq_sub_4994D0(a1);
    return 0;
}

int SND_i_TrackGeti(unsigned int a1, int a2)
{
    int v2;
    tTrackListElem* v3;

    if (!SND_gst_Params.soundEnabled)
        return -1;
    v2 = BAS_bsearch(a1, &SND_gst_TrackId);
    if (v2 == -1)
        return -1;
    v3 = &SND_gax_TrackPlayingList[v2];
    if (a2 == 1)
        return ((unsigned int)v3->flags >> 5) & 1;
    if (a2 == 2)
        return v3->unk1 != 0;
    if (a2 != 5)
        return -1;
    return v3->trackID;
}

void SND_TrackSetFadeOut(int key, float a2)
{
    int v2;
    int* v3;
    int v4;
    int v5;
    int v6;
    tSoundInstanceElem* v7;
    tSoundModifierDataFade a3;

    if (SND_gst_Params.soundEnabled)
    {
        if (a2 > 0.0)
        {
            v2 = BAS_bsearch(key, &SND_gst_TrackId);
            if (v2 != -1)
            {
                v3 = &SND_gax_TrackPlayingList[v2].trackID;
                v4 = *v3;
                if (*v3 >= 0 && v4 < SND_gst_Params.soundInstanceNum)
                {
                    v5 = v4;
                    v6 = SND_gst_Params.soundInstances[v5].flags;
                    v7 = &SND_gst_Params.soundInstances[v5];
                    if ((v6 & 0x400) != 0)
                    {
                        a3.i3 = a2;
                        a3.i2 = 0.0;
                        a3.i1 = v7->volume;
                        a3.i5 = 0x80;
                        a3.gen.flags = 0;
                        a3.gen.sndKey = SND_gst_Params.allocatedSounds[v7->allocatedSoundsIdx].soundKey;
                        a3.gen.sndIndexInAllocatedSounds = v7->allocatedSoundsIdx;
                        SND_SModifierAddInInstance(*v3, 4, &a3, 36);
                    }
                }
            }
        }
    }
}

int SND_i_TrackSetf(int a1, int a2, float a3)
{
    int v4;
    tTrackListElem* v5;
    int v6;

    if (!SND_gst_Params.soundEnabled)
        return -1;
    v4 = BAS_bsearch(a1, &SND_gst_TrackId);
    if (v4 == -1)
        return -1;
    v5 = &SND_gax_TrackPlayingList[v4];
    if (a2 == 1)
        return 0;
    if (a2 == 3)
    {
        v5->f1 = a3;
        v6 = v5->trackID;
        if (v6 >= 0)
            SND_SetInstVolume(&SND_gst_Params.soundInstances[v6]);
        return 0;
    }
    if (a2 != 6)
        return -1;
    SND_TrackSetFadeOut(a1, a3);
    return 0;
}

float SND_f_TrackGetf(unsigned int a1, int a2)
{
    int v2;
    tTrackListElem* v3;
    double result;
    int v5;

    if (!SND_gst_Params.soundEnabled)
        return -1.0f;
    v2 = BAS_bsearch(a1, &SND_gst_TrackId);
    if (v2 == -1)
        return -1.0f;
    v3 = &SND_gax_TrackPlayingList[v2];
    if (a2 == 1)
    {
        if ((v3->flags & 0x20) == 0
            || (v5 = v3->trackID, v5 >= 0)
            && v5 < SND_gst_Params.soundInstanceNum
            && (SND_gst_Params.soundInstances[v5].flags & 0x400) != 0)
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
        if (a2 != 3)
            return -1.0f;
        result = v3->f1;
    }
    return (float)result;
}

void SND_MergeBank(tGameObjectMain* a1, tGameObjectMain* a2, int a3, int a4, int a5)
{
    int v5;
    tGameObjectExtended* v6;
    tSndBank* v7;
    tGameObjectExtended* v8;
    tSndBank* v9;
    int v10;
    int v11;
    int* v12;
    int* v13;
    unsigned int v14;
    int v15;
    int v16;
    tSndSoundElem* v17;
    int v18;

    v5 = a3;
    if (a3 >= 0)
    {
        if (a2)
        {
            if ((a2->flags & 0x8000000) != 0)
            {
                v6 = a2->extended;
                if (v6)
                {
                    v7 = v6->snd;
                    if (v7)
                    {
                        if (v7->soundIndices)
                        {
                            if (a1)
                            {
                                if ((a1->flags & 0x8000000) != 0)
                                {
                                    v8 = a1->extended;
                                    if (v8)
                                    {
                                        v9 = v8->snd;
                                        if (v9)
                                        {
                                            if (v9->soundIndices)
                                            {
                                                v10 = a4;
                                                if (a4 == -1)
                                                {
                                                    v18 = v9->elemNum;
                                                    if (v7->elemNum <= v18)
                                                        v18 = v7->elemNum;
                                                    a4 = v18 - 1;
                                                    v10 = a4;
                                                }
                                                if (v10 < v7->elemNum)
                                                {
                                                    v11 = v9->elemNum;
                                                    if (v10 < v11)
                                                    {
                                                        if (!v9->soundIndicesV2)
                                                        {
                                                            v9->soundIndicesV2 = v9->soundIndices;
                                                            v12 = (int*)MEM_p_Alloc(4 * v11);
                                                            v13 = v9->soundIndicesV2;
                                                            v10 = a4;
                                                            v14 = 4 * v9->elemNum;
                                                            v9->soundIndices = v12;
                                                            memcpy(v12, v13, v14);
                                                            v5 = a3;
                                                        }
                                                        for (; v5 <= v10; ++v5)
                                                        {
                                                            v15 = v9->soundIndices[v5];
                                                            v16 = v7->soundIndices[v5];
                                                            if (v15 != v16 && v16 != -1 && (v15 == -1 || a5))
                                                            {
                                                                v17 = &SND_gst_Params.allocatedSounds[v16];
                                                                if ((v17->flags & 0x400) != 0)
                                                                    ++v17->usageCnt;
                                                                if (v9->soundIndices[v5] != v9->soundIndicesV2[v5])
                                                                {
                                                                    SND_DeleteByIndex(v9->soundIndices[v5]);
                                                                    v10 = a4;
                                                                }
                                                                v9->soundIndices[v5] = v7->soundIndices[v5];
                                                            }
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

void SND_MergeBankOver(struct sGameObjectMain* a1, struct sGameObjectMain* a2)
{
    SND_MergeBank(a2, a1, 0, -1, 1);
}

void SND_MergeBankNoOver(struct sGameObjectMain* a1, struct sGameObjectMain* a2)
{
    SND_MergeBank(a2, a1, 0, -1, 0);
}

void SND_PrefetchMdF(struct sSoundMdfStruct* a1)
{
    int v1;

    if (SND_gst_Params.soundEnabled)
    {
        MDF_SndGetSoundIndexFromBank(a1);
        v1 = a1->idx;
        if (v1 >= 0)
            SND_StreamPrefetchOneSound(v1, 1);
    }
}

void SND_StreamPrefetch(int a1)
{
    tSoundInstanceElem* v1;
    int v2;
    tSndSoundElem* v3;
    int v4;
    unsigned int v5;

    if (a1 >= 0 && a1 < SND_gst_Params.soundInstanceNum)
    {
        v1 = &SND_gst_Params.soundInstances[a1];
        if ((v1->flags & 0x400) != 0)
        {
            v2 = v1->allocatedSoundsIdx;
            v3 = &SND_gst_Params.allocatedSounds[v1->allocatedSoundsIdx];
            if ((v3->flags & 0x86000) != 0)
            {
                v4 = v3->startDataPos;
                if (v4)
                {
                    if (v4 != -1)
                    {
                        v5 = v3->fullsoundLen;
                        if (v5)
                        {
                            if (v5 >= 0x25800)
                                SND_StreamPrefetchOneSound(v2, 1);
                        }
                    }
                }
            }
        }
    }
}

void SND_FlushMdF(tSoundMdfStruct* a1)
{
    int v1;

    if (SND_gst_Params.soundEnabled)
    {
        MDF_SndGetSoundIndexFromBank(a1);
        v1 = a1->idx;
        if (v1 >= 0)
            SND_StreamFlushOneSound_0(v1, 1);
    }
}

void SND_StreamFlush(int instanceIdx)
{
    tSoundInstanceElem* v1;
    int v2;
    tSndSoundElem* v3;
    int v4;

    if (instanceIdx >= 0 && instanceIdx < SND_gst_Params.soundInstanceNum)
    {
        v1 = &SND_gst_Params.soundInstances[instanceIdx];
        if ((v1->flags & 0x400) != 0)
        {
            v2 = v1->allocatedSoundsIdx;
            v3 = &SND_gst_Params.allocatedSounds[v1->allocatedSoundsIdx];
            if ((v3->flags & 0x86000) != 0)
            {
                v4 = v3->startDataPos;
                if (v4)
                {
                    if (v4 != -1)
                        SND_StreamFlushOneSound_0(v2, 1);
                }
            }
        }
    }
}

int SND_i_InstanceSeti_return_negative_1(int i1, int i2, int i3)
{
    return -1;
}

int SND_i_InstanceGeti(int a1, int a2)
{
    tSoundInstanceElem* v2;

    if (a1 >= 0 && a1 < SND_gst_Params.soundInstanceNum)
    {
        v2 = &SND_gst_Params.soundInstances[a1];
        if ((v2->flags & 0x400) != 0)
        {
            if (a2 == 4112)
            {
                v2->soundIntf->GetStatus(v2->soundBufElemFromModel, &a1);
                return ((unsigned int)a1 >> 4) & 1;
            }
            if (a2 == 4113)
                return v2->familyBitmask;
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

void SND_Fade(int a1, float a2, float a3, float a4)
{
    tMutesSetupStruct* fg;

    switch (a1)
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
        fg = &SND_Mutes_B30600_fadergroups[a1];
        if (a4 <= 0.0 || a2 == a3)
        {
            fg->elapsedTime = 0.0;
            fg->flags = 0;
            fg->muteEnabled = 0;
        }
        else
        {
            fg->muteSetUp2ToSetWithThis = a3;
            fg->elapsedTime = 0.0;
            fg->i7 = a2;
            fg->i6 = (256.0f / a4);
            if (a3 <= a2)
                fg->flags = 2;
            else
                fg->flags = 1;
            fg->muteSetUp2 = a2;
            fg->muteSetUp1 = 1.0f;
            fg->muteEnabled = 1;
        }
        break;
    default:
        return;
    }
}

void SND_PlaySoundOnTrackFromTo(int a1, float a2, float a3)
{
    tSoundInstanceElem* v3;
    int v4;
    double v5;
    double v6;
    double v7;

    if (a1 >= 0 && a1 < SND_gst_Params.soundInstanceNum)
    {
        v3 = &SND_gst_Params.soundInstances[a1];
        if ((v3->flags & 0x400) != 0)
        {
            if ((v3->muteFactorFlags & 0x1000) != 0
                || (v3->soundIntf->GetStatus(v3->soundBufElemFromModel, &a1),
                    (a1 & 1) == 0))
            {
                v4 = v3->allocatedSoundsIdx;
                v3->flags = v3->flags & ~4u | 0x200;
                v5 = (double)(2 * v3->freq * SND_gst_Params.allocatedSounds[v4].numChannels);
                if (((v3->freq * SND_gst_Params.allocatedSounds[v4].numChannels) & 0x40000000) != 0)
                    v5 = v5 + 4294967300.0; // TODO FIXME ....?????!
                v6 = a2 * v5 + MATH_gd_magicDoubleForRound;
                v7 = v5 * a3 + MATH_gd_magicDoubleForRound;
                SND_SetInstVolume(v3);
                SND_PlayRq((*(uint32_t *)&v6) & 0xFFFFFFFC, v3, 0x8000000, (*(uint32_t *)&v7) & 0xFFFFFFFC);
            }
        }
    }
}

void SND_PlaySoundLoopingOnTrackFrom(int a1, float a2)
{
    tSoundInstanceElem* v2;
    int v3;
    double v4;
    double v5;

    if (a1 >= 0 && a1 < SND_gst_Params.soundInstanceNum)
    {
        v2 = &SND_gst_Params.soundInstances[a1];
        if ((v2->flags & 0x400) != 0)
        {
            if ((v2->muteFactorFlags & 0x1000) != 0
                || (v2->soundIntf->GetStatus(v2->soundBufElemFromModel, &a1),
                    (a1 & 1) == 0))
            {
                v3 = v2->allocatedSoundsIdx;
                v2->flags = v2->flags & ~4u | 0x200;
                v4 = (double)(2 * v2->freq * SND_gst_Params.allocatedSounds[v3].numChannels);
                if (((v2->freq * SND_gst_Params.allocatedSounds[v3].numChannels) & 0x40000000) != 0)
                    v4 = v4 + 4294967300.0; // TODO FIXME ....?????!
                v5 = v4 * a2 + MATH_gd_magicDoubleForRound;
                SND_SetInstVolume(v2);
                SND_PlayRq((*(uint32_t *)&v5) & 0xFFFFFFFC, v2, 0x8000001, -1);
            }
        }
    }
}

float SND_SoundDurationGet(struct sGameObjectMain* a1, int sndIdx)
{
    float v3;
    float v4;
    tSndSoundElem* v5;

    if (!SND_gst_Params.soundEnabled)
        return 0.0;
    if ((a1->flags & 0x8000000) == 0)
        return 0.0;
    if (!a1->extended)
        return 0.0;
    if (!a1->extended->snd)
        return 0.0;
    if (!a1->extended->snd->soundIndices)
        return 0.0;
    if (sndIdx < 0 || sndIdx >= a1->extended->snd->elemNum)
        return 0.0;
    if (sndIdx >= SND_gst_Params.allocatedSoundsMaxNum)
        return 0.0;
    v5 = &SND_gst_Params.allocatedSounds[a1->extended->snd->soundIndices[sndIdx]];
    if ((v5->flags & 0x400) == 0)
        return 0.0;
    if ((v5->flags & 0x200000) == 0)
    {
        if ((v5->flags & 0x4000) != 0 || (v5->flags & 0x2000) != 0 || (v5->flags & 0x80000) != 0)
        {
            SND_l_AddStreamLoad(v5->soundKey);
        }
        else if ((v5->flags & 8) != 0)
        {
            SND_l_LoadLoadingSoundHeader(v5->soundKey);
        }
        else
        {
            SND_l_LoadHeader(v5->soundKey);
        }
    }
    v4 = (float)v5->fullsoundLen;
    v3 = (float)v5->byteRate;
    return v4 / v3;
}

void SND_ResetAllMute()
{
    tMutesSetupStruct* v0;

    v0 = SND_Mutes_B30600_fadergroups;
    do
    {
        if (v0 != &SND_Mutes_B30600_fadergroups[6])
        {
            v0->muteEnabled = 0;
            v0->muteSetUp1 = 1.0f;
            v0->muteSetUp2 = 0.5f;
            v0->flags = 0;
            v0->elapsedTime = 0.0f;
        }
        ++v0;
    } while (v0 <= &SND_Mutes_B30600_fadergroups[11]);

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