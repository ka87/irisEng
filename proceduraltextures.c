#include "memory.h"
#include "loading.h"
#include "time.h"
#include "bigfiles.h"
#include "bink.h"
#include "inputoutput.h"
#include "textures.h"

tProceduralIntf TEX_gast_ProceduralInterface[6] = { 0 };
int TexPro_WaterDistruptCounter = 0; // game doesn't sets to 0
int gBinkReadUsedSlots = 0; // --||--
float KX_442[2];
float KY_443[2];
float ALPHA_441[2];

struct sTexProcWater* TEXPRO_pv_WaterLoad(char* offset, int vmilyensize, int width, int height)
{
    unsigned int v4;
    tTexProcWater* v5;
    __int16* v6;
    __int16* v7;
    int v8;
    long double v10;
    unsigned int v11;

    v4 = 2 * height * width;
    v5 = (tTexProcWater*)MEM_p_Alloc(sizeof(tTexProcWater));
    v6 = (__int16*)MEM_p_Alloc(v4);
    v5->ptr1 = v6;
    memset(v6, 0, v4);
    v7 = (__int16*)MEM_p_Alloc(v4);
    v5->ptr2 = v7;
    memset(v7, 0, v4);
    v8 = 256;
    do
    {
        --v8;
        v10 = fabs(sin((double)v8 * 0.049087387));
        if (v8 <= 128)
            v11 = 0;
        else
            v11 = (unsigned int)(v10 * v10 * (v10 * v10) * 128.0);
        v5->values[v8] = v11 | (((unsigned int)v11 | (((unsigned int)v11 | ((unsigned int)(unsigned int)(255.0 * v10) << 8)) << 8)) << 8);
    } while (v8);
    return v5;
}

int TEXPRO_l_WaterUpdate(tTexProceduralStruct* a1)
{
    unsigned int v1;
    __int16* v2;
    __int16* v3;
    int v4;
    int v5;
    __int16* v6;
    int v7;
    __int16* v8;
    __int16* v9;
    __int16* v10;
    __int16* v11;
    int v12;
    __int16 v13;
    unsigned __int16 v14;
    __int16* v15;
    __int16* v16;
    int* i; 
    int j;
    unsigned __int16 v19;
    unsigned __int16 v20;
    tTexProcWater* v22;
    int v23;
    int v24;
    int v25;

    v22 = (tTexProcWater*)a1->proceduralTexPtr;
    v1 = 2 * a1->width;
    v2 = &v22->ptr1[v1 / 2 + 1];
    v24 = a1->height;
    v23 = a1->width;
    v3 = &v22->ptr2[v1 / 2 + 1];
    memset(v22->ptr1, 0, v1);
    memset(v22->ptr2, 0, v1);
    memset((char*)v22->ptr1 + v23 * (2 * v24 - 2), 0, v1);
    memset((char*)v22->ptr2 + v23 * (2 * v24 - 2), 0, v1);
    v4 = v24;
    if (v24 != 2)
    {
        v25 = v24 - 2;
        do
        {
            v5 = v23 - 2;
            if (v23 != 2)
            {
                v6 = &v3[v1 / -2];
                do
                {
                    v7 = ((v3[1] + *v6 + *(v3 - 1) + v3[v1 / 2]) >> 1)
                        - *v2
                        - ((((v3[1] + *v6 + *(v3 - 1) + v3[v1 / 2]) >> 1) - *v2) >> 8);
                    if (v7 >= (int)0xFFFFE000)
                    {
                        if (v7 > 0x2000)
                            v7 = (v7 & 0xffff0000) | 0x2000;
                    }
                    else
                    {
                        v7 =  (v7 & 0xffff0000) | 0xE000;
                    }
                    *v2++ = v7;
                    ++v3;
                    ++v6;
                    --v5;
                } while (v5);
                v4 = v24;
            }
            *v2 = 0;
            v8 = v2 + 1;
            *v8 = 0;
            *v3 = 0;
            v9 = v3 + 1;
            *v9 = 0;
            v2 = v8 + 1;
            v3 = v9 + 1;
            --v25;
        } while (v25);
    }
    v10 = v22->ptr1;
    v11 = v22->ptr2;
    if (v23 * v4)
    {
        v12 = v23 * v4;
        do
        {
            v13 = *v10;
            v14 = *v10++ >> 6;
            ++v11;
            *(v10 - 1) = v13 - v14;
            --v12;
            *(v11 - 1) -= *(v11 - 1) >> 6;
        } while (v12);
    }
    v15 = v22->ptr2;
    v22->ptr2 = v22->ptr1;
    v22->ptr1 = v15;
    v16 = v15;
    for (i = a1->proceduralPicData; v4; --v4)
    {
        for (j = v23; j; *(i - 1) = v22->values[(char)((v19 - v20) >> 6) + 128])
        {
            v19 = *v16;
            v20 = v16[1];
            ++i;
            ++v16;
            --j;
        }
    }
    return 1;
}

void TEXPRO_WaterDisrupt(tTexProceduralStruct* a1)
{
    tTexProcWater* v1;
    unsigned int v2;
    unsigned int v3;
    __int16* v4;
    __int16* v5;
    __int16* v6;
    unsigned int v7;
    int v8;
    long double v9;
    long double v10;
    unsigned int v11;
    unsigned int v12;
    int v13; 
    unsigned int v14;
    unsigned int v15;
    int v16;
    float v17;
    float v18;
    signed int v19;
    int idx;

    v1 = (tTexProcWater*)a1->proceduralTexPtr;
    v2 = a1->width;
    v3 = a1->height;
    v4 = v1->ptr2;
    ++TexPro_WaterDistruptCounter;
    v15 = v3;
    v14 = v2;
    v5 = &v1->ptr1[v2];
    v6 = &v4[v2];
    v7 = 4;
    v16 = RND_JADErand() % 100;
    while (1)
    {
        --v7;
        v8 = RND_JADErand();
        if (v7 >= 2)
        {
            v19 = v8 % (int)(v2 - 8) + 4;
            v11 = RND_JADErand() % (int)(v15 - 8) + 4;
        }
        else
        {
            v9 = (double)(50 - v8 % 100) * 0.0049999999 + ALPHA_441[v7];
            ALPHA_441[v7] = (float)v9;
            v18 = (float)(unsigned int)v16;
            v17 = (float)(sin(v9) * v18 * 0.02 + KX_442[v7]);
            KX_442[v7] = v17;
            v10 = (float)(cos(v9) * v18 * 0.02 + KY_443[v7]);
            KY_443[v7] = (float)v10;
            v19 = (unsigned int)v17 % v2;
            v11 = (unsigned int)v10 % v15;
            if (v19 > (int)(v14 - 8))
                v19 = v14 - 8;
            if ((int)((unsigned int)v10 % v15) > (int)(v15 - 8))
                v11 = v15 - 8;
        }
        v12 = (unsigned int)(sin((double)TexPro_WaterDistruptCounter * 0.2) * 200.0);
        v13 = v19 + v14 * v11;
        v5[v13] += v12;
        idx = 2 * (v19 + v14 * (v11 + 1));
        v5[idx] += v12;
        v5[v13 + 1] += v12;
        v5[idx + 1] += v12;
        v6[v13] += v12;
        v6[idx] += v12;
        v6[v13 + 1] += v12;
        v6[idx + 1] += v12;
        if (!(v7 * 4))
            break;
        v2 = v14;
    }

}

