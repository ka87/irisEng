#include "common.h"
#include "loading.h"
#include "memory.h"
#include "bigfiles.h"
#include "textures.h"
#include "minilzo/minilzo.h"
#include "ai/ai_debugfunctions.h"
#include "dbg.h"

BOOL LOA_gb_DoReadAllFats = TRUE;
int BIGcomp_M_CompressedSize = 0x100000;
int BIGcomp_M_BlockSize = 0x7d000;
int gi_TestBin = -1;
int gi_CurBin = -1;
int BIG_gi_SpecialRead = 0; // game doesn't set it expicitly

void BIG_InitModule(void)
{
    LOA_InitModule();
}

void BIG_CloseModule(void)
{
    LOA_CloseModule();
}

int BIG_ul_SearchKeyToPos(uint32_t key)
{
    uint32_t ret;

    ret = BAS_bsearch(key, &BIG_gst.bfBigKeyToPos);
    if (ret == -1)
    {
        if (LOA_gb_SpeedMode)
            ret = key;
    }
    return ret;
}

void BIG_FreeGlobalBuffer()
{
    if (BIG_gp_GlobalBuffer && MEM_gp_AllocTmpFirst)
        MEM_Free(BIG_gp_GlobalBuffer);
    BIG_gp_GlobalBuffer = NULL;
    BIG_gi_GlobalBufferSize = 0;
    if (BIG_gp_GlobalSpecialBuffer)             // BIG_FreeSpecialBuffer inline
        MEM_Free(BIG_gp_GlobalSpecialBuffer);
    BIG_gp_GlobalSpecialBuffer = NULL;
    BIG_gi_GlobalSpecialBufferSize = 0;
}

int BIG_fseek(FILE* fp, int offset, int origin)
{
    fpos_t posToSet;

    if (origin)
    {
        if (origin == 1)
        {
            fgetpos(fp, &posToSet);
            posToSet += (unsigned int)offset;
            return fsetpos(fp, &posToSet);
        }
        else
        {
            return fseek(fp, offset, origin);
        }
    }
    else
    {
        posToSet = (unsigned int)offset;
        return fsetpos(fp, &posToSet);
    }
}

int BIGcomp_ui_Read32(char* dataPtr)
{
    return *(uint32_t*)dataPtr;
}

BOOL BIGcomp_CheckBlock(char* dataPtr, int size)
{
    if (LOA_gb_CompressBin)
        return (size >= BIGcomp_ui_Read32(dataPtr + 4) + 8);
    else
        return (size >= BIGcomp_M_BlockSize);
}

BOOL BIGcomp_CheckBuffer(char* dataPtr, int size)
{
    if (LOA_gb_CompressBin)
        return (size >= BIGcomp_ui_Read32(dataPtr));
    else
        return (size >= BIGcomp_M_BlockSize);
}

int BIGcomp_i_CompressedSize(char* dataPtr)
{
    if (LOA_gb_CompressBin)
        return BIGcomp_ui_Read32(dataPtr + 4) + 8;
    else
        return BIGcomp_M_BlockSize;
}

int BIGcomp_i_CompressedSize_v2(char* dataPtr)
{
    if (LOA_gb_CompressBin)
        return BIGcomp_ui_Read32(dataPtr);
    else
        return BIGcomp_M_BlockSize;
}

char* BIGcomp_DecodeBlocks(BIGCompressedBlockStruct* cbs, unsigned int blocksNum)
{
    char* currBuf, *destBuf;
    unsigned int compressedSize, uncompressedSize;
    char* result;
    int sizeRet;
    unsigned int i;

    if (LOA_gb_CompressBin)
    {
        destBuf = cbs->dest;
        currBuf = cbs->src;

        cbs->totalSize = 0;
 
        for (i = 0; i < blocksNum; i++)
        {
            uncompressedSize = BIGcomp_ui_Read32(currBuf);
            currBuf += sizeof(uint32_t);

            if (!uncompressedSize)
                break;

            compressedSize = BIGcomp_ui_Read32(currBuf);
            currBuf += sizeof(uint32_t);

            if (compressedSize >= uncompressedSize)
            {
                memcpy(destBuf, currBuf, compressedSize);
            }
            else
            {
                sizeRet = uncompressedSize;
                lzo1x_decompress(currBuf, compressedSize, destBuf, &sizeRet, NULL);
                if (sizeRet != uncompressedSize)
                    DBG_assert(0, __FILE__, __LINE__, "0", "Decode error", 0, 1);
            }
            currBuf += compressedSize;
            cbs->totalSize += uncompressedSize;
            destBuf += uncompressedSize;
        }
        result = currBuf;
    }
    else
    {
        memcpy(cbs->dest, cbs->src, blocksNum * BIGcomp_M_BlockSize);
        cbs->totalSize = blocksNum * BIGcomp_M_BlockSize;
        result = cbs->src + cbs->totalSize;
    }
    return result;
}

int BIG_fseekAndRead(FILE* fp, int offset, void** dst, unsigned int size)
{
    BIG_fseek(fp, offset, 0);
    
    return fread(*dst, 1, size, fp);
}

