#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <math.h>

#include "snp_mocks.h"
#include "../memory.h"
#include "../loading.h"

tSnpMockCall SNPMOCK_log[SNPMOCK_LOG_MAX];
int SNPMOCK_logLen;
int SNPMOCK_logOverflow;

unsigned char SNPMOCK_arena[SNPMOCK_ARENA];
size_t SNPMOCK_arenaUsed;

tSnpScene SNPMOCK_scene;
tSnpScratch SNPMOCK_scratch;

int SNPMOCK_cases;
int SNPMOCK_failures;
long SNPMOCK_compares;

static const char* SNPMOCK_case = "?";
static int SNPMOCK_reported;

/* the saved side */
static unsigned char SNPMOCK_savedArena[SNPMOCK_ARENA];
static size_t SNPMOCK_savedArenaUsed;
static tSnpMockCall SNPMOCK_savedLog[SNPMOCK_LOG_MAX];
static int SNPMOCK_savedLogLen;
static tSnpScene SNPMOCK_savedScene;
static tSnpScratch SNPMOCK_savedScratch;

void SNPMOCK_SetCase(const char* name)
{
    SNPMOCK_case = name;
    SNPMOCK_reported = 0;
    SNPMOCK_cases++;
}

/* ---- the call log ---------------------------------------------------- */

static void SNPMOCK_Record(const char* fn, int ni, int nf, ...)
{
    va_list ap;
    int i;
    tSnpMockCall* c;

    if (SNPMOCK_logLen >= SNPMOCK_LOG_MAX)
    {
        SNPMOCK_logOverflow = 1;
        return;
    }
    c = &SNPMOCK_log[SNPMOCK_logLen++];
    memset(c, 0, sizeof(*c));
    c->fn = fn;
    c->ni = ni;
    c->nf = nf;
    va_start(ap, nf);
    for (i = 0; i < ni; i++)
        c->ia[i] = va_arg(ap, int);
    for (i = 0; i < nf; i++)
        c->fa[i] = (float)va_arg(ap, double);
    va_end(ap);
}

/* ---- the allocator --------------------------------------------------- */

#define SNPMOCK_GUARD 64

void* MEM_p_Alloc(unsigned int size)
{
    size_t off;
    void* p;

    off = (SNPMOCK_arenaUsed + 15) & ~(size_t)15;
    off += SNPMOCK_GUARD;
    if (off + size + SNPMOCK_GUARD > SNPMOCK_ARENA)
    {
        printf("  [%s] arena exhausted at %u bytes\n", SNPMOCK_case, size);
        exit(2);
    }
    p = &SNPMOCK_arena[off];
    SNPMOCK_arenaUsed = off + size + SNPMOCK_GUARD;
    SNPMOCK_Record("MEM_p_Alloc", 2, 0, (int)size, (int)off);
    return p;
}

void* MEM_p_Realloc(void* src, size_t size)
{
    void* p;
    int oldOff;

    oldOff = src ? (int)((unsigned char*)src - SNPMOCK_arena) : -1;
    SNPMOCK_Record("MEM_p_Realloc", 2, 0, oldOff, (int)size);
    p = MEM_p_Alloc((unsigned int)size);
    if (src)
        memcpy(p, src, size);
    return p;
}

void* MEM_p_AllocAligned(int size, int alignment)
{
    (void)alignment;
    return MEM_p_Alloc((unsigned int)size);
}

void* MEM_p_ReallocAligned(void* ptr, unsigned int size, int alignment)
{
    (void)alignment;
    return MEM_p_Realloc(ptr, size);
}

void MEM_FreeAlign(void* ptr)
{
    MEM_Free(ptr);
}

void MEM_Free(void* ptr)
{
    int off;

    off = ptr ? (int)((unsigned char*)ptr - SNPMOCK_arena) : -1;
    SNPMOCK_Record("MEM_Free", 1, 0, off);
}

/* ---- the engine around snp.c ----------------------------------------- */

int SNPMOCK_ObjectIndex(const tGameObjectMain* gao)
{
    int i;

    for (i = 0; i < SNPMOCK_OBJECTS; i++)
        if (&SNPMOCK_scene.gaos[i] == gao)
            return i;
    return -1;
}

Mtx4x4* SNPMOCK_GetAbsoluteMatrix(tGameObjectMain* gao)
{
    int i = SNPMOCK_ObjectIndex(gao);

    SNPMOCK_Record("OBJ_pst_GetAbsoluteMatrix", 1, 0, i);
    if (i < 0)
    {
        printf("  [%s] OBJ_pst_GetAbsoluteMatrix on an unknown object\n", SNPMOCK_case);
        exit(2);
    }
    return &SNPMOCK_scene.mtx[i];
}

BOOL SNPMOCK_BV_IsAABBox(tGameObjectBV* bv)
{
    int i;

    for (i = 0; i < SNPMOCK_OBJECTS; i++)
        if (&SNPMOCK_scene.bvs[i] == bv)
        {
            SNPMOCK_Record("OBJ_BV_IsAABBox", 2, 0, i, SNPMOCK_scene.isBox[i]);
            return SNPMOCK_scene.isBox[i];
        }
    SNPMOCK_Record("OBJ_BV_IsAABBox", 2, 0, -1, 0);
    return FALSE;
}

BOOL SNPMOCK_TableElemIsInvalid(void* elem)
{
    int i = SNPMOCK_ObjectIndex((const tGameObjectMain*)elem);

    SNPMOCK_Record("TAB_TableElemIsInvalid", 2, 0, i,
                   i >= 0 ? SNPMOCK_scene.invalid[i] : 1);
    return i >= 0 ? SNPMOCK_scene.invalid[i] : TRUE;
}

void SNPMOCK_TransformVertexNoScale(Vec3* dest, Mtx4x4* mtx, Vec3* src)
{
    MATH_TransformVertexNoScale(dest, mtx, src);
    SNPMOCK_Record("MATH_TransformVertexNoScale", 0, 3,
                   (double)dest->x, (double)dest->y, (double)dest->z);
}

/* ---- the scene ------------------------------------------------------- */

static unsigned int SNPMOCK_rngState;

static unsigned int SNPMOCK_Rand(void)
{
    SNPMOCK_rngState = SNPMOCK_rngState * 1103515245u + 12345u;
    return (SNPMOCK_rngState >> 16) & 0x7FFF;
}

static float SNPMOCK_RandRange(float lo, float hi)
{
    return lo + (hi - lo) * ((float)SNPMOCK_Rand() / 32767.0f);
}

void SNPMOCK_Init(void)
{
    MATH_InitModule();
}

void SNPMOCK_Reset(void)
{
    memset(SNPMOCK_arena, 0, SNPMOCK_arenaUsed + 256 < SNPMOCK_ARENA
                                 ? SNPMOCK_arenaUsed + 256 : SNPMOCK_ARENA);
    SNPMOCK_arenaUsed = 0;
    SNPMOCK_logLen = 0;
    SNPMOCK_logOverflow = 0;
    memset(&SNPMOCK_scratch, 0, sizeof(SNPMOCK_scratch));
}

void SNPMOCK_BuildScene(unsigned int seed, int pattern)
{
    int i;
    float angle;

    memset(&SNPMOCK_scene, 0, sizeof(SNPMOCK_scene));
    SNPMOCK_rngState = seed;

    for (i = 0; i < SNPMOCK_OBJECTS; i++)
    {
        tGameObjectMain* gao = &SNPMOCK_scene.gaos[i];
        float ex = SNPMOCK_RandRange(0.5f, 4.0f);
        float ey = SNPMOCK_RandRange(0.5f, 4.0f);
        float ez = SNPMOCK_RandRange(0.5f, 4.0f);

        {
            float span = ((pattern >> 19) & 1) ? 7.0f : 40.0f;

            SNPMOCK_scene.basePos[i].x = SNPMOCK_RandRange(-span, span);
            SNPMOCK_scene.basePos[i].y = SNPMOCK_RandRange(-span, span);
            SNPMOCK_scene.basePos[i].z = SNPMOCK_RandRange(-span, span);
        }

        angle = SNPMOCK_RandRange(0.0f, 6.2831f);
        memset(&SNPMOCK_scene.mtx[i], 0, sizeof(Mtx4x4));
        SNPMOCK_scene.mtx[i].v1.x = (float)cos((double)angle);
        SNPMOCK_scene.mtx[i].v1.z = (float)sin((double)angle);
        SNPMOCK_scene.mtx[i].v2.y = 1.0f;
        SNPMOCK_scene.mtx[i].v3.x = -(float)sin((double)angle);
        SNPMOCK_scene.mtx[i].v3.z = (float)cos((double)angle);
        SNPMOCK_scene.mtx[i].w = 1.0f;
        SNPMOCK_scene.mtx[i].mtxflag = 0x3C;
        SNPMOCK_scene.mtx[i].Pos = SNPMOCK_scene.basePos[i];

        SNPMOCK_scene.isBox[i] = ((pattern >> (i & 15)) & 1) ^ ((i & 1) == 0);
        if (SNPMOCK_scene.isBox[i])
        {
            SNPMOCK_scene.bvs[i].BVMin.x = -ex;
            SNPMOCK_scene.bvs[i].BVMin.y = -ey;
            SNPMOCK_scene.bvs[i].BVMin.z = -ez;
            SNPMOCK_scene.bvs[i].BVMax.x = ex;
            SNPMOCK_scene.bvs[i].BVMax.y = ey;
            SNPMOCK_scene.bvs[i].BVMax.z = ez;
        }
        else
        {
            /* a sphere: BVMin is the centre offset the nodes carry a
               pointer to, BVMax.x the radius */
            SNPMOCK_scene.bvs[i].BVMin.x = SNPMOCK_RandRange(-1.0f, 1.0f);
            SNPMOCK_scene.bvs[i].BVMin.y = SNPMOCK_RandRange(-1.0f, 1.0f);
            SNPMOCK_scene.bvs[i].BVMin.z = SNPMOCK_RandRange(-1.0f, 1.0f);
            SNPMOCK_scene.bvs[i].BVMax.x = ex;
        }

        gao->matrices = &SNPMOCK_scene.mtx[i];
        gao->boundingVolume = &SNPMOCK_scene.bvs[i];
        gao->snpIndex = 0xFFFF;
        gao->flags2 = 0x80;
        if ((pattern >> 16) & 1 ? (i % 4 != 3) : (i % 3 != 2))
            gao->flags2 |= 0x20;

        /* the three shapes of missing collision state snp.c guards for */
        SNPMOCK_scene.ext[i].coll = &SNPMOCK_scene.coll[i];
        SNPMOCK_scene.coll[i].detectionList = &SNPMOCK_scene.dlist[i];
        SNPMOCK_scene.dlist[i].detectionListGaos = SNPMOCK_scene.dgaos[i];
        SNPMOCK_scene.dlist[i].overlapNum = 0;
        gao->extended = &SNPMOCK_scene.ext[i];

        if (i == 5 && ((pattern >> 17) & 1))
            gao->extended = NULL;
        if (i == 7 && ((pattern >> 17) & 1))
            SNPMOCK_scene.ext[i].coll = NULL;
        if (i == 9 && ((pattern >> 17) & 1))
            SNPMOCK_scene.coll[i].detectionList = NULL;

        SNPMOCK_scene.invalid[i] = ((pattern >> 18) & 1) && (i % 5 == 4);
        SNPMOCK_scene.table[i].objectPtr = gao;
        SNPMOCK_scene.table[i].objectFlags = 0;
    }

    SNPMOCK_scene.tableDesc.firstElemPtr = SNPMOCK_scene.table;
    SNPMOCK_scene.tableDesc.nextElemPtr = &SNPMOCK_scene.table[SNPMOCK_OBJECTS];
    SNPMOCK_scene.world.snp = NULL;
    SNPMOCK_scene.world.SnPRefreshNeeded = 0;
}

void SNPMOCK_ChainKeys(double step, float extent)
{
    int i;
    float c[SNPMOCK_OBJECTS];

    c[SNPMOCK_OBJECTS - 1] = 3.5f;
    for (i = SNPMOCK_OBJECTS - 2; i >= 0; i--)
        c[i] = (float)((double)c[i + 1] + step);

    for (i = 0; i < SNPMOCK_OBJECTS; i++)
    {
        SNPMOCK_scene.isBox[i] = 1;
        SNPMOCK_scene.basePos[i].x = 0.0f;
        SNPMOCK_scene.basePos[i].y = 0.0f;
        SNPMOCK_scene.basePos[i].z = 0.0f;
        memset(&SNPMOCK_scene.mtx[i], 0, sizeof(Mtx4x4));
        SNPMOCK_scene.mtx[i].v1.x = 1.0f;
        SNPMOCK_scene.mtx[i].v2.y = 1.0f;
        SNPMOCK_scene.mtx[i].v3.z = 1.0f;
        SNPMOCK_scene.mtx[i].w = 1.0f;
        SNPMOCK_scene.mtx[i].mtxflag = 0x3C;

        SNPMOCK_scene.bvs[i].BVMin.x = c[i];
        SNPMOCK_scene.bvs[i].BVMin.y = c[i];
        SNPMOCK_scene.bvs[i].BVMin.z = c[i];
        SNPMOCK_scene.bvs[i].BVMax.x = c[i] + extent;
        SNPMOCK_scene.bvs[i].BVMax.y = c[i] + extent;
        SNPMOCK_scene.bvs[i].BVMax.z = c[i] + extent;
        SNPMOCK_scene.gaos[i].snpIndex = 0xFFFF;
    }
}

void SNPMOCK_MoveObjects(unsigned int seed, float amount)
{
    int i;

    SNPMOCK_rngState = seed;
    for (i = 0; i < SNPMOCK_OBJECTS; i++)
    {
        SNPMOCK_scene.mtx[i].Pos.x = SNPMOCK_scene.basePos[i].x
            + SNPMOCK_RandRange(-amount, amount);
        SNPMOCK_scene.mtx[i].Pos.y = SNPMOCK_scene.basePos[i].y
            + SNPMOCK_RandRange(-amount, amount);
        SNPMOCK_scene.mtx[i].Pos.z = SNPMOCK_scene.basePos[i].z
            + SNPMOCK_RandRange(-amount, amount);
    }
}

/* Every element of every axis table is given a valid object and a valid
   axis pointer before anything runs. The engine's element shuffles move
   a hard-coded 16 bytes per element, which is the 32-bit size of
   tSnpAxisElem and not the size this build has, so an element above
   currElemIdx can be read as part of a shuffle; leaving those zero
   would be a null object pointer rather than a comparison. Both sides
   get the same fill. */
void SNPMOCK_FillAxisTails(tSnp* snp)
{
    int axis;
    int i;

    for (axis = 0; axis < 3; axis++)
        for (i = 0; i < 2048; i++)
        {
            tSnpAxisElem* e = &snp->axisPtr[axis]->elems[i];

            e->gao = &SNPMOCK_scene.gaos[0];
            e->gaoSnPIndex = 1023;
            e->flags = 0x11;
            e->gap = 0;
            e->ptrToBVAxisFloat = &SNPMOCK_scene.bvs[0].BVMin.x;
            e->posPlusBVSetIn40DB10 = 0.0f;
        }
}

/* ---- comparison ------------------------------------------------------ */

void SNPMOCK_Save(void)
{
    SNPMOCK_savedArenaUsed = SNPMOCK_arenaUsed;
    memcpy(SNPMOCK_savedArena, SNPMOCK_arena, SNPMOCK_arenaUsed);
    SNPMOCK_savedLogLen = SNPMOCK_logLen;
    memcpy(SNPMOCK_savedLog, SNPMOCK_log, sizeof(tSnpMockCall) * SNPMOCK_logLen);
    SNPMOCK_savedScene = SNPMOCK_scene;
    SNPMOCK_savedScratch = SNPMOCK_scratch;
}

int SNPMOCK_SameInt(int base, int New, const char* what, const char* where)
{
    SNPMOCK_compares++;
    if (base == New)
        return 0;
    if (!SNPMOCK_reported)
    {
        SNPMOCK_reported = 1;
        SNPMOCK_failures++;
        printf("  [%s] %s %s: base %d new %d\n", SNPMOCK_case, where, what, base, New);
    }
    return 1;
}

int SNPMOCK_SameLong(long long base, long long New, const char* what, const char* where)
{
    SNPMOCK_compares++;
    if (base == New)
        return 0;
    if (!SNPMOCK_reported)
    {
        SNPMOCK_reported = 1;
        SNPMOCK_failures++;
        printf("  [%s] %s %s: base %lld new %lld\n", SNPMOCK_case, where, what, base, New);
    }
    return 1;
}

int SNPMOCK_SameFloat(float base, float New, const char* what, const char* where)
{
    SNPMOCK_compares++;
    if (memcmp(&base, &New, sizeof(float)) == 0)
        return 0;
    if (isnan(base) && isnan(New))
        return 0;
    if (!SNPMOCK_reported)
    {
        SNPMOCK_reported = 1;
        SNPMOCK_failures++;
        printf("  [%s] %s %s: base %.9g new %.9g\n", SNPMOCK_case, where, what,
               (double)base, (double)New);
    }
    return 1;
}

int SNPMOCK_SameBytes(const void* base, const void* New, size_t n,
                      const char* what, const char* where)
{
    size_t i;
    const unsigned char* a = (const unsigned char*)base;
    const unsigned char* b = (const unsigned char*)New;

    SNPMOCK_compares++;
    if (memcmp(a, b, n) == 0)
        return 0;
    for (i = 0; i < n; i++)
        if (a[i] != b[i])
            break;
    if (!SNPMOCK_reported)
    {
        SNPMOCK_reported = 1;
        SNPMOCK_failures++;
        printf("  [%s] %s %s differs at byte %lu of %lu: base %02x new %02x\n",
               SNPMOCK_case, where, what, (unsigned long)i, (unsigned long)n,
               a[i], b[i]);
    }
    return 1;
}

static int SNPMOCK_CompareLog(const char* where)
{
    int i;
    int j;

    if (SNPMOCK_logOverflow)
    {
        printf("  [%s] call log overflowed\n", SNPMOCK_case);
        SNPMOCK_failures++;
        return 1;
    }
    if (SNPMOCK_SameInt(SNPMOCK_savedLogLen, SNPMOCK_logLen, "call count", where))
    {
        for (i = 0; i < SNPMOCK_savedLogLen && i < SNPMOCK_logLen; i++)
            if (SNPMOCK_savedLog[i].fn != SNPMOCK_log[i].fn)
            {
                printf("      first differing call %d: base %s new %s\n", i,
                       SNPMOCK_savedLog[i].fn, SNPMOCK_log[i].fn);
                break;
            }
        return 1;
    }
    for (i = 0; i < SNPMOCK_logLen; i++)
    {
        const tSnpMockCall* a = &SNPMOCK_savedLog[i];
        const tSnpMockCall* b = &SNPMOCK_log[i];

        if (a->fn != b->fn)
        {
            if (!SNPMOCK_reported)
            {
                SNPMOCK_reported = 1;
                SNPMOCK_failures++;
                printf("  [%s] %s call %d: base %s new %s\n", SNPMOCK_case, where, i,
                       a->fn, b->fn);
            }
            return 1;
        }
        for (j = 0; j < a->ni; j++)
            if (SNPMOCK_SameInt(a->ia[j], b->ia[j], a->fn, where))
                return 1;
        for (j = 0; j < a->nf; j++)
            if (SNPMOCK_SameFloat(a->fa[j], b->fa[j], a->fn, where))
                return 1;
    }
    return 0;
}

int SNPMOCK_CompareWithSaved(const char* where)
{
    int bad = 0;

    bad |= SNPMOCK_CompareLog(where);
    bad |= SNPMOCK_SameLong((long long)SNPMOCK_savedArenaUsed,
                            (long long)SNPMOCK_arenaUsed, "arena used", where);
    if (SNPMOCK_savedArenaUsed == SNPMOCK_arenaUsed)
        bad |= SNPMOCK_SameBytes(SNPMOCK_savedArena, SNPMOCK_arena,
                                 SNPMOCK_arenaUsed, "arena", where);
    bad |= SNPMOCK_SameBytes(&SNPMOCK_savedScene, &SNPMOCK_scene,
                             sizeof(tSnpScene), "scene", where);
    bad |= SNPMOCK_SameBytes(&SNPMOCK_savedScratch, &SNPMOCK_scratch,
                             sizeof(tSnpScratch), "scratch", where);
    return bad;
}

void SNPMOCK_Summary(const char* title)
{
    printf("%s: %d cases, %ld comparisons, %d failing\n", title, SNPMOCK_cases,
           SNPMOCK_compares, SNPMOCK_failures);
}