int TEX_l_File_LoadProcedural(char* a1, tEngTexIntermediateStruct* a2, int readPtr)
{
    int v3;
    int v4;
    tEngTexIntermediateStruct* v5;
    int v6;
    __int16 v7;
    __int16 v8;
    int v9;

    v3 = readPtr;
    v4 = 0;
    if (readPtr)
    {
        v4 = LOA_ReadLong(&a1, 0, 1);
        v8 = LOA_ReadShort(&a1, 0, 1);
        v5 = a2;
        v7 = v8;
        v5->realWidth = LOA_ReadShort(&a1, 0, 1);
        v5->realHeight = LOA_ReadShort(&a1, 0, 1);
        v6 = (uint16_t)LOA_ReadShort(&a1, 0, 1);
    }
    else
    {
        v5 = a2;
        a2->realWidth = 128;
        v5->realHeight = 128;
        v6 = 0;
        v7 = 0;
    }
    v5->flags |= 0x20u;
    v5->colorDepth1 = 32;
    v5->colorDepth2 = 32;
    v5->alphaMask = 0xFF000000;
    v5->redMask = 0xFF0000;
    v5->greenMask = 0xFF00;
    v5->blueMask = 0xFF;
    if ((v7 & 1) != 0)
        v5->tp.flagsUnkAndProceduralUnkb1 |= 0x200; // HIBYTE(v5->tp.flagsUnkAndProceduralUnkb1) |= 2
    v9 = v5->realWidth;
    v5->tp.proceduralParams.interfaceIdx = v6;
    v5->tp.proceduralParams.interfacePtr = TEX_gast_ProceduralInterface[v6].Load(a1, v3 - v4, v9, v5->realHeight); // FIXME, értékadás
    v5->tp.proceduralParams.flags = v7;
    return 1;
}

void TEX_Procedural_Add(__int16 elemIdx, int a2, __int16 a3, void* a4)
{
    int v4;
    tTexProceduralStruct* v5;
    tTexProceduralStruct* v6;
    tEngTexListElemStruct* v7;
    tEngTexListElemStruct* v8;

    v4 = TEX_gi_NbProcedural;
    if (!TEX_gi_NbProcedural)
    {
        v5 = (tTexProceduralStruct*)MEM_p_Alloc(16 * sizeof(tTexProceduralStruct));
        v4 = TEX_gi_NbProcedural;
        TEX_gdst_Procedural = v5;
    }
    if (v4 < 16)
    {
        v6 = &TEX_gdst_Procedural[v4];
        TEX_gi_NbProcedural = v4 + 1;
        v7 = TEX_gst_GlobalList.texElems;
        v6->elemIdx = elemIdx;
        v8 = &v7[elemIdx];
        v6->s2 = a3;
        v6->height = v8->normalData.width;
        v6->width = v8->normalData.height;
        v6->proceduralPicData = 0;
        v6->proceduralIntf = &TEX_gast_ProceduralInterface[a2];
        v6->proceduralTexPtr = a4;
    }
}

void PTR4* RADLINK calls_mem_Alloc_sub_46D600(U32 bytes)
{
    return (void PTR4*)MEM_p_Alloc(bytes);
}

void RADLINK calls_MEM_free_sub_46D610(void PTR4* ptr) 
{
    MEM_Free((void*)ptr);
}

void TEXPRO_Bink_RemoveActSlotIDFromUsedSlots(tBinkUnkStruct* a1)
{
    int v1;
    int v2;

    v1 = 1 << a1->actSlotID;
    v2 = gBinkReadUsedSlots;
    a1->stateMachine2MeansBusy = 1;
    a1->actSlotID = -1;
    gBinkReadUsedSlots = ~v1 & v2;
}

int TEXPRO_Bink_GetNextFreeSlotID()
{
    int result;

    result = 0;
    if ((gBinkReadUsedSlots & 1) != 0)
    {
        do
            ++result;
        while (((1 << result) & gBinkReadUsedSlots) != 0);
    }
    return result;
}

int TEXPRO_Bink_ReadActBlock(tBinkIFPtr* a1)
{
    tBinkUnkStruct* v1;
    int v2;
    tBinkUnkStruct* v3;
    int v4;
    int v5;
    int v6;

    a1->actualElemInLinkedList->actSlotID = TEXPRO_Bink_GetNextFreeSlotID();
    v1 = a1->actualElemInLinkedList;
    if (v1->actSlotID >= 10u)
        return 0;
    v2 = a1->size;
    if (!v2)
        return 0;
    if (v2 >= 4096)
        v2 = 4096;
    BINK_SeekAndReadFromBF(a1->filePosInBF, v1->blockDataStartPtr, v2);
    v3 = a1->actualElemInLinkedList;
    v4 = a1->size;
    gBinkReadUsedSlots |= 1 << v3->actSlotID;
    v5 = 4096;
    if (v4 < 4096)
        v5 = v4;
    a1->filePosInBF += v5;
    v6 = 4096;
    if (v4 < 4096)
        v6 = v4;
    a1->size = v4 - v6;
    v3->stateMachine2MeansBusy = 2;
    TEXPRO_Bink_RemoveActSlotIDFromUsedSlots(a1->actualElemInLinkedList);
    return 1;
}

void TEXPRO_BINK_ReadActBlockAndAdvance(tBinkIFPtr* a1)
{
    tBinkUnkStruct* v1;

    if (a1->dataAndLinkedList && !a1->actualElemInLinkedList->stateMachine2MeansBusy)
    {
        do
        {
            if (!TEXPRO_Bink_ReadActBlock(a1))
                break;
            v1 = a1->actualElemInLinkedList->nextDataBlock;
            a1->actualElemInLinkedList = v1;
        } while (!v1->stateMachine2MeansBusy);
    }
}

int TEXPRO_Bink_GetAlreadyProcessedFileSize(tBinkIFPtr* a1)
{
    int result;
    int v2;
    tBinkUnkStruct* v3;

    if (!a1->dataAndLinkedList || !a1->size)
        return -1;
    v2 = a1->dataBlockStartCurrOffset;
    v3 = a1->linkedListStart;
    result = 0;
    if (v3->stateMachine2MeansBusy == 1)
    {
        do
        {
            v3 = v3->nextDataBlock;
            result += 4096 - v2;
            v2 = 0;
        } while (a1->linkedListStart != v3 && v3->stateMachine2MeansBusy == 1);
    }
    return result;
}

void BINK_releaseLinkedList(tBinkIFPtr* a1)
{
    tBinkUnkStruct* v1;
    tBinkUnkStruct* i;

    if (a1->dataAndLinkedList)
    {
        v1 = a1->actualElemInLinkedList;
        for (i = v1->nextDataBlock; i != v1; i = i->nextDataBlock)
        {
            while (i->stateMachine2MeansBusy == 2);
            i->stateMachine2MeansBusy = 0;
        }
        while (i->stateMachine2MeansBusy == 2);
        i->stateMachine2MeansBusy = 0;
        a1->linkedListStart = i;
        a1->actualElemInLinkedList = i;
        a1->dataBlockStartCurrOffset = 0;
    }
}

