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

void SND_SModifierDeleteOneSModifier(tSndModifierElem** a1, int type)
{
    tSndModifierElem* v2;
    tSndModifierElem* v3;

    v2 = *a1;
    v3 = 0;
    if (*a1)
    {
        while (v2->sndModifierModelIntf)
        {
            if (v2->sndModifierModelIntf->Type == type)
            {
                v2->sndModifierModelIntf->Destroy(v2, 0, 0);
                if (v3)
                    v3->nextModifier = v2->nextModifier;
                else
                    *a1 = v2->nextModifier;
                MEM_Free(v2);
                return;
            }
            v3 = v2;
            v2 = v2->nextModifier;
            if (!v2)
                return;
        }
    }
}

void SND_SModifierDeleteAll(tSndModifierElem** a1)
{
    tSndModifierElem* i;

    for (i = *a1; *a1; i = *a1)
        SND_SModifierDeleteOneSModifier(a1, i->sndModifierModelIntf->Type);
}

void SND_SModifierDeleteAllLinkedSound(tSndModifierElem* a1)
{
    tSndModifierElem* i;
    tSoundModifierDataGeneric* v2;

    for (i = a1; i; i = i->nextModifier)
    {
        if (!i->sndModifierModelIntf)
            break;
        v2 = (tSoundModifierDataGeneric*)(i->linkedSoundData);
        if (v2)
            SND_DeleteByIndex(v2->sndIndexInAllocatedSounds);
    }
}

struct sSndModifierElem* SND_pst_SModifierInsertOneSModifier(struct sSndModifierElem** a1, int type, void* linkedSound, int a4)
{
    tSndModifierElem* v5;
    tSndModifierElem* v6;
    tSoundModifierElemIntf* v7;

    if (type >= 7)
        return *a1;
    if (type < 0)
        return *a1;
    v5 = *a1;
    if (*a1)
    {
        while (v5->sndModifierModelIntf)
        {
            if (v5->sndModifierModelIntf->Type == type)
            {
                v5->sndModifierModelIntf->InitChainedElemVersion(v5, linkedSound, a4); // init
                return v5;
            }
            v5 = v5->nextModifier;
            if (!v5)
                break;
        }
    }
    v6 = (tSndModifierElem*)MEM_p_Alloc(sizeof(tSndModifierElem));
    v6->sndModifierModelIntf = 0;
    v6->nextModifier = 0;
    v6->linkedSoundDataSize = 0;
    v6->linkedSoundData = 0;
    v7 = SND_gs_SoundModifiers[type].sndModifierModelIntf;
    v6->sndModifierModelIntf = v7;
    v7->Init(v6, linkedSound, a4);// ez is ugyanaz az init
    v6->nextModifier = *a1;
    *a1 = v6;
    return v6;
}

void SND_SModifierInstanciate(struct sSndModifierElem* a1, struct sSoundInstanceElem* a2)
{
    tSndModifierElem* i;
    tSndModifierElem* v3;

    for (i = a1; i; i = i->nextModifier)
    {
        v3 = SND_pst_SModifierInsertOneSModifier(&a2->modifierInstances, i->sndModifierModelIntf->Type, i->linkedSoundData, i->linkedSoundDataSize);
        i->sndModifierModelIntf->PlayApplyOnInstance(v3, i->linkedSoundData, i->linkedSoundDataSize, a2);
    }
}

int SND_l_AddSModifierNoLoad(int a1, char** a2)
{
    int v3;
    int v4;
    tSndSoundElem* v5;
    int v6;
    int* v7;
    int v8;
    int* v9;
    int v10;
    int* v11;
    int v12;
    char Buffer[256];
    void* v15;
    int v16;
    int Offset;
    int DstBuf;

    if (!SND_gst_Params.soundEnabled)
        return -1;
    if (!a1)
        return -1;
    if (a1 == -1)
        return -1;
    Offset = BIG_ul_SearchKeyToPos(a1);
    if (Offset == -1)
        return -1;
    v3 = SND_l_GetSoundIndex(a1);
    DstBuf = v3;
    if (v3 != -1)
    {
        ++SND_gst_Params.allocatedSounds[v3].usageCnt;
        return DstBuf;
    }
    v4 = SND_i_AllocSound();
    v5 = &SND_gst_Params.allocatedSounds[v4];
    v16 = v4;
    v5->soundKey = a1;
    v5->sndSndBufElem = (tSndSoundBufferElem*)-1;
    v5->flags = 0x10400;
    v5->muteFacorFlags = 64;
    v5->freq = 0;
    v5->startDataPos = 0;
    v5->numChannels = 0;
    v5->bitsPerSample = 0;
    v5->usageCnt = 1;
    v5->sndInterface = (tSndInterfaceStruct*)&SND_gpst_Interface;
    v5->soundModifier = 0;
    v5->fullsoundLen = 0;
    if (!isSpeedModeAndBinFile())
        BIG_ReadNoSeek(Offset, &DstBuf, 4u);
    if (isSpeedModeAndBinFile())
    {
        if (!a2)
        {
            v6 = 0;
            DstBuf = 0;
            goto LABEL_14;
        }
        v6 = LOA_ReadLong(a2, 0, 1);
        DstBuf = v6;
    }
    else
    {
        v6 = DstBuf;
    }
    if (v6 == 8)
    {
        sprintf(Buffer, "Bad size (8oct) of SModifier file (.smd) : 0x%x, loading canceled, Please check the file !", a1);
        v5->flags = 0;
        return -1;
    }
LABEL_14:
    v7 = (int*)MEM_p_Alloc(v6);
    memset(v7, 0, DstBuf);
    v15 = v7;
    if (!isSpeedModeAndBinFile())
        BIG_ReadNoSeek(Offset + 4, v7, DstBuf);
    if (isSpeedModeAndBinFile() && a2)
        LOA_ReadCharArray(a2, (char*)v7, DstBuf, 1);
    if (*v7 != 1)
    {
        sprintf(Buffer, "Bad SModifier file (.smd) : 0x%x, loading canceled", a1);
        v5->flags = 0;
        MEM_Free(v7);
        return -1;
    }
    v8 = v7[2];
    v9 = v7 + 2; // TODO: => sSoundModifierDataGeneric
    if (v8 != -1)
    {
        do
        {
            if (v9[1])
            {
                v10 = v9[3];
                v11 = (v9 + 2);
                v9[4] = -1;
                if ((v10 & 0x4000) != 0)
                {
                    v12 = SND_l_AddDialogNoLoad(v11[0]);
                }
                else if ((v10 & 0x2000) != 0)
                {
                    v12 = SND_l_AddMusicNoLoad(v11[0]);
                }
                else if ((v10 & 0x80000) != 0)
                {
                    v12 = SND_l_AddAmbienceNoLoad(v11[0]);
                }
                else if ((v10 & 8) != 0)
                {
                    v12 = SND_l_AddLoadingSound(v11[0]);
                }
                else
                {
                    v12 = SND_l_AddSound(v11[0]);
                }
                v9[4] = v12;
                v5 = &(SND_gst_Params.allocatedSounds[v4]);
            }
            else
            {
                v11 = 0;
            }
            SND_pst_SModifierInsertOneSModifier(&v5->soundModifier, v9[0], (char*)v11, v9[1]); // [0] = type, [1] = len, [2] = sndKey, [3] = flags, [4] = idx
            v9 = (int*)((char*)v9 + v9[1] + 8);
        } while (v9[0] != -1);
    }
    MEM_Free(v15);
    return v16;
}

