#include "common.h"
#include "memory.h"
#include "loading.h"
#include "bigfiles.h"
#include "modifiers/gensoundmodifier.h"
#include "dbg.h"
#include "time.h"
#include "sound.h"

int SND_SModifier_FadeTable[] = {
0xFFFFFFF9, 0xFFFFFFE4, 0xFFFFFFD0, 0xFFFFFFBD, 0xFFFFFFA7,
0xFFFFFF94, 0xFFFFFF80, 0xFFFFFF6B, 0xFFFFFF57, 0xFFFFFF42,
0xFFFFFF2E, 0xFFFFFF1B, 0xFFFFFF05, 0xFFFFFEF2, 0xFFFFFEDE,
0xFFFFFEC9, 0xFFFFFEC9, 0xFFFFFEB3, 0xFFFFFE9E, 0xFFFFFE8A,
0xFFFFFE74, 0xFFFFFE5F, 0xFFFFFE49, 0xFFFFFE33, 0xFFFFFE1E,
0xFFFFFE0A, 0xFFFFFDF4, 0xFFFFFDDF, 0xFFFFFDC9, 0xFFFFFDB3,
0xFFFFFD9E, 0xFFFFFD8A, 0xFFFFFD8A, 0xFFFFFD72, 0xFFFFFD5C,
0xFFFFFD46, 0xFFFFFD2F, 0xFFFFFD19, 0xFFFFFD03, 0xFFFFFCED,
0xFFFFFCD5, 0xFFFFFCBF, 0xFFFFFCAA, 0xFFFFFC92, 0xFFFFFC7C,
0xFFFFFC66, 0xFFFFFC50, 0xFFFFFC3A, 0xFFFFFC3A, 0xFFFFFC22,
0xFFFFFC0D, 0xFFFFFBF5, 0xFFFFFBDF, 0xFFFFFBC9, 0xFFFFFBB3,
0xFFFFFB9D, 0xFFFFFB85, 0xFFFFFB70, 0xFFFFFB5A, 0xFFFFFB42,
0xFFFFFB2C, 0xFFFFFB16, 0xFFFFFB00, 0xFFFFFAE9, 0xFFFFFAE9,
0xFFFFFAD8, 0xFFFFFAC7, 0xFFFFFAB6, 0xFFFFFAA5, 0xFFFFFA94,
0xFFFFFA83, 0xFFFFFA72, 0xFFFFFA61, 0xFFFFFA50, 0xFFFFFA3F,
0xFFFFFA2E, 0xFFFFFA1D, 0xFFFFFA0C, 0xFFFFF9FB, 0xFFFFF9E9, 0xFFFFF9E9,
0xFFFFF9D5, 0xFFFFF9C0, 0xFFFFF9AC, 0xFFFFF999, 0xFFFFF983,
0xFFFFF970, 0xFFFFF95C, 0xFFFFF947, 0xFFFFF933, 0xFFFFF91E,
0xFFFFF90A, 0xFFFFF8F7, 0xFFFFF8E1, 0xFFFFF8CE, 0xFFFFF8BA,0xFFFFF8BA,
0xFFFFF8A2, 0xFFFFF88A, 0xFFFFF872, 0xFFFFF85A, 0xFFFFF842,
0xFFFFF82A, 0xFFFFF811, 0xFFFFF7F9, 0xFFFFF7E1, 0xFFFFF7C9,
0xFFFFF7B1, 0xFFFFF799, 0xFFFFF781, 0xFFFFF769, 0xFFFFF752,0xFFFFF752,
0xFFFFF73F, 0xFFFFF72E, 0xFFFFF71D, 0xFFFFF70A, 0xFFFFF6F9,
0xFFFFF6E8, 0xFFFFF6D7, 0xFFFFF6C4, 0xFFFFF6B3, 0xFFFFF6A2,
0xFFFFF68F, 0xFFFFF67E, 0xFFFFF66D, 0xFFFFF65A, 0xFFFFF649,0xFFFFF649,
0xFFFFF636, 0xFFFFF623, 0xFFFFF611, 0xFFFFF5FE, 0xFFFFF5EB,
0xFFFFF5DA, 0xFFFFF5C7, 0xFFFFF5B3, 0xFFFFF5A2, 0xFFFFF58F,
0xFFFFF57C, 0xFFFFF569, 0xFFFFF557, 0xFFFFF544, 0xFFFFF531,0xFFFFF531,
0xFFFFF520, 0xFFFFF50D, 0xFFFFF4FB, 0xFFFFF4E8, 0xFFFFF4D7,
0xFFFFF4C4, 0xFFFFF4B3, 0xFFFFF4A0, 0xFFFFF48F, 0xFFFFF47C,
0xFFFFF469, 0xFFFFF457, 0xFFFFF446, 0xFFFFF433, 0xFFFFF422,0xFFFFF422,
0xFFFFF40F, 0xFFFFF3FE, 0xFFFFF3EB, 0xFFFFF3D8, 0xFFFFF3C7,
0xFFFFF3B5, 0xFFFFF3A2, 0xFFFFF391, 0xFFFFF37E, 0xFFFFF36D,
0xFFFFF35A, 0xFFFFF347, 0xFFFFF336, 0xFFFFF323, 0xFFFFF311,0xFFFFF311,
0xFFFFF2FC, 0xFFFFF2E6, 0xFFFFF2D2, 0xFFFFF2BD, 0xFFFFF2A7,
0xFFFFF291, 0xFFFFF27C, 0xFFFFF266, 0xFFFFF252, 0xFFFFF23D,
0xFFFFF227, 0xFFFFF211, 0xFFFFF1FC, 0xFFFFF1E6, 0xFFFFF1D2,0xFFFFF1D2,
0xFFFFF1B6, 0xFFFFF199, 0xFFFFF17C, 0xFFFFF161, 0xFFFFF144,
0xFFFFF127, 0xFFFFF10C, 0xFFFFF0F0, 0xFFFFF0D3, 0xFFFFF0B6,
0xFFFFF09B, 0xFFFFF07E, 0xFFFFF063, 0xFFFFF045, 0xFFFFF02A,0xFFFFF02A,
0xFFFFF005, 0xFFFFEFE1, 0xFFFFEFBD, 0xFFFFEF99, 0xFFFFEF74,
0xFFFFEF4F, 0xFFFFEF2C, 0xFFFFEF08, 0xFFFFEEE4, 0xFFFFEEBE,
0xFFFFEE9B, 0xFFFFEE77, 0xFFFFEE53, 0xFFFFEE2D, 0xFFFFEE09,0xFFFFEE09,
0xFFFFEDCD, 0xFFFFED8F, 0xFFFFED51, 0xFFFFED15, 0xFFFFECD6,
0xFFFFEC9A, 0xFFFFEC5C, 0xFFFFEC1F, 0xFFFFEBE1, 0xFFFFEBA5,
0xFFFFEB66, 0xFFFFEB2A, 0xFFFFEAEC, 0xFFFFEAB0, 0xFFFFEA72,0xFFFFEA72,
0xFFFFE9CD, 0xFFFFE927, 0xFFFFE882, 0xFFFFE7DC, 0xFFFFE737,
0xFFFFE691, 0xFFFFE5ED, 0xFFFFE548, 0xFFFFE4A1, 0xFFFFE3FD,
0xFFFFE357, 0xFFFFE2B2, 0xFFFFE20D, 0xFFFFE168, 0xFFFFE0C1
};

void SND_SModifierDeleteOneSModifier(tSndModifierElem** list, int type)
{
    tSndModifierElem* cur;
    tSndModifierElem* prev;

    prev = 0;
    for (cur = *list; cur && cur->sndModifierModelIntf; cur = cur->nextModifier)
    {
        if (cur->sndModifierModelIntf->Type == type)
        {
            cur->sndModifierModelIntf->Destroy(cur, 0, 0);
            if (prev)
                prev->nextModifier = cur->nextModifier;
            else
                *list = cur->nextModifier;
            MEM_Free(cur);
            return;
        }
        prev = cur;
    }
}

