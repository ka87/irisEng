#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "engine_mocks.h"

void BASE_PROTEX_InitModifier(tProtexMdfStruct* protex);
void NEW_PROTEX_InitModifier(tProtexMdfStruct* protex);
void BASE_PROTEX_UpdateWaterEffect(tProtexMdfStruct* protex);
void NEW_PROTEX_UpdateWaterEffect(tProtexMdfStruct* protex);
void BASE_PROTEX_Apply2DMatrix(tProtexMdfStruct* protex, tMultiTextureElemStruct* elem);
void NEW_PROTEX_Apply2DMatrix(tProtexMdfStruct* protex, tMultiTextureElemStruct* elem);
void BASE_PROTEX_ComputeStUstVstP(tProtexMdfStruct* protex, tMultiTextureElemStruct* elem,
                                  tGameObjectMain* gao);
void NEW_PROTEX_ComputeStUstVstP(tProtexMdfStruct* protex, tMultiTextureElemStruct* elem,
                                 tGameObjectMain* gao);
void BASE_PROTEX_Disturb(struct sGameObjectMain* gao, struct sGEOGRO* geogro);
void NEW_PROTEX_Disturb(struct sGameObjectMain* gao, struct sGEOGRO* geogro);
void BASE_PROTEX_BeforeDraw(void);
void NEW_PROTEX_BeforeDraw(void);

#define DIM 16
#define CELLS (DIM * DIM)
#define VERTS 64
#define CHAIN 5

static int g_fails;
static int g_diffCount;
static double g_worstAbs;
static double g_worstRel;

static unsigned s_rngState;

static void rngSeed(unsigned seed)
{
    s_rngState = seed ? seed : 1u;
}

static unsigned rngNext(void)
{
    s_rngState = s_rngState * 1664525u + 1013904223u;
    return s_rngState >> 8;
}

static float rngFloat(float lo, float hi)
{
    return lo + (hi - lo) * (float)(rngNext() & 0xFFFF) / 65535.0f;
}

static int closeEnough(float a, float b, const char* what, int idx)
{
    double d, r, m;

    if (memcmp(&a, &b, sizeof(float)) == 0)
        return 1;
    d = fabs((double)a - (double)b);
    m = fabs((double)a) > fabs((double)b) ? fabs((double)a) : fabs((double)b);
    r = m > 0.0 ? d / m : d;
    g_diffCount++;
    if (d > g_worstAbs)
        g_worstAbs = d;
    if (r > g_worstRel)
        g_worstRel = r;
    if (r > 1e-5 && d > 1e-5)
    {
        printf("    DIFF %s[%d]: old %.9g  new %.9g  (rel %.3g)\n",
               what, idx, (double)a, (double)b, r);
        return 0;
    }
    return 1;
}

static int cmpVec(Vec3* a, Vec3* b, const char* what)
{
    int ok = 1;

    ok &= closeEnough(a->x, b->x, what, 0);
    ok &= closeEnough(a->y, b->y, what, 1);
    ok &= closeEnough(a->z, b->z, what, 2);
    return ok;
}

static int cmpShorts(const int16_t* a, const int16_t* b, int n, const char* what)
{
    int i;

    for (i = 0; i < n; i++)
    {
        if (a[i] != b[i])
        {
            printf("    DIFF %s[%d]: old %d new %d\n", what, i, a[i], b[i]);
            return 0;
        }
    }
    return 1;
}

static int cmpBytes(const uint8_t* a, const uint8_t* b, int n, const char* what)
{
    int i;

    for (i = 0; i < n; i++)
    {
        if (a[i] != b[i])
        {
            printf("    DIFF %s[%d]: old %u new %u\n", what, i, a[i], b[i]);
            return 0;
        }
    }
    return 1;
}

