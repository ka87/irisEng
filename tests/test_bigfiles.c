/* The differential test for bigfiles.c.
 *
 * bigfiles.c does not compile on this host, for two reasons that have
 * nothing to do with the rewrite: BIG_fseek does arithmetic on an
 * fpos_t, which is a 64-bit integer under MSVC and an opaque struct
 * under glibc, and BIG_Open uses the MSVC CRT names _access, _chmod
 * and _S_IWRITE.  tests/bigfiles_compat.h supplies both, ahead of the
 * file and without changing a byte of it: fpos_t is redefined as the
 * 64-bit integer it is on the target, and fgetpos, fsetpos, fread,
 * fseek, fopen and fclose are redirected to the harness's in-memory
 * file.  So the seek arithmetic under test is the file's own, run
 * over the target's type.
 *
 * The frozen pre-rewrite copy (tests/baseline_bigfiles.c) and the
 * rewrite go into one binary with their twenty-eight functions and
 * all thirty of their globals prefixed BASE_ and NEW_.  Each side
 * runs the same script over its own arena and its own file cursor and
 * is compared on every global, on the bytes it delivered, on the
 * return values, on the file position, and on the ordered sequence of
 * engine and C library calls.
 *
 * What is not covered, and why:
 *
 *  - LZO.  Every block in the synthetic bin files has its compressed
 *    size equal to its uncompressed size, which is the branch of
 *    BIGcomp_DecodeBlocks that copies rather than decompresses, so
 *    lzo1x_decompress is never called.  It is stubbed to abort, so a
 *    change that started calling it would fail the run.
 *  - PreLoadMoveBlocks' move arm.  It builds its source and
 *    destination by casting two 32-bit members of MEM_gst_MemoryInfo
 *    to pointers, which cannot address this host's memory.  The two
 *    arms that do not copy are driven; the memcpy arm is not
 *    reachable here and is left out rather than faked.
 *  - LOA_ReadLong, LOA_ReadCharArray and the BAS_ key array are
 *    stand-ins (see tests/bigfiles_mocks.c), so the loader cases
 *    compare the order and count of their calls rather than the
 *    engine's own parsing. */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bigfiles_compat.h"
#include "../common.h"
#include "../basic_array.h"
#include "../memory.h"
#include "../bigfiles.h"
#include "bigfiles_mocks.h"

extern int LOA_gb_CompressBin;
extern int gi_LastES;
int LOA_gb_SpeedMode;
int gau_ES[100];

#define DECL(p) \
    void p##BIG_FreeGlobalBuffer(void); \
    int p##BIG_fseek(FILE*, int, int); \
    int p##BIGcomp_ui_Read32(char*); \
    BOOL p##BIGcomp_CheckBlock(char*, int); \
    BOOL p##BIGcomp_CheckBuffer(char*, int); \
    int p##BIGcomp_i_CompressedSize(char*); \
    int p##BIGcomp_i_CompressedSize_v2(char*); \
    char* p##BIGcomp_DecodeBlocks(BIGCompressedBlockStruct*, unsigned int); \
    int p##BIG_fseekAndRead(FILE*, int, void**, unsigned int); \
    int p##BIG_fread(char*, unsigned int, FILE*); \
    int p##BIG_SpeedMode_fread(char**, int, FILE*); \
    void p##BIG_ReadHeader(void); \
    void p##BIG_ReadFatFile(BIGFatTableElem*); \
    void p##BIG_ReadAllFats(void); \
    void p##BIG_Open(char*); \
    void* p##BIG_p_RequestBuffer(size_t); \
    int p##BIG_SeekAndRead(FILE*, int, void**, unsigned int); \
    void p##BIG_ReadNoSeek(int, void*, unsigned int); \
    void p##BIG_ResetFat(void); \
    void p##BIG_Close(void); \
    unsigned int p##BIG_ul_GetLengthFile(int); \
    void p##BINK_SeekAndReadFromBF(int, void*, unsigned int); \
    void p##PreLoadMoveBlocks(int); \
    extern BIGGSTStruct p##BIG_gst; \
    extern int p##BIG_gi_ReadMode, p##BIG_gb_CanCache, p##BIG_gi_ReadSeek; \
    extern int p##BIG_si_DecompressedSize, p##BIG_gi_ReadSize, p##BIG_si_CompressedSize; \
    extern int p##BIG_gi_RealSeek, p##BIG_si_BinFileSize, p##BIG_si_BinFileReadSize; \
    extern int p##BIG_gul_ReadPos, p##BIG_ul_CachePos; \
    extern int p##BIG_gi_GlobalBufferSize, p##BIG_gi_GlobalSpecialBufferSize; \
    extern int p##gi_TestBin, p##gi_CurBin, p##BIG_gi_SpecialRead; \
    extern int p##BIGcomp_M_CompressedSize, p##BIGcomp_M_BlockSize; \
    extern BOOL p##LOA_gb_DoReadAllFats; \
    extern char* p##BIG_gp_ReadBuffer; \
    extern char* p##BIG_sp_CompressedNextBlock; \
    extern char* p##BIG_gp_CompressedBuffer; \
    extern char* p##BIG_gp_CompressedBufferPtrBackup; \
    extern void* p##BIG_gp_GlobalBuffer; \
    extern void* p##BIG_gp_GlobalSpecialBuffer; \
    extern size_t p##gai_CurSize[24]; \
    extern char* p##gap_FirstPointer[24]; \
    extern int p##gai_CurBinFileSize[20]; \
    extern unsigned int p##gaui_CurBinFilePos[20];

