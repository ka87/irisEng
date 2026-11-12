#include "../memory.h"
#include "../sound.h"
#include "../loading.h"
#include "gensoundmodifier.h"

void GEN_ModifierSound_Create(struct sGameObjectMain* gao, struct sModifier* mdf, void* readptr)
{
    tSoundMdfStruct* gsm;

    gsm = (tSoundMdfStruct*)MEM_p_Alloc(sizeof(tSoundMdfStruct));
    if (gsm)
    {
        memset(gsm, 0, sizeof(tSoundMdfStruct));
        gsm->nearFarFactor = 0.5f;
        gsm->nearFar2factor = 0.5f;
        gsm->nearFar3factor = 0.5f;
        gsm->f4 = 0.5f;
        gsm->i1 = 273;
        gsm->flags = 16;
        gsm->modifierID = -1;
        gsm->ii5 = 0;
        gsm->far1 = 20.0f;
        gsm->near1 = 0.0;
        gsm->far2 = 20.0f;
        gsm->near2 = 0.0;
        gsm->far3 = 20.0f;
        gsm->near3 = 0.0;
        gsm->xx = 0.0;
        gsm->unkIsInActiveVolume = 2.0f;
        gsm->ii2 = 0.0;
        gsm->ii8 = 1;
        gsm->ii4 = 0.0;
        gsm->ii9 = 128;
        gsm->volume = 1.0;
        gsm->volume2 = 0.0;
        gsm->pan = 0;
        gsm->b0 = 0.0;
        gsm->freq2 = 0;
        gsm->flags2 = 0;
        gsm->ii11 = 0;
        gsm->idx = -1;
        gsm->sndInstanceID = -1;
        gsm->i4 = -1;
        gsm->trackID = -1;
        gsm->soundIndex = -1;
        gsm->soundBank = 0;
        gsm->distPrefetchVolume = 60.0f;
        gsm->b3 = 0;
        gsm->b4 = 0;
        gsm->freq = 0.0;
        mdf->mdfPrivateDataPtr = gsm;
        
        if (readptr)
        {
            mdf->modifierIntf->Load(mdf, (char*)readptr);
            gsm->sndInstanceID = -1;
            gsm->flags &= ~0x84;
            gsm->flags2 &= 0x5F1B700C;
            gsm->ii11 &= 0xFCB;
        }

        mdf->modifierFlags |= 0x4000000;
    }
    else
    {
        mdf->mdfPrivateDataPtr = NULL;
        mdf->modifierFlags |= 0x4000000;
    }
}

void GEN_ModifierSound_Destroy(struct sModifier* mdf)
{
    tSoundMdfStruct* gsm = (tSoundMdfStruct*)mdf->mdfPrivateDataPtr;

    if (gsm)
    {
        if (gsm->idx != -1)
            SND_StreamFlushOneSound_0(gsm->idx, 1);
        
        if ((gsm->flags & 0x800) != 0)
        {
            if (gsm->soundBank)
                SND_FreeSndBank(&gsm->soundBank);

            gsm->soundBank = 0;
            gsm->idx = -1;
            SND_DelMdFBankUser(gsm);
        }

        if (gsm->idx != -1)
            SND_DeleteByIndex(gsm->idx);

        MEM_Free(mdf->mdfPrivateDataPtr);
        
        mdf->mdfPrivateDataPtr = NULL;
    }
    mdf->modifierFlags = 0x2000000;
}

