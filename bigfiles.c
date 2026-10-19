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

int BIG_fread(char* DstBuf, unsigned int sizeToRead, FILE* Stream)
{
    signed int v3;
    int v4;
    char* readbuff;
    signed int v6;
    char* v7;
    int blockSize;
    int testBin;
    signed int currOffset;
    int v12;
    int v13;
    char* v14;
    int fullSizeToRead;
    char* ddestBuf;
    unsigned int remainingSizeToCopy;
    void* v19;
    char* nextBlock;
    int readSizeRounded;
    int v22;
    int lenToRead;
    char* v24;
    char* v25;
    char* v26;
    unsigned int v28;
    int v33;
    int preloadTexFinished;
    BIGCompressedBlockStruct v35;
    int ffullSizeToRead;

    v3 = 0;
    v4 = 0;
    if (BIG_gi_ReadMode != 2)
        return fread(DstBuf, 1, sizeToRead, Stream) / (int)sizeToRead;
    if (BIG_gp_ReadBuffer)
    {
        fullSizeToRead = sizeToRead;
        if ((int)(BIG_gi_ReadSeek + sizeToRead) <= BIG_si_DecompressedSize)
            goto LABEL_29;
        ddestBuf = DstBuf;
        remainingSizeToCopy = BIG_si_DecompressedSize - BIG_gi_ReadSeek;
        if (DstBuf)
        {
            memcpy(DstBuf, &BIG_gp_ReadBuffer[BIG_gi_ReadSeek], remainingSizeToCopy);
            ddestBuf = &DstBuf[remainingSizeToCopy];
            DstBuf += remainingSizeToCopy;
        }
        BIG_gi_ReadSeek += remainingSizeToCopy;
        fullSizeToRead = sizeToRead - remainingSizeToCopy;
        ffullSizeToRead = fullSizeToRead;
        if (fullSizeToRead <= BIG_gi_ReadSize)
        {
            if ((int)(fullSizeToRead + remainingSizeToCopy) <= BIG_gi_ReadSize)
            {
            LABEL_40:
                nextBlock = BIG_sp_CompressedNextBlock;
                while (BIGcomp_CheckBlock(nextBlock, BIG_si_CompressedSize + BIG_gp_CompressedBuffer - nextBlock))
                {
                LABEL_51:
                    if (!BIGcomp_CheckBuffer(BIG_sp_CompressedNextBlock, BIG_gi_ReadSize - BIG_si_DecompressedSize))
                    {
                        v28 = BIG_si_DecompressedSize - BIG_gi_ReadSeek;
                        if (BIG_si_DecompressedSize != BIG_gi_ReadSeek && ddestBuf)
                        {
                            memcpy(ddestBuf, &BIG_gp_ReadBuffer[BIG_gi_ReadSeek], 4 * (v28 >> 2));
                            ddestBuf += v28;
                            memcpy(&ddestBuf[4 * (v28 >> 2)], &BIG_gp_ReadBuffer[BIG_gi_ReadSeek + (4 * (v28 >> 2))], v28 & 3);
                            DstBuf = ddestBuf;
                        }
                        BIG_gi_ReadSeek = 0;
                        BIG_si_DecompressedSize = 0;
                        ffullSizeToRead = (fullSizeToRead - v28) & (((int)(fullSizeToRead - v28) <= 0) - 1);
                        fullSizeToRead = ffullSizeToRead;
                    }
                    v35.src = BIG_sp_CompressedNextBlock;
                    v35.i4 = 0;
                    v35.dest = &BIG_gp_ReadBuffer[BIG_si_DecompressedSize];
                    nextBlock = BIGcomp_DecodeBlocks(&v35, 1);                    
                    BIG_sp_CompressedNextBlock = nextBlock;
                    BIG_gi_RealSeek += nextBlock - v35.src;
                    BIG_si_DecompressedSize += v35.totalSize;
                    if (v35.totalSize + BIG_si_DecompressedSize >= fullSizeToRead)
                        goto LABEL_30;
                }
                readSizeRounded = ((BIG_gp_CompressedBuffer - BIG_sp_CompressedNextBlock + BIG_si_CompressedSize) / 2048) << 11;
                v22 = BIG_si_CompressedSize + BIG_gp_CompressedBuffer - BIG_sp_CompressedNextBlock - readSizeRounded;
                
                BIGcomp_i_CompressedSize(BIG_sp_CompressedNextBlock);
                
                memcpy(BIG_gp_CompressedBufferPtrBackup, BIG_sp_CompressedNextBlock, readSizeRounded);
                
                BIG_gp_CompressedBuffer = &BIG_gp_CompressedBufferPtrBackup[readSizeRounded];
                BIG_si_BinFileSize = ((BIG_si_BinFileSize + v22 + 2047) / 2048) << 11;
                lenToRead = BIG_si_BinFileSize;
                
                if (BIG_si_BinFileSize >= (int)BIGcomp_M_CompressedSize - readSizeRounded)
                    lenToRead = BIGcomp_M_CompressedSize - readSizeRounded;
                BIG_si_BinFileReadSize = lenToRead;
                BIG_si_CompressedSize = lenToRead;

                BIG_fseekAndRead(Stream, readSizeRounded + BIG_gul_ReadPos + BIG_gi_RealSeek, (void**)&BIG_gp_CompressedBuffer, lenToRead);
                
                BIG_si_BinFileSize = BIG_si_BinFileSize - BIG_si_BinFileReadSize <= 0 ? 0 : BIG_si_BinFileSize
                    - BIG_si_BinFileReadSize;
                BIG_si_BinFileReadSize = BIG_si_BinFileSize;
                if (BIG_si_BinFileSize >= BIGcomp_M_CompressedSize)
                    BIG_si_BinFileReadSize = BIGcomp_M_CompressedSize;
                v24 = BIG_gp_CompressedBufferPtrBackup;
                v25 = BIG_gp_CompressedBuffer;
                v26 = &BIG_gp_CompressedBufferPtrBackup[readSizeRounded];
                if (BIG_gp_CompressedBuffer != &BIG_gp_CompressedBufferPtrBackup[readSizeRounded])
                {
                    if (BIG_si_CompressedSize <= readSizeRounded)
                    {                        
                        v33 = (unsigned int)BIG_si_CompressedSize >> 2;
                        memcpy(v26, BIG_gp_CompressedBuffer, 4 * ((unsigned int)BIG_si_CompressedSize >> 2));
                        memcpy(&v26[4 * v33], &v25[4 * v33], BIG_si_CompressedSize & 3);
                        BIG_gp_CompressedBuffer = BIG_gp_CompressedBufferPtrBackup;
                        goto LABEL_50;
                    }
                    memcpy(&BIG_gp_CompressedBuffer - readSizeRounded, BIG_gp_CompressedBufferPtrBackup, readSizeRounded);
                    v24 = BIG_gp_CompressedBuffer - readSizeRounded;
                }
                BIG_gp_CompressedBuffer = v24;
            LABEL_50:
                ddestBuf = DstBuf;
                fullSizeToRead = ffullSizeToRead;
                BIG_si_CompressedSize += readSizeRounded;
                BIG_sp_CompressedNextBlock = BIG_gp_CompressedBuffer;
                goto LABEL_51;
            }
        }
        else
        {
            MEM_FreeFromEnd(BIG_gp_ReadBuffer);
            v19 = MEM_p_AllocFromEnd(fullSizeToRead);
            memcpy(v19, BIG_gp_ReadBuffer, BIG_gi_ReadSize);
            BIG_gp_ReadBuffer = (char*)v19;
            BIG_gi_ReadSize = fullSizeToRead;
        }
        BIG_gi_ReadSeek = 0;
        BIG_si_DecompressedSize = 0;
        goto LABEL_40;
    }

    BIG_gp_ReadBuffer = (char*)MEM_p_AllocFromEnd(BIG_gi_ReadSize);// ez a beginseedmode-ba 0x200000 lesz
    readbuff = BIG_gp_ReadBuffer;

    preloadTexFinished = 0;
    PreLoadTex_Finished(&preloadTexFinished);                     // ez semmit nem csinál csak átállítja 1-re a változót

    if (!preloadTexFinished)
        AI_EvalFunc_DbgTraceInt_C();
    if (gi_TestBin == -1                         // a testbin 0 ha pályát töltünk be
        || gi_TestBin > gi_CurBin
        || (v6 = gai_CurSize[gi_TestBin]) == 0
        || (v7 = gap_FirstPointer[gi_TestBin], (int)&v7[-v6] <= MEM_gp_MaxBlock)) // ??? TODO FIXME
    {
        BIG_fseek(Stream, BIG_gul_ReadPos, 0);
        fread(BIG_gp_ReadBuffer, 1, 2048, Stream);
        blockSize = *(uint32_t*)BIG_gp_ReadBuffer;
        BIG_si_BinFileSize = *(uint32_t*)BIG_gp_ReadBuffer;
        BIG_gp_ReadBuffer = readbuff;
    }
    else
    {
        blockSize = *((uint32_t*)v7 - 0x2800);
        v3 = gai_CurSize[gi_TestBin];
        BIG_si_BinFileSize = blockSize;
        if (v6 > blockSize)
            v3 = blockSize;
        v4 = 1;
    }
    if (BIGcomp_M_CompressedSize < blockSize)
        blockSize = BIGcomp_M_CompressedSize;
    BIG_si_BinFileReadSize = blockSize;
    BIG_si_CompressedSize = blockSize;
    BIG_gp_CompressedBuffer = (char*)MEM_p_AllocTmp(BIGcomp_M_CompressedSize + 6144);
    BIG_gp_CompressedBufferPtrBackup = BIG_gp_CompressedBuffer;
    if (!v4 || v3 < BIG_si_BinFileReadSize)
    {
        BIG_fseek(Stream, v3 + BIG_gul_ReadPos, 0);
        BIG_gp_CompressedBuffer += v3;
        fread(BIG_gp_CompressedBuffer, 1, BIG_si_CompressedSize - v3, Stream);
        BIG_gp_CompressedBuffer -= v3;
    }

    BIG_si_BinFileSize = (BIG_si_BinFileSize - BIG_si_BinFileReadSize) & ((BIG_si_BinFileSize - BIG_si_BinFileReadSize <= 0) - 1);
    if (v4)
    {
        testBin = gi_TestBin;
        for (currOffset = 0; currOffset < (int)gai_CurSize[gi_TestBin]; currOffset += 40960)
        {
            memcpy(&BIG_gp_CompressedBuffer[currOffset], &gap_FirstPointer[testBin][-currOffset - 40960], 40960u);
            testBin = gi_TestBin;
        }
        v12 = currOffset - 40960;
        v13 = 0;
        if (gi_CurBin > 0)
        {
            do
            {
                v14 = gap_FirstPointer[v13];
                if (v14 < gap_FirstPointer[gi_TestBin])
                {
                    memcpy(&v14[v12 - gai_CurSize[v13]], &v14[- (int) gai_CurSize[v13]], gai_CurSize[v13]);
                    gap_FirstPointer[v13] += v12;
                }
                ++v13;
            } while (v13 < gi_CurBin);
        }
    }
    BIG_si_BinFileReadSize = BIG_si_BinFileSize;
    if (BIG_si_BinFileSize >= BIGcomp_M_CompressedSize)
        BIG_si_BinFileReadSize = BIGcomp_M_CompressedSize;
    BIG_gi_ReadSeek = 0;
    v35.i4 = 0;
    v35.src = BIG_gp_CompressedBuffer + 4;
    v35.dest = BIG_gp_ReadBuffer;
    fullSizeToRead = sizeToRead;
    BIG_sp_CompressedNextBlock = BIGcomp_DecodeBlocks(&v35, 1);
    BIG_gi_RealSeek = BIG_sp_CompressedNextBlock - BIG_gp_CompressedBuffer;
    BIG_si_DecompressedSize = v35.totalSize;
    BIG_ul_CachePos = BIG_si_CompressedSize + BIG_gul_ReadPos - 2048;
LABEL_29:
    ddestBuf = DstBuf;
LABEL_30:
    if (ddestBuf)
        memcpy(ddestBuf, &BIG_gp_ReadBuffer[BIG_gi_ReadSeek], fullSizeToRead);
    BIG_gi_ReadSeek += fullSizeToRead;

    return 1;
}

