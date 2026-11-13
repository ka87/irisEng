/* Differential test for profiler.c.
 *
 * The frozen pre-pass copy and the rewrite are compiled into one binary
 * with their functions and their own file-scope globals prefixed BASE_
 * and NEW_. The raster structures themselves are tentative definitions
 * in profiler.h, so both copies share them; each case therefore runs one
 * side over a cleared world, takes a normalised snapshot, clears again
 * and runs the other.
 *
 * Compared per case: every field of every raster the case touches, the
 * list linkage as indices, the three name strings by content (the two
 * copies' string literals live at different addresses), the performance
 * counter arrays, the return values, and the ordered sequence of GDI
 * calls with their arguments and the text drawn.
 */

#include "profiler_mocks.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define POOL 64

static profilerRasterStructElem POOL_rasters[POOL];

/* ---------- the two sides ---------- */

typedef void (*fnRasterAdd)(profilerRasterStructElem*, char*, char*, char*, int, float);
typedef void (*fnVoid)(void);
typedef void (*fnRaster)(profilerRasterStructElem*);
typedef void (*fnInt)(int);
typedef void (*fnInitRasters)(profilerRasterStructElem*, char*);
typedef int  (*fnDisplayTexts)(HWND, HDC, int, profilerRasterStructElem*, int);
typedef void (*fnDisplayHandler)(HWND, HDC, int);

typedef struct sSide
{
    const char* name;
    fnRasterAdd RasterAdd;
    fnVoid InitModule;
    fnVoid DbgInit;
    fnVoid ForceStart;
    fnRaster RecordTimeDecRef;
    fnRaster StartBlock;
    fnRaster EndBlock;
    fnInt QpcDec;
    fnRaster ClearOne;
    fnRaster ClearAll;
    fnInitRasters InitRasters;
    fnDisplayTexts DisplayTexts;
    fnDisplayHandler DisplayHandler;
    fnVoid WindowHandler;
    int* rasterWindowIsOn;
    int* rasterDetailPage;
    profilerRasterStructElem** firstRasterPtr;
    profilerRasterStructElem** unkRasterPtr;
    profilerRasterStructElem** currRaster;
    HWND* rasterHWND;
    float* dbgRasterSetTo60;
} tSide;

#define DECL(p) \
    void p##PRO_RasterAdd(profilerRasterStructElem*, char*, char*, char*, int, float); \
    void p##PRO_InitModule(void); \
    void p##profiler_dbg_init(void); \
    void p##ENG_ForceStartRasters(void); \
    void p##PRO_RecordTimeDecRef(profilerRasterStructElem*); \
    void p##PRO_PerfCounterStartBlock(profilerRasterStructElem*); \
    void p##PRO_PerfCounterEndBlock(profilerRasterStructElem*); \
    void p##PRO_QPC_RecordTimeDecRef(int); \
    void p##PRO_ClearOneRaster(profilerRasterStructElem*); \
    void p##PRO_ClearAllRasters(profilerRasterStructElem*); \
    void p##PRO_InitRasters(profilerRasterStructElem*, char*); \
    int  p##PRO_DisplayTextsOnRasterWindow(HWND, HDC, int, profilerRasterStructElem*, int); \
    void p##PRO_ProfilerDisplayHandler(HWND, HDC, int); \
    void p##PRO_ProfilerWindowHandler(void); \
    extern int p##PRO_rasterWindowIsOn; \
    extern int p##PRO_rasterDetailPage; \
    extern profilerRasterStructElem* p##PRO_FirstRasterPtr; \
    extern profilerRasterStructElem* p##pointsToUnkRaster_dword_98F944; \
    extern profilerRasterStructElem* p##PRO_CurrRaster; \
    extern HWND p##PRO_rasterHWND; \
    extern float p##gfDBGRasterSetTo60;

DECL(BASE_)
DECL(NEW_)

