#pragma once
#include "common.h"

typedef struct memoryInfoElem
{
    int holeChainElemCount;
    int size;
    int id;
    int* firstHole;
} memoryInfoElem;

typedef struct memoryInfoStruct
{
    uint32_t memoryStartPointer;
    uint32_t nextFreeMemoryPointer;
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
int MEM_gp_MaxBlock;
uint32_t gp_LastPointer;
BOOL MEM_gb_EnableTmp;
int MEM_gi_AllocTmpNb;
int i_Last_245;
extern uint32_t MEM_gpv_LastFree;
uint32_t MEM_gul_RealSize;
uint32_t MEM_gul_FromEndMaxAllocSize;

HANDLE memMngEvent;

void MEM_InitModule();
void* MEM_p_Alloc(unsigned int size);
void* MEM_p_AllocFromEnd(int a1);
void* MEM_p_Realloc(void* Src, size_t Size);
void MEM_Free(void* ptr);
void MEM_FreeFromEnd(void* ptr);
void* MEM_p_AllocTmp(int size);
void* MEM_p_AllocAligned(int a1, int alignment);
void* MEM_GEO_p_AllocAlign(int size, int alignment);
void* MEM_GEO_p_Alloc(unsigned int size);
void* MEM_p_ReallocAligned(void* ptr, unsigned int size, int alignment);
void MEM_FreeAlign(void* ptr);
void MEM_GetHolesStat(int* holesNum, int* sizeOfHoles);
BOOL MEM_Defrag(int onlyOneHole);
void MEM_ResetTmpMemory();
int MEM_MemBlockSizeGetAlignedWithTempMemoryAddressSpaceCheck(void* v1);
int MEM_MemBlockSizeGetAligned(void* v1);
int MEM_MemBlockSizeGetTmpAddressSpace(void* v1);
BOOL MEM_b_CheckBlockHasBeenFreed(void* a1);
BOOL MEM_CloseModule();