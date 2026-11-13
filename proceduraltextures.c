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

struct sTexProcWater* TEXPRO_pv_WaterLoad(char* data, int size, int width, int height)
{
    unsigned int bytes;
    tTexProcWater* water;
    int16_t* current;
    int16_t* previous;
    int i;
    long double wave;
    unsigned int intensity;

    bytes = 2 * height * width;
    water = (tTexProcWater*)MEM_p_Alloc(sizeof(tTexProcWater));
    current = (int16_t*)MEM_p_Alloc(bytes);
    water->ptr1 = current;
    memset(current, 0, bytes);
    previous = (int16_t*)MEM_p_Alloc(bytes);
    water->ptr2 = previous;
    memset(previous, 0, bytes);
    i = 256;
    do
    {
        --i;
        wave = fabs(sin((double)i * 0.049087387));
        if (i <= 128)
            intensity = 0;
        else
            intensity = (unsigned int)(wave * wave * (wave * wave) * 128.0);
        water->values[i] = intensity | (intensity << 8) | (intensity << 16)
                         | ((unsigned int)(255.0 * wave) << 24);
    } while (i);
    return water;
}

int TEXPRO_l_WaterUpdate(tTexProceduralStruct* procedural)
{
    tTexProcWater* water;
    unsigned int rowBytes;
    int16_t* current;
    int16_t* previous;
    int16_t* above;
    int16_t* damped;
    int width;
    int height;
    int row;
    int col;
    int value;
    uint32_t* dst;
    uint16_t here;
    uint16_t right;

    water = (tTexProcWater*)procedural->proceduralTexPtr;
    rowBytes = 2 * procedural->width;
    current = &water->ptr1[rowBytes / 2 + 1];
    height = procedural->height;
    width = procedural->width;
    previous = &water->ptr2[rowBytes / 2 + 1];
    memset(water->ptr1, 0, rowBytes);
    memset(water->ptr2, 0, rowBytes);
    memset((char*)water->ptr1 + width * (2 * height - 2), 0, rowBytes);
    memset((char*)water->ptr2 + width * (2 * height - 2), 0, rowBytes);
    if (height != 2)
    {
        for (row = height - 2; row; --row)
        {
            if (width != 2)
            {
                // preserved: rowBytes is unsigned, so this divides by 4294967294 and samples the centre cell, not the row above
                above = &previous[rowBytes / -2];
                for (col = width - 2; col; --col)
                {
                    value = ((previous[1] + *above + *(previous - 1) + previous[rowBytes / 2]) >> 1)
                          - *current;
                    value -= value >> 8;
                    if (value < -8192)
                        value = -8192;
                    else if (value > 8192)
                        value = 8192;
                    *current++ = value;
                    ++previous;
                    ++above;
                }
            }
            current[0] = 0;
            current[1] = 0;
            previous[0] = 0;
            previous[1] = 0;
            current += 2;
            previous += 2;
        }
    }
    current = water->ptr1;
    previous = water->ptr2;
    for (col = width * height; col; --col)
    {
        *current -= (uint16_t)(*current >> 6);
        ++current;
        *previous -= *previous >> 6;
        ++previous;
    }
    damped = water->ptr2;
    water->ptr2 = water->ptr1;
    water->ptr1 = damped;
    dst = procedural->proceduralPicData;
    for (row = height; row; --row)
    {
        for (col = width; col; --col)
        {
            here = *damped;
            // preserved: the last pixel of the buffer reads damped[1], one entry past the end
            right = damped[1];
            ++damped;
            *dst++ = water->values[(char)((here - right) >> 6) + 128];
        }
    }
    return 1;
}

void TEXPRO_WaterDisrupt(tTexProceduralStruct* procedural)
{
    tTexProcWater* water;
    unsigned int width;
    unsigned int height;
    int16_t* current;
    int16_t* previous;
    int splash;
    int noise;
    long double angle;
    float kx;
    float ky;
    unsigned int y;
    unsigned int amount;
    int base;
    int below;
    float radius;
    signed int x;
    int spin;

    water = (tTexProcWater*)procedural->proceduralTexPtr;
    width = procedural->width;
    height = procedural->height;
    ++TexPro_WaterDistruptCounter;
    current = &water->ptr1[width];
    previous = &water->ptr2[width];
    splash = 4;
    spin = RND_JADErand() % 100;
    while (1)
    {
        --splash;
        noise = RND_JADErand();
        if (splash >= 2)
        {
            x = noise % (int)(width - 8) + 4;
            y = RND_JADErand() % (int)(height - 8) + 4;
        }
        else
        {
            angle = (double)(50 - noise % 100) * 0.0049999999 + ALPHA_441[splash];
            ALPHA_441[splash] = (float)angle;
            radius = (float)(unsigned int)spin;
            kx = (float)(sin(angle) * radius * 0.02 + KX_442[splash]);
            KX_442[splash] = kx;
            ky = (float)(cos(angle) * radius * 0.02 + KY_443[splash]);
            KY_443[splash] = ky;
            x = (unsigned int)kx % width;
            y = (unsigned int)ky % height;
            if (x > (int)(width - 8))
                x = width - 8;
            if ((int)y > (int)(height - 8))
                y = height - 8;
        }
        amount = (unsigned int)(sin((double)TexPro_WaterDistruptCounter * 0.2) * 200.0);
        base = x + width * y;
        // preserved: the lower pair of the 2x2 splash is indexed at twice its element offset
        below = 2 * (x + width * (y + 1));
        current[base] += amount;
        current[below] += amount;
        current[base + 1] += amount;
        current[below + 1] += amount;
        previous[base] += amount;
        previous[below] += amount;
        previous[base + 1] += amount;
        previous[below + 1] += amount;
        if (!(splash * 4))
            break;
    }

}