static int cmpLogs(tMockCall* la, int na, tMockCall* lb, int nb)
{
    int i, j, ok = 1;

    if (na != nb)
    {
        printf("    DIFF call count: old %d new %d\n", na, nb);
        for (i = 0; i < (na > nb ? na : nb); i++)
            printf("      %-28s | %s\n",
                   i < na ? la[i].fn : "-",
                   i < nb ? lb[i].fn : "-");
        return 0;
    }
    for (i = 0; i < na; i++)
    {
        if (strcmp(la[i].fn, lb[i].fn) != 0)
        {
            printf("    DIFF call %d: old %s new %s\n", i, la[i].fn, lb[i].fn);
            ok = 0;
            continue;
        }
        for (j = 0; j < la[i].nargs; j++)
            ok &= closeEnough(la[i].arg[j], lb[i].arg[j], la[i].fn, i);
    }
    return ok;
}

static void report(const char* name, int ok)
{
    if (ok)
    {
        printf("  ok   %s\n", name);
    }
    else
    {
        printf("  FAIL %s\n", name);
        g_fails++;
    }
}

/* --- the water surface ------------------------------------------- */

typedef struct sWater
{
    tProtexMdfStruct protex;
    uint8_t* block;
    float* ramp;
} tWater;

static int s_requestCount;
static void* s_requestParam;

static int mockRequest(int type, void* param)
{
    s_requestCount++;
    s_requestParam = param;
    MOCK_Record("DD.Request", 0, 1, (double)type);
    return 0;
}

static tGdi_DisplayData s_dd;

static void waterBuild(tWater* w, unsigned seed, int dim1, int dim2, int frames)
{
    int cells = dim1 * dim2;
    int16_t* cur;
    int16_t* prev;
    int i;

    memset(w, 0, sizeof(*w));
    w->block = (uint8_t*)calloc(1, (size_t)(5 * cells));
    w->ramp = (float*)calloc((size_t)cells, sizeof(float));
    cur = (int16_t*)w->block;
    prev = (int16_t*)(w->block + 2 * cells);

    rngSeed(seed);
    for (i = 0; i < cells; i++)
    {
        cur[i] = (int16_t)((int)(rngNext() % 0x3000) - 0x1800);
        prev[i] = (int16_t)((int)(rngNext() % 0x3000) - 0x1800);
    }

    w->protex.unkProbablyDim1 = dim1;
    w->protex.unkProbablyDim2 = dim2;
    w->protex.unkPtrFreedInUnapply = cur;
    w->protex.unkPtr2 = prev;
    w->protex.unkNumProtexWater = w->block + 4 * cells;
    w->protex.ptr3 = w->ramp;
    w->protex.flags = 0x40;
    w->protex.texElemIdx = 7;
    (void)frames;
}

static void runWater(const char* name, unsigned seed, int dim1, int dim2, int frames)
{
    tWater a, b;
    tMockCall logA[MOCK_LOG_MAX];
    tProtexMdfStruct ramped;
    int cells = dim1 * dim2;
    int nA, f, ok = 1;

    waterBuild(&a, seed, dim1, dim2, frames);
    waterBuild(&b, seed, dim1, dim2, frames);

    gpst_DX_stDD = &s_dd;
    s_dd.Request = mockRequest;

    /* each copy builds its own colour ramp, and the surface reads it back */
    BASE_PROTEX_InitModifier(&ramped);
    NEW_PROTEX_InitModifier(&ramped);

    MOCK_Reset();
    for (f = 0; f < frames; f++)
        BASE_PROTEX_UpdateWaterEffect(&a.protex);
    nA = MOCK_logLen;
    memcpy(logA, MOCK_log, sizeof(tMockCall) * (size_t)nA);

    MOCK_Reset();
    for (f = 0; f < frames; f++)
        NEW_PROTEX_UpdateWaterEffect(&b.protex);

    ok &= cmpLogs(logA, nA, MOCK_log, MOCK_logLen);
    ok &= cmpShorts(a.protex.unkPtrFreedInUnapply, b.protex.unkPtrFreedInUnapply,
                    cells, "cur");
    ok &= cmpShorts(a.protex.unkPtr2, b.protex.unkPtr2, cells, "prev");
    ok &= cmpBytes(a.protex.unkNumProtexWater, b.protex.unkNumProtexWater, cells, "bump");
    for (f = 0; f < cells; f++)
        ok &= closeEnough(a.ramp[f], b.ramp[f], "ptr3", f);

    if ((a.protex.unkPtrFreedInUnapply == (int16_t*)a.block)
        != (b.protex.unkPtrFreedInUnapply == (int16_t*)b.block))
    {
        printf("    DIFF buffers swapped differently\n");
        ok = 0;
    }

    report(name, ok);
    free(a.block);
    free(b.block);
    free(a.ramp);
    free(b.ramp);
}