BOOL MDF_b_SndIsInPrefetchVolume(struct sSoundMdfStruct* a1)
{
    BOOL result;
    int v2;
    tSoundInstanceElem* v3;
    Vec3* v4;

    if (!SND_gst_Params.sndMatrixRef)
        return 0;
    v2 = a1->sndInstanceID;
    result = 0;
    if (v2 >= 0 && v2 < SND_gst_Params.soundInstanceNum)
    {
        v3 = &SND_gst_Params.soundInstances[v2];
        if ((v3->flags & 0x400) != 0)
        {
            v4 = v3->sndPositionSetInSetPosPtr;
            if (v4)
            {
                if (MATH_DistanceOfTwoPoints(&SND_gst_Params.sndMatrixRef->Pos, v4) < a1->distPrefetchVolume)
                    result = 1;
            }
        }
    }
    return result;
}

void MDF_SndCreateInstance(struct sSoundMdfStruct* a1, struct sGameObjectMain* a2)
{
    int v3;
    int v4;
    int v5;
    int v6;
    int v7;
    float v8;
    float v9;
    int v10;
    int v11;
    int v12;
    float v13;
    float v14;
    int v15;
    int v16;
    int v17;
    int v18;
    int v19;
    tLinkedSoundFade a3;
    float v21;
    float v22;
    float v23;
    float v24;

    v3 = a1->idx;
    if (v3 >= 0)
    {
        v4 = 0;
        v5 = SND_l_Request(0, v3);
        a1->sndInstanceID = v5;
        if (v5 != -1)
        {
            SND_SetVol(v5, a1->volume2);
            SND_SetFlags(a2, a1->sndInstanceID, a1->flags2);
            SND_SetFlags(a2, a1->sndInstanceID, a1->ii11 & 0xFCB | 0x40000000);
            SND_i_CheckOrInitGAO(a2);
            SND_gst_Params.soundInstances[a1->sndInstanceID].gao = a2;
            SND_SetPos(a2, a1->sndInstanceID, a2->matrices);
            if (a1->freq2)
                SND_SetFreq(a1->sndInstanceID, a1->freq2);
            v6 = a1->pan;
            if (v6)
                SND_SetPan(a1->sndInstanceID, v6, 0);
            if ((a1->flags2 & 0x10000000) != 0)
            {
                v21 = MATH_LinearInterpolateFloats(a1->near1, a1->far1, a1->nearFarFactor);
                SND_SetNearFar(a1->sndInstanceID, 4096, a1->near1, a1->far1);
                SND_i_InstanceSetf(a1->sndInstanceID, 4099, v21);
                v4 = 1;
            }
            if ((a1->flags2 & 0x1000000) != 0)
            {
                v22 = MATH_LinearInterpolateFloats(a1->near1, a1->far1, a1->nearFarFactor);
                SND_SetNearFar(a1->sndInstanceID, 4096, a1->near1, a1->far1);
                SND_i_InstanceSetf(a1->sndInstanceID, 4099, v22);
                v4 = 1;
            }
            if ((a1->flags2 & 0x2000000) != 0)
            {
                v23 = MATH_LinearInterpolateFloats(a1->near2, a1->far2, a1->nearFar2factor);
                SND_SetNearFar(a1->sndInstanceID, 4097, a1->near2, a1->far2);
                SND_i_InstanceSetf(a1->sndInstanceID, 4100, v23);
                v4 = 1;
            }
            if ((a1->flags2 & 0x4000000) != 0)
            {
                v24 = MATH_LinearInterpolateFloats(a1->near3, a1->far3, a1->nearFar3factor);
                SND_SetNearFar(a1->sndInstanceID, 4098, a1->near3, a1->far3);
                SND_i_InstanceSetf(a1->sndInstanceID, 4101, v24);
                v4 = 1;
            }
            if ((a1->flags2 & 0x8000000) != 0)
            {
                SND_i_InstanceSetf(a1->sndInstanceID, 4105, a1->b0);
                v4 = 1;
            }
            if ((a1->ii11 & 3) != 0)
                SND_i_InstanceSetf(a1->sndInstanceID, 4104, a1->unkIsInActiveVolume);
            if (v4)
            {
                SND_i_InstanceSetf(a1->sndInstanceID, 4103, a1->f4);
                SND_i_InstanceSetf(a1->sndInstanceID, 4102, a1->xx);
            }
            v7 = a1->trackID;
            if (v7 != -1 && v7)
                SND_SetTrack(a1->sndInstanceID, a1->trackID);
            if (a1->ii2 != 0.0)
            {
                v8 = a1->volume2;
                v9 = a1->volume;
                a3.maxTimeToLast = a1->ii2;
                v10 = a1->i4;
                a3.minVolume = v8;
                a3.sndKey = v10;
                v11 = a1->sndInstanceID;
                a3.maxVolume = v9;
                v12 = a1->idx;
                a3.fadeFlags = 1;
                a3.flags = 0;
                a3.sndIndexInAllocatedSounds = v12;
                SND_SModifierAddInInstance(v11, 3, &a3, 36);// fadein
            }
            if (a1->ii4 != 0.0)
            {
                v13 = a1->volume2;
                v14 = a1->volume;
                a3.maxTimeToLast = a1->ii4;
                v15 = a1->flags;
                a3.fadeFlags = 0x80;
                a3.maxVolume = v13;
                a3.minVolume = v14;
                if ((v15 & 0x10) != 0)
                    a3.fadeFlags = 160;
                if ((v15 & 0x20) != 0)
                    a3.fadeFlags |= 8u;
                v16 = a1->idx;
                a3.sndKey = a1->i4;
                v19 = a1->sndInstanceID;
                a3.flags = 0;
                a3.sndIndexInAllocatedSounds = v16;
                SND_SModifierAddInInstance(v19, 4, &a3, 36);// fadeout
            }
            v17 = a1->ii5;
            if (v17)
                SND_LinkToFamilly(v17, a1->sndInstanceID);
            v18 = a1->flags;
            if ((v18 & 1) != 0 && (a1->trackID == -1 || (v18 & 0x400) == 0 || !SND_i_IsTrackPlaying(a1->trackID)))
                SND_PlayMdF(a1);
        }
    }
}