int TEX_l_File_LoadProcedural(char* offset, tEngTexIntermediateStruct* tex, int size)
{
    int used;
    int16_t flags;
    int interfaceIdx;

    used = 0;
    if (size)
    {
        used = LOA_ReadLong(&offset, 0, 1);
        flags = LOA_ReadShort(&offset, 0, 1);
        tex->realWidth = LOA_ReadShort(&offset, 0, 1);
        tex->realHeight = LOA_ReadShort(&offset, 0, 1);
        interfaceIdx = (uint16_t)LOA_ReadShort(&offset, 0, 1);
    }
    else
    {
        tex->realWidth = 128;
        tex->realHeight = 128;
        interfaceIdx = 0;
        flags = 0;
    }
    tex->flags |= 0x20u;
    tex->colorDepth1 = 32;
    tex->colorDepth2 = 32;
    tex->alphaMask = 0xFF000000;
    tex->redMask = 0xFF0000;
    tex->greenMask = 0xFF00;
    tex->blueMask = 0xFF;
    if ((flags & 1) != 0)
        tex->tp.flagsUnkAndProceduralUnkb1 |= 0x200; // HIBYTE(v5->tp.flagsUnkAndProceduralUnkb1) |= 2
    tex->tp.proceduralParams.interfaceIdx = interfaceIdx;
    tex->tp.proceduralParams.interfacePtr = TEX_gast_ProceduralInterface[interfaceIdx].Load(offset, size - used, tex->realWidth, tex->realHeight); // FIXME, értékadás
    tex->tp.proceduralParams.flags = flags;
    return 1;
}

void TEX_Procedural_Add(int16_t elemIdx, int interfaceIdx, int16_t flags, void* texPtr)
{
    int count;
    tTexProceduralStruct* entry;
    tEngTexListElemStruct* elem;

    count = TEX_gi_NbProcedural;
    if (!TEX_gi_NbProcedural)
    {
        TEX_gdst_Procedural = (tTexProceduralStruct*)MEM_p_Alloc(16 * sizeof(tTexProceduralStruct));
        count = TEX_gi_NbProcedural;
    }
    if (count < 16)
    {
        entry = &TEX_gdst_Procedural[count];
        TEX_gi_NbProcedural = count + 1;
        entry->elemIdx = elemIdx;
        elem = &TEX_gst_GlobalList.texElems[elemIdx];
        entry->s2 = flags;
        entry->height = elem->normalData.width;
        entry->width = elem->normalData.height;
        entry->proceduralPicData = 0;
        entry->proceduralIntf = &TEX_gast_ProceduralInterface[interfaceIdx];
        entry->proceduralTexPtr = texPtr;
    }
}

static void PTR4* RADLINK BINK_pv_MemAlloc(U32 bytes)
{
    return (void PTR4*)MEM_p_Alloc(bytes);
}

static void RADLINK BINK_MemFree(void PTR4* ptr)
{
    MEM_Free((void*)ptr);
}

void TEXPRO_Bink_RemoveActSlotIDFromUsedSlots(tBinkUnkStruct* block)
{
    int mask;

    mask = 1 << block->actSlotID;
    block->stateMachine2MeansBusy = 1;
    block->actSlotID = -1;
    gBinkReadUsedSlots &= ~mask;
}

int TEXPRO_Bink_GetNextFreeSlotID()
{
    int slot;

    for (slot = 0; (gBinkReadUsedSlots & (1 << slot)) != 0; ++slot)
        ;
    return slot;
}

int TEXPRO_Bink_ReadActBlock(tBinkIFPtr* stream)
{
    tBinkUnkStruct* block;
    int chunk;

    block = stream->actualElemInLinkedList;
    block->actSlotID = TEXPRO_Bink_GetNextFreeSlotID();
    if ((unsigned int)block->actSlotID >= 10u)
        return 0;
    if (!stream->size)
        return 0;
    chunk = stream->size;
    if (chunk >= 4096)
        chunk = 4096;
    BINK_SeekAndReadFromBF(stream->filePosInBF, block->blockDataStartPtr, chunk);
    gBinkReadUsedSlots |= 1 << block->actSlotID;
    stream->filePosInBF += chunk;
    stream->size -= chunk;
    block->stateMachine2MeansBusy = 2;
    TEXPRO_Bink_RemoveActSlotIDFromUsedSlots(stream->actualElemInLinkedList);
    return 1;
}

void TEXPRO_BINK_ReadActBlockAndAdvance(tBinkIFPtr* stream)
{
    while (stream->dataAndLinkedList
           && !stream->actualElemInLinkedList->stateMachine2MeansBusy)
    {
        if (!TEXPRO_Bink_ReadActBlock(stream))
            break;
        stream->actualElemInLinkedList = stream->actualElemInLinkedList->nextDataBlock;
    }
}

int TEXPRO_Bink_GetAlreadyProcessedFileSize(tBinkIFPtr* stream)
{
    int total;
    int offset;
    tBinkUnkStruct* block;

    if (!stream->dataAndLinkedList || !stream->size)
        return -1;
    offset = stream->dataBlockStartCurrOffset;
    block = stream->linkedListStart;
    total = 0;
    while (block->stateMachine2MeansBusy == 1)
    {
        block = block->nextDataBlock;
        total += 4096 - offset;
        offset = 0;
        if (block == stream->linkedListStart)
            break;
    }
    return total;
}

void BINK_releaseLinkedList(tBinkIFPtr* stream)
{
    tBinkUnkStruct* last;
    tBinkUnkStruct* block;

    if (stream->dataAndLinkedList)
    {
        last = stream->actualElemInLinkedList;
        block = last;
        do
        {
            block = block->nextDataBlock;
            while (block->stateMachine2MeansBusy == 2)
                ;
            block->stateMachine2MeansBusy = 0;
        } while (block != last);
        stream->linkedListStart = last;
        stream->actualElemInLinkedList = last;
        stream->dataBlockStartCurrOffset = 0;
    }
}