void SND_SModifierDeleteAll(tSndModifierElem** list)
{
    while (*list)
        SND_SModifierDeleteOneSModifier(list, (*list)->sndModifierModelIntf->Type);
}

void SND_SModifierDeleteAllLinkedSound(tSndModifierElem* list)
{
    tSndModifierElem* cur;
    tSoundModifierDataGeneric* data;

    for (cur = list; cur; cur = cur->nextModifier)
    {
        if (!cur->sndModifierModelIntf)
            break;
        data = (tSoundModifierDataGeneric*)(cur->linkedSoundData);
        if (data)
            SND_DeleteByIndex(data->sndIndexInAllocatedSounds);
    }
}

struct sSndModifierElem* SND_pst_SModifierInsertOneSModifier(struct sSndModifierElem** list, int type, void* linkedSound, int dataSize)
{
    tSndModifierElem* cur;
    tSndModifierElem* added;
    tSoundModifierElemIntf* intf;

    if (type >= 7 || type < 0)
        return *list;
    for (cur = *list; cur && cur->sndModifierModelIntf; cur = cur->nextModifier)
    {
        if (cur->sndModifierModelIntf->Type == type)
        {
            cur->sndModifierModelIntf->InitChainedElemVersion(cur, linkedSound, dataSize); // init
            return cur;
        }
    }
    added = (tSndModifierElem*)MEM_p_Alloc(sizeof(tSndModifierElem));
    added->sndModifierModelIntf = 0;
    added->nextModifier = 0;
    added->linkedSoundDataSize = 0;
    added->linkedSoundData = 0;
    intf = SND_gs_SoundModifiers[type].sndModifierModelIntf;
    added->sndModifierModelIntf = intf;
    intf->Init(added, linkedSound, dataSize);// ez is ugyanaz az init
    added->nextModifier = *list;
    *list = added;
    return added;
}

void SND_SModifierInstanciate(struct sSndModifierElem* list, struct sSoundInstanceElem* inst)
{
    tSndModifierElem* cur;
    tSndModifierElem* applied;

    for (cur = list; cur; cur = cur->nextModifier)
    {
        applied = SND_pst_SModifierInsertOneSModifier(&inst->modifierInstances, cur->sndModifierModelIntf->Type, cur->linkedSoundData, cur->linkedSoundDataSize);
        cur->sndModifierModelIntf->PlayApplyOnInstance(applied, cur->linkedSoundData, cur->linkedSoundDataSize, inst);
    }
}

int SND_l_AddSModifierNoLoad(int soundKey, char** cursor)
{
    int existingIdx;
    int slot;
    tSndSoundElem* sound;
    int* header;
    int* record;
    int* payload;
    int recordFlags;
    int recordSoundIdx;
    int speedMode;
    char msg[256];
    int offset;
    int size;

    if (!SND_gst_Params.soundEnabled)
        return -1;
    if (!soundKey)
        return -1;
    if (soundKey == -1)
        return -1;
    offset = BIG_ul_SearchKeyToPos(soundKey);
    if (offset == -1)
        return -1;
    existingIdx = SND_l_GetSoundIndex(soundKey);
    if (existingIdx != -1)
    {
        ++SND_gst_Params.allocatedSounds[existingIdx].usageCnt;
        return existingIdx;
    }
    slot = SND_i_AllocSound();
    sound = &SND_gst_Params.allocatedSounds[slot];
    sound->soundKey = soundKey;
    sound->sndSndBufElem = (tSndSoundBufferElem*)-1;
    sound->flags = 0x10400;
    sound->muteFacorFlags = 64;
    sound->freq = 0;
    sound->startDataPos = 0;
    sound->numChannels = 0;
    sound->bitsPerSample = 0;
    sound->usageCnt = 1;
    sound->sndInterface = (tSndInterfaceStruct*)&SND_gpst_Interface;
    sound->soundModifier = 0;
    sound->fullsoundLen = 0;
    speedMode = isSpeedModeAndBinFile();
    if (!speedMode)
        BIG_ReadNoSeek(offset, &size, 4u);
    else if (cursor)
        size = LOA_ReadLong(cursor, 0, 1);
    else
        size = 0;
    if (size == 8)
    {
        sprintf(msg, "Bad size (8oct) of SModifier file (.smd) : 0x%x, loading canceled, Please check the file !", soundKey);
        sound->flags = 0;
        return -1;
    }
    header = (int*)MEM_p_Alloc(size);
    memset(header, 0, size);
    if (!speedMode)
        BIG_ReadNoSeek(offset + 4, header, size);
    else if (cursor)
        LOA_ReadCharArray(cursor, (char*)header, size, 1);
    if (*header != 1)
    {
        sprintf(msg, "Bad SModifier file (.smd) : 0x%x, loading canceled", soundKey);
        sound->flags = 0;
        MEM_Free(header);
        return -1;
    }
    record = header + 2; // TODO: => sSoundModifierDataGeneric
    if (record[0] != -1)
    {
        do
        {
            if (record[1])
            {
                recordFlags = record[3];
                payload = record + 2;
                record[4] = -1;
                if ((recordFlags & 0x4000) != 0)
                {
                    recordSoundIdx = SND_l_AddDialogNoLoad(payload[0]);
                }
                else if ((recordFlags & 0x2000) != 0)
                {
                    recordSoundIdx = SND_l_AddMusicNoLoad(payload[0]);
                }
                else if ((recordFlags & 0x80000) != 0)
                {
                    recordSoundIdx = SND_l_AddAmbienceNoLoad(payload[0]);
                }
                else if ((recordFlags & 8) != 0)
                {
                    recordSoundIdx = SND_l_AddLoadingSound(payload[0]);
                }
                else
                {
                    recordSoundIdx = SND_l_AddSound(payload[0]);
                }
                record[4] = recordSoundIdx;
                sound = &(SND_gst_Params.allocatedSounds[slot]);
            }
            else
            {
                payload = 0;
            }
            SND_pst_SModifierInsertOneSModifier(&sound->soundModifier, record[0], (char*)payload, record[1]); // [0] = type, [1] = len, [2] = sndKey, [3] = flags, [4] = idx
            record = (int*)((char*)record + record[1] + 8);
        } while (record[0] != -1);
    }
    MEM_Free(header);
    return slot;
}

BOOL MDF_b_SndIsInPrefetchVolume(struct sSoundMdfStruct* sndMdf)
{
    BOOL result;
    int instanceIdx;
    tSoundInstanceElem* inst;
    Vec3* instPos;

    if (!SND_gst_Params.sndMatrixRef)
        return 0;
    instanceIdx = sndMdf->sndInstanceID;
    result = 0;
    if (instanceIdx >= 0 && instanceIdx < SND_gst_Params.soundInstanceNum)
    {
        inst = &SND_gst_Params.soundInstances[instanceIdx];
        if ((inst->flags & 0x400) != 0)
        {
            instPos = inst->sndPositionSetInSetPosPtr;
            if (instPos)
            {
                if (MATH_DistanceOfTwoPoints(&SND_gst_Params.sndMatrixRef->Pos, instPos) < sndMdf->distPrefetchVolume)
                    result = 1;
            }
        }
    }
    return result;
}

