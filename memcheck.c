#include "memcheck.h"

#if MEMCHK_ENABLED

#include "common.h"
#include "memory.h"
#include "dbg.h"

static unsigned int MEMCHK_gul_Calls;
static unsigned int MEMCHK_gul_Faults;

static unsigned int MEMCHK_Used(void)
{
    return MEM_gst_MemoryInfo.nextFreeMemoryPointer
         - MEM_gst_MemoryInfo.memoryStartPointer;
}

static int MEMCHK_InHeap(unsigned int p)
{
    return p >= MEM_gst_MemoryInfo.memoryStartPointer
        && p <  MEM_gst_MemoryInfo.memoryStartPointer
                + (unsigned int)MEM_gst_MemoryInfo.memorySize;
}

/* A hole descriptor sits four bytes below the block it describes. Its
   first word is the link to the next hole of the same size class, its
   second the block size, and the word below it carries the freed
   marker MEM_Free wrote. Each of those is checked before the chain is
   followed. */
int MEMCHK_HoleIsSound(unsigned int elemID, int* holeDesc)
{
    unsigned int link, marker, recorded;
    int expected;

    if (!holeDesc)
        return 1;                       /* not a hole path at all */

    if (!MEMCHK_InHeap((unsigned int)holeDesc))
    {
        DBG_OutputDebugString("MEMCHK hole for elem %u is outside the heap: %p"
                              " (heap %08x + %d)\n", elemID, (void*)holeDesc,
                              MEM_gst_MemoryInfo.memoryStartPointer,
                              MEM_gst_MemoryInfo.memorySize);
        ++MEMCHK_gul_Faults;
        return 0;
    }
    if (((unsigned int)holeDesc & 3) != 0)
    {
        DBG_OutputDebugString("MEMCHK hole for elem %u is misaligned: %p\n",
                              elemID, (void*)holeDesc);
        ++MEMCHK_gul_Faults;
        return 0;
    }

    marker   = ((unsigned int*)holeDesc)[-1];   /* the word below the descriptor */
    link     = (unsigned int)holeDesc[0];
    recorded = (unsigned int)holeDesc[1];
    expected = MEM_gst_MemoryInfo.memInfoElems[elemID].size + 8;

    if (marker != 0xB0110270u)
        DBG_OutputDebugString("MEMCHK hole %p for elem %u: marker is %08x,"
                              " expected B0110270 -- the block was written after"
                              " it was freed, or this is not a hole\n",
                              (void*)holeDesc, elemID, marker);
    if ((int)recorded != expected)
        DBG_OutputDebugString("MEMCHK hole %p for elem %u: records size %u,"
                              " the size class says %d\n",
                              (void*)holeDesc, elemID, recorded, expected);
    if (link && !MEMCHK_InHeap(link))
    {
        DBG_OutputDebugString("MEMCHK hole %p for elem %u: next link %08x is"
                              " outside the heap -- following it is the crash\n",
                              (void*)holeDesc, elemID, link);
        ++MEMCHK_gul_Faults;
        return 0;
    }
    if (marker != 0xB0110270u || (int)recorded != expected)
    {
        ++MEMCHK_gul_Faults;
        return 0;
    }
    return 1;
}

void MEMCHK_Note(const char* path, unsigned int size, unsigned int elemID, int blockSize)
{
    ++MEMCHK_gul_Calls;

    if (elemID >= 2304u)
    {
        DBG_OutputDebugString("MEMCHK size %u gave element %u, which is past the"
                              " table -- a request this large has no size class\n",
                              size, elemID);
        ++MEMCHK_gul_Faults;
    }

    if (MEMCHK_VERBOSE)
        DBG_OutputDebugString("MEMCHK %s size %u elem %u block %d used %u of %d\n",
                              path, size, elemID, blockSize,
                              MEMCHK_Used(), MEM_gst_MemoryInfo.memorySize);
    else if (MEMCHK_SUMMARY_EVERY && (MEMCHK_gul_Calls % MEMCHK_SUMMARY_EVERY) == 0)
        DBG_OutputDebugString("MEMCHK %u allocations, %u faults, used %u of %d\n",
                              MEMCHK_gul_Calls, MEMCHK_gul_Faults,
                              MEMCHK_Used(), MEM_gst_MemoryInfo.memorySize);
}

void MEMCHK_Exhausted(unsigned int size, unsigned int elemID, int blockSize)
{
    int holesNum = 0;
    int sizeOfHoles = 0;   /* MEM_GetHolesStat takes int* for both */

    MEM_GetHolesStat(&holesNum, &sizeOfHoles);
    DBG_OutputDebugString("MEMCHK OUT OF MEMORY asking %u (elem %u, block %d):"
                          " used %u of %d, %d holes totalling %d\n",
                          size, elemID, blockSize, MEMCHK_Used(),
                          MEM_gst_MemoryInfo.memorySize, holesNum, sizeOfHoles);
}

#endif
