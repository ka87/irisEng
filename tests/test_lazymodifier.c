#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "engine_mocks.h"
#include "../modifiers/lazymodifier.h"

void BASE_GAO_ModifierLazy_Compute(tLazyMdfStruct* lazy, Mtx4x4* mtx, Mtx4x4* parentMtx);
void NEW_GAO_ModifierLazy_Compute(tLazyMdfStruct* lazy, Mtx4x4* mtx, Mtx4x4* parentMtx);
void BASE_GAO_ModifierLazy_Compute_SemiMecanical_Model(tLazyMdfStruct* lazy, Mtx4x4* mtx,
                                                       Mtx4x4* parentMtx);
void NEW_GAO_ModifierLazy_Compute_SemiMecanical_Model(tLazyMdfStruct* lazy, Mtx4x4* mtx,
                                                      Mtx4x4* parentMtx);
void BASE_GAO_ModifierLazy_Inherit(tLazyMdfStruct* lazy, tGameObjectMain* src,
                                   tGameObjectMain* dst);
void NEW_GAO_ModifierLazy_Inherit(tLazyMdfStruct* lazy, tGameObjectMain* src,
                                  tGameObjectMain* dst);
void BASE_GAO_ModifierLazy_Apply(tModifier* mdf, void* unk1);
void NEW_GAO_ModifierLazy_Apply(tModifier* mdf, void* unk1);
void BASE_GAO_ModifierLazy_Reinit(tModifier* mdf);
void NEW_GAO_ModifierLazy_Reinit(tModifier* mdf);
int BASE_GAO_ul_ModifierLazy_Load(tModifier* mdf, char* data);
int NEW_GAO_ul_ModifierLazy_Load(tModifier* mdf, char* data);
void BASE_GAO_ModifierLazy_UnApply_Local(tLazyMdfStruct* lazy);
void NEW_GAO_ModifierLazy_UnApply_Local(tLazyMdfStruct* lazy);

static int g_fails;
static int g_diffCount;
static double g_worstAbs;
static double g_worstRel;
static double g_worstRelAbs;
static double g_worstAbsRel;
static const char* g_worstWhat = "-";
static const char* g_worstAbsWhat = "-";
static char g_worstCase[96] = "-";
static char g_worstAbsCase[96] = "-";
static char g_case[96] = "-";

static void setCase(const char* name)
{
    strncpy(g_case, name ? name : "-", sizeof(g_case) - 1);
    g_case[sizeof(g_case) - 1] = 0;
}

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
    {
        g_worstAbs = d;
        g_worstAbsWhat = what;
        g_worstAbsRel = r;
        strcpy(g_worstAbsCase, g_case);
    }
    if (r > g_worstRel)
    {
        g_worstRel = r;
        g_worstWhat = what;
        g_worstRelAbs = d;
        strcpy(g_worstCase, g_case);
    }
    if (r > 1e-4 && d > 1e-4)
    {
        printf("    DIFF %s[%d]: old %.9g  new %.9g  (rel %.3g)\n",
               what, idx, (double)a, (double)b, r);
        return 0;
    }
    return 1;
}

static int cmpMtx(Mtx4x4* a, Mtx4x4* b, const char* what)
{
    float* pa = (float*)a;
    float* pb = (float*)b;
    int ok = 1;
    int i;

    for (i = 0; i < 20; i++)
        ok &= closeEnough(pa[i], pb[i], what, i);
    if (a->mtxflag != b->mtxflag)
    {
        printf("    DIFF %s.mtxflag: old %u new %u\n", what, a->mtxflag, b->mtxflag);
        ok = 0;
    }
    return ok;
}