int MDF_b_SndIsInActiveVolume(struct sSoundMdfStruct* a1)
{
    int v2;
    tSoundInstanceElem* v3;
    tSoundInstanceElem* v4;
    int v5;
    Mtx4x4* v6;
    int v7;
    int v8;
    double v9; 
    double v10;
    double v11;
    Vec3* v12; 
    Vec3* v13; 
    Vec3 dest; 
    int v15[3];

    if (!SND_gst_Params.sndMatrixRef)
        return 0;
    v2 = a1->sndInstanceID;
    if (v2 >= 0 && v2 < SND_gst_Params.soundInstanceNum)
    {
        v3 = &SND_gst_Params.soundInstances[v2];
        v4 = v3;
        v5 = v3->flags;
        if ((v5 & 0x400) != 0 && (v5 & 0x17000000) != 0)
        {
            if (v3->sndPositionSetInSetPosPtr)
            {
                v6 = v3->sndPosMtxSetInSetPos;
                if (v6 || (v5 & 0x7000000) == 0)
                {
                    v13 = v3->sndPositionSetInSetPosPtr;
                    v12 = &SND_gst_Params.sndMatrixRef->Pos;
                    if ((v5 & 0x10000000) != 0)
                        return a1->far1 + v3->dtFar > MATH_DistanceOfTwoPoints(v12, v13);
                    v15[0] = 0x1000000;
                    v15[1] = 0x2000000;
                    v15[2] = 0x4000000;
                    MATH_SubstractVec(&dest, v12, v13);
                    v7 = 0;
                    while (1)
                    {
                        v8 = v15[v7];
                        if ((v8 & v5) != 0)
                            break;
                        v15[v7] = 2;
                    LABEL_27:
                        if (++v7 >= 3)
                            return (uint8_t)(v15[2] & v15[1] & (v15[0] & 0xff) & 2) >> 1;
                    }
                    if (v7)
                    {
                        if (v7 == 1)
                        {
                            v10 = v6->v2.z * dest.z + v6->v2.y * dest.y;
                            v11 = dest.x * v6->v2.x;
                        }
                        else
                        {
                            if (v7 != 2)
                            {
                                v9 = 0.0;
                                goto LABEL_23;
                            }
                            v10 = v6->v3.z * dest.z + v6->v3.y * dest.y;
                            v11 = dest.x * v6->v3.x;
                        }
                    }
                    else
                    {
                        v10 = v6->v1.z * dest.z + v6->v1.y * dest.y;
                        v11 = dest.x * v6->v1.x;
                    }
                    v9 = fabs(v10 + v11);
                LABEL_23:
                    if (v9 >= v4->farArr[v7] + v4->dtFar)
                        v15[v7] = 0;
                    else
                        v15[v7] = v8 | 2;
                    goto LABEL_27;
                }
            }
        }
    }
    return 0;
}

int MDF_SndGetSoundIndexFromBank(tSoundMdfStruct* a1)
{
    int result;
    tSndBank* v2;
    int* v3;

    result = a1->flags;
    if ((result & 0x800) != 0 && a1->idx == -1)
    {
        v2 = a1->soundBank;
        if (v2)
        {
            v3 = v2->soundIndices;
            if (v3)
            {
                result = a1->soundIndex;
                if (result >= 0 && result < v2->elemNum)
                {
                    result = v3[result];
                    a1->idx = result;
                    if (result >= 0 && result < SND_gst_Params.allocatedSoundsMaxNum)
                    {
                        result = SND_gst_Params.allocatedSounds[result].flags & 0x96008;
                        a1->flags2 |= result;
                    }
                }
            }
        }
    }
    return result;
}

// Specific SoundModifier functions --------------------------------------------------------------

void SND_SModifierEmptyInit(tSndModifierElem* a1, tSoundModifierDataEmpty* a2, int x)
{
    tSoundModifierDataEmpty* v3;

    if (!a1->linkedSoundData)
        a1->linkedSoundData = MEM_p_Alloc(sizeof(tSoundModifierDataEmpty));
    v3 = (tSoundModifierDataEmpty*)a1->linkedSoundData;
    a1->linkedSoundDataSize = sizeof(tSoundModifierDataEmpty);
    *v3 = *a2;
}

void SND_SModifierEmptyDestroy(tSndModifierElem* a1, int unk1, int unk2)
{
    MEM_Free(a1->linkedSoundData);
    a1->linkedSoundData = 0;
    a1->linkedSoundDataSize = 0;
}

void SND_SModifierFadeInit(tSndModifierElem* a1, tSoundModifierDataFade* a2, int x)
{
    long double v2;
    int v3;
    int v4;
    double v5;
    tSoundModifierDataFade* result;

    if (a2->i2 >= 0.0000001)
    {
        if (a2->i2 > 1.0)
            a2->i2 = 1.0;
    }
    else
    {
        a2->i2 = 0.0;
    }
    if (a2->i1 >= 0.0000001)
    {
        if (a2->i1 > 1.0)
            a2->i1 = 1.0;
    }
    else
    {
        a2->i1 = 0.0;
    }
    if (a2->i3 == 0.0 || a2->i1 == a2->i2)
    {
        v5 = a2->i1;
        a2->i3 = 0.0000099999997f;
        a2->i4 = 10000.0f;
        a2->i2 = (float)(v5 + 0.000099999997);
    }
    else
    {
        v2 = fabs(a2->i3);
        v3 = (a2->i5 & 0x83) - 1;
        a2->i3 = (float)v2;
        if (v3)
        {
            v4 = v3 - 1;
            if (v4)
            {
                if (v4 != 126)
                    DBG_assert(0, __FILE__, __LINE__, "0", "Bad Fade identification", 0, 1);
                if (a2->i1 == 0.0)
                    a2->i1 = 0.0000001f;
                if (a2->i2 == 0.0)
                    a2->i2 = 0.0000001f;
                a2->i6 = 0.0;
                a2->i4 = (float)(255.0 / v2);
            }
            else
            {
                if (a2->i1 == 0.0)
                    a2->i1 = 0.0000001f;
                if (a2->i2 == 0.0)
                    a2->i2 = 0.0000001f;
                //a2->i4 = __FYL2X__(a2->i2 / a2->i1, 0.30102999566398119802) / v2;
                a2->i4 = (float)((log10(2) * log2(a2->i2 / a2->i1)) / v2);
            }
        }
        else
        {
            a2->i4 = (float)((a2->i2 - a2->i1) / v2);
        }
    }
    if (!a1->linkedSoundData)
        a1->linkedSoundData = MEM_p_Alloc(sizeof(tSoundModifierDataFade));
    result = (tSoundModifierDataFade*)a1->linkedSoundData;
    a1->linkedSoundDataSize = sizeof(tSoundModifierDataFade);
    *result = *a2;
}

