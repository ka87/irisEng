#include "common.h"
#include "textures.h"
#include "memory.h"
#include "ai/ai_debugfunctions.h"
#include "soft.h"

uint32_t MEM_gpv_LastFree = 0; // engine doesn't explcitly set it zero, but it could have caused problems later (eg AllocFromEnd)

void MEM_InitDynamic(SIZE_T dwBytes)
{
    int size;
    int i, j;
    memoryInfoElem*  currSizeBlockStart;
    memoryInfoElem*  currStart;

    size = 0;
    MEM_gst_MemoryInfo.memoryStartPointer = (int) HeapAlloc(GetProcessHeap(), 0, dwBytes);
    i = 0;
    currSizeBlockStart = MEM_gst_MemoryInfo.memInfoElems;
    do
    {
        currStart = currSizeBlockStart;
        j = 128;
        currSizeBlockStart += 128;
        do
        {
            size += 1 << (i + 2);
            currStart->holeChainElemCount = 0;
            currStart->size = size;
            currStart->firstHole = 0;
            currStart++;
            --j;
        } while (j);
        ++i;
    } while (i < 18);
    MEM_gst_MemoryInfo.usedMemoryByEngine = 0;
    MEM_gst_MemoryInfo.usedMemoryByTextures = 0;
    MEM_gst_MemoryInfo.totalUsedMemory = 0;
    MEM_gst_MemoryInfo.nextFreeMemoryPointer = MEM_gst_MemoryInfo.memoryStartPointer;
    MEM_gst_MemoryInfo.memorySize = dwBytes;
}

void MEM_InitModule()
{
    memMngEvent = 0;
    memMngEvent = CreateEventA(NULL, FALSE, FALSE, NULL);
    SetEvent(memMngEvent);
    MEM_InitDynamic(31457280u);
}

BOOL MEM_CloseModule()
{
    return HeapFree(GetProcessHeap(), 0, (void *)MEM_gst_MemoryInfo.memoryStartPointer);
}

BOOL MEM_b_CheckBlockHasBeenFreed(void* a1)
{
    int marker;
    int markerType;
    unsigned int size;
    BOOL ret;
    uint32_t* tmp = (uint32_t *)a1;

    marker = tmp[-2];
    ret = 1;
    if ((marker & 0xF0000000) == 0xA0000000)
    {
        markerType = marker & 0xFFFFFF;
        if (markerType == 0x110270 || markerType == 0xC0FFEE)
        {
            size = tmp[-1];
            if (size)
            {
                if (size < 0x7FFFE00)
                    ret = 0;
            }
        }
    }
    return ret;
}

unsigned int MEM_GetElemIDBySize(unsigned int size)
{
    if (size <= 512)
        return (size - 1) >> 2;
    if (size <= 1536)
        return ((size - 513) >> 3) + 128;
    if (size <= 3584)
        return ((size - 1537) >> 4) + 256;
    if (size <= 7680)
        return ((size - 3585) >> 5) + 384;
    if (size <= 15872)
        return ((size - 7681) >> 6) + 512;
    if (size <= 32256)
        return ((size - 15873) >> 7) + 640;
    if (size <= 65024)
        return ((size - 32257) >> 8) + 768;
    if (size <= 130560)
        return ((size - 65025) >> 9) + 896;
    if (size <= 261632)
        return ((size - 130561) >> 10) + 1024;
    if (size <= 523776)
        return ((size - 261633) >> 11) + 1152;
    if (size <= 1048064)
        return ((size - 523777) >> 12) + 1280;
    if (size <= 2096640)
        return ((size - 1048065) >> 13) + 1408;
    if (size <= 4193792)
        return ((size - 2096641) >> 14) + 1536;
    if (size <= 8388096)
        return ((size - 4193793) >> 15) + 1664;
    if (size <= 16776704)
        return ((size - 8388097) >> 16) + 1792;
    if (size <= 33553920)
        return ((size - 16776705) >> 17) + 1920;
    if (size <= 67108352)
        return ((size - 33553921) >> 18) + 2048;
    if (size > 134217216)
        return 0;
    return ((size - 67108353) >> 19) + 2176;
}

