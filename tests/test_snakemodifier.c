#include <stdio.h>
#include <string.h>
#include <math.h>

#include "engine_mocks.h"

void BASE_GAO_ModifierSNAKE_Apply(struct sModifier* mdf, void* unk1);
void NEW_GAO_ModifierSNAKE_Apply(struct sModifier* mdf, void* unk1);
void BASE_GAO_ModifierSNAKE_UpdatePointers(tModifier* mdf);
void NEW_GAO_ModifierSNAKE_UpdatePointers(tModifier* mdf);
int BASE_GAO_ModifierSNAKE_Load(struct sModifier* mdf, char* data);
int NEW_GAO_ModifierSNAKE_Load(struct sModifier* mdf, char* data);

#define MAXG 8

typedef struct sScene
{
    tGameObjectMain gao[MAXG];
    tGameObjectMain* gaoPtr[MAXG];
    Mtx4x4 mtx[MAXG];
    tGaoMatrix gmtx[MAXG];
    tGameObjectBasic basic[MAXG];
    tSnakeMdfStruct snake;
    tModifier mdf;
} tScene;

static unsigned s_rngState;

static void rngSeed(unsigned seed)
{
    s_rngState = seed ? seed : 1u;
}

static float rngFloat(float lo, float hi)
{
    s_rngState = s_rngState * 1664525u + 1013904223u;
    return lo + (hi - lo) * (float)((s_rngState >> 8) & 0xFFFF) / 65535.0f;
}

static void fillMtx(Mtx4x4* m)
{
    m->v1.x = rngFloat(-1.0f, 1.0f);
    m->v1.y = rngFloat(-1.0f, 1.0f);
    m->v1.z = rngFloat(-1.0f, 1.0f);
    m->v2.x = rngFloat(-1.0f, 1.0f);
    m->v2.y = rngFloat(-1.0f, 1.0f);
    m->v2.z = rngFloat(-1.0f, 1.0f);
    m->v3.x = rngFloat(-1.0f, 1.0f);
    m->v3.y = rngFloat(-1.0f, 1.0f);
    m->v3.z = rngFloat(-1.0f, 1.0f);
    m->Pos.x = rngFloat(-400.0f, 400.0f);
    m->Pos.y = rngFloat(-400.0f, 400.0f);
    m->Pos.z = rngFloat(-400.0f, 400.0f);
    m->_m14 = rngFloat(0.5f, 2.0f);
    m->_m24 = rngFloat(0.5f, 2.0f);
    m->_m34 = rngFloat(0.5f, 2.0f);
    m->w = 1.0f;
    m->mtxflag = 0x8;
}

static void build(tScene* s, unsigned seed, int num, int flags, int holeAt)
{
    int i;

    memset(s, 0, sizeof(*s));
    rngSeed(seed);
    for (i = 0; i < MAXG; i++)
    {
        fillMtx(&s->mtx[i]);
        fillMtx(&s->gmtx[i].mtx);
        s->basic[i].mtx = &s->gmtx[i];
        s->gao[i].matrices = &s->mtx[i];
        s->gao[i].basic = &s->basic[i];
        s->gaoPtr[i] = &s->gao[i];
    }
    s->snake.num = num;
    s->snake.inertia = rngFloat(0.1f, 3.0f);
    s->snake.f2 = rngFloat(-5.0f, 60.0f);
    s->snake.f3 = rngFloat(-2.0f, 0.5f);
    for (i = 0; i < num && i < MAXG; i++)
    {
        s->snake.gaos[i] = (i == holeAt) ? NULL : &s->gao[i];
        s->snake.distances[i] = rngFloat(1.0f, 40.0f);
        s->snake.ranks[i] = i;
    }
    s->snake.gao = s->snake.gaos[0];
    s->mdf.mdfPrivateDataPtr = &s->snake;
    s->mdf.modifierFlags = flags;
}

static int idxIn(tScene* s, const void* p)
{
    int i;

    if (!p)
        return -1;
    for (i = 0; i < MAXG; i++)
    {
        if ((const void*)&s->gao[i] == p)
            return i;
    }
    return -2;
}

static int g_fails;
static double g_worstAbs;
static double g_worstRel;
static char g_worstWhat[64];
static int g_worstIdx;
static int g_diffCount;

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
    {
        g_worstRel = r;
        strncpy(g_worstWhat, what, sizeof(g_worstWhat) - 1);
        g_worstIdx = idx;
    }
    if (r > 1e-5 && d > 1e-5)
    {
        printf("    DIFF %s[%d]: old %.9g  new %.9g  (rel %.3g)\n",
               what, idx, (double)a, (double)b, r);
        return 0;
    }
    return 1;
}