void PC_VideoStreamRestart_Bink(tBinkIFPtr* stream)
{
    BINK_IF_PTR_GLOBAL = stream;
    if (stream->filePosInBF)
    {
        BinkClose(stream->binkHandle);
        BINK_releaseLinkedList(stream);
        PC_VideoStreamCreate_Bink(stream, stream->binkVideoKey);
    }
}

void PC_VideoStreamSync_Bink(tBinkIFPtr* stream)
{
    int frames;

    BINK_IF_PTR_GLOBAL = stream;
    if (!stream->filePosInBF)
        return;
    if (stream->videofilesize < TEXPRO_Bink_GetAlreadyProcessedFileSize(stream)
        && !BinkWait(stream->binkHandle))
    {
        frames = 0;
        while (1)
        {
            BinkDoFrame(stream->binkHandle);
            if (stream->binkHandle->FrameNum >= stream->binkHandle->Frames)
            {
                PC_VideoStreamRestart_Bink(stream);
                return;
            }
            BinkNextFrame(stream->binkHandle);
            ++frames;
            if (BinkWait(stream->binkHandle) || frames >= 3)
            {
                BinkCopyToBuffer(stream->binkHandle, stream->proceduralStructPtr->proceduralPicData, 4 * stream->proceduralStructPtr->width, stream->proceduralStructPtr->height, 0, 0, BINKSURFACE32 | BINKCOPYALL);
                break;
            }
        }
    }
    TEXPRO_BINK_ReadActBlockAndAdvance(stream);
}


U32 RADLINK BINK_GetBufferSize(BINKIO* binkIO, U32 size)
{
    return 2048;
}

int TEXPRO_Bink_ReadAndCopyNextBlockToReadBuffer(tBinkIFPtr* stream, char* dest, signed int remaining)
{
    tBinkUnkStruct* block;
    signed int chunk;
    int offset;

    if (!stream->dataAndLinkedList)
        return -1;
    while (remaining)
    {
        block = stream->linkedListStart;
        if (block->stateMachine2MeansBusy != 1)
        {
            stream->dataBlockStartCurrOffset = 0;
            if (!block->stateMachine2MeansBusy)
                TEXPRO_BINK_ReadActBlockAndAdvance(stream);
            block = stream->linkedListStart;
            while (block->stateMachine2MeansBusy == 2)
                ;
            if (block->stateMachine2MeansBusy != 1)
                return -1;
        }
        chunk = 4096 - stream->dataBlockStartCurrOffset;
        if (remaining <= chunk)
            chunk = remaining;
        memcpy(dest, &block->blockDataStartPtr[stream->dataBlockStartCurrOffset], chunk);
        offset = chunk + stream->dataBlockStartCurrOffset;
        dest += chunk;
        stream->dataBlockStartCurrOffset = offset;
        if (offset == 4096)
        {
            stream->linkedListStart->stateMachine2MeansBusy = 0;
            stream->linkedListStart = stream->linkedListStart->nextDataBlock;
            stream->dataBlockStartCurrOffset = 0;
        }
        remaining -= chunk;
    }
    return 0;
}

int TEXPRO_BINK_ReadNumBytes(BINKIO* binkIO, void* DstBuf, unsigned int MaxCharCount)
{
    if (TEXPRO_Bink_ReadAndCopyNextBlockToReadBuffer(BINK_IF_PTR_GLOBAL, (char*)DstBuf, MaxCharCount))
    {
        BINK_SeekAndReadFromBF(BINK_IF_PTR_GLOBAL->filePosInBF, DstBuf, MaxCharCount);
        BINK_IF_PTR_GLOBAL->filePosInBF += MaxCharCount;
    }
    binkIO->BytesRead = MaxCharCount;
    return 0;
}

U32 RADLINK BINK_ReadHeader(BINKIO* binkIO, S32 offset, void PTR4* DstBuf, U32 MaxCharCount)
{
    return TEXPRO_BINK_ReadNumBytes(binkIO, DstBuf, MaxCharCount);
}

U32 RADLINK BINK_ReadFrame(BINKIO* binkIO, U32 frameNum, S32 offset, void* DstBuf, U32 MaxCharCount)
{
    return TEXPRO_BINK_ReadNumBytes(binkIO, DstBuf, MaxCharCount);
}

void RADLINK BINK_SetInfo(struct BINKIO PTR4* Bnkio, void PTR4* Buf, U32 Size, U32 FileSize, U32 simulate) {
}

U32 RADLINK BINK_Idle(struct BINKIO PTR4* Bnkio) {
    return 0;
}

void TEXPRO_BINK_Close(tBinkIFPtr* stream)
{
    if (stream->dataAndLinkedList)
    {
        BINK_releaseLinkedList(stream);
        MEM_Free(stream->dataAndLinkedList);
        stream->dataBlockStartCurrOffset = 0;
        stream->actualElemInLinkedList = 0;
        stream->linkedListStart = 0;
        stream->dataAndLinkedList = 0;
    }
}

void RADLINK BINK_Close(struct BINKIO PTR4* Bnkio)
{
    BINK_IF_PTR_GLOBAL->filePosInBF = 0;
    TEXPRO_BINK_Close(BINK_IF_PTR_GLOBAL);
}