void MDF_SndCreateInstance(struct sSoundMdfStruct* sndMdf, struct sGameObjectMain* gao)
{
    int soundIdx;
    int instanceIdx;
    int coeffNeeded;
    int pan;
    int trackID;
    int familyMask;
    int mdfFlags;
    float blend;
    tLinkedSoundFade fade;

    soundIdx = sndMdf->idx;
    if (soundIdx < 0)
        return;
    coeffNeeded = 0;
    instanceIdx = SND_l_Request(0, soundIdx);
    sndMdf->sndInstanceID = instanceIdx;
    if (instanceIdx == -1)
        return;
    SND_SetVol(instanceIdx, sndMdf->volume2);
    SND_SetFlags(gao, sndMdf->sndInstanceID, sndMdf->flags2);
    SND_SetFlags(gao, sndMdf->sndInstanceID, (sndMdf->ii11 & 0xFCB) | 0x40000000);
    SND_i_CheckOrInitGAO(gao);
    SND_gst_Params.soundInstances[sndMdf->sndInstanceID].gao = gao;
    SND_SetPos(gao, sndMdf->sndInstanceID, gao->matrices);
    if (sndMdf->freq2)
        SND_SetFreq(sndMdf->sndInstanceID, sndMdf->freq2);
    pan = sndMdf->pan;
    if (pan)
        SND_SetPan(sndMdf->sndInstanceID, pan, 0);
    if ((sndMdf->flags2 & 0x10000000) != 0)
    {
        blend = MATH_LinearInterpolateFloats(sndMdf->near1, sndMdf->far1, sndMdf->nearFarFactor);
        SND_SetNearFar(sndMdf->sndInstanceID, 4096, sndMdf->near1, sndMdf->far1);
        SND_i_InstanceSetf(sndMdf->sndInstanceID, 4099, blend);
        coeffNeeded = 1;
    }
    if ((sndMdf->flags2 & 0x1000000) != 0)
    {
        blend = MATH_LinearInterpolateFloats(sndMdf->near1, sndMdf->far1, sndMdf->nearFarFactor);
        SND_SetNearFar(sndMdf->sndInstanceID, 4096, sndMdf->near1, sndMdf->far1);
        SND_i_InstanceSetf(sndMdf->sndInstanceID, 4099, blend);
        coeffNeeded = 1;
    }
    if ((sndMdf->flags2 & 0x2000000) != 0)
    {
        blend = MATH_LinearInterpolateFloats(sndMdf->near2, sndMdf->far2, sndMdf->nearFar2factor);
        SND_SetNearFar(sndMdf->sndInstanceID, 4097, sndMdf->near2, sndMdf->far2);
        SND_i_InstanceSetf(sndMdf->sndInstanceID, 4100, blend);
        coeffNeeded = 1;
    }
    if ((sndMdf->flags2 & 0x4000000) != 0)
    {
        blend = MATH_LinearInterpolateFloats(sndMdf->near3, sndMdf->far3, sndMdf->nearFar3factor);
        SND_SetNearFar(sndMdf->sndInstanceID, 4098, sndMdf->near3, sndMdf->far3);
        SND_i_InstanceSetf(sndMdf->sndInstanceID, 4101, blend);
        coeffNeeded = 1;
    }
    if ((sndMdf->flags2 & 0x8000000) != 0)
    {
        SND_i_InstanceSetf(sndMdf->sndInstanceID, 4105, sndMdf->b0);
        coeffNeeded = 1;
    }
    if ((sndMdf->ii11 & 3) != 0)
        SND_i_InstanceSetf(sndMdf->sndInstanceID, 4104, sndMdf->unkIsInActiveVolume);
    if (coeffNeeded)
    {
        SND_i_InstanceSetf(sndMdf->sndInstanceID, 4103, sndMdf->f4);
        SND_i_InstanceSetf(sndMdf->sndInstanceID, 4102, sndMdf->xx);
    }
    trackID = sndMdf->trackID;
    if (trackID != -1 && trackID)
        SND_SetTrack(sndMdf->sndInstanceID, sndMdf->trackID);
    if (sndMdf->ii2 != 0.0)
    {
        fade.maxTimeToLast = sndMdf->ii2;
        fade.minVolume = sndMdf->volume2;
        fade.maxVolume = sndMdf->volume;
        fade.sndKey = sndMdf->i4;
        fade.fadeFlags = 1;
        fade.flags = 0;
        fade.sndIndexInAllocatedSounds = sndMdf->idx;
        SND_SModifierAddInInstance(sndMdf->sndInstanceID, 3, &fade, 36);// fadein
    }
    if (sndMdf->ii4 != 0.0)
    {
        mdfFlags = sndMdf->flags;
        fade.maxTimeToLast = sndMdf->ii4;
        fade.fadeFlags = 0x80;
        fade.maxVolume = sndMdf->volume2;
        fade.minVolume = sndMdf->volume;
        if ((mdfFlags & 0x10) != 0)
            fade.fadeFlags = 160;
        if ((mdfFlags & 0x20) != 0)
            fade.fadeFlags |= 8u;
        fade.sndKey = sndMdf->i4;
        fade.flags = 0;
        fade.sndIndexInAllocatedSounds = sndMdf->idx;
        SND_SModifierAddInInstance(sndMdf->sndInstanceID, 4, &fade, 36);// fadeout
    }
    familyMask = sndMdf->ii5;
    if (familyMask)
        SND_LinkToFamilly(familyMask, sndMdf->sndInstanceID);
    mdfFlags = sndMdf->flags;
    if ((mdfFlags & 1) != 0 && (sndMdf->trackID == -1 || (mdfFlags & 0x400) == 0 || !SND_i_IsTrackPlaying(sndMdf->trackID)))
        SND_PlayMdF(sndMdf);
}

int MDF_b_SndIsInActiveVolume(struct sSoundMdfStruct* sndMdf)
{
    int instanceIdx;
    tSoundInstanceElem* inst;
    int instFlags;
    Mtx4x4* posMtx;
    Vec3* axis;
    int axisIdx;
    int axisBit;
    double proj;
    double projYZ;
    double projX;
    Vec3* refPos;
    Vec3* instPos;
    Vec3 delta;
    int axisState[3];

    if (!SND_gst_Params.sndMatrixRef)
        return 0;
    instanceIdx = sndMdf->sndInstanceID;
    if (instanceIdx < 0 || instanceIdx >= SND_gst_Params.soundInstanceNum)
        return 0;
    inst = &SND_gst_Params.soundInstances[instanceIdx];
    instFlags = inst->flags;
    if ((instFlags & 0x400) == 0 || (instFlags & 0x17000000) == 0)
        return 0;
    if (!inst->sndPositionSetInSetPosPtr)
        return 0;
    posMtx = inst->sndPosMtxSetInSetPos;
    if (!posMtx && (instFlags & 0x7000000) != 0)
        return 0;
    instPos = inst->sndPositionSetInSetPosPtr;
    refPos = &SND_gst_Params.sndMatrixRef->Pos;
    if ((instFlags & 0x10000000) != 0)
        return sndMdf->far1 + inst->dtFar > MATH_DistanceOfTwoPoints(refPos, instPos);
    axisState[0] = 0x1000000;
    axisState[1] = 0x2000000;
    axisState[2] = 0x4000000;
    MATH_SubstractVec(&delta, refPos, instPos);
    for (axisIdx = 0; axisIdx < 3; ++axisIdx)
    {
        axisBit = axisState[axisIdx];
        if ((axisBit & instFlags) == 0)
        {
            axisState[axisIdx] = 2;
            continue;
        }
        if (axisIdx == 0)
            axis = &posMtx->v1;
        else if (axisIdx == 1)
            axis = &posMtx->v2;
        else
            axis = &posMtx->v3;
        projYZ = axis->z * delta.z + axis->y * delta.y;
        projX = delta.x * axis->x;
        proj = fabs(projYZ + projX);
        if (proj >= inst->farArr[axisIdx] + inst->dtFar)
            axisState[axisIdx] = 0;
        else
            axisState[axisIdx] = axisBit | 2;
    }
    return (axisState[0] & axisState[1] & axisState[2] & 2) >> 1;
}

int MDF_SndGetSoundIndexFromBank(tSoundMdfStruct* gsm)
{
    int result;
    tSndBank* bank;
    int* indices;

    result = gsm->flags;
    if ((result & 0x800) != 0 && gsm->idx == -1)
    {
        bank = gsm->soundBank;
        if (bank)
        {
            indices = bank->soundIndices;
            if (indices)
            {
                result = gsm->soundIndex;
                if (result >= 0 && result < bank->elemNum)
                {
                    result = indices[result];
                    gsm->idx = result;
                    if (result >= 0 && result < SND_gst_Params.allocatedSoundsMaxNum)
                    {
                        result = SND_gst_Params.allocatedSounds[result].flags & 0x96008;
                        gsm->flags2 |= result;
                    }
                }
            }
        }
    }
    return result;
}

