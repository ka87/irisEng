#include "../memory.h"
#include "../loading.h"
#include "loadingsoundmodifier.h"

void MDF_LoadingSoundReleaseCallback(void* privateData)
{
    ((tLoadingSoundMdfStruct*)(privateData))->soundInstanceIndex = -1;
}

void MDF_LoadingSoundGetInstance(tGameObjectMain* gao, tLoadingSoundMdfStruct* lsmdf)
{
    int idx;
    tSoundInstanceElem* instance;

    idx = SND_l_GetLoadingInstance();
    if (idx >= 0 && idx < SND_gst_Params.soundInstanceNum)
    {
        instance = &SND_gst_Params.soundInstances[idx];
        if ((instance->flags & 0x400) != 0)
        {
            lsmdf->soundInstanceIndex = idx;
            SND_SetPos(gao, idx, gao->matrices);
            instance->gao = gao;
        }
    }
}

void MDF_LoadingSoundCreateInstance(tGameObjectMain* gao, tLoadingSoundMdfStruct* lsmdf)
{
    int idx;

    if (lsmdf->sndIndex == -1)
        return;

    idx = SND_l_Request(0, lsmdf->sndIndex);
    lsmdf->soundInstanceIndex = idx;
    if (idx == -1)
        return;

    SND_SetVol(idx, lsmdf->vol);
    SND_SetFlags(gao, idx, lsmdf->flags & ~0x1F000000u);
    SND_i_CheckOrInitGAO(gao);
    SND_gst_Params.soundInstances[idx].gao = gao;
    SND_SetPos(gao, idx, gao->matrices);
    if (lsmdf->freq)
        SND_SetFreq(idx, lsmdf->freq);
}

void MDF_LoadingSound_Apply(tModifier* mdf, tGameObjectMain* gao)
{
    tLoadingSoundMdfStruct* lsmdf;
    int idx;

    if (!SND_gst_Params.soundEnabled || !SND_gst_Params.sndMatrixRef)
        return;

    lsmdf = (tLoadingSoundMdfStruct*)mdf->mdfPrivateDataPtr;
    if (lsmdf->sndIndex < 0)
        return;

    if (MATH_DistanceOfTwoPoints(&SND_gst_Params.sndMatrixRef->Pos,
                                 &gao->matrices->Pos) >= lsmdf->maxDist)
    {
        lsmdf->soundInstanceIndex = -1;
    }
    else if (lsmdf->soundInstanceIndex < 0)
    {
        if (!SND_b_IsLoadingSound(lsmdf->sndIndex) || SND_l_GetLoadingInstance() == -1)
            MDF_LoadingSoundCreateInstance(gao, lsmdf);
        else
            MDF_LoadingSoundGetInstance(gao, lsmdf);
        idx = lsmdf->soundInstanceIndex;
        if (idx >= 0)
            SND_LoadingSoundRegisterInstanceUserCallback(idx, MDF_LoadingSoundReleaseCallback, lsmdf);
    }
}

void MDF_LoadingSound_Create(struct sGameObjectMain* gao, struct sModifier* mdf, void* initData)
{
    tLoadingSoundMdfStruct* lsmdf;

    lsmdf = (tLoadingSoundMdfStruct*)MEM_p_Alloc(sizeof(tLoadingSoundMdfStruct));
    if (lsmdf)
    {
        memset(lsmdf, 0, sizeof(tLoadingSoundMdfStruct));
        lsmdf->ii23 = 0.5;
        lsmdf->ii24 = 0.5;
        lsmdf->ii25 = 0.5;
        lsmdf->ii27 = 0.5;
        lsmdf->i1 = 256;
        lsmdf->i2 = 0;
        lsmdf->ii7 = 0;
        lsmdf->ii17 = 20.0;
        lsmdf->ii14 = 0.0;
        lsmdf->ii18 = 20.0;
        lsmdf->ii15 = 0.0;
        lsmdf->ii19 = 20.0;
        lsmdf->ii16 = 0.0;
        lsmdf->ii26 = 0.0;
        lsmdf->ii20 = 2.0;
        lsmdf->ii5 = 0.0;
        lsmdf->ii6 = 0.0;
        lsmdf->vol = 1.0;
        lsmdf->i5 = 0.0;
        lsmdf->ii9 = 0;
        lsmdf->ii28 = 0.0;
        lsmdf->freq = 0;
        lsmdf->flags = 4;
        lsmdf->ii11 = 0;
        lsmdf->sndIndex = -1;
        lsmdf->soundInstanceIndex = -1;
        lsmdf->i3 = -1;
        lsmdf->ii22 = -1;
        lsmdf->maxDist = 32.0;
        mdf->mdfPrivateDataPtr = lsmdf;
        if (initData)
        {
            mdf->modifierIntf->Load(mdf, (char*)initData);
            lsmdf->soundInstanceIndex = -1;
            lsmdf->flags &= 0x5F1B700C;
            lsmdf->ii11 &= 0xFCB;
        }
    }
    else
    {
        mdf->mdfPrivateDataPtr = 0;
    }
    mdf->modifierFlags |= 0x4000000u;
}