#define SIDE(nm, p) { nm, \
    p##PRO_RasterAdd, p##PRO_InitModule, p##profiler_dbg_init, p##ENG_ForceStartRasters, \
    p##PRO_RecordTimeDecRef, p##PRO_PerfCounterStartBlock, p##PRO_PerfCounterEndBlock, \
    p##PRO_QPC_RecordTimeDecRef, p##PRO_ClearOneRaster, p##PRO_ClearAllRasters, \
    p##PRO_InitRasters, p##PRO_DisplayTextsOnRasterWindow, p##PRO_ProfilerDisplayHandler, \
    p##PRO_ProfilerWindowHandler, \
    &p##PRO_rasterWindowIsOn, &p##PRO_rasterDetailPage, &p##PRO_FirstRasterPtr, \
    &p##pointsToUnkRaster_dword_98F944, &p##PRO_CurrRaster, &p##PRO_rasterHWND, \
    &p##gfDBGRasterSetTo60 }

static tSide sides[2] = { SIDE("base", BASE_), SIDE("new", NEW_) };

/* ---------- the address table, so linkage can be compared as indices ---------- */

#define NAMED 23
static profilerRasterStructElem* namedRasters[NAMED];

static void BuildNamed(void)
{
    int i = 0;
    namedRasters[i++] = &PRO_EngineDt;
    namedRasters[i++] = &PRO_EngineRealDT;
    namedRasters[i++] = &PRO_EngineFrames;
    namedRasters[i++] = &PRO_Synchro;
    namedRasters[i++] = &PRO_WinMsg;
    namedRasters[i++] = &PRO_Input;
    namedRasters[i++] = &PRO_Display;
    namedRasters[i++] = &PRO_Activation;
    namedRasters[i++] = &PRO_Visibility;
    namedRasters[i++] = &PRO_TablesManager;
    namedRasters[i++] = &PRO_EOTCreation;
    namedRasters[i++] = &PRO_AI;
    namedRasters[i++] = &PRO_SnP;
    namedRasters[i++] = &PRO_Events;
    namedRasters[i++] = &PRO_Animation;
    namedRasters[i++] = &PRO_Dynamics;
    namedRasters[i++] = &PRO_Collision;
    namedRasters[i++] = &PRO_Records;
    namedRasters[i++] = &PRO_GameEngine;
    namedRasters[i++] = &PRO_FullLoop;
    namedRasters[i++] = &PRO_ShortWay;
    namedRasters[i++] = &PRO_RayObjectDist;
    namedRasters[i++] = &PRO_RayObjectVector;
}

#define GROUPN 11
#define TOTAL (POOL + GROUPN + NAMED)

static profilerRasterStructElem* RasterAt(int idx)
{
    if (idx < POOL)
        return &POOL_rasters[idx];
    if (idx < POOL + GROUPN)
        return &((profilerRasterStructElem*)&PRO_RasterGroup)[idx - POOL];
    return namedRasters[idx - POOL - GROUPN];
}

static int RasterIndex(const profilerRasterStructElem* p)
{
    int i;

    if (!p)
        return -1;
    for (i = 0; i < TOTAL; i++)
        if (RasterAt(i) == p)
            return i;
    return -2;
}

/* ---------- normalised snapshot ---------- */

typedef struct sRasterShot
{
    int unk1;
    unsigned int unk2bits;
    int insideCounter;
    int unk3s2;
    int unk7;
    int startTick;
    int sumTime;
    int unk10;
    int prev;
    int next;
    char group[48];
    char subgroup[48];
    char module[48];
} tRasterShot;

typedef struct sShot
{
    tRasterShot r[TOTAL];
    int first;
    long long pc[54];
    long long pcSum[54];
    unsigned int ticks;
    long long qpc;
    int ret[256];
    int nret;
} tShot;

static tShot shot[2];
static tProMockCall logCopy[2][PROMOCK_LOG_MAX];
static int logLen[2];

static void CopyName(char* dst, const char* src)
{
    if (!src)
    {
        strcpy(dst, "<null>");
        return;
    }
    strncpy(dst, src, 47);
    dst[47] = 0;
}