void PC_VideoStreamRestart_Bink(tBinkIFPtr* a1)
{
    BINK_IF_PTR_GLOBAL = a1;
    if (a1->filePosInBF)
    {
        BinkClose(a1->binkHandle);
        BINK_releaseLinkedList(a1);
        PC_VideoStreamCreate_Bink(a1, a1->binkVideoKey);
    }
}

void PC_VideoStreamSync_Bink(tBinkIFPtr* a1)
{
    int v1;

    BINK_IF_PTR_GLOBAL = a1;
    if (a1->filePosInBF)
    {
        if (a1->videofilesize >= TEXPRO_Bink_GetAlreadyProcessedFileSize(a1) || BinkWait(a1->binkHandle))
        {
        LABEL_9:
            TEXPRO_BINK_ReadActBlockAndAdvance(a1);
        }
        else
        {
            v1 = 0;
            while (1)
            {
                BinkDoFrame(a1->binkHandle);
                if (a1->binkHandle->FrameNum >= a1->binkHandle->Frames)
                    break;
                BinkNextFrame(a1->binkHandle);
                ++v1;
                if (BinkWait(a1->binkHandle) || v1 >= 3)
                {
                    BinkCopyToBuffer(a1->binkHandle, a1->proceduralStructPtr->proceduralPicData, 4 * a1->proceduralStructPtr->width, a1->proceduralStructPtr->height, 0, 0, BINKSURFACE32 | BINKCOPYALL);
                    goto LABEL_9;
                }
            }
            PC_VideoStreamRestart_Bink(a1);
        }
    }
}


U32 RADLINK BINK_GetBufferSize(BINKIO* a1, U32 a2)
{
    return 2048;
}

int TEXPRO_Bink_ReadAndCopyNextBlockToReadBuffer(tBinkIFPtr* a1, char* dest, signed int a3)
{
    signed int v4;
    tBinkUnkStruct* v6;
    signed int v7;
    int v8;

    if (!a1->dataAndLinkedList)
        return -1;
    v4 = a3;
    if (!a3)
        return 0;
    while (1)
    {
        v6 = a1->linkedListStart;
        if (v6->stateMachine2MeansBusy != 1)
        {
            a1->dataBlockStartCurrOffset = 0;
            if (!v6->stateMachine2MeansBusy)
                TEXPRO_BINK_ReadActBlockAndAdvance(a1);
            v6 = a1->linkedListStart;
            while (v6->stateMachine2MeansBusy == 2)
                ;
            if (v6->stateMachine2MeansBusy != 1)
                break;
        }
        v7 = 4096 - a1->dataBlockStartCurrOffset;
        if (v4 <= v7)
            v7 = v4;
        memcpy(dest, &v6->blockDataStartPtr[a1->dataBlockStartCurrOffset], v7);
        v8 = v7 + a1->dataBlockStartCurrOffset;
        dest += v7;
        a1->dataBlockStartCurrOffset = v8;
        if (v8 == 4096)
        {
            a1->linkedListStart->stateMachine2MeansBusy = 0;
            a1->linkedListStart = a1->linkedListStart->nextDataBlock;
            a1->dataBlockStartCurrOffset = 0;
        }
        a3 -= v7;
        if (!a3)
            return 0;
        v4 = a3;
    }
    return -1;
}

int TEXPRO_BINK_ReadNumBytes(BINKIO* a1, void* DstBuf, unsigned int MaxCharCount)
{
    if (TEXPRO_Bink_ReadAndCopyNextBlockToReadBuffer(BINK_IF_PTR_GLOBAL, (char*)DstBuf, MaxCharCount))
    {
        BINK_SeekAndReadFromBF(BINK_IF_PTR_GLOBAL->filePosInBF, DstBuf, MaxCharCount);
        BINK_IF_PTR_GLOBAL->filePosInBF += MaxCharCount;
    }
    a1->BytesRead = MaxCharCount;
    return 0;
}

U32 RADLINK BINK_ReadHeader(BINKIO* a1, S32 a2, void PTR4* DstBuf, U32 MaxCharCount)
{
    return TEXPRO_BINK_ReadNumBytes(a1, DstBuf, MaxCharCount);
}

U32 RADLINK BINK_ReadFrame(BINKIO* a1, U32 a2, S32 a3, void* DstBuf, U32 MaxCharCount)
{
    return TEXPRO_BINK_ReadNumBytes(a1, DstBuf, MaxCharCount);
}

void RADLINK BINK_SetInfo(struct BINKIO PTR4* Bnkio, void PTR4* Buf, U32 Size, U32 FileSize, U32 simulate) {
}

U32 RADLINK BINK_Idle(struct BINKIO PTR4* Bnkio) {
    return 0;
}

void TEXPRO_BINK_Close(tBinkIFPtr* a1)
{
    if (a1->dataAndLinkedList)
    {
        BINK_releaseLinkedList(a1);
        MEM_Free(a1->dataAndLinkedList);
        a1->dataBlockStartCurrOffset = 0;
        a1->actualElemInLinkedList = 0;
        a1->linkedListStart = 0;
        a1->dataAndLinkedList = 0;
    }
}

void RADLINK BINK_Close(struct BINKIO PTR4* Bnkio)
{
    BINK_IF_PTR_GLOBAL->filePosInBF = 0;
    TEXPRO_BINK_Close(BINK_IF_PTR_GLOBAL);
}

void BINK_AllocLinkedList(tBinkIFPtr* binkIFPtr, int videosize)
{
    unsigned int v2;
    unsigned int v3;
    char* v4;
    char* v5;
    tBinkUnkStruct* v6;

    binkIFPtr->videofilesize = videosize;
    v2 = 2 * (videosize & 0xFFFFF000) + 0x2000;
    v3 = v2 >> 12;
    v4 = (char*)MEM_p_Alloc(16 * (v2 >> 12) + v2);// >> 12 => / 4096
    binkIFPtr->dataAndLinkedList = v4;
    memset(v4, 0, 4 * ((16 * (v2 >> 12) + v2) >> 2));
    v5 = binkIFPtr->dataAndLinkedList;
    v6 = (tBinkUnkStruct*)&v5[v2];
    binkIFPtr->linkedListStart = (tBinkUnkStruct*)&v5[v2];
    binkIFPtr->actualElemInLinkedList = (tBinkUnkStruct*)&v5[v2];
    if (v2 >> 12)
    {
        do
        {
            v6->blockDataStartPtr = v5;
            v5 += 4096;
            --v3;
            v6->stateMachine2MeansBusy = 0;
            v6->nextDataBlock = v6 + 1;
            ++v6;
        } while (v3);
    }
    v6[-1].nextDataBlock = (tBinkUnkStruct*)&binkIFPtr->dataAndLinkedList[v2];// ez csinálja meg a loopot, gondolom ha a végére ér az elejére ugorjon
    binkIFPtr->dataBlockStartCurrOffset = 0;
}