// Specific SoundModifier functions --------------------------------------------------------------

void SND_SModifierEmptyInit(tSndModifierElem* modifier, tSoundModifierDataEmpty* data, int dataSize)
{
    tSoundModifierDataEmpty* stored;

    if (!modifier->linkedSoundData)
        modifier->linkedSoundData = MEM_p_Alloc(sizeof(tSoundModifierDataEmpty));
    stored = (tSoundModifierDataEmpty*)modifier->linkedSoundData;
    modifier->linkedSoundDataSize = sizeof(tSoundModifierDataEmpty);
    *stored = *data;
}

void SND_SModifierEmptyDestroy(tSndModifierElem* modifier, int unk1, int unk2)
{
    MEM_Free(modifier->linkedSoundData);
    modifier->linkedSoundData = 0;
    modifier->linkedSoundDataSize = 0;
}

void SND_SModifierFadeInit(tSndModifierElem* modifier, tSoundModifierDataFade* data, int dataSize)
{
    long double duration;
    int mode;
    double startVol;
    tSoundModifierDataFade* stored;

    if (data->i2 >= 0.0000001)
    {
        if (data->i2 > 1.0)
            data->i2 = 1.0;
    }
    else
    {
        data->i2 = 0.0;
    }
    if (data->i1 >= 0.0000001)
    {
        if (data->i1 > 1.0)
            data->i1 = 1.0;
    }
    else
    {
        data->i1 = 0.0;
    }
    if (data->i3 == 0.0 || data->i1 == data->i2)
    {
        startVol = data->i1;
        data->i3 = 0.0000099999997f;
        data->i4 = 10000.0f;
        data->i2 = (float)(startVol + 0.000099999997);
    }
    else
    {
        duration = fabs(data->i3);
        mode = data->i5 & 0x83;
        data->i3 = (float)duration;
        if (mode == 1)
        {
            data->i4 = (float)((data->i2 - data->i1) / duration);
        }
        else if (mode == 2)
        {
            if (data->i1 == 0.0)
                data->i1 = 0.0000001f;
            if (data->i2 == 0.0)
                data->i2 = 0.0000001f;
            //a2->i4 = __FYL2X__(a2->i2 / a2->i1, 0.30102999566398119802) / v2;
            data->i4 = (float)((log10(2) * log2(data->i2 / data->i1)) / duration);
        }
        else
        {
            if (mode != 0x80)
                DBG_assert(0, __FILE__, __LINE__, "0", "Bad Fade identification", 0, 1);
            if (data->i1 == 0.0)
                data->i1 = 0.0000001f;
            if (data->i2 == 0.0)
                data->i2 = 0.0000001f;
            data->i6 = 0.0;
            data->i4 = (float)(255.0 / duration);
        }
    }
    if (!modifier->linkedSoundData)
        modifier->linkedSoundData = MEM_p_Alloc(sizeof(tSoundModifierDataFade));
    stored = (tSoundModifierDataFade*)modifier->linkedSoundData;
    modifier->linkedSoundDataSize = sizeof(tSoundModifierDataFade);
    *stored = *data;
}

void SND_SModifierFadeUpdate(tSndModifierElem* modifier, tSoundInstanceElem* inst, int dataSize)
{
    tSoundModifierDataFade* data;
    int mode;

    data = (tSoundModifierDataFade*)modifier->linkedSoundData;
    if ((data->i5 & 0x10) == 0)
        return;
    mode = data->i5 & 0x83;
    if (mode == 1)
        inst->volume = (float)(TIM_gf_dt * data->i4 + inst->volume);
    else if (mode == 2)
        inst->volume = (float)(pow(10.0, data->i4 * TIM_gf_dt) * inst->volume);
    if (inst->volume >= 0.0000001)
    {
        if (inst->volume > 1.0)
            inst->volume = 1.0;
    }
    else
    {
        inst->volume = 0.0;
    }
    if (data->i4 <= 0.0)
    {
        if (inst->volume <= data->i2)
        {
            inst->volume = data->i2;
            data->i5 &= ~0x10u;
            if ((data->i5 & 0x20) != 0)
                SND_Stop((unsigned int)((char*)inst - (char*)SND_gst_Params.soundInstances) / sizeof(tSoundInstanceElem));
            if ((data->i5 & 8) != 0)
                inst->flags |= 0x280u;
        }
    }
    else if (inst->volume >= data->i2)
    {
        inst->volume = data->i2;
        data->i5 &= ~0x10u;
    }
}

void SND_SModifierFadeInitInstance(struct sSndModifierElem* modifier, void* linkedsound, int dataSize, struct sSoundInstanceElem* inst)
{
    tSoundModifierDataFade* data = (tSoundModifierDataFade*)linkedsound;
    // preserved: the subtraction already yields an element index, so dividing by the size again collapses every instance onto 0 or 1
    SND_SetVol(((unsigned int)(inst - (struct sSoundInstanceElem*)SND_gst_Params.soundInstances) / sizeof(tSoundInstanceElem)), data->i1);
}

void SND_SModifierFadeInUpdate(tSndModifierElem* modifier, tSoundInstanceElem* inst, int dataSize)
{
    tSoundModifierDataFade* data;
    double elapsed;
    int step;
    double rounded;
    int mode;

    data = (tSoundModifierDataFade*)modifier->linkedSoundData;
    if ((data->i5 & 0x10) == 0)
        return;
    mode = data->i5 & 0x83;
    if (mode == 1)
    {
        inst->volume = (float)(TIM_gf_dt * data->i4 + inst->volume);
    }
    else if (mode == 2)
    {
        inst->volume = (float)(pow(10.0, data->i4 * TIM_gf_dt) * inst->volume);
    }
    else if (mode == 0x80)
    {
        elapsed = TIM_gf_dt + data->i6;
        data->i6 = (float)elapsed;
        rounded = elapsed * data->i4 + MATH_gd_magicDoubleForRound;
        // preserved: the subtraction is unsigned, so the lower clamp below it can never fire and a late step indexes before the table
        step = 255 - *(uint32_t*)&rounded;
        if (step >= 256)
            step = 255;
        inst->volume = SND_f_GetVolFromAtt(SND_SModifier_FadeTable[step]) * (data->i2 - data->i1) + data->i1;
        if (data->i6 > data->i3)
            inst->volume = data->i2;
    }
    if (inst->volume >= 0.0000001)
    {
        if (inst->volume > 1.0)
            inst->volume = 1.0;
    }
    else
    {
        inst->volume = 0.0;
    }
    if (inst->volume >= data->i2)
    {
        inst->volume = data->i2;
        data->i5 &= ~0x10u;
    }
}

void SND_SModifierFadeInOrOutNotifyPause(tSndModifierElem* modifier, tSoundInstanceElem* inst)
{
    tSoundModifierDataFade* data;
    int fadeFlags;

    data = (tSoundModifierDataFade*)modifier->linkedSoundData;
    fadeFlags = data->i5;
    if ((fadeFlags & 0x10) != 0)
    {
        data->i5 = (fadeFlags & ~0x10u) | 0x40;
    }
    else if ((fadeFlags & 0x40) != 0)
    {
        data->i5 = (fadeFlags & ~0x40u) | 0x10;
    }
}

void SND_SModifierFadeInNotifyPlay(tSndModifierElem* modifier, tSoundInstanceElem* inst)
{
    tSoundModifierDataFade* data;
    unsigned int fadeFlags;

    data = (tSoundModifierDataFade*)modifier->linkedSoundData;
    fadeFlags = data->i5 & ~0x40u;
    data->i5 = fadeFlags;
    if ((fadeFlags & 0x10) == 0)
    {
        data->i5 = fadeFlags | 0x10;
        data->i6 = 0.0;
        SND_SetVol(((unsigned int)((char*)inst - (char*)SND_gst_Params.soundInstances) / sizeof(tSoundInstanceElem)), data->i1);
    }
}