void SND_SModifierFadeUpdate(tSndModifierElem* a1, tSoundInstanceElem* a2, int x)
{
    tSoundModifierDataFade* v3;
    double v4;
    double v6;
    BOOL v9;

    v3 = (tSoundModifierDataFade*)a1->linkedSoundData;
    if ((v3->i5 & 0x10) == 0)
        return;
    if ((v3->i5 & 0x83) == 1)
    {
        v4 = TIM_gf_dt * v3->i4 + a2->volume;
        goto LABEL_6;
    }
    if ((v3->i5 & 0x83) == 2)
    {
        v4 = pow(10.0, v3->i4 * TIM_gf_dt) * a2->volume;
    LABEL_6:
        a2->volume = (float)v4;
    }
    if (a2->volume >= 0.0000001)
    {
        if (a2->volume > 1.0)
            a2->volume = 1.0;
    }
    else
    {
        a2->volume = 0.0;
    }
    v6 = a2->volume;
    if (v3->i4 <= 0.0)
    {
        if (v6 <= v3->i2)
        {
            a2->volume = v3->i2;
            v9 = (v3->i5 & 0x20) == 0;
            v3->i5 &= ~0x10u;
            if (!v9)
                SND_Stop((unsigned int)((char*)a2 - (char*)SND_gst_Params.soundInstances) / sizeof(tSoundInstanceElem));
            if ((v3->i5 & 8) != 0)
                a2->flags |= 0x280u;
        }
    }
    else if (v6 >= v3->i2)
    {
        a2->volume = v3->i2;
        v3->i5 &= ~0x10u;
    }
}

void SND_SModifierFadeInitInstance(struct sSndModifierElem* a1, void* linkedsound, int dataSize, struct sSoundInstanceElem* a2)
{
    tSoundModifierDataFade* lsf = (tSoundModifierDataFade*)linkedsound;
    SND_SetVol(((unsigned int)(a2 - (struct sSoundInstanceElem*)SND_gst_Params.soundInstances) / sizeof(tSoundInstanceElem)), lsf->i1);
}

void SND_SModifierFadeInUpdate(tSndModifierElem* a1, tSoundInstanceElem* a2, int x)
{
    tSoundModifierDataFade* v2;
    double v3;
    int v4;
    double v5;
    double v6;

    v2 = (tSoundModifierDataFade*)a1->linkedSoundData;
    if ((v2->i5 & 0x10) == 0)
        return;
    if ((v2->i5 & 0x83) == 1)
    {
        v5 = TIM_gf_dt * v2->i4 + a2->volume;
    }
    else
    {
        if ((v2->i5 & 0x83) != 2)
        {
            if ((v2->i5 & 0x83) == 0x80)
            {
                v3 = TIM_gf_dt + v2->i6;
                v2->i6 = (float)v3;
                v6 = v3 * v2->i4 + MATH_gd_magicDoubleForRound;
                v4 = 255 - (*(uint32_t *)&v6);
                if (255 - (*(uint32_t*)&v6) >= 0)
                {
                    if (v4 >= 256)
                        v4 = 255;
                }
                else
                {
                    v4 = 0;
                }
                a2->volume = SND_f_GetVolFromAtt(SND_SModifier_FadeTable[v4]) * (v2->i2 - v2->i1) + v2->i1;
                if (v2->i6 > (double)v2->i3)
                    a2->volume = v2->i2;
            }
            goto LABEL_14;
        }
        v5 = pow(10.0, v2->i4 * TIM_gf_dt) * a2->volume;
    }
    a2->volume = (float)v5;
LABEL_14:
    if (a2->volume >= 0.0000001)
    {
        if (a2->volume > 1.0)
            a2->volume = 1.0;
    }
    else
    {
        a2->volume = 0.0;
    }
    if (a2->volume >= (double)v2->i2)
    {
        a2->volume = v2->i2;
        v2->i5 &= ~0x10u;
    }
}

void SND_SModifierFadeInOrOutNotifyPause(tSndModifierElem* a1, tSoundInstanceElem* a2)
{
    tSoundModifierDataFade* v2;
    int v3;

    v2 = (tSoundModifierDataFade*)a1->linkedSoundData;
    v3 = v2->i5;
    if ((v3 & 0x10) != 0)
    {
        v2->i5 = v3 & ~0x10u | 0x40;
    }
    else if ((v3 & 0x40) != 0)
    {
        v2->i5 = v3 & ~0x40u | 0x10;
    }
}

void SND_SModifierFadeInNotifyPlay(tSndModifierElem* a1, tSoundInstanceElem* a2)
{
    tSoundModifierDataFade* v2;
    unsigned int v3;
    BOOL v4;
    float v5;

    v2 = (tSoundModifierDataFade*)a1->linkedSoundData;
    v3 = v2->i5 & ~0x40u;
    v4 = (v2->i5 & 0x10) == 0;
    v2->i5 = v3;
    if (v4)
    {
        v2->i5 = v3 | 0x10;
        v5 = v2->i1;
        v2->i6 = 0.0;
        SND_SetVol(((unsigned int)((char*)a2 - (char*)SND_gst_Params.soundInstances) / sizeof(tSoundInstanceElem)), v5);
    }
}

void  SND_SModifierFadeInNotifyStop(tSndModifierElem* a1, tSoundInstanceElem* x)
{
    tSoundModifierDataFade* lsf = (tSoundModifierDataFade*)(a1->linkedSoundData);
    lsf->i5 &= ~0x50u;
}