S32 RADLINK BINK_IO_FUNC(BINKIO* binkIOPtr, const char PTR4* name, U32 flags)
{
    int chunkSize;
    int videoSize;

    BINK_IF_PTR_GLOBAL->filePosInBF = BIG_ul_SearchKeyToPos(BINK_IF_PTR_GLOBAL->binkVideoKey);

    if (BINK_IF_PTR_GLOBAL->filePosInBF == -1)
        BINK_IF_PTR_GLOBAL->filePosInBF = 0;

    if (!BINK_IF_PTR_GLOBAL->filePosInBF)
        return 0;

    BINK_SeekAndReadFromBF(BINK_IF_PTR_GLOBAL->filePosInBF, &chunkSize, 4);
    
    if (chunkSize == 8)
    {
        BINK_IF_PTR_GLOBAL->filePosInBF = 0;
        return 0;
    }

    BINK_IF_PTR_GLOBAL->size = chunkSize;
    BINK_IF_PTR_GLOBAL->filePosInBF += 4;
    
    BINK_SeekAndReadFromBF(BINK_IF_PTR_GLOBAL->filePosInBF + 12, &videoSize, 4);
    BINK_AllocLinkedList(BINK_IF_PTR_GLOBAL, videoSize);
    
    binkIOPtr->GetBufferSize = BINK_GetBufferSize;
    binkIOPtr->ReadHeader = BINK_ReadHeader;
    binkIOPtr->ReadFrame = BINK_ReadFrame;
    binkIOPtr->SetInfo = BINK_SetInfo;
    binkIOPtr->Idle = BINK_Idle;
    binkIOPtr->Close = BINK_Close;
    
    return -1;
}

void PC_VideoStreamCreate_Bink(tBinkIFPtr* binkIFPtr, int videoKey)
{
    BINK_IF_PTR_GLOBAL = binkIFPtr;
    binkIFPtr->binkVideoKey = videoKey;
    BinkSetSoundTrack(0, 0);
    BinkSetMemory(calls_mem_Alloc_sub_46D600, calls_MEM_free_sub_46D610);
    BinkSetIOSize(2048);
    BinkSetIO(BINK_IO_FUNC);
    binkIFPtr->binkHandle = BinkOpen((char*)binkIFPtr, BINKNOTHREADEDIO | BINKIOPROCESSOR | BINKIOSIZE | BINKSNDTRACK);
}

void TEXPRO_MPEG_Start()
{
    int v0;

    v0 = TEXPRO_pst_CurrentMpeg->pos;
    if (v0 != -1)
    {
        TEXPRO_pst_CurrentMpeg->startOffset = v0 + 4;
        TEXPRO_pst_CurrentMpeg->flags &= ~0x100; //BYTE1(TEXPRO_pst_CurrentMpeg->flags) &= ~1u;
    }
}

int TEXPRO_MPEG_ReadNext(void* DstBuf)
{
    tTexProcMpeg* v1;
    signed int v3;

    v1 = TEXPRO_pst_CurrentMpeg;
    if (TEXPRO_pst_CurrentMpeg->pos == -1)
        return 0;
    v3 = TEXPRO_pst_CurrentMpeg->endOffset - TEXPRO_pst_CurrentMpeg->startOffset;
    if (v3 <= 2048)
    {
        TEXPRO_pst_CurrentMpeg->flags |= 100u; // BYTE1(TEXPRO_pst_CurrentMpeg->flags) |= 1u;
        v1 = TEXPRO_pst_CurrentMpeg;
    }
    else
    {
        v3 = 2048;
    }
    BINK_SeekAndReadFromBF(v1->startOffset, DstBuf, v3);
    TEXPRO_pst_CurrentMpeg->startOffset += v3;
    return v3;
}

tTexProcMpeg* TEXPRO_pv_MpegLoad(char* readptr, int vmilyensize, int width, int height)
{
    tTexProcMpeg* v4;
    int v5;
    BOOL v6;
    int v7;

    v4 = (tTexProcMpeg*)MEM_p_Alloc(sizeof(tTexProcMpeg));
    memset(v4, 0, sizeof(tTexProcMpeg));
    v5 = vmilyensize;
    v6 = vmilyensize == 0;
    v4->sub.bufferSize = 2048;
    v4->sub.Start = TEXPRO_MPEG_Start;
    v4->sub.ReadNext = TEXPRO_MPEG_ReadNext;
    if (v6 || v5 != LOA_ReadLong(&readptr, 0, 1))
    {
        v4->key = -1;
        v4->i6 = -1;
        v4->sub.binkKey = -1;
        v4->pos = -1;
    }
    else
    {
        v4->key = LOA_ReadLong(&readptr, 0, 1);
        if (v5 == 8)
        {
            v4->i6 = -1;
            v4->sub.binkKey = -1;
            v4->sub.ii5 = -1;
            v4->i8 = 0;
            v4->i9 = 0;
            v4->i11 = 0;
            v4->i10 = 0;
            v4->i7 = 0;
        }
        else if (v5 == 32)
        {
            v4->i6 = LOA_ReadLong(&readptr, 0, 1);
            v4->i7 = LOA_ReadLong(&readptr, 0, 1);
            v4->i10 = LOA_ReadLong(&readptr, 0, 1);
            v4->i11 = LOA_ReadLong(&readptr, 0, 1);
            v4->i9 = LOA_ReadLong(&readptr, 0, 1);
            v4->i8 = LOA_ReadLong(&readptr, 0, 1);
            v4->sub.binkKey = -1;
            v4->sub.ii3 = 0;
            v4->sub.ii1 = 0;
            v4->sub.ii2 = 0;
            v4->sub.ii5 = -1;
        }
        else
        {
            v4->i6 = LOA_ReadLong(&readptr, 0, 1);
            v4->i7 = LOA_ReadLong(&readptr, 0, 1);
            v4->i10 = LOA_ReadLong(&readptr, 0, 1);
            v4->i11 = LOA_ReadLong(&readptr, 0, 1);
            v4->i9 = LOA_ReadLong(&readptr, 0, 1);
            v4->i8 = LOA_ReadLong(&readptr, 0, 1);
            v4->sub.binkKey = LOA_ReadLong(&readptr, 0, 1);
            v4->sub.ii1 = LOA_ReadLong(&readptr, 0, 1);
            v4->sub.ii2 = LOA_ReadLong(&readptr, 0, 1);
            v4->sub.ii3 = LOA_ReadLong(&readptr, 0, 1);
            if (v5 == 48)
                v4->sub.ii5 = -1;
            else
                v4->sub.ii5 = LOA_ReadLong(&readptr, 0, 1);
        }
        v7 = BIG_ul_SearchKeyToPos(v4->key);
        v4->pos = v7;
        if (v7 != -1)
        {
            v4->startOffset = v7 + 4;
            v4->endOffset = v4->startOffset + BIG_ul_GetLengthFile(v7);
            TEXPRO_pst_CurrentMpeg = v4;
            if (!LOA_gb_SpeedMode || BIG_gi_ReadMode != 2)
            {
                //TODO FIXME: mpeg2dec should be linked, pointless isnce it's kind of a leftover and doesn't even work.
                //mpeg_init_and_open_sub_486910(&v4->sub);
                return v4;
            }
        }
    }
    return v4;
}

