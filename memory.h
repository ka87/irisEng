#pragma once
#include "common.h"

/* One word of the allocator's own bookkeeping, inside the arena.
 *
 * The allocator carves one block from the C library and keeps, in
 * words of the arena itself, a block's size, a freed block's chain
 * link and its length, and the real block's address in front of an
 * aligned block's payload. Some of those are addresses, so the word
 * has to be as wide as a pointer; they are all the same width because
 * they share the same slots.
 *
 * Nothing about this reaches a file: the arena is built at run time
 * and never read or written from disk, so the layout is free to differ
 * between builds. On a 32-bit build every size below comes out exactly
 * as it always did. */
/* How many bytes an element of an array of pointers takes.
 *
 * Forty-odd allocations wrote this out as the literal 4, which is right
 * in the 32-bit build the engine ships as and half of what it needs
 * where a pointer is eight bytes: the array then ends before its last
 * elements do.
 *
 * It is a macro and not simply sizeof because the frozen copies in
 * tests/ still write 4 and cannot be changed, and five of their
 * differentials compare the figure the allocator was asked for -- the
 * mock logs it and the driver compares the log. Those builds define it
 * as 4, which is what those copies use, and the mock's own double
 * headroom covers the shortfall, which is what that headroom was put
 * there for. Every other build gets the real width. */
#ifndef MEM_PTR_BYTES
#define MEM_PTR_BYTES ((unsigned int)sizeof(void*))
#endif

typedef uintptr_t MEM_Word;

#define MEM_WORD_BYTES   ((unsigned int)sizeof(MEM_Word))
/* The low bits an arena address cannot have. */
#define MEM_WORD_MASK    (MEM_WORD_BYTES - 1u)
/* A marker and a size in front of every block. */
#define MEM_HEADER_WORDS 2u
#define MEM_HEADER_BYTES (MEM_HEADER_WORDS * MEM_WORD_BYTES)
/* The smallest block there is. A freed block has to hold its chain
   link, its length and the length again as a footer, and the footer is
   placed from the end -- so a block shorter than two words would put
   the footer back on top of the link. */
#define MEM_MIN_BLOCK_BYTES (2u * MEM_WORD_BYTES)

typedef struct memoryInfoElem
{
    int holeChainElemCount;
    int size;
    int id;
    MEM_Word* firstHole;
} memoryInfoElem;

typedef struct memoryInfoStruct
{
    MEM_Word memoryStartPointer;
    MEM_Word nextFreeMemoryPointer;
    int memorySize;
    char gap1[4];
    unsigned int usedMemoryByEngine;
    char gap2[4];
    unsigned int usedMemoryByTextures;
    unsigned int totalUsedMemory;
    memoryInfoElem memInfoElems[2304];
    char* allocatedMemoryEnd;
} memoryInfoStruct;

memoryInfoStruct MEM_gst_MemoryInfo;
void *MEM_gp_AllocTmpFirst;
void *MEM_gp_AllocTmpLast;
char* MEM_gp_AllocTmpNext;
MEM_Word MEM_gp_MaxBlock;
MEM_Word gp_LastPointer;
BOOL MEM_gb_EnableTmp;
int MEM_gi_AllocTmpNb;
int i_Last_245;
extern MEM_Word MEM_gpv_LastFree;
MEM_Word MEM_gul_RealSize;
MEM_Word MEM_gul_FromEndMaxAllocSize;

/* The allocator's lock is memory.c's own. engine.c closes it on the
   way out, which is the only thing outside that file that touches
   it, so that is the only thing exposed. */
void MEM_CloseLock(void);

void MEM_InitModule();
void* MEM_p_Alloc(unsigned int size);
void* MEM_p_AllocFromEnd(int size);
void* MEM_p_Realloc(void* Src, size_t Size);
void MEM_Free(void* ptr);
void MEM_FreeFromEnd(void* ptr);
void* MEM_p_AllocTmp(int size);
void* MEM_p_AllocAligned(int size, int alignment);
void* MEM_GEO_p_AllocAlign(int size, int alignment);
void* MEM_GEO_p_Alloc(unsigned int size);
void* MEM_p_ReallocAligned(void* ptr, unsigned int size, int alignment);
void MEM_FreeAlign(void* ptr);
void MEM_GetHolesStat(int* holesNum, int* sizeOfHoles);
BOOL MEM_Defrag(int onlyOneHole);
void MEM_ResetTmpMemory();
int MEM_MemBlockSizeGetAlignedWithTempMemoryAddressSpaceCheck(void* ptr);
int MEM_MemBlockSizeGetAligned(void* ptr);
int MEM_MemBlockSizeGetTmpAddressSpace(void* ptr);
BOOL MEM_b_CheckBlockHasBeenFreed(void* ptr);
/* Whether an address is the start of a block this allocator handed
   out, and how big it was, written into out as a line of text. For a
   pointer that turned out to be wrong, this is what separates a
   block that was overwritten from an address that was never one. */
void MEM_DescribeBlock(const void* ptr, char* out, unsigned int n);
BOOL MEM_CloseModule();