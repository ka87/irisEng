#include "common.h"
#include "textures.h"
#include "memory.h"
#include "ai/ai_debugfunctions.h"
#include "soft.h"
#include "platform.h"

#include <SDL3/SDL_mutex.h>

/* The memory debugging build (cmake -DIRIS_MEM_DEBUG=ON): the pool
   below is renamed out of the way and memdebug.inc, included at the end
   of this file, takes its names -- every block its own malloc, so that
   AddressSanitizer sees each one. Without IRIS_MEM_DEBUG none of this
   exists and the pool is the allocator. */
#ifdef IRIS_MEM_DEBUG
#define MEM_p_Alloc MEMPOOL_MEM_p_Alloc
#define MEM_p_AllocAligned MEMPOOL_MEM_p_AllocAligned
#define MEM_Free MEMPOOL_MEM_Free
#define MEM_FreeAlign MEMPOOL_MEM_FreeAlign
#define MEM_FreeTmp MEMPOOL_MEM_FreeTmp
#define MEM_p_AllocTmp MEMPOOL_MEM_p_AllocTmp
#define MEM_ResetTmpMemory MEMPOOL_MEM_ResetTmpMemory
#define MEM_Defrag MEMPOOL_MEM_Defrag
#define MEM_GetHolesStat MEMPOOL_MEM_GetHolesStat
#define MEM_MoveToTempMem MEMPOOL_MEM_MoveToTempMem
#define MEM_GEO_p_Alloc MEMPOOL_MEM_GEO_p_Alloc
#define MEM_GEO_p_AllocAlign MEMPOOL_MEM_GEO_p_AllocAlign
#define MEM_p_ReallocAligned MEMPOOL_MEM_p_ReallocAligned
#define MEM_p_Realloc MEMPOOL_MEM_p_Realloc
#define MEM_p_AllocFromEnd MEMPOOL_MEM_p_AllocFromEnd
#define MEM_FreeFromEnd MEMPOOL_MEM_FreeFromEnd
#define MEM_MemBlockSizeGetAlignedWithTempMemoryAddressSpaceCheck MEMPOOL_MEM_MemBlockSizeGetAlignedWithTempMemoryAddressSpaceCheck
#define MEM_MemBlockSizeGetAligned MEMPOOL_MEM_MemBlockSizeGetAligned
#define MEM_MemBlockSizeGetTmpAddressSpace MEMPOOL_MEM_MemBlockSizeGetTmpAddressSpace
#define MEM_GetBlockSizeBySize MEMPOOL_MEM_GetBlockSizeBySize
#define MEM_b_CheckBlockHasBeenFreed MEMPOOL_MEM_b_CheckBlockHasBeenFreed
#endif

/* The lock around the allocator. A semaphore standing at one, which
   is what the auto-reset event it replaces was: a taker waits for it
   and a giver signals it, and nothing requires that the two be the
   same thread -- which matters, because several paths in here signal
   it without having waited.

   A mutex would be the obvious reach and would be wrong for that
   reason: giving back a mutex this thread does not hold is undefined
   where signalling a semaphore is not. */
static SDL_Semaphore* mem_lock;

static void MEM_l_TakeLock(void)
{
    if (mem_lock)
        SDL_WaitSemaphore(mem_lock);
}

static void MEM_l_GiveLock(void)
{
    if (mem_lock)
        SDL_SignalSemaphore(mem_lock);
}

uint32_t MEM_gpv_LastFree = 0; // engine doesn't explcitly set it zero, but it could have caused problems later (eg AllocFromEnd)

void MEM_InitDynamic(SIZE_T dwBytes)
{
    int size;
    int i, j;
    memoryInfoElem*  currSizeBlockStart;
    memoryInfoElem*  currStart;

    size = 0;
    /* The arena, in one piece. It used to come from the Win32 heap,
       which is one allocator asking another for forty megabytes and
       then never using any of its services: everything below carves
       this block up itself. */
    MEM_gst_MemoryInfo.memoryStartPointer = (int)(size_t)malloc(dwBytes);
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
    /* One, which is to say available. This used to be an auto-reset
       event made unsignalled and then signalled, which comes to the
       same thing and is why there is no signal here: a semaphore
       created at one and then signalled would stand at two and let
       two callers into the allocator at once. */
    mem_lock = SDL_CreateSemaphore(1);
    MEM_InitDynamic(41943040u);
}

BOOL MEM_CloseModule()
{
    free((void*)(size_t)MEM_gst_MemoryInfo.memoryStartPointer);
    MEM_gst_MemoryInfo.memoryStartPointer = 0;
    return TRUE;
}