int TEXPRO_l_MpegUpdate(tTexProceduralStruct* a1)
{
    tTexProcMpeg* v1;
    tTexProcMpeg* v2;
    tBinkIFPtr* v3;
    char v4;

    v1 = (tTexProcMpeg*)a1->proceduralTexPtr;
    v2 = v1;
    TEXPRO_pst_CurrentMpeg = v1;
    if (v1->sub.binkKey != -1)
    {
        if (!v1->sub.binkIFPtr)
        {
            v3 = (tBinkIFPtr*)MEM_p_Alloc(sizeof(tBinkIFPtr));
            v1->sub.binkIFPtr = v3;
            memset(v3, 0, sizeof(tBinkIFPtr));
            v1->sub.binkIFPtr->proceduralStructPtr = a1;
            PC_VideoStreamCreate_Bink(v1->sub.binkIFPtr, v1->sub.binkKey);
            v2 = TEXPRO_pst_CurrentMpeg;
        }
        if ((v2->flags & 0x100) != 0 && (v2->flags & 2) != 0 && ((v2->flags >> 16) & 0xff) != 2) // ...BYTE2(v2->flags) != 2
        {
            v2->flags &= 0xff00ffff;
            v2->flags |= 0x00010000;
            v2 = TEXPRO_pst_CurrentMpeg;
        }
        v4 = (v2->flags >> 16) & 0xff; //v4 = BYTE2(v2->flags);
        if (v4)
        {
            if (v4 == 1)
            {
                v2->flags |= 1u;
                memset(a1->proceduralPicData, 0, 4 * a1->width * a1->height);
                TEXPRO_pst_CurrentMpeg->flags &= 0xff00ffff; //BYTE2(TEXPRO_pst_CurrentMpeg->flags) = 0;
                return 1;
            }
            if (v4 == 2)
            {
                v2->flags &= ~1u;
                PC_VideoStreamRestart_Bink(v1->sub.binkIFPtr);
                v2 = TEXPRO_pst_CurrentMpeg;
            }
            v2->flags &= 0xff00ffff; //BYTE2(v2->flags) = 0;
            v2 = TEXPRO_pst_CurrentMpeg;
        }
        if ((v2->flags & 1) != 0)
            return 0;
        PC_VideoStreamSync_Bink(v1->sub.binkIFPtr);
    }
    return 1;
}

void BINK_CloseAll(tBinkIFPtr* a1)
{
    BINK_IF_PTR_GLOBAL = a1;
    if (a1->filePosInBF)
    {
        BinkClose(a1->binkHandle);
        TEXPRO_BINK_Close(a1);
    }
}

void TEXPRO_MpegFree(tTexProceduralStruct* a1)
{
    tTexProcMpeg* v1;

    v1 = (tTexProcMpeg*)a1->proceduralTexPtr;
    if (v1->sub.binkKey != -1 && v1->sub.binkIFPtr)
    {
        BINK_CloseAll(v1->sub.binkIFPtr);
        MEM_Free(v1->sub.binkIFPtr);
        v1->sub.binkIFPtr = 0;
    }
    MEM_Free(v1);
}

tTexProFire* TEXPRO_pv_FireLoad(char* readptr, int vmilyensize, int width, int height)
{
    tTexProFire* v4;
    uint8_t* v5;
    uint8_t* v6;
    int v7;

    v4 = (tTexProFire*)MEM_p_Alloc(sizeof(tTexProFire));
    v5 = (uint8_t*)MEM_p_Alloc(height * width);
    v4->ptr = v5;
    memset(v5, 0, height * width);
    v6 = (uint8_t*)MEM_p_Alloc(height * width);
    v4->ptr2 = v6;
    memset(v6, 0, height * width);
    v7 = 64;
    do
    {
        --v7;
        v4->arr[v7] = (v7 | ((v7 | (v7 << 8)) << 8)) << 10;
        v4->arr[v7 + 64] = 4 * (v7 | 0xFFFFFFC0);
        v4->arr[v7 + 128] = -1;
        v4->arr[v7 + 172] = (4 * (63 - v7)) | ((63 - v7) << 18) | ((63 - v7) << 10) | ((-1 - v7) << 26);
    } while (v7);
    return v4;
}

int TEXPRO_l_FireUpdate(tTexProceduralStruct* a1)
{
    int v1;
    tTexProFire* v2;
    int v3;
    int v4;
    unsigned __int8* v5;
    unsigned __int8* v6;
    int v7;
    int v8;
    unsigned __int8* v9;
    unsigned __int8* v10;
    unsigned __int8* v11;
    unsigned __int8* v12;
    unsigned __int8* v13;
    int* i;
    int v15;
    int v17;
    int v18;
    int v19;
    tTexProFire* v20;

    v1 = a1->height;
    v2 = (tTexProFire*)a1->proceduralTexPtr;
    v3 = a1->width;
    v4 = v2->unknum;
    v20 = v2;
    v19 = v1;
    if (v4)
    {
        if (v4 == 1)
        {
            v10 = &v2->ptr[v3 * v1];
            if (v3 * (v1 - 2))
            {
                // TODO: ezeket jó lenne nem - offsetesre megcsinálni
                v11 = &v10[-2 * v3];
                v12 = &v10[-v3 + 1];
                v18 = v3 * (v1 - 2);
                do
                {
                    *v10-- = (*v12 + *v11-- + *(v12 - 1) + *(v12 - 2)) >> 2;
                    --v12;
                    --v18;
                } while (v18);
            }
        }
    }
    else
    {
        v5 = &v2->ptr[3 * v3 + 1];
        v6 = &v2->ptr2[v3 + 1];
        if (v1 != 4)
        {
            v17 = v1 - 4;
            do
            {
                v7 = v3 - 2;
                if (v3 != 2)
                {
                    do
                    {
                        v8 = (*(v6 - 1) + v6[1] + v6[-v3] + v6[v3]) >> 2;
                        *v5++ = v8 - ((unsigned __int8)v8 >> 4);
                        ++v6;
                        --v7;
                    } while (v7);
                    v2 = v20;
                    v1 = v19;
                }
                v5 += 2;
                v6 += 2;
                --v17;
            } while (v17);
        }
        v9 = v2->ptr;
        v2->ptr = v2->ptr2;
        v2->ptr2 = v9;
    }
    v13 = v2->ptr;
    for (i = (int*)a1->proceduralPicData; v3; --v3)
    {
        if (v1)
        {
            v15 = v1;
            do
            {
                *i++ = v2->arr[*v13++];
                --v15;
            } while (v15);
        }
    }
    return 1;
}

