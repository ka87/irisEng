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

/* What the word in front of a block says about it. The high nibble
   marks the word as one of these at all, and the rest says which. */
#define MEM_MARK_BLOCK           0xA0110270u  /* handed out */
#define MEM_MARK_ALIGNED         0xA0C0FFEEu  /* handed out by the aligned path */
#define MEM_MARK_HOLE            0xB0110270u  /* freed, and on a chain */
#define MEM_MARK_FROMEND         0xCAFEDECAu  /* handed out from the far end */
#define MEM_MARK_FROMEND_FREED   0x5A5A5AA5u  /* freed, but not the far-end block */

/* Recognising one of the first two without caring which. */
#define MEM_MARK_CLASS_MASK      0xF0000000u
#define MEM_MARK_CLASS_HANDED    0xA0000000u
#define MEM_MARK_KIND_MASK       0x00FFFFFFu
#define MEM_MARK_KIND_BLOCK      (MEM_MARK_BLOCK   & MEM_MARK_KIND_MASK)
#define MEM_MARK_KIND_ALIGNED    (MEM_MARK_ALIGNED & MEM_MARK_KIND_MASK)

/* A size this big is not a size, so a word holding one is not the
   header of a live block. */
#define MEM_IMPLAUSIBLE_SIZE     0x7FFFE00u

/* How close to the texture pointer the bump pointer may come before
   the preloaded textures are dropped to make room. */
#define MEM_LOW_WATER_BYTES      40960u

/* Temporary allocations are rounded up to this, with that much again
   added first; their own one-word header holds a size and never an
   address, so it stays as wide as it was. */
#define MEM_TMP_ALIGN            64u
typedef uint32_t MEM_TmpWord;