void BINK_AllocLinkedList(tBinkIFPtr* binkIFPtr, int videosize)
{
    unsigned int bufferBytes;
    unsigned int blockCount;
    unsigned int total;
    char* blockData;
    tBinkUnkStruct* block;

    binkIFPtr->videofilesize = videosize;
    bufferBytes = 2 * (videosize & 0xFFFFF000) + 0x2000;
    blockCount = bufferBytes >> 12;// >> 12 => / 4096
    total = 16 * blockCount + bufferBytes;
    blockData = (char*)MEM_p_Alloc(total);
    binkIFPtr->dataAndLinkedList = blockData;
    memset(blockData, 0, total);
    block = (tBinkUnkStruct*)&blockData[bufferBytes];
    binkIFPtr->linkedListStart = block;
    binkIFPtr->actualElemInLinkedList = block;
    while (blockCount)
    {
        block->blockDataStartPtr = blockData;
        blockData += 4096;
        --blockCount;
        block->stateMachine2MeansBusy = 0;
        block->nextDataBlock = block + 1;
        ++block;
    }
    block[-1].nextDataBlock = (tBinkUnkStruct*)&binkIFPtr->dataAndLinkedList[bufferBytes];// ez csinálja meg a loopot, gondolom ha a végére ér az elejére ugorjon
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
    BinkSetMemory(BINK_pv_MemAlloc, BINK_MemFree);
    BinkSetIOSize(2048);
    BinkSetIO(BINK_IO_FUNC);
    binkIFPtr->binkHandle = BinkOpen((char*)binkIFPtr, BINKNOTHREADEDIO | BINKIOPROCESSOR | BINKIOSIZE | BINKSNDTRACK);
}

void TEXPRO_MPEG_Start()
{
    int pos;

    pos = TEXPRO_pst_CurrentMpeg->pos;
    if (pos != -1)
    {
        TEXPRO_pst_CurrentMpeg->startOffset = pos + 4;
        TEXPRO_pst_CurrentMpeg->flags &= ~0x100; //BYTE1(TEXPRO_pst_CurrentMpeg->flags) &= ~1u;
    }
}

int TEXPRO_MPEG_ReadNext(void* DstBuf)
{
    tTexProcMpeg* mpeg;
    signed int chunk;

    mpeg = TEXPRO_pst_CurrentMpeg;
    if (mpeg->pos == -1)
        return 0;
    chunk = mpeg->endOffset - mpeg->startOffset;
    if (chunk <= 2048)
        // preserved: decimal 100, where the note beside it is the byte-1 bit, 0x100
        mpeg->flags |= 100u; // BYTE1(TEXPRO_pst_CurrentMpeg->flags) |= 1u;
    else
        chunk = 2048;
    BINK_SeekAndReadFromBF(mpeg->startOffset, DstBuf, chunk);
    mpeg->startOffset += chunk;
    return chunk;
}
tTexProcMpeg* TEXPRO_pv_MpegLoad(char* readptr, int size, int width, int height)
{
    tTexProcMpeg* mpeg;
    int pos;

    mpeg = (tTexProcMpeg*)MEM_p_Alloc(sizeof(tTexProcMpeg));
    memset(mpeg, 0, sizeof(tTexProcMpeg));
    mpeg->sub.bufferSize = 2048;
    mpeg->sub.Start = TEXPRO_MPEG_Start;
    mpeg->sub.ReadNext = TEXPRO_MPEG_ReadNext;
    if (size == 0 || size != LOA_ReadLong(&readptr, 0, 1))
    {
        mpeg->key = -1;
        mpeg->i6 = -1;
        mpeg->sub.binkKey = -1;
        mpeg->pos = -1;
    }
    else
    {
        mpeg->key = LOA_ReadLong(&readptr, 0, 1);
        if (size == 8)
        {
            mpeg->i6 = -1;
            mpeg->sub.binkKey = -1;
            mpeg->sub.ii5 = -1;
            mpeg->i8 = 0;
            mpeg->i9 = 0;
            mpeg->i11 = 0;
            mpeg->i10 = 0;
            mpeg->i7 = 0;
        }
        else if (size == 32)
        {
            mpeg->i6 = LOA_ReadLong(&readptr, 0, 1);
            mpeg->i7 = LOA_ReadLong(&readptr, 0, 1);
            mpeg->i10 = LOA_ReadLong(&readptr, 0, 1);
            mpeg->i11 = LOA_ReadLong(&readptr, 0, 1);
            mpeg->i9 = LOA_ReadLong(&readptr, 0, 1);
            mpeg->i8 = LOA_ReadLong(&readptr, 0, 1);
            mpeg->sub.binkKey = -1;
            mpeg->sub.ii3 = 0;
            mpeg->sub.ii1 = 0;
            mpeg->sub.ii2 = 0;
            mpeg->sub.ii5 = -1;
        }
        else
        {
            mpeg->i6 = LOA_ReadLong(&readptr, 0, 1);
            mpeg->i7 = LOA_ReadLong(&readptr, 0, 1);
            mpeg->i10 = LOA_ReadLong(&readptr, 0, 1);
            mpeg->i11 = LOA_ReadLong(&readptr, 0, 1);
            mpeg->i9 = LOA_ReadLong(&readptr, 0, 1);
            mpeg->i8 = LOA_ReadLong(&readptr, 0, 1);
            mpeg->sub.binkKey = LOA_ReadLong(&readptr, 0, 1);
            mpeg->sub.ii1 = LOA_ReadLong(&readptr, 0, 1);
            mpeg->sub.ii2 = LOA_ReadLong(&readptr, 0, 1);
            mpeg->sub.ii3 = LOA_ReadLong(&readptr, 0, 1);
            if (size == 48)
                mpeg->sub.ii5 = -1;
            else
                mpeg->sub.ii5 = LOA_ReadLong(&readptr, 0, 1);
        }
        pos = BIG_ul_SearchKeyToPos(mpeg->key);
        mpeg->pos = pos;
        if (pos != -1)
        {
            mpeg->startOffset = pos + 4;
            mpeg->endOffset = mpeg->startOffset + BIG_ul_GetLengthFile(pos);
            TEXPRO_pst_CurrentMpeg = mpeg;
            if (!LOA_gb_SpeedMode || BIG_gi_ReadMode != 2)
            {
                //TODO FIXME: mpeg2dec should be linked, pointless isnce it's kind of a leftover and doesn't even work.
                //mpeg_init_and_open_sub_486910(&v4->sub);
                return mpeg;
            }
        }
    }
    return mpeg;
}