static int cmpMtx(Mtx4x4* a, Mtx4x4* b, const char* what, int idx)
{
    int ok = 1;
    float* pa = (float*)a;
    float* pb = (float*)b;
    int i;

    for (i = 0; i < 20; i++)
        ok &= closeEnough(pa[i], pb[i], what, idx);
    if (a->mtxflag != b->mtxflag)
    {
        printf("    DIFF %s[%d].mtxflag: old %u new %u\n",
               what, idx, a->mtxflag, b->mtxflag);
        ok = 0;
    }
    return ok;
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
        if (strcmp(la[i].fn, lb[i].fn) != 0 || la[i].gao != lb[i].gao)
        {
            printf("    DIFF call %d: old %s(gao %d) new %s(gao %d)\n",
                   i, la[i].fn, la[i].gao, lb[i].fn, lb[i].gao);
            ok = 0;
            continue;
        }
        for (j = 0; j < la[i].nargs; j++)
            ok &= closeEnough(la[i].arg[j], lb[i].arg[j], la[i].fn, i);
    }
    return ok;
}

static void runApply(const char* name, unsigned seed, int num, int flags,
                     int holeAt, float dt)
{
    tScene a, b;
    tMockCall logA[MOCK_LOG_MAX];
    int nA, i, ok = 1;

    build(&a, seed, num, flags, holeAt);
    build(&b, seed, num, flags, holeAt);

    MOCK_RegisterGaos(a.gaoPtr, MAXG);
    MOCK_Reset();
    TIM_gf_dt = dt;
    BASE_GAO_ModifierSNAKE_Apply(&a.mdf, NULL);
    nA = MOCK_logLen;
    memcpy(logA, MOCK_log, sizeof(tMockCall) * (size_t)nA);

    MOCK_RegisterGaos(b.gaoPtr, MAXG);
    MOCK_Reset();
    TIM_gf_dt = dt;
    NEW_GAO_ModifierSNAKE_Apply(&b.mdf, NULL);

    ok &= cmpLogs(logA, nA, MOCK_log, MOCK_logLen);
    for (i = 0; i < MAXG; i++)
    {
        ok &= cmpMtx(&a.mtx[i], &b.mtx[i], "mtx", i);
        ok &= cmpMtx(&a.gmtx[i].mtx, &b.gmtx[i].mtx, "gaoMtx", i);
    }
    for (i = 0; i < MAXG; i++)
        ok &= closeEnough(a.snake.distances[i], b.snake.distances[i],
                          "distances", i);
    printf("  %-44s %s  (%d calls)\n", name, ok ? "ok" : "FAIL", nA);
    if (!ok)
        g_fails++;
}

static void runUpdatePointers(const char* name, unsigned seed, int num,
                              int ranksAvailable, int haveGroup)
{
    tScene a, b;
    tMockCall logA[MOCK_LOG_MAX];
    int nA, i, ok = 1;
    int dummyGroup = 1;

    build(&a, seed, num, 0, -1);
    build(&b, seed, num, 0, -1);

    MOCK_RegisterGaos(a.gaoPtr, MAXG);
    MOCK_SetGroupRanks(a.gaoPtr, ranksAvailable);
    MOCK_SetCurGroup(haveGroup ? &dummyGroup : NULL);
    MOCK_Reset();
    BASE_GAO_ModifierSNAKE_UpdatePointers(&a.mdf);
    nA = MOCK_logLen;
    memcpy(logA, MOCK_log, sizeof(tMockCall) * (size_t)nA);

    MOCK_RegisterGaos(b.gaoPtr, MAXG);
    MOCK_SetGroupRanks(b.gaoPtr, ranksAvailable);
    MOCK_SetCurGroup(haveGroup ? &dummyGroup : NULL);
    MOCK_Reset();
    NEW_GAO_ModifierSNAKE_UpdatePointers(&b.mdf);

    ok &= cmpLogs(logA, nA, MOCK_log, MOCK_logLen);
    for (i = 0; i < 20; i++)
        ok &= closeEnough(a.snake.distances[i], b.snake.distances[i],
                          "distances", i);
    for (i = 0; i < 20; i++)
    {
        if (idxIn(&a, a.snake.gaos[i]) != idxIn(&b, b.snake.gaos[i]))
        {
            printf("    DIFF gaos[%d]\n", i);
            ok = 0;
        }
    }
    if (idxIn(&a, a.snake.gao) != idxIn(&b, b.snake.gao))
    {
        printf("    DIFF snake->gao: old %d new %d\n",
               idxIn(&a, a.snake.gao), idxIn(&b, b.snake.gao));
        ok = 0;
    }
    printf("  %-44s %s  (%d calls)\n", name, ok ? "ok" : "FAIL", nA);
    if (!ok)
        g_fails++;
}

