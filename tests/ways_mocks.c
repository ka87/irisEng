#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ways_mocks.h"
#include "../memory.h"
#include "../loading.h"
#include "../bigfiles.h"
#include "../ways.h"

tWMockCall WMOCK_log[WMOCK_LOG_MAX];
int WMOCK_logLen;
unsigned char* WMOCK_arena;
size_t WMOCK_used;
unsigned char WMOCK_stream[4096];
int WMOCK_streamSize;
int WMOCK_returnNullStream;

static unsigned char s_arena[WMOCK_ARENA_SIZE];

void WMOCK_Log(const char* name, long a, long b, long c)
{
    if (WMOCK_logLen < WMOCK_LOG_MAX)
    {
        WMOCK_log[WMOCK_logLen].name = name;
        WMOCK_log[WMOCK_logLen].a = a;
        WMOCK_log[WMOCK_logLen].b = b;
        WMOCK_log[WMOCK_logLen].c = c;
    }
    ++WMOCK_logLen;
}

long WMOCK_Offset(const void* p)
{
    if (!p)
        return -1;
    return (long)((const unsigned char*)p - s_arena);
}

void WMOCK_Reset(void)
{
    WMOCK_arena = s_arena;
    memset(s_arena, 0xCD, sizeof(s_arena));
    WMOCK_used = 0;
    WMOCK_logLen = 0;
}

/* Nothing is handed back between the two sides, so the first side's
   state stays readable while the second runs; the log carries the
   allocation sequence instead. Every block sits 32 bytes into a larger
   one and is padded, because the two network callbacks ask for a
   literal 8 bytes for a struct that is 8 bytes only in the 32-bit
   build this engine is compiled for. */
void* MEM_p_Alloc(unsigned int size)
{
    void* p;

    WMOCK_Log("MEM_p_Alloc", (long)size, 0, 0);
    if (WMOCK_used + size + 96 > WMOCK_ARENA_SIZE)
    {
        fprintf(stderr, "ways mock arena exhausted\n");
        exit(2);
    }
    WMOCK_used += 32;
    p = s_arena + WMOCK_used;
    WMOCK_used += (size + 64 + 15) & ~(size_t)15;
    return p;
}

/* basic_array.c grows the seen set with this. The arena never hands
   anything back, so the grow is a fresh block and a copy. */
void* MEM_p_Realloc(void* ptr, size_t size)
{
    void* p;

    WMOCK_Log("MEM_p_Realloc", WMOCK_Offset(ptr), (long)size, 0);
    p = MEM_p_Alloc((unsigned int)size);
    if (ptr)
        memcpy(p, ptr, (size_t)size);
    return p;
}

void MEM_Free(void* p)
{
    WMOCK_Log("MEM_Free", WMOCK_Offset(p), 0, 0);
}

void LOA_DeleteAddress(int pos)
{
    WMOCK_Log("LOA_DeleteAddress", WMOCK_Offset((const void*)(intptr_t)pos), 0, 0);
}

/* The real one resolves the key later; here it logs which callback was
   asked for and writes a pointer derived from the key, so the field it
   filled is comparable without standing up the loader. */
void LOA_MakeFileRef(int key, void** retVal, void* (*func)(uint32_t), char flags)
{
    const char* which = "?";

    if ((void*)func == (void*)OBJ_ul_GameObjectCallback)
        which = "gao";
    WMOCK_Log("LOA_MakeFileRef", key, (long)flags, which[0]);
    if (retVal)
        *retVal = (void*)(intptr_t)(0x10000 + key);
}

char* BIG_pc_ReadFileTmp(int Offset, int* size)
{
    WMOCK_Log("BIG_pc_ReadFileTmp", Offset, 0, 0);
    if (size)
        *size = WMOCK_streamSize;
    if (WMOCK_returnNullStream)
        return 0;
    return (char*)WMOCK_stream + Offset;
}

int LOA_ReadLong(char** offset, uint32_t* retVal, char readMode)
{
    int result;

    result = *(uint32_t*)*offset;
    *offset += 4;
    if (retVal)
        *retVal = result;
    WMOCK_Log("LOA_ReadLong", result, 0, 0);
    return result;
}

uint16_t LOA_ReadShort(char** offset, uint16_t* retVal, char readMode)
{
    int16_t result;

    result = *(uint16_t*)*offset;
    *offset += 2;
    if (retVal)
        *retVal = result;
    WMOCK_Log("LOA_ReadShort", result, 0, 0);
    return result;
}

char LOA_ReadChar(char** offset, char* retVal, char readMode)
{
    char result;

    result = **offset;
    *offset += 1;
    if (retVal)
        *retVal = result;
    WMOCK_Log("LOA_ReadChar", result, 0, 0);
    return result;
}

/* Only ever compared against, never called. */
struct sGameObjectMain* OBJ_ul_GameObjectCallback(uint32_t Offset)
{
    fprintf(stderr, "OBJ_ul_GameObjectCallback reached\n");
    exit(2);
    return 0;
}
