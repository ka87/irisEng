/* Everything proceduraltextures.c reaches outside itself, for the
   differential test.

   Each side gets its own arena, laid out by the same bump allocator, so
   the two can be compared byte for byte rather than only at the buffers
   the driver happens to know about. Three things make that work:

   - the arena is refilled with a fixed pattern before every case, so
     the file's several reads of memory it never wrote -- the fire
     palette above index 235, tTexProFire.unknum, tTexProcPhoto's gap --
     are a defined and identical value on both sides instead of noise;
   - nothing is ever handed back, and every block sits inside a larger
     one, so a copy that writes a little past its block is recorded
     rather than killing the process;
   - a word that is a pointer into the writer's own arena is compared
     relative to that arena, and the few function pointers the file
     stores are compared through a registered BASE/NEW pair. */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../common.h"
#include "../memory.h"
#include "../loading.h"
#include "../bigfiles.h"
#include "../textures.h"
/* the Bink types and entry points the frozen copy still uses; the
   engine itself decodes through libav now */
#include "bink.h"
#include "../time.h"
#include "../gdi.h"
#include "../soft.h"
#include "../lights.h"
#include "../inputoutput.h"
#include "../gameobject.h"
#include "proctex_mocks.h"

unsigned char MOCKP_arena[2][MOCKP_ARENA];
tMockProcCall MOCKP_log[2][MOCKP_LOG_MAX];
int MOCKP_logLen[2];
int MOCKP_overflow;

static int side;
static size_t arenaUsed[2];
static unsigned int rngState;

static const unsigned char* streamBase;
static int streamLen;
static int bigPos;
static unsigned int bigLen;
static char* photoData;
static int photoLen;

#define GUARD_BEFORE 64
#define GUARD_AFTER  4096

#define PAIR_MAX 16
static const void* pairBase[PAIR_MAX];
static const void* pairNew[PAIR_MAX];
static int pairLen;

static const void* rangeBase[PAIR_MAX];
static const void* rangeNew[PAIR_MAX];
static size_t rangeSize[PAIR_MAX];
static int rangeLen;

#define WORD_MAX 8
static int pendingWord[WORD_MAX];
static int pendingLen;
static int pendingPos;

void MOCKP_SetSide(int s) { side = s; pendingPos = 0; }
int MOCKP_Side(void) { return side; }

void MOCKP_AddPair(const void* b, const void* n)
{
    if (pairLen < PAIR_MAX)
    {
        pairBase[pairLen] = b;
        pairNew[pairLen] = n;
        ++pairLen;
    }
}

void MOCKP_AddRange(const void* b, const void* n, size_t size)
{
    if (rangeLen < PAIR_MAX)
    {
        rangeBase[rangeLen] = b;
        rangeNew[rangeLen] = n;
        rangeSize[rangeLen] = size;
        ++rangeLen;
    }
}

void MOCKP_PushWord(int v)
{
    if (pendingLen < WORD_MAX)
        pendingWord[pendingLen++] = v;
}

void MOCKP_Reset(void)
{
    int s;
    size_t i;

    pendingLen = 0;
    pendingPos = 0;

    for (s = 0; s < 2; ++s)
    {
        for (i = 0; i < MOCKP_ARENA; ++i)
            MOCKP_arena[s][i] = (unsigned char)(0xA5 ^ (i * 31 + (i >> 8) * 7));
        arenaUsed[s] = 0;
        MOCKP_logLen[s] = 0;
    }
    MOCKP_overflow = 0;
    MEM_gst_MemoryInfo.usedMemoryByTextures = 0;
}

void MOCKP_SeedRand(unsigned int seed)
{
    rngState = seed;
    srand(seed);
}

void MOCKP_SetStream(const unsigned char* data, int len)
{
    streamBase = data;
    streamLen = len;
}

void MOCKP_SetBigFile(int pos, unsigned int len)
{
    bigPos = pos;
    bigLen = len;
}

void MOCKP_SetPhoto(char* data, int len)
{
    photoData = data;
    photoLen = len;
}

void MOCKP_Record(const char* fn, long a, long b, long c)
{
    int n = MOCKP_logLen[side];

    if (n >= MOCKP_LOG_MAX)
    {
        MOCKP_overflow = 1;
        return;
    }
    MOCKP_log[side][n].fn = fn;
    MOCKP_log[side][n].a = a;
    MOCKP_log[side][n].b = b;
    MOCKP_log[side][n].c = c;
    MOCKP_logLen[side] = n + 1;
}