void GEN_ModifierSound_Apply(struct sModifier* mdf, struct sGameObjectMain* gao)
{
    tSoundMdfStruct* gsm = (tSoundMdfStruct*)mdf->mdfPrivateDataPtr;

    if (gsm->idx == -1)
        MDF_SndGetSoundIndexFromBank(gsm);

    if (gsm->idx < 0) return;

    if (gsm->sndInstanceID >= 0)
    {
        if ((SND_gst_Params.soundInstances[gsm->sndInstanceID].flags & 0x400) == 0 || gsm->idx != SND_gst_Params.soundInstances[gsm->sndInstanceID].allocatedSoundsIdx)
            gsm->sndInstanceID = -1;
    }
    if (gsm->sndInstanceID == -1)
    {
        MDF_SndCreateInstance(gsm, gao);
    }
    else
    {
        if ((gsm->flags & 0x84) != 0)
        {
            if ((gsm->flags & 0x84) == 0x4 && !SND_i_IsPlaying(gsm->sndInstanceID))
            {
                gsm->flags &= ~0x84;
                SND_Stop(gsm->sndInstanceID);
            }
        }
        else
        {
            if ((gsm->flags & 0x8000) == 0 && (gsm->flags & 0x2000) != 0 && MDF_b_SndIsInPrefetchVolume(gsm))
            {
                SND_StreamPrefetchOneSound(gsm->idx, 1);
                gsm->flags |= 0x8000;
            }
            if ((gsm->trackID == -1 || (gsm->flags & 0x400) == 0 || !SND_i_IsTrackPlaying(gsm->trackID))
                && (gsm->flags & 0x100) != 0
                && MDF_b_SndIsInActiveVolume(gsm))
            {
                SND_PlayMdF(gsm);
            }
        }
    }
}

