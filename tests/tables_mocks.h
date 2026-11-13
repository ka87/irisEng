#pragma once
#include "../common.h"
#include "../memory.h"
#include "../tables.h"

/* tables.c needs an allocator and nothing else.
 *
 * Each side gets its own arena, so a block handed to the frozen copy
 * and the matching block handed to the rewrite land at different
 * addresses and still compare equal: every pointer that reaches the
 * comparison is reported as (allocation ordinal, byte offset) rather
 * than as an address.  Nothing is ever handed back, so a block stays
 * readable after the free that logs it. */

#define TBM_LOG_MAX 400000
#define TBM_ARENA_BYTES (160 * 1024 * 1024)
#define TBM_BLOCKS 100000
#define TBM_POISON 0x5C

typedef struct sTabCall
{
    const char* fn;
    unsigned long long arg[3];
} tTabCall;

extern tTabCall TBM_log[2][TBM_LOG_MAX];
extern int TBM_logLen[2];
extern int TBM_logOverflow;
extern int TBM_side;

void TBM_Reset(int side);

/* (allocation ordinal << 32) | byte offset, or a sentinel for NULL and
   for an address the allocator never handed out. */
#define TBM_NULLLOC 0xFFFFFFFFFFFFFFFFull
#define TBM_FOREIGNLOC 0xFFFFFFFEFFFFFFFFull
unsigned long long TBM_Loc(const void* p);