void MEM_CloseLock(void)
{
    if (mem_lock)
        SDL_DestroySemaphore(mem_lock);
    mem_lock = NULL;
}

BOOL MEM_b_CheckBlockHasBeenFreed(void* ptr)
{
    int marker;
    int markerType;
    unsigned int size;
    BOOL ret;
    uint32_t* tmp = (uint32_t *)ptr;

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

int MEM_GetBlockSizeBySize(void* ptr)
{
    uint32_t* tmp = (uint32_t*)ptr;
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
    MEM_l_TakeLock();
    elemID = MEM_GetElemIDBySize(size);
    blockSize = MEM_gst_MemoryInfo.memInfoElems[elemID].size;
    memInfoElem = &MEM_gst_MemoryInfo.memInfoElems[elemID];
    MEM_gst_MemoryInfo.totalUsedMemory += blockSize - size;
    ++MEM_gst_MemoryInfo.memInfoElems[elemID].id;
    if (!MEM_gst_MemoryInfo.memInfoElems[elemID].firstHole)
    {
        /* bytes handed out so far, plus this block and its header, against
           the size of the heap. Taking the addresses of the two fields
           instead of their values made this the constant 1, so the test
           always passed: the bump pointer ran past the end of the heap and
           the hole reuse below it was never reached. */
        if ((int)(MEM_gst_MemoryInfo.nextFreeMemoryPointer - MEM_gst_MemoryInfo.memoryStartPointer)
                + blockSize + 8 < MEM_gst_MemoryInfo.memorySize)
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
            MEM_l_GiveLock();
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
    MEM_l_GiveLock();
    return destinationAddress;

err:
    MEM_l_GiveLock();
    MEM_GetHolesStat(&holesNum, &sizeOfHoles);
    sprintf(Buffer,
        "[Engine %u ko -- Textures %u ko -- WH %u ko -- Gran %u ko -- Holes %u -- Size Holes %u ko]",
        MEM_gst_MemoryInfo.usedMemoryByEngine >> 10,
        MEM_gst_MemoryInfo.usedMemoryByTextures >> 10,
        (MEM_gst_MemoryInfo.nextFreeMemoryPointer - MEM_gst_MemoryInfo.memoryStartPointer) / 1024,
        MEM_gst_MemoryInfo.totalUsedMemory >> 10,
        holesNum,
        sizeOfHoles >> 10);
    /* An error box rather than the plain notice the Windows call was
       asked for: there is no memory left, the next line exits, and a
       box with no icon reads like something optional. */
    PLT_Message("No more memory", "No more memory", 1);
    return 0;
}

void* MEM_p_AllocAligned(int size, int alignment)
{
    void* ptr;
    void* ret;

    ptr = MEM_p_Alloc(2 * alignment + size);
    ret = (void*)(2 * alignment + ((unsigned int)ptr & ~(alignment - 1)));
    uint32_t* tmp = (uint32_t*)ptr;
    uint32_t* tmpRet = (uint32_t*)ret;
    tmp[-2] = 0xA0C0FFEE;
    tmpRet[-1] = (uint32_t)ptr;
    return ret;
}

void MEM_ResetTmpMemory()
{
    MEM_l_TakeLock();
    MEM_gp_AllocTmpNext = MEM_gp_AllocTmpFirst;
    MEM_gi_AllocTmpNb = 0;
    MEM_l_GiveLock();
    SOFT_ZList_Init();
    SOFT_ZList_Clear();
}

void MEM_FreeTmp(void* ptr)
{
    int size;

    uint32_t* tmp = (uint32_t*)ptr;

    size = tmp[-1];
    MEM_l_TakeLock();
    if ((char*)ptr + size - 4 == (char*)MEM_gp_AllocTmpNext) // if this is the last elem, we have to adjust MEM_gp_AllocTmpNext
        MEM_gp_AllocTmpNext = (char*)ptr - 4;
    --MEM_gi_AllocTmpNb;
    MEM_l_GiveLock();
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
            MEM_l_TakeLock();
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
            MEM_l_GiveLock();
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

    MEM_l_TakeLock();
    ptr = MEM_gp_AllocTmpNext;
    *(uint32_t*)MEM_gp_AllocTmpNext = sizeToAlloc;
    MEM_gp_AllocTmpNext += sizeToAlloc;
    ret = ptr + 4;
    ++MEM_gi_AllocTmpNb;

    MEM_l_GiveLock();
    return ret;
}

BOOL MEM_Defrag(int onlyOneHole)
{
    int lastReclaimed;
    int i;
    memoryInfoElem* currMie;
    uint32_t nextFreeMemoryPointer;
    uint32_t blockEnd;
    uint32_t memStartPtr;
    int currBlockSize;
    uint32_t holeStart;
    uint32_t nextHole;
    unsigned int sizeClass;
    memoryInfoElem* currMemoryInfoElem;
    uint32_t currHole;
    int* prevLink;

    lastReclaimed = 0;
    MEM_l_TakeLock();
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
    blockEnd = MEM_gst_MemoryInfo.nextFreeMemoryPointer;

    i_Last_245 = i >= 2304 ? 0 : i;
    for (memStartPtr = MEM_gst_MemoryInfo.memoryStartPointer; nextFreeMemoryPointer != MEM_gst_MemoryInfo.memoryStartPointer; blockEnd = nextFreeMemoryPointer)
    {
        uint32_t* blockEndTmp = (uint32_t*)blockEnd;
        currBlockSize = blockEndTmp[-1];
        if (currBlockSize <= 0)
            break;
        if ((currBlockSize & 3) != 0)
            break;
        if (currBlockSize < 8)
            break;
        holeStart = (uint32_t)&blockEndTmp[-(currBlockSize >> 2)];
        if (holeStart <= memStartPtr)
            break;
        if (holeStart > nextFreeMemoryPointer)
            break;
        uint32_t* holeTmp = (uint32_t*)holeStart;
        if (holeTmp[0] != 0xB0110270)          // ez nem egy hole (0xB0110270 = hole)
            break;
        nextHole = holeTmp[1];
        if (nextHole)
        {
            if (nextHole < memStartPtr)
                break;
        }
        if (nextHole > (nextFreeMemoryPointer + MEM_gst_MemoryInfo.memorySize))
            break;
        if (holeTmp[2] != currBlockSize)
            break;
        sizeClass = MEM_GetElemIDBySize(currBlockSize - 8);
        ++MEM_gst_MemoryInfo.memInfoElems[sizeClass].holeChainElemCount;
        nextFreeMemoryPointer = MEM_gst_MemoryInfo.nextFreeMemoryPointer - currBlockSize;
        MEM_gst_MemoryInfo.nextFreeMemoryPointer = MEM_gst_MemoryInfo.nextFreeMemoryPointer - currBlockSize;
        lastReclaimed = currBlockSize;
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
                prevLink = 0;
                while (1)
                {
                    for (; currHole - 4 < nextFreeMemoryPointer; currHole = *(uint32_t*)currHole)
                        prevLink = (int*)currHole;
                    *(uint32_t*)(currHole - 4) = 0;
                    --currMemoryInfoElem->holeChainElemCount;
                    currHole = *(uint32_t*)currHole;
                    if (prevLink)
                        *prevLink = currHole;
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
                    MEM_gst_MemoryInfo.nextFreeMemoryPointer = nextFreeMemoryPointer + lastReclaimed;
                currMemoryInfoElem->holeChainElemCount = 0;
            }
            nextFreeMemoryPointer = MEM_gst_MemoryInfo.nextFreeMemoryPointer;
        }
        ++currMemoryInfoElem;
    } while ((int)currMemoryInfoElem < (int)&MEM_gst_MemoryInfo.allocatedMemoryEnd);

    MEM_l_GiveLock();
    return TRUE;
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
    unsigned int newSize;
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
    newSize = MEM_GetElemIDBySize(tmpPtr);
    if (MEM_gst_MemoryInfo.memInfoElems[newSize].size == MEM_gst_MemoryInfo.memInfoElems[MEM_GetElemIDBySize(Size)].size)
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

void* MEM_p_AllocFromEnd(int size)
{
    int lastFree;
    unsigned int alignedSize;
    void* newPtr;

    MEM_l_TakeLock();
    lastFree = MEM_gpv_LastFree;
    if (!MEM_gpv_LastFree)
    {
        lastFree = MEM_gst_MemoryInfo.memorySize + MEM_gst_MemoryInfo.memoryStartPointer;
        MEM_gpv_LastFree = MEM_gst_MemoryInfo.memorySize + MEM_gst_MemoryInfo.memoryStartPointer;
        MEM_gul_RealSize = MEM_gst_MemoryInfo.memorySize;
        MEM_gul_FromEndMaxAllocSize = 0;
    }
    alignedSize = (size & 0xFFFFFFFC) + 4;
    if (lastFree - MEM_gst_MemoryInfo.nextFreeMemoryPointer - 8 <= alignedSize)
    {
        /* MEM_p_Alloc takes the lock itself, and it is not a recursive
           one: calling it with the lock held waited on itself forever,
           which froze the game instead of reaching "No more memory". */
        MEM_l_GiveLock();
        newPtr = MEM_p_Alloc((size & 0xFFFFFFFC) + 4);
        MEM_l_TakeLock();
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
            MEM_l_GiveLock();
            return newPtr;
        }
    }
    MEM_l_GiveLock();
    return newPtr;
}

void MEM_FreeFromEnd(void* ptr)
{
    int blockSize;
    int newLastFree;
    uint32_t nextHeader;
    int usedSize;
    unsigned int realSize;
    BOOL allFreed;
    int nextSize;
    uint32_t* ptrTmp = (uint32_t*)ptr;
    uint32_t* nextTmp;

    if (ptrTmp[-2] == 0xCAFEDECA)
    {
        MEM_l_TakeLock();
        if (((uint32_t)ptr - 8) == MEM_gpv_LastFree)
        {
            blockSize = ptrTmp[-1];
            ptrTmp[-2] = 0;
            newLastFree = MEM_gpv_LastFree + blockSize + 8;
            nextHeader = (uint32_t)ptr + blockSize + 8;
            nextTmp = (uint32_t*)nextHeader;
            usedSize = newLastFree - MEM_gst_MemoryInfo.memoryStartPointer;
            realSize = MEM_gul_RealSize;
            allFreed = newLastFree - MEM_gst_MemoryInfo.memoryStartPointer == MEM_gul_RealSize;
            MEM_gpv_LastFree = newLastFree;
            MEM_gst_MemoryInfo.memorySize = newLastFree - MEM_gst_MemoryInfo.memoryStartPointer;
            if (newLastFree - MEM_gst_MemoryInfo.memoryStartPointer < MEM_gul_RealSize)
            {
                do
                {
                    if (nextHeader - 8 != newLastFree)
                        break;
                    if (nextTmp[-2] != 0x5A5A5AA5)
                        break;
                    nextSize = nextTmp[-1];
                    nextTmp[-2] = 0;
                    newLastFree = MEM_gpv_LastFree + nextSize + 8;
                    nextHeader += nextSize + 8;
                    usedSize = newLastFree - MEM_gst_MemoryInfo.memoryStartPointer;
                    realSize = MEM_gul_RealSize;
                    MEM_gpv_LastFree = newLastFree;
                    MEM_gst_MemoryInfo.memorySize = newLastFree - MEM_gst_MemoryInfo.memoryStartPointer;
                } while (newLastFree - MEM_gst_MemoryInfo.memoryStartPointer < MEM_gul_RealSize);
                allFreed = usedSize == realSize;
            }
            if (allFreed)
            {
                MEM_gpv_LastFree = 0;
                MEM_l_GiveLock();
                return;
            }
        }
        else
        {
            ptrTmp[-2] = 0x5A5A5AA5;
        }
        MEM_l_GiveLock();
    }
    else
    {
        MEM_Free(ptr);
    }
}

int MEM_MemBlockSizeGetAlignedWithTempMemoryAddressSpaceCheck(void *ptr) {
    if (ptr < MEM_gp_AllocTmpFirst || ptr >= MEM_gp_AllocTmpLast)
        return MEM_gst_MemoryInfo.memInfoElems[MEM_GetElemIDBySize(*(uint32_t*)(*((uint32_t*)ptr - 1) - 4))].size;
    else
        return *((uint32_t*)ptr - 1);
}

int MEM_MemBlockSizeGetAligned(void* ptr) {
    return MEM_gst_MemoryInfo.memInfoElems[MEM_GetElemIDBySize(*(uint32_t*)(*((uint32_t*)ptr - 1) - 4))].size;
}

int MEM_MemBlockSizeGetTmpAddressSpace(void* ptr) {
    return *((uint32_t*)ptr - 1);
}

#ifdef IRIS_MEM_DEBUG /* the replacements for the names renamed at the top */
#undef MEM_p_Alloc
#undef MEM_p_AllocAligned
#undef MEM_Free
#undef MEM_FreeAlign
#undef MEM_FreeTmp
#undef MEM_p_AllocTmp
#undef MEM_ResetTmpMemory
#undef MEM_Defrag
#undef MEM_GetHolesStat
#undef MEM_MoveToTempMem
#undef MEM_GEO_p_Alloc
#undef MEM_GEO_p_AllocAlign
#undef MEM_p_ReallocAligned
#undef MEM_p_Realloc
#undef MEM_p_AllocFromEnd
#undef MEM_FreeFromEnd
#undef MEM_MemBlockSizeGetAlignedWithTempMemoryAddressSpaceCheck
#undef MEM_MemBlockSizeGetAligned
#undef MEM_MemBlockSizeGetTmpAddressSpace
#undef MEM_GetBlockSizeBySize
#undef MEM_b_CheckBlockHasBeenFreed
#include "memdebug.inc"
#endif