void SND_SModifierFadeOutUpdate(struct sSndModifierElem* a1, struct sSoundInstanceElem* a2, int x)
{
    tSoundModifierDataFade* v2;
    double v3;
    int v4;
    double v5;
    BOOL v6;
    double v7;

    v2 = (tSoundModifierDataFade*)a1->linkedSoundData;
    if ((v2->i5 & 0x10) == 0)
        return;
    if ((v2->i5 & 0x83) == 1)
    {
        v5 = TIM_gf_dt * v2->i4 + a2->volume;
    }
    else
    {
        if ((v2->i5 & 0x83) != 2)
        {
            if ((v2->i5 & 0x83) == 128)
            {
                v3 = TIM_gf_dt + v2->i6;
                v2->i6 = (float)v3;
                v7 = v3 * v2->i4 + MATH_gd_magicDoubleForRound;
                v4 = (*(uint32_t*)&v7);
                if ((*(int32_t*)&v7) < 256)
                {
                    if ((*(int32_t*)&v7) < 0)
                        v4 = 0;
                }
                else
                {
                    v4 = 255;
                }
                a2->volume = SND_f_GetVolFromAtt(SND_SModifier_FadeTable[v4]) * (v2->i1 - v2->i2) + v2->i2;
                if (v2->i6 > (double)v2->i3)
                    a2->volume = v2->i2;
            }
            goto LABEL_14;
        }
        v5 = pow(10.0, v2->i4 * TIM_gf_dt) * a2->volume;
    }
    a2->volume = (float)v5;
LABEL_14:
    if (a2->volume >= 0.0000001)
    {
        if (a2->volume > 1.0)
            a2->volume = 1.0;
    }
    else
    {
        a2->volume = 0.0;
    }
    if (a2->volume <= (double)v2->i2)
    {
        a2->volume = v2->i2;
        v6 = (v2->i5 & 0x20) == 0;
        v2->i5 &= ~0x10u;
        if (!v6)
            SND_Stop((unsigned int)((char*)a2 - (char*)SND_gst_Params.soundInstances) / sizeof(tSoundInstanceElem));
        if ((v2->i5 & 8) != 0)
            a2->flags |= 0x280u;
    }
}

void SND_SModifierFadeOutNotifyPlay(tSndModifierElem* a1, tSoundInstanceElem* a2)
{
    tSoundModifierDataFade* v2;

    v2 = (tSoundModifierDataFade*)a1->linkedSoundData;
    v2->i5 &= ~0x50u;
    v2->i6 = 0.0;
}

void SND_SModifierFadeOutNotifyStop(tSndModifierElem* a1, tSoundInstanceElem* a2)
{
    tSoundModifierDataFade* v2;

    v2 = (tSoundModifierDataFade*)a1->linkedSoundData;
    v2->i5 = v2->i5 & ~0x40u | 0x10;
    v2->i6 = 0.0;
}

void SND_SModifierPlayInit(tSndModifierElem* a1, tSoundModifierDataPlayer* a2, int x)
{
    tSoundModifierDataPlayer* v3;
    tSoundModifierDataPlayer* v4;
    int v5;
    int v6;

    v3 = a2;
    v4 = 0;
    if (x == 28)
    {
        v4 = (tSoundModifierDataPlayer*)MEM_p_Alloc(sizeof(tSoundModifierDataPlayer));
        memcpy(v4, a2, 28u);
        v4->i6 = 0;
    }
    else
    {
        if (x != 36)
            goto LABEL_6;
        v4 = (tSoundModifierDataPlayer*)MEM_p_Alloc(sizeof(tSoundModifierDataPlayer));
        memcpy(v4, a2, 36u);
    }
    v4->timeRemainTillStart = 0.0;
    v3 = v4;
LABEL_6:
    v5 = v3->i6;
    if (!v5)
    {
        v3->flags = 64;
        goto LABEL_10;
    }
    if (v5 == 1)
        LABEL_10:
    v3->i6 = 2;
    if (v3->vol >= 0.0000001)
    {
        if (v3->vol > 1.0)
            v3->vol = 1.0;
    }
    else
    {
        v3->vol = 0.0;
    }
    if (v3->ffar < 0.0)
        v3->ffar = -v3->ffar;
    if (v3->fnear < 0.0)
        v3->fnear = -v3->fnear;
    v6 = v3->freq;
    if (v6 < 0)
        v3->freq = -v6;
    if (v3->timeRemainTillStart < 0.0)
        v3->timeRemainTillStart = 0.0;
    if (v3->timeRemainTillStart > 1000000.0)
        v3->timeRemainTillStart = 0.0;
    if (!a1->linkedSoundData)
        a1->linkedSoundData = MEM_p_Alloc(sizeof(tSoundModifierDataPlayer));
    a1->linkedSoundDataSize = sizeof(tSoundModifierDataPlayer);
    memcpy(a1->linkedSoundData, v3, sizeof(tSoundModifierDataPlayer));
    if (v4)
        MEM_Free(v4);
}

void SND_SModifierPlayApplyOnInstance(tSndModifierElem* a1, void* llinkedsound, int dataSize, tSoundInstanceElem* a2)
{
    int v4;
    tSoundModifierDataPlayer* linkedsound = (tSoundModifierDataPlayer*)llinkedsound;

    SND_SetNearFar(((unsigned int)((char*)a2 - (char*)SND_gst_Params.soundInstances) / sizeof(tSoundInstanceElem)), 4096, linkedsound->fnear, linkedsound->ffar);
    a2->volume = linkedsound->vol;
    v4 = linkedsound->freq;
    if (v4)
        a2->freq = v4;
    if ((linkedsound->gen.flags & 0x100000) != 0)
        a2->flags |= 0x100000u;
    if (linkedsound->flags)
        SND_SetFlags(0, ((unsigned int)((char*)a2 - (char*)SND_gst_Params.soundInstances) / sizeof(tSoundInstanceElem)), linkedsound->flags & 0xFCB | 0x40000000);
}

void SND_SModifierPlayerNotifyPlay(tSndModifierElem* a1, tSoundInstanceElem* a2)
{
    tSoundModifierDataPlayer* lsf = (tSoundModifierDataPlayer*)(a1->linkedSoundData);

    a2->timeRemainTillStart = lsf->timeRemainTillStart;
}

void SND_SModifierPlayApplyOnSound(tSndModifierElem* a1, void* linkedsound, int linkedsoundsize, tSndSoundElem* a4)
{
    tSoundModifierDataPlayer* v4;
    int v5;

    v4 = (tSoundModifierDataPlayer*)a1->linkedSoundData;
    a4->freq = v4->freq;
    v5 = v4->gen.flags;
    if (v5)
        a4->flags |= v5;
}