int BIG_fread(char* dstBuf, unsigned int sizeToRead, FILE* fp)
{
    BIGCompressedBlockStruct block;
    void* newBuffer;
    char* preloaded;
    char* binPtr;
    char* kept;
    signed int preloadedSize;
    signed int cachedSize;
    int fromPreloadedBin;
    int blockSize;
    int keptSize;
    int restSize;
    int sizeToReadFromFile;
    int dwordCount;
    int movedSize;
    signed int offset;
    int bin;
    int preloadTexFinished;
    unsigned int buffered;
    unsigned int decompressedLeft;
    int sizeLeft;

    preloadedSize = 0;
    fromPreloadedBin = 0;

    if (BIG_gi_ReadMode != 2)
        return fread(dstBuf, 1, sizeToRead, fp) / (int)sizeToRead;

    sizeLeft = sizeToRead;
    if (!BIG_gp_ReadBuffer)
    {
        BIG_gp_ReadBuffer = (char*)MEM_p_AllocFromEnd(BIG_gi_ReadSize);// ez a beginseedmode-ba 0x200000 lesz

        preloadTexFinished = 0;
        PreLoadTex_Finished(&preloadTexFinished);                     // ez semmit nem csinál csak átállítja 1-re a változót

        if (!preloadTexFinished)
            AI_EvalFunc_DbgTraceInt_C();
        if (gi_TestBin == -1                         // a testbin 0 ha pályát töltünk be
            || gi_TestBin > gi_CurBin
            || (cachedSize = gai_CurSize[gi_TestBin]) == 0
            || (preloaded = gap_FirstPointer[gi_TestBin], (int)&preloaded[-cachedSize] <= MEM_gp_MaxBlock)) // ??? TODO FIXME
        {
            BIG_fseek(fp, BIG_gul_ReadPos, 0);
            fread(BIG_gp_ReadBuffer, 1, 2048, fp);
            blockSize = *(uint32_t*)BIG_gp_ReadBuffer;
            BIG_si_BinFileSize = blockSize;
        }
        else
        {
            blockSize = *((uint32_t*)preloaded - 0x2800);
            preloadedSize = gai_CurSize[gi_TestBin];
            BIG_si_BinFileSize = blockSize;
            if (cachedSize > blockSize)
                preloadedSize = blockSize;
            fromPreloadedBin = 1;
        }
        if (BIGcomp_M_CompressedSize < blockSize)
            blockSize = BIGcomp_M_CompressedSize;
        BIG_si_BinFileReadSize = blockSize;
        BIG_si_CompressedSize = blockSize;
        BIG_gp_CompressedBuffer = (char*)MEM_p_AllocTmp(BIGcomp_M_CompressedSize + 6144);
        BIG_gp_CompressedBufferPtrBackup = BIG_gp_CompressedBuffer;
        if (!fromPreloadedBin || preloadedSize < BIG_si_BinFileReadSize)
        {
            BIG_fseek(fp, preloadedSize + BIG_gul_ReadPos, 0);
            BIG_gp_CompressedBuffer += preloadedSize;
            fread(BIG_gp_CompressedBuffer, 1, BIG_si_CompressedSize - preloadedSize, fp);
            BIG_gp_CompressedBuffer -= preloadedSize;
        }

        BIG_si_BinFileSize = BIG_si_BinFileSize - BIG_si_BinFileReadSize <= 0
            ? 0
            : BIG_si_BinFileSize - BIG_si_BinFileReadSize;
        if (fromPreloadedBin)
        {
            for (offset = 0; offset < (int)gai_CurSize[gi_TestBin]; offset += 40960)
                memcpy(&BIG_gp_CompressedBuffer[offset], &gap_FirstPointer[gi_TestBin][-offset - 40960], 40960u);
            movedSize = offset - 40960;

            for (bin = 0; bin < gi_CurBin; ++bin)
            {
                binPtr = gap_FirstPointer[bin];
                if (binPtr < gap_FirstPointer[gi_TestBin])
                {
                    memcpy(&binPtr[movedSize - gai_CurSize[bin]], &binPtr[-(int)gai_CurSize[bin]], gai_CurSize[bin]);
                    gap_FirstPointer[bin] += movedSize;
                }
            }
        }
        BIG_si_BinFileReadSize = BIG_si_BinFileSize;
        if (BIG_si_BinFileSize >= BIGcomp_M_CompressedSize)
            BIG_si_BinFileReadSize = BIGcomp_M_CompressedSize;
        BIG_gi_ReadSeek = 0;
        block.i4 = 0;
        block.src = BIG_gp_CompressedBuffer + 4;
        block.dest = BIG_gp_ReadBuffer;
        BIG_sp_CompressedNextBlock = BIGcomp_DecodeBlocks(&block, 1);
        BIG_gi_RealSeek = BIG_sp_CompressedNextBlock - BIG_gp_CompressedBuffer;
        BIG_si_DecompressedSize = block.totalSize;
        BIG_ul_CachePos = BIG_si_CompressedSize + BIG_gul_ReadPos - 2048;
    }
    else if ((int)(BIG_gi_ReadSeek + sizeToRead) > BIG_si_DecompressedSize)
    {
        buffered = BIG_si_DecompressedSize - BIG_gi_ReadSeek;
        if (dstBuf)
        {
            memcpy(dstBuf, &BIG_gp_ReadBuffer[BIG_gi_ReadSeek], buffered);
            dstBuf += buffered;
        }
        BIG_gi_ReadSeek += buffered;
        sizeLeft = sizeToRead - buffered;

        if (sizeLeft > BIG_gi_ReadSize)
        {
            MEM_FreeFromEnd(BIG_gp_ReadBuffer);
            newBuffer = MEM_p_AllocFromEnd(sizeLeft);
            memcpy(newBuffer, BIG_gp_ReadBuffer, BIG_gi_ReadSize);
            BIG_gp_ReadBuffer = (char*)newBuffer;
            BIG_gi_ReadSize = sizeLeft;
            BIG_gi_ReadSeek = 0;
            BIG_si_DecompressedSize = 0;
        }
        else if ((int)(sizeLeft + buffered) > BIG_gi_ReadSize)
        {
            BIG_gi_ReadSeek = 0;
            BIG_si_DecompressedSize = 0;
        }

        for (;;)
        {
            if (!BIGcomp_CheckBlock(BIG_sp_CompressedNextBlock,
                                    BIG_si_CompressedSize + BIG_gp_CompressedBuffer - BIG_sp_CompressedNextBlock))
            {
                keptSize = ((BIG_gp_CompressedBuffer - BIG_sp_CompressedNextBlock + BIG_si_CompressedSize) / 2048) << 11;
                restSize = BIG_si_CompressedSize + BIG_gp_CompressedBuffer - BIG_sp_CompressedNextBlock - keptSize;

                BIGcomp_i_CompressedSize(BIG_sp_CompressedNextBlock);

                memcpy(BIG_gp_CompressedBufferPtrBackup, BIG_sp_CompressedNextBlock, keptSize);

                BIG_gp_CompressedBuffer = &BIG_gp_CompressedBufferPtrBackup[keptSize];
                BIG_si_BinFileSize = ((BIG_si_BinFileSize + restSize + 2047) / 2048) << 11;
                sizeToReadFromFile = BIG_si_BinFileSize;

                if (BIG_si_BinFileSize >= (int)BIGcomp_M_CompressedSize - keptSize)
                    sizeToReadFromFile = BIGcomp_M_CompressedSize - keptSize;
                BIG_si_BinFileReadSize = sizeToReadFromFile;
                BIG_si_CompressedSize = sizeToReadFromFile;

                BIG_fseekAndRead(fp, keptSize + BIG_gul_ReadPos + BIG_gi_RealSeek,
                                 (void**)&BIG_gp_CompressedBuffer, sizeToReadFromFile);

                BIG_si_BinFileSize = BIG_si_BinFileSize - BIG_si_BinFileReadSize <= 0
                    ? 0
                    : BIG_si_BinFileSize - BIG_si_BinFileReadSize;
                BIG_si_BinFileReadSize = BIG_si_BinFileSize;
                if (BIG_si_BinFileSize >= BIGcomp_M_CompressedSize)
                    BIG_si_BinFileReadSize = BIGcomp_M_CompressedSize;

                kept = &BIG_gp_CompressedBufferPtrBackup[keptSize];
                if (BIG_gp_CompressedBuffer != kept)
                {
                    if (BIG_si_CompressedSize <= keptSize)
                    {
                        dwordCount = (unsigned int)BIG_si_CompressedSize >> 2;
                        memcpy(kept, BIG_gp_CompressedBuffer, 4 * dwordCount);
                        memcpy(&kept[4 * dwordCount], &BIG_gp_CompressedBuffer[4 * dwordCount],
                               BIG_si_CompressedSize & 3);
                        BIG_gp_CompressedBuffer = BIG_gp_CompressedBufferPtrBackup;
                    }
                    else
                    {
                        // preserved: this lands before the address of the pointer variable, where the twin writes before the buffer it points at
                        memcpy(&BIG_gp_CompressedBuffer - keptSize, BIG_gp_CompressedBufferPtrBackup, keptSize);
                        BIG_gp_CompressedBuffer -= keptSize;
                    }
                }
                else
                {
                    BIG_gp_CompressedBuffer = BIG_gp_CompressedBufferPtrBackup;
                }

                BIG_si_CompressedSize += keptSize;
                BIG_sp_CompressedNextBlock = BIG_gp_CompressedBuffer;
            }
            if (!BIGcomp_CheckBuffer(BIG_sp_CompressedNextBlock, BIG_gi_ReadSize - BIG_si_DecompressedSize))
            {
                decompressedLeft = BIG_si_DecompressedSize - BIG_gi_ReadSeek;
                if (BIG_si_DecompressedSize != BIG_gi_ReadSeek && dstBuf)
                {
                    memcpy(dstBuf, &BIG_gp_ReadBuffer[BIG_gi_ReadSeek], 4 * (decompressedLeft >> 2));
                    dstBuf += decompressedLeft;
                    // preserved: the tail of this split copy goes one whole run past the dwords it belongs behind
                    memcpy(&dstBuf[4 * (decompressedLeft >> 2)],
                           &BIG_gp_ReadBuffer[BIG_gi_ReadSeek + (4 * (decompressedLeft >> 2))],
                           decompressedLeft & 3);
                }
                BIG_gi_ReadSeek = 0;
                BIG_si_DecompressedSize = 0;
                sizeLeft = (int)(sizeLeft - decompressedLeft) <= 0 ? 0 : (int)(sizeLeft - decompressedLeft);
            }
            block.src = BIG_sp_CompressedNextBlock;
            block.i4 = 0;
            block.dest = &BIG_gp_ReadBuffer[BIG_si_DecompressedSize];
            BIG_sp_CompressedNextBlock = BIGcomp_DecodeBlocks(&block, 1);
            BIG_gi_RealSeek += BIG_sp_CompressedNextBlock - block.src;
            BIG_si_DecompressedSize += block.totalSize;
            // preserved: the block is counted twice in this test, where the twin in BIG_SpeedMode_fread tests the sum before the increment
            if (block.totalSize + BIG_si_DecompressedSize >= sizeLeft)
                break;
        }
    }

    if (dstBuf)
        memcpy(dstBuf, &BIG_gp_ReadBuffer[BIG_gi_ReadSeek], sizeLeft);
    BIG_gi_ReadSeek += sizeLeft;

    return 1;
}

