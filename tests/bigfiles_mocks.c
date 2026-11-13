#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bigfiles_compat.h"
#include "../common.h"
#include "../basic_array.h"
#include "../memory.h"
#include "../minilzo/minilzo.h"
#include "bigfiles_mocks.h"

tBFCall BFM_log[2][BFM_LOG_MAX];
int BFM_logLen[2];
int BFM_logOverflow;
int BFM_side;
int BFM_assertCount[2];

/* declared extern by the engine's headers, so the harness owns them */
int LOA_gb_CompressBin;
int BAS_bsortmode;
int gi_LastES;

typedef struct sBFile
{
    const unsigned char* data;
    long long size;
    long long pos;
} tBFile;

static tBFile bfile[2];
static unsigned char* arena[2];
static unsigned int upUsed[2];
static unsigned int downUsed[2];
static unsigned char* blockPtr[2][BFM_BLOCKS];
static unsigned int blockSize[2][BFM_BLOCKS];
static int blockCount[2];

static void Log(const char* fn, long long a0, long long a1, long long a2, long long a3)
{
    int s = BFM_side;
    if (BFM_logLen[s] >= BFM_LOG_MAX)
    {
        BFM_logOverflow = 1;
        return;
    }
    BFM_log[s][BFM_logLen[s]].fn = fn;
    BFM_log[s][BFM_logLen[s]].arg[0] = a0;
    BFM_log[s][BFM_logLen[s]].arg[1] = a1;
    BFM_log[s][BFM_logLen[s]].arg[2] = a2;
    BFM_log[s][BFM_logLen[s]].arg[3] = a3;
    ++BFM_logLen[s];
}

static int Register(int s, unsigned char* p, unsigned int size)
{
    if (blockCount[s] >= BFM_BLOCKS)
    {
        fprintf(stderr, "bigfiles mock block table full\n");
        exit(2);
    }
    blockPtr[s][blockCount[s]] = p;
    blockSize[s][blockCount[s]] = size;
    return blockCount[s]++;
}

void BFM_Reset(int side)
{
    if (!arena[side])
    {
        arena[side] = (unsigned char*)malloc(BFM_ARENA_BYTES);
        if (!arena[side])
        {
            fprintf(stderr, "bigfiles mock arena allocation failed\n");
            exit(2);
        }
    }
    memset(arena[side], BFM_POISON, BFM_ARENA_BYTES);
    upUsed[side] = 0;
    downUsed[side] = BFM_ARENA_BYTES - (1024 * 1024); /* guard band above the first block */
    blockCount[side] = 0;
}

long long BFM_Loc(const void* p)
{
    int s = BFM_side;
    int i;
    if (!p)
        return BFM_NULLLOC;
    for (i = blockCount[s] - 1; i >= 0; --i)
    {
        if ((const unsigned char*)p >= blockPtr[s][i] &&
            (const unsigned char*)p < blockPtr[s][i] + blockSize[s][i])
            return ((long long)i << 32) | (long long)((const unsigned char*)p - blockPtr[s][i]);
    }
    return BFM_FOREIGNLOC;
}

/* A block the driver owns, used for the preloaded-bin pointers. */
void* BFM_PreloadArena(int side, unsigned int size)
{
    unsigned char* p;
    upUsed[side] = (upUsed[side] + 4095u) & ~4095u;
    p = arena[side] + upUsed[side];
    upUsed[side] += size + 4096u;
    Register(side, p, size);
    return p;
}

void BFM_SetFile(int side, const unsigned char* data, long long size)
{
    bfile[side].data = data;
    bfile[side].size = size;
    bfile[side].pos = 0;
}

void* BFM_FilePtr(int side)
{
    return &bfile[side];
}

long long BFM_FilePos(int side)
{
    return bfile[side].pos;
}

/* ---- the C library, in memory ---- */

