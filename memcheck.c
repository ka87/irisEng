#include "memcheck.h"

#if MEMCHK_ENABLED

#include "common.h"
#include "memory.h"
#include "dbg.h"
#include <windows.h>

static unsigned int MEMCHK_Used(void);

static unsigned int MEMCHK_gul_Calls;
static unsigned int MEMCHK_gul_Faults;

/* The first thing to establish after a crash with an address in hand:
   where the heap actually is. HeapAlloc's result is not checked here or
   in the original, so a failed reservation leaves the base at zero and
   every pointer derived from it is nonsense. */
void MEMCHK_Init(unsigned int base, int size)
{
    if (!base)
    {
        DBG_OutputDebugString("MEMCHK the heap was not reserved at all:"
                              " HeapAlloc(%d) returned null\n", size);
        return;
    }
    DBG_OutputDebugString("MEMCHK heap %08x .. %08x (%d bytes, %d MB)\n",
                          base, base + (unsigned int)size, size, size >> 20);
}

void MEMCHK_Where(const char* what, unsigned int addr)
{
    const unsigned int base = MEM_gst_MemoryInfo.memoryStartPointer;
    const unsigned int end  = base + (unsigned int)MEM_gst_MemoryInfo.memorySize;

    if (addr == base)
        DBG_OutputDebugString("MEMCHK %s %08x is the heap base exactly\n", what, addr);
    else if (addr >= base && addr < end)
        DBG_OutputDebugString("MEMCHK %s %08x is inside the heap, %u bytes in"
                              " (used %u)\n", what, addr, addr - base, MEMCHK_Used());
    else
        DBG_OutputDebugString("MEMCHK %s %08x is OUTSIDE the heap %08x .. %08x\n",
                              what, addr, base, end);
}

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
        MEMCHK_Dump("the corrupted descriptor", holeDesc);
        /* The size is recorded twice: once just above the descriptor and
           once in the last word of the block. If both copies survived,
           nothing ran over either end of the block, and the one bad word
           between them was written on purpose by something holding a
           stale pointer four bytes ahead of the user data. */
        if ((int)recorded == expected)
        {
            unsigned int footer = (unsigned int)holeDesc[(expected >> 2) - 2];

            DBG_OutputDebugString("MEMCHK   footer at %+d is %08x, expected"
                                  " %08x -- %s\n", expected - 8, footer,
                                  (unsigned int)expected,
                                  footer == (unsigned int)expected
                                    ? "intact, so neither end of the block was"
                                      " run over and the one bad word was"
                                      " written deliberately"
                                    : "also wrong, so something ran over the"
                                      " block");
        }
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

#if MEMCHK_PAGE_TRAP

static void*  MEMCHK_gp_TrappedPage;
static LONG CALLBACK MEMCHK_OnAccessViolation(EXCEPTION_POINTERS* info);
static int    MEMCHK_gi_TrapInstalled;
static unsigned int MEMCHK_gul_TrapHits;

/* The handler runs on the faulting thread before anything else gets a
   look at the exception, so the instruction address it reports is the
   writer itself. It then lifts the protection and returns to the same
   instruction, which now succeeds -- the write is allowed through, so
   the game carries on and the engine behaves as it did without the
   trap. The page stays writable afterwards; a later scan arms it
   again. */
static LONG CALLBACK MEMCHK_OnAccessViolation(EXCEPTION_POINTERS* info)
{
    EXCEPTION_RECORD* rec = info->ExceptionRecord;
    void* page = MEMCHK_gp_TrappedPage;
    unsigned int target;
    DWORD wasProtected;

    if (rec->ExceptionCode != (DWORD)EXCEPTION_ACCESS_VIOLATION
        || rec->NumberParameters < 2 || !page)
        return EXCEPTION_CONTINUE_SEARCH;

    target = (unsigned int)rec->ExceptionInformation[1];
    if ((target & ~0xFFFu) != (unsigned int)page)
        return EXCEPTION_CONTINUE_SEARCH;       /* not our page */

    ++MEMCHK_gul_TrapHits;
    DBG_OutputDebugString("MEMCHK TRAP hit %u: the instruction at %p %s %08x"
                          " -- this is the code to look at\n",
                          MEMCHK_gul_TrapHits, rec->ExceptionAddress,
                          rec->ExceptionInformation[0] == 1 ? "wrote to"
                                                            : "read from",
                          target);

    MEMCHK_gp_TrappedPage = 0;
    VirtualProtect(page, 0x1000, PAGE_READWRITE, &wasProtected);
    return EXCEPTION_CONTINUE_EXECUTION;
}

void MEMCHK_ArmPageTrap(void* addr)
{
    void* page = (void*)((unsigned int)addr & ~0xFFFu);
    DWORD wasProtected;

    if (!addr || MEMCHK_gp_TrappedPage)
        return;                         /* one at a time */

    if (!MEMCHK_gi_TrapInstalled)
    {
        if (!AddVectoredExceptionHandler(1, MEMCHK_OnAccessViolation))
        {
            DBG_OutputDebugString("MEMCHK could not install the trap handler\n");
            return;
        }
        MEMCHK_gi_TrapInstalled = 1;
    }

    if (!VirtualProtect(page, 0x1000, PAGE_READONLY, &wasProtected))
    {
        DBG_OutputDebugString("MEMCHK could not protect the page at %p\n", page);
        return;
    }

    MEMCHK_gp_TrappedPage = page;
    DBG_OutputDebugString("MEMCHK trap armed on the page at %p, covering %p"
                          " -- the next access to it will be reported\n",
                          page, addr);
}

#endif  /* MEMCHK_PAGE_TRAP */

/* Print the words around a descriptor. The descriptor sits at ptr-4,
   between the marker at ptr-8 and the recorded size at ptr+0, so a
   value that is wrong there was written four bytes ahead of a block's
   user data. Seeing its neighbours says whether something wrote a
   single word or a run of them, and whether the run looks like a
   structure. Marked up so the three words the allocator owns are
   obvious. */
void MEMCHK_Dump(const char* what, int* holeDesc)
{
    unsigned int* w = (unsigned int*)holeDesc;
    int i;

    DBG_OutputDebugString("MEMCHK %s: words around descriptor %p\n",
                          what, (void*)holeDesc);
    for (i = -4; i <= 8; i++)
    {
        const char* tag = "";

        if (i == -1) tag = "   <- marker, should be B0110270";
        if (i ==  0) tag = "   <- the link";
        if (i ==  1) tag = "   <- recorded size";

        DBG_OutputDebugString("MEMCHK   %p  %+4d  %08x%s\n",
                              (void*)(w + i), i * 4, w[i], tag);
    }
}

/* Walk all the chains and report the first descriptor that has gone
   bad, with the allocation count, so two consecutive scans bracket
   whatever wrote it. Chains are capped in case one has become a loop. */
int MEMCHK_ScanChains(const char* when)
{
    int elem, bad = 0;

    for (elem = 0; elem < 2304; elem++)
    {
        int* hole = MEM_gst_MemoryInfo.memInfoElems[elem].firstHole;
        int steps = 0;

        while (hole && steps < 100000)
        {
            unsigned int link, marker;

            if (!MEMCHK_InHeap((unsigned int)hole) || ((unsigned int)hole & 3))
            {
                DBG_OutputDebugString("MEMCHK scan %s at %u allocations: elem %d"
                                      " chain step %d has descriptor %p, not a heap"
                                      " address\n", when, MEMCHK_gul_Calls, elem,
                                      steps, (void*)hole);
                ++bad;
                break;
            }

            marker = ((unsigned int*)hole)[-1];
            link   = (unsigned int)hole[0];

            if (marker != 0xB0110270u)
            {
                DBG_OutputDebugString("MEMCHK scan %s at %u allocations: elem %d"
                                      " step %d descriptor %p marker is %08x\n",
                                      when, MEMCHK_gul_Calls, elem, steps,
                                      (void*)hole, marker);
                ++bad;
                break;
            }
            if (link && !MEMCHK_InHeap(link))
            {
                DBG_OutputDebugString("MEMCHK scan %s at %u allocations: elem %d"
                                      " step %d descriptor %p link %08x is outside"
                                      " the heap -- this is the corruption, and it"
                                      " happened since the previous scan\n",
                                      when, MEMCHK_gul_Calls, elem, steps,
                                      (void*)hole, link);
                ++bad;
                break;
            }

            hole = (int*)link;
            ++steps;
        }

        if (steps >= 100000)
        {
            DBG_OutputDebugString("MEMCHK scan %s: elem %d chain does not end --"
                                  " it has become a loop\n", when, elem);
            ++bad;
        }
        if (bad)
            break;
    }
    return bad;
}

void MEMCHK_Note(const char* path, unsigned int size, unsigned int elemID, int blockSize)
{
    ++MEMCHK_gul_Calls;

    if (MEMCHK_SCAN_EVERY && (MEMCHK_gul_Calls % MEMCHK_SCAN_EVERY) == 0)
        MEMCHK_gul_Faults += MEMCHK_ScanChains("periodic");

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