int MEM_GetBlockSizeBySize(void* a1)
{
    uint32_t* tmp = (uint32_t*)a1;
    return MEM_gst_MemoryInfo.memInfoElems[MEM_GetElemIDBySize(tmp[-1])].size;
}

void MEM_GetHolesStat(int* holesNum, int* sizeOfHoles)
{
    memoryInfoElem* currMemInfoElem;
    int* firstHole;
    int* nextHole;

    if (sizeOfHoles)
        *sizeOfHoles = 0;
    if (holesNum)
        *holesNum = 0;
    currMemInfoElem = MEM_gst_MemoryInfo.memInfoElems;
    do
    {
        firstHole = currMemInfoElem->firstHole;
        if (firstHole)
        {
            nextHole = (int*)firstHole[0]; // next hole
            if (sizeOfHoles)
                *sizeOfHoles += currMemInfoElem->size + 8;
            if (holesNum)
                ++*holesNum;
            while (nextHole)
            {
                if (nextHole == (int*)0xffffffff)
                    break;
                nextHole = (int*)nextHole[0]; // next hole
                if (sizeOfHoles)
                    *sizeOfHoles += currMemInfoElem->size + 8;
                if (holesNum)
                    ++*holesNum;
            }
        }
        ++currMemInfoElem;
    } while (currMemInfoElem < &MEM_gst_MemoryInfo.memInfoElems[2304]);
}

/*
!!!
Ha egy blokkból hole lesz, akkor az eddig sizeként használt mezõbõl a következõ hole-ra mutató ptr lesz
*/

void* MEM_p_Alloc(unsigned int size)
{
    unsigned int elemID;
    int blockSize;
    memoryInfoElem* memInfoElem;
    int* holeDesc;
    int* secondHole;
    char* destinationAddress;
    int holesNum;
    unsigned int sizeOfHoles;
    char Buffer[1000];

    if (size < 8)
        size = 8;
    WaitForSingleObject(memMngEvent, 0xFFFFFFFF);
    elemID = MEM_GetElemIDBySize(size);
    blockSize = MEM_gst_MemoryInfo.memInfoElems[elemID].size;
    memInfoElem = &MEM_gst_MemoryInfo.memInfoElems[elemID];
    MEM_gst_MemoryInfo.totalUsedMemory += blockSize - size;
    ++MEM_gst_MemoryInfo.memInfoElems[elemID].id;
    if (!MEM_gst_MemoryInfo.memInfoElems[elemID].firstHole)
    {
        if (&(MEM_gst_MemoryInfo.nextFreeMemoryPointer) - &(MEM_gst_MemoryInfo.memoryStartPointer) + blockSize + 8 < MEM_gst_MemoryInfo.memorySize)
        {
            destinationAddress = (char*)MEM_gst_MemoryInfo.nextFreeMemoryPointer + 8;
            MEM_gst_MemoryInfo.nextFreeMemoryPointer = MEM_gst_MemoryInfo.nextFreeMemoryPointer + blockSize + 8;
            MEM_gp_MaxBlock = MEM_gst_MemoryInfo.nextFreeMemoryPointer;
            
            if (MEM_gst_MemoryInfo.nextFreeMemoryPointer >= gp_LastPointer - 40960)
            {
                 AI_EvalFunc_DbgTraceInt_C();
                 PreLoadTexReset(); 
            }

            uint32_t* tmpDestinationAddress = (uint32_t*)destinationAddress;
            tmpDestinationAddress[-1] = size;
            MEM_gst_MemoryInfo.usedMemoryByEngine += memInfoElem->size + 8;
            tmpDestinationAddress[-2] = 0xA0110270;
            SetEvent(memMngEvent);
            return destinationAddress;
        }
        if (++memInfoElem >= (memoryInfoElem*)&MEM_gst_MemoryInfo.allocatedMemoryEnd)
        {
            goto err;
        }
        while (!memInfoElem->firstHole)
        {
            if (++memInfoElem >= &MEM_gst_MemoryInfo.memInfoElems[2304]) {
                goto err;
            }
        }
        size += memInfoElem->size - blockSize;
    }

    holeDesc = memInfoElem->firstHole;
    secondHole = (int*)holeDesc[0]; // ptr to the next hole (was the size field originally)
    destinationAddress = (char*)&holeDesc[1]; // ptr to the memory block
    memInfoElem->firstHole = secondHole;


    uint32_t* tmpDestinationAddress = (uint32_t*)destinationAddress;
    tmpDestinationAddress[-1] = size;
    MEM_gst_MemoryInfo.usedMemoryByEngine += memInfoElem->size + 8;
    tmpDestinationAddress[-2] = 0xA0110270;    
    SetEvent(memMngEvent);
    return destinationAddress;

err:
    SetEvent(memMngEvent);
    MEM_GetHolesStat(&holesNum, &sizeOfHoles);
    sprintf(Buffer,
        "[Engine %u ko -- Textures %u ko -- WH %u ko -- Gran %u ko -- Holes %u -- Size Holes %u ko]",
        MEM_gst_MemoryInfo.usedMemoryByEngine >> 10,
        MEM_gst_MemoryInfo.usedMemoryByTextures >> 10,
        (MEM_gst_MemoryInfo.nextFreeMemoryPointer - MEM_gst_MemoryInfo.memoryStartPointer) / 1024,
        MEM_gst_MemoryInfo.totalUsedMemory >> 10,
        holesNum,
        sizeOfHoles >> 10);
    MessageBoxA(0, "No more memory", "No more memory", 0);
    return 0;
}

