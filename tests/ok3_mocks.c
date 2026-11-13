#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "ok3_mocks.h"

tOk3Call OK3M_log[OK3M_LOG_MAX];
int OK3M_logLen;
int OK3M_logOverflow;

unsigned char OK3M_arena[OK3M_ARENA_BYTES];
unsigned int OK3M_arenaUsed;

int COL_OK3_Realloc;

unsigned long long OK3M_F(float f)
{
    unsigned int bits;
    memcpy(&bits, &f, 4);
    return bits;
}

unsigned long long OK3M_P(const void* p)
{
    return (unsigned long long)(size_t)p;
}

static void Log(const char* fn, int n, ...)
{
    va_list ap;
    int i;
    if (OK3M_logLen >= OK3M_LOG_MAX)
    {
        OK3M_logOverflow = 1;
        return;
    }
    OK3M_log[OK3M_logLen].fn = fn;
    OK3M_log[OK3M_logLen].nargs = n;
    va_start(ap, n);
    for (i = 0; i < n && i < OK3M_ARGS; ++i)
        OK3M_log[OK3M_logLen].arg[i] = va_arg(ap, unsigned long long);
    va_end(ap);
    for (; i < OK3M_ARGS; ++i)
        OK3M_log[OK3M_logLen].arg[i] = 0;
    ++OK3M_logLen;
}

#define OK3M_BLOCKS 262144
static void* blockPtr[OK3M_BLOCKS];
static unsigned int blockSize[OK3M_BLOCKS];
static int blockCount;

void* OK3M_Bump(unsigned int size, unsigned int align)
{
    unsigned int base;
    unsigned int total;
    if (align < 8)
        align = 8;
    OK3M_arenaUsed = (OK3M_arenaUsed + align - 1u) & ~(align - 1u);
    base = OK3M_arenaUsed;
    total = size * OK3M_SLACK + 256u;
    if (base + total > (unsigned int)OK3M_ARENA_BYTES)
    {
        fprintf(stderr, "ok3 mock arena exhausted\n");
        exit(2);
    }
    OK3M_arenaUsed = base + total;
    memset(OK3M_arena + base, OK3M_POISON, total);
    if (blockCount < OK3M_BLOCKS)
    {
        blockPtr[blockCount] = OK3M_arena + base;
        blockSize[blockCount] = size;
        ++blockCount;
    }
    return OK3M_arena + base;
}

void* MEM_p_Alloc(unsigned int size)
{
    void* p = OK3M_Bump(size, 8);
    Log("MEM_p_Alloc", 2, (unsigned long long)size, OK3M_P(p));
    return p;
}

void* MEM_p_Realloc(void* old, size_t size)
{
    void* p;
    unsigned int oldSize = 0;
    int i;
    for (i = blockCount - 1; i >= 0; --i)
        if (blockPtr[i] == old) { oldSize = blockSize[i]; break; }
    p = OK3M_Bump((unsigned int)size, 8);
    if (old)
        memcpy(p, old, (size_t)oldSize < size ? (size_t)oldSize : size);
    Log("MEM_p_Realloc", 3, OK3M_P(old), (unsigned long long)size, OK3M_P(p));
    return p;
}

/* A freed block is left readable on purpose.  Scrubbing it, which is
   what tests/gfxcore_mocks.c does, cannot work here: a box moved by
   one of the 32-byte memcpys keeps the chunk pointer that was in the
   slot before, and on this host that pointer belongs to the box that
   was just freed.  Scrubbing turns the whole build group into a
   crash rather than a comparison. */
void MEM_Free(void* p)
{
    Log("MEM_Free", 1, OK3M_P(p));
}

/* math.c's own table setup wants these; ok3.c never calls them. */
void* MEM_p_AllocAligned(int size, int align)
{
    return OK3M_Bump((unsigned int)size, (unsigned int)align);
}

void* MEM_p_ReallocAligned(void* old, unsigned int size, int align)
{
    void* p = OK3M_Bump(size, (unsigned int)align);
    (void)old;
    return p;
}

void MEM_FreeAlign(void* p)
{
    (void)p;
}

/* Deterministic, and every argument it is given is logged. */
int INT_PointInTriangle(Vec3* point, Vec3* p1, Vec3* p2, Vec3* p3, Vec3* normal, uint8_t* edgeOut)
{
    unsigned int h;
    Log("INT_PointInTriangle", 8, OK3M_F(point->x), OK3M_F(point->y), OK3M_F(point->z),
        OK3M_F(normal->x), OK3M_F(normal->y), OK3M_F(normal->z),
        OK3M_P(p1), OK3M_P(p3));
    h = (unsigned int)(OK3M_F(point->x) ^ (OK3M_F(point->y) << 1) ^ (OK3M_F(point->z) << 2));
    return (h & 3u) == 0u;
}

void OK3M_Reset(void)
{
    memset(OK3M_arena, 0, sizeof(OK3M_arena));
    OK3M_arenaUsed = 0;
    blockCount = 0;
    OK3M_logLen = 0;
    OK3M_logOverflow = 0;
    COL_OK3_Realloc = 0;
}