/* --- the 2D UV matrix and the basis ------------------------------ */

typedef struct sBasisScene
{
    tProtexMdfStruct protex;
    tMultiTextureElemStruct elem;
    tGameObjectMain gao;
    Mtx4x4 mtx;
} tBasisScene;

static void basisBuild(tBasisScene* s, unsigned seed, int subFlags, int param)
{
    memset(s, 0, sizeof(*s));
    rngSeed(seed);

    s->protex.v1.x = rngFloat(-10.0f, 10.0f);
    s->protex.v1.y = rngFloat(-10.0f, 10.0f);
    s->protex.v1.z = rngFloat(-10.0f, 10.0f);
    s->protex.v2.x = rngFloat(-2.0f, 2.0f);
    s->protex.v2.y = rngFloat(-2.0f, 2.0f);
    s->protex.v2.z = rngFloat(-2.0f, 2.0f);
    s->protex.v3.x = rngFloat(-2.0f, 2.0f);
    s->protex.v3.y = rngFloat(-2.0f, 2.0f);
    s->protex.v3.z = rngFloat(-2.0f, 2.0f);

    s->mtx.v1.x = rngFloat(-1.0f, 1.0f);
    s->mtx.v1.y = rngFloat(-1.0f, 1.0f);
    s->mtx.v1.z = rngFloat(-1.0f, 1.0f);
    s->mtx.v2.x = rngFloat(-1.0f, 1.0f);
    s->mtx.v2.y = rngFloat(-1.0f, 1.0f);
    s->mtx.v2.z = rngFloat(-1.0f, 1.0f);
    s->mtx.v3.x = rngFloat(-1.0f, 1.0f);
    s->mtx.v3.y = rngFloat(-1.0f, 1.0f);
    s->mtx.v3.z = rngFloat(-1.0f, 1.0f);
    s->mtx.Pos.x = rngFloat(-50.0f, 50.0f);
    s->mtx.Pos.y = rngFloat(-50.0f, 50.0f);
    s->mtx.Pos.z = rngFloat(-50.0f, 50.0f);
    s->mtx.w = 1.0f;
    s->mtx.mtxflag = 2;

    s->gao.matrices = &s->mtx;
    s->elem.subTextureFlags = subFlags;
    s->elem.phongOrPlanarParam = (uint16_t)param;
    s->elem.texElemIdx = -1;
}

static void setUv(unsigned seed, int identity)
{
    rngSeed(seed ^ 0x5bd1u);
    MOCK_vuisIdentity = identity;
    MOCK_uv.f1 = rngFloat(0.2f, 3.0f);
    MOCK_uv.f2 = rngFloat(-2.0f, 2.0f);
    MOCK_uv.f3 = rngFloat(-2.0f, 2.0f);
    MOCK_uv.f4 = rngFloat(0.2f, 3.0f);
    MOCK_uv.speedU = rngFloat(-1.0f, 1.0f);
    MOCK_uv.speedV = rngFloat(-1.0f, 1.0f);
}