void* MEM_p_AllocAligned(int a1, int alignment)
{
    void* ptr;
    void* ret;

    ptr = MEM_p_Alloc(2 * alignment + a1);
    ret = (void*)(2 * alignment + ((unsigned int)ptr & ~(alignment - 1)));
    uint32_t* tmp = (uint32_t*)ptr;
    uint32_t* tmpRet = (uint32_t*)ret;
    tmp[-2] = 0xA0C0FFEE;
    tmpRet[-1] = (uint32_t)ptr;
    return ret;
}

void MEM_ResetTmpMemory()
{
    WaitForSingleObject(memMngEvent, 0xFFFFFFFF);
    MEM_gp_AllocTmpNext = MEM_gp_AllocTmpFirst;
    MEM_gi_AllocTmpNb = 0;
    SetEvent(memMngEvent);
    SOFT_ZList_Init();
    SOFT_ZList_Clear();
}

void MEM_FreeTmp(void* a1)
{
    int size;

    uint32_t* tmp = (uint32_t*)a1;

    size = tmp[-1];
    WaitForSingleObject(memMngEvent, 0xFFFFFFFF);
    if ((char*)a1 + size - 4 == (char*)MEM_gp_AllocTmpNext) // if this is the last elem, we have to adjust MEM_gp_AllocTmpNext
        MEM_gp_AllocTmpNext = (char*)a1 - 4;
    --MEM_gi_AllocTmpNb;
    SetEvent(memMngEvent);
    if (!MEM_gi_AllocTmpNb)
        MEM_ResetTmpMemory();
}

