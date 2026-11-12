#include "../memory.h"
#include "../loading.h"
#include "loadingsoundmodifier.h"

void MDF_LoadingSoundReleaseCallback(void* a1)
{
    ((tLoadingSoundMdfStruct*)(a1))->soundInstanceIndex = -1;
}

void MDF_LoadingSoundGetInstance(tGameObjectMain* a1, tLoadingSoundMdfStruct* a2)
{
    int v2;
    tSoundInstanceElem* v3;

    v2 = SND_l_GetLoadingInstance();
    if (v2 >= 0 && v2 < SND_gst_Params.soundInstanceNum)
    {
        v3 = &SND_gst_Params.soundInstances[v2];
        if ((v3->flags & 0x400) != 0)
        {
            a2->soundInstanceIndex = v2;
            SND_SetPos(a1, v2, a1->matrices);
            v3->gao = a1;
        }
    }
}

void MDF_LoadingSoundCreateInstance(tGameObjectMain* a1, tLoadingSoundMdfStruct* a2)
{
    int v2; // eax

    if (a2->sndIndex != -1)
    {
        v2 = SND_l_Request(0, a2->sndIndex);
        a2->soundInstanceIndex = v2;
        if (v2 != -1)
        {
            SND_SetVol(v2, a2->vol);
            SND_SetFlags(a1, a2->soundInstanceIndex, a2->flags & ~0x1F000000u);
            SND_i_CheckOrInitGAO(a1);
            SND_gst_Params.soundInstances[a2->soundInstanceIndex].gao = a1;
            SND_SetPos(a1, a2->soundInstanceIndex, a1->matrices);
            if (a2->freq)
                SND_SetFreq(a2->soundInstanceIndex, a2->freq);
        }
    }
}

void MDF_LoadingSound_Apply(tModifier* a1, tGameObjectMain* a2)
{
    tLoadingSoundMdfStruct* v2;
    int v3;
    Mtx4x4* v4;
    double v5;
    double v6;
    double v7;
    int v8;

    if (SND_gst_Params.soundEnabled)
    {
        if (SND_gst_Params.sndMatrixRef)
        {
            v2 = (tLoadingSoundMdfStruct*)a1->mdfPrivateDataPtr;
            v3 = v2->sndIndex;
            if (v3 >= 0)
            {
                v4 = a2->matrices;
                v5 = SND_gst_Params.sndMatrixRef->Pos.x - v4->Pos.x;
                v6 = SND_gst_Params.sndMatrixRef->Pos.y - v4->Pos.y;
                v7 = SND_gst_Params.sndMatrixRef->Pos.z - v4->Pos.z;
                if (sqrt(v7 * v7 + v5 * v5 + v6 * v6) >= v2->maxDist)
                {
                    v2->soundInstanceIndex = -1;
                }
                else if (v2->soundInstanceIndex < 0)
                {
                    if (!SND_b_IsLoadingSound(v3) || SND_l_GetLoadingInstance() == -1)
                        MDF_LoadingSoundCreateInstance(a2, v2); 
                    else
                        MDF_LoadingSoundGetInstance(a2, v2);
                    v8 = v2->soundInstanceIndex;
                    if (v8 >= 0)
                        SND_LoadingSoundRegisterInstanceUserCallback(v8, MDF_LoadingSoundReleaseCallback, v2);
                }
            }
        }
    }
}