int TEXPRO_l_MpegUpdate(tTexProceduralStruct* procedural)
{
    tTexProcMpeg* mpeg;
    tBinkIFPtr* stream;
    char state;

    mpeg = (tTexProcMpeg*)procedural->proceduralTexPtr;
    TEXPRO_pst_CurrentMpeg = mpeg;
    if (mpeg->sub.binkKey != -1)
    {
        if (!mpeg->sub.binkIFPtr)
        {
            stream = (tBinkIFPtr*)MEM_p_Alloc(sizeof(tBinkIFPtr));
            mpeg->sub.binkIFPtr = stream;
            memset(stream, 0, sizeof(tBinkIFPtr));
            mpeg->sub.binkIFPtr->proceduralStructPtr = procedural;
            PC_VideoStreamCreate_Bink(mpeg->sub.binkIFPtr, mpeg->sub.binkKey);
        }
        if ((mpeg->flags & 0x100) != 0 && (mpeg->flags & 2) != 0 && ((mpeg->flags >> 16) & 0xff) != 2) // ...BYTE2(v2->flags) != 2
        {
            mpeg->flags &= 0xff00ffff;
            mpeg->flags |= 0x00010000;
        }
        state = (mpeg->flags >> 16) & 0xff; //v4 = BYTE2(v2->flags);
        if (state)
        {
            if (state == 1)
            {
                mpeg->flags |= 1u;
                memset(procedural->proceduralPicData, 0, 4 * procedural->width * procedural->height);
                mpeg->flags &= 0xff00ffff; //BYTE2(TEXPRO_pst_CurrentMpeg->flags) = 0;
                return 1;
            }
            if (state == 2)
            {
                mpeg->flags &= ~1u;
                PC_VideoStreamRestart_Bink(mpeg->sub.binkIFPtr);
            }
            mpeg->flags &= 0xff00ffff; //BYTE2(v2->flags) = 0;
        }
        if ((mpeg->flags & 1) != 0)
            return 0;
        PC_VideoStreamSync_Bink(mpeg->sub.binkIFPtr);
    }
    return 1;
}

void BINK_CloseAll(tBinkIFPtr* stream)
{
    BINK_IF_PTR_GLOBAL = stream;
    if (stream->filePosInBF)
    {
        BinkClose(stream->binkHandle);
        TEXPRO_BINK_Close(stream);
    }
}

void TEXPRO_MpegFree(tTexProceduralStruct* procedural)
{
    tTexProcMpeg* mpeg;

    mpeg = (tTexProcMpeg*)procedural->proceduralTexPtr;
    if (mpeg->sub.binkKey != -1 && mpeg->sub.binkIFPtr)
    {
        BINK_CloseAll(mpeg->sub.binkIFPtr);
        MEM_Free(mpeg->sub.binkIFPtr);
        mpeg->sub.binkIFPtr = 0;
    }
    MEM_Free(mpeg);
}

tTexProFire* TEXPRO_pv_FireLoad(char* readptr, int size, int width, int height)
{
    tTexProFire* fire;
    uint8_t* current;
    uint8_t* previous;
    int i;

    fire = (tTexProFire*)MEM_p_Alloc(sizeof(tTexProFire));
    current = (uint8_t*)MEM_p_Alloc(height * width);
    fire->ptr = current;
    memset(current, 0, height * width);
    previous = (uint8_t*)MEM_p_Alloc(height * width);
    fire->ptr2 = previous;
    memset(previous, 0, height * width);
    // preserved: the fourth quarter is written at 172, not 192, so arr[236..255] is never set and the struct is not zeroed
    for (i = 63; i >= 0; --i)
    {
        fire->arr[i] = (i << 10) | (i << 18) | (i << 26);
        fire->arr[i + 64] = 4 * (i | 0xFFFFFFC0);
        fire->arr[i + 128] = -1;
        fire->arr[i + 172] = (4 * (63 - i)) | ((63 - i) << 18) | ((63 - i) << 10) | ((-1 - i) << 26);
    }
    return fire;
}

int TEXPRO_l_FireUpdate(tTexProceduralStruct* procedural)
{
    int height;
    tTexProFire* fire;
    int width;
    uint8_t* current;
    uint8_t* previous;
    uint8_t* tail;
    uint8_t* above;
    uint8_t* left;
    uint8_t* swap;
    uint8_t* src;
    uint32_t* dst;
    int row;
    int col;
    int cell;
    int average;

    height = procedural->height;
    fire = (tTexProFire*)procedural->proceduralTexPtr;
    width = procedural->width;
    // preserved: unknum is never written anywhere in the tree and the struct is not zeroed, so this branches on uninitialised memory
    if (fire->unknum)
    {
        if (fire->unknum == 1)
        {
            // preserved: the walk starts one entry past the end of the buffer
            tail = &fire->ptr[width * height];
            if (width * (height - 2))
            {
                // TODO: ezeket jó lenne nem - offsetesre megcsinálni
                above = &tail[-2 * width];
                left = &tail[-width + 1];
                for (cell = width * (height - 2); cell; --cell)
                {
                    *tail-- = (*left + *above-- + *(left - 1) + *(left - 2)) >> 2;
                    --left;
                }
            }
        }
    }
    else
    {
        current = &fire->ptr[3 * width + 1];
        previous = &fire->ptr2[width + 1];
        if (height != 4)
        {
            for (row = height - 4; row; --row)
            {
                if (width != 2)
                {
                    for (col = width - 2; col; --col)
                    {
                        average = (*(previous - 1) + previous[1] + previous[-width] + previous[width]) >> 2;
                        *current++ = average - ((uint8_t)average >> 4);
                        ++previous;
                    }
                }
                current += 2;
                previous += 2;
            }
        }
        swap = fire->ptr;
        fire->ptr = fire->ptr2;
        fire->ptr2 = swap;
    }
    src = fire->ptr;
    dst = procedural->proceduralPicData;
    for (col = width; col; --col)
    {
        for (row = height; row; --row)
            *dst++ = fire->arr[*src++];
    }
    return 1;
}