void MEM_Free(void* ptr)
{
    void* realBlockStartPtr;
    uint32_t alignedPtrDesc, *alignedPtrDescPtr;
    int* holeElem;
    unsigned int elemID;
    int realBlockSize;
    unsigned int blockElemSize;

    realBlockStartPtr = ptr;
    if (ptr)
    {
        if (ptr < MEM_gp_AllocTmpFirst || ptr >= MEM_gp_AllocTmpLast)
        {
            uint32_t* tmpPtr = (uint32_t*)ptr;
            alignedPtrDesc = tmpPtr[-1];
            alignedPtrDescPtr = (int*)alignedPtrDesc;
            if (alignedPtrDesc >= MEM_gst_MemoryInfo.memoryStartPointer && alignedPtrDesc < MEM_gst_MemoryInfo.nextFreeMemoryPointer
                && (alignedPtrDesc & 3) == 0 && alignedPtrDescPtr[-2] == 0xA0C0FFEE)  // aligned malloc
            {
                alignedPtrDescPtr[-2] = 0xA0110270;
                realBlockStartPtr = (void *)alignedPtrDescPtr;
            }
            WaitForSingleObject(memMngEvent, 0xFFFFFFFF);
            tmpPtr = (uint32_t*)realBlockStartPtr;
            holeElem = (int*)&tmpPtr[-1];
            elemID = MEM_GetElemIDBySize(tmpPtr[-1]);
            realBlockSize = MEM_gst_MemoryInfo.memInfoElems[elemID].size;
            blockElemSize = realBlockSize + 8;
            MEM_gst_MemoryInfo.totalUsedMemory += tmpPtr[-1] - realBlockSize;
            if (MEM_gst_MemoryInfo.nextFreeMemoryPointer == (unsigned int)realBlockStartPtr + realBlockSize)// ha az utolsó foglalt blokkot szabadítjuk fel
            {
                MEM_gst_MemoryInfo.nextFreeMemoryPointer = MEM_gst_MemoryInfo.nextFreeMemoryPointer - blockElemSize;
                tmpPtr[-2] = 0;
            }
            else
            {
                *holeElem = (int)MEM_gst_MemoryInfo.memInfoElems[elemID].firstHole;
                tmpPtr[0] = blockElemSize;
                holeElem[(blockElemSize >> 2) - 2] = blockElemSize;
                MEM_gst_MemoryInfo.memInfoElems[elemID].firstHole = holeElem;
                tmpPtr[-2] = 0xB0110270;
            }
            MEM_gst_MemoryInfo.usedMemoryByEngine -= blockElemSize;
            SetEvent(memMngEvent);
        }
        else
        {
            MEM_FreeTmp(ptr);
        }
    }
}

void MEM_FreeAlign(void* ptr)
{
    MEM_Free(ptr);
}

void* MEM_p_AllocTmp(int size)
{
    unsigned int sizeToAlloc;
    char* ptr;
    void* ret;

    sizeToAlloc = (size + 128) & 0xFFFFFFC0;
    if (!MEM_gp_AllocTmpFirst || !MEM_gb_EnableTmp || &MEM_gp_AllocTmpNext[sizeToAlloc] >= (char *)MEM_gp_AllocTmpLast)
        return (void*)MEM_p_AllocAligned((size + 128) & 0xFFFFFFC0, 64);

    WaitForSingleObject(memMngEvent, 0xFFFFFFFF);
    ptr = MEM_gp_AllocTmpNext;
    *(uint32_t*)MEM_gp_AllocTmpNext = sizeToAlloc;
    MEM_gp_AllocTmpNext += sizeToAlloc;
    ret = ptr + 4;
    ++MEM_gi_AllocTmpNb;

    SetEvent(memMngEvent);
    return ret;
}