size_t BIGM_fread(void* buf, size_t size, size_t count, FILE* fp)
{
    tBFile* f = (tBFile*)fp;
    long long want = (long long)size * (long long)count;
    long long have = f->size - f->pos;
    long long n = want < have ? want : have;
    if (n < 0)
        n = 0;
    if (n)
        memcpy(buf, f->data + f->pos, (size_t)n);
    f->pos += n;
    Log("fread", (long long)size, (long long)count, f->pos - n, n);
    return size ? (size_t)(n / (long long)size) : 0;
}

int BIGM_fseek(FILE* fp, long offset, int origin)
{
    tBFile* f = (tBFile*)fp;
    long long base = origin == 1 ? f->pos : (origin == 2 ? f->size : 0);
    f->pos = base + offset;
    Log("fseek", offset, origin, f->pos, 0);
    return 0;
}

int BIGM_fgetpos(FILE* fp, BIGM_fpos_t* pos)
{
    tBFile* f = (tBFile*)fp;
    *pos = f->pos;
    Log("fgetpos", f->pos, 0, 0, 0);
    return 0;
}

int BIGM_fsetpos(FILE* fp, const BIGM_fpos_t* pos)
{
    tBFile* f = (tBFile*)fp;
    f->pos = *pos;
    Log("fsetpos", *pos, 0, 0, 0);
    return 0;
}

FILE* BIGM_fopen(const char* path, const char* mode)
{
    Log("fopen", 0, 0, 0, 0);
    return (FILE*)&bfile[BFM_side];
}

int BIGM_fclose(FILE* fp)
{
    Log("fclose", 0, 0, 0, 0);
    return 0;
}

int _access(const char* path, int mode)
{
    Log("_access", mode, 0, 0, 0);
    return 0;
}

int _chmod(const char* path, int pmode)
{
    Log("_chmod", pmode, 0, 0, 0);
    return 0;
}

/* ---- the engine ---- */

void* MEM_p_Alloc(unsigned int size)
{
    int s = BFM_side;
    unsigned char* p;
    int idx;
    upUsed[s] = (upUsed[s] + 15u) & ~15u;
    p = arena[s] + upUsed[s];
    upUsed[s] += size + 4096u;
    if (upUsed[s] >= downUsed[s])
    {
        fprintf(stderr, "bigfiles mock arena exhausted\n");
        exit(2);
    }
    memset(p, BFM_POISON, size + 4096u);
    idx = Register(s, p, size);
    Log("MEM_p_Alloc", size, idx, 0, 0);
    return p;
}

void* MEM_p_AllocTmp(int size)
{
    int s = BFM_side;
    unsigned char* p;
    int idx;
    upUsed[s] = (upUsed[s] + 15u) & ~15u;
    p = arena[s] + upUsed[s];
    upUsed[s] += (unsigned int)size + 4096u;
    if (upUsed[s] >= downUsed[s])
    {
        fprintf(stderr, "bigfiles mock arena exhausted\n");
        exit(2);
    }
    memset(p, BFM_POISON, (size_t)size + 4096u);
    idx = Register(s, p, (unsigned int)size);
    Log("MEM_p_AllocTmp", size, idx, 0, 0);
    return p;
}

void* MEM_p_AllocFromEnd(int size)
{
    int s = BFM_side;
    unsigned char* p;
    int idx;
    unsigned int want = ((unsigned int)size + 15u) & ~15u;
    if (downUsed[s] < want + upUsed[s] + 65536u)
    {
        fprintf(stderr, "bigfiles mock end arena exhausted\n");
        exit(2);
    }
    downUsed[s] -= want;
    p = arena[s] + downUsed[s];
    memset(p, BFM_POISON, want);
    idx = Register(s, p, (unsigned int)size);
    Log("MEM_p_AllocFromEnd", size, idx, 0, 0);
    return p;
}