void MDF_LoadingSound_Destroy(struct sModifier* mdf)
{
    tLoadingSoundMdfStruct* lsmdf;

    lsmdf = (tLoadingSoundMdfStruct*)mdf->mdfPrivateDataPtr;
    if (lsmdf)
    {
        SND_DeleteByIndex(lsmdf->sndIndex);
        MEM_Free(lsmdf);
    }
    mdf->mdfPrivateDataPtr = 0;
    mdf->modifierFlags = 0x2000000;
}

int MDF_LoadingSound_Load(tModifier* mdf, char* data)
{
    tLoadingSoundMdfStruct* lsmdf;
    char* start;

    lsmdf = (tLoadingSoundMdfStruct*)mdf->mdfPrivateDataPtr;
    start = data;
    if (lsmdf)
    {
        lsmdf->i1 = LOA_ReadLong(&data, 0, 1);
        if (lsmdf->i1 == 256)
        {
            lsmdf->i2 = LOA_ReadLong(&data, 0, 1);
            lsmdf->ll = LOA_ReadLong(&data, 0, 1);
            lsmdf->i3 = LOA_ReadLong(&data, 0, 1);
            lsmdf->vol = LOA_ReadFloat(&data, 0, 1);
            lsmdf->i5 = LOA_ReadFloat(&data, 0, 1);
            lsmdf->ii5 = LOA_ReadFloat(&data, 0, 1);
            lsmdf->ii6 = LOA_ReadFloat(&data, 0, 1);
            lsmdf->ii7 = LOA_ReadLong(&data, 0, 1);
            lsmdf->freq = LOA_ReadLong(&data, 0, 1);
            lsmdf->ii9 = LOA_ReadLong(&data, 0, 1);
            lsmdf->flags = LOA_ReadLong(&data, 0, 1);
            lsmdf->ii11 = LOA_ReadLong(&data, 0, 1);
            lsmdf->sndIndex = LOA_ReadLong(&data, 0, 1);
            lsmdf->soundInstanceIndex = LOA_ReadLong(&data, 0, 1);
            lsmdf->ii14 = LOA_ReadFloat(&data, 0, 1);
            lsmdf->ii15 = LOA_ReadFloat(&data, 0, 1);
            lsmdf->ii16 = LOA_ReadFloat(&data, 0, 1);
            lsmdf->ii17 = LOA_ReadFloat(&data, 0, 1);
            lsmdf->ii18 = LOA_ReadFloat(&data, 0, 1);
            lsmdf->ii19 = LOA_ReadFloat(&data, 0, 1);
            lsmdf->ii20 = LOA_ReadFloat(&data, 0, 1);
            lsmdf->maxDist = LOA_ReadFloat(&data, 0, 1);
            lsmdf->ii22 = LOA_ReadLong(&data, 0, 1);
            lsmdf->ii23 = LOA_ReadFloat(&data, 0, 1);
            lsmdf->ii24 = LOA_ReadFloat(&data, 0, 1);
            lsmdf->ii25 = LOA_ReadFloat(&data, 0, 1);
            lsmdf->ii26 = LOA_ReadFloat(&data, 0, 1);
            lsmdf->ii27 = LOA_ReadFloat(&data, 0, 1);
            lsmdf->ii28 = LOA_ReadFloat(&data, 0, 1);
            LOA_ReadCharArray(&data, 0, 256u, 2);
        }
    }
    lsmdf->sndIndex = SND_l_AddLoadingSound(lsmdf->i3);
    lsmdf->soundInstanceIndex = -1;
    mdf->modifierFlags = 0x4000000;
    return data - start;
}

void MDF_LoadingSound_Reinit(struct sModifier* mdf)
{
    tLoadingSoundMdfStruct* lsmdf;
    int idx; 

    lsmdf = (tLoadingSoundMdfStruct*)mdf->mdfPrivateDataPtr;
    mdf->modifierFlags = 0x4000000;
    idx = lsmdf->soundInstanceIndex;
    if (idx == SND_l_GetLoadingInstance())
        SND_ReinitLoadingInstance();
    else
        SND_Release(idx);
    lsmdf->soundInstanceIndex = -1;
}

void MDF_LoadingSound_Unapply(tModifier* mdf, tGameObjectMain* gao)
{
}