int BIG_SpeedMode_fread(char** currOffset, int size, FILE* Stream)
{
    char* readBuffer; // eax
    signed int curSizeForBin; // esi
    int v5; // ebx
    char* rreadBuffer; // ebp
    size_t v7; // eax
    char* v8; // ecx
    int blockSize; // ecx
    int binFileSize; // edx
    int testBin; // eax
    signed int i; // ebx
    int v13; // ebx
    int curBin; // esi
    char* firstPtr; // eax
    int ssize; // edi
    int readSeek; // ecx
    int decompressedSize; // edx
    void* readBuffPtr; // esi
    char* v20; // eax
    int roundedSize; // ebx
    int v22; // ebp
    int sizeToRead; // eax
    char* v24; // eax
    char* v25; // esi
    char* v26; // edi
    void* v27; // esi
    BOOL loopTest; // cc
    char ccompressedSize; // al
    unsigned int v31; // ecx
    int v32; // [esp+10h] [ebp-18h] BYREF
    BIGCompressedBlockStruct v33; // [esp+14h] [ebp-14h] BYREF

    readBuffer = BIG_gp_ReadBuffer;
    curSizeForBin = 0;
    v5 = 0;
    if (BIG_gp_ReadBuffer)
    {
        readSeek = BIG_gi_ReadSeek;
        ssize = size;
        if (BIG_gi_ReadSeek + size <= BIG_si_DecompressedSize)
            goto LABEL_46;
        decompressedSize = BIG_si_DecompressedSize - BIG_gi_ReadSeek;
        BIG_si_DecompressedSize = decompressedSize;
        if (decompressedSize)
        {
            memcpy(BIG_gp_ReadBuffer, &BIG_gp_ReadBuffer[BIG_gi_ReadSeek], decompressedSize);
            readBuffer = BIG_gp_ReadBuffer;
        }
        BIG_gi_ReadSeek = 0;
        if (size > BIG_gi_ReadSize)
        {
            MEM_FreeFromEnd(readBuffer);
            BIG_gi_ReadSize = size;
            readBuffPtr = MEM_p_AllocFromEnd(size);
            memcpy(readBuffPtr, BIG_gp_ReadBuffer, BIG_si_DecompressedSize);
            BIG_gp_ReadBuffer = (char*)readBuffPtr;
            BIG_gi_ReadSeek = 0;
        }
        v20 = BIG_sp_CompressedNextBlock;
        while (BIGcomp_CheckBlock(v20, BIG_si_CompressedSize + BIG_gp_CompressedBuffer - v20))
        {
        LABEL_42:
            if (!BIGcomp_CheckBuffer(BIG_sp_CompressedNextBlock, BIG_gi_ReadSize - BIG_si_DecompressedSize))
            {
                MEM_FreeFromEnd(BIG_gp_ReadBuffer);
                BIG_gi_ReadSize = BIG_si_DecompressedSize + BIGcomp_i_CompressedSize_v2(BIG_sp_CompressedNextBlock);
                v27 = MEM_p_AllocFromEnd(BIG_gi_ReadSize);
                memcpy(v27, BIG_gp_ReadBuffer, BIG_si_DecompressedSize);
                BIG_gp_ReadBuffer = (char*)v27;
            }
            v33.src = BIG_sp_CompressedNextBlock;
            v33.i4 = 0;
            v33.dest = &BIG_gp_ReadBuffer[BIG_si_DecompressedSize];
            v20 = BIGcomp_DecodeBlocks(&v33, 1u);
            loopTest = v33.totalSize + BIG_si_DecompressedSize < ssize;
            BIG_sp_CompressedNextBlock = v20;
            BIG_gi_RealSeek += v20 - v33.src;
            BIG_si_DecompressedSize += v33.totalSize;
            if (!loopTest)
                goto LABEL_45;
        }
        roundedSize = ((BIG_gp_CompressedBuffer - BIG_sp_CompressedNextBlock + BIG_si_CompressedSize) / 2048) << 11;
        v22 = BIG_si_CompressedSize + BIG_gp_CompressedBuffer - BIG_sp_CompressedNextBlock - roundedSize;
        BIGcomp_i_CompressedSize(BIG_sp_CompressedNextBlock);
        memcpy(BIG_gp_CompressedBufferPtrBackup, BIG_sp_CompressedNextBlock, roundedSize);
        BIG_gp_CompressedBuffer = &BIG_gp_CompressedBufferPtrBackup[roundedSize];
        BIG_si_BinFileSize = ((BIG_si_BinFileSize + v22 + 2047) / 2048) << 11;
        sizeToRead = BIG_si_BinFileSize;
        if (BIG_si_BinFileSize >= BIGcomp_M_CompressedSize - roundedSize)
            sizeToRead = BIGcomp_M_CompressedSize - roundedSize;
        BIG_si_BinFileReadSize = sizeToRead;
        BIG_si_CompressedSize = sizeToRead;
        BIG_fseekAndRead(
            Stream,
            roundedSize + BIG_gul_ReadPos + BIG_gi_RealSeek,
            (void**)&BIG_gp_CompressedBuffer,
            sizeToRead);
        BIG_si_BinFileSize = BIG_si_BinFileSize - BIG_si_BinFileReadSize <= 0 ? 0 : BIG_si_BinFileSize
            - BIG_si_BinFileReadSize;
        BIG_si_BinFileReadSize = BIG_si_BinFileSize;
        if (BIG_si_BinFileSize >= BIGcomp_M_CompressedSize)
            BIG_si_BinFileReadSize = BIGcomp_M_CompressedSize;
        v24 = BIG_gp_CompressedBufferPtrBackup;
        v25 = BIG_gp_CompressedBuffer;
        v26 = &BIG_gp_CompressedBufferPtrBackup[roundedSize];
        if (BIG_gp_CompressedBuffer != &BIG_gp_CompressedBufferPtrBackup[roundedSize])
        {
            if (BIG_si_CompressedSize <= roundedSize)
            {
                ccompressedSize = BIG_si_CompressedSize;
                v31 = (unsigned int)BIG_si_CompressedSize >> 2;
                memcpy(v26, BIG_gp_CompressedBuffer, 4 * ((unsigned int)BIG_si_CompressedSize >> 2));
                memcpy(&v26[4 * v31], &v25[4 * v31], ccompressedSize & 3);
                BIG_gp_CompressedBuffer = BIG_gp_CompressedBufferPtrBackup;
                goto LABEL_41;
            }
            memcpy(&BIG_gp_CompressedBuffer[-roundedSize], BIG_gp_CompressedBufferPtrBackup, roundedSize);
            v24 = &BIG_gp_CompressedBuffer[-roundedSize];
        }
        BIG_gp_CompressedBuffer = v24;
    LABEL_41:
        ssize = size;
        BIG_si_CompressedSize += roundedSize;
        BIG_sp_CompressedNextBlock = BIG_gp_CompressedBuffer;
        goto LABEL_42;
    }
    BIG_gp_ReadBuffer = (char*)MEM_p_AllocFromEnd(BIG_gi_ReadSize);
    rreadBuffer = BIG_gp_ReadBuffer;
    v32 = 0;
    PreLoadTex_Finished(&v32);
    if (!v32)
        AI_EvalFunc_DbgTraceInt_C();
    if (gi_TestBin == -1
        || gi_TestBin > gi_CurBin
        || (v7 = gai_CurSize[gi_TestBin]) == 0
        || (v8 = gap_FirstPointer[gi_TestBin], (int)&v8[-(int)v7] <= MEM_gp_MaxBlock)) // ??? TODO
    {
        BIG_fseek(Stream, BIG_gul_ReadPos, 0);
        fread(BIG_gp_ReadBuffer, 1, 2048, Stream);
        blockSize = *(uint32_t*)BIG_gp_ReadBuffer;
        BIG_gp_ReadBuffer = rreadBuffer;
    }
    else
    {
        blockSize = *((uint32_t*)v8 - 0x2800);
        curSizeForBin = gai_CurSize[gi_TestBin];
        v5 = 1;
    }
    BIG_si_BinFileSize = blockSize;
    if (BIGcomp_M_CompressedSize < blockSize)
        blockSize = BIGcomp_M_CompressedSize;
    BIG_si_BinFileReadSize = blockSize;
    BIG_si_CompressedSize = blockSize;
    BIG_gp_CompressedBuffer = (char*)MEM_p_AllocTmp(BIGcomp_M_CompressedSize + 6144);
    BIG_gp_CompressedBufferPtrBackup = BIG_gp_CompressedBuffer;
    if (!v5 || curSizeForBin < BIG_si_BinFileReadSize)
    {
        BIG_fseek(Stream, curSizeForBin + BIG_gul_ReadPos, 0);
        BIG_gp_CompressedBuffer += curSizeForBin;
        fread(BIG_gp_CompressedBuffer, 1, BIG_si_CompressedSize - curSizeForBin, Stream);
        BIG_gp_CompressedBuffer -= curSizeForBin;
    }
    binFileSize = (BIG_si_BinFileSize - BIG_si_BinFileReadSize) & ((BIG_si_BinFileSize - BIG_si_BinFileReadSize <= 0) - 1);
    BIG_si_BinFileSize = binFileSize;
    if (v5)
    {
        testBin = gi_TestBin;
        for (i = 0; i < (int)gai_CurSize[gi_TestBin]; i += 0xA000)
        {
            memcpy(&BIG_gp_CompressedBuffer[i], &gap_FirstPointer[testBin][-i - 0xA000], 0xA000);
            testBin = gi_TestBin;
        }
        v13 = i - 0xA000;
        curBin = 0;
        if (gi_CurBin > 0)
        {
            do
            {
                firstPtr = gap_FirstPointer[curBin];
                if (firstPtr < gap_FirstPointer[gi_TestBin])
                {
                    memcpy(&firstPtr[v13 - gai_CurSize[curBin]], &firstPtr[-(int)gai_CurSize[curBin]], gai_CurSize[curBin]);
                    gap_FirstPointer[curBin] += v13;
                }
                ++curBin;
            } while (curBin < gi_CurBin);
            binFileSize = BIG_si_BinFileSize;
        }
    }
    BIG_si_BinFileReadSize = binFileSize;
    if (binFileSize >= BIGcomp_M_CompressedSize)
        BIG_si_BinFileReadSize = BIGcomp_M_CompressedSize;
    BIG_gi_ReadSeek = 0;
    v33.i4 = 0;
    v33.src = BIG_gp_CompressedBuffer + 4;
    v33.dest = BIG_gp_ReadBuffer;
    ssize = size;
    BIG_sp_CompressedNextBlock = BIGcomp_DecodeBlocks(&v33, 1);
    BIG_gi_RealSeek = BIG_sp_CompressedNextBlock - BIG_gp_CompressedBuffer;
    BIG_si_DecompressedSize = v33.totalSize;
    BIG_ul_CachePos = BIG_si_CompressedSize + BIG_gul_ReadPos - 2048;
LABEL_45:
    readBuffer = BIG_gp_ReadBuffer;
    readSeek = BIG_gi_ReadSeek;
LABEL_46:
    *currOffset = &readBuffer[readSeek];
    BIG_gi_ReadSeek += ssize;

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

        if (fatElem->fatNum)
        {
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
}

void BIG_ReadAllFats()
{
    int i;
    char* currPtr = NULL;
    
    BIG_gst.bfFatTable = (BIGFatTableElem*)MEM_p_Alloc(sizeof(BIGFatTableElem) * BIG_gst.hdr.fatTableNum);
    
    if (!BIG_gst.bfFatTable)
        DBG_assert(0, __FILE__, __LINE__, "", "Not enough memory", 0, 1);

    if (BIG_gst.hdr.fatTableNum)
    {
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
    
    if (BIG_gst.hdr.fatTableNum)
    {
        for (i = 0; i < BIG_gst.hdr.fatTableNum; i++)
        {
            BIG_ReadFatFile(&BIG_gst.bfFatTable[i]);
        }
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
    char* ret;
    char* buff;
    uint32_t len;

    if (BIG_gi_ReadMode == 2)
    {
        ret = LOA_FetchFile(size);
        if (ret)
            return ret;

        return NULL;
    }
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
void PreLoadMoveBlocks(int a1)
{
    char* srcPtr, *dstPtr;
    size_t size;
    int i;

    if (gi_LastES != -1 && a1 == gau_ES[gi_LastES])
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