MEM_Word MEM_gpv_LastFree = 0; // engine doesn't explcitly set it zero, but it could have caused problems later (eg AllocFromEnd)

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
    MEM_gst_MemoryInfo.memoryStartPointer = (MEM_Word)malloc(dwBytes);
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
            /* Rounded up to a whole number of words, because a freed
               block of this size has to hold its chain link, its
               length and a footer at word offsets. Every size in this
               table is already a multiple of four, so on a build with
               four-byte words this changes nothing at all. */
            currStart->size = (int)(((unsigned int)size + MEM_WORD_MASK)
                                        & ~MEM_WORD_MASK);
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
    free((void*)MEM_gst_MemoryInfo.memoryStartPointer);
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
    MEM_Word marker;
    MEM_Word markerType;
    MEM_Word size;
    BOOL ret;
    MEM_Word* tmp = (MEM_Word*)ptr;

    marker = tmp[-2];
    ret = 1;
    if ((marker & MEM_MARK_CLASS_MASK) == MEM_MARK_CLASS_HANDED)
    {
        markerType = marker & MEM_MARK_KIND_MASK;
        if (markerType == MEM_MARK_KIND_BLOCK || markerType == MEM_MARK_KIND_ALIGNED)
        {
            size = tmp[-1];
            if (size)
            {
                if (size < MEM_IMPLAUSIBLE_SIZE)
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
    MEM_Word* tmp = (MEM_Word*)ptr;
    return MEM_gst_MemoryInfo.memInfoElems[MEM_GetElemIDBySize((unsigned int)tmp[-1])].size;
}

void MEM_GetHolesStat(int* holesNum, int* sizeOfHoles)
{
    memoryInfoElem* currMemInfoElem;
    MEM_Word* firstHole;
    MEM_Word* nextHole;

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
            nextHole = (MEM_Word*)firstHole[0]; // next hole
            if (sizeOfHoles)
                *sizeOfHoles += currMemInfoElem->size + (int)MEM_HEADER_BYTES;
            if (holesNum)
                ++*holesNum;
            while (nextHole)
            {
                /* An all-ones link is not an address; it is how a
                   chain that has been cut off ends. */
                if (nextHole == (MEM_Word*)~(MEM_Word)0)
                    break;
                nextHole = (MEM_Word*)nextHole[0]; // next hole
                if (sizeOfHoles)
                    *sizeOfHoles += currMemInfoElem->size + (int)MEM_HEADER_BYTES;
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
    MEM_Word* holeDesc;
    MEM_Word* secondHole;
    char* destinationAddress;
    int holesNum;
    unsigned int sizeOfHoles;
    char Buffer[1000];

    if (size < MEM_MIN_BLOCK_BYTES)
        size = MEM_MIN_BLOCK_BYTES;
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
                + blockSize + (int)MEM_HEADER_BYTES < MEM_gst_MemoryInfo.memorySize)
        {
            destinationAddress = (char*)MEM_gst_MemoryInfo.nextFreeMemoryPointer + MEM_HEADER_BYTES;
            MEM_gst_MemoryInfo.nextFreeMemoryPointer = MEM_gst_MemoryInfo.nextFreeMemoryPointer + blockSize + MEM_HEADER_BYTES;
            MEM_gp_MaxBlock = MEM_gst_MemoryInfo.nextFreeMemoryPointer;
            
            if (MEM_gst_MemoryInfo.nextFreeMemoryPointer >= gp_LastPointer - MEM_LOW_WATER_BYTES)
            {
                 AI_EvalFunc_DbgTraceInt_C();
                 PreLoadTexReset(); 
            }

            MEM_Word* tmpDestinationAddress = (MEM_Word*)destinationAddress;
            tmpDestinationAddress[-1] = size;
            MEM_gst_MemoryInfo.usedMemoryByEngine += memInfoElem->size + MEM_HEADER_BYTES;
            tmpDestinationAddress[-2] = MEM_MARK_BLOCK;
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
    secondHole = (MEM_Word*)holeDesc[0]; // ptr to the next hole (was the size field originally)
    destinationAddress = (char*)&holeDesc[1]; // ptr to the memory block
    memInfoElem->firstHole = secondHole;


    MEM_Word* tmpDestinationAddress = (MEM_Word*)destinationAddress;
    tmpDestinationAddress[-1] = size;
    MEM_gst_MemoryInfo.usedMemoryByEngine += memInfoElem->size + MEM_HEADER_BYTES;
    tmpDestinationAddress[-2] = MEM_MARK_BLOCK;    
    MEM_l_GiveLock();
    return destinationAddress;

err:
    MEM_l_GiveLock();
    MEM_GetHolesStat(&holesNum, &sizeOfHoles);
    sprintf(Buffer,
        "[Engine %u ko -- Textures %u ko -- WH %u ko -- Gran %u ko -- Holes %u -- Size Holes %u ko]",
        MEM_gst_MemoryInfo.usedMemoryByEngine >> 10,
        MEM_gst_MemoryInfo.usedMemoryByTextures >> 10,
        (unsigned int)((MEM_gst_MemoryInfo.nextFreeMemoryPointer - MEM_gst_MemoryInfo.memoryStartPointer) / 1024),
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
    ret = (void*)(2 * alignment + ((MEM_Word)ptr & ~(MEM_Word)(alignment - 1)));
    MEM_Word* tmp = (MEM_Word*)ptr;
    MEM_Word* tmpRet = (MEM_Word*)ret;
    tmp[-2] = MEM_MARK_ALIGNED;
    tmpRet[-1] = (MEM_Word)ptr;
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

    MEM_TmpWord* tmp = (MEM_TmpWord*)ptr;

    size = (int)tmp[-1];
    MEM_l_TakeLock();
    if ((char*)ptr + size - sizeof(MEM_TmpWord) == (char*)MEM_gp_AllocTmpNext) // if this is the last elem, we have to adjust MEM_gp_AllocTmpNext
        MEM_gp_AllocTmpNext = (char*)ptr - sizeof(MEM_TmpWord);
    --MEM_gi_AllocTmpNb;
    MEM_l_GiveLock();
    if (!MEM_gi_AllocTmpNb)
        MEM_ResetTmpMemory();
}

void MEM_Free(void* ptr)
{
    void* realBlockStartPtr;
    MEM_Word alignedPtrDesc, *alignedPtrDescPtr;
    MEM_Word* holeElem;
    unsigned int elemID;
    int realBlockSize;
    unsigned int blockElemSize;

    realBlockStartPtr = ptr;
    if (ptr)
    {
        if (ptr < MEM_gp_AllocTmpFirst || ptr >= MEM_gp_AllocTmpLast)
        {
            MEM_Word* tmpPtr = (MEM_Word*)ptr;
            alignedPtrDesc = tmpPtr[-1];
            alignedPtrDescPtr = (MEM_Word*)alignedPtrDesc;
            if (alignedPtrDesc >= MEM_gst_MemoryInfo.memoryStartPointer && alignedPtrDesc < MEM_gst_MemoryInfo.nextFreeMemoryPointer
                && (alignedPtrDesc & MEM_WORD_MASK) == 0 && alignedPtrDescPtr[-2] == MEM_MARK_ALIGNED)  // aligned malloc
            {
                alignedPtrDescPtr[-2] = MEM_MARK_BLOCK;
                realBlockStartPtr = (void *)alignedPtrDescPtr;
            }
            MEM_l_TakeLock();
            tmpPtr = (MEM_Word*)realBlockStartPtr;
            holeElem = &tmpPtr[-1];
            elemID = MEM_GetElemIDBySize((unsigned int)tmpPtr[-1]);
            realBlockSize = MEM_gst_MemoryInfo.memInfoElems[elemID].size;
            blockElemSize = realBlockSize + MEM_HEADER_BYTES;
            MEM_gst_MemoryInfo.totalUsedMemory += (unsigned int)tmpPtr[-1] - realBlockSize;
            if (MEM_gst_MemoryInfo.nextFreeMemoryPointer == (MEM_Word)realBlockStartPtr + realBlockSize)// ha az utolsó foglalt blokkot szabadítjuk fel
            {
                MEM_gst_MemoryInfo.nextFreeMemoryPointer = MEM_gst_MemoryInfo.nextFreeMemoryPointer - blockElemSize;
                tmpPtr[-2] = 0;
            }
            else
            {
                *holeElem = (MEM_Word)MEM_gst_MemoryInfo.memInfoElems[elemID].firstHole;
                tmpPtr[0] = blockElemSize;
                /* The length again, at the far end, so that the block
                   above this one can find where this one starts. The
                   index counts words from the link, which is why a
                   block can never be shorter than two of them. */
                holeElem[(blockElemSize / MEM_WORD_BYTES) - 2] = blockElemSize;
                MEM_gst_MemoryInfo.memInfoElems[elemID].firstHole = holeElem;
                tmpPtr[-2] = MEM_MARK_HOLE;
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

    sizeToAlloc = (size + 2 * MEM_TMP_ALIGN) & ~(MEM_TMP_ALIGN - 1u);
    if (!MEM_gp_AllocTmpFirst || !MEM_gb_EnableTmp || &MEM_gp_AllocTmpNext[sizeToAlloc] >= (char *)MEM_gp_AllocTmpLast)
        return (void*)MEM_p_AllocAligned((int)sizeToAlloc, (int)MEM_TMP_ALIGN);

    MEM_l_TakeLock();
    ptr = MEM_gp_AllocTmpNext;
    *(MEM_TmpWord*)MEM_gp_AllocTmpNext = sizeToAlloc;
    MEM_gp_AllocTmpNext += sizeToAlloc;
    ret = ptr + sizeof(MEM_TmpWord);
    ++MEM_gi_AllocTmpNb;

    MEM_l_GiveLock();
    return ret;
}

BOOL MEM_Defrag(int onlyOneHole)
{
    int lastReclaimed;
    int i;
    memoryInfoElem* currMie;
    MEM_Word nextFreeMemoryPointer;
    MEM_Word blockEnd;
    MEM_Word memStartPtr;
    int currBlockSize;
    MEM_Word holeStart;
    MEM_Word nextHole;
    unsigned int sizeClass;
    memoryInfoElem* currMemoryInfoElem;
    MEM_Word currHole;
    MEM_Word* prevLink;

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
        MEM_Word* blockEndTmp = (MEM_Word*)blockEnd;
        currBlockSize = (int)blockEndTmp[-1];
        if (currBlockSize <= 0)
            break;
        if (((unsigned int)currBlockSize & MEM_WORD_MASK) != 0)
            break;
        if (currBlockSize < (int)MEM_HEADER_BYTES)
            break;
        holeStart = (MEM_Word)&blockEndTmp[-(currBlockSize / (int)MEM_WORD_BYTES)];
        if (holeStart <= memStartPtr)
            break;
        if (holeStart > nextFreeMemoryPointer)
            break;
        MEM_Word* holeTmp = (MEM_Word*)holeStart;
        if (holeTmp[0] != MEM_MARK_HOLE)
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
        sizeClass = MEM_GetElemIDBySize((unsigned int)currBlockSize - MEM_HEADER_BYTES);
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
            currHole = (MEM_Word)currMemoryInfoElem->firstHole;
            if (currHole)
            {
                prevLink = 0;
                while (1)
                {
                    for (; currHole - MEM_WORD_BYTES < nextFreeMemoryPointer; currHole = *(MEM_Word*)currHole)
                        prevLink = (MEM_Word*)currHole;
                    *(MEM_Word*)(currHole - MEM_WORD_BYTES) = 0;
                    --currMemoryInfoElem->holeChainElemCount;
                    currHole = *(MEM_Word*)currHole;
                    if (prevLink)
                        *prevLink = currHole;
                    else
                        currMemoryInfoElem->firstHole = (MEM_Word*)currHole;
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
    } while ((void*)currMemoryInfoElem < (void*)&MEM_gst_MemoryInfo.allocatedMemoryEnd);

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
    MEM_Word tmpPtr;
    unsigned int newSize;
    void* newBlock; 
    int origBlockSize;
    int newBlockSize;

    MEM_Word* tmpSrc = (MEM_Word*)Src;

    if (Src >= MEM_gp_AllocTmpFirst && Src < MEM_gp_AllocTmpLast)
        return MEM_MoveToTempMem(Src, Size);
    tmpPtr = tmpSrc[-1];
    MEM_Word* v3Tmp = (MEM_Word*)tmpPtr;
    if (tmpPtr >= MEM_gst_MemoryInfo.memoryStartPointer && tmpPtr < MEM_gst_MemoryInfo.nextFreeMemoryPointer
        && (tmpPtr & MEM_WORD_MASK) == 0 && v3Tmp[-2] == MEM_MARK_ALIGNED)      // aligned alloc
    {
        return MEM_p_ReallocAligned(Src, Size, (int)MEM_TMP_ALIGN);
    }
    newSize = MEM_GetElemIDBySize((unsigned int)tmpPtr);
    if (MEM_gst_MemoryInfo.memInfoElems[newSize].size == MEM_gst_MemoryInfo.memInfoElems[MEM_GetElemIDBySize(Size)].size)
        return Src;
    newBlock = MEM_p_Alloc(Size);
    MEM_Word* newBlockTmp = (MEM_Word*)newBlock;
    origBlockSize = MEM_gst_MemoryInfo.memInfoElems[MEM_GetElemIDBySize((unsigned int)tmpSrc[-1])].size;
    newBlockSize = MEM_gst_MemoryInfo.memInfoElems[MEM_GetElemIDBySize((unsigned int)newBlockTmp[-1])].size;
    if (newBlockSize > origBlockSize)
        newBlockSize = origBlockSize;
    memcpy(newBlock, Src, newBlockSize);
    MEM_Free(Src);
    return newBlock;
}

void* MEM_p_AllocFromEnd(int size)
{
    MEM_Word lastFree;
    MEM_Word alignedSize;
    void* newPtr;

    MEM_l_TakeLock();
    lastFree = MEM_gpv_LastFree;
    if (!MEM_gpv_LastFree)
    {
        lastFree = (MEM_Word)MEM_gst_MemoryInfo.memorySize + MEM_gst_MemoryInfo.memoryStartPointer;
        MEM_gpv_LastFree = lastFree;
        MEM_gul_RealSize = (MEM_Word)MEM_gst_MemoryInfo.memorySize;
        MEM_gul_FromEndMaxAllocSize = 0;
    }
    alignedSize = ((MEM_Word)size & ~(MEM_Word)MEM_WORD_MASK) + MEM_WORD_BYTES;
    if (lastFree - MEM_gst_MemoryInfo.nextFreeMemoryPointer - MEM_HEADER_BYTES <= alignedSize)
    {
        /* MEM_p_Alloc takes the lock itself, and it is not a recursive
           one: calling it with the lock held waited on itself forever,
           which froze the game instead of reaching "No more memory". */
        MEM_l_GiveLock();
        newPtr = MEM_p_Alloc((unsigned int)alignedSize);
        MEM_l_TakeLock();
    }
    else
    {
        newPtr = (void*)(lastFree - alignedSize);
        MEM_gpv_LastFree = lastFree - alignedSize - MEM_HEADER_BYTES;
        *(MEM_Word*)MEM_gpv_LastFree = MEM_MARK_FROMEND;
        *(MEM_Word*)(lastFree - alignedSize - MEM_WORD_BYTES) = alignedSize;
        MEM_gst_MemoryInfo.memorySize = (int)(MEM_gpv_LastFree - MEM_gst_MemoryInfo.memoryStartPointer);
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
    MEM_Word newLastFree;
    MEM_Word nextHeader;
    MEM_Word usedSize;
    MEM_Word realSize;
    BOOL allFreed;
    int nextSize;
    MEM_Word* ptrTmp = (MEM_Word*)ptr;
    MEM_Word* nextTmp;

    if (ptrTmp[-2] == MEM_MARK_FROMEND)
    {
        MEM_l_TakeLock();
        if (((MEM_Word)ptr - MEM_HEADER_BYTES) == MEM_gpv_LastFree)
        {
            blockSize = (int)ptrTmp[-1];
            ptrTmp[-2] = 0;
            newLastFree = MEM_gpv_LastFree + blockSize + MEM_HEADER_BYTES;
            nextHeader = (MEM_Word)ptr + blockSize + MEM_HEADER_BYTES;
            nextTmp = (MEM_Word*)nextHeader;
            usedSize = newLastFree - MEM_gst_MemoryInfo.memoryStartPointer;
            realSize = MEM_gul_RealSize;
            allFreed = newLastFree - MEM_gst_MemoryInfo.memoryStartPointer == MEM_gul_RealSize;
            MEM_gpv_LastFree = newLastFree;
            MEM_gst_MemoryInfo.memorySize = (int)(newLastFree - MEM_gst_MemoryInfo.memoryStartPointer);
            if (newLastFree - MEM_gst_MemoryInfo.memoryStartPointer < MEM_gul_RealSize)
            {
                do
                {
                    if (nextHeader - MEM_HEADER_BYTES != newLastFree)
                        break;
                    if (nextTmp[-2] != MEM_MARK_FROMEND_FREED)
                        break;
                    nextSize = (int)nextTmp[-1];
                    nextTmp[-2] = 0;
                    newLastFree = MEM_gpv_LastFree + nextSize + MEM_HEADER_BYTES;
                    nextHeader += nextSize + MEM_HEADER_BYTES;
                    usedSize = newLastFree - MEM_gst_MemoryInfo.memoryStartPointer;
                    realSize = MEM_gul_RealSize;
                    MEM_gpv_LastFree = newLastFree;
                    MEM_gst_MemoryInfo.memorySize = (int)(newLastFree - MEM_gst_MemoryInfo.memoryStartPointer);
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
            ptrTmp[-2] = MEM_MARK_FROMEND_FREED;
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
        return MEM_MemBlockSizeGetAligned(ptr);
    else
        return (int)*((MEM_TmpWord*)ptr - 1);
}

/* Through the real block's address, which the aligned path leaves in
   the word in front of the payload, to the size in front of that. */
int MEM_MemBlockSizeGetAligned(void* ptr) {
    MEM_Word realBlock = *((MEM_Word*)ptr - 1);
    unsigned int size = (unsigned int)*(MEM_Word*)(realBlock - MEM_WORD_BYTES);
    return MEM_gst_MemoryInfo.memInfoElems[MEM_GetElemIDBySize(size)].size;
}

/* The size a temporary allocation wrote in front of itself. */
int MEM_MemBlockSizeGetTmpAddressSpace(void* ptr) {
    return (int)*((MEM_TmpWord*)ptr - 1);
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