static void TakeShot(tShot* s)
{
    int i;

    for (i = 0; i < TOTAL; i++)
    {
        profilerRasterStructElem* p = RasterAt(i);
        s->r[i].unk1 = p->unk1;
        memcpy(&s->r[i].unk2bits, &p->unk2, 4);
        s->r[i].insideCounter = p->insideCounter;
        s->r[i].unk3s2 = p->unk3s2;
        s->r[i].unk7 = p->unk7;
        s->r[i].startTick = p->startTick;
        s->r[i].sumTime = p->sumTime;
        s->r[i].unk10 = p->unk10;
        s->r[i].prev = RasterIndex(p->prev);
        s->r[i].next = RasterIndex(p->next);
        CopyName(s->r[i].group, p->group);
        CopyName(s->r[i].subgroup, p->subgroup);
        CopyName(s->r[i].module, p->module);
    }
    s->first = RasterIndex(DBG_profiler_first_raster);
    for (i = 0; i < 54; i++)
    {
        s->pc[i] = PerfCounterStruct[i].QuadPart;
        s->pcSum[i] = PerfCountertimeSumArray[i];
    }
    s->ticks = PROMOCK_ticks;
    s->qpc = PROMOCK_qpc;
}

static void ClearWorld(void)
{
    int i;

    memset(POOL_rasters, 0, sizeof(POOL_rasters));
    memset(&PRO_RasterGroup, 0, sizeof(PRO_RasterGroup));
    for (i = 0; i < NAMED; i++)
        memset(namedRasters[i], 0, sizeof(profilerRasterStructElem));
    memset(PerfCounterStruct, 0, sizeof(PerfCounterStruct));
    memset(PerfCountertimeSumArray, 0, sizeof(PerfCountertimeSumArray));
    DBG_profiler_first_raster = NULL;
    TIM_gul_TicksPerSecond = 3579545;
    TIM_gf_dt = 0.0f;
    TIM_dtTicks = 0.0f;
    MAI_gh_MainWindow = NULL;
    PROMOCK_Reset();
}

/* ---------- comparison ---------- */

static int cases;
static int failures;
static int rasterFieldsCompared;
static int callsCompared;

static void Fail(const char* what, const char* detail)
{
    if (failures < 20)
        printf("  DIFF %-28s %s\n", what, detail);
    failures++;
}

static void CompareCase(const char* name)
{
    char buf[256];
    int i, j;

    cases++;
    for (i = 0; i < TOTAL; i++)
    {
        tRasterShot* a = &shot[0].r[i];
        tRasterShot* b = &shot[1].r[i];
        rasterFieldsCompared += 13;
        if (memcmp(a, b, sizeof(tRasterShot)) != 0)
        {
            sprintf(buf, "%s raster %d: unk1 %d/%d unk2 %08x/%08x cnt %d/%d unk7 %d/%d "
                         "start %d/%d sum %d/%d unk10 %d/%d prev %d/%d next %d/%d",
                    name, i, a->unk1, b->unk1, a->unk2bits, b->unk2bits,
                    a->insideCounter, b->insideCounter, a->unk7, b->unk7,
                    a->startTick, b->startTick, a->sumTime, b->sumTime,
                    a->unk10, b->unk10, a->prev, b->prev, a->next, b->next);
            Fail("raster", buf);
            if (strcmp(a->group, b->group) || strcmp(a->subgroup, b->subgroup)
                || strcmp(a->module, b->module))
            {
                sprintf(buf, "%s raster %d names: [%s|%s|%s] vs [%s|%s|%s]", name, i,
                        a->group, a->subgroup, a->module,
                        b->group, b->subgroup, b->module);
                Fail("names", buf);
            }
            return;
        }
    }
    if (shot[0].first != shot[1].first)
    {
        sprintf(buf, "%s head %d vs %d", name, shot[0].first, shot[1].first);
        Fail("list head", buf);
    }
    for (i = 0; i < 54; i++)
    {
        if (shot[0].pc[i] != shot[1].pc[i] || shot[0].pcSum[i] != shot[1].pcSum[i])
        {
            sprintf(buf, "%s perf[%d] %lld/%lld sum %lld/%lld", name, i,
                    shot[0].pc[i], shot[1].pc[i], shot[0].pcSum[i], shot[1].pcSum[i]);
            Fail("perfcounter", buf);
            break;
        }
    }
    if (shot[0].ticks != shot[1].ticks || shot[0].qpc != shot[1].qpc)
    {
        sprintf(buf, "%s clock %u/%u qpc %lld/%lld", name,
                shot[0].ticks, shot[1].ticks, shot[0].qpc, shot[1].qpc);
        Fail("clock draws", buf);
    }
    if (shot[0].nret != shot[1].nret)
    {
        sprintf(buf, "%s return count %d vs %d", name, shot[0].nret, shot[1].nret);
        Fail("returns", buf);
    }
    else
    {
        for (i = 0; i < shot[0].nret; i++)
            if (shot[0].ret[i] != shot[1].ret[i])
            {
                sprintf(buf, "%s return %d: %d vs %d", name, i,
                        shot[0].ret[i], shot[1].ret[i]);
                Fail("returns", buf);
                break;
            }
    }
    if (logLen[0] != logLen[1])
    {
        sprintf(buf, "%s call count %d vs %d", name, logLen[0], logLen[1]);
        Fail("call sequence", buf);
        return;
    }
    for (i = 0; i < logLen[0]; i++)
    {
        tProMockCall* a = &logCopy[0][i];
        tProMockCall* b = &logCopy[1][i];
        callsCompared++;
        if (strcmp(a->fn, b->fn) || a->nargs != b->nargs || strcmp(a->text, b->text))
        {
            sprintf(buf, "%s call %d: %s[%s] vs %s[%s]", name, i,
                    a->fn, a->text, b->fn, b->text);
            Fail("call sequence", buf);
            return;
        }
        for (j = 0; j < a->nargs; j++)
            if (a->arg[j] != b->arg[j])
            {
                sprintf(buf, "%s call %d (%s) arg %d: %d vs %d", name, i,
                        a->fn, j, a->arg[j], b->arg[j]);
                Fail("call sequence", buf);
                return;
            }
    }
}