void SND_SModifierExtPlayerInit(tSndModifierElem* a1, tSoundModifierDataExtPlayer* a2, int size)
{
    tSoundModifierDataExtPlayer* v3;
    tSoundModifierDataExtPlayer* v4;
    int v5;
    tSoundModifierDataExtPlayer* v6;
    int v7;
    int v8;

    v3 = a2;
    v4 = 0;
    if (a2->i6 <= 0.0000099999997)
        a2->i6 = 0.0;
    if (a2->i7 <= 0.0000099999997)
        a2->i7 = 0.0;
    if (a2->trackID != -1 || (a2->gen.flags & 0x20000) != 0)
    {
        v5 = a2->gen.flags;
        if ((v5 & 0x86000) == 0)
        {
            a2->trackID = -1;
            a2->gen.flags = v5 & ~0x20000u;
        }
    }
    if (!a1->linkedSoundData)
        a1->linkedSoundData = MEM_p_Alloc(116u);
    v6 = (tSoundModifierDataExtPlayer*)a1->linkedSoundData;
    a1->linkedSoundDataSize = sizeof(tSoundModifierDataExtPlayer);
    switch (size)
    {
    case 84:
        v4 = (tSoundModifierDataExtPlayer*)MEM_p_Alloc(sizeof(tSoundModifierDataExtPlayer));
        memcpy(v4, a2, 84u);
        v4->i25 = 0;
        break;
    case 108:
        v4 = (tSoundModifierDataExtPlayer*)MEM_p_Alloc(sizeof(tSoundModifierDataExtPlayer));
        memcpy(v4, a2, 108u);
        v4->i25 = 1;
        break;
    case 112:
        v4 = (tSoundModifierDataExtPlayer*)MEM_p_Alloc(sizeof(tSoundModifierDataExtPlayer));
        memcpy(v4, a2, 112u);
        v4->i25 = 2;
        break;
    default:
        goto LABEL_18;
    }
    v3 = v4;
LABEL_18:
    switch (v3->i25)
    {
    case 0:
        v3->interpolationFactor = 0.5;
        v3->interpolationFactor2 = 0.5;
        v3->interpolationFactor3 = 0.5;
        v3->farCoefficient = 0.0;
        v3->middleCoefficient = 0.5;
        v3->nearfar = 0.0;
        goto LABEL_20;
    case 1:
    LABEL_20:
        v3->i1 = v3->i1 & 0x80B | 0x40;
        goto LABEL_21;
    case 2:
    LABEL_21:
        v3->timeRemainTillStart = 0.0;
        goto LABEL_22;
    case 3:
    LABEL_22:
        v3->i25 = 3;
        memcpy(v6, v3, sizeof(tSoundModifierDataExtPlayer));
        break;
    default:
        break;
    }
    v7 = v6->gen.flags & 0x5F1B700C;
    v8 = v6->i1 & 0xFCB;
    v6->idx = -1;
    v6->gen.flags = v7;
    v6->i1 = v8;
    if (v4)
        MEM_Free(v4);
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
    SND_SetFlags(0, extPlayer->idx, extPlayer->i1 & 0xFCB | 0x40000000);
    
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

void SND_SModifier3DSettingsApplyOnInstance(tSndModifierElem* a1, void* linkedsound, int dataSize, tSoundInstanceElem* a2)
{
    tSoundModifierData3DSettings* v5;
    double v6;
    int v7;
    double v8;
    double v9;
    float v10;
    float a1a;
    float a2a;

    v5 = (tSoundModifierData3DSettings*)a1->linkedSoundData;
    v6 = v5->i1;
    v7 = (unsigned int)((char*)a2 - (char*)SND_gst_Params.soundInstances) / sizeof(tSoundInstanceElem);
    if (v6 >= 0.0)
    {
        if (v6 > 1.0)
            v6 = 1.0;
    }
    else
    {
        v6 = 0.0;
    }
    a1a = (float)((1.0 - v6) * a2->nearArr[0] + v6 * a2->farArr[0]);
    v8 = v5->i1;
    if (v8 >= 0.0)
    {
        if (v8 > 1.0)
            v8 = 1.0;
    }
    else
    {
        v8 = 0.0;
    }
    a2a = (float)((1.0 - v8) * a2->nearArr[0] + v8 * a2->farArr[0]);
    v9 = v5->i1;
    if (v9 >= 0.0)
    {
        if (v9 > 1.0)
            v9 = 1.0;
    }
    else
    {
        v9 = 0.0;
    }
    v10 = (float)((1.0 - v9) * a2->nearArr[0] + v9 * a2->farArr[0]);
    SND_i_InstanceSetf(v7, 4099, a1a);
    SND_i_InstanceSetf(v7, 4100, a2a);
    SND_i_InstanceSetf(v7, 4101, v10);
    SND_i_InstanceSetf(v7, 4102, v5->farCoefficient);
    SND_i_InstanceSetf(v7, 4103, v5->middleCoefficient);
    SND_SetNearFar(v7, 4105, v5->nearfar, v5->nearfar);
}

void SND_SModifier3DSettingsInit(tSndModifierElem* a1, tSoundModifierData3DSettings* a2, int x)
{
    tSoundModifierData3DSettings* result;
    int v4;

    if (!a1->linkedSoundData)
        a1->linkedSoundData = MEM_p_Alloc(sizeof(tSoundModifierData3DSettings));
    result = (tSoundModifierData3DSettings*)a1->linkedSoundData;
    a1->linkedSoundDataSize = sizeof(tSoundModifierData3DSettings);
    memcpy(result, a2, sizeof(tSoundModifierData3DSettings));
    v4 = result->gen.flags & 0x5F1B700C;
    result->gen.sndIndexInAllocatedSounds = -1;
    result->gen.flags = v4;
}

void SND_SModifierEmptyApplyOnInstance (struct sSndModifierElem* a1, void* a2, int a3, struct sSoundInstanceElem* a4) {
}

void SND_SModifierEmptyApplyOnSound(struct sSndModifierElem* a1, void* a2, int a3, struct sSndSoundElem* a4) {
}

void SND_SModifierFadeApplyOnInstance(struct sSndModifierElem* a1, void* a2, int a3, struct sSoundInstanceElem* a4) {
}

void SND_SModifierExtPlayerApplyOnSound(struct sSndModifierElem* a1, void* a2, int a3, struct sSndSoundElem* a4) {
}

int SND_InitMdFModule()
{
    int result;

    SND_gst_BankMdFUserList = 0;
    sndMdfBankUser = 0;
    result = SND_gst_Params.soundEnabled;
    if (SND_gst_Params.soundEnabled)
    {
        SND_gst_BankMdFUserList = 16;
        sndMdfBankUser = (tSoundMdfStruct**)MEM_p_Alloc(16 * 4);
        result = 0;
        memset(sndMdfBankUser, 0, 4 * SND_gst_BankMdFUserList);
    }
    return result;
}

void SND_SModifierInitModule()
{
    tSoundModifierDataEmpty* v0;
    tSoundModifierDataFade* v1;
    tSoundModifierDataFade* v2;
    tSoundModifierDataPlayer* v3;
    tSoundModifierDataExtPlayer* v4;
    tSoundModifierData3DSettings* v5;

    memset(SND_gs_SoundModifiers, 0, sizeof(SND_gs_SoundModifiers));
    SND_gs_SoundModifiers[0].sndModifierModelIntf = (tSoundModifierElemIntf*)MEM_p_Alloc(sizeof(tSoundModifierElemIntf));
    SND_gs_SoundModifiers[0].sndModifierModelIntf->Type = 0;
    SND_gs_SoundModifiers[0].sndModifierModelIntf->Destroy = SND_SModifierEmptyDestroy;
    SND_gs_SoundModifiers[0].sndModifierModelIntf->Init = SND_SModifierEmptyInit;
    SND_gs_SoundModifiers[0].sndModifierModelIntf->Update = NULL;
    SND_gs_SoundModifiers[0].sndModifierModelIntf->InitChainedElemVersion = SND_SModifierEmptyInit;
    SND_gs_SoundModifiers[0].sndModifierModelIntf->PlayApplyOnInstance = SND_SModifierEmptyApplyOnInstance;
    SND_gs_SoundModifiers[0].sndModifierModelIntf->PlayApplyOnSound = SND_SModifierEmptyApplyOnSound;
    SND_gs_SoundModifiers[0].sndModifierModelIntf->NotifyPause = NULL;
    SND_gs_SoundModifiers[0].sndModifierModelIntf->NotifyPlay = NULL;
    SND_gs_SoundModifiers[0].sndModifierModelIntf->NotifyStop = NULL;
    SND_gs_SoundModifiers[0].nextModifier = 0;
    v0 = (tSoundModifierDataEmpty*)MEM_p_Alloc(sizeof(tSoundModifierDataEmpty));
    SND_gs_SoundModifiers[0].linkedSoundData = (char *)v0;
    SND_gs_SoundModifiers[0].linkedSoundDataSize = sizeof(tSoundModifierDataEmpty);
    v0->gen.sndIndexInAllocatedSounds = -1;
    v0->gen.flags = 0;
    v0->gen.sndKey = -1;
    SND_gs_SoundModifiers[1].sndModifierModelIntf = (tSoundModifierElemIntf*)MEM_p_Alloc(sizeof(tSoundModifierElemIntf));
    memcpy(SND_gs_SoundModifiers[1].sndModifierModelIntf, SND_gs_SoundModifiers[0].sndModifierModelIntf, sizeof(tSoundModifierElemIntf));
    SND_gs_SoundModifiers[1].sndModifierModelIntf->Type = 1;
    SND_gs_SoundModifiers[1].sndModifierModelIntf->Init = SND_SModifierFadeInit;
    SND_gs_SoundModifiers[1].sndModifierModelIntf->Update = SND_SModifierFadeUpdate;
    SND_gs_SoundModifiers[1].sndModifierModelIntf->InitChainedElemVersion = SND_SModifierFadeInit;
    SND_gs_SoundModifiers[1].sndModifierModelIntf->PlayApplyOnInstance = SND_SModifierFadeInitInstance;
    v1 = (tSoundModifierDataFade*)MEM_p_Alloc(sizeof(tSoundModifierDataFade));
    SND_gs_SoundModifiers[1].linkedSoundData = (char*)v1;
    SND_gs_SoundModifiers[1].linkedSoundDataSize = sizeof(tSoundModifierDataFade);
    v1->gen.sndKey = -1;
    v1->gen.sndIndexInAllocatedSounds = -1;
    v1->gen.flags = 0;
    v1->i5 = 1;
    v1->i1 = 0.0;
    v1->i2 = 1.0;
    v1->i3 = 1.0;
    v1->i4 = 1.0;
    SND_gs_SoundModifiers[3].sndModifierModelIntf = (tSoundModifierElemIntf*)MEM_p_Alloc(sizeof(tSoundModifierElemIntf));
    memcpy(SND_gs_SoundModifiers[3].sndModifierModelIntf, SND_gs_SoundModifiers[1].sndModifierModelIntf, sizeof(tSoundModifierElemIntf));
    SND_gs_SoundModifiers[3].sndModifierModelIntf->Type = 3;
    SND_gs_SoundModifiers[3].sndModifierModelIntf->Init = SND_SModifierFadeInit;
    SND_gs_SoundModifiers[3].sndModifierModelIntf->Update = SND_SModifierFadeInUpdate;
    SND_gs_SoundModifiers[3].sndModifierModelIntf->InitChainedElemVersion = SND_SModifierFadeInit;
    SND_gs_SoundModifiers[3].sndModifierModelIntf->PlayApplyOnInstance = SND_SModifierFadeApplyOnInstance;
    SND_gs_SoundModifiers[3].sndModifierModelIntf->NotifyPause = SND_SModifierFadeInOrOutNotifyPause;
    SND_gs_SoundModifiers[3].sndModifierModelIntf->NotifyPlay = SND_SModifierFadeInNotifyPlay;
    SND_gs_SoundModifiers[3].sndModifierModelIntf->NotifyStop = SND_SModifierFadeInNotifyStop;
    SND_gs_SoundModifiers[3].linkedSoundData = SND_gs_SoundModifiers[1].linkedSoundData;
    SND_gs_SoundModifiers[3].linkedSoundDataSize = SND_gs_SoundModifiers[1].linkedSoundDataSize;
    SND_gs_SoundModifiers[4].sndModifierModelIntf = (tSoundModifierElemIntf*)MEM_p_Alloc(sizeof(tSoundModifierElemIntf));
    memcpy(SND_gs_SoundModifiers[4].sndModifierModelIntf, SND_gs_SoundModifiers[3].sndModifierModelIntf, sizeof(tSoundModifierElemIntf));
    SND_gs_SoundModifiers[4].sndModifierModelIntf->Type = 4;
    SND_gs_SoundModifiers[4].sndModifierModelIntf->Init = SND_SModifierFadeInit;
    SND_gs_SoundModifiers[4].sndModifierModelIntf->Update = SND_SModifierFadeOutUpdate;
    SND_gs_SoundModifiers[4].sndModifierModelIntf->InitChainedElemVersion = SND_SModifierFadeInit;
    SND_gs_SoundModifiers[4].sndModifierModelIntf->NotifyPause = SND_SModifierFadeInOrOutNotifyPause;
    SND_gs_SoundModifiers[4].sndModifierModelIntf->NotifyPlay = SND_SModifierFadeOutNotifyPlay;
    SND_gs_SoundModifiers[4].sndModifierModelIntf->NotifyStop = SND_SModifierFadeOutNotifyStop;
    v2 = (tSoundModifierDataFade*)MEM_p_Alloc(sizeof(tSoundModifierDataFade));
    SND_gs_SoundModifiers[4].linkedSoundData = (char *)v2;
    SND_gs_SoundModifiers[4].linkedSoundDataSize = sizeof(tSoundModifierDataFade);
    v2->gen.sndKey = -1;
    v2->gen.flags = 0;
    v2->gen.sndIndexInAllocatedSounds = -1;
    v2->i5 = 0x80;
    v2->i1 = 1.0;
    v2->i2 = 0.0;
    v2->i3 = 1.0;
    v2->i4 = 1.0;
    SND_gs_SoundModifiers[2].sndModifierModelIntf = (tSoundModifierElemIntf*)MEM_p_Alloc(sizeof(tSoundModifierElemIntf));
    memcpy(SND_gs_SoundModifiers[2].sndModifierModelIntf, SND_gs_SoundModifiers[0].sndModifierModelIntf, sizeof(tSoundModifierElemIntf));
    SND_gs_SoundModifiers[2].sndModifierModelIntf->Type = 2;
    SND_gs_SoundModifiers[2].sndModifierModelIntf->Init = SND_SModifierPlayInit;
    SND_gs_SoundModifiers[2].sndModifierModelIntf->InitChainedElemVersion = SND_SModifierPlayInit;
    SND_gs_SoundModifiers[2].sndModifierModelIntf->PlayApplyOnInstance = SND_SModifierPlayApplyOnInstance;
    SND_gs_SoundModifiers[2].sndModifierModelIntf->PlayApplyOnSound = SND_SModifierPlayApplyOnSound;
    SND_gs_SoundModifiers[2].sndModifierModelIntf->NotifyPlay = SND_SModifierPlayerNotifyPlay;
    v3 = (tSoundModifierDataPlayer*)MEM_p_Alloc(sizeof(tSoundModifierDataPlayer));
    SND_gs_SoundModifiers[2].linkedSoundData = (char *)v3;
    SND_gs_SoundModifiers[2].linkedSoundDataSize = sizeof(tSoundModifierDataPlayer);
    v3->gen.sndKey = -1;
    v3->gen.flags = 0x100000;
    v3->gen.sndIndexInAllocatedSounds = -1;
    v3->ffar = 20.0f;
    v3->fnear = 0.0;
    v3->vol = 0.5f;
    v3->freq = 44100;
    v3->flags = 64;
    v3->i6 = 2;
    v3->timeRemainTillStart = 0.0;
    SND_gs_SoundModifiers[5].sndModifierModelIntf = (tSoundModifierElemIntf*)MEM_p_Alloc(sizeof(tSoundModifierElemIntf));
    memcpy(SND_gs_SoundModifiers[5].sndModifierModelIntf, SND_gs_SoundModifiers[2].sndModifierModelIntf, sizeof(tSoundModifierElemIntf));
    SND_gs_SoundModifiers[5].sndModifierModelIntf->Type = 5;
    SND_gs_SoundModifiers[5].sndModifierModelIntf->Init = SND_SModifierExtPlayerInit;
    SND_gs_SoundModifiers[5].sndModifierModelIntf->InitChainedElemVersion = SND_SModifierExtPlayerInit;
    SND_gs_SoundModifiers[5].sndModifierModelIntf->PlayApplyOnInstance = SND_SModifierExtPlayerApplyOnInstance;
    SND_gs_SoundModifiers[5].sndModifierModelIntf->PlayApplyOnSound = SND_SModifierExtPlayerApplyOnSound;
    SND_gs_SoundModifiers[5].sndModifierModelIntf->NotifyPlay = SND_SModifierExtPlayerNotifyPlay;
    v4 = (tSoundModifierDataExtPlayer*)MEM_p_Alloc(sizeof(tSoundModifierDataExtPlayer));
    SND_gs_SoundModifiers[5].linkedSoundData = (char *)v4;
    SND_gs_SoundModifiers[5].linkedSoundDataSize = sizeof(tSoundModifierDataExtPlayer);
    v4->gen.sndKey = -1;
    v4->gen.sndIndexInAllocatedSounds = -1;
    v4->idx = -1;
    v4->trackID = -1;
    v4->gen.flags = 0x100000;
    v4->i1 = 64;
    v4->i3 = 16;
    v4->ffar = 20.0f;
    v4->ffar2 = 20.0f;
    v4->ffar3 = 20.0f;
    v4->fnear = 0.0;
    v4->fnear2 = 0.0;
    v4->fnear3 = 0.0;
    v4->unkIsInActiveVolume = 2.0f;
    v4->i6 = 0.0;
    v4->i7 = 0.0;
    v4->vol = 0.5;
    v4->vol2 = 0.0;
    v4->familyID = 0;
    v4->pan = 0;
    v4->freq = 0;
    v4->interpolationFactor = 0.5f;
    v4->interpolationFactor2 = 0.5f;
    v4->interpolationFactor3 = 0.5f;
    v4->farCoefficient = 0.0;
    v4->middleCoefficient = 0.5f;
    v4->nearfar = 0.0;
    v4->i25 = 3;
    v4->timeRemainTillStart = 0.0;
    SND_gs_SoundModifiers[6].sndModifierModelIntf = (tSoundModifierElemIntf*)MEM_p_Alloc(sizeof(tSoundModifierElemIntf));
    memcpy(SND_gs_SoundModifiers[6].sndModifierModelIntf, SND_gs_SoundModifiers[0].sndModifierModelIntf, sizeof(tSoundModifierElemIntf));
    SND_gs_SoundModifiers[6].sndModifierModelIntf->Type = 6;
    SND_gs_SoundModifiers[6].sndModifierModelIntf->Init = SND_SModifier3DSettingsInit;
    SND_gs_SoundModifiers[6].sndModifierModelIntf->InitChainedElemVersion = SND_SModifier3DSettingsInit;
    SND_gs_SoundModifiers[6].sndModifierModelIntf->PlayApplyOnInstance = SND_SModifier3DSettingsApplyOnInstance;
    v5 = (tSoundModifierData3DSettings*)MEM_p_Alloc(sizeof(tSoundModifierData3DSettings));
    SND_gs_SoundModifiers[6].linkedSoundData = (char *)v5;
    SND_gs_SoundModifiers[6].linkedSoundDataSize = sizeof(tSoundModifierData3DSettings);
    v5->middleCoefficient = 0.5f;
    v5->i1 = 0.5f;
    v5->i2 = 0.5f;
    v5->i3 = 0.5f;
    v5->gen.flags = 0;
    v5->farCoefficient = 0.0;
    v5->nearfar = 0.0;
    v5->gen.sndKey = -1;
    v5->gen.sndIndexInAllocatedSounds = -1;
}