void  SND_SModifierFadeInNotifyStop(tSndModifierElem* modifier, tSoundInstanceElem* inst)
{
    tSoundModifierDataFade* data = (tSoundModifierDataFade*)(modifier->linkedSoundData);
    data->i5 &= ~0x50u;
}

void SND_SModifierFadeOutUpdate(struct sSndModifierElem* modifier, struct sSoundInstanceElem* inst, int dataSize)
{
    tSoundModifierDataFade* data;
    double elapsed;
    int step;
    double rounded;
    int mode;

    data = (tSoundModifierDataFade*)modifier->linkedSoundData;
    if ((data->i5 & 0x10) == 0)
        return;
    mode = data->i5 & 0x83;
    if (mode == 1)
    {
        inst->volume = (float)(TIM_gf_dt * data->i4 + inst->volume);
    }
    else if (mode == 2)
    {
        inst->volume = (float)(pow(10.0, data->i4 * TIM_gf_dt) * inst->volume);
    }
    else if (mode == 0x80)
    {
        elapsed = TIM_gf_dt + data->i6;
        data->i6 = (float)elapsed;
        rounded = elapsed * data->i4 + MATH_gd_magicDoubleForRound;
        step = *(int32_t*)&rounded;
        if (step < 0)
            step = 0;
        else if (step > 255)
            step = 255;
        inst->volume = SND_f_GetVolFromAtt(SND_SModifier_FadeTable[step]) * (data->i1 - data->i2) + data->i2;
        if (data->i6 > data->i3)
            inst->volume = data->i2;
    }
    if (inst->volume >= 0.0000001)
    {
        if (inst->volume > 1.0)
            inst->volume = 1.0;
    }
    else
    {
        inst->volume = 0.0;
    }
    if (inst->volume <= data->i2)
    {
        inst->volume = data->i2;
        data->i5 &= ~0x10u;
        if ((data->i5 & 0x20) != 0)
            SND_Stop((unsigned int)((char*)inst - (char*)SND_gst_Params.soundInstances) / sizeof(tSoundInstanceElem));
        if ((data->i5 & 8) != 0)
            inst->flags |= 0x280u;
    }
}

void SND_SModifierFadeOutNotifyPlay(tSndModifierElem* modifier, tSoundInstanceElem* inst)
{
    tSoundModifierDataFade* data;

    data = (tSoundModifierDataFade*)modifier->linkedSoundData;
    data->i5 &= ~0x50u;
    data->i6 = 0.0;
}

void SND_SModifierFadeOutNotifyStop(tSndModifierElem* modifier, tSoundInstanceElem* inst)
{
    tSoundModifierDataFade* data;

    data = (tSoundModifierDataFade*)modifier->linkedSoundData;
    data->i5 = (data->i5 & ~0x40u) | 0x10;
    data->i6 = 0.0;
}

void SND_SModifierPlayInit(tSndModifierElem* modifier, tSoundModifierDataPlayer* source, int dataSize)
{
    tSoundModifierDataPlayer* data;
    tSoundModifierDataPlayer* upgraded;
    int freq;

    data = source;
    upgraded = 0;
    if (dataSize == 28 || dataSize == 36)
    {
        upgraded = (tSoundModifierDataPlayer*)MEM_p_Alloc(sizeof(tSoundModifierDataPlayer));
        memcpy(upgraded, source, (unsigned int)dataSize);
        if (dataSize == 28)
            upgraded->i6 = 0;
        upgraded->timeRemainTillStart = 0.0;
        data = upgraded;
    }
    if (data->i6 == 0)
    {
        data->flags = 64;
        data->i6 = 2;
    }
    else if (data->i6 == 1)
    {
        data->i6 = 2;
    }
    if (data->vol >= 0.0000001)
    {
        if (data->vol > 1.0)
            data->vol = 1.0;
    }
    else
    {
        data->vol = 0.0;
    }
    if (data->ffar < 0.0)
        data->ffar = -data->ffar;
    if (data->fnear < 0.0)
        data->fnear = -data->fnear;
    freq = data->freq;
    if (freq < 0)
        data->freq = -freq;
    if (data->timeRemainTillStart < 0.0)
        data->timeRemainTillStart = 0.0;
    if (data->timeRemainTillStart > 1000000.0)
        data->timeRemainTillStart = 0.0;
    if (!modifier->linkedSoundData)
        modifier->linkedSoundData = MEM_p_Alloc(sizeof(tSoundModifierDataPlayer));
    modifier->linkedSoundDataSize = sizeof(tSoundModifierDataPlayer);
    memcpy(modifier->linkedSoundData, data, sizeof(tSoundModifierDataPlayer));
    if (upgraded)
        MEM_Free(upgraded);
}

void SND_SModifierPlayApplyOnInstance(tSndModifierElem* modifier, void* llinkedsound, int dataSize, tSoundInstanceElem* inst)
{
    int freq;
    tSoundModifierDataPlayer* linkedsound = (tSoundModifierDataPlayer*)llinkedsound;

    SND_SetNearFar(((unsigned int)((char*)inst - (char*)SND_gst_Params.soundInstances) / sizeof(tSoundInstanceElem)), 4096, linkedsound->fnear, linkedsound->ffar);
    inst->volume = linkedsound->vol;
    freq = linkedsound->freq;
    if (freq)
        inst->freq = freq;
    if ((linkedsound->gen.flags & 0x100000) != 0)
        inst->flags |= 0x100000u;
    if (linkedsound->flags)
        SND_SetFlags(0, ((unsigned int)((char*)inst - (char*)SND_gst_Params.soundInstances) / sizeof(tSoundInstanceElem)), (linkedsound->flags & 0xFCB) | 0x40000000);
}

void SND_SModifierPlayerNotifyPlay(tSndModifierElem* modifier, tSoundInstanceElem* inst)
{
    tSoundModifierDataPlayer* data = (tSoundModifierDataPlayer*)(modifier->linkedSoundData);

    inst->timeRemainTillStart = data->timeRemainTillStart;
}

void SND_SModifierPlayApplyOnSound(tSndModifierElem* modifier, void* linkedsound, int linkedsoundsize, tSndSoundElem* sound)
{
    tSoundModifierDataPlayer* data;
    int soundFlags;

    data = (tSoundModifierDataPlayer*)modifier->linkedSoundData;
    sound->freq = data->freq;
    soundFlags = data->gen.flags;
    if (soundFlags)
        sound->flags |= soundFlags;
}