void MEM_FreeFromEnd(void* ptr)
{
    int s = BFM_side;
    Log("MEM_FreeFromEnd", BFM_Loc(ptr), 0, 0, 0);
    if (ptr && (unsigned char*)ptr == arena[s] + downUsed[s])
    {
        int i;
        for (i = blockCount[s] - 1; i >= 0; --i)
        {
            if (blockPtr[s][i] == (unsigned char*)ptr)
            {
                downUsed[s] += (blockSize[s][i] + 15u) & ~15u;
                break;
            }
        }
    }
}

void MEM_Free(void* ptr)
{
    Log("MEM_Free", BFM_Loc(ptr), 0, 0, 0);
}

void* MEM_p_Realloc(void* src, size_t size)
{
    void* p = MEM_p_Alloc((unsigned int)size);
    Log("MEM_p_Realloc", BFM_Loc(src), (long long)size, 0, 0);
    return p;
}

void PreLoadTex_Finished(int* ret)
{
    Log("PreLoadTex_Finished", 0, 0, 0, 0);
    *ret = 1;
}

void AI_EvalFunc_DbgTraceInt_C(void)
{
    Log("AI_EvalFunc_DbgTraceInt_C", 0, 0, 0, 0);
}

void DBG_assert(int isError, char* sourceCodeInfo, int lineNo, char* sourceCodeLine,
                const char* error, const char* extra, int showMsgBox)
{
    /* the message, not the line number: the line numbers moved with
       the rewrite and say nothing about behaviour */
    long long h = 0;
    const char* c;
    for (c = error; c && *c; ++c)
        h = h * 131 + (unsigned char)*c;
    Log("DBG_assert", h, extra ? 1 : 0, 0, 0);
    ++BFM_assertCount[BFM_side];
}

/* Reached only by the loader entry points, which this test does not
   drive: any call is a new dependency and fails the run rather than
   passing on a stand-in. */
static void Unreachable(const char* fn)
{
    fprintf(stderr, "bigfiles test: unexpected call to %s\n", fn);
    exit(3);
}

void LOA_InitModule(void) { Unreachable("LOA_InitModule"); }
void LOA_CloseModule(void) { Unreachable("LOA_CloseModule"); }
char* LOA_FetchBuffer(int size) { Unreachable("LOA_FetchBuffer"); return 0; }
char* LOA_FetchFile(int* sizeRet) { Unreachable("LOA_FetchFile"); return 0; }

/* The byte readers and the sorted key array are stand-ins, not the
   engine's: loading.c does not compile on this host and basic_array.c
   would bring its own state into the comparison.  Both sides call
   these same two, every call goes into the log with its arguments,
   and what is compared is the order and the count -- which is what
   the loader loops were rewritten in. */
int LOA_ReadLong(char** offset, uint32_t* retVal, char readMode)
{
    uint32_t v;
    memcpy(&v, *offset, 4);
    *offset += 4;
    if (retVal)
        *retVal = v;
    Log("LOA_ReadLong", (long long)(int)v, readMode, 0, 0);
    return (int)v;
}

char** LOA_ReadCharArray(char** offset, char* retVal, unsigned int len, char readMode)
{
    if (retVal != *offset)
        memcpy(retVal, *offset, len);
    *offset += len;
    Log("LOA_ReadCharArray", len, readMode, 0, 0);
    return offset;
}

void BAS_binit(BASStruct* bas, int elemNum) { Log("BAS_binit", elemNum, 0, 0, 0); }
void BAS_bfree(BASStruct* bas) { Log("BAS_bfree", 0, 0, 0, 0); }
void BAS_binsert(unsigned int key, int val, BASStruct* bas) { Log("BAS_binsert", key, val, 0, 0); }
int BAS_bsearch(int key, BASStruct* bas) { Log("BAS_bsearch", key, 0, 0, 0); return key & 0xFFFF; }
void BAS_bsort(BASStruct* bas) { Log("BAS_bsort", 0, 0, 0, 0); }
int lzo1x_decompress(const lzo_bytep src, lzo_uint srcLen, lzo_bytep dst,
                     lzo_uintp dstLen, lzo_voidp wrkmem)
{
    Unreachable("lzo1x_decompress");
    return 0;
}