int BIG_SpeedMode_fread(char** currOffset, int size, FILE* fp)
{
    BIGCompressedBlockStruct block; // [esp+14h] [ebp-14h] BYREF
    void* newBuffer; // esi
    char* preloaded; // ecx
    char* binPtr; // eax
    char* kept; // edi
    signed int preloadedSize; // esi
    signed int cachedSize; // eax
    int fromPreloadedBin; // ebx
    int blockSize; // ecx
    int keptSize; // ebx
    int restSize; // ebp
    int sizeToReadFromFile; // eax
    int dwordCount; // ecx
    char tailSize; // al
    int movedSize; // ebx
    signed int offset; // ebx
    int bin; // esi
    int preloadTexFinished; // [esp+10h] [ebp-18h] BYREF
    int more; // cc

    preloadedSize = 0;
    fromPreloadedBin = 0;

    if (!BIG_gp_ReadBuffer)
    {
        BIG_gp_ReadBuffer = (char*)MEM_p_AllocFromEnd(BIG_gi_ReadSize);

        preloadTexFinished = 0;
        PreLoadTex_Finished(&preloadTexFinished);

        if (!preloadTexFinished)
            AI_EvalFunc_DbgTraceInt_C();
        if (gi_TestBin == -1
            || gi_TestBin > gi_CurBin
            || (cachedSize = gai_CurSize[gi_TestBin]) == 0
            || (preloaded = gap_FirstPointer[gi_TestBin], (int)&preloaded[-cachedSize] <= MEM_gp_MaxBlock)) // ??? TODO
        {
            BIG_fseek(fp, BIG_gul_ReadPos, 0);
            fread(BIG_gp_ReadBuffer, 1, 2048, fp);
            blockSize = *(uint32_t*)BIG_gp_ReadBuffer;
        }
        else
        {
            blockSize = *((uint32_t*)preloaded - 0x2800);
            preloadedSize = gai_CurSize[gi_TestBin];
            fromPreloadedBin = 1;
        }
        BIG_si_BinFileSize = blockSize;
        if (BIGcomp_M_CompressedSize < blockSize)
            blockSize = BIGcomp_M_CompressedSize;
        BIG_si_BinFileReadSize = blockSize;
        BIG_si_CompressedSize = blockSize;
        BIG_gp_CompressedBuffer = (char*)MEM_p_AllocTmp(BIGcomp_M_CompressedSize + 6144);
        BIG_gp_CompressedBufferPtrBackup = BIG_gp_CompressedBuffer;
        if (!fromPreloadedBin || preloadedSize < BIG_si_BinFileReadSize)
        {
            BIG_fseek(fp, preloadedSize + BIG_gul_ReadPos, 0);
            BIG_gp_CompressedBuffer += preloadedSize;
            fread(BIG_gp_CompressedBuffer, 1, BIG_si_CompressedSize - preloadedSize, fp);
            BIG_gp_CompressedBuffer -= preloadedSize;
        }

        BIG_si_BinFileSize = BIG_si_BinFileSize - BIG_si_BinFileReadSize <= 0
            ? 0
            : BIG_si_BinFileSize - BIG_si_BinFileReadSize;
        if (fromPreloadedBin)
        {
            for (offset = 0; offset < (int)gai_CurSize[gi_TestBin]; offset += 40960)
                memcpy(&BIG_gp_CompressedBuffer[offset], &gap_FirstPointer[gi_TestBin][-offset - 40960], 40960u);
            movedSize = offset - 40960;

            for (bin = 0; bin < gi_CurBin; ++bin)
            {
                binPtr = gap_FirstPointer[bin];
                if (binPtr < gap_FirstPointer[gi_TestBin])
                {
                    memcpy(&binPtr[movedSize - gai_CurSize[bin]], &binPtr[-(int)gai_CurSize[bin]], gai_CurSize[bin]);
                    gap_FirstPointer[bin] += movedSize;
                }
            }
        }
        BIG_si_BinFileReadSize = BIG_si_BinFileSize;
        if (BIG_si_BinFileSize >= BIGcomp_M_CompressedSize)
            BIG_si_BinFileReadSize = BIGcomp_M_CompressedSize;
        BIG_gi_ReadSeek = 0;
        block.i4 = 0;
        block.src = BIG_gp_CompressedBuffer + 4;
        block.dest = BIG_gp_ReadBuffer;
        BIG_sp_CompressedNextBlock = BIGcomp_DecodeBlocks(&block, 1);
        BIG_gi_RealSeek = BIG_sp_CompressedNextBlock - BIG_gp_CompressedBuffer;
        BIG_si_DecompressedSize = block.totalSize;
        BIG_ul_CachePos = BIG_si_CompressedSize + BIG_gul_ReadPos - 2048;
    }
    else if (BIG_gi_ReadSeek + size > BIG_si_DecompressedSize)
    {
        BIG_si_DecompressedSize -= BIG_gi_ReadSeek;
        if (BIG_si_DecompressedSize)
            memcpy(BIG_gp_ReadBuffer, &BIG_gp_ReadBuffer[BIG_gi_ReadSeek], BIG_si_DecompressedSize);
        BIG_gi_ReadSeek = 0;
        if (size > BIG_gi_ReadSize)
        {
            MEM_FreeFromEnd(BIG_gp_ReadBuffer);
            BIG_gi_ReadSize = size;
            newBuffer = MEM_p_AllocFromEnd(size);
            memcpy(newBuffer, BIG_gp_ReadBuffer, BIG_si_DecompressedSize);
            BIG_gp_ReadBuffer = (char*)newBuffer;
            BIG_gi_ReadSeek = 0;
        }

        for (;;)
        {
            if (!BIGcomp_CheckBlock(BIG_sp_CompressedNextBlock,
                                    BIG_si_CompressedSize + BIG_gp_CompressedBuffer - BIG_sp_CompressedNextBlock))
            {
                keptSize = ((BIG_gp_CompressedBuffer - BIG_sp_CompressedNextBlock + BIG_si_CompressedSize) / 2048) << 11;
                restSize = BIG_si_CompressedSize + BIG_gp_CompressedBuffer - BIG_sp_CompressedNextBlock - keptSize;

                BIGcomp_i_CompressedSize(BIG_sp_CompressedNextBlock);

                memcpy(BIG_gp_CompressedBufferPtrBackup, BIG_sp_CompressedNextBlock, keptSize);

                BIG_gp_CompressedBuffer = &BIG_gp_CompressedBufferPtrBackup[keptSize];
                BIG_si_BinFileSize = ((BIG_si_BinFileSize + restSize + 2047) / 2048) << 11;
                sizeToReadFromFile = BIG_si_BinFileSize;

                if (BIG_si_BinFileSize >= BIGcomp_M_CompressedSize - keptSize)
                    sizeToReadFromFile = BIGcomp_M_CompressedSize - keptSize;
                BIG_si_BinFileReadSize = sizeToReadFromFile;
                BIG_si_CompressedSize = sizeToReadFromFile;

                BIG_fseekAndRead(fp, keptSize + BIG_gul_ReadPos + BIG_gi_RealSeek,
                                 (void**)&BIG_gp_CompressedBuffer, sizeToReadFromFile);

                BIG_si_BinFileSize = BIG_si_BinFileSize - BIG_si_BinFileReadSize <= 0
                    ? 0
                    : BIG_si_BinFileSize - BIG_si_BinFileReadSize;
                BIG_si_BinFileReadSize = BIG_si_BinFileSize;
                if (BIG_si_BinFileSize >= BIGcomp_M_CompressedSize)
                    BIG_si_BinFileReadSize = BIGcomp_M_CompressedSize;

                kept = &BIG_gp_CompressedBufferPtrBackup[keptSize];
                if (BIG_gp_CompressedBuffer != kept)
                {
                    if (BIG_si_CompressedSize <= keptSize)
                    {
                        tailSize = BIG_si_CompressedSize;
                        dwordCount = (unsigned int)BIG_si_CompressedSize >> 2;
                        memcpy(kept, BIG_gp_CompressedBuffer, 4 * dwordCount);
                        memcpy(&kept[4 * dwordCount], &BIG_gp_CompressedBuffer[4 * dwordCount],
                               tailSize & 3);
                        BIG_gp_CompressedBuffer = BIG_gp_CompressedBufferPtrBackup;
                    }
                    else
                    {
                        memcpy(&BIG_gp_CompressedBuffer[-keptSize], BIG_gp_CompressedBufferPtrBackup, keptSize);
                        BIG_gp_CompressedBuffer -= keptSize;
                    }
                }
                else
                {
                    BIG_gp_CompressedBuffer = BIG_gp_CompressedBufferPtrBackup;
                }

                BIG_si_CompressedSize += keptSize;
                BIG_sp_CompressedNextBlock = BIG_gp_CompressedBuffer;
            }
            if (!BIGcomp_CheckBuffer(BIG_sp_CompressedNextBlock, BIG_gi_ReadSize - BIG_si_DecompressedSize))
            {
                MEM_FreeFromEnd(BIG_gp_ReadBuffer);
                BIG_gi_ReadSize = BIG_si_DecompressedSize + BIGcomp_i_CompressedSize_v2(BIG_sp_CompressedNextBlock);
                newBuffer = MEM_p_AllocFromEnd(BIG_gi_ReadSize);
                memcpy(newBuffer, BIG_gp_ReadBuffer, BIG_si_DecompressedSize);
                BIG_gp_ReadBuffer = (char*)newBuffer;
            }
            block.src = BIG_sp_CompressedNextBlock;
            block.i4 = 0;
            block.dest = &BIG_gp_ReadBuffer[BIG_si_DecompressedSize];
            BIG_sp_CompressedNextBlock = BIGcomp_DecodeBlocks(&block, 1u);
            more = block.totalSize + BIG_si_DecompressedSize < size;
            BIG_gi_RealSeek += BIG_sp_CompressedNextBlock - block.src;
            BIG_si_DecompressedSize += block.totalSize;
            if (!more)
                break;
        }
    }

    *currOffset = &BIG_gp_ReadBuffer[BIG_gi_ReadSeek];
    BIG_gi_ReadSeek += size;

    return 1;
}