BOOL MEM_Defrag(int onlyOneHole)
{
    int v1;
    int i;
    memoryInfoElem* currMie;
    uint32_t nextFreeMemoryPointer;
    uint32_t v5;
    uint32_t memStartPtr;
    int currBlockSize;
    uint32_t v8;
    uint32_t v9;
    unsigned int v10;
    memoryInfoElem* currMemoryInfoElem;
    uint32_t currHole;
    int* v13;

    v1 = 0;
    WaitForSingleObject(memMngEvent, 0xFFFFFFFF);
    i = i_Last_245;
    if (i_Last_245 < 2304)
    {
        currMie = &MEM_gst_MemoryInfo.memInfoElems[i_Last_245];
        do
        {
            if (currMie->firstHole && onlyOneHole)
                break;
            ++currMie;
            ++i;
        } while (currMie < &MEM_gst_MemoryInfo.memInfoElems[2304]);
    }
    nextFreeMemoryPointer = MEM_gst_MemoryInfo.nextFreeMemoryPointer;// ez az egész nem csinál mást, mint a memória végérõl az üres láncokat leszedni, mivel a sok felszabadítás következtében a memória végén hole-ok marad(hat)nak beláncolva és köztük adat nélkül. Ez eltakarítja ezt.
    v5 = MEM_gst_MemoryInfo.nextFreeMemoryPointer;

    i_Last_245 = i >= 2304 ? 0 : i;
    for (memStartPtr = MEM_gst_MemoryInfo.memoryStartPointer; nextFreeMemoryPointer != MEM_gst_MemoryInfo.memoryStartPointer; v5 = nextFreeMemoryPointer)
    {
        uint32_t* v5Tmp = (uint32_t*)v5;
        currBlockSize = v5Tmp[-1];
        if (currBlockSize <= 0)
            break;
        if ((currBlockSize & 3) != 0)
            break;
        if (currBlockSize < 8)
            break;
        v8 = (uint32_t)&v5Tmp[-(currBlockSize >> 2)];
        if (v8 <= memStartPtr)
            break;
        if (v8 > nextFreeMemoryPointer)
            break;
        uint32_t* v8Tmp = (uint32_t*)v8;
        if (v8Tmp[0] != 0xB0110270)          // ez nem egy hole (0xB0110270 = hole)
            break;
        v9 = v8Tmp[1];
        if (v9)
        {
            if (v9 < memStartPtr)
                break;
        }
        if (v9 > (nextFreeMemoryPointer + MEM_gst_MemoryInfo.memorySize))
            break;
        if (v8Tmp[2] != currBlockSize)
            break;
        v10 = MEM_GetElemIDBySize(currBlockSize - 8);
        ++MEM_gst_MemoryInfo.memInfoElems[v10].holeChainElemCount;
        nextFreeMemoryPointer = MEM_gst_MemoryInfo.nextFreeMemoryPointer - currBlockSize;
        MEM_gst_MemoryInfo.nextFreeMemoryPointer = MEM_gst_MemoryInfo.nextFreeMemoryPointer - currBlockSize;
        v1 = currBlockSize;
        if (onlyOneHole)
            break;
        memStartPtr = MEM_gst_MemoryInfo.memoryStartPointer;
    }
    currMemoryInfoElem = MEM_gst_MemoryInfo.memInfoElems;
    do
    {
        if (currMemoryInfoElem->holeChainElemCount)
        {
            currHole = (uint32_t)currMemoryInfoElem->firstHole;
            if (currHole)
            {
                v13 = 0;
                while (1)
                {
                    for (; currHole - 4 < nextFreeMemoryPointer; currHole = *(uint32_t*)currHole)
                        v13 = (int*)currHole;
                    *(uint32_t*)(currHole - 4) = 0;
                    --currMemoryInfoElem->holeChainElemCount;
                    currHole = *(uint32_t*)currHole;
                    if (v13)
                        *v13 = currHole;
                    else
                        currMemoryInfoElem->firstHole = (uint32_t*)currHole;
                    if (!currMemoryInfoElem->holeChainElemCount)
                        break;
                    nextFreeMemoryPointer = MEM_gst_MemoryInfo.nextFreeMemoryPointer;
                }
            }
            else
            {
                if (onlyOneHole)
                    MEM_gst_MemoryInfo.nextFreeMemoryPointer = nextFreeMemoryPointer + v1;
                currMemoryInfoElem->holeChainElemCount = 0;
            }
            nextFreeMemoryPointer = MEM_gst_MemoryInfo.nextFreeMemoryPointer;
        }
        ++currMemoryInfoElem;
    } while ((int)currMemoryInfoElem < (int)&MEM_gst_MemoryInfo.allocatedMemoryEnd);

    return SetEvent(memMngEvent);
}