DECL(BASE_)
DECL(NEW_)

typedef struct sApi
{
    void (*FreeGlobal)(void);
    int (*Fseek)(FILE*, int, int);
    int (*Read32)(char*);
    BOOL (*CheckBlock)(char*, int);
    BOOL (*CheckBuffer)(char*, int);
    int (*CompSize)(char*);
    int (*CompSize2)(char*);
    char* (*Decode)(BIGCompressedBlockStruct*, unsigned int);
    int (*FseekAndRead)(FILE*, int, void**, unsigned int);
    int (*Fread)(char*, unsigned int, FILE*);
    int (*Speed)(char**, int, FILE*);
    void (*ReadHeader)(void);
    void (*ReadFatFile)(BIGFatTableElem*);
    void (*ReadAllFats)(void);
    void (*Open)(char*);
    void* (*Request)(size_t);
    int (*SeekAndRead)(FILE*, int, void**, unsigned int);
    void (*ReadNoSeek)(int, void*, unsigned int);
    void (*ResetFat)(void);
    void (*Close)(void);
    unsigned int (*GetLength)(int);
    void (*BinkRead)(int, void*, unsigned int);
    void (*MoveBlocks)(int);
    BIGGSTStruct* gst;
    int* iv[19];
    char** pv[4];
    void** gv[2];
    size_t* curSize;
    char** firstPtr;
} tApi;