void SND_SModifierExtPlayerInit(tSndModifierElem* modifier, tSoundModifierDataExtPlayer* source, int dataSize)
{
    tSoundModifierDataExtPlayer* data;
    tSoundModifierDataExtPlayer* upgraded;
    int sourceFlags;
    tSoundModifierDataExtPlayer* stored;
    int keptFlags;
    int keptI1;

    data = source;
    upgraded = 0;
    if (source->i6 <= 0.0000099999997)
        source->i6 = 0.0;
    if (source->i7 <= 0.0000099999997)
        source->i7 = 0.0;
    if (source->trackID != -1 || (source->gen.flags & 0x20000) != 0)
    {
        sourceFlags = source->gen.flags;
        if ((sourceFlags & 0x86000) == 0)
        {
            source->trackID = -1;
            source->gen.flags = sourceFlags & ~0x20000u;
        }
    }
    if (!modifier->linkedSoundData)
        modifier->linkedSoundData = MEM_p_Alloc(sizeof(tSoundModifierDataExtPlayer));
    stored = (tSoundModifierDataExtPlayer*)modifier->linkedSoundData;
    modifier->linkedSoundDataSize = sizeof(tSoundModifierDataExtPlayer);
    switch (dataSize)
    {
    case 84:
        upgraded = (tSoundModifierDataExtPlayer*)MEM_p_Alloc(sizeof(tSoundModifierDataExtPlayer));
        memcpy(upgraded, source, 84u);
        upgraded->i25 = 0;
        data = upgraded;
        break;
    case 108:
        upgraded = (tSoundModifierDataExtPlayer*)MEM_p_Alloc(sizeof(tSoundModifierDataExtPlayer));
        memcpy(upgraded, source, 108u);
        upgraded->i25 = 1;
        data = upgraded;
        break;
    case 112:
        upgraded = (tSoundModifierDataExtPlayer*)MEM_p_Alloc(sizeof(tSoundModifierDataExtPlayer));
        memcpy(upgraded, source, 112u);
        upgraded->i25 = 2;
        data = upgraded;
        break;
    default:
        break;
    }
    switch (data->i25)
    {
    case 0:
        data->interpolationFactor = 0.5;
        data->interpolationFactor2 = 0.5;
        data->interpolationFactor3 = 0.5;
        data->farCoefficient = 0.0;
        data->middleCoefficient = 0.5;
        data->nearfar = 0.0;
    case 1:
        data->i1 = (data->i1 & 0x80B) | 0x40;
    case 2:
        data->timeRemainTillStart = 0.0;
    case 3:
        data->i25 = 3;
        memcpy(stored, data, sizeof(tSoundModifierDataExtPlayer));
        break;
    default:
        break;
    }
    keptFlags = stored->gen.flags & 0x5F1B700C;
    keptI1 = stored->i1 & 0xFCB;
    stored->idx = -1;
    stored->gen.flags = keptFlags;
    stored->i1 = keptI1;
    if (upgraded)
        MEM_Free(upgraded);
}

void SND_SModifierExtPlayerApplyOnInstance(tSndModifierElem* sndModElem, tSoundModifierDataPlayer* linkedSound, int dataSize, tSoundInstanceElem* sndInstElem)
{
    tSoundModifierDataExtPlayer* extPlayer;
    tSoundModifierDataFade fadeData;
    int shouldCoeffSet;
    float interpolatedVal;

    extPlayer = (tSoundModifierDataExtPlayer*)sndModElem->linkedSoundData;
    extPlayer->idx = (unsigned int)((char*)sndInstElem - (char*)SND_gst_Params.soundInstances) / sizeof(tSoundInstanceElem);
    
    SND_SetVol(extPlayer->idx, extPlayer->vol2);
    SND_SetFlags(0, extPlayer->idx, extPlayer->gen.flags);
    SND_SetFlags(0, extPlayer->idx, (extPlayer->i1 & 0xFCB) | 0x40000000);
    
    if (sndInstElem->gao)
    {
        SND_i_CheckOrInitGAO(sndInstElem->gao);
        SND_SetPos(sndInstElem->gao, extPlayer->idx, sndInstElem->gao->matrices);
    }
    if (extPlayer->freq)
        SND_SetFreq(extPlayer->idx, extPlayer->freq);
    if (extPlayer->pan)
        SND_SetPan(extPlayer->idx, extPlayer->pan, 0);

    shouldCoeffSet = 0;

    if ((extPlayer->gen.flags & 0x10000000) != 0)
    {
        shouldCoeffSet = 1;
        SND_SetNearFar(extPlayer->idx, 4096, extPlayer->fnear, extPlayer->ffar);
        if (extPlayer->interpolationFactor != 0.5f)
        {
            interpolatedVal = MATH_LinearInterpolateFloats(extPlayer->fnear, extPlayer->ffar, extPlayer->interpolationFactor);
            SND_i_InstanceSetf(extPlayer->idx, 4099, interpolatedVal);
        }
    }
    else
    {
        if ((extPlayer->gen.flags & 0x1000000) != 0)
        {
            shouldCoeffSet = 1;
            SND_SetNearFar(extPlayer->idx, 4096, extPlayer->fnear, extPlayer->ffar);
            if (extPlayer->interpolationFactor != 0.5f)
            {
                interpolatedVal = MATH_LinearInterpolateFloats(extPlayer->fnear, extPlayer->ffar, extPlayer->interpolationFactor);
                SND_i_InstanceSetf(extPlayer->idx, 4099, interpolatedVal);
            }
        }
        if ((extPlayer->gen.flags & 0x2000000) != 0)
        {
            shouldCoeffSet = 1;
            SND_SetNearFar(extPlayer->idx, 4097, extPlayer->fnear2, extPlayer->ffar2);
            if (extPlayer->interpolationFactor2 != 0.5f)
            {
                interpolatedVal = MATH_LinearInterpolateFloats(extPlayer->fnear2, extPlayer->ffar2, extPlayer->interpolationFactor2);
                SND_i_InstanceSetf(extPlayer->idx, 4100, interpolatedVal);
            }
        }
        if ((extPlayer->gen.flags & 0x4000000) != 0)
        {
            shouldCoeffSet = 1;
            SND_SetNearFar(extPlayer->idx, 4098, extPlayer->fnear3, extPlayer->ffar3);
            if (extPlayer->interpolationFactor3 != 0.5f)
            {
                interpolatedVal = MATH_LinearInterpolateFloats(extPlayer->fnear3, extPlayer->ffar3, extPlayer->interpolationFactor3);
                SND_i_InstanceSetf(extPlayer->idx, 4101, interpolatedVal);
            }
        }
    }
    if ((extPlayer->gen.flags & 0x8000000) != 0)
    {
        SND_SetNearFar(extPlayer->idx, 4105, extPlayer->nearfar, extPlayer->nearfar);
        SND_i_InstanceSetf(extPlayer->idx, 4102, extPlayer->farCoefficient);
        SND_i_InstanceSetf(extPlayer->idx, 4103, extPlayer->middleCoefficient);
    }
    else if (shouldCoeffSet)
    {
        SND_i_InstanceSetf(extPlayer->idx, 4102, extPlayer->farCoefficient);
        SND_i_InstanceSetf(extPlayer->idx, 4103, extPlayer->middleCoefficient);
    }

    if ((extPlayer->i1 & 3) != 0)
        SND_i_InstanceSetf(extPlayer->idx, 4104, extPlayer->unkIsInActiveVolume);

    if (extPlayer->trackID != -1 && extPlayer->trackID)
        SND_SetTrack(extPlayer->idx, extPlayer->trackID);

    if (extPlayer->i6 == 0.0f)
    {
        SND_SetVol(extPlayer->idx, extPlayer->vol);
    }
    else
    {
        fadeData.i3 = extPlayer->i6;
        fadeData.i2 = extPlayer->vol;
        fadeData.i1 = extPlayer->vol2;
        fadeData.i5 = 1;
        fadeData.gen.flags = 0;
        fadeData.gen.sndKey = extPlayer->gen.sndKey;
        fadeData.gen.sndIndexInAllocatedSounds = extPlayer->gen.sndIndexInAllocatedSounds;
        SND_SModifierAddInInstance(extPlayer->idx, 3, &fadeData, 36);// fade in
    }

    if (extPlayer->i7 != 0.0f)
    {
        fadeData.i3 = extPlayer->i7;
        fadeData.i2 = extPlayer->vol2;
        fadeData.i1 = extPlayer->vol;
        fadeData.i5 = 128;
        if ((extPlayer->i3 & 0x10) != 0)
            fadeData.i5 = 160;
        if ((extPlayer->i3 & 0x20) != 0)
            fadeData.i5 |= 8u;
        fadeData.gen.flags = 0;
        fadeData.gen.sndKey = extPlayer->gen.sndKey;
        fadeData.gen.sndIndexInAllocatedSounds = extPlayer->gen.sndIndexInAllocatedSounds;
        SND_SModifierAddInInstance(extPlayer->idx, 4, &fadeData, 36);// fade out
    }

    if (extPlayer->familyID)
        SND_LinkToFamilly(extPlayer->familyID, extPlayer->idx);

    if ((extPlayer->i3 & 1) != 0)
    {
        if ((extPlayer->gen.flags & 0x20000) != 0)
        {
            if ((extPlayer->gen.flags & 4) != 0)
                SND_PlaySoundLoopingOnTrack(extPlayer->idx);
            else
                SND_PlaySoundOnTrack(extPlayer->idx);
        }
        else if ((extPlayer->gen.flags & 4) != 0)
        {
            SND_PlaySoundLooping(extPlayer->idx);
        }
        else
        {
            SND_PlaySound(extPlayer->idx);
        }
    }
}