tTexProcPlasma* TEXPRO_pv_PlasmaLoad(char* readptr, int vmilyensize, int width, int height)
{
    int v4;
    int v5;
    unsigned __int8* v6;
    unsigned __int8* v7;
    int v8;
    unsigned __int8* v9;
    int v10;
    unsigned __int8* v11;
    int v12;
    int v13;
    int v14;
    int v15;
    int v16;
    int v17;
    unsigned __int8* v18;
    unsigned __int8* v19;
    unsigned __int8* v20;
    int v21;
    int v22;
    int v23;
    int v24;
    int v25;
    int v26;
    int v27;
    int i;
    int v31;
    tTexProcPlasma* v32;
    int v33;
    int v34;
    int v35;
    unsigned __int8* v36;
    unsigned __int8* v37;
    int v38;
    int v39;
    int v40;
    int j; 
    int v42;
    int v43;
    int v44;
    int v45;
    unsigned __int8* v46;

    v4 = width;
    v5 = height;
    v32 = (tTexProcPlasma*)MEM_p_Alloc(sizeof(tTexProcPlasma));
    v6 = (unsigned __int8*)MEM_p_Alloc(height * width);
    v32->ptr1 = v6;
    memset(v6, 0, height * width);
    v7 = (unsigned __int8*)MEM_p_Alloc(height * width);
    v32->ptr2 = v7;
    memset(v7, 0, height * width);
    v32->f1 = (float)((double)rand() * 0.000030518509 * 360.0);
    v8 = rand();
    v9 = v32->ptr1;
    v46 = v32->ptr2;
    v39 = 0;
    v32->f2 = (float)((double)v8 * 0.000030518509 * 360.0);
    do
    {
        v10 = 0;
        for (i = 0; v10 < v4; i = v10)
        {
            if (v5 > 0)
            {
                v11 = &v9[v10];
                do
                {
                    *v11 = RND_JADErand();
                    v11 += v4;
                    --v5;
                } while (v5);
                v10 = i;
                v5 = height;
            }
            ++v10;
        }
        v12 = 32;
        v38 = 32;
        do
        {
            v31 = 0;
            if (v4 > 0)
            {
                v13 = v12 / 2;
                v45 = v12 / 2;
                v34 = v12 / 2;
                do
                {
                    v14 = v12 + v31;
                    v15 = v12 + v31;
                    v44 = v12 + v31;
                    v35 = v12 + v31;
                    if (v12 + v31 == v4)
                    {
                        v35 = 0;
                        v15 = 0;
                    }
                    v40 = 0;
                    if (v5 > 0)
                    {
                        v16 = v4 * v13;
                        v33 = v12;
                        v17 = v4 * v12;
                        v18 = &v9[v16 + v34];
                        v19 = &v9[v16 + v15];
                        v20 = &v9[v15];
                        v21 = v31 - v15;
                        v36 = &v9[v15];
                        v37 = v18;
                        for (j = v31 - v15; ; v21 = j)
                        {
                            v22 = v33;
                            v43 = v20[v21];
                            v23 = *v20;
                            if (v33 == height)
                                v22 = 0;
                            v24 = width * v22;
                            v42 = v9[v24 + v31];
                            v25 = v9[v24 + v15];
                            *v19 = (v25 + v23) / 2;
                            v26 = v42 + v25;
                            *v37 = (v43 + v26 + v23) / 4;
                            v9[v24 + v34] = v26 / 2;
                            v33 += v38;
                            v36 += v17;
                            v19 += v17;
                            v40 += v38;
                            v37 += v17;
                            if (v40 >= height)
                                break;
                            v15 = v35;
                            v20 = v36;
                        }
                        v4 = width;
                        v12 = v38;
                        v14 = v44;
                        v13 = v45;
                        v5 = height;
                    }
                    v31 = v14;
                    v34 += v12;
                } while (v14 < v4);
            }
            v12 >>= 1;
            v38 = v12;
        } while (v12);
        if (v39)
        {
            memcpy(v46, v9, height * width);
            v5 = height;
        }
        ++v39;
    } while (v39 < 2);
    v27 = 0;
    do
    {
        v32->values[v27 + 64] = 4 * (v27 | 0xFFC00000);
        v32->values[v27] = 0xFF000000;
        v32->values[v27 + 128] = (4 * (64 - v27)) | 0xFF000000;
        v32->values[v27 + 172] = 0xFF000000;
        ++v27;
    } while (v27 < 64);
    return v32;
}

int TEXPRO_l_PlasmaUpdate(tTexProceduralStruct* a1)
{
    tTexProcPlasma* v1;
    int v2;
    long double v3;
    int v4;
    int v5;
    unsigned int v6; 
    double v7;
    double v10;
    int v11;
    int v14;
    int v15;
    int v16;
    int v17;
    int v18;
    int v19;
    int v20;
    int v22;
    int v23;
    int* v24;
    int v25;
    int v26;
    int v27;
    int v28;
    int v29;

    v1 = (tTexProcPlasma*)a1->proceduralTexPtr;
    v2 = v1->values[0];
    memcpy(v1->values, &v1->values[1], sizeof(v1->values) - 4); // egyel odébbtoljuk az egészet
    v3 = sin(v1->f1 * 0.017453292);
    v1->values[255] = (v2 & 0xFF000000) + ((uint16_t)v2 << 8) + ((v2 >> 16) && 0xff);
    v4 = a1->width;
    v5 = a1->height;
    v26 = v5;
    v24 = (int*)a1->proceduralPicData;
    v6 = (unsigned int)((v3 + 1.0) * (double)(v4 >> 1));
    v7 = v1->f1 + 1.666;
    v22 = v6;
    v1->f1 = (float)v7;
    if (v7 >= 360.0)
        v1->f1 = (float)(v7 - 360.0);
    v10 = v1->f2 + 1.88;
    v11 = (unsigned int)((sin(v1->f2 * 0.017453292) + 1.0) * (double)(v5 >> 1));
    v27 = v11;
    v1->f2 = (float)v10;
    if (v10 >= 360.0)
        v1->f2 = (float)(v10 - 360.0);
    v14 = 0;
    v28 = 0;
    if (v4 > 0)
    {
        v15 = v6 - v4;
        do
        {
            v23 = v14 + v22;
            if (v14 + v22 >= v4)
                v23 = v15 + v14;
            v16 = 0;
            if (v5 > 0)
            {
                v17 = v4 * v11;
                v18 = v4 * (v11 - v5);
                v29 = 0;
                v25 = v18;
                do
                {
                    v19 = v17;
                    if (v16 + v27 >= v26)
                        v19 = v18;
                    v20 = v1->ptr2[v19 + v23];
                    v14 = v28;
                    *v24++ = v1->values[(v1->ptr1[v29 + v28] + v20) >> 1];
                    v29 += v4;
                    v5 = v26;
                    ++v16;
                    v18 = v4 + v25;
                    v17 += v4;
                    v25 += v4;
                } while (v16 < v26);
                v15 = v22 - v4;
                v11 = v27;
            }
            v28 = ++v14;
        } while (v14 < v4);
    }
    return 1;
}

tTexProcPhoto* TEXPRO_pv_PhotoLoad(char* offset, int vmilyensize, int width, int height)
{
    tTexProcPhoto* v4;
    BOOL v5;

    v4 = (tTexProcPhoto*)MEM_p_Alloc(32u);
    v5 = vmilyensize == 0;
    v4->b2 = -1;
    v4->b1 = -1;
    v4->b3 = 0;
    v4->b4 = 0;
    v4->x = 0;
    if (!v5)
    {
        LOA_ReadLong(&offset, 0, 2);
        v4->b1 = LOA_ReadChar(&offset, 0, 1);
    }
    return v4;
}