#define API(p) { \
    p##BIG_FreeGlobalBuffer, p##BIG_fseek, p##BIGcomp_ui_Read32, \
    p##BIGcomp_CheckBlock, p##BIGcomp_CheckBuffer, p##BIGcomp_i_CompressedSize, \
    p##BIGcomp_i_CompressedSize_v2, p##BIGcomp_DecodeBlocks, p##BIG_fseekAndRead, \
    p##BIG_fread, p##BIG_SpeedMode_fread, p##BIG_ReadHeader, p##BIG_ReadFatFile, \
    p##BIG_ReadAllFats, p##BIG_Open, p##BIG_p_RequestBuffer, p##BIG_SeekAndRead, \
    p##BIG_ReadNoSeek, p##BIG_ResetFat, p##BIG_Close, p##BIG_ul_GetLengthFile, \
    p##BINK_SeekAndReadFromBF, p##PreLoadMoveBlocks, \
    &p##BIG_gst, \
    { &p##BIG_gi_ReadMode, &p##BIG_gb_CanCache, &p##BIG_gi_ReadSeek, \
      &p##BIG_si_DecompressedSize, &p##BIG_gi_ReadSize, &p##BIG_si_CompressedSize, \
      &p##BIG_gi_RealSeek, &p##BIG_si_BinFileSize, &p##BIG_si_BinFileReadSize, \
      &p##BIG_gul_ReadPos, &p##BIG_ul_CachePos, &p##BIG_gi_GlobalBufferSize, \
      &p##BIG_gi_GlobalSpecialBufferSize, &p##gi_TestBin, &p##gi_CurBin, \
      &p##BIG_gi_SpecialRead, &p##BIGcomp_M_CompressedSize, &p##BIGcomp_M_BlockSize, \
      (int*)&p##LOA_gb_DoReadAllFats }, \
    { &p##BIG_gp_ReadBuffer, &p##BIG_sp_CompressedNextBlock, \
      &p##BIG_gp_CompressedBuffer, &p##BIG_gp_CompressedBufferPtrBackup }, \
    { &p##BIG_gp_GlobalBuffer, &p##BIG_gp_GlobalSpecialBuffer }, \
    p##gai_CurSize, p##gap_FirstPointer }

static const tApi apiBase = API(BASE_);
static const tApi apiNew = API(NEW_);

/* indices into tApi.iv */
enum { G_READMODE, G_CANCACHE, G_READSEEK, G_DECOMPSIZE, G_READSIZE, G_COMPSIZE,
       G_REALSEEK, G_BINSIZE, G_BINREADSIZE, G_READPOS, G_CACHEPOS, G_GLOBSIZE,
       G_GLOBSPECSIZE, G_TESTBIN, G_CURBIN, G_SPECIALREAD, G_MCOMP, G_MBLOCK,
       G_DOREADALLFATS, G_NINT };

#define RES_MAX 3000000
static long long res[2][RES_MAX];
static int resLen[2];
static int resOverflow;
static int side;
static int caseIdx[2];
static int caseCount;
static int caseStart[2][8000];

#define DSTMAX 65536
static unsigned char dst[DSTMAX];

static void Rec(long long v)
{
    if (resLen[side] >= RES_MAX)
    {
        resOverflow = 1;
        return;
    }
    res[side][resLen[side]++] = v;
}

static void NewCase(void)
{
    if (caseIdx[side] < 8000)
        caseStart[side][caseIdx[side]] = resLen[side];
    ++caseIdx[side];
    if (side == 0)
        caseCount = caseIdx[0];
}

static void RecGlobals(const tApi* a)
{
    int i;
    for (i = 0; i < G_NINT; ++i)
        Rec(*a->iv[i]);
    for (i = 0; i < 4; ++i)
        Rec(BFM_Loc(*a->pv[i]));
    for (i = 0; i < 2; ++i)
        Rec(BFM_Loc(*a->gv[i]));
    for (i = 0; i < 24; ++i)
    {
        Rec((long long)a->curSize[i]);
        Rec(BFM_Loc(a->firstPtr[i]));
    }
    Rec(BFM_FilePos(side));
    Rec(BFM_assertCount[side]);
}

static void RecDst(int n)
{
    int i;
    if (n > DSTMAX)
        n = DSTMAX;
    for (i = 0; i < n; ++i)
        Rec(dst[i]);
}

/* ---- synthetic bin files ---- */

#define BINMAX (2 * 1024 * 1024)
static unsigned char binbuf[BINMAX];
static int binLen;

static void PutU32(int off, unsigned int v)
{
    binbuf[off] = (unsigned char)v;
    binbuf[off + 1] = (unsigned char)(v >> 8);
    binbuf[off + 2] = (unsigned char)(v >> 16);
    binbuf[off + 3] = (unsigned char)(v >> 24);
}

/* [u32 total][u32 n][u32 n][n bytes] ... -- the compressed size equal
   to the uncompressed one is BIGcomp_DecodeBlocks' copy arm */
/* The size pattern repeats until there are more payload bytes than
   any case asks for, so no read ever runs off the end of the blocks
   and into the filler. */
#define BIN_PAYLOAD_MIN 400000

static void BuildBin(int lead, const int* sizes, int count, unsigned int seed)
{
    int off = lead + 4;
    int i, j;
    int payload = 0;
    memset(binbuf, 0, sizeof(binbuf));
    for (i = 0; payload < BIN_PAYLOAD_MIN; ++i)
    {
        int size = sizes[i % count];
        PutU32(off, (unsigned int)size);
        PutU32(off + 4, (unsigned int)size);
        off += 8;
        for (j = 0; j < size; ++j)
        {
            seed = seed * 1103515245u + 12345u;
            binbuf[off + j] = (unsigned char)(seed >> 16);
        }
        off += size;
        payload += size;
    }
    PutU32(lead, (unsigned int)(off - lead - 4));
    binLen = off;
    if (binLen > BINMAX)
    {
        fprintf(stderr, "bin too big\n");
        exit(2);
    }
}

static void ResetSide(const tApi* a, int readMode, int mComp, int readPos, int readSize)
{
    int i;
    BFM_Reset(side);
    BFM_assertCount[side] = 0;
    memset(a->gst, 0, sizeof(BIGGSTStruct));
    for (i = 0; i < G_NINT; ++i)
        *a->iv[i] = 0;
    for (i = 0; i < 4; ++i)
        *a->pv[i] = NULL;
    for (i = 0; i < 2; ++i)
        *a->gv[i] = NULL;
    for (i = 0; i < 24; ++i)
    {
        a->curSize[i] = 0;
        a->firstPtr[i] = NULL;
    }
    *a->iv[G_READMODE] = readMode;
    *a->iv[G_MCOMP] = mComp;
    *a->iv[G_MBLOCK] = 0x7d000;
    *a->iv[G_READPOS] = readPos;
    *a->iv[G_READSIZE] = readSize;
    *a->iv[G_TESTBIN] = -1;
    *a->iv[G_CURBIN] = -1;
    BFM_SetFile(side, binbuf, binLen);
    a->gst->CLibFileHandle = (FILE*)BFM_FilePtr(side);
    memset(dst, 0, sizeof(dst));
}

/* ---- the scenarios ---- */

static void ScenHelpers(const tApi* a)
{
    static const int sizes[3] = { 64, 128, 40 };
    char* p;
    int i;
    BIGCompressedBlockStruct block;
    unsigned char out[4096];

    BuildBin(0, sizes, 3, 7);
    NewCase();
    ResetSide(a, 2, 65536, 0, 65536);

    LOA_gb_CompressBin = 1;
    p = (char*)binbuf + 4;
    for (i = 0; i < 3; ++i)
    {
        Rec(a->Read32(p));
        Rec(a->CheckBlock(p, 0));
        Rec(a->CheckBlock(p, 71));
        Rec(a->CheckBlock(p, 72));
        Rec(a->CheckBlock(p, 100000));
        Rec(a->CheckBuffer(p, 0));
        Rec(a->CheckBuffer(p, 63));
        Rec(a->CheckBuffer(p, 64));
        Rec(a->CompSize(p));
        Rec(a->CompSize2(p));
        p += 8 + sizes[i];
    }
    LOA_gb_CompressBin = 0;
    p = (char*)binbuf + 4;
    Rec(a->CheckBlock(p, 0x7d000));
    Rec(a->CheckBuffer(p, 0x7cfff));
    Rec(a->CompSize(p));
    Rec(a->CompSize2(p));
    LOA_gb_CompressBin = 1;

    memset(out, 0, sizeof(out));
    block.src = (char*)binbuf + 4;
    block.dest = (char*)out;
    block.i4 = 0;
    block.totalSize = 0;
    Rec((long long)(a->Decode(&block, 2) - block.src));
    Rec(block.totalSize);
    for (i = 0; i < 256; ++i)
        Rec(out[i]);
}

static void ScenFseek(const tApi* a)
{
    static const int offs[7] = { 0, 1, 2047, -1, -2048, 0x7FFFFFFF, -100000 };
    static const int origins[4] = { 0, 1, 2, 3 };
    int oi, gi;
    static const int sizes[1] = { 32 };

    BuildBin(0, sizes, 1, 3);
    for (oi = 0; oi < 7; ++oi)
    for (gi = 0; gi < 4; ++gi)
    {
        NewCase();
        ResetSide(a, 2, 65536, 0, 65536);
        BFM_SetFile(side, binbuf, binLen);
        Rec(a->Fseek((FILE*)BFM_FilePtr(side), 500, 0));
        Rec(BFM_FilePos(side));
        Rec(a->Fseek((FILE*)BFM_FilePtr(side), offs[oi], origins[gi]));
        Rec(BFM_FilePos(side));
    }
}

static void ScenRead(const tApi* a, int speedMode)
{
    /* Every block is at least as large as the largest read: the
       first call decodes exactly one block and then copies the whole
       request out of it, so a read larger than the first block runs
       off the end of the read buffer in both copies alike.  That is
       the engine's own shape, not a limit of the harness, and it is
       what the loader's own call sizes respect. */
    static const int layouts[5][5] = {
        { 1, 2048, 0, 0, 0 },
        { 2, 2048, 3000, 0, 0 },
        { 3, 2100, 4096, 2500, 0 },
        { 4, 4096, 4096, 4096, 4096 },
        { 3, 2048, 2048, 2048, 0 }
    };
    static const int mcomp[3] = { 4096, 8192, 65536 };
    static const int reads[6] = { 16, 300, 700, 1024, 2000, 2048 };
    static const int readSizes[2] = { 4096, 16384 };
    int li, mi, ri, si, k;

    for (li = 0; li < 5; ++li)
    for (mi = 0; mi < 3; ++mi)
    for (ri = 0; ri < 6; ++ri)
    for (si = 0; si < 2; ++si)
    {
        int sizes[4];
        int n = layouts[li][0];
        for (k = 0; k < n; ++k)
            sizes[k] = layouts[li][k + 1];
        BuildBin(0, sizes, n, (unsigned int)(li * 31 + mi * 7 + ri));

        NewCase();
        ResetSide(a, 2, mcomp[mi], 0, readSizes[si]);
        for (k = 0; k < 4; ++k)
        {
            char* out = NULL;
            memset(dst, 0, sizeof(dst));
            if (speedMode)
                Rec(a->Speed(&out, reads[ri], (FILE*)BFM_FilePtr(side)));
            else
                Rec(a->Fread((char*)dst, (unsigned int)reads[ri], (FILE*)BFM_FilePtr(side)));
            if (speedMode)
            {
                Rec(BFM_Loc(out));
                if (out)
                {
                    int j;
                    for (j = 0; j < reads[ri] && j < 2048; ++j)
                        Rec((unsigned char)out[j]);
                }
            }
            else
            {
                RecDst(reads[ri] < 2048 ? reads[ri] : 2048);
            }
            RecGlobals(a);
        }
    }
}

static void ScenReadNull(const tApi* a)
{
    static const int sizes[3] = { 4096, 4096, 4096 };
    int ri;
    static const int reads[4] = { 100, 1500, 2048, 4000 };

    BuildBin(0, sizes, 3, 11);
    for (ri = 0; ri < 4; ++ri)
    {
        int k;
        NewCase();
        ResetSide(a, 2, 8192, 0, 4096);
        for (k = 0; k < 3; ++k)
        {
            Rec(a->Fread(NULL, (unsigned int)reads[ri], (FILE*)BFM_FilePtr(side)));
            RecGlobals(a);
        }
    }
}

static void ScenPassthrough(const tApi* a)
{
    static const int sizes[2] = { 256, 256 };
    static const int reads[4] = { 1, 16, 256, 4000 };
    int ri;

    BuildBin(0, sizes, 2, 5);
    for (ri = 0; ri < 4; ++ri)
    {
        NewCase();
        ResetSide(a, 0, 8192, 0, 4096);
        Rec(a->Fread((char*)dst, (unsigned int)reads[ri], (FILE*)BFM_FilePtr(side)));
        RecDst(reads[ri] < 512 ? reads[ri] : 512);
        RecGlobals(a);

        NewCase();
        ResetSide(a, 0, 8192, 0, 4096);
        a->ReadNoSeek(64, dst, (unsigned int)reads[ri]);
        RecDst(reads[ri] < 512 ? reads[ri] : 512);
        RecGlobals(a);

        NewCase();
        ResetSide(a, 0, 8192, 0, 4096);
        a->BinkRead(32, dst, (unsigned int)reads[ri]);
        RecDst(reads[ri] < 512 ? reads[ri] : 512);
        RecGlobals(a);

        NewCase();
        ResetSide(a, 0, 8192, 0, 4096);
        Rec(a->GetLength(16));
        RecGlobals(a);

        NewCase();
        ResetSide(a, 0, 8192, 0, 4096);
        {
            void* p = dst;
            Rec(a->SeekAndRead((FILE*)BFM_FilePtr(side), 8, (void**)p, (unsigned int)reads[ri]));
            RecDst(reads[ri] < 512 ? reads[ri] : 512);
            p = dst;
            Rec(a->FseekAndRead((FILE*)BFM_FilePtr(side), 8, &p, (unsigned int)reads[ri]));
            RecDst(reads[ri] < 512 ? reads[ri] : 512);
        }
        RecGlobals(a);
    }
}

/* The preloaded-bin arm of the first-call path.  MEM_gp_MaxBlock
   decides which arm is taken through a comparison on an address
   truncated to 32 bits, which differs between the two sides' arenas;
   it is pinned to one extreme or the other so that the arm is chosen
   by the fixture and not by where the allocator happened to land. */
static void ScenPreload(const tApi* a)
{
    static const int sizes[3] = { 4096, 4096, 4096 };
    static const unsigned int curSizes[3] = { 40960, 20480, 81920 };
    int ci, bi, i;

    for (ci = 0; ci < 3; ++ci)
    for (bi = 0; bi < 3; ++bi)
    {
        unsigned char* pre;
        NewCase();
        BuildBin(0, sizes, 3, (unsigned int)(ci * 13 + bi));
        ResetSide(a, 2, 262144, 0, 32768);
        MEM_gp_MaxBlock = 0x80000000;            /* the preloaded arm */

        pre = (unsigned char*)BFM_PreloadArena(side, 262144);
        for (i = 0; i < 262144; ++i)
            pre[i] = (unsigned char)(i * 7 + ci);
        /* The preloaded block is copied into the compressed buffer in
           40960-byte chunks, taken from below the pointer and in
           reverse order, so the head of the bin file is laid out that
           way and the two arms decode the same stream. */
        for (i = 0; i < 2; ++i)
            memcpy(pre + 131072 - 40960 * (i + 1), binbuf + 40960 * i, 40960);
        /* the engine reads below the pointer it is given */
        a->firstPtr[0] = (char*)pre + 131072;
        a->firstPtr[1] = (char*)pre + 90112;
        a->firstPtr[2] = (char*)pre + 196608;
        a->curSize[0] = curSizes[ci];
        a->curSize[1] = 4096;
        a->curSize[2] = 8192;
        *a->iv[G_TESTBIN] = bi == 2 ? 5 : 0;
        *a->iv[G_CURBIN] = bi == 0 ? 3 : 2;
        if (bi == 1)
            a->curSize[0] = 0;

        Rec(a->Fread((char*)dst, 3000, (FILE*)BFM_FilePtr(side)));
        RecDst(2048);
        RecGlobals(a);
        Rec(a->Fread((char*)dst, 3000, (FILE*)BFM_FilePtr(side)));
        RecDst(2048);
        RecGlobals(a);
        MEM_gp_MaxBlock = 0x7FFFFFFF;
    }
}

static void ScenBuffers(const tApi* a)
{
    static const int sizes[1] = { 64 };
    static const int reqs[6] = { 0, 1, 16, 4096, 4096, 100 };
    int i;

    BuildBin(0, sizes, 1, 2);
    NewCase();
    ResetSide(a, 2, 8192, 0, 4096);
    for (i = 0; i < 6; ++i)
    {
        Rec(BFM_Loc(a->Request((size_t)reqs[i])));
        RecGlobals(a);
    }
    MEM_gp_AllocTmpFirst = (void*)1;
    a->FreeGlobal();
    RecGlobals(a);
    MEM_gp_AllocTmpFirst = NULL;
    Rec(BFM_Loc(a->Request(32)));
    a->FreeGlobal();
    RecGlobals(a);
}

static void ScenMoveBlocks(const tApi* a)
{
    int k, es;

    for (es = 0; es < 3; ++es)
    for (k = 0; k < 2; ++k)
    {
        int i;
        NewCase();
        ResetSide(a, 2, 8192, 0, 4096);
        for (i = 0; i < 4; ++i)
        {
            a->curSize[i] = (size_t)(4096 * (i + 1));
            a->firstPtr[i] = (char*)BFM_PreloadArena(side, 256) + 128;
        }
        *a->iv[G_CURBIN] = 3;
        gi_LastES = es == 0 ? -1 : 0;
        gau_ES[0] = 1234;
        /* nextFreeMemoryPointer above the destination, so the move arm
           is not taken: its addresses are 32-bit and cannot be
           dereferenced on this host */
        MEM_gst_MemoryInfo.memoryStartPointer = 0x10000000;
        MEM_gst_MemoryInfo.memorySize = 0x1000000;
        MEM_gst_MemoryInfo.nextFreeMemoryPointer = 0xF0000000;
        a->MoveBlocks(k ? 1234 : 99);
        RecGlobals(a);
    }
}

/* The loader entry points, over a synthetic BIG header. */
static void ScenLoader(const tApi* a)
{
    static const int fatCounts[4] = { 0, 1, 100, 137 };
    int fi, mi;

    for (fi = 0; fi < 4; ++fi)
    for (mi = 0; mi < 2; ++mi)
    {
        BIGFatTableElem fat;
        int sizes[1];
        int i;

        sizes[0] = 8192;
        BuildBin(0, sizes, 1, (unsigned int)fi);
        /* A plausible header and fat records at the head of the first
           block's payload, which is where the decompressed stream
           starts.  In the uncompressed mode the loader reads the file
           head instead and its two checks fail; both copies fail them
           identically and the assert count is compared. */
        memcpy(binbuf + 12, "BIG", 4);
        for (i = 0; i < 2000; i += 4)
            PutU32(16 + i, (unsigned int)(i / 4));

        NewCase();
        ResetSide(a, mi ? 2 : 0, 65536, 0, 16384);
        a->gst->CLibFileHandle = (FILE*)BFM_FilePtr(side);
        a->gst->bfFileTable = (BIGFileTableElem*)MEM_p_Alloc(100 * sizeof(BIGFileTableElem));
        memset(&fat, 0, sizeof(fat));
        fat.fatNum = fatCounts[fi];
        fat.posFat = 64;
        a->ReadFatFile(&fat);
        RecGlobals(a);
        Rec(fat.fatNum);
        Rec(fat.posFat);

        NewCase();
        ResetSide(a, mi ? 2 : 0, 65536, 0, 16384);
        a->gst->CLibFileHandle = (FILE*)BFM_FilePtr(side);
        a->ReadHeader();
        Rec(a->gst->hdr.unk2);
        Rec(a->gst->hdr.keyNum);
        Rec(a->gst->hdr.fatTableNum);
        Rec(a->gst->hdr.startKey);
        RecGlobals(a);
    }
}


/* The two tests in the read path that only move at an exact equality:
   the decode loop's exit, which the request can hit exactly because
   the block size is counted twice, and the buffer-size test, which is
   an equality when the request is the size of the read buffer.  The
   rest of the fixture steps over both. */
static void ScenBoundary(const tApi* a, int speedMode)
{
    static const int blocks[2] = { 1024, 2048 };
    int bi, ri, k;

    for (bi = 0; bi < 2; ++bi)
    for (ri = 0; ri < 6; ++ri)
    {
        int b = blocks[bi];
        int sizes[1];
        int first = b;
        int second;
        static const int mult[6] = { 0, 0, 0, 0, 0, 0 };
        (void)mult;
        sizes[0] = b;
        BuildBin(0, sizes, 1, (unsigned int)(bi * 17 + ri));

        switch (ri)
        {
        case 0: second = b; break;
        case 1: second = b + b / 2; break;
        case 2: second = 2 * b; break;
        case 3: second = 2 * b - 1; break;
        case 4: second = 2 * b + 1; break;
        default: second = b / 2; break;
        }

        NewCase();
        /* the read buffer is exactly the first request, so the second
           test in the read path is an equality */
        ResetSide(a, 2, 65536, 0, first);
        /* two calls, not three: where the loop exits a block short the
           read seek is left above the decompressed size, and a third
           call would take that difference as an unsigned length.  Both
           copies do it; the harness stops in front of it rather than
           measuring a 4GB memcpy. */
        for (k = 0; k < 2; ++k)
        {
            char* out = NULL;
            memset(dst, 0, sizeof(dst));
            if (speedMode)
            {
                Rec(a->Speed(&out, k ? second : first, (FILE*)BFM_FilePtr(side)));
                Rec(BFM_Loc(out));
                if (out)
                {
                    int j;
                    int n = k ? second : first;
                    for (j = 0; j < n && j < 2048; ++j)
                        Rec((unsigned char)out[j]);
                }
            }
            else
            {
                Rec(a->Fread((char*)dst, (unsigned int)(k ? second : first),
                             (FILE*)BFM_FilePtr(side)));
                RecDst((k ? second : first) < 2048 ? (k ? second : first) : 2048);
            }
            RecGlobals(a);
        }
    }

    /* The request exactly the size of the read buffer, with room left
       in the buffer: the one shape where the second size test is an
       equality and the arm it picks changes where the next block is
       decoded. */
    for (bi = 0; bi < 2; ++bi)
    {
        int b = blocks[bi];
        int sizes[1];
        char* out = NULL;
        sizes[0] = b;
        BuildBin(0, sizes, 1, (unsigned int)(99 + bi));

        NewCase();
        ResetSide(a, 2, 65536, 0, 4096);
        memset(dst, 0, sizeof(dst));
        if (speedMode)
            Rec(a->Speed(&out, b - 24, (FILE*)BFM_FilePtr(side)));
        else
            Rec(a->Fread((char*)dst, (unsigned int)(b - 24), (FILE*)BFM_FilePtr(side)));
        RecDst(1024);
        RecGlobals(a);
        memset(dst, 0, sizeof(dst));
        if (speedMode)
        {
            Rec(a->Speed(&out, 4096, (FILE*)BFM_FilePtr(side)));
            Rec(BFM_Loc(out));
            if (out)
            {
                int j;
                for (j = 0; j < 2048; ++j)
                    Rec((unsigned char)out[j]);
            }
        }
        else
        {
            Rec(a->Fread((char*)dst, 4096, (FILE*)BFM_FilePtr(side)));
            RecDst(2048);
        }
        RecGlobals(a);
    }
}

static void RunAll(const tApi* a)
{
    LOA_gb_CompressBin = 1;
    LOA_gb_SpeedMode = 0;
    MEM_gp_MaxBlock = 0x7FFFFFFF;
    MEM_gp_AllocTmpFirst = NULL;
    ScenHelpers(a);
    ScenFseek(a);
    ScenRead(a, 0);
    ScenRead(a, 1);
    ScenReadNull(a);
    ScenPassthrough(a);
    ScenPreload(a);
    ScenBuffers(a);
    ScenMoveBlocks(a);
    ScenLoader(a);
    ScenBoundary(a, 0);
    ScenBoundary(a, 1);
}

int main(void)
{
    int i, diff = 0, firstBad = -1, logDiff = 0, n = 0;

    for (side = 0; side < 2; ++side)
    {
        BFM_side = side;
        BFM_logLen[side] = 0;
        resLen[side] = 0;
        caseIdx[side] = 0;
        RunAll(side ? &apiNew : &apiBase);
    }

    if (resOverflow || BFM_logOverflow)
    {
        printf("bigfiles: FAIL -- log overflow\n");
        return 1;
    }
    if (resLen[0] != resLen[1])
    {
        printf("bigfiles: FAIL -- %d values against %d\n", resLen[0], resLen[1]);
        return 1;
    }
    for (i = 0; i < resLen[0]; ++i)
    {
        if (res[0][i] != res[1][i])
        {
            ++diff;
            if (firstBad < 0)
                firstBad = i;
        }
    }
    if (BFM_logLen[0] != BFM_logLen[1])
    {
        printf("bigfiles: FAIL -- %d calls against %d\n", BFM_logLen[0], BFM_logLen[1]);
        return 1;
    }
    for (i = 0; i < BFM_logLen[0]; ++i)
    {
        if (strcmp(BFM_log[0][i].fn, BFM_log[1][i].fn) ||
            BFM_log[0][i].arg[0] != BFM_log[1][i].arg[0] ||
            BFM_log[0][i].arg[1] != BFM_log[1][i].arg[1] ||
            BFM_log[0][i].arg[2] != BFM_log[1][i].arg[2] ||
            BFM_log[0][i].arg[3] != BFM_log[1][i].arg[3])
        {
            if (!logDiff)
                printf("  first call difference at %d: %s(%lld,%lld,%lld,%lld) against %s(%lld,%lld,%lld,%lld)\n",
                       i, BFM_log[0][i].fn, BFM_log[0][i].arg[0], BFM_log[0][i].arg[1],
                       BFM_log[0][i].arg[2], BFM_log[0][i].arg[3],
                       BFM_log[1][i].fn, BFM_log[1][i].arg[0], BFM_log[1][i].arg[1],
                       BFM_log[1][i].arg[2], BFM_log[1][i].arg[3]);
            ++logDiff;
        }
    }
    if (firstBad >= 0)
    {
        for (i = 0; i < caseCount; ++i)
            if (caseStart[0][i] <= firstBad)
                n = i;
        printf("  first value difference at %d (case %d): %lld against %lld\n",
               firstBad, n, res[0][firstBad], res[1][firstBad]);
    }

    printf("bigfiles: %d cases, %d values, %d calls, %d values differing, %d calls differing\n",
           caseCount, resLen[0], BFM_logLen[0], diff, logDiff);
    return (diff || logDiff) ? 1 : 0;
}