void MDF_LoadingSound_Create(struct sGameObjectMain* a1, struct sModifier* a2, void* a3)
{
    tLoadingSoundMdfStruct* v3;
    int v4;
    int v5;
    int v6;

    v3 = (tLoadingSoundMdfStruct*)MEM_p_Alloc(sizeof(tLoadingSoundMdfStruct));
    if (v3)
    {
        memset(v3, 0, sizeof(tLoadingSoundMdfStruct));
        v3->ii23 = 0.5;
        v3->ii24 = 0.5;
        v3->ii25 = 0.5;
        v3->ii27 = 0.5;
        v3->i1 = 256;
        v3->i2 = 0;
        v3->ii7 = 0;
        v3->ii17 = 20.0;
        v3->ii14 = 0.0;
        v3->ii18 = 20.0;
        v3->ii15 = 0.0;
        v3->ii19 = 20.0;
        v3->ii16 = 0.0;
        v3->ii26 = 0.0;
        v3->ii20 = 2.0;
        v3->ii5 = 0.0;
        v3->ii6 = 0.0;
        v3->vol = 1.0;
        v3->i5 = 0.0;
        v3->ii9 = 0;
        v3->ii28 = 0.0;
        v3->freq = 0;
        v3->flags = 4;
        v3->ii11 = 0;
        v3->sndIndex = -1;
        v3->soundInstanceIndex = -1;
        v3->i3 = -1;
        v3->ii22 = -1;
        v3->maxDist = 32.0;
        a2->mdfPrivateDataPtr = v3;
        if (a3)
        {
            a2->modifierIntf->Load(a2, (char *)a3);
            v4 = v3->flags & 0x5F1B700C;
            v5 = v3->ii11 & 0xFCB;
            v3->soundInstanceIndex = -1;
            v3->flags = v4;
            v3->ii11 = v5;
        }
        a2->modifierFlags |= 0x4000000u;
    }
    else
    {
        v6 = a2->modifierFlags | 0x4000000;
        a2->mdfPrivateDataPtr = 0;
        a2->modifierFlags = v6;
    }
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

int MDF_LoadingSound_Load(tModifier* arg0, char* a1)
{
    tLoadingSoundMdfStruct* v2;
    char* v3;
    int v4;
    int result;

    v2 = (tLoadingSoundMdfStruct*)arg0->mdfPrivateDataPtr;
    v3 = a1;
    if (v2)
    {
        v4 = LOA_ReadLong(&a1, 0, 1);
        v2->i1 = v4;
        if (v4 == 256)
        {
            v2->i2 = LOA_ReadLong(&a1, 0, 1);
            v2->ll = LOA_ReadLong(&a1, 0, 1);
            v2->i3 = LOA_ReadLong(&a1, 0, 1);
            v2->vol = LOA_ReadFloat(&a1, 0, 1);
            v2->i5 = LOA_ReadFloat(&a1, 0, 1);
            v2->ii5 = LOA_ReadFloat(&a1, 0, 1);
            v2->ii6 = LOA_ReadFloat(&a1, 0, 1);
            v2->ii7 = LOA_ReadLong(&a1, 0, 1);
            v2->freq = LOA_ReadLong(&a1, 0, 1);
            v2->ii9 = LOA_ReadLong(&a1, 0, 1);
            v2->flags = LOA_ReadLong(&a1, 0, 1);
            v2->ii11 = LOA_ReadLong(&a1, 0, 1);
            v2->sndIndex = LOA_ReadLong(&a1, 0, 1);
            v2->soundInstanceIndex = LOA_ReadLong(&a1, 0, 1);
            v2->ii14 = LOA_ReadFloat(&a1, 0, 1);
            v2->ii15 = LOA_ReadFloat(&a1, 0, 1);
            v2->ii16 = LOA_ReadFloat(&a1, 0, 1);
            v2->ii17 = LOA_ReadFloat(&a1, 0, 1);
            v2->ii18 = LOA_ReadFloat(&a1, 0, 1);
            v2->ii19 = LOA_ReadFloat(&a1, 0, 1);
            v2->ii20 = LOA_ReadFloat(&a1, 0, 1);
            v2->maxDist = LOA_ReadFloat(&a1, 0, 1);
            v2->ii22 = LOA_ReadLong(&a1, 0, 1);
            v2->ii23 = LOA_ReadFloat(&a1, 0, 1);
            v2->ii24 = LOA_ReadFloat(&a1, 0, 1);
            v2->ii25 = LOA_ReadFloat(&a1, 0, 1);
            v2->ii26 = LOA_ReadFloat(&a1, 0, 1);
            v2->ii27 = LOA_ReadFloat(&a1, 0, 1);
            v2->ii28 = LOA_ReadFloat(&a1, 0, 1);
            LOA_ReadCharArray(&a1, 0, 256u, 2);
        }
    }
    v2->sndIndex = SND_l_AddLoadingSound(v2->i3);
    v2->soundInstanceIndex = -1;
    result = a1 - v3;
    arg0->modifierFlags = 0x4000000;
    return result;
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

void MDF_LoadingSound_Unapply(tModifier* a1, tGameObjectMain* a2) {
}