static void runApply2D(const char* name, unsigned seed, int identity)
{
    tBasisScene a, b;
    tMockCall logA[MOCK_LOG_MAX];
    int nA, ok = 1;

    basisBuild(&a, seed, 0x600000, 0);
    basisBuild(&b, seed, 0x600000, 0);

    setUv(seed, identity);
    MOCK_Reset();
    BASE_PROTEX_Apply2DMatrix(&a.protex, &a.elem);
    nA = MOCK_logLen;
    memcpy(logA, MOCK_log, sizeof(tMockCall) * (size_t)nA);

    setUv(seed, identity);
    MOCK_Reset();
    NEW_PROTEX_Apply2DMatrix(&b.protex, &b.elem);

    ok &= cmpLogs(logA, nA, MOCK_log, MOCK_logLen);
    ok &= cmpVec(&a.protex.v1, &b.protex.v1, "v1");
    ok &= cmpVec(&a.protex.v2, &b.protex.v2, "v2");
    ok &= cmpVec(&a.protex.v3, &b.protex.v3, "v3");
    report(name, ok);
}

static void runCompute(const char* name, unsigned seed, int subFlags, int param)
{
    tBasisScene a, b;
    tMockCall logA[MOCK_LOG_MAX];
    int nA, ok = 1;

    basisBuild(&a, seed, subFlags, param);
    basisBuild(&b, seed, subFlags, param);

    setUv(seed, 0);
    MOCK_Reset();
    BASE_PROTEX_ComputeStUstVstP(&a.protex, &a.elem, &a.gao);
    nA = MOCK_logLen;
    memcpy(logA, MOCK_log, sizeof(tMockCall) * (size_t)nA);

    setUv(seed, 0);
    MOCK_Reset();
    NEW_PROTEX_ComputeStUstVstP(&b.protex, &b.elem, &b.gao);

    ok &= cmpLogs(logA, nA, MOCK_log, MOCK_logLen);
    ok &= cmpVec(&a.protex.v1, &b.protex.v1, "v1");
    ok &= cmpVec(&a.protex.v2, &b.protex.v2, "v2");
    ok &= cmpVec(&a.protex.v3, &b.protex.v3, "v3");
    report(name, ok);
}

/* --- the stamping pass ------------------------------------------- */

typedef struct sDisturbScene
{
    tProtexMdfStruct protex[CHAIN];
    tGameObjectMain gao[CHAIN];
    tGameObjectMain target;
    Mtx4x4 mtx;
    tGEOGRO geogro;
    Vec3 verts[VERTS];
    int16_t cur[CHAIN][CELLS];
    int16_t prev[CHAIN][CELLS];
} tDisturbScene;