static void runLoad(const char* name, unsigned seed)
{
    tScene a, b;
    unsigned char stream[512];
    int i, ok = 1, ra, rb;

    rngSeed(seed);
    for (i = 0; i < (int)sizeof(stream); i++)
        stream[i] = (unsigned char)(int)rngFloat(0.0f, 255.0f);

    build(&a, seed, 4, 0, -1);
    build(&b, seed, 4, 0, -1);

    MOCK_Reset();
    ra = BASE_GAO_ModifierSNAKE_Load(&a.mdf, (char*)stream);
    MOCK_Reset();
    rb = NEW_GAO_ModifierSNAKE_Load(&b.mdf, (char*)stream);

    if (ra != rb)
    {
        printf("    DIFF bytes consumed: old %d new %d\n", ra, rb);
        ok = 0;
    }
    if (a.snake.num != b.snake.num)
    {
        printf("    DIFF num: old %d new %d\n", a.snake.num, b.snake.num);
        ok = 0;
    }
    ok &= closeEnough(a.snake.inertia, b.snake.inertia, "inertia", 0);
    ok &= closeEnough(a.snake.f2, b.snake.f2, "f2", 0);
    ok &= closeEnough(a.snake.f3, b.snake.f3, "f3", 0);
    for (i = 0; i < 20; i++)
    {
        if (a.snake.ranks[i] != b.snake.ranks[i])
        {
            printf("    DIFF ranks[%d]: old %d new %d\n",
                   i, a.snake.ranks[i], b.snake.ranks[i]);
            ok = 0;
        }
    }
    printf("  %-44s %s  (%d bytes)\n", name, ok ? "ok" : "FAIL", ra);
    if (!ok)
        g_fails++;
}

int main(void)
{
    unsigned seed;
    char nm[96];

    printf("snakemodifier: pre-refactor against rewritten\n\nApply\n");
    for (seed = 1; seed <= 12; seed++)
    {
        sprintf(nm, "seed %u, 5 links, sight mode", seed);
        runApply(nm, seed, 5, 1, -1, 0.016f);
        sprintf(nm, "seed %u, 5 links, banking mode", seed);
        runApply(nm, seed, 5, 0, -1, 0.016f);
    }
    runApply("8 links, banking", 101, 8, 0, -1, 0.016f);
    runApply("2 links, minimum chain", 102, 2, 0, -1, 0.016f);
    runApply("1 link, loop must not run", 103, 1, 0, -1, 0.016f);
    runApply("0 links", 104, 0, 0, -1, 0.016f);
    runApply("hole at link 3, break path", 105, 6, 0, 3, 0.016f);
    runApply("hole at link 1, immediate break", 106, 6, 0, 1, 0.016f);
    runApply("dt zero, fallback to 0.02", 107, 5, 0, -1, 0.0f);
    runApply("dt large, weight clamps high", 108, 5, 0, -1, 1.0f);
    runApply("dt tiny, weight clamps low", 109, 5, 0, -1, 1e-6f);
    runApply("sight mode, 7 links", 110, 7, 1, -1, 0.033f);

    printf("\nUpdatePointers\n");
    for (seed = 1; seed <= 6; seed++)
    {
        sprintf(nm, "seed %u, 5 links, all ranks resolve", seed);
        runUpdatePointers(nm, seed, 5, 8, 1);
    }
    runUpdatePointers("rank 3 missing, early return", 201, 6, 3, 1);
    runUpdatePointers("rank 0 missing, immediate return", 202, 6, 0, 1);
    runUpdatePointers("no group", 203, 5, 8, 0);
    runUpdatePointers("num 1, below minimum", 204, 1, 8, 1);
    runUpdatePointers("num 0", 205, 0, 8, 1);
    runUpdatePointers("num 2, minimum", 206, 2, 8, 1);

    printf("\nLoad\n");
    for (seed = 1; seed <= 4; seed++)
    {
        sprintf(nm, "seed %u, 512-byte stream", seed);
        runLoad(nm, seed);
    }

    printf("\n%d of the compared floats differed at all; worst abs %.3g, "
           "worst rel %.3g at %s[%d]\n",
           g_diffCount, g_worstAbs, g_worstRel, g_worstWhat, g_worstIdx);
    printf("%s\n", g_fails ? "FAILURES" : "all equivalent");
    return g_fails ? 1 : 0;
}