static int cmpLazy(tLazyMdfStruct* a, tLazyMdfStruct* b)
{
    int ok = 1;

    ok &= cmpMtx(&a->mtx1, &b->mtx1, "lazy.mtx1");
    ok &= cmpMtx(&a->mtx2, &b->mtx2, "lazy.mtx2");
    ok &= closeEnough(a->vv.x, b->vv.x, "lazy.vv", 0);
    ok &= closeEnough(a->vv.y, b->vv.y, "lazy.vv", 1);
    ok &= closeEnough(a->vv.z, b->vv.z, "lazy.vv", 2);
    ok &= closeEnough(a->f2, b->f2, "lazy.f2", 0);
    ok &= closeEnough(a->fff1, b->fff1, "lazy.fff1", 0);
    ok &= closeEnough(a->fff2, b->fff2, "lazy.fff2", 0);
    ok &= closeEnough(a->ff1, b->ff1, "lazy.ff1", 0);
    ok &= closeEnough(a->ff2, b->ff2, "lazy.ff2", 0);
    ok &= closeEnough(a->ff3, b->ff3, "lazy.ff3", 0);
    ok &= closeEnough(a->upperMax, b->upperMax, "lazy.upperMax", 0);
    ok &= closeEnough(a->ff5, b->ff5, "lazy.ff5", 0);
    ok &= closeEnough(a->ff6, b->ff6, "lazy.ff6", 0);
    if (a->upperFlag != b->upperFlag)
    {
        printf("    DIFF lazy.upperFlag: old %#x new %#x\n", a->upperFlag, b->upperFlag);
        ok = 0;
    }
    if (a->upperFlagBak != b->upperFlagBak)
    {
        printf("    DIFF lazy.upperFlagBak: old %#x new %#x\n",
               a->upperFlagBak, b->upperFlagBak);
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
    setCase("-");
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

static void fillMtx(Mtx4x4* m, int withScale)
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
    m->Pos.x = rngFloat(-40.0f, 40.0f);
    m->Pos.y = rngFloat(-40.0f, 40.0f);
    m->Pos.z = rngFloat(-40.0f, 40.0f);
    m->_m14 = rngFloat(0.5f, 2.0f);
    m->_m24 = rngFloat(0.5f, 2.0f);
    m->_m34 = rngFloat(0.5f, 2.0f);
    m->w = 1.0f;
    m->mtxflag = withScale ? 0xA : 2;
}

/* --- Compute and the semi-mechanical model ----------------------- */

typedef struct sComputeScene
{
    tLazyMdfStruct lazy;
    Mtx4x4 mtx;
    Mtx4x4 parentMtx;
} tComputeScene;

static void computeBuild(tComputeScene* s, unsigned seed, int flags, int scaled)
{
    memset(s, 0, sizeof(*s));
    rngSeed(seed);

    fillMtx(&s->mtx, scaled);
    fillMtx(&s->parentMtx, 0);
    fillMtx(&s->lazy.mtx1, 0);
    fillMtx(&s->lazy.mtx2, scaled);

    s->lazy.upperFlag = flags;
    s->lazy.f2 = rngFloat(0.05f, 1.4f);
    s->lazy.fff1 = rngFloat(-3.0f, 3.0f);
    s->lazy.fff2 = rngFloat(0.3f, 1.0f);
    s->lazy.ff1 = rngFloat(-2.0f, -0.05f);
    s->lazy.ff2 = rngFloat(-2.0f, -0.05f);
    s->lazy.ff3 = rngFloat(-2.0f, -0.05f);
    s->lazy.upperMax = rngFloat(0.05f, 2.0f);
    s->lazy.ff5 = rngFloat(0.05f, 2.0f);
    s->lazy.ff6 = rngFloat(0.05f, 2.0f);
    s->lazy.vv.x = rngFloat(-5.0f, 5.0f);
    s->lazy.vv.y = rngFloat(-5.0f, 5.0f);
    s->lazy.vv.z = rngFloat(-5.0f, 5.0f);
}

static void runCompute(const char* name, unsigned seed, int flags, int scaled,
                       int noParent, float dt, int frames)
{
    tComputeScene a, b;
    tMockCall logA[MOCK_LOG_MAX];
    int nA, f, ok = 1;

    computeBuild(&a, seed, flags, scaled);
    computeBuild(&b, seed, flags, scaled);

    setCase(name);
    TIM_gf_dt = dt;
    MOCK_Reset();
    for (f = 0; f < frames; f++)
        BASE_GAO_ModifierLazy_Compute(&a.lazy, &a.mtx, noParent ? NULL : &a.parentMtx);
    nA = MOCK_logLen;
    memcpy(logA, MOCK_log, sizeof(tMockCall) * (size_t)nA);

    MOCK_Reset();
    for (f = 0; f < frames; f++)
        NEW_GAO_ModifierLazy_Compute(&b.lazy, &b.mtx, noParent ? NULL : &b.parentMtx);

    ok &= cmpLogs(logA, nA, MOCK_log, MOCK_logLen);
    ok &= cmpLazy(&a.lazy, &b.lazy);
    ok &= cmpMtx(&a.mtx, &b.mtx, "mtx");
    report(name, ok);
}

static void runSemi(const char* name, unsigned seed, int flags, float dt, int frames)
{
    tComputeScene a, b;
    tMockCall logA[MOCK_LOG_MAX];
    int nA, f, ok = 1;

    computeBuild(&a, seed, flags, 0);
    computeBuild(&b, seed, flags, 0);

    setCase(name);
    TIM_gf_dt = dt;
    MOCK_Reset();
    for (f = 0; f < frames; f++)
        BASE_GAO_ModifierLazy_Compute_SemiMecanical_Model(&a.lazy, &a.mtx, &a.parentMtx);
    nA = MOCK_logLen;
    memcpy(logA, MOCK_log, sizeof(tMockCall) * (size_t)nA);

    MOCK_Reset();
    for (f = 0; f < frames; f++)
        NEW_GAO_ModifierLazy_Compute_SemiMecanical_Model(&b.lazy, &b.mtx, &b.parentMtx);

    ok &= cmpLogs(logA, nA, MOCK_log, MOCK_logLen);
    ok &= cmpLazy(&a.lazy, &b.lazy);
    ok &= cmpMtx(&a.mtx, &b.mtx, "mtx");
    report(name, ok);
}

static void runSemiNoParent(const char* name)
{
    tComputeScene a, b;
    int ok = 1;

    computeBuild(&a, 99u, 0x400, 0);
    computeBuild(&b, 99u, 0x400, 0);
    TIM_gf_dt = 0.016f;
    MOCK_Reset();
    BASE_GAO_ModifierLazy_Compute_SemiMecanical_Model(&a.lazy, &a.mtx, NULL);
    NEW_GAO_ModifierLazy_Compute_SemiMecanical_Model(&b.lazy, &b.mtx, NULL);
    ok &= cmpLazy(&a.lazy, &b.lazy);
    ok &= cmpMtx(&a.mtx, &b.mtx, "mtx");
    report(name, ok);
}

/* --- Inherit, Reinit, Apply -------------------------------------- */

#define MAXMDF 4

typedef struct sGaoScene
{
    tLazyMdfStruct lazy;
    tLazyMdfStruct other[MAXMDF];
    tGameObjectMain gao;
    tGameObjectMain refGao;
    tGameObjectBasic basic;
    tGameObjectBasic refBasic;
    tGaoMatrix gmtx;
    tGaoMatrix refGmtx;
    tGameObjectExtended refExt;
    tModifier mdf;
    tModifier chain[MAXMDF];
    tModifierInterface intf[MAXMDF];
    tModifierInterface ownIntf;
    Mtx4x4 gaoMtx;
    Mtx4x4 refMtx;
} tGaoScene;

static void gaoBuild(tGaoScene* s, unsigned seed, int gaoFlags, int lazyFlags,
                     int refFlags, int haveRef, int chainLen, int srcScaled)
{
    int i;

    memset(s, 0, sizeof(*s));
    rngSeed(seed);

    fillMtx(&s->gaoMtx, 0);
    fillMtx(&s->refMtx, srcScaled);
    fillMtx(&s->gmtx.mtx, 0);
    fillMtx(&s->refGmtx.mtx, srcScaled);
    fillMtx(&s->lazy.mtx1, 0);
    fillMtx(&s->lazy.mtx2, 0);

    s->gao.flags = gaoFlags;
    s->gao.basic = &s->basic;
    s->gao.matrices = &s->gaoMtx;
    s->basic.mtx = &s->gmtx;

    s->refGao.flags = refFlags;
    s->refGao.basic = &s->refBasic;
    s->refGao.matrices = &s->refMtx;
    s->refBasic.mtx = &s->refGmtx;
    s->refGao.extended = &s->refExt;

    s->gmtx.refGao2 = haveRef ? &s->refGao : NULL;

    s->lazy.upperFlag = lazyFlags;
    s->lazy.f2 = rngFloat(0.05f, 1.4f);
    s->lazy.fff1 = rngFloat(-3.0f, 3.0f);
    s->lazy.fff2 = rngFloat(0.3f, 1.0f);
    s->lazy.ff1 = -0.5f;
    s->lazy.ff2 = -0.5f;
    s->lazy.ff3 = -0.5f;
    s->lazy.upperMax = 0.5f;
    s->lazy.ff5 = 0.5f;
    s->lazy.ff6 = 0.5f;
    s->lazy.gao = &s->gao;

    s->ownIntf.mdfId = 24;
    s->mdf.modifierIntf = &s->ownIntf;
    s->mdf.mdfPrivateDataPtr = &s->lazy;
    s->mdf.modifierFlags = 0x1000000;
    s->mdf.gao = &s->gao;

    for (i = 0; i < chainLen; i++)
    {
        s->intf[i].mdfId = (i == 1) ? 7 : 24;
        s->chain[i].modifierIntf = &s->intf[i];
        s->chain[i].mdfPrivateDataPtr = &s->other[i];
        s->chain[i].modifierFlags = (i == 2) ? 0x2000000 : 0x1000000;
        s->chain[i].gao = &s->refGao;
        s->chain[i].nextElem = (i + 1 < chainLen) ? &s->chain[i + 1] : NULL;

        fillMtx(&s->other[i].mtx1, 0);
        fillMtx(&s->other[i].mtx2, 0);
        s->other[i].upperFlag = 9;
        s->other[i].f2 = 0.5f;
        s->other[i].fff2 = 0.9f;
        s->other[i].gao = &s->refGao;
    }
    s->refExt.mdf = chainLen ? &s->chain[0] : NULL;
}

static void runInherit(const char* name, unsigned seed, int srcFlags, int lazyFlags,
                       int srcScaled)
{
    tGaoScene* a = (tGaoScene*)malloc(sizeof(tGaoScene));
    tGaoScene* b = (tGaoScene*)malloc(sizeof(tGaoScene));
    int ok = 1;

    gaoBuild(a, seed, 0x400000, lazyFlags, srcFlags, 1, 0, srcScaled);
    gaoBuild(b, seed, 0x400000, lazyFlags, srcFlags, 1, 0, srcScaled);
    setCase(name);

    MOCK_Reset();
    BASE_GAO_ModifierLazy_Inherit(&a->lazy, &a->refGao, &a->gao);
    NEW_GAO_ModifierLazy_Inherit(&b->lazy, &b->refGao, &b->gao);

    ok &= cmpMtx(&a->gmtx.mtx, &b->gmtx.mtx, "dst.mtx");
    ok &= cmpMtx(&a->refGmtx.mtx, &b->refGmtx.mtx, "src.gmtx");
    ok &= cmpMtx(&a->refMtx, &b->refMtx, "src.matrices");
    report(name, ok);
    free(a);
    free(b);
}

static void runApply(const char* name, unsigned seed, int gaoFlags, int lazyFlags,
                     int refFlags, int haveRef, int chainLen, float dt)
{
    tGaoScene* a = (tGaoScene*)malloc(sizeof(tGaoScene));
    tGaoScene* b = (tGaoScene*)malloc(sizeof(tGaoScene));
    tMockCall logA[MOCK_LOG_MAX];
    int nA, i, ok = 1;

    gaoBuild(a, seed, gaoFlags, lazyFlags, refFlags, haveRef, chainLen, 0);
    gaoBuild(b, seed, gaoFlags, lazyFlags, refFlags, haveRef, chainLen, 0);
    setCase(name);

    TIM_gf_dt = dt;
    p_FirsToUnapply = NULL;
    MOCK_Reset();
    BASE_GAO_ModifierLazy_Apply(&a->mdf, NULL);
    nA = MOCK_logLen;
    memcpy(logA, MOCK_log, sizeof(tMockCall) * (size_t)nA);
    if (p_FirsToUnapply)
        BASE_GAO_ModifierLazy_UnApply_Local(&a->lazy);

    p_FirsToUnapply = NULL;
    MOCK_Reset();
    NEW_GAO_ModifierLazy_Apply(&b->mdf, NULL);
    if (p_FirsToUnapply)
        NEW_GAO_ModifierLazy_UnApply_Local(&b->lazy);
    p_FirsToUnapply = NULL;

    ok &= cmpLogs(logA, nA, MOCK_log, MOCK_logLen);
    ok &= cmpLazy(&a->lazy, &b->lazy);
    ok &= cmpMtx(&a->gaoMtx, &b->gaoMtx, "gao.matrices");
    ok &= cmpMtx(&a->gmtx.mtx, &b->gmtx.mtx, "gao.basic.mtx");
    for (i = 0; i < chainLen; i++)
        ok &= cmpLazy(&a->other[i], &b->other[i]);
    report(name, ok);
    free(a);
    free(b);
}

static void runReinit(const char* name, unsigned seed, int gaoFlags, int haveRef, int noGao)
{
    tGaoScene* a = (tGaoScene*)malloc(sizeof(tGaoScene));
    tGaoScene* b = (tGaoScene*)malloc(sizeof(tGaoScene));
    int ok = 1;

    gaoBuild(a, seed, gaoFlags, 9, 0x2000, haveRef, 0, 0);
    gaoBuild(b, seed, gaoFlags, 9, 0x2000, haveRef, 0, 0);
    if (noGao)
    {
        a->mdf.gao = NULL;
        b->mdf.gao = NULL;
    }

    MOCK_Reset();
    BASE_GAO_ModifierLazy_Reinit(&a->mdf);
    NEW_GAO_ModifierLazy_Reinit(&b->mdf);

    ok &= cmpMtx(&a->lazy.mtx2, &b->lazy.mtx2, "lazy.mtx2");
    report(name, ok);
    free(a);
    free(b);
}

/* --- Load -------------------------------------------------------- */

static void runLoad(const char* name, unsigned version)
{
    tGaoScene* a = (tGaoScene*)malloc(sizeof(tGaoScene));
    tGaoScene* b = (tGaoScene*)malloc(sizeof(tGaoScene));
    unsigned char stream[128];
    unsigned i;
    int ra, rb, ok = 1;

    gaoBuild(a, 7u, 0x400000, 0, 0x2000, 1, 0, 0);
    gaoBuild(b, 7u, 0x400000, 0, 0x2000, 1, 0, 0);

    rngSeed(version + 1u);
    for (i = 0; i < sizeof(stream); i++)
        stream[i] = (unsigned char)(rngNext() & 0x7f);
    memcpy(stream + 8, &version, sizeof(version));

    MOCK_Reset();
    ra = BASE_GAO_ul_ModifierLazy_Load(&a->mdf, (char*)stream);
    rb = NEW_GAO_ul_ModifierLazy_Load(&b->mdf, (char*)stream);

    if (ra != rb)
    {
        printf("    DIFF bytes read: old %d new %d\n", ra, rb);
        ok = 0;
    }
    ok &= cmpLazy(&a->lazy, &b->lazy);
    report(name, ok);
    free(a);
    free(b);
}

int main(void)
{
    char name[96];
    int flags;
    static const int flagSets[] = {
        0, 1, 2, 4, 8, 9, 0xB, 0xD, 0x409, 0x40B, 0x40D, 0x40F, 0x1000, 0x1400
    };
    unsigned i;

    setvbuf(stdout, NULL, _IONBF, 0);
    printf("lazymodifier: old against new\n");

    for (i = 0; i < sizeof(flagSets) / sizeof(flagSets[0]); i++)
    {
        flags = flagSets[i];
        sprintf(name, "compute, flags %#x", flags);
        runCompute(name, 101u + i, flags, 0, 0, 0.016f, 1);
        sprintf(name, "compute, flags %#x, scaled, eight frames", flags);
        runCompute(name, 201u + i, flags, 1, 0, 0.033f, 8);
        sprintf(name, "compute, flags %#x, no parent", flags);
        runCompute(name, 301u + i, flags, 0, 1, 0.016f, 1);
    }
    runCompute("compute, dt zero", 401u, 0x40F, 1, 0, 0.0f, 1);
    runCompute("compute, dt one", 402u, 0x40F, 1, 0, 1.0f, 1);

    runSemi("semi, no box clamp", 501u, 0, 0.016f, 1);
    runSemi("semi, box clamp", 502u, 0x400, 0.016f, 1);
    runSemi("semi, box clamp, eight frames", 503u, 0x400, 0.02f, 8);
    runSemiNoParent("semi, no parent");

    runInherit("inherit, hierarchy source", 601u, 0x400000, 0x70, 0);
    runInherit("inherit, hierarchy source, scaled", 602u, 0x400000, 0x70, 1);
    runInherit("inherit, plain source", 603u, 0x2000, 0x70, 0);
    runInherit("inherit, plain source, scaled", 604u, 0x2000, 0x70, 1);
    runInherit("inherit, position only", 605u, 0x400000, 0x10, 0);
    runInherit("inherit, axes only", 606u, 0x400000, 0x20, 1);
    runInherit("inherit, scale only", 607u, 0x400000, 0x40, 1);
    runInherit("inherit, nothing", 608u, 0x400000, 0, 1);

    runApply("apply, plain object", 701u, 0x2000, 9, 0x2000, 0, 0, 0.016f);
    runApply("apply, hierarchy, no reference", 702u, 0x400000, 9, 0x2000, 0, 0, 0.016f);
    runApply("apply, hierarchy, reference", 703u, 0x400000, 9, 0x2000, 1, 0, 0.016f);
    runApply("apply, reference with a chain", 704u, 0x400000, 9, 0x2000, 1, 4, 0.016f);
    runApply("apply, reference not scanned", 705u, 0x400000, 9, 0, 1, 4, 0.016f);
    runApply("apply, inherit and chain", 706u, 0x400000, 0x79, 0x2000, 1, 4, 0.016f);
    runApply("apply, flag restore bits", 707u, 0x400000, 0x989, 0x2000, 1, 2, 0.016f);
    runApply("apply, already applied", 708u, 0x400000, 0x10000009, 0x2000, 1, 2, 0.016f);

    runReinit("reinit, plain object", 801u, 0x2000, 0, 0);
    runReinit("reinit, hierarchy with reference", 802u, 0x400000, 1, 0);
    runReinit("reinit, hierarchy without reference", 803u, 0x400000, 0, 0);
    runReinit("reinit, no object", 804u, 0x2000, 0, 1);

    runLoad("load, version 0", 0);
    runLoad("load, version 1", 1);
    runLoad("load, version 2", 2);
    runLoad("load, version 3", 3);

    printf("%d of the compared floats differed at all\n", g_diffCount);
    printf("  worst abs %.3g (rel %.3g) at %s (%s)\n",
           g_worstAbs, g_worstAbsRel, g_worstAbsWhat, g_worstAbsCase);
    printf("  worst rel %.3g (abs %.3g) at %s (%s)\n",
           g_worstRel, g_worstRelAbs, g_worstWhat, g_worstCase);
    if (g_fails)
        printf("%d case(s) differed\n", g_fails);
    else
        printf("all cases equivalent\n");
    return g_fails ? 1 : 0;
}