void BIG_InsertKeyToPos(unsigned int pos, int key)
{
    BAS_binsert(pos, key, &BIG_gst.bfBigKeyToPos);
}

void BIG_ReadHeader()
{
    char* currPtr;

    if (BIG_fread((char *)&BIG_gst.hdr, sizeof(BIGHeaderStruct), BIG_gst.CLibFileHandle) != 1)
        DBG_assert(0, __FILE__, __LINE__, "", "Error while reading file", NULL, 1);

    currPtr = (char*)&BIG_gst.hdr;
    LOA_ReadCharArray(&currPtr, BIG_gst.hdr.ac_def, 4, 1);
    BIG_gst.hdr.unk2 = LOA_ReadLong(&currPtr, 0, 1);
    BIG_gst.hdr.keyNum = LOA_ReadLong(&currPtr, 0, 1);
    BIG_gst.hdr.unk4 = LOA_ReadLong(&currPtr, 0, 1);
    BIG_gst.hdr.unk5 = LOA_ReadLong(&currPtr, 0, 1);
    BIG_gst.hdr.unk6 = LOA_ReadLong(&currPtr, 0, 1);
    BIG_gst.hdr.unk7 = LOA_ReadLong(&currPtr, 0, 1);
    BIG_gst.hdr.unk8 = LOA_ReadLong(&currPtr, 0, 1);
    BIG_gst.hdr.unk9 = LOA_ReadLong(&currPtr, 0, 1);
    BIG_gst.hdr.fatTableNum = LOA_ReadLong(&currPtr, 0, 1);
    BIG_gst.hdr.startKey = LOA_ReadLong(&currPtr, 0, 1);

    if (strcmp(BIG_gst.hdr.ac_def, "BIG"))
        DBG_assert(0, __FILE__, __LINE__, "", "Big file corrupt", NULL, 1);
}