tTexProcPlasma* TEXPRO_pv_PlasmaLoad(char* readptr, int size, int width, int height)
{
    tTexProcPlasma* plasma;
    uint8_t* field;
    uint8_t* copy;
    uint8_t* column;
    int pass;
    int step;
    int half;
    int col;
    int halfCol;
    int nextCol;
    int row;
    int rowBelow;
    int topLeft;
    int topRight;
    int bottomLeft;
    int bottomRight;
    int sum;
    int i;

    plasma = (tTexProcPlasma*)MEM_p_Alloc(sizeof(tTexProcPlasma));
    field = (uint8_t*)MEM_p_Alloc(height * width);
    plasma->ptr1 = field;
    memset(field, 0, height * width);
    copy = (uint8_t*)MEM_p_Alloc(height * width);
    plasma->ptr2 = copy;
    memset(copy, 0, height * width);
    plasma->f1 = (float)((double)rand() * 0.000030518509 * 360.0);
    plasma->f2 = (float)((double)rand() * 0.000030518509 * 360.0);
    field = plasma->ptr1;
    copy = plasma->ptr2;
    // preserved: both passes generate into ptr1, so the first is discarded and the two fields end up identical
    for (pass = 0; pass < 2; ++pass)
    {
        for (col = 0; col < width; ++col)
        {
            column = &field[col];
            for (row = 0; row < height; ++row)
            {
                *column = RND_JADErand();
                column += width;
            }
        }
        for (step = 32; step; step >>= 1)
        {
            half = step / 2;
            for (col = 0; col < width; col += step)
            {
                halfCol = col + half;
                nextCol = col + step;
                if (nextCol == width)
                    nextCol = 0;
                for (row = 0; row < height; row += step)
                {
                    rowBelow = row + step;
                    if (rowBelow == height)
                        rowBelow = 0;
                    topLeft = field[row * width + col];
                    topRight = field[row * width + nextCol];
                    bottomLeft = field[rowBelow * width + col];
                    bottomRight = field[rowBelow * width + nextCol];
                    field[(row + half) * width + nextCol] = (bottomRight + topRight) / 2;
                    sum = bottomLeft + bottomRight;
                    field[(row + half) * width + halfCol] = (topLeft + sum + topRight) / 4;
                    field[rowBelow * width + halfCol] = sum / 2;
                }
            }
        }
        if (pass)
            memcpy(copy, field, height * width);
    }
    // preserved: the fourth quarter is written at 172, not 192, so values[236..255] is never set
    for (i = 0; i < 64; ++i)
    {
        plasma->values[i + 64] = 4 * (i | 0xFFC00000);
        plasma->values[i] = 0xFF000000;
        plasma->values[i + 128] = (4 * (64 - i)) | 0xFF000000;
        plasma->values[i + 172] = 0xFF000000;
    }
    return plasma;
}

int TEXPRO_l_PlasmaUpdate(tTexProceduralStruct* procedural)
{
    tTexProcPlasma* plasma;
    int rotated;
    long double phaseX;
    int width;
    int height;
    unsigned int offsetX;
    double nextX;
    double nextY;
    unsigned int offsetY;
    int col;
    int row;
    int srcCol;
    int srcRow;
    uint32_t* dst;

    plasma = (tTexProcPlasma*)procedural->proceduralTexPtr;
    rotated = plasma->values[0];
    memcpy(plasma->values, &plasma->values[1], sizeof(plasma->values) - 4); // egyel odébbtoljuk az egészet
    phaseX = sin(plasma->f1 * 0.017453292);
    // preserved: a logical and where the two terms beside it are channel masks
    plasma->values[255] = (rotated & 0xFF000000) + ((uint16_t)rotated << 8) + ((rotated >> 16) && 0xff);
    width = procedural->width;
    height = procedural->height;
    dst = procedural->proceduralPicData;
    offsetX = (unsigned int)((phaseX + 1.0) * (double)(width >> 1));
    nextX = plasma->f1 + 1.666;
    plasma->f1 = (float)nextX;
    if (nextX >= 360.0)
        plasma->f1 = (float)(nextX - 360.0);
    nextY = plasma->f2 + 1.88;
    offsetY = (unsigned int)((sin(plasma->f2 * 0.017453292) + 1.0) * (double)(height >> 1));
    plasma->f2 = (float)nextY;
    if (nextY >= 360.0)
        plasma->f2 = (float)(nextY - 360.0);
    for (col = 0; col < width; ++col)
    {
        srcCol = col + offsetX;
        if (srcCol >= width)
            srcCol -= width;
        for (row = 0; row < height; ++row)
        {
            srcRow = row + offsetY;
            if (srcRow >= height)
                srcRow -= height;
            *dst++ = plasma->values[(plasma->ptr1[row * width + col]
                                     + plasma->ptr2[srcRow * width + srcCol]) >> 1];
        }
    }
    return 1;
}
tTexProcPhoto* TEXPRO_pv_PhotoLoad(char* offset, int size, int width, int height)
{
    tTexProcPhoto* photo;

    photo = (tTexProcPhoto*)MEM_p_Alloc(sizeof(tTexProcPhoto));
    photo->b2 = -1;
    photo->b1 = -1;
    photo->b3 = 0;
    photo->b4 = 0;
    photo->x = 0;
    if (size)
    {
        LOA_ReadLong(&offset, 0, 2);
        photo->b1 = LOA_ReadChar(&offset, 0, 1);
    }
    return photo;
}