void* MEM_MoveToTempMem(void* ptr, size_t Size)
{
    void *newPtr = MEM_p_AllocTmp(Size);
    MEM_Free(ptr);
    memcpy(newPtr, ptr, Size);
    return newPtr;
}

void* MEM_GEO_p_Alloc(unsigned int size)
{
    return MEM_p_Alloc(size);
}

void* MEM_GEO_p_AllocAlign(int size, int alignment)
{
    return MEM_p_AllocAligned(size, alignment);
}

void* MEM_p_ReallocAligned(void* ptr, unsigned int size, int alignment)
{
    void* newPtr;

    newPtr = MEM_p_AllocAligned(size, alignment);
    memcpy(newPtr, ptr, size);
    MEM_FreeAlign(ptr);
    return newPtr;
}

void* MEM_p_Realloc(void* Src, size_t Size)
{
    unsigned int tmpPtr;
    unsigned int v4;
    void* newBlock; 
    int origBlockSize;
    int newBlockSize;

    uint32_t* tmpSrc = (uint32_t*)Src;

    if (Src >= MEM_gp_AllocTmpFirst && Src < MEM_gp_AllocTmpLast)
        return MEM_MoveToTempMem(Src, Size);
    tmpPtr = tmpSrc[-1];
    uint32_t* v3Tmp = (uint32_t*)tmpPtr;
    if (tmpPtr >= MEM_gst_MemoryInfo.memoryStartPointer && tmpPtr < MEM_gst_MemoryInfo.nextFreeMemoryPointer
        && (tmpPtr & 3) == 0 && v3Tmp[-2] == 0xA0C0FFEE)      // aligned alloc
    {
        return MEM_p_ReallocAligned(Src, Size, 64);
    }
    v4 = MEM_GetElemIDBySize(tmpPtr);
    if (MEM_gst_MemoryInfo.memInfoElems[v4].size == MEM_gst_MemoryInfo.memInfoElems[MEM_GetElemIDBySize(Size)].size)
        return Src;
    newBlock = MEM_p_Alloc(Size);
    uint32_t* newBlockTmp = (uint32_t*)newBlock;
    origBlockSize = MEM_gst_MemoryInfo.memInfoElems[MEM_GetElemIDBySize(tmpSrc[-1])].size;
    newBlockSize = MEM_gst_MemoryInfo.memInfoElems[MEM_GetElemIDBySize(newBlockTmp[-1])].size;
    if (newBlockSize > origBlockSize)
        newBlockSize = origBlockSize;
    memcpy(newBlock, Src, newBlockSize);
    MEM_Free(Src);
    return newBlock;
}

void* MEM_p_AllocFromEnd(int a1)
{
    int lastFree;
    unsigned int alignedSize;
    void* newPtr;

    WaitForSingleObject(memMngEvent, 0xFFFFFFFF);
    lastFree = MEM_gpv_LastFree;
    if (!MEM_gpv_LastFree)
    {
        lastFree = MEM_gst_MemoryInfo.memorySize + MEM_gst_MemoryInfo.memoryStartPointer;
        MEM_gpv_LastFree = MEM_gst_MemoryInfo.memorySize + MEM_gst_MemoryInfo.memoryStartPointer;
        MEM_gul_RealSize = MEM_gst_MemoryInfo.memorySize;
        MEM_gul_FromEndMaxAllocSize = 0;
    }
    alignedSize = (a1 & 0xFFFFFFFC) + 4;
    if (lastFree - MEM_gst_MemoryInfo.nextFreeMemoryPointer - 8 <= alignedSize)
    {
        newPtr = MEM_p_Alloc((a1 & 0xFFFFFFFC) + 4);
    }
    else
    {
        newPtr = (void*)(lastFree - alignedSize);
        MEM_gpv_LastFree = lastFree - alignedSize - 8;
        *(uint32_t*)MEM_gpv_LastFree = 0xCAFEDECA;
        *(uint32_t*)(lastFree - alignedSize - 4) = alignedSize;
        MEM_gst_MemoryInfo.memorySize = MEM_gpv_LastFree - MEM_gst_MemoryInfo.memoryStartPointer;
        if (MEM_gul_FromEndMaxAllocSize <= MEM_gul_RealSize - (MEM_gpv_LastFree - MEM_gst_MemoryInfo.memoryStartPointer))
        {
            MEM_gul_FromEndMaxAllocSize = MEM_gul_RealSize - (MEM_gpv_LastFree - MEM_gst_MemoryInfo.memoryStartPointer);
            SetEvent(memMngEvent);
            return newPtr;
        }
    }
    SetEvent(memMngEvent);
    return newPtr;
}