void BIG_ReadFatFile(BIGFatTableElem* fatElem)
{
    int i, j, fatElemNumToRead;
    char* currPtr;

    if (fatElem->fatNum)
    {
        if (BIG_fseek(BIG_gst.CLibFileHandle, fatElem->posFat, 0))
            DBG_assert(0, __FILE__, __LINE__, "", "fseek error", 0, 1);

        for (i = 0; i < fatElem->fatNum; i += 100)
        {
            fatElemNumToRead = 100;

            if (i + 100 > fatElem->fatNum)
                fatElemNumToRead = fatElem->fatNum - i;
            if (BIG_fread((char*)BIG_gst.bfFileTable, sizeof(BIGFileTableElem) * fatElemNumToRead, BIG_gst.CLibFileHandle) != 1)
                DBG_assert(0, __FILE__, __LINE__, "", "Error while reading file", 0, 1);

            currPtr = (char*)BIG_gst.bfFileTable;

            for (j = 0; j < fatElemNumToRead; j++)
            {
                BIG_gst.bfFileTable[j].key = LOA_ReadLong(&currPtr, 0, 1);
                BIG_gst.bfFileTable[j].pos = LOA_ReadLong(&currPtr, 0, 1);

                if (BIG_gst.bfFileTable[j].pos != -1)
                {
                    BIG_InsertKeyToPos(BIG_gst.bfFileTable[j].pos, BIG_gst.bfFileTable[j].key);
                }
            }
        }
    }
}