static void EndSide(int side)
{
    TakeShot(&shot[side]);
    memcpy(logCopy[side], PROMOCK_log, sizeof(PROMOCK_log));
    logLen[side] = PROMOCK_logLen;
}

/* ---------- cases ---------- */

static void Case_DbgInitAndForceStart(const tSide* s, tShot* out)
{
    int i;

    s->InitModule();
    s->DbgInit();
    for (i = 0; i < NAMED; i++)
        namedRasters[i]->insideCounter = (int16_t)(i + 1);
    for (i = 0; i < GROUPN; i++)
        ((profilerRasterStructElem*)&PRO_RasterGroup)[i].insideCounter = (int16_t)(i + 3);
    s->ForceStart();
    out->nret = 0;
}

static void Case_InitAndClearRasters(const tSide* s, tShot* out)
{
    int i;

    s->InitModule();
    s->InitRasters(POOL_rasters, "Display Data");
    s->ClearAll(POOL_rasters);
    out->nret = 0;
    out->ret[out->nret++] = RasterIndex(DBG_profiler_first_raster);
    /* the clear above happens to reach the last raster when its
       neighbours have already gone, so the 47th call returns early and
       a short bound is invisible. This list puts 46 in the middle. */
    s->InitModule();
    memset(POOL_rasters, 0, sizeof(POOL_rasters));
    for (i = 55; i >= 0; i--)
        s->RasterAdd(&POOL_rasters[i], "G", "S", "M", 0, 0.0f);
    s->ClearAll(POOL_rasters);
    out->ret[out->nret++] = RasterIndex(DBG_profiler_first_raster);
}

static void Case_ClearOne(const tSide* s, tShot* out)
{
    int which;

    out->nret = 0;
    for (which = 0; which < 6; which++)
    {
        int i;
        s->InitModule();
        memset(POOL_rasters, 0, sizeof(POOL_rasters));
        for (i = 0; i < 5; i++)
            s->RasterAdd(&POOL_rasters[i], "G", "S", "M", 0, 0.0f);
        /* 5 is never linked, so it exercises the early return */
        s->ClearOne(&POOL_rasters[which == 5 ? 5 : which]);
        out->ret[out->nret++] = RasterIndex(DBG_profiler_first_raster);
        /* the linkage has to be recorded here: the next iteration
           clears the pool, so only the last one would survive */
        for (i = 0; i < 6; i++)
        {
            out->ret[out->nret++] = RasterIndex(POOL_rasters[i].prev);
            out->ret[out->nret++] = RasterIndex(POOL_rasters[i].next);
        }
    }
}