void SND_SModifierExtPlayerNotifyPlay(tSndModifierElem* sndModElem, tSoundInstanceElem* sndInstElem)
{
    tSoundModifierDataExtPlayer* extPlayer;

    if (!SND_pst_SModifierGet(sndInstElem->modifierInstances, 3))
    {
        extPlayer = (tSoundModifierDataExtPlayer*)sndModElem->linkedSoundData;
        SND_SetVol((unsigned int)((char*)sndInstElem - (char*)SND_gst_Params.soundInstances) / sizeof(tSoundInstanceElem), extPlayer->vol);
        sndInstElem->timeRemainTillStart = extPlayer->timeRemainTillStart;
    }
}

void SND_SModifier3DSettingsApplyOnInstance(tSndModifierElem* modifier, void* linkedsound, int dataSize, tSoundInstanceElem* inst)
{
    tSoundModifierData3DSettings* data;
    double factor;
    int instanceIdx;
    float blend;

    data = (tSoundModifierData3DSettings*)modifier->linkedSoundData;
    instanceIdx = (unsigned int)((char*)inst - (char*)SND_gst_Params.soundInstances) / sizeof(tSoundInstanceElem);
    factor = data->i1;
    if (factor >= 0.0)
    {
        if (factor > 1.0)
            factor = 1.0;
    }
    else
    {
        factor = 0.0;
    }
    // preserved: all three near/far channels blend i1 over nearArr[0]/farArr[0]; i2, i3 and index 1 and 2 are never read
    blend = (float)((1.0 - factor) * inst->nearArr[0] + factor * inst->farArr[0]);
    SND_i_InstanceSetf(instanceIdx, 4099, blend);
    SND_i_InstanceSetf(instanceIdx, 4100, blend);
    SND_i_InstanceSetf(instanceIdx, 4101, blend);
    SND_i_InstanceSetf(instanceIdx, 4102, data->farCoefficient);
    SND_i_InstanceSetf(instanceIdx, 4103, data->middleCoefficient);
    SND_SetNearFar(instanceIdx, 4105, data->nearfar, data->nearfar);
}

void SND_SModifier3DSettingsInit(tSndModifierElem* modifier, tSoundModifierData3DSettings* data, int dataSize)
{
    tSoundModifierData3DSettings* stored;
    int keptFlags;

    if (!modifier->linkedSoundData)
        modifier->linkedSoundData = MEM_p_Alloc(sizeof(tSoundModifierData3DSettings));
    stored = (tSoundModifierData3DSettings*)modifier->linkedSoundData;
    modifier->linkedSoundDataSize = sizeof(tSoundModifierData3DSettings);
    memcpy(stored, data, sizeof(tSoundModifierData3DSettings));
    keptFlags = stored->gen.flags & 0x5F1B700C;
    stored->gen.sndIndexInAllocatedSounds = -1;
    stored->gen.flags = keptFlags;
}

void SND_SModifierEmptyApplyOnInstance (struct sSndModifierElem* modifier, void* linkedSound, int dataSize, struct sSoundInstanceElem* inst) {
}

void SND_SModifierEmptyApplyOnSound(struct sSndModifierElem* modifier, void* linkedSound, int dataSize, struct sSndSoundElem* sound) {
}

void SND_SModifierFadeApplyOnInstance(struct sSndModifierElem* modifier, void* linkedSound, int dataSize, struct sSoundInstanceElem* inst) {
}

void SND_SModifierExtPlayerApplyOnSound(struct sSndModifierElem* modifier, void* linkedSound, int dataSize, struct sSndSoundElem* sound) {
}

int SND_InitMdFModule()
{
    SND_gst_BankMdFUserList = 0;
    sndMdfBankUser = 0;
    if (SND_gst_Params.soundEnabled)
    {
        SND_gst_BankMdFUserList = 16;
        sndMdfBankUser = (tSoundMdfStruct**)MEM_p_Alloc(16 * 4);
        memset(sndMdfBankUser, 0, 4 * SND_gst_BankMdFUserList);
    }
    return 0;
}