void BIG_ReadAllFats()
{
    int i;
    char* currPtr = NULL;
    
    BIG_gst.bfFatTable = (BIGFatTableElem*)MEM_p_Alloc(sizeof(BIGFatTableElem) * BIG_gst.hdr.fatTableNum);
    
    if (!BIG_gst.bfFatTable)
        DBG_assert(0, __FILE__, __LINE__, "", "Not enough memory", 0, 1);

    for (i = 0; i < BIG_gst.hdr.fatTableNum; i++)
    {
        if (BIG_fread((char*)&BIG_gst.bfFatTable[i], sizeof(BIGFatTableElem), BIG_gst.CLibFileHandle) != 1)
            DBG_assert(0, __FILE__, __LINE__, "", "Error while reading file", 0, 1);

        currPtr = (char *)(&BIG_gst.bfFatTable[i]);

        BIG_gst.bfFatTable[i].fatNum = LOA_ReadLong(&currPtr, 0, 1);
        BIG_gst.bfFatTable[i].i2 = LOA_ReadLong(&currPtr, 0, 1);
        BIG_gst.bfFatTable[i].posFat = LOA_ReadLong(&currPtr, 0, 1);
        BIG_gst.bfFatTable[i].nextPosFat = LOA_ReadLong(&currPtr, 0, 1);
        BIG_gst.bfFatTable[i].i5 = LOA_ReadLong(&currPtr, 0, 1);
        BIG_gst.bfFatTable[i].i6 = LOA_ReadLong(&currPtr, 0, 1);

        if (BIG_gst.bfFatTable[i].nextPosFat != -1 && BIG_fseek(BIG_gst.CLibFileHandle, BIG_gst.bfFatTable[i].nextPosFat - sizeof(BIGFatTableElem), 0))
            DBG_assert(0, __FILE__, __LINE__, "", "fseek error", 0, 1);
    }
    
    BIG_gst.bfFileTable = (BIGFileTableElem*)MEM_p_Alloc(100 * sizeof(BASKeyValStruct));
    if (!BIG_gst.bfFileTable)
        DBG_assert(0, __FILE__, __LINE__, "", "Not enough memory", 0, 1);
    BAS_binit(&BIG_gst.bfBigKeyToPos, 0);
    BIG_gst.bfBigKeyToPos.elemNumToAddWhenFull = 200;
    BIG_gst.bfBigKeyToPos.maxElemNum = BIG_gst.hdr.keyNum + 1;
    BIG_gst.bfBigKeyToPos.keyValPtr = (BASKeyValStruct*)MEM_p_Alloc(sizeof(BASKeyValStruct) * (BIG_gst.hdr.keyNum + 1));
    
    if (!BIG_gst.bfBigKeyToPos.keyValPtr)
        DBG_assert(0, __FILE__, __LINE__, "", "Not enough memory", 0, 1);

    BIG_gst.hdr.unk5 = 0;
    BAS_bsortmode = 0;
    
    for (i = 0; i < BIG_gst.hdr.fatTableNum; i++)
    {
        BIG_ReadFatFile(&BIG_gst.bfFatTable[i]);
    }
    BAS_bsortmode = 1;
    BAS_bsort(&BIG_gst.bfBigKeyToPos);
    MEM_Free(BIG_gst.bfFileTable);
    BIG_gst.bfFileTable = NULL;
}