static void Case_Counters(const tSide* s, tShot* out)
{
    int i, k;

    out->nret = 0;
    for (i = 0; i < 8; i++)
    {
        profilerRasterStructElem* r = &POOL_rasters[i];
        memset(r, 0, sizeof(*r));
        r->insideCounter = (int16_t)(i - 2);
        r->startTick = i * 11;
        r->sumTime = i * 3;
        for (k = 0; k < (i % 3) + 1; k++)
            s->StartBlock(r);
        for (k = 0; k < (i % 4) + 1; k++)
            s->EndBlock(r);
        s->RecordTimeDecRef(r);
        out->ret[out->nret++] = r->insideCounter;
        out->ret[out->nret++] = r->sumTime;
    }
}

static void Case_Qpc(const tSide* s, tShot* out)
{
    int i;

    out->nret = 0;
    for (i = 0; i < 12; i++)
    {
        PerfCounterStruct[i].QuadPart = (i % 3) ? (long long)(1000 * i) : 0;
        PerfCountertimeSumArray[i] = i * 7;
    }
    for (i = 0; i < 12; i++)
        s->QpcDec(i);
}

static const int kindList[6] = { 0, 1, 2, 3, 4, 5 };

static void Case_DisplayTexts(const tSide* s, tShot* out)
{
    int ki, sub, unk, cur, sum;

    out->nret = 0;
    for (ki = 0; ki < 6; ki++)
    for (sub = 0; sub < 2; sub++)
    for (unk = 0; unk < 2; unk++)
    for (cur = 0; cur < 2; cur++)
    for (sum = 0; sum < 2; sum++)
    {
        profilerRasterStructElem* r = &POOL_rasters[0];
        profilerRasterStructElem* other = &POOL_rasters[1];
        float v = 1.25f;

        memset(r, 0, sizeof(*r));
        memset(other, 0, sizeof(*other));
        r->unk1 = kindList[ki];
        r->group = "Grp";
        r->subgroup = "Sub";
        r->module = "Mod";
        if (r->unk1 == 2)
            memcpy(&r->unk2, "abc", 4);
        else if (r->unk1 == 1)
        {
            int n = 4242;
            memcpy(&r->unk2, &n, 4);
        }
        else
            r->unk2 = v;
        r->sumTime = sum ? 123456 : 0;
        r->unk10 = 654321;
        r->unk7 = 1;
        other->group = "Grp";
        other->subgroup = "Sub";
        other->module = "Mod";
        *s->unkRasterPtr = unk ? other : NULL;
        *s->currRaster = cur ? other : NULL;
        out->ret[out->nret++] = s->DisplayTexts((HWND)1, (HDC)2, 7 + ki, r, sub);
        out->ret[out->nret++] = r->unk7;
        out->ret[out->nret++] = r->sumTime;
        out->ret[out->nret++] = r->unk10;
    }
    *s->unkRasterPtr = NULL;
    *s->currRaster = NULL;
}