void MEM_FreeFromEnd(void* ptr)
{
    int v1;
    int v2;
    uint32_t v3;
    int v4;
    unsigned int v5;
    BOOL v6;
    int v7;
    uint32_t* ptrTmp = (uint32_t*)ptr;
    uint32_t* v3Tmp;

    if (ptrTmp[-2] == 0xCAFEDECA)
    {
        WaitForSingleObject(memMngEvent, 0xFFFFFFFF);
        if (((uint32_t)ptr - 8) == MEM_gpv_LastFree)
        {
            v1 = ptrTmp[-1];
            ptrTmp[-2] = 0;
            v2 = MEM_gpv_LastFree + v1 + 8;
            v3 = (uint32_t)ptr + v1 + 8;
            v3Tmp = (uint32_t*)v3;
            v4 = v2 - MEM_gst_MemoryInfo.memoryStartPointer;
            v5 = MEM_gul_RealSize;
            v6 = v2 - MEM_gst_MemoryInfo.memoryStartPointer == MEM_gul_RealSize;
            MEM_gpv_LastFree = v2;
            MEM_gst_MemoryInfo.memorySize = v2 - MEM_gst_MemoryInfo.memoryStartPointer;
            if (v2 - MEM_gst_MemoryInfo.memoryStartPointer < MEM_gul_RealSize)
            {
                do
                {
                    if (v3 - 8 != v2)
                        break;
                    if (v3Tmp[-2] != 0x5A5A5AA5)
                        break;
                    v7 = v3Tmp[-1];
                    v3Tmp[-2] = 0;
                    v2 = MEM_gpv_LastFree + v7 + 8;
                    v3 += v7 + 8;
                    v4 = v2 - MEM_gst_MemoryInfo.memoryStartPointer;
                    v5 = MEM_gul_RealSize;
                    MEM_gpv_LastFree = v2;
                    MEM_gst_MemoryInfo.memorySize = v2 - MEM_gst_MemoryInfo.memoryStartPointer;
                } while (v2 - MEM_gst_MemoryInfo.memoryStartPointer < MEM_gul_RealSize);
                v6 = v4 == v5;
            }
            if (v6)
            {
                MEM_gpv_LastFree = 0;
                SetEvent(memMngEvent);
                return;
            }
        }
        else
        {
            ptrTmp[-2] = 0x5A5A5AA5;
        }
        SetEvent(memMngEvent);
    }
    else
    {
        MEM_Free(ptr);
    }
}

int MEM_MemBlockSizeGetAlignedWithTempMemoryAddressSpaceCheck(void *v1) {
    if (v1 < MEM_gp_AllocTmpFirst || v1 >= MEM_gp_AllocTmpLast)
        return MEM_gst_MemoryInfo.memInfoElems[MEM_GetElemIDBySize(*(uint32_t*)(*((uint32_t*)v1 - 1) - 4))].size;
    else
        return *((uint32_t*)v1 - 1);
}

int MEM_MemBlockSizeGetAligned(void* v1) {
    return MEM_gst_MemoryInfo.memInfoElems[MEM_GetElemIDBySize(*(uint32_t*)(*((uint32_t*)v1 - 1) - 4))].size;
}

int MEM_MemBlockSizeGetTmpAddressSpace(void* v1) {
    return *((uint32_t*)v1 - 1);
}