int TEXPRO_l_PhotoUpdate(tTexProceduralStruct* procedural)
{
    tTexProcPhoto* photo;
    int wasAt;
    double remaining;
    int now;
    int scan;
    int scanOffset;
    uint32_t* px;
    int crossings;
    int edge;
    int next;
    int y0;
    int y1;
    int left;
    int right;
    int x;
    int n;
    unsigned int colour;
    char requested;
    char* photoData;
    uint16_t width;
    uint16_t height;
    uint32_t* line;
    uint32_t* bottom;
    uint32_t* last;
    uint32_t* cursor;
    int* pixel;
    int* pixelEnd;
    int span[2];
    size_t photoSize;
    tEngTexIntermediateStruct dst;
    tEngTexIntermediateStruct src;

    photo = (tTexProcPhoto*)procedural->proceduralTexPtr;
    wasAt = photo->x;
    if (wasAt && wasAt <= 128)
    {
        remaining = photo->remainingTime - TIM_gf_dt;
        photo->remainingTime = (float)remaining;
        if (remaining < 0.0)
            photo->x = 0;
        else
            photo->x = (unsigned int)(remaining / photo->f1 * 128.0);
        now = photo->x;
        if (now >= wasAt)
            return 0;
        scan = wasAt - 1;
        // preserved: a byte row stride and a byte indent applied to a uint32_t pointer, so every write lands four times too far in
        scanOffset = scan << 10;
        while (scan >= photo->x)
        {
            if (scan >= 2 && scan <= 125)
            {
                px = &procedural->proceduralPicData[scanOffset + 8];
                crossings = 0;
                for (edge = 0; edge < 4; ++edge)
                {
                    next = (edge + 1) % 4;
                    y0 = photo->sa[4 + edge];
                    y1 = photo->sa[4 + next];
                    // preserved: span[2] is written when a vertex on the scanline is followed by a third crossing
                    if (y0 == scan)
                    {
                        if (y1 == scan)
                            break;
                        span[crossings++] = photo->sa[edge];
                    }
                    else if ((y0 > scan && y1 < scan) || (y0 < scan && y1 > scan))
                    {
                        span[crossings++] = photo->sa[edge]
                                          + (scan - y0) * (photo->sa[next] - photo->sa[edge]) / (y1 - y0);
                        if (crossings == 2)
                            break;
                    }
                }
                if (crossings != 2)
                {
                    for (n = 252; n; --n)
                    {
                        *px = LIGHT_ul_Interpol2Colors(*px, 0, 0.5);
                        ++px;
                    }
                }
                else
                {
                    left = span[0];
                    right = span[1];
                    if (left > right)
                    {
                        left = span[1];
                        right = span[0];
                    }
                    if (left > 254)
                    {
                        left = 254;
                        right = 254;
                    }
                    else
                    {
                        if (left < 2)
                            left = 2;
                        if (right > 254)
                            right = 254;
                    }
                    x = 2;
                    if (left > 2)
                    {
                        for (n = left - 2; n; --n)
                        {
                            *px = LIGHT_ul_Interpol2Colors(*px, 0, 0.5);
                            ++px;
                        }
                        x = left;
                    }
                    if ((scan & 7) != 0)
                    {
                        for (; x < right; ++x)
                        {
                            colour = ~*px | 0xFF000000;
                            *px = colour;
                            if ((x & 7) == 0)
                                *px = colour | 0xFF00FF00;
                            ++px;
                        }
                    }
                    else if (x < right)
                    {
                        for (n = right - x; n; --n)
                        {
                            *px = ~*px | 0xFF00FF00;
                            ++px;
                        }
                        x = right;
                    }
                    if (x < 254)
                    {
                        for (n = 254 - x; n; --n)
                        {
                            *px = LIGHT_ul_Interpol2Colors(*px, 0, 0.5);
                            ++px;
                        }
                    }
                }
            }
            --scan;
            scanOffset -= 1024;
        }
        return 1;
    }
    if (!photo->b4)
    {
        if (photo->b3 || photo->b2 != photo->b1)
            photo->b4 = 1;
        return 0;
    }
    requested = photo->b2;
    photo->b4 = 0;
    photo->b3 = 0;
    photo->b1 = requested;
    if (requested == -1)
        return 0;
    memset(&dst, 0, sizeof(dst));
    dst.flags = 3;
    if (photo->b1 <= 100)
    {
        photoData = SAV_PhotoGet(photo->b1, (int*)&photoSize);
        if (!photoData || !photoSize)
            return 0;
        TEX_l_File_LoadJpeg(photoData, &dst, photoSize);
    }
    width = procedural->width;
    memcpy(&src, &dst, sizeof(src));
    dst.imgDataPtr = (char*)procedural->proceduralPicData;
    dst.colorDepth1 = 32;
    if (dst.realWidth == width && dst.realHeight == procedural->height)
    {
        TEX_l_ConvertPF(&src, &dst);
    }
    else
    {
        height = procedural->height;
        dst.realWidth = width;
        dst.realHeight = height;
        TEX_l_ConvertSize_24To32(&src, &dst);
    }
    pixel = (int*)dst.imgDataPtr;
    pixelEnd = (int*)&dst.imgDataPtr[4 * procedural->width * procedural->height];
    while (pixel < pixelEnd)
    {
        *pixel = *pixel & 0xFF00FF00 | *((uint8_t*)pixel + 2) | ((uint8_t)*pixel << 16);
        ++pixel;
    }
    cursor = procedural->proceduralPicData;
    line = &cursor[procedural->width];
    bottom = &cursor[procedural->width * (procedural->height - 2)];
    last = &cursor[procedural->width * (procedural->height - 1)];
    n = 0;
    if (procedural->width)
    {
        do
        {
            *cursor++ = -1;
            *line++ = -1;
            *bottom++ = -1;
            *last++ = -1;
            ++n;
        } while (n < procedural->width);
    }
    cursor = procedural->proceduralPicData;
    n = 0;
    if (procedural->height)
    {
        do
        {
            cursor[0] = -1;
            cursor[1] = -1;
            cursor[procedural->width - 2] = -1;
            cursor[procedural->width - 1] = -1;
            cursor += procedural->width;
            ++n;
        } while (n < procedural->height);
    }
    if (photo->x == 129)
        photo->x = 128;
    TEX_File_FreeDescription(&src);
    return 1;
}