static void disturbBuild(tDisturbScene* s, unsigned seed, int targetFlags, int chainMask)
{
    int i, p;

    memset(s, 0, sizeof(*s));
    rngSeed(seed);

    s->mtx.v1.x = 1.0f;
    s->mtx.v2.y = 1.0f;
    s->mtx.v3.z = 1.0f;
    s->mtx.w = 1.0f;
    s->mtx.mtxflag = 0;

    s->target.matrices = &s->mtx;
    s->target.flags = targetFlags;

    s->geogro.vertexNum = VERTS;

    for (p = 0; p < CHAIN; p++)
    {
        s->gao[p].matrices = &s->mtx;
        s->protex[p].gao = &s->gao[p];
        s->protex[p].unkProbablyDim1 = DIM;
        s->protex[p].unkProbablyDim2 = DIM;
        s->protex[p].unkPtrFreedInUnapply = ((chainMask >> p) & 1) ? s->cur[p] : NULL;
        s->protex[p].unkPtr2 = s->prev[p];
        s->protex[p].prevProtex = (p + 1 < CHAIN) ? &s->protex[p + 1] : NULL;

        s->protex[p].v1.x = rngFloat(-5.0f, 5.0f);
        s->protex[p].v1.y = rngFloat(-5.0f, 5.0f);
        s->protex[p].v1.z = rngFloat(-5.0f, 5.0f);
        s->protex[p].v2.x = 2.0f;
        s->protex[p].v2.y = 0.0f;
        s->protex[p].v2.z = 0.0f;
        s->protex[p].v3.x = 0.0f;
        s->protex[p].v3.y = 3.0f;
        s->protex[p].v3.z = 0.0f;

        for (i = 0; i < CELLS; i++)
        {
            s->cur[p][i] = (int16_t)((int)(rngNext() % 0x1000) - 0x800);
            s->prev[p][i] = (int16_t)((int)(rngNext() % 0x1000) - 0x800);
        }
    }

    /* half the vertices sit inside the first protex's UV square, so the
       scan has hits to average; the rest fall outside it or off plane */
    for (i = 0; i < VERTS; i++)
    {
        float u = (i & 1) ? rngFloat(0.05f, 0.95f) : rngFloat(1.2f, 3.0f);
        float v = (i & 1) ? rngFloat(0.05f, 0.95f) : rngFloat(-2.0f, -0.2f);
        float off = (i % 8 == 3) ? rngFloat(0.5f, 2.0f) : rngFloat(-0.05f, 0.05f);

        s->verts[i].x = s->protex[0].v1.x + u * 2.0f;
        s->verts[i].y = s->protex[0].v1.y + v * 3.0f;
        s->verts[i].z = s->protex[0].v1.z + off;
    }
}

static void runDisturb(const char* name, unsigned seed, int targetFlags, int chainMask)
{
    tDisturbScene* a = (tDisturbScene*)malloc(sizeof(tDisturbScene));
    tDisturbScene* b = (tDisturbScene*)malloc(sizeof(tDisturbScene));
    tMockCall logA[MOCK_LOG_MAX];
    int nA, p, ok = 1;

    disturbBuild(a, seed, targetFlags, chainMask);
    disturbBuild(b, seed, targetFlags, chainMask);

    gpst_DX_stDD = &s_dd;

    s_dd.verticesCurrent = a->verts;
    p_FirstActiveProtex = &a->protex[0];
    MOCK_Reset();
    BASE_PROTEX_Disturb(&a->target, &a->geogro);
    nA = MOCK_logLen;
    memcpy(logA, MOCK_log, sizeof(tMockCall) * (size_t)nA);

    s_dd.verticesCurrent = b->verts;
    p_FirstActiveProtex = &b->protex[0];
    MOCK_Reset();
    NEW_PROTEX_Disturb(&b->target, &b->geogro);

    ok &= cmpLogs(logA, nA, MOCK_log, MOCK_logLen);
    for (p = 0; p < CHAIN; p++)
    {
        ok &= cmpShorts(a->cur[p], b->cur[p], CELLS, "cur");
        ok &= cmpShorts(a->prev[p], b->prev[p], CELLS, "prev");
    }
    p_FirstActiveProtex = NULL;
    report(name, ok);
    free(a);
    free(b);
}

/* --- the active list -------------------------------------------- */

typedef struct sListScene
{
    tProtexMdfStruct protex[CHAIN];
} tListScene;

static void listBuild(tListScene* s, int keepMask)
{
    int p;

    memset(s, 0, sizeof(*s));
    for (p = 0; p < CHAIN; p++)
    {
        s->protex[p].flags = 0x20 | (((keepMask >> p) & 1) ? 8 : 0);
        s->protex[p].prevProtex = (p + 1 < CHAIN) ? &s->protex[p + 1] : NULL;
    }
}

static int listOrder(tListScene* s, tProtexMdfStruct* head, int* out)
{
    tProtexMdfStruct* it;
    int n = 0, p;

    for (it = head; it && n < CHAIN; it = it->prevProtex)
    {
        out[n] = -1;
        for (p = 0; p < CHAIN; p++)
        {
            if (it == &s->protex[p])
                out[n] = p;
        }
        n++;
    }
    return n;
}

