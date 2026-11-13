#pragma once
#include <stddef.h>

/* The engine side of bigfiles.c: an in-memory bin file, the two
   allocators it uses, and a log of every call either copy makes.
 *
 * Each side gets its own arena and its own file position, so no
 * address and no file cursor ever reaches the comparison: pointers
 * are reported as (block ordinal, byte offset) within the side's own
 * arena.
 *
 * MEM_p_AllocFromEnd really does hand out blocks downward from the
 * top of the arena and MEM_FreeFromEnd really does give the top back,
 * because bigfiles.c frees the read buffer and then reads it through
 * the pointer it has just handed back.  A bump allocator that never
 * reused the block would hide that; this one reproduces it. */

#define BFM_LOG_MAX 400000
#define BFM_ARENA_BYTES (64 * 1024 * 1024)
#define BFM_BLOCKS 256
#define BFM_POISON 0x5C

typedef struct sBFCall
{
    const char* fn;
    long long arg[4];
} tBFCall;

extern tBFCall BFM_log[2][BFM_LOG_MAX];
extern int BFM_logLen[2];
extern int BFM_logOverflow;
extern int BFM_side;
extern int BFM_assertCount[2];

#define BFM_NULLLOC (-1LL)
#define BFM_FOREIGNLOC (-2LL)

void BFM_Reset(int side);
long long BFM_Loc(const void* p);
void* BFM_PreloadArena(int side, unsigned int size);
void BFM_SetFile(int side, const unsigned char* data, long long size);
void* BFM_FilePtr(int side);
long long BFM_FilePos(int side);
