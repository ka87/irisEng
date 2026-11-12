#include "../memory.h"
#include "../loading.h"
#include "soundfxmodifier.h"

void GEN_ModifierSoundFx_Apply(struct sModifier* mdf, struct sGameObjectMain* gao)
{
    tSoundFxMdfStruct* sfx;
    float dist;
    int flags;

    if (!SND_gst_Params.soundEnabled)
        return;

    sfx = (tSoundFxMdfStruct*)mdf->mdfPrivateDataPtr;
    if (!SND_gst_Params.sndMatrixRef)
        return;

    dist = MATH_DistanceOfTwoPoints(&SND_gst_Params.sndMatrixRef->Pos, &gao->matrices->Pos);
    flags = sfx->flags;
    if ((flags & 1) != 0)
    {
        if (dist > sfx->i5 + sfx->i4)
        {
            sfx->flags = flags & ~1u;
            SND_i_FxSeti(8199, 1);
        }
    }
    else if (dist < sfx->i4)
    {
        sfx->flags = flags | 1;
        SND_i_FxSeti(8194, sfx->mode);
        SND_i_FxSeti(8195, sfx->delay);
        SND_i_FxSeti(8196, sfx->feedback);
        SND_i_FxSetf(8197, sfx->vol);
        SND_i_FxSeti(8198, sfx->wetPan);
        SND_i_FxSeti(8193, 1);
    }
}

void GEN_ModifierSoundFx_Create(struct sGameObjectMain* gao, struct sModifier* mdf, void* initData)
{
    tSoundFxMdfStruct* sfx;

    sfx = (tSoundFxMdfStruct*)MEM_p_Alloc(sizeof(tSoundFxMdfStruct));
    if (sfx)
    {
        memset(sfx, 0, sizeof(tSoundFxMdfStruct));
        sfx->i5 = 0.5;
        sfx->vol = 0.5;
        sfx->i1 = 256;
        sfx->flags = 0;
        sfx->i3 = 1;
        sfx->i4 = 10.0;
        sfx->mode = 0;
        sfx->delay = 300;
        sfx->feedback = 50;
        sfx->wetPan = 0;
        mdf->mdfPrivateDataPtr = sfx;
        if (initData)
        {
            mdf->modifierIntf->Load(mdf, initData);
            sfx->flags = 0;
        }
    }
    else
    {
        mdf->mdfPrivateDataPtr = 0;
    }
    mdf->modifierFlags |= 0x4000000u;
}

void GEN_ModifierSoundFx_Destroy(struct sModifier* mdf)
{
    if (mdf->mdfPrivateDataPtr)
        MEM_Free(mdf->mdfPrivateDataPtr);
    mdf->mdfPrivateDataPtr = 0;
    mdf->modifierFlags = 0x2000000;
}

int GEN_ModifierSoundFx_Load(tModifier* mdf, char* data)
{
    tSoundFxMdfStruct* sfx;
    char* start;

    sfx = (tSoundFxMdfStruct*)mdf->mdfPrivateDataPtr;
    start = data;
    if (sfx)
    {
        sfx->i1 = LOA_ReadLong(&data, 0, 1);
        if (sfx->i1 == 256)
        {
            sfx->flags = LOA_ReadLong(&data, 0, 1);
            sfx->i3 = LOA_ReadLong(&data, 0, 1);
            sfx->i4 = LOA_ReadFloat(&data, 0, 1);
            sfx->i5 = LOA_ReadFloat(&data, 0, 1);
            sfx->mode = LOA_ReadLong(&data, 0, 1);
            sfx->delay = LOA_ReadLong(&data, 0, 1);
            sfx->feedback = LOA_ReadLong(&data, 0, 1);
            sfx->vol = LOA_ReadFloat(&data, 0, 1);
            sfx->wetPan = LOA_ReadLong(&data, 0, 1);
            LOA_ReadCharArray(&data, 0, 0x100u, 2);
            if (sfx->i4 <= 0.0)
                sfx->i4 = 10.0;
            if (sfx->i5 <= 0.0)
                sfx->i4 = 0.5;
            switch (sfx->mode)
            {
            case 7:
                sfx->mode = 10;
                break;
            case 8:
                sfx->mode = 11;
                break;
            case 0:
            case 1:
            case 2:
            case 3:
            case 4:
            case 5:
            case 6:
            case 9:
            case 0xA:
            case 0xB:
            case 0xC:
                break;
            default:
                sfx->mode = 0;
                break;
            }
            if (sfx->delay < 0 || sfx->delay > 1000)
                sfx->delay = 300;
            if (sfx->feedback < 0 || sfx->feedback > 100)
                sfx->feedback = 50;
            if (sfx->vol < 0.0 || sfx->vol > 1.0)
                sfx->vol = 0.5;
            if (sfx->wetPan < -10000 || sfx->wetPan > 10000)
                sfx->wetPan = 0;
            mdf->modifierFlags = 0x4000000;
        }
    }
    return data - start;
}

void GEN_ModifierSoundFx_Reinit(struct sModifier* mdf)
{
    mdf->modifierFlags = 0x4000000;
    ((tSoundFxMdfStruct*)(mdf->mdfPrivateDataPtr))->flags = 0;
}

void GEN_ModifierSoundFx_Unapply(struct sModifier* mdf, struct sGameObjectMain* gao)
{
}