int GEN_ModifierSound_Load(tModifier* mdf, char* data)
{
    tSoundMdfStruct* gsm;
    char* start;
    int version;
    int verIs266;
    int verUpTo267;
    int verUpTo268;
    int verUpTo269;
    int verUpTo270;
    int verUpTo272;
    int zoneChecked;
    int flags;
    int flags2;
    int channelBits;
    int soundKey;
    double volume;
    double volume2;

    gsm = (tSoundMdfStruct*)mdf->mdfPrivateDataPtr;
    start = data;
    version = LOA_ReadLong(&data, 0, 1);
    if (!gsm || version < 266 || version > 273)
        return data - start;

    verIs266 = (version == 266);
    verUpTo267 = (version <= 267);
    verUpTo268 = (version <= 268);
    verUpTo269 = (version <= 269);
    verUpTo270 = (version <= 270);
    verUpTo272 = (version <= 272);
    zoneChecked = 0;

    gsm->flags = LOA_ReadLong(&data, 0, 1);
    gsm->modifierID = LOA_ReadLong(&data, 0, 1);
    gsm->i4 = LOA_ReadLong(&data, 0, 1);
    gsm->volume = LOA_ReadFloat(&data, 0, 1);
    volume2 = LOA_ReadFloat(&data, 0, 1);
    gsm->volume2 = (float)volume2;
    if (verIs266)
    {
        volume = gsm->volume + gsm->volume;
        gsm->volume = (float)volume;
        if (volume < 0.0000001)
            gsm->volume = 0.0;
        else if (volume > 1.0)
            gsm->volume = 1.0;

        volume2 = volume2 + volume2;
        gsm->volume2 = (float)volume2;
        if (volume2 < 0.0000001)
            gsm->volume2 = 0.0;
        else if (volume2 > 1.0)
            gsm->volume2 = 1.0;
    }

    gsm->ii2 = LOA_ReadFloat(&data, 0, 1);
    gsm->distPrefetchVolume = LOA_ReadFloat(&data, 0, 1);
    gsm->ii4 = LOA_ReadFloat(&data, 0, 1);
    gsm->ii5 = LOA_ReadLong(&data, 0, 1);
    if (gsm->ii2 <= 0.0000099999997)
        gsm->ii2 = 0.0;
    if (gsm->ii4 <= 0.0000099999997)
        gsm->ii4 = 0.0;
    gsm->freq2 = LOA_ReadLong(&data, 0, 1);
    gsm->pan = LOA_ReadLong(&data, 0, 1);
    gsm->ii8 = LOA_ReadLong(&data, 0, 1);
    gsm->ii9 = LOA_ReadLong(&data, 0, 1);
    if (verUpTo272)
    {
        gsm->ii8 = 1;
        gsm->ii9 = 128;
    }
    gsm->flags2 = LOA_ReadLong(&data, 0, 1);
    gsm->ii11 = LOA_ReadLong(&data, 0, 1);
    LOA_ReadLong(&data, 0, 2);

    if (verUpTo268 || (gsm->flags & 0x800) == 0)
    {
        flags2 = gsm->flags2;
        if ((flags2 & 0x4000) != 0)
            gsm->idx = SND_l_AddDialogNoLoad(gsm->i4);
        else if ((flags2 & 0x2000) != 0)
            gsm->idx = SND_l_AddMusicNoLoad(gsm->i4);
        else if ((flags2 & 0x80000) != 0)
            gsm->idx = SND_l_AddAmbienceNoLoad(gsm->i4);
        else if ((flags2 & 0x10000) != 0)
            gsm->idx = SND_l_AddSModifierNoLoad(gsm->i4, &data);
        else if ((flags2 & 8) != 0)
            gsm->idx = SND_l_AddLoadingSound(gsm->i4);
        else
            gsm->idx = SND_l_AddSound(gsm->i4);
    }
    else
    {
        gsm->idx = -1;
    }

    if (verUpTo267)
    {
        flags2 = gsm->flags2;
        channelBits = gsm->ii11 & ~0x7C0u;
        if ((flags2 & 0x4000) != 0)
            channelBits |= 0x80;
        else if ((flags2 & 0x2000) != 0)
            channelBits |= 0x100;
        else if ((flags2 & 0x80000) != 0)
            channelBits |= 0x80;
        else
            channelBits |= 0x40;
        gsm->ii11 = channelBits;
    }

    if (verUpTo269)
    {
        gsm->flags &= ~0x7000u;
        gsm->distPrefetchVolume = 0.0;
    }

    flags = gsm->flags;
    if ((flags & 0x1000) != 0 && (flags & 0x800) == 0)
        SND_StreamPrefetchOneSound(gsm->idx, 0);

    LOA_ReadLong(&data, 0, 2);
    gsm->sndInstanceID = -1;
    gsm->near1 = LOA_ReadFloat(&data, 0, 1);
    gsm->near2 = LOA_ReadFloat(&data, 0, 1);
    gsm->near3 = LOA_ReadFloat(&data, 0, 1);
    gsm->far1 = LOA_ReadFloat(&data, 0, 1);
    gsm->far2 = LOA_ReadFloat(&data, 0, 1);
    gsm->far3 = LOA_ReadFloat(&data, 0, 1);
    gsm->unkIsInActiveVolume = LOA_ReadFloat(&data, 0, 1);
    gsm->trackID = LOA_ReadLong(&data, 0, 1);

    flags2 = gsm->flags2;
    if ((flags2 & 0x20000) == 0)
        gsm->trackID = -1;
    if ((verUpTo268 || (gsm->flags & 0x800) == 0)
        && (gsm->trackID != -1 || (flags2 & 0x20000) != 0)
        && (flags2 & 0x86000) == 0)
    {
        gsm->trackID = -1;
        gsm->flags2 = flags2 & ~0x20000u;
    }

    gsm->nearFarFactor = LOA_ReadFloat(&data, 0, 1);
    gsm->nearFar2factor = LOA_ReadFloat(&data, 0, 1);
    gsm->nearFar3factor = LOA_ReadFloat(&data, 0, 1);
    gsm->xx = LOA_ReadFloat(&data, 0, 1);
    gsm->f4 = LOA_ReadFloat(&data, 0, 1);
    gsm->b0 = LOA_ReadFloat(&data, 0, 1);
    gsm->soundIndex = LOA_ReadLong(&data, 0, 1);
    LOA_ReadLong(&data, 0, 1);
    gsm->soundBank = 0;
    gsm->timeRemainTillStart = LOA_ReadFloat(&data, 0, 1);
    if (verUpTo270)
        gsm->timeRemainTillStart = 0.0;
    LOA_ReadCharArray(&data, 0, 4u, 2);
    LOA_ReadCharArray(&data, 0, 4u, 2);
    LOA_ReadCharArray(&data, 0, 4u, 2);
    LOA_ReadCharArray(&data, 0, 0x28u, 2);

    flags2 = gsm->flags2;
    if ((flags2 & 0x11000000) != 0)
    {
        zoneChecked = 1;
        if (gsm->far1 < gsm->near1)
        {
            gsm->near1 = 10.0;
            gsm->far1 = 80.0;
        }
        if (gsm->nearFarFactor > 1.0 || gsm->nearFarFactor < 0.0)
            gsm->nearFarFactor = 0.5;
    }
    if ((flags2 & 0x2000000) != 0)
    {
        zoneChecked = 1;
        if (gsm->far2 < gsm->near2)
        {
            gsm->near2 = 10.0;
            gsm->far2 = 80.0;
        }
        if (gsm->nearFar2factor > 1.0 || gsm->nearFar2factor < 0.0)
            gsm->nearFar2factor = 0.5;
    }
    if ((flags2 & 0x4000000) != 0)
    {
        zoneChecked = 1;
        if (gsm->far3 < (double)gsm->near3)
        {
            gsm->near3 = 10.0;
            gsm->far3 = 80.0;
        }
        if (gsm->nearFar3factor > 1.0 || gsm->nearFar3factor < 0.0)
            gsm->nearFar3factor = 0.5;
    }

    if (zoneChecked)
    {
        if (gsm->unkIsInActiveVolume <= 0.0)
            gsm->unkIsInActiveVolume = 2.0;
        if (gsm->unkIsInActiveVolume >= 10000.0)
            gsm->unkIsInActiveVolume = 2.0;
        if (gsm->xx < 0.0)
            gsm->xx = 0.0;
        if (gsm->xx > 1.0)
            gsm->xx = 0.0;
        if (gsm->f4 < 0.0)
            gsm->xx = 0.0;
        if (gsm->f4 > 1.0)
            gsm->xx = 0.0;
    }

    if (verUpTo268)
    {
        gsm->soundIndex = -1;
        gsm->flags &= ~0x800u;
        gsm->soundBank = 0;
    }
    else if ((gsm->flags & 0x800) != 0)
    {
        soundKey = gsm->i4;
        gsm->flags2 = flags2 & ~0x96008u;
        if (soundKey && soundKey != -1)
            LOA_MakeFileRef(soundKey, &gsm->soundBank, SND_ul_CallbackLoad, 1);
        SND_AddMdFBankUser(gsm);
    }

    mdf->modifierFlags = 0x4000000;
    return data - start;
}

void GEN_ModifierSound_Reinit(tModifier* mdf)
{
    tSoundMdfStruct* gsm;
    int instance;
    int flags;

    gsm = (tSoundMdfStruct*)mdf->mdfPrivateDataPtr;
    mdf->modifierFlags = 0x4000000;
    instance = gsm->sndInstanceID;
    if (instance != -1)
    {
        if (SND_gst_Params.soundInstances[instance].allocatedSoundsIdx == gsm->idx)
            SND_Release(gsm->sndInstanceID);
        gsm->sndInstanceID = -1;
    }

    flags = gsm->flags;
    if ((flags & 0x800) != 0)
        gsm->idx = -1;
    gsm->flags = flags & ~0x8084u;
    if ((gsm->flags & 0x1000) != 0)
        SND_StreamPrefetchOneSound(gsm->idx, 0);
}

void GEN_ModifierSound_Unapply(struct sModifier* mdf, struct sGameObjectMain* gao)
{
}