/* ---- allocator ---- */

void* MEM_p_Alloc(unsigned int size)
{
    size_t need = (size + 15u) & ~(size_t)15;
    unsigned char* p;

    MOCKP_Record("MEM_p_Alloc", (long)size, 0, 0);
    if (arenaUsed[side] + GUARD_BEFORE + need + GUARD_AFTER > MOCKP_ARENA)
    {
        MOCKP_overflow = 1;
        fprintf(stderr, "arena exhausted (%u)\n", size);
        exit(2);
    }
    p = MOCKP_arena[side] + arenaUsed[side] + GUARD_BEFORE;
    arenaUsed[side] += GUARD_BEFORE + need + GUARD_AFTER;
    MEM_gst_MemoryInfo.usedMemoryByTextures += size;
    return p;
}

void MEM_Free(void* ptr)
{
    MOCKP_Record("MEM_Free", ptr ? 1 : 0, 0, 0);
}

/* ---- random ---- */

int RND_JADErand(void)
{
    rngState = rngState * 1103515245u + 12345u;
    return (int)((rngState >> 16) & 0x7FFF);
}

/* ---- loader ---- */

static int streamPos(char** offset)
{
    return (int)((const unsigned char*)*offset - streamBase);
}

int LOA_ReadLong(char** offset, uint32_t* retVal, char readMode)
{
    int p = streamPos(offset);
    uint32_t v = 0;

    if (p >= 0 && p + 4 <= streamLen)
        memcpy(&v, streamBase + p, 4);
    *offset += 4;
    MOCKP_Record("LOA_ReadLong", p, (long)v, readMode);
    if (retVal)
        *retVal = v;
    return (int)v;
}

uint16_t LOA_ReadShort(char** offset, uint16_t* retVal, char readMode)
{
    int p = streamPos(offset);
    uint16_t v = 0;

    if (p >= 0 && p + 2 <= streamLen)
        memcpy(&v, streamBase + p, 2);
    *offset += 2;
    MOCKP_Record("LOA_ReadShort", p, (long)v, readMode);
    if (retVal)
        *retVal = v;
    return v;
}

char LOA_ReadChar(char** offset, char* retVal, char readMode)
{
    int p = streamPos(offset);
    char v = 0;

    if (p >= 0 && p + 1 <= streamLen)
        v = (char)streamBase[p];
    *offset += 1;
    MOCKP_Record("LOA_ReadChar", p, (long)v, readMode);
    if (retVal)
        *retVal = v;
    return v;
}

/* ---- big file ---- */

int BIG_ul_SearchKeyToPos(uint32_t key)
{
    MOCKP_Record("BIG_ul_SearchKeyToPos", (long)key, bigPos, 0);
    return bigPos;
}

unsigned int BIG_ul_GetLengthFile(int Offset)
{
    MOCKP_Record("BIG_ul_GetLengthFile", Offset, (long)bigLen, 0);
    return bigLen;
}

void BINK_SeekAndReadFromBF(int Offset, void* DstBuf, unsigned int MaxCharCount)
{
    unsigned int i;
    unsigned char* d = (unsigned char*)DstBuf;

    MOCKP_Record("BINK_SeekAndReadFromBF", Offset, (long)MaxCharCount, 0);
    if (MaxCharCount == 4 && pendingLen)
    {
        int v = pendingWord[pendingPos < pendingLen ? pendingPos : pendingLen - 1];
        ++pendingPos;
        memcpy(d, &v, 4);
        return;
    }
    for (i = 0; i < MaxCharCount; ++i)
        d[i] = (unsigned char)((Offset + (int)i) * 13 + 7);
}

/* ---- lights ---- */

unsigned int LIGHT_ul_Interpol2Colors(int color1, int color2, float factor)
{
    unsigned int a = (unsigned int)color1;
    unsigned int b = (unsigned int)color2;
    unsigned int out = 0;
    int shift;

    for (shift = 0; shift < 32; shift += 8)
    {
        unsigned int ca = (a >> shift) & 0xFF;
        unsigned int cb = (b >> shift) & 0xFF;
        unsigned int cc = (unsigned int)(ca + (cb - ca) * factor) & 0xFF;
        out |= cc << shift;
    }
    MOCKP_Record("LIGHT_ul_Interpol2Colors", (long)a, (long)b, (long)out);
    return out;
}

/* ---- save / jpeg / texture conversion ---- */

/* The caller hands this the address of a size_t and casts it to int*.
   On the 32-bit target those are the same width and the object is
   written whole; on a 64-bit host only half of it would be, and the
   other half is stack noise that differs between the two copies. The
   whole object is written here so the host behaves as the target does,
   and the mismatch itself is recorded in the notes rather than being
   measured by this test. */
char* SAV_PhotoGet(int a1, int32_t* a2)
{
    MOCKP_Record("SAV_PhotoGet", a1, photoLen, 0);
    if (a2)
    {
        size_t wide = (size_t)photoLen;
        memcpy(a2, &wide, sizeof(wide));
    }
    return photoData;
}

int TEX_l_File_LoadJpeg(char* data, struct sEngTexIntermediateStruct* tex, size_t size)
{
    MOCKP_Record("TEX_l_File_LoadJpeg", (long)size, 0, 0);
    tex->realWidth = 64;
    tex->realHeight = 32;
    tex->colorDepth1 = 24;
    tex->colorDepth2 = 24;
    tex->imgDataPtr = data;
    return 1;
}

int TEX_l_ConvertPF(struct sEngTexIntermediateStruct* src, struct sEngTexIntermediateStruct* dest)
{
    MOCKP_Record("TEX_l_ConvertPF", src->realWidth, dest->realWidth, dest->colorDepth1);
    return 1;
}

void TEX_l_ConvertSize_24To32(struct sEngTexIntermediateStruct* src, struct sEngTexIntermediateStruct* dest)
{
    MOCKP_Record("TEX_l_ConvertSize_24To32", src->realWidth, dest->realWidth, dest->realHeight);
}

void TEX_File_FreeDescription(struct sEngTexIntermediateStruct* tex)
{
    MOCKP_Record("TEX_File_FreeDescription", tex->realWidth, tex->realHeight, 0);
}

/* ---- game object / soft transform ---- */

static tGEOGRO* theGro;
static Mtx4x4 theAbsolute;

void MOCKP_SetGro(tGEOGRO* gro) { theGro = gro; }
Mtx4x4* MOCKP_AbsoluteMatrix(void) { return &theAbsolute; }

struct sGroGeneric* OBJ_p_GetGro(struct sGameObjectMain* a1)
{
    MOCKP_Record("OBJ_p_GetGro", a1 ? 1 : 0, 0, 0);
    return (struct sGroGeneric*)theGro;
}

Mtx4x4* OBJ_pst_GetAbsoluteMatrix(tGameObjectMain* a1)
{
    MOCKP_Record("OBJ_pst_GetAbsoluteMatrix", a1 ? 1 : 0, 0, 0);
    return &theAbsolute;
}

void MATH_SubstractVec(Vec3* dest, Vec3* src1, Vec3* src2)
{
    dest->x = src1->x - src2->x;
    dest->y = src1->y - src2->y;
    dest->z = src1->z - src2->z;
    MOCKP_Record("MATH_SubstractVec", (long)(dest->x * 1000), (long)(dest->y * 1000),
                 (long)(dest->z * 1000));
}

/* The first sixteen floats of Mtx4x4 are the matrix; the fields after
   them are flags the test never touches. */
#define MTX(p, r, c) (((float*)(p))[(r) * 4 + (c)])

void MATH_MulMatrixMatrix(Mtx4x4* dest, Mtx4x4* src1, Mtx4x4* src2)
{
    int r, c, k;

    MOCKP_Record("MATH_MulMatrixMatrix", 0, 0, 0);
    for (r = 0; r < 4; ++r)
        for (c = 0; c < 4; ++c)
        {
            float s = 0.0f;
            for (k = 0; k < 4; ++k)
                s += MTX(src1, r, k) * MTX(src2, k, c);
            MTX(dest, r, c) = s;
        }
}

void SOFT_TransformAndProject(Vec3* dstVecs, Vec3* srcVecs, int verticesNum, tDDCamStruct* cam)
{
    int i;

    MOCKP_Record("SOFT_TransformAndProject", verticesNum,
                 (long)(srcVecs->x * 100), (long)(srcVecs->y * 100));
    for (i = 0; i < verticesNum; ++i)
    {
        Mtx4x4* m = cam->viewMtxPtr_setInGRORender;
        float wx = srcVecs[i].x, wy = srcVecs[i].y, wz = srcVecs[i].z;
        dstVecs[i].x = MTX(m, 0, 0) * wx + MTX(m, 1, 0) * wy + MTX(m, 2, 0) * wz + MTX(m, 3, 0);
        dstVecs[i].y = MTX(m, 0, 1) * wx + MTX(m, 1, 1) * wy + MTX(m, 2, 1) * wz + MTX(m, 3, 1);
        dstVecs[i].z = MTX(m, 0, 2) * wx + MTX(m, 1, 2) * wy + MTX(m, 2, 2) * wz + MTX(m, 3, 2);
    }
}

/* ---- video.c, logged rather than run ----

   The rewrite decodes through libav. Linking it here would drag the
   real library into a test about this file's logic, so the module is
   mocked the same way the Bink SDK below it is: a handle that is not
   dereferenced, and a script of answers for the three questions the
   frame pacing asks. */

static int   vidOpened[2];
static int   vidWaitScript[2];
static int   vidWaitPos[2];
static int   vidDueScript[2];
static int   vidDuePos[2];
static int   vidFrames[2];      /* frames left before the stream ends */
static int   vidOpenFails[2];

void MOCKP_SetVideo(int s, int frames, int waitScript, int dueScript, int openFails)
{
    vidFrames[s] = frames;
    vidWaitScript[s] = waitScript;
    vidWaitPos[s] = 0;
    vidDueScript[s] = dueScript;
    vidDuePos[s] = 0;
    vidOpenFails[s] = openFails;
    vidOpened[s] = 0;
}

int MOCKP_VideoOpened(int s) { return vidOpened[s]; }

/* a handle the file only ever tests and passes back */
static int vidHandle[2];

tVideoStream* VID_pst_OpenReader(tVideoReadFunc read, void* user, int wantSound)
{
    MOCKP_Record("VID_pst_OpenReader", read ? 1 : 0, user ? 1 : 0, wantSound);
    if (vidOpenFails[side])
        return NULL;
    vidOpened[side] = 1;
    return (tVideoStream*)&vidHandle[side];
}

tVideoStream* VID_pst_OpenFile(const char* path, int wantSound)
{
    MOCKP_Record("VID_pst_OpenFile", path ? 1 : 0, wantSound, 0);
    return NULL;
}

void VID_Close(tVideoStream* video)
{
    MOCKP_Record("VID_Close", video ? 1 : 0, 0, 0);
    if (video)
        vidOpened[side] = 0;
}

int VID_b_Waiting(tVideoStream* video)
{
    int bit = (vidWaitScript[side] >> (vidWaitPos[side] & 31)) & 1;

    ++vidWaitPos[side];
    MOCKP_Record("VID_b_Waiting", bit, 0, 0);
    return bit;
}

int VID_b_DecodeFrame(tVideoStream* video)
{
    int got = vidFrames[side] > 0;

    if (got)
        --vidFrames[side];
    MOCKP_Record("VID_b_DecodeFrame", got, vidFrames[side], 0);
    return got;
}

int VID_b_NextAlsoDue(tVideoStream* video)
{
    int bit = (vidDueScript[side] >> (vidDuePos[side] & 31)) & 1;

    ++vidDuePos[side];
    MOCKP_Record("VID_b_NextAlsoDue", bit, 0, 0);
    return bit;
}

void VID_NextFrame(tVideoStream* video)
{
    MOCKP_Record("VID_NextFrame", 0, 0, 0);
}

void VID_CopyToBuffer(tVideoStream* video, void* dest, int pitch,
                      int destHeight, int withAlpha)
{
    MOCKP_Record("VID_CopyToBuffer", pitch, destHeight, withAlpha);
}

int VID_i_Width(const tVideoStream* video) { (void)video; return 64; }
int VID_i_Height(const tVideoStream* video) { (void)video; return 48; }
int VID_b_Ended(const tVideoStream* video) { return video ? 0 : 1; }

/* ---- the Bink SDK, logged rather than run ---- */

static BINK theBink[2];
static int binkWaitScript[2];
static int binkWaitPos[2];

void MOCKP_SetBink(int s, unsigned int frames, unsigned int frameNum, int waitScript)
{
    theBink[s].Frames = frames;
    theBink[s].FrameNum = frameNum;
    binkWaitScript[s] = waitScript;
    binkWaitPos[s] = 0;
}

HBINK MOCKP_BinkHandle(int s) { return &theBink[s]; }

HBINK BinkOpen(const char* name, uint32_t flags)
{
    MOCKP_Record("BinkOpen", (long)flags, 0, 0);
    return &theBink[side];
}

void BinkClose(HBINK bink)
{
    MOCKP_Record("BinkClose", bink ? 1 : 0, 0, 0);
}

int BinkWait(HBINK bink)
{
    int bit = (binkWaitScript[side] >> (binkWaitPos[side] & 31)) & 1;

    ++binkWaitPos[side];
    MOCKP_Record("BinkWait", bit, 0, 0);
    return bit;
}

int BinkDoFrame(HBINK bink)
{
    MOCKP_Record("BinkDoFrame", (long)bink->FrameNum, 0, 0);
    return 0;
}

void BinkNextFrame(HBINK bink)
{
    ++bink->FrameNum;
    MOCKP_Record("BinkNextFrame", (long)bink->FrameNum, 0, 0);
}

int BinkCopyToBuffer(HBINK bink, void* dest, int destpitch, uint32_t destheight,
                     uint32_t destx, uint32_t desty, uint32_t flags)
{
    MOCKP_Record("BinkCopyToBuffer", destpitch, (long)destheight, (long)flags);
    return 0;
}

int BinkSetSoundTrack(U32 total_tracks, U32 PTR4* tracks)
{
    MOCKP_Record("BinkSetSoundTrack", (long)total_tracks, tracks ? 1 : 0, 0);
    return 0;
}

void BinkSetMemory(BINKMEMALLOC alloc, BINKMEMFREE freefn)
{
    MOCKP_Record("BinkSetMemory", alloc ? 1 : 0, freefn ? 1 : 0, 0);
}

void BinkSetIOSize(U32 iosize)
{
    MOCKP_Record("BinkSetIOSize", (long)iosize, 0, 0);
}

void BinkSetIO(BINKIOOPEN io)
{
    MOCKP_Record("BinkSetIO", io ? 1 : 0, 0, 0);
}

/* ---- arena and log comparison ---- */

static int normalise(const unsigned char* p, int s, unsigned long long* out)
{
    unsigned long long w;
    unsigned long long base = (unsigned long long)(size_t)MOCKP_arena[s];
    int i;

    memcpy(&w, p, sizeof(w));
    if (w >= base && w < base + MOCKP_ARENA)
    {
        *out = 0x1000000000000000ull + (w - base);
        return 1;
    }
    for (i = 0; i < pairLen; ++i)
    {
        unsigned long long b = (unsigned long long)(size_t)(s ? pairNew[i] : pairBase[i]);
        if (w == b)
        {
            *out = 0x2000000000000000ull + (unsigned)i;
            return 1;
        }
    }
    for (i = 0; i < rangeLen; ++i)
    {
        unsigned long long b = (unsigned long long)(size_t)(s ? rangeNew[i] : rangeBase[i]);
        if (w >= b && w < b + rangeSize[i])
        {
            *out = 0x3000000000000000ull + (unsigned)i * 0x10000000ull + (w - b);
            return 1;
        }
    }
    *out = w;
    return 0;
}

int MOCKP_CompareArenas(void)
{
    size_t i;
    int diff = 0;

    for (i = 0; i + 8 <= MOCKP_ARENA; )
    {
        unsigned long long a, b;
        int na = normalise(&MOCKP_arena[0][i], 0, &a);
        int nb = normalise(&MOCKP_arena[1][i], 1, &b);

        if (na || nb)
        {
            if (a != b)
                diff += 8;
            i += 8;
            continue;
        }
        if (MOCKP_arena[0][i] != MOCKP_arena[1][i])
            ++diff;
        ++i;
    }
    for (; i < MOCKP_ARENA; ++i)
        if (MOCKP_arena[0][i] != MOCKP_arena[1][i])
            ++diff;
    return diff;
}

int MOCKP_CompareLogs(void)
{
    int n = MOCKP_logLen[0] < MOCKP_logLen[1] ? MOCKP_logLen[0] : MOCKP_logLen[1];
    int i;

    for (i = 0; i < n; ++i)
    {
        tMockProcCall* a = &MOCKP_log[0][i];
        tMockProcCall* b = &MOCKP_log[1][i];

        if (strcmp(a->fn, b->fn) != 0 || a->a != b->a || a->b != b->b || a->c != b->c)
            return i;
    }
    if (MOCKP_logLen[0] != MOCKP_logLen[1])
        return n;
    return -1;
}