static void Case_DisplayHandler(const tSide* s, tShot* out, int variant)
{
    profilerRasterStructElem* r;
    int i;

    out->nret = 0;
    s->InitModule();
    if (variant < 5)
        s->DbgInit();
    else
        s->InitRasters(POOL_rasters, "Display Data");

    /* a raster is displayed only while its unk7 is set: PRO_RasterAdd
       clears it and only the handler's own walk sets it again, so a
       freshly built list displays nothing at all */
    for (i = 0; i < TOTAL; i++)
        RasterAt(i)->unk7 = 1;

    *s->firstRasterPtr = NULL;
    *s->currRaster = NULL;
    *s->unkRasterPtr = NULL;
    switch (variant)
    {
    case 0:
        break;
    case 1:
        *s->firstRasterPtr = &PRO_EngineDt;
        break;
    case 2:
        *s->firstRasterPtr = &PRO_Synchro;
        break;
    case 3:
        *s->firstRasterPtr = &PRO_Synchro;
        *s->currRaster = &PRO_Animation;
        break;
    case 4:
        *s->firstRasterPtr = &PRO_Synchro;
        *s->unkRasterPtr = &PRO_RasterGroup.PRO_UserProfilerRasters[0];
        break;
    case 5:
        *s->firstRasterPtr = &POOL_rasters[9];
        break;
    case 6:
        *s->firstRasterPtr = &POOL_rasters[41];
        *s->currRaster = &POOL_rasters[31];
        break;
    case 7:
        *s->firstRasterPtr = &POOL_rasters[41];
        *s->unkRasterPtr = &POOL_rasters[38];
        break;
    }
    s->DisplayHandler((HWND)1, (HDC)2, 0);
    /* and again, over the state the first pass left */
    s->DisplayHandler((HWND)1, (HDC)2, 100);
    for (i = 0; i < TOTAL; i++)
    {
        r = RasterAt(i);
        if (r->unk7 == 0)
            out->ret[out->nret++] = i;
    }
    *s->firstRasterPtr = NULL;
    *s->currRaster = NULL;
    *s->unkRasterPtr = NULL;
}

static void Case_WindowHandler(const tSide* s, tShot* out, int variant)
{
    out->nret = 0;
    *s->rasterHWND = (variant & 1) ? (HWND)0x1234 : NULL;
    *s->rasterDetailPage = (variant >> 1) & 1;
    *s->rasterWindowIsOn = (variant >> 2) & 1;
    TIM_gf_dt = (variant & 8) ? 0.05f : 0.001f;
    TIM_dtTicks = 0.002f;
    s->InitModule();
    s->DbgInit();
    *s->firstRasterPtr = &PRO_EngineDt;
    s->WindowHandler();
    out->ret[out->nret++] = (*s->rasterHWND != NULL);
    *s->rasterHWND = NULL;
    *s->rasterDetailPage = 0;
    *s->rasterWindowIsOn = 0;
    *s->firstRasterPtr = NULL;
}

/* ---------- driver ---------- */

static void Run(const char* name, void (*fn)(const tSide*, tShot*, int), int variant)
{
    int side;

    for (side = 0; side < 2; side++)
    {
        ClearWorld();
        fn(&sides[side], &shot[side], variant);
        EndSide(side);
    }
    CompareCase(name);
}

static void W0(const tSide* s, tShot* o, int v) { Case_DbgInitAndForceStart(s, o); }
static void W1(const tSide* s, tShot* o, int v) { Case_InitAndClearRasters(s, o); }
static void W2(const tSide* s, tShot* o, int v) { Case_ClearOne(s, o); }
static void W3(const tSide* s, tShot* o, int v) { Case_Counters(s, o); }
static void W4(const tSide* s, tShot* o, int v) { Case_Qpc(s, o); }
static void W5(const tSide* s, tShot* o, int v) { Case_DisplayTexts(s, o); }
static void W6(const tSide* s, tShot* o, int v) { Case_DisplayHandler(s, o, v); }
static void W7(const tSide* s, tShot* o, int v) { Case_WindowHandler(s, o, v); }

int main(void)
{
    char buf[64];
    int v;

    BuildNamed();

    Run("dbg_init+force_start", W0, 0);
    Run("init+clear_rasters", W1, 0);
    Run("clear_one_raster", W2, 0);
    Run("perf_counters", W3, 0);
    Run("qpc_decref", W4, 0);
    Run("display_texts", W5, 0);
    for (v = 0; v < 8; v++)
    {
        sprintf(buf, "display_handler/%d", v);
        Run(buf, W6, v);
    }
    for (v = 0; v < 16; v++)
    {
        sprintf(buf, "window_handler/%d", v);
        Run(buf, W7, v);
    }

    printf("profiler: %d cases, %d raster fields, %d GDI calls compared, %d differing\n",
           cases, rasterFieldsCompared, callsCompared, failures);
    if (PROMOCK_overflow)
    {
        printf("profiler: CALL LOG OVERFLOWED -- raise PROMOCK_LOG_MAX\n");
        return 1;
    }
    return failures ? 1 : 0;
}