int TEXPRO_l_PhotoUpdate(tTexProceduralStruct* a1)
{
    tTexProceduralStruct* v1;
    tTexProcPhoto* v2;
    int v3;
    double v4;
    int v8;
    int v9;
    int v10;
    int v11;
    int v12;
    int* v13;
    __int16* v14;
    int v15;
    int v16;
    int v17;
    int v18;
    int v19;
    int v20;
    int v21;
    int v22;
    int v23;
    int v24;
    int i;
    unsigned int v26;
    int v27;
    int v28;
    char v30;
    char* v31;
    unsigned __int16 v32;
    uint32_t* v33;
    unsigned __int16 v34;
    int* v35;
    size_t v36;
    int* v37;
    int v38;
    int v39;
    int* v40;
    int v41; 
    int* v42;
    int* v43;
    int* v44;
    int* v45;
    int v46; 
    int* v47;
    int* v48;
    int v49; 
    tTexProcPhoto* v50;
    int v51[2];
    size_t a3; 
    int v53; 
    int v54; 
    tEngTexIntermediateStruct a2;
    tEngTexIntermediateStruct a1a; 

    v1 = a1;
    v2 = (tTexProcPhoto*)a1->proceduralTexPtr;
    v3 = v2->x;
    v50 = v2;    
    if (v3 && v3 <= 128)
    {
        v4 = v2->remainingTime - TIM_gf_dt;
        v2->remainingTime = (float)v4;
        if (v4 < 0.0)     
            v2->x = 0;
        else
            v2->x = (unsigned int)(v4 / v2->f1 * 128.0);
        v8 = v2->x;
        if (v8 < v3)
        {
            v9 = v3 - 1;
            v49 = v9;
            if (v9 < v8)
                return 1;
            v10 = v9 << 10;
            v53 = v9 << 10;
            while (1)
            {
                if (v9 < 2 || v9 > 125)
                    goto LABEL_53;
                v11 = 0;
                v12 = 0;
                v13 = (int*)&v1->proceduralPicData[v10 + 8];
                v14 = &v50->sa[4];
                while (1)
                {
                    v54 = v12 + 1;
                    v15 = (v12 + 1) % 4;
                    v16 = *v14;
                    if (v16 == v49)
                        break;
                    if (v16 >= v49)
                    {
                        if (v16 <= v49)
                            goto LABEL_21;
                        v17 = v50->sa[v15 + 4];
                        if (v17 < v49)
                        {
                        LABEL_20:
                            v51[v11++] = *(v14 - 4) + (v49 - v16) * (v50->sa[v15] - *(v14 - 4)) / (v17 - v16);
                        LABEL_21:
                            if (v11 == 2)
                                goto LABEL_29;
                            goto LABEL_22;
                        }
                    }
                    else
                    {
                        v17 = v50->sa[v15 + 4];
                        if (v17 > v49)
                            goto LABEL_20;
                    }
                LABEL_22:
                    v12 = v54;
                    ++v14;
                    if (v54 >= 4)
                        goto LABEL_25;
                }
                if (v50->sa[v15 + 4] != v49)
                    break;
                v18 = v50->sa[v12 + 4];
                v51[0] = v50->sa[v12];
                v51[1] = v18;
            LABEL_25:
                if (v11 != 2)
                {
                    v19 = 252;
                    do
                    {
                        *v13 = LIGHT_ul_Interpol2Colors(*v13, 0, 0.5);
                        ++v13;
                        --v19;
                    } while (v19);
                    goto LABEL_52;
                }
            LABEL_29:
                v20 = v51[0];
                v21 = v51[1];
                if (v51[0] > v51[1])
                {
                    v21 = v51[0];
                    v20 = v51[1];
                    v51[1] = v51[0];
                    v51[0] = v20;
                }
                if (v20 > 254)
                {
                    v20 = 254;
                    v51[0] = 254;
                    goto LABEL_36;
                }
                if (v20 < 2)
                {
                    v20 = 2;
                    v51[0] = 2;
                }
                if (v21 > 254)
                    LABEL_36:
                v51[1] = 254;
                v22 = 2;
                if (v20 > 2)
                {
                    v23 = v20 - 2;
                    v22 = v23 + 2;
                    do
                    {
                        *v13 = LIGHT_ul_Interpol2Colors(*v13, 0, 0.5);
                        ++v13;
                        --v23;
                    } while (v23);
                }
                if ((v49 & 7) != 0)
                {
                    for (i = v51[1]; v22 < i; ++v22)
                    {
                        v26 = ~*v13 | 0xFF000000;
                        *v13 = v26;
                        if ((v22 & 7) == 0)
                            *v13 = v26 | 0xFF00FF00;
                        ++v13;
                    }
                }
                else if (v22 < v51[1])
                {
                    v24 = v51[1] - v22;
                    v22 = v51[1];
                    do
                    {
                        *v13 = ~*v13 | 0xFF00FF00;
                        ++v13;
                        --v24;
                    } while (v24);
                }
                if (v22 < 254)
                {
                    v27 = 254 - v22;
                    do
                    {
                        *v13 = LIGHT_ul_Interpol2Colors(*v13, 0, 0.5);
                        ++v13;
                        --v27;
                    } while (v27);
                }
            LABEL_52:
                v9 = v49;
                v1 = a1;
            LABEL_53:
                v28 = v50->x;
                --v9;
                v10 = v53 - 1024;
                v49 = v9;
                v53 -= 1024;
                if (v9 < v28)
                    return 1;
            }
            v51[v11++] = *(v14 - 4);
            goto LABEL_22;
        }
        return 0;
    }
    if (!v2->b4)
    {
        if (v2->b3 || v2->b2 != v2->b1)
            v2->b4 = 1;
        return 0;
    }
    v30 = v2->b2;
    v2->b4 = 0;
    v2->b3 = 0;
    v2->b1 = v30;
    if (v30 == -1)
        return 0;
    memset(&a2, 0, sizeof(a2));
    a2.flags = 3;
    if (v2->b1 > 100)
        goto LABEL_65;
    v31 = SAV_PhotoGet(v2->b1, (int*)&a3);
    if (!v31 || !a3)
        return 0;
    TEX_l_File_LoadJpeg(v31, &a2, a3);
LABEL_65:
    v32 = a1->width;
    v33 = a1->proceduralPicData;
    memcpy(&a1a, &a2, sizeof(a1a));
    a2.imgDataPtr = (char *)v33;
    a2.colorDepth1 = 32;
    if (a2.realWidth == v32 && a2.realHeight == a1->height)
    {
        TEX_l_ConvertPF(&a1a, &a2);
    }
    else
    {
        v34 = a1->height;
        a2.realWidth = v32;
        a2.realHeight = v34;
        TEX_l_ConvertSize_24To32(&a1a, &a2);
    }
    v35 = (int*)a2.imgDataPtr;
    v36 = 4 * a1->width * a1->height;
    v37 = (int*)&a2.imgDataPtr[v36];
    a3 = v36;
    if (a2.imgDataPtr < &a2.imgDataPtr[v36])
    {
        do
        {
            *v35 = *v35 & 0xFF00FF00 | *((unsigned __int8*)v35 + 2) | ((unsigned __int8)*v35 << 16);
            ++v35;
        } while (v35 < v37);
    }
    v38 = a1->width;
    v39 = a1->height;
    v40 = (int*)a1->proceduralPicData;
    v41 = 0;
    v42 = &v40[v38];
    v43 = &v40[v38 * (v39 - 2)];
    v44 = &v40[v38 * (v39 - 1)];
    if (a1->width)
    {
        do
        {
            *v40 = -1;
            *v42 = -1;
            *v43 = -1;
            *v44 = -1;
            ++v40;
            ++v42;
            ++v43;
            ++v44;
            ++v41;
        } while (v41 < a1->width);
    }
    v45 = (int*)a1->proceduralPicData;
    v46 = 0;
    if (a1->height)
    {
        do
        {
            *v45 = -1;
            v47 = v45 + 1;
            *v47 = -1;
            v48 = &v47[a1->width - 3];
            *v48++ = -1;
            *v48 = -1;
            v45 = v48 + 1;
            ++v46;
        } while (v46 < a1->height);
    }
    if (v50->x == 129)
        v50->x = 128;
    TEX_File_FreeDescription(&a1a);
    return 1;
}