void SND_SModifierInitModule()
{
    tSoundModifierElemIntf* intf;
    tSoundModifierDataEmpty* emptyData;
    tSoundModifierDataFade* fadeInData;
    tSoundModifierDataFade* fadeOutData;
    tSoundModifierDataPlayer* playerData;
    tSoundModifierDataExtPlayer* extPlayerData;
    tSoundModifierData3DSettings* settingsData;

    memset(SND_gs_SoundModifiers, 0, sizeof(SND_gs_SoundModifiers));

    intf = (tSoundModifierElemIntf*)MEM_p_Alloc(sizeof(tSoundModifierElemIntf));
    SND_gs_SoundModifiers[0].sndModifierModelIntf = intf;
    intf->Type = 0;
    intf->Destroy = SND_SModifierEmptyDestroy;
    intf->Init = SND_SModifierEmptyInit;
    intf->Update = NULL;
    intf->InitChainedElemVersion = SND_SModifierEmptyInit;
    intf->PlayApplyOnInstance = SND_SModifierEmptyApplyOnInstance;
    intf->PlayApplyOnSound = SND_SModifierEmptyApplyOnSound;
    intf->NotifyPause = NULL;
    intf->NotifyPlay = NULL;
    intf->NotifyStop = NULL;
    SND_gs_SoundModifiers[0].nextModifier = 0;
    emptyData = (tSoundModifierDataEmpty*)MEM_p_Alloc(sizeof(tSoundModifierDataEmpty));
    SND_gs_SoundModifiers[0].linkedSoundData = (char *)emptyData;
    SND_gs_SoundModifiers[0].linkedSoundDataSize = sizeof(tSoundModifierDataEmpty);
    emptyData->gen.sndIndexInAllocatedSounds = -1;
    emptyData->gen.flags = 0;
    emptyData->gen.sndKey = -1;

    intf = (tSoundModifierElemIntf*)MEM_p_Alloc(sizeof(tSoundModifierElemIntf));
    SND_gs_SoundModifiers[1].sndModifierModelIntf = intf;
    memcpy(intf, SND_gs_SoundModifiers[0].sndModifierModelIntf, sizeof(tSoundModifierElemIntf));
    intf->Type = 1;
    intf->Init = SND_SModifierFadeInit;
    intf->Update = SND_SModifierFadeUpdate;
    intf->InitChainedElemVersion = SND_SModifierFadeInit;
    intf->PlayApplyOnInstance = SND_SModifierFadeInitInstance;
    fadeInData = (tSoundModifierDataFade*)MEM_p_Alloc(sizeof(tSoundModifierDataFade));
    SND_gs_SoundModifiers[1].linkedSoundData = (char*)fadeInData;
    SND_gs_SoundModifiers[1].linkedSoundDataSize = sizeof(tSoundModifierDataFade);
    fadeInData->gen.sndKey = -1;
    fadeInData->gen.sndIndexInAllocatedSounds = -1;
    fadeInData->gen.flags = 0;
    fadeInData->i5 = 1;
    fadeInData->i1 = 0.0;
    fadeInData->i2 = 1.0;
    fadeInData->i3 = 1.0;
    fadeInData->i4 = 1.0;

    intf = (tSoundModifierElemIntf*)MEM_p_Alloc(sizeof(tSoundModifierElemIntf));
    SND_gs_SoundModifiers[3].sndModifierModelIntf = intf;
    memcpy(intf, SND_gs_SoundModifiers[1].sndModifierModelIntf, sizeof(tSoundModifierElemIntf));
    intf->Type = 3;
    intf->Init = SND_SModifierFadeInit;
    intf->Update = SND_SModifierFadeInUpdate;
    intf->InitChainedElemVersion = SND_SModifierFadeInit;
    intf->PlayApplyOnInstance = SND_SModifierFadeApplyOnInstance;
    intf->NotifyPause = SND_SModifierFadeInOrOutNotifyPause;
    intf->NotifyPlay = SND_SModifierFadeInNotifyPlay;
    intf->NotifyStop = SND_SModifierFadeInNotifyStop;
    SND_gs_SoundModifiers[3].linkedSoundData = SND_gs_SoundModifiers[1].linkedSoundData;
    SND_gs_SoundModifiers[3].linkedSoundDataSize = SND_gs_SoundModifiers[1].linkedSoundDataSize;

    intf = (tSoundModifierElemIntf*)MEM_p_Alloc(sizeof(tSoundModifierElemIntf));
    SND_gs_SoundModifiers[4].sndModifierModelIntf = intf;
    memcpy(intf, SND_gs_SoundModifiers[3].sndModifierModelIntf, sizeof(tSoundModifierElemIntf));
    intf->Type = 4;
    intf->Init = SND_SModifierFadeInit;
    intf->Update = SND_SModifierFadeOutUpdate;
    intf->InitChainedElemVersion = SND_SModifierFadeInit;
    intf->NotifyPause = SND_SModifierFadeInOrOutNotifyPause;
    intf->NotifyPlay = SND_SModifierFadeOutNotifyPlay;
    intf->NotifyStop = SND_SModifierFadeOutNotifyStop;
    fadeOutData = (tSoundModifierDataFade*)MEM_p_Alloc(sizeof(tSoundModifierDataFade));
    SND_gs_SoundModifiers[4].linkedSoundData = (char *)fadeOutData;
    SND_gs_SoundModifiers[4].linkedSoundDataSize = sizeof(tSoundModifierDataFade);
    fadeOutData->gen.sndKey = -1;
    fadeOutData->gen.flags = 0;
    fadeOutData->gen.sndIndexInAllocatedSounds = -1;
    fadeOutData->i5 = 0x80;
    fadeOutData->i1 = 1.0;
    fadeOutData->i2 = 0.0;
    fadeOutData->i3 = 1.0;
    fadeOutData->i4 = 1.0;

    intf = (tSoundModifierElemIntf*)MEM_p_Alloc(sizeof(tSoundModifierElemIntf));
    SND_gs_SoundModifiers[2].sndModifierModelIntf = intf;
    memcpy(intf, SND_gs_SoundModifiers[0].sndModifierModelIntf, sizeof(tSoundModifierElemIntf));
    intf->Type = 2;
    intf->Init = SND_SModifierPlayInit;
    intf->InitChainedElemVersion = SND_SModifierPlayInit;
    intf->PlayApplyOnInstance = SND_SModifierPlayApplyOnInstance;
    intf->PlayApplyOnSound = SND_SModifierPlayApplyOnSound;
    intf->NotifyPlay = SND_SModifierPlayerNotifyPlay;
    playerData = (tSoundModifierDataPlayer*)MEM_p_Alloc(sizeof(tSoundModifierDataPlayer));
    SND_gs_SoundModifiers[2].linkedSoundData = (char *)playerData;
    SND_gs_SoundModifiers[2].linkedSoundDataSize = sizeof(tSoundModifierDataPlayer);
    playerData->gen.sndKey = -1;
    playerData->gen.flags = 0x100000;
    playerData->gen.sndIndexInAllocatedSounds = -1;
    playerData->ffar = 20.0f;
    playerData->fnear = 0.0;
    playerData->vol = 0.5f;
    playerData->freq = 44100;
    playerData->flags = 64;
    playerData->i6 = 2;
    playerData->timeRemainTillStart = 0.0;

    intf = (tSoundModifierElemIntf*)MEM_p_Alloc(sizeof(tSoundModifierElemIntf));
    SND_gs_SoundModifiers[5].sndModifierModelIntf = intf;
    memcpy(intf, SND_gs_SoundModifiers[2].sndModifierModelIntf, sizeof(tSoundModifierElemIntf));
    intf->Type = 5;
    intf->Init = SND_SModifierExtPlayerInit;
    intf->InitChainedElemVersion = SND_SModifierExtPlayerInit;
    intf->PlayApplyOnInstance = SND_SModifierExtPlayerApplyOnInstance;
    intf->PlayApplyOnSound = SND_SModifierExtPlayerApplyOnSound;
    intf->NotifyPlay = SND_SModifierExtPlayerNotifyPlay;
    extPlayerData = (tSoundModifierDataExtPlayer*)MEM_p_Alloc(sizeof(tSoundModifierDataExtPlayer));
    SND_gs_SoundModifiers[5].linkedSoundData = (char *)extPlayerData;
    SND_gs_SoundModifiers[5].linkedSoundDataSize = sizeof(tSoundModifierDataExtPlayer);
    extPlayerData->gen.sndKey = -1;
    extPlayerData->gen.sndIndexInAllocatedSounds = -1;
    extPlayerData->idx = -1;
    extPlayerData->trackID = -1;
    extPlayerData->gen.flags = 0x100000;
    extPlayerData->i1 = 64;
    extPlayerData->i3 = 16;
    extPlayerData->ffar = 20.0f;
    extPlayerData->ffar2 = 20.0f;
    extPlayerData->ffar3 = 20.0f;
    extPlayerData->fnear = 0.0;
    extPlayerData->fnear2 = 0.0;
    extPlayerData->fnear3 = 0.0;
    extPlayerData->unkIsInActiveVolume = 2.0f;
    extPlayerData->i6 = 0.0;
    extPlayerData->i7 = 0.0;
    extPlayerData->vol = 0.5;
    extPlayerData->vol2 = 0.0;
    extPlayerData->familyID = 0;
    extPlayerData->pan = 0;
    extPlayerData->freq = 0;
    extPlayerData->interpolationFactor = 0.5f;
    extPlayerData->interpolationFactor2 = 0.5f;
    extPlayerData->interpolationFactor3 = 0.5f;
    extPlayerData->farCoefficient = 0.0;
    extPlayerData->middleCoefficient = 0.5f;
    extPlayerData->nearfar = 0.0;
    extPlayerData->i25 = 3;
    extPlayerData->timeRemainTillStart = 0.0;

    intf = (tSoundModifierElemIntf*)MEM_p_Alloc(sizeof(tSoundModifierElemIntf));
    SND_gs_SoundModifiers[6].sndModifierModelIntf = intf;
    memcpy(intf, SND_gs_SoundModifiers[0].sndModifierModelIntf, sizeof(tSoundModifierElemIntf));
    intf->Type = 6;
    intf->Init = SND_SModifier3DSettingsInit;
    intf->InitChainedElemVersion = SND_SModifier3DSettingsInit;
    intf->PlayApplyOnInstance = SND_SModifier3DSettingsApplyOnInstance;
    settingsData = (tSoundModifierData3DSettings*)MEM_p_Alloc(sizeof(tSoundModifierData3DSettings));
    SND_gs_SoundModifiers[6].linkedSoundData = (char *)settingsData;
    SND_gs_SoundModifiers[6].linkedSoundDataSize = sizeof(tSoundModifierData3DSettings);
    settingsData->middleCoefficient = 0.5f;
    settingsData->i1 = 0.5f;
    settingsData->i2 = 0.5f;
    settingsData->i3 = 0.5f;
    settingsData->gen.flags = 0;
    settingsData->farCoefficient = 0.0;
    settingsData->nearfar = 0.0;
    settingsData->gen.sndKey = -1;
    settingsData->gen.sndIndexInAllocatedSounds = -1;
}