void TEXPRO_Free(tTexProceduralStruct* procedural)
{
    tTexProcPlasma* common; // esi

    common = (tTexProcPlasma*)procedural->proceduralTexPtr;  // a water, fire, plasma struktúrának ugyanaz az eleje, ezért ez az egy függvény jó rájuk
    MEM_Free(common->ptr1);
    MEM_Free(common->ptr2);
    MEM_Free(common);
}

void TEXPRO_PhotoFree(tTexProceduralStruct* procedural)
{
    MEM_Free(procedural->proceduralTexPtr);
}

void TEXPRO_DummyDisrupt(tTexProceduralStruct* procedural) {

}

void TEXPRO_DummyFree(tTexProceduralStruct* procedural) {
}

void* TEXPRO_pv_EmptyLoad(char* offset, int size, int width, int height) {
    return NULL;
}

int TEXPRO_l_DummyUpdate(tTexProceduralStruct* procedural) {
    return 0;
}

void TEX_Procedural_Init()
{
    int i;

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
    for (i = 0; i < 256; ++i)
        TEXPRO_Cos[i] = (unsigned int)(cos((double)i * 0.049087387) * 30.0);
}

void TEXPRO_Photo_SetMapGao(struct sTexProceduralStruct* procedural, struct sGameObjectMain* gao)
{
    tGEOGRO* gro;
    tTexProcPhoto* photo;
    int i;
    int j;
    double dot;
    tDDContainerStruct* container;
    Mtx4x4* absolute;
    int16_t* coord;
    float best;
    int offsetX;
    int offsetY;
    float fOffsetX;
    float fOffsetY;
    int order[4];
    Vec3 edge0;
    Vec3 edge1;
    Mtx4x4 viewMtx;
    Vec3* projWalk;

    if (!gpst_DX_stDD || !procedural || !gao)
        return;
    if (gpst_DX_stDD->winWidth < 512 || gpst_DX_stDD->winHeight < 256)
        return;
    photo = (tTexProcPhoto*)procedural->proceduralTexPtr;
    gro = (tGEOGRO*)OBJ_p_GetGro(gao);
    if (!gro)
        return;
    if (gro->intf->id != 1 || gro->vertexNum != 4)
        return;
    best = 0.0;
    order[0] = 0;
    for (i = 1; i <= 3; ++i)
    {
        MATH_SubstractVec(&edge0, gro->vertices, &gro->vertices[i]);
        j = i + 1;
        if (i + 1 > 3)
            j = i - 2;
        MATH_SubstractVec(&edge1, &gro->vertices[j], &gro->vertices[6 - j - i]);
        dot = (float)(edge1.z * edge0.z + edge1.y * edge0.y + edge1.x * edge0.x);
        if (dot < 0.0)
        {
            dot = -dot;
            j = 6 - j - i;
        }
        if (dot > best)
        {
            best = (float)dot;
            order[1] = i;
            order[2] = 6 - j - i;
            order[3] = j;
        }
    }
    offsetX = (gpst_DX_stDD->winWidth - 512) / 2;
    container = &gpst_DX_stDD->ct;
    offsetY = (gpst_DX_stDD->winHeight - 256) / 2;
    absolute = OBJ_pst_GetAbsoluteMatrix(gao);
    MATH_MulMatrixMatrix(&viewMtx, absolute, &gpst_DX_stDD->ct.cam.cameraMtx);
    fOffsetX = (float)offsetX;
    container->cam.viewMtxPtr_setInGRORender = &viewMtx;
    coord = &photo->sa[4]; // TODO megcsinálni igényesre
    fOffsetY = (float)offsetY;
    // preserved: the walk starts in edge0 and advances one Vec3 per corner,
    // so the third and fourth projections land past edge1, on viewMtx --
    // the matrix container->cam is pointing at while they are taken.
    projWalk = &edge0;
    for (i = 0; i < 4; ++i)
    {
        SOFT_TransformAndProject(projWalk, &gro->vertices[order[i]], 1, &container->cam);
        *(coord - 4) = (unsigned int)((projWalk->x - fOffsetX) * 0.5);
        *coord = (unsigned int)((projWalk->y - fOffsetY) * 0.5);
        ++coord;
        ++projWalk;
    }
    // preserved: no test in the file matches 0x10000000, so the reveal this field drives never starts
    photo->x = 0x10000000;
    photo->remainingTime = 1.0;
    photo->f1 = 1.0;
}