void TEXPRO_Free(tTexProceduralStruct* a1)
{
    tTexProcPlasma* v1; // esi

    v1 = (tTexProcPlasma*)a1->proceduralTexPtr;  // a water, fire, plasma struktúrának ugyanaz az eleje, ezért ez az egy függvény jó rájuk
    MEM_Free(v1->ptr1);
    MEM_Free(v1->ptr2);
    MEM_Free(v1);
}

void TEXPRO_PhotoFree(tTexProceduralStruct* a1)
{
    MEM_Free(a1->proceduralTexPtr);
}

void TEXPRO_DummyDisrupt(tTexProceduralStruct* a1) {

}

void TEXPRO_DummyFree(tTexProceduralStruct* a1) {
}

void* TEXPRO_pv_EmptyLoad(char* offset, int vmilyensize, int width, int height) {
    return NULL;
}

int TEXPRO_l_DummyUpdate(tTexProceduralStruct* a1) {
    return 0;
}

void TEX_Procedural_Init()
{
    int v0;
    int v1;

    v0 = 0;
    TEX_gast_ProceduralInterface[0].id = 0;
    TEX_gast_ProceduralInterface[0].Update = TEXPRO_l_DummyUpdate;
    TEX_gast_ProceduralInterface[0].Load = TEXPRO_pv_EmptyLoad;
    TEX_gast_ProceduralInterface[0].Disrupt = TEXPRO_DummyDisrupt;
    TEX_gast_ProceduralInterface[0].Free = TEXPRO_DummyFree;
    TEX_gast_ProceduralInterface[1].id = 1;
    TEX_gast_ProceduralInterface[1].Update = TEXPRO_l_WaterUpdate;
    TEX_gast_ProceduralInterface[1].Load = TEXPRO_pv_WaterLoad;
    TEX_gast_ProceduralInterface[1].Disrupt = TEXPRO_WaterDisrupt;
    TEX_gast_ProceduralInterface[1].Free = TEXPRO_Free;
    TEX_gast_ProceduralInterface[2].id = 2;
    TEX_gast_ProceduralInterface[2].Update = TEXPRO_l_FireUpdate;
    TEX_gast_ProceduralInterface[2].Load = TEXPRO_pv_FireLoad;
    TEX_gast_ProceduralInterface[2].Disrupt = TEXPRO_DummyDisrupt;
    TEX_gast_ProceduralInterface[2].Free = TEXPRO_Free;
    TEX_gast_ProceduralInterface[3].id = 3;
    TEX_gast_ProceduralInterface[3].Update = TEXPRO_l_MpegUpdate;
    TEX_gast_ProceduralInterface[3].Load = TEXPRO_pv_MpegLoad;
    TEX_gast_ProceduralInterface[3].Disrupt = TEXPRO_DummyDisrupt;
    TEX_gast_ProceduralInterface[3].Free = TEXPRO_MpegFree;
    TEX_gast_ProceduralInterface[4].id = 4;
    TEX_gast_ProceduralInterface[4].Update = TEXPRO_l_PhotoUpdate;
    TEX_gast_ProceduralInterface[4].Load = TEXPRO_pv_PhotoLoad;
    TEX_gast_ProceduralInterface[4].Disrupt = TEXPRO_DummyDisrupt;
    TEX_gast_ProceduralInterface[4].Free = TEXPRO_PhotoFree;
    TEX_gast_ProceduralInterface[5].id = 5;
    TEX_gast_ProceduralInterface[5].Update = TEXPRO_l_PlasmaUpdate;
    TEX_gast_ProceduralInterface[5].Load = TEXPRO_pv_PlasmaLoad;
    TEX_gast_ProceduralInterface[5].Disrupt = TEXPRO_DummyDisrupt;
    TEX_gast_ProceduralInterface[5].Free = TEXPRO_Free;
    v1 = 0;
    do
    {
        TEXPRO_Cos[v0++] = (unsigned int)(cos((double)v1 * 0.049087387) * 30.0);
        v1 = v0;
    } while (v0 < 256);
}

void TEXPRO_Photo_SetMapGao(struct sTexProceduralStruct* a1, struct sGameObjectMain* a2)
{
    tGEOGRO* v2;
    int v3;
    int v4;
    double v5;
    tDDContainerStruct* v6;
    Mtx4x4* v7;
    int v8; 
    Vec3* v9;
    __int16* v10;
    Mtx4x4* v11;
    float v12;
    int v13;
    float v14;
    int v15;
    float v16;
    int a4; 
    tDDContainerStruct* a4a;
    tGEOGRO* v19;
    tTexProcPhoto* v20; 
    int v21[4];
    Vec3 dest;
    Vec3 v23; 
    Mtx4x4 v24;

    if (gpst_DX_stDD)
    {
        if (a1)
        {
            if (a2)
            {
                if (gpst_DX_stDD->winWidth >= 512 && gpst_DX_stDD->winHeight >= 256)
                {
                    v20 = (tTexProcPhoto*)a1->proceduralTexPtr;
                    v2 = (tGEOGRO*)OBJ_p_GetGro(a2);
                    v19 = v2;
                    if (v2)
                    {
                        if (v2->intf->id == 1 && v2->vertexNum == 4)
                        {
                            v12 = 0.0;
                            v21[0] = 0;
                            v3 = 1;
                            a4 = 1;
                            while (1)
                            {
                                MATH_SubstractVec(&dest, v2->vertices, &v2->vertices[a4]);
                                v4 = v3 + 1;
                                if (v3 + 1 > 3)
                                    v4 = v3 - 2;
                                MATH_SubstractVec(&v23, &v19->vertices[v4], &v19->vertices[6 - v4 - v3]);
                                v5 = (float)(v23.z * dest.z + v23.y * dest.y + v23.x * dest.x);
                                if (v5 < 0.0)
                                {
                                    v5 = -v5;
                                    v4 = 6 - v4 - v3;
                                }
                                if (v5 > v12)
                                {
                                    v12 = (float)v5;
                                    v21[1] = v3;
                                    v21[2] = 6 - v4 - v3;
                                    v21[3] = v4;
                                }
                                ++v3;
                                if (++a4 > 3)
                                    break;
                                v2 = v19;
                            }
                            v15 = (gpst_DX_stDD->winWidth - 512) / 2;
                            v6 = &gpst_DX_stDD->ct;
                            v13 = (gpst_DX_stDD->winHeight - 256) / 2;
                            v11 = &gpst_DX_stDD->ct.cam.cameraMtx;
                            a4a = &gpst_DX_stDD->ct;
                            v7 = OBJ_pst_GetAbsoluteMatrix(a2);
                            MATH_MulMatrixMatrix(&v24, v7, v11);
                            v16 = (float)v15;
                            v6->cam.viewMtxPtr_setInGRORender = &v24;
                            v8 = 0;
                            v9 = &dest;
                            v10 = &v20->sa[4]; // TODO megcsinálni igényesre
                            v14 = (float)v13;
                            do
                            {
                                SOFT_TransformAndProject(v9, &v19->vertices[v21[v8]], 1, &a4a->cam);
                                *(v10 - 4) = (unsigned int)((v9->x - v16) * 0.5);
                                *v10 = (unsigned int)((v9->y - v14) * 0.5);
                                ++v8;
                                ++v10;
                                ++v9;
                            } while (v8 < 4);
                            v20->x = 0x10000000;
                            v20->remainingTime = 1.0;
                            v20->f1 = 1.0;
                        }
                    }
                }
            }
        }
    }
}