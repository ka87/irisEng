#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tables_mocks.h"

tTabCall TBM_log[2][TBM_LOG_MAX];
int TBM_logLen[2];
int TBM_logOverflow;
int TBM_side;

static unsigned char* arena[2];
static unsigned int arenaUsed[2];
static unsigned char* blockPtr[2][TBM_BLOCKS];
static unsigned int blockSize[2][TBM_BLOCKS];
static int blockCount[2];

static void Log(const char* fn, unsigned long long a0, unsigned long long a1,
                unsigned long long a2)
{
    int s = TBM_side;
    if (TBM_logLen[s] >= TBM_LOG_MAX)
    {
        TBM_logOverflow = 1;
        return;
    }
    TBM_log[s][TBM_logLen[s]].fn = fn;
    TBM_log[s][TBM_logLen[s]].arg[0] = a0;
    TBM_log[s][TBM_logLen[s]].arg[1] = a1;
    TBM_log[s][TBM_logLen[s]].arg[2] = a2;
    ++TBM_logLen[s];
}

void TBM_Reset(int side)
{
    if (!arena[side])
    {
        arena[side] = (unsigned char*)malloc(TBM_ARENA_BYTES);
        if (!arena[side])
        {
            fprintf(stderr, "tables mock arena allocation failed\n");
            exit(2);
        }
    }
    memset(arena[side], TBM_POISON, TBM_ARENA_BYTES);
    arenaUsed[side] = 0;
    blockCount[side] = 0;
    TBM_logLen[side] = 0;
}

/* The blocks come out of one bump arena, so their addresses rise with
   their ordinals and the lookup can binary search. */
unsigned long long TBM_Loc(const void* p)
{
    int s = TBM_side;
    int lo = 0;
    int hi = blockCount[s] - 1;
    int found = -1;

    if (!p)
        return TBM_NULLLOC;
    while (lo <= hi)
    {
        int mid = (lo + hi) / 2;
        if (blockPtr[s][mid] <= (const unsigned char*)p)
        {
            found = mid;
            lo = mid + 1;
        }
        else
        {
            hi = mid - 1;
        }
    }
    if (found >= 0 &&
        (const unsigned char*)p < blockPtr[s][found] + blockSize[s][found])
        return ((unsigned long long)found << 32) |
               (unsigned long long)((const unsigned char*)p - blockPtr[s][found]);
    return TBM_FOREIGNLOC;
}

void* MEM_p_Alloc(unsigned int size)
{
    int s = TBM_side;
    unsigned int base;
    unsigned int total;

    arenaUsed[s] = (arenaUsed[s] + 15u) & ~15u;
    base = arenaUsed[s];
    /* generous on purpose: in a shrinking resize the frozen copy
       copies past the end of the block it was just handed, because
       the element count it computes is the 32-bit one.  The slack is
       what keeps that from reaching the next block. */
    total = 2u * size + 8192u;
    if (base + total > (unsigned int)TBM_ARENA_BYTES)
    {
        fprintf(stderr, "tables mock arena exhausted\n");
        exit(2);
    }
    arenaUsed[s] = base + total;
    memset(arena[s] + base, TBM_POISON, total);
    if (blockCount[s] >= TBM_BLOCKS)
    {
        fprintf(stderr, "tables mock block table full\n");
        exit(2);
    }
    blockPtr[s][blockCount[s]] = arena[s] + base;
    blockSize[s][blockCount[s]] = size;
    ++blockCount[s];
    Log("MEM_p_Alloc", size, (unsigned long long)(blockCount[s] - 1), 0);
    return arena[s] + base;
}

void MEM_Free(void* ptr)
{
    Log("MEM_Free", TBM_Loc(ptr), 0, 0);
}