void BIG_Open(char* fileName)
{
    memset(&BIG_gst, 0, sizeof(BIG_gst));
    BIG_gst.CLibFileHandle = fopen(fileName, "rbS");
    if (!BIG_gst.CLibFileHandle)
    {
        if (_access(fileName, 2)) // 2 = write-only
        {
            _chmod(fileName, _S_IWRITE);
            BIG_gst.CLibFileHandle = fopen(fileName, "r+b");
        }
        if (!BIG_gst.CLibFileHandle)
            DBG_assert(0, __FILE__, __LINE__, "", "Unable to open file", fileName, 1);            
    }
    BIG_ReadHeader();
    if (LOA_gb_DoReadAllFats)
        BIG_ReadAllFats();
    strcpy(BIG_gst.filePath, fileName);
}

void* BIG_p_RequestBuffer(size_t size)
{
    void* ret;

    if ((int)size <= BIG_gi_GlobalBufferSize)
        return BIG_gp_GlobalBuffer;
    BIG_gi_GlobalBufferSize = size;
    if (BIG_gp_GlobalBuffer)
        ret = MEM_p_Realloc(BIG_gp_GlobalBuffer, size);
    else
        ret = MEM_p_AllocTmp(size);

    BIG_gp_GlobalBuffer = ret;

    return ret;
}

int BIG_SeekAndRead(FILE* fp, int offset, void** dest, unsigned int size)
{
    BIG_fseek(fp, offset, 0);

    // preserved: the read lands on the caller's pointer variable rather than on what it points at
    return fread(dest, 1, size, fp);
}

