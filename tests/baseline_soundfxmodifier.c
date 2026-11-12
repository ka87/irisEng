#include "../memory.h"
#include "../loading.h"
#include "soundfxmodifier.h"

void GEN_ModifierSoundFx_Apply(struct sModifier* a1, struct sGameObjectMain* a2)
{
    tSoundFxMdfStruct* v2;
    Mtx4x4* v3;
    double v4; 
    double v5; 
    int v6; 
    double v7;
    double v8;
    int v9; 

    if (SND_gst_Params.soundEnabled)
    {
        v2 = (tSoundFxMdfStruct*)a1->mdfPrivateDataPtr;
        if (SND_gst_Params.sndMatrixRef)
        {
            v3 = a2->matrices;
            v4 = SND_gst_Params.sndMatrixRef->Pos.x - v3->Pos.x;
            v5 = SND_gst_Params.sndMatrixRef->Pos.y - v3->Pos.y;
            v6 = v2->flags;
            v7 = SND_gst_Params.sndMatrixRef->Pos.z - v3->Pos.z;
            v8 = sqrt(v7 * v7 + v4 * v4 + v5 * v5);
            if ((v6 & 1) != 0)
            {
                if (v8 > v2->i5 + v2->i4)
                {
                    v2->flags = v6 & ~1u;
                    SND_i_FxSeti(8199, 1);
                }
            }
            else if (v8 < v2->i4)
            {
                v9 = v2->mode;
                v2->flags = v6 | 1;
                SND_i_FxSeti(8194, v9);
                SND_i_FxSeti(8195, v2->delay);
                SND_i_FxSeti(8196, v2->feedback);
                SND_i_FxSetf(8197, v2->vol);
                SND_i_FxSeti(8198, v2->wetPan);
                SND_i_FxSeti(8193, 1);
            }
        }
    }
}

void GEN_ModifierSoundFx_Create(struct sGameObjectMain* a1, struct sModifier* a2, void* a3)
{
    tSoundFxMdfStruct* v3;
    int v4;

    v3 = (tSoundFxMdfStruct*)MEM_p_Alloc(sizeof(tSoundFxMdfStruct));
    if (v3)
    {
        memset(v3, 0, sizeof(tSoundFxMdfStruct));
        v3->i5 = 0.5;
        v3->vol = 0.5;
        v3->i1 = 256;
        v3->flags = 0;
        v3->i3 = 1;
        v3->i4 = 10.0;
        v3->mode = 0;
        v3->delay = 300;
        v3->feedback = 50;
        v3->wetPan = 0;
        a2->mdfPrivateDataPtr = v3;
        if (a3)
        {
            a2->modifierIntf->Load(a2, a3);
            v3->flags = 0;
        }
        a2->modifierFlags |= 0x4000000u;
    }
    else
    {
        v4 = a2->modifierFlags | 0x4000000;
        a2->mdfPrivateDataPtr = 0;
        a2->modifierFlags = v4;
    }
}

void GEN_ModifierSoundFx_Destroy(struct sModifier* a1)
{
    if (a1->mdfPrivateDataPtr)
        MEM_Free(a1->mdfPrivateDataPtr);
    a1->mdfPrivateDataPtr = 0;
    a1->modifierFlags = 0x2000000;
}

int GEN_ModifierSoundFx_Load(tModifier* arg0, char* a2)
{
    int v2;
    tSoundFxMdfStruct* v3;
    int v4;
    int v5;
    int v6;
    int v7;
    char* a1;
    char Buffer[1024];

    v2 = LOA_ul_GetCurrentKey();
    sprintf(Buffer, "0x%x", v2);
    v3 = (tSoundFxMdfStruct*)arg0->mdfPrivateDataPtr;
    a1 = a2;
    if (v3)
    {
        v4 = LOA_ReadLong(&a1, 0, 1);
        v3->i1 = v4;
        if (v4 == 256)
        {
            v3->flags = LOA_ReadLong(&a1, 0, 1);
            v3->i3 = LOA_ReadLong(&a1, 0, 1);
            v3->i4 = LOA_ReadFloat(&a1, 0, 1);
            v3->i5 = LOA_ReadFloat(&a1, 0, 1);
            v3->mode = LOA_ReadLong(&a1, 0, 1);
            v3->delay = LOA_ReadLong(&a1, 0, 1);
            v3->feedback = LOA_ReadLong(&a1, 0, 1);
            v3->vol = LOA_ReadFloat(&a1, 0, 1);
            v3->wetPan = LOA_ReadLong(&a1, 0, 1);
            LOA_ReadCharArray(&a1, 0, 0x100u, 2);
            if (v3->i4 <= 0.0)
                v3->i4 = 10.0;
            if (v3->i5 <= 0.0)
                v3->i4 = 0.5;
            switch (v3->mode)
            {
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
            case 7:
                v3->mode = 10;
                break;
            case 8:
                v3->mode = 11;
                break;
            default:
                v3->mode = 0;
                break;
            }
            v5 = v3->delay;
            if (v5 < 0 || v5 > 1000)
                v3->delay = 300;
            v6 = v3->feedback;
            if (v6 < 0 || v6 > 100)
                v3->feedback = 50;
            if (v3->vol < 0.0 || v3->vol > 1.0)
                v3->vol = 0.5;
            v7 = v3->wetPan;
            if (v7 < -10000 || v7 > 10000)
                v3->wetPan = 0;
            arg0->modifierFlags = 0x4000000;
        }
    }
    return a1 - a2;
}

void GEN_ModifierSoundFx_Reinit(struct sModifier* a1)
{
    a1->modifierFlags = 0x4000000;
    ((tSoundFxMdfStruct*)(a1->mdfPrivateDataPtr))->flags = 0;
}

void GEN_ModifierSoundFx_Unapply(struct sModifier* a1, struct sGameObjectMain* a2) {
}