static void runBeforeDraw(const char* name, int keepMask)
{
    tListScene a, b;
    int orderA[CHAIN];
    int orderB[CHAIN];
    int nA, nB, p, ok = 1;

    listBuild(&a, keepMask);
    listBuild(&b, keepMask);

    p_FirstActiveProtex = &a.protex[0];
    MOCK_Reset();
    BASE_PROTEX_BeforeDraw();
    nA = listOrder(&a, p_FirstActiveProtex, orderA);

    p_FirstActiveProtex = &b.protex[0];
    MOCK_Reset();
    NEW_PROTEX_BeforeDraw();
    nB = listOrder(&b, p_FirstActiveProtex, orderB);

    if (nA != nB)
    {
        printf("    DIFF list length: old %d new %d\n", nA, nB);
        ok = 0;
    }
    else
    {
        for (p = 0; p < nA; p++)
        {
            if (orderA[p] != orderB[p])
            {
                printf("    DIFF list[%d]: old %d new %d\n", p, orderA[p], orderB[p]);
                ok = 0;
            }
        }
    }
    for (p = 0; p < CHAIN; p++)
    {
        if (a.protex[p].flags != b.protex[p].flags)
        {
            printf("    DIFF protex[%d].flags: old %#x new %#x\n",
                   p, a.protex[p].flags, b.protex[p].flags);
            ok = 0;
        }
    }
    p_FirstActiveProtex = NULL;
    report(name, ok);
}

int main(void)
{
    int mode, uvBit;
    char name[96];

    setvbuf(stdout, NULL, _IONBF, 0);
    printf("protexmodifier: old against new\n");

    runWater("water 16x16, one frame", 11u, 16, 16, 1);
    runWater("water 16x16, eight frames", 12u, 16, 16, 8);
    runWater("water 32x16, four frames", 13u, 32, 16, 4);
    runWater("water 8x8, four frames", 14u, 8, 8, 4);
    runWater("water 4x2, one frame", 15u, 4, 2, 1);
    runWater("water 2x4, one frame", 16u, 2, 4, 1);

    runApply2D("apply2D, real matrix", 21u, 0);
    runApply2D("apply2D, identity", 22u, 1);
    runApply2D("apply2D, real matrix, other seed", 23u, 0);

    runCompute("compute, wrong subtexture kind", 31u, 0x500000, 0);
    for (mode = 0; mode < 4; mode++)
    {
        for (uvBit = 0; uvBit < 3; uvBit++)
        {
            int param = (mode << 4) | (uvBit == 0 ? 0 : (uvBit == 1 ? 0x40 : 0xC0));

            sprintf(name, "compute, mode %d, uv bits %#x", mode, param & 0xC0);
            runCompute(name, 32u + (unsigned)(mode * 4 + uvBit), 0x600000, param);
        }
    }

    runDisturb("disturb, whole chain", 41u, 5, 0x1F);
    runDisturb("disturb, some without buffers", 42u, 5, 0x15);
    runDisturb("disturb, no buffers at all", 43u, 5, 0x00);
    runDisturb("disturb, target not drawable", 44u, 2, 0x1F);
    runDisturb("disturb, other seed", 45u, 1, 0x1F);

    runBeforeDraw("beforeDraw, all kept", 0x1F);
    runBeforeDraw("beforeDraw, head dropped", 0x1E);
    runBeforeDraw("beforeDraw, tail dropped", 0x0F);
    runBeforeDraw("beforeDraw, alternating", 0x15);
    runBeforeDraw("beforeDraw, none kept", 0x00);

    printf("%d of the compared floats differed at all; worst abs %.3g, worst rel %.3g\n",
           g_diffCount, g_worstAbs, g_worstRel);
    if (g_fails)
        printf("%d case(s) differed\n", g_fails);
    else
        printf("all cases equivalent\n");
    return g_fails ? 1 : 0;
}