void BIG_ReadNoSeek(int pos, void* dest, unsigned int len)
{
    if (len)
    {
        if (BIG_gi_ReadMode != 2 && BIG_fseek(BIG_gst.CLibFileHandle, pos, 0))
            DBG_assert(0, __FILE__, __LINE__, "", "fseek error", 0, 1);
        if (BIG_fread((char*)dest, len, BIG_gst.CLibFileHandle) != 1)
            DBG_assert(0, __FILE__, __LINE__, "", "Error while reading file", 0, 1);
    }
}

void BIG_ResetFat()
{
    BAS_bfree(&BIG_gst.bfBigKeyToPos);
    MEM_Free(BIG_gst.bfFatTable);
    BIG_gst.bfFatTable = NULL;
}

void BIG_Close()
{
    BIG_ResetFat();
    if (BIG_gst.CLibFileHandle)
    {
        if (fclose(BIG_gst.CLibFileHandle))
            DBG_assert(0, __FILE__, __LINE__, "", "fclose error", 0, 1);           
    }
}

unsigned int BIG_ul_GetLengthFile(int offset)
{
    int sizeRet;
    char* currPtr;

    if (BIG_gi_ReadMode != 2 && BIG_fseek(BIG_gst.CLibFileHandle, offset, 0))
        DBG_assert(0, __FILE__, __LINE__, "", "fseek error", 0, 1);

    if ((!LOA_gb_SpeedMode || BIG_gi_ReadMode != 2) && BIG_fread((char*)&sizeRet, 4, BIG_gst.CLibFileHandle) != 1)
        DBG_assert(0, __FILE__, __LINE__, "", "Error while reading file", 0, 1);

    if (!LOA_gb_SpeedMode || BIG_gi_ReadMode != 2)
        return sizeRet;

    currPtr = LOA_FetchBuffer(4);
    
    return LOA_ReadLong(&currPtr, 0, 1);
}

char* BIG_pc_ReadFileTmp(int offset, int* size)
{
    char* buff;
    uint32_t len;

    if (BIG_gi_ReadMode == 2)
        return LOA_FetchFile(size);

    if (BIG_fseek(BIG_gst.CLibFileHandle, offset, 0))
        DBG_assert(0, __FILE__, __LINE__, "", "fseek error", 0, 1);

    if (BIG_fread((char*)&len, sizeof(len), BIG_gst.CLibFileHandle) != 1)
        DBG_assert(0, __FILE__, __LINE__, "", "Error while reading file", 0, 1);

    if (size)
        *size = len & 0x7fffffff;

    LOA_FetchFile(size); // FIXME: LEAK
    buff = BIG_p_RequestBuffer(len + 1);
    
    if (!buff)
        DBG_assert(0, __FILE__, __LINE__, "", "Not enough memory", 0, 1);
    
    if (!len)
        return buff;

    if (BIG_fread(buff, len, BIG_gst.CLibFileHandle) != 1)
        DBG_assert(0, __FILE__, __LINE__, "", "Error while reading file", 0, 1);
    
    buff[len] = '\0'; // trailing zero

    return buff;
}

void BINK_SeekAndReadFromBF(int offset, void* dstBuf, unsigned int size)
{
    if (size)
    {
        if (BIG_fseek(BIG_gst.CLibFileHandle, offset, 0))
            DBG_assert(0, __FILE__, __LINE__, "", "fseek error", 0, 1);            
        if (BIG_fread((char*)dstBuf, size, BIG_gst.CLibFileHandle) != 1)
            DBG_assert(0, __FILE__, __LINE__, "", "Error while reading file", 0, 1);
    }
}

// TODO FIXME: ez nem tudom mit csinál, de a PCs verzióban nem is mûködik, mert a gi_LastES nem fog átállni, mivel az egyetlen hely ahol átállna az a AI_EvalFunc_WORPreLoadTex_C(), aminek az elején van egy return
// és effektíve nem csinál semmit. Szóval a gi_LastES sosem áll át.
void PreLoadMoveBlocks(int worldKey)
{
    char* srcPtr, *dstPtr;
    size_t size;
    int i;

    if (gi_LastES != -1 && worldKey == gau_ES[gi_LastES])
    {
        srcPtr = (char*)(MEM_gst_MemoryInfo.memorySize + MEM_gst_MemoryInfo.memoryStartPointer);
        dstPtr = (char*)(MEM_gst_MemoryInfo.memorySize + MEM_gst_MemoryInfo.memoryStartPointer - 0x27D000);
        size = 0;

        for (i = 0; i <= gi_CurBin; ++i)
        {
            dstPtr -= gai_CurSize[i];
            srcPtr -= gai_CurSize[i];
            size += gai_CurSize[i];
            gap_FirstPointer[i] += 0xFFD83000;
        }

        if (MEM_gst_MemoryInfo.nextFreeMemoryPointer <= (unsigned int)dstPtr)
            memcpy(dstPtr, srcPtr, size);
        else
            gi_CurBin = -1;
    }
    else
    {
        gi_CurBin = -1;
    }
}
