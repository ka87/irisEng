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

void GEN_ModifierSound_Apply(struct sModifier* a1, struct sGameObjectMain* a2)
{
    tSoundMdfStruct* gsm = (tSoundMdfStruct*)a1->mdfPrivateDataPtr;

    if (gsm->idx == -1)
        MDF_SndGetSoundIndexFromBank((tSoundMdfStruct*)a1->mdfPrivateDataPtr);

    if (gsm->idx < 0) return;

    if (gsm->sndInstanceID >= 0)
    {
        if ((SND_gst_Params.soundInstances[gsm->sndInstanceID].flags & 0x400) == 0 || gsm->idx != SND_gst_Params.soundInstances[gsm->sndInstanceID].allocatedSoundsIdx)
            gsm->sndInstanceID = -1;
    }
    if (gsm->sndInstanceID == -1)
    {
        MDF_SndCreateInstance(gsm, a2);
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

int GEN_ModifierSound_Load(tModifier* arg0, char* a2)
{
    tSoundMdfStruct* v2;
    int v3;
    int v4;
    int v5;
    int v6;
    double v7;
    double v8;
    double v12;
    int v16;
    double v17;
    int v18;
    int v19;
    int v20;
    int v21;
    int v22;
    int v23;
    int v24;
    int v25;
    int v26;
    double v27;
    double v28;
    int v29;
    int v30;
    char* a1;
    int v33;
    int v34;
    int v35;
    int v36;

    v2 = (tSoundMdfStruct*)arg0->mdfPrivateDataPtr;
    v3 = 0;
    a1 = a2;
    v33 = 0;
    LOA_ul_GetCurrentKey();
    v4 = LOA_ReadLong(&a1, 0, 1);
    v5 = 0;
    v6 = 0;
    v34 = 0;
    v35 = 0;
    v36 = 0;
    if (v2)
    {
        switch (v4)
        {
        case 266:
            v3 = 1;
            goto LABEL_4;
        case 267:
        LABEL_4:
            v5 = 1;
            goto LABEL_5;
        case 268:
        LABEL_5:
            v34 = 1;
            goto LABEL_6;
        case 269:
        LABEL_6:
            v35 = 1;
            goto LABEL_7;
        case 270:
        LABEL_7:
            v36 = 1;
            goto LABEL_8;
        case 271:
        case 272:
        LABEL_8:
            v6 = 1;
            goto LABEL_9;
        case 273:
        LABEL_9:
            v2->flags = LOA_ReadLong(&a1, 0, 1);
            v2->modifierID = LOA_ReadLong(&a1, 0, 1);
            v2->i4 = LOA_ReadLong(&a1, 0, 1);
            v2->volume = LOA_ReadFloat(&a1, 0, 1);
            v7 = LOA_ReadFloat(&a1, 0, 1);
            v2->volume2 = (float)v7;
            if (v3)
            {
                v8 = v2->volume + v2->volume;
                v2->volume = (float)v8;
                if (v8 < 0.0000001)
                {
                    v2->volume = 0.0;
                }
                else if (v8 > 1.0)
                {
                    v2->volume = 1.0;
                }
                v12 = v7 + v7;
                v2->volume2 = (float)v12;
                if (v12 < 0.0000001)
                {
                    v2->volume2 = 0.0;
                }
                else if (v12 > 1.0)
                {
                    v2->volume2 = 1.0;
                }
            }
            v2->ii2 = LOA_ReadFloat(&a1, 0, 1);
            v2->distPrefetchVolume = LOA_ReadFloat(&a1, 0, 1);
            v2->ii4 = LOA_ReadFloat(&a1, 0, 1);
            v16 = LOA_ReadLong(&a1, 0, 1);
            v17 = v2->ii2;
            v2->ii5 = v16;
            if (v17 <= 0.0000099999997)
                v2->ii2 = 0.0;
            if (v2->ii4 <= 0.0000099999997)
                v2->ii4 = 0.0;
            v2->freq2 = LOA_ReadLong(&a1, 0, 1);
            v2->pan = LOA_ReadLong(&a1, 0, 1);
            v2->ii8 = LOA_ReadLong(&a1, 0, 1);
            v2->ii9 = LOA_ReadLong(&a1, 0, 1);
            if (v6)
            {
                v2->ii8 = 1;
                v2->ii9 = 128;
            }
            v2->flags2 = LOA_ReadLong(&a1, 0, 1);
            v2->ii11 = LOA_ReadLong(&a1, 0, 1);
            LOA_ReadLong(&a1, 0, 2);
            v18 = v34;
            if (v34 || (v2->flags & 0x800) == 0)
            {
                v19 = v2->flags2;
                if ((v19 & 0x4000) != 0)
                {
                    v20 = SND_l_AddDialogNoLoad(v2->i4);
                }
                else if ((v19 & 0x2000) != 0)
                {
                    v20 = SND_l_AddMusicNoLoad(v2->i4);
                }
                else if ((v19 & 0x80000) != 0)
                {
                    v20 = SND_l_AddAmbienceNoLoad(v2->i4);
                }
                else if ((v19 & 0x10000) != 0)
                {
                    v20 = SND_l_AddSModifierNoLoad(v2->i4, &a1);
                }
                else if ((v19 & 8) != 0)
                {
                    v20 = SND_l_AddLoadingSound(v2->i4);
                }
                else
                {
                    v20 = SND_l_AddSound(v2->i4);
                }
                v2->idx = v20;
            }
            else
            {
                v2->idx = -1;
            }
            if (v5)
            {
                v21 = v2->flags2;
                v22 = v2->ii11 & ~0x7C0u;
                v2->ii11 = v22;
                if ((v21 & 0x4000) != 0)
                {
                    v23 = v22 | 0x80;
                }
                else if ((v21 & 0x2000) != 0)
                {
                    v23 = v22 | 0x100;
                }
                else if ((v21 & 0x80000) != 0)
                {
                    v23 = v22 | 0x80;
                }
                else
                {
                    v23 = v22 | 0x40;
                }
                v2->ii11 = v23;
            }
            if (v35)
            {
                v2->flags &= ~0x7000u;
                v2->distPrefetchVolume = 0.0;
            }
            v24 = v2->flags;
            if ((v24 & 0x1000) != 0 && (v24 & 0x800) == 0)
                SND_StreamPrefetchOneSound(v2->idx, 0);
            LOA_ReadLong(&a1, 0, 2);
            v2->sndInstanceID = -1;
            v2->near1 = LOA_ReadFloat(&a1, 0, 1);
            v2->near2 = LOA_ReadFloat(&a1, 0, 1);
            v2->near3 = LOA_ReadFloat(&a1, 0, 1);
            v2->far1 = LOA_ReadFloat(&a1, 0, 1);
            v2->far2 = LOA_ReadFloat(&a1, 0, 1);
            v2->far3 = LOA_ReadFloat(&a1, 0, 1);
            v2->unkIsInActiveVolume = LOA_ReadFloat(&a1, 0, 1);
            v2->trackID = LOA_ReadLong(&a1, 0, 1);
            v25 = v2->flags2;
            if ((v25 & 0x20000) == 0)
                v2->trackID = -1;
            if ((v18 || (v2->flags & 0x800) == 0) && (v2->trackID != -1 || (v25 & 0x20000) != 0) && (v25 & 0x86000) == 0)
            {
                v2->trackID = -1;
                v2->flags2 = v25 & ~0x20000u;
            }
            v2->nearFarFactor = LOA_ReadFloat(&a1, 0, 1);
            v2->nearFar2factor = LOA_ReadFloat(&a1, 0, 1);
            v2->nearFar3factor = LOA_ReadFloat(&a1, 0, 1);
            v2->xx = LOA_ReadFloat(&a1, 0, 1);
            v2->f4 = LOA_ReadFloat(&a1, 0, 1);
            v2->b0 = LOA_ReadFloat(&a1, 0, 1);
            v2->soundIndex = LOA_ReadLong(&a1, 0, 1);
            LOA_ReadLong(&a1, 0, 1);
            v2->soundBank = 0;
            v2->timeRemainTillStart = LOA_ReadFloat(&a1, 0, 1);
            if (v36)
                v2->timeRemainTillStart = 0.0;
            LOA_ReadCharArray(&a1, 0, 4u, 2);
            LOA_ReadCharArray(&a1, 0, 4u, 2);
            LOA_ReadCharArray(&a1, 0, 4u, 2);
            LOA_ReadCharArray(&a1, 0, 0x28u, 2);
            v26 = v2->flags2;
            if ((v26 & 0x11000000) != 0)
            {
                v27 = v2->far1;
                v33 = 1;
                if (v27 < v2->near1)
                {
                    v2->near1 = 10.0;
                    v2->far1 = 80.0;
                }
                if (v2->nearFarFactor > 1.0 || v2->nearFarFactor < 0.0)
                    v2->nearFarFactor = 0.5;
            }
            if ((v26 & 0x2000000) != 0)
            {
                v28 = v2->far2;
                v33 = 1;
                if (v28 < v2->near2)
                {
                    v2->near2 = 10.0;
                    v2->far2 = 80.0;
                }
                if (v2->nearFar2factor > 1.0 || v2->nearFar2factor < 0.0)
                    v2->nearFar2factor = 0.5;
            }
            if ((v26 & 0x4000000) != 0)
            {
                if (v2->far3 < (double)v2->near3)
                {
                    v2->near3 = 10.0;
                    v2->far3 = 80.0;
                }
                if (v2->nearFar3factor > 1.0 || v2->nearFar3factor < 0.0)
                    v2->nearFar3factor = 0.5;
            }
            else if (!v33)
            {
                goto LABEL_94;
            }
            if (v2->unkIsInActiveVolume <= 0.0)
                v2->unkIsInActiveVolume = 2.0;
            if (v2->unkIsInActiveVolume >= 10000.0)
                v2->unkIsInActiveVolume = 2.0;
            if (v2->xx < 0.0)
                v2->xx = 0.0;
            if (v2->xx > 1.0)
                v2->xx = 0.0;
            if (v2->f4 < 0.0)
                v2->xx = 0.0;
            if (v2->f4 > 1.0)
                v2->xx = 0.0;
        LABEL_94:
            v29 = v2->flags;
            if (v34)
            {
                v2->soundIndex = -1;
                v2->flags = v29 & ~0x800u;
                v2->soundBank = 0;
            }
            else if ((v29 & 0x800) != 0)
            {
                v30 = v2->i4;
                v2->flags2 = v26 & ~0x96008u;
                if (v30 && v30 != -1)
                    LOA_MakeFileRef(v30, &v2->soundBank, SND_ul_CallbackLoad, 1);
                SND_AddMdFBankUser(v2);
            }
            arg0->modifierFlags = 0x4000000;
            break;
        default:
            return a1 - a2;
        }
    }
    return a1 - a2;
}

void GEN_ModifierSound_Reinit(tModifier* a1)
{
    tSoundMdfStruct* v1;
    int v2;
    int v3;
    int v4;

    v1 = (tSoundMdfStruct*)a1->mdfPrivateDataPtr;
    a1->modifierFlags = 0x4000000;
    v2 = v1->sndInstanceID;
    if (v2 != -1)
    {
        if (SND_gst_Params.soundInstances[v2].allocatedSoundsIdx == v1->idx)
            SND_Release(v1->sndInstanceID);
        v1->sndInstanceID = -1;
    }
    v3 = v1->flags;
    if ((v3 & 0x800) != 0)
        v1->idx = -1;
    v4 = v3 & ~0x8084u;
    v1->flags = v4;
    if ((v4 & 0x1000) != 0)
        SND_StreamPrefetchOneSound(v1->idx, 0);
}

void GEN_ModifierSound_Unapply(struct sModifier* a1, struct sGameObjectMain* a2) {
}