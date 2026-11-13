/* The differential test for gfx.c.
 *
 * The frozen pre-rewrite copy (tests/baseline_gfx.c) and the rewrite are
 * compiled into one binary with their symbols prefixed BASE_ and NEW_,
 * run over identical state, and compared on three things: every byte the
 * allocator handed out, every gfx global, and the ordered sequence of
 * engine calls each side made.
 *
 * Both sides allocate out of one bump arena that is wiped and rewound
 * between them, so a pointer one side produced is the same pointer the
 * other side produces and the comparison can be bitwise throughout. */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gfxcore_mocks.h"

/* ---- the two sides ---- */

typedef struct sApi
{
    void (*NeedGeomEx)(unsigned int, unsigned int, unsigned int, unsigned int, int);
    void (*NeedGeom)(unsigned int, unsigned int, unsigned int, int);
    void (*NeedGeomExElem)(unsigned int, unsigned int);
    void (*NeedGeomNbElemSprites)(unsigned int);
    void (*NeedGeomElemNbSprites)(unsigned int, unsigned int);
    void (*End)(void);
    void (*SetQuadIndices)(void);
    void (*SetQuadUVs)(void);
    void (*RenderGeom)(unsigned int);
    void (*SetCurrent)(tGfxElem*, int16_t);
    void (*FlagSet)(tGfxElem*, int16_t, char, int);
    void (*SetMaterial)(tGfxElem*, int16_t, struct sGEOMAT*);
    int (*Request)(int);
    tGfxElem* (*FindById)(tGfxElem**, int16_t);
    void (*SetRenderState)(void);
    tGfxElem* (*Attached)(tGfxElem*);
    void (*FreeOne)(tGfxElem*);
    void (*Del)(tGfxElem**, int16_t);
    void (*Test)(tGfxElem*);
    int (*SetDeath)(tGfxElem**, tGfxElem*);
    void (*Render)(tGfxElem**, int);
    int (*Add)(tGfxElem**, char, struct sGameObjectMain*);
    int (*AddSorted)(tGfxElem**, char, uint16_t, struct sGameObjectMain*);
    Vec3* (*Getv)(tGfxElem*, int16_t, int);
    float (*Getf)(tGfxElem*, int16_t, int);
    int (*Geti)(tGfxElem*, int16_t, int);
    void (*Seti)(tGfxElem*, int16_t, int, int);
    void (*Setf)(tGfxElem*, int, int, float);
    void (*Setv)(tGfxElem*, int16_t, int, Vec3*);
    void (*CreateExplosion)(float, float, Vec3*, struct sGameObjectMain*, struct sGEOMAT*);
    void (*Free)(tGfxElem**, int);
    uint16_t* idCounter;
} tApi;

#define DECL(p) \
    void p##GFX_NeedGeomEx(unsigned int, unsigned int, unsigned int, unsigned int, int); \
    void p##GFX_NeedGeom(unsigned int, unsigned int, unsigned int, int); \
    void p##GFX_NeedGeomExElem(unsigned int, unsigned int); \
    void p##GFX_NeedGeomNbElemSprites(unsigned int); \
    void p##GFX_NeedGeomElemNbSprites(unsigned int, unsigned int); \
    void p##GFX_End(void); \
    void p##GFX_SetQuadIndices(void); \
    void p##GFX_SetQuadUVs(void); \
    void p##GFX_RenderGeom(unsigned int); \
    void p##GFX_SetCurrent(struct sGfxElem*, int16_t); \
    void p##GFX_FlagSet(tGfxElem*, int16_t, char, int); \
    void p##GFX_SetMaterial(tGfxElem*, int16_t, struct sGEOMAT*); \
    int p##GFX_i_Request(int); \
    struct sGfxElem* p##GFX_pst_FindById(struct sGfxElem**, int16_t); \
    void p##GFX_SetRenderState(void); \
    struct sGfxElem* p##GFX_Attached(struct sGfxElem*); \
    void p##GFX_FreeOne(tGfxElem*); \
    void p##GFX_Del(tGfxElem**, int16_t); \
    void p##GFX_Test(tGfxElem*); \
    int p##GFX_i_SetDeath(struct sGfxElem**, struct sGfxElem*); \
    void p##GFX_Render(tGfxElem**, int); \
    int p##GFX_i_Add(tGfxElem**, char, struct sGameObjectMain*); \
    int p##GFX_i_AddSorted(tGfxElem**, char, uint16_t, struct sGameObjectMain*); \
    Vec3* p##GFX_pst_Getv(tGfxElem*, int16_t, int); \
    float p##GFX_f_Getf(tGfxElem*, int16_t, int); \
    int p##GFX_i_Geti(tGfxElem*, int16_t, int); \
    void p##GFX_Seti(tGfxElem*, int16_t, int, int); \
    void p##GFX_Setf(tGfxElem*, int, int, float); \
    void p##GFX_Setv(tGfxElem*, int16_t, int, Vec3*); \
    void p##GFX_CreateExplosion(float, float, Vec3*, struct sGameObjectMain*, struct sGEOMAT*); \
    void p##GFX_Free(struct sGfxElem**, int); \
    extern uint16_t p##GFX_sguw_Id;

DECL(BASE_)
DECL(NEW_)

#define FILL(p) { \
    p##GFX_NeedGeomEx, p##GFX_NeedGeom, p##GFX_NeedGeomExElem, \
    p##GFX_NeedGeomNbElemSprites, p##GFX_NeedGeomElemNbSprites, p##GFX_End, \
    p##GFX_SetQuadIndices, p##GFX_SetQuadUVs, p##GFX_RenderGeom, \
    p##GFX_SetCurrent, p##GFX_FlagSet, p##GFX_SetMaterial, p##GFX_i_Request, \
    p##GFX_pst_FindById, p##GFX_SetRenderState, p##GFX_Attached, \
    p##GFX_FreeOne, p##GFX_Del, p##GFX_Test, p##GFX_i_SetDeath, \
    p##GFX_Render, p##GFX_i_Add, p##GFX_i_AddSorted, p##GFX_pst_Getv, \
    p##GFX_f_Getf, p##GFX_i_Geti, p##GFX_Seti, p##GFX_Setf, p##GFX_Setv, \
    p##GFX_CreateExplosion, p##GFX_Free, &p##GFX_sguw_Id }

static const tApi API_base = FILL(BASE_);
static const tApi API_new = FILL(NEW_);

extern int NEW_ga_Paramf_First[], NEW_ga_Paramf_Number[];
extern int NEW_ga_Parami_First[], NEW_ga_Parami_Number[];
extern int NEW_ga_Paramv_First[], NEW_ga_Paramv_Number[];
extern int16_t NEW_gaa_Paramf_Offset[], NEW_gaa_Parami_Offset[], NEW_gaa_Paramv_Offset[];
extern int BASE_ga_Paramf_First[], BASE_ga_Paramf_Number[];
extern int BASE_ga_Parami_First[], BASE_ga_Parami_Number[];
extern int BASE_ga_Paramv_First[], BASE_ga_Paramv_Number[];
extern int16_t BASE_gaa_Paramf_Offset[], BASE_gaa_Parami_Offset[], BASE_gaa_Paramv_Offset[];

/* ---- what a case records ---- */

#define SNAP_ARENA (1024 * 1024)

typedef struct sGlobals
{
    unsigned long long rli;
    unsigned int maxRli;
    unsigned long long go;
    unsigned long long geo;
    unsigned int maxPoints;
    unsigned int maxUVs;
    unsigned int maxTriangles[64];
    unsigned int maxElements;
    unsigned int maxElemSprites;
    unsigned long long current;
    unsigned long long currentList;
    int maxSprites[32];
    int globalId;
    unsigned int idCounter;
    unsigned long long listHead;
    int geoIsInFix;
} tGlobals;

typedef struct sSnap
{
    tGlobals g;
    unsigned int arenaUsed;
    unsigned char arena[SNAP_ARENA];
    unsigned char dd[sizeof(tGdi_DisplayData)];
    int logLen;
    tGfxcCall log[8192];
    int logOver;
    /* whatever the case itself chose to record */
    int ret[64];
    int retCount;
} tSnap;

static tSnap SNAP[2];
static tGfxElem* gList;

static void Snapshot(tSnap* s, const tApi* api)
{
    memset(&s->g, 0, sizeof(s->g));
    s->g.rli = GFXC_P(GFX_pul_RLI);
    s->g.maxRli = GFX_ul_MaxRLI;
    s->g.go = GFXC_P(GFX_gpst_GO);
    s->g.geo = GFXC_P(GFX_gpst_Geo);
    s->g.maxPoints = GFX_ul_MaxPoints;
    s->g.maxUVs = GFX_ul_MaxUVs;
    memcpy(s->g.maxTriangles, GFX_aul_MaxTriangles, sizeof(s->g.maxTriangles));
    s->g.maxElements = GFX_ul_MaxElements;
    s->g.maxElemSprites = GFX_ul_MaxElemSprites;
    s->g.current = GFXC_P(GFX_gpst_Current);
    s->g.currentList = GFXC_P(GFX_ppst_CurrentList);
    memcpy(s->g.maxSprites, GFX_aul_MaxSprites, sizeof(s->g.maxSprites));
    s->g.globalId = GFX_i_GlobalID;
    s->g.idCounter = *api->idCounter;
    s->g.listHead = GFXC_P(gList);
    s->g.geoIsInFix = GEO_b_IsInFix;

    s->arenaUsed = GFXC_arenaUsed;
    if (s->arenaUsed > SNAP_ARENA)
    {
        fprintf(stderr, "snapshot arena too small (%u)\n", s->arenaUsed);
        exit(2);
    }
    memcpy(s->arena, GFXC_arena, s->arenaUsed);
    memcpy(s->dd, gpst_DX_stDD, sizeof(tGdi_DisplayData));
    s->logLen = GFXC_logLen;
    s->logOver = GFXC_logOverflow;
    if (s->logLen > 8192)
    {
        fprintf(stderr, "snapshot log too small (%d)\n", s->logLen);
        exit(2);
    }
    memcpy(s->log, GFXC_log, (size_t)s->logLen * sizeof(tGfxcCall));
}

static int gFail;
static int gCases;
static const char* gGroup;
static int gCaseId;

static void Fail(const char* what, const char* detail)
{
    if (gFail < 12)
        fprintf(stderr, "  %s case %d: %s %s\n", gGroup, gCaseId, what, detail ? detail : "");
    ++gFail;
}

static void Compare(void)
{
    int i, j;
    char buf[160];

    if (memcmp(&SNAP[0].g, &SNAP[1].g, sizeof(tGlobals)) != 0)
    {
        const unsigned char* a = (const unsigned char*)&SNAP[0].g;
        const unsigned char* b = (const unsigned char*)&SNAP[1].g;
        for (i = 0; i < (int)sizeof(tGlobals); ++i)
            if (a[i] != b[i]) break;
        sprintf(buf, "first differing byte at offset %d", i);
        Fail("globals differ,", buf);
    }
    if (SNAP[0].arenaUsed != SNAP[1].arenaUsed)
    {
        sprintf(buf, "%u vs %u", SNAP[0].arenaUsed, SNAP[1].arenaUsed);
        Fail("arena use differs,", buf);
    }
    else if (memcmp(SNAP[0].arena, SNAP[1].arena, SNAP[0].arenaUsed) != 0)
    {
        for (i = 0; i < (int)SNAP[0].arenaUsed; ++i)
            if (SNAP[0].arena[i] != SNAP[1].arena[i]) break;
        sprintf(buf, "first differing byte at arena offset %d (%02x vs %02x)",
                i, SNAP[0].arena[i], SNAP[1].arena[i]);
        Fail("arena differs,", buf);
    }
    if (memcmp(SNAP[0].dd, SNAP[1].dd, sizeof(tGdi_DisplayData)) != 0)
    {
        for (i = 0; i < (int)sizeof(tGdi_DisplayData); ++i)
            if (SNAP[0].dd[i] != SNAP[1].dd[i]) break;
        sprintf(buf, "first differing byte at offset %d", i);
        Fail("display data differs,", buf);
    }
    if (SNAP[0].logLen != SNAP[1].logLen)
    {
        sprintf(buf, "%d vs %d calls", SNAP[0].logLen, SNAP[1].logLen);
        Fail("call count differs,", buf);
    }
    else
    {
        for (i = 0; i < SNAP[0].logLen; ++i)
        {
            if (SNAP[0].log[i].fn != SNAP[1].log[i].fn)
            {
                sprintf(buf, "call %d: %s vs %s", i, SNAP[0].log[i].fn, SNAP[1].log[i].fn);
                Fail("call sequence differs,", buf);
                break;
            }
            if (SNAP[0].log[i].state != SNAP[1].log[i].state
                || SNAP[0].log[i].drawMask != SNAP[1].log[i].drawMask)
            {
                sprintf(buf, "call %d (%s): state %08x/%08x vs %08x/%08x", i,
                        SNAP[0].log[i].fn, SNAP[0].log[i].state, SNAP[0].log[i].drawMask,
                        SNAP[1].log[i].state, SNAP[1].log[i].drawMask);
                Fail("render state at a call differs,", buf);
                break;
            }
            for (j = 0; j < GFXC_ARGS; ++j)
                if (SNAP[0].log[i].arg[j] != SNAP[1].log[i].arg[j])
                {
                    sprintf(buf, "call %d (%s) arg %d: %llx vs %llx", i,
                            SNAP[0].log[i].fn, j, SNAP[0].log[i].arg[j], SNAP[1].log[i].arg[j]);
                    Fail("call argument differs,", buf);
                    i = SNAP[0].logLen;
                    break;
                }
        }
    }
    if (SNAP[0].retCount != SNAP[1].retCount
        || memcmp(SNAP[0].ret, SNAP[1].ret, (size_t)SNAP[0].retCount * sizeof(int)) != 0)
        Fail("returned values differ", 0);
    ++gCases;
}

/* ---- the case bodies ---- */

static int gRet[64];
static int gRetN;

/* A game object GFX_Free really dereferences.  Arena-allocated, so the
   two sides see the same address. */
static struct sGameObjectMain* Gao(unsigned int flags2)
{
    tGameObjectMain* g = (tGameObjectMain*)MEM_p_Alloc(sizeof(tGameObjectMain));
    g->flags2 = flags2;
    return (struct sGameObjectMain*)g;
}

static void Rec(int v)
{
    if (gRetN < 64)
        gRet[gRetN++] = v;
}

/* Every live element's own state, recorded directly rather than left
   to show up in the allocator's bytes: a freed block is scrubbed, so
   a field that is only ever read back out of a dead element would go
   unchecked. */
static void RecList(tGfxElem* head)
{
    tGfxElem* e;
    int n = 0;
    for (e = head; e && n < 12; e = e->prevGfxElem, ++n)
    {
        unsigned int life;
        memcpy(&life, &e->lifeTime, 4);
        Rec(e->id);
        Rec(e->type);
        Rec(e->gfxFlags);
        Rec((int)life);
    }
    Rec(n);
}

static void RecF(float f)
{
    unsigned int bits;
    memcpy(&bits, &f, 4);
    Rec((int)bits);
}

typedef void (*tCase)(const tApi*, int);

static void Run(const char* group, tCase fn, int n)
{
    int c, side;
    gGroup = group;
    for (c = 0; c < n; ++c)
    {
        gCaseId = c;
        for (side = 0; side < 2; ++side)
        {
            GFXC_Reset();
            gList = 0;
            gRetN = 0;
            *(side ? API_new.idCounter : API_base.idCounter) = 1024;
            fn(side ? &API_new : &API_base, c);
            memcpy(SNAP[side].ret, gRet, sizeof(gRet));
            SNAP[side].retCount = gRetN;
            Snapshot(&SNAP[side], side ? &API_new : &API_base);
        }
        Compare();
    }
}

/* --- group A: the geometry buffer --- */

static const unsigned int A_pts[] = { 0, 1, 4, 64, 100, 5000, 5001 };
static const unsigned int A_uv[] = { 0, 1, 4, 64 };
static const unsigned int A_chk[] = { 1, 2, 5 };
static const unsigned int A_tri[] = { 0, 1, 2, 40 };

#define A_N ((int)(sizeof(A_pts)/sizeof(A_pts[0])) * (int)(sizeof(A_uv)/sizeof(A_uv[0])) \
             * (int)(sizeof(A_chk)/sizeof(A_chk[0])) * (int)(sizeof(A_tri)/sizeof(A_tri[0])) * 2)

static void caseBuffer(const tApi* a, int c)
{
    int colors = c & 1;
    int k = c >> 1;
    unsigned int p = A_pts[k % 7];
    unsigned int u = A_uv[(k / 7) % 4];
    unsigned int ch = A_chk[(k / 28) % 3];
    unsigned int t = A_tri[(k / 84) % 4];

    a->NeedGeomEx(p, u, ch, t, colors);
    a->NeedGeomEx(p, u, ch, t, colors);
    a->NeedGeomEx(p + 7, u + 3, ch + 1, t + 5, !colors);
    a->NeedGeomEx(1, 1, 1, 1, colors);
    a->NeedGeom(p + 2, u + 2, t + 2, colors);
    a->NeedGeomExElem(0, t + 9);
    a->NeedGeomExElem(ch, t);
    a->NeedGeomExElem(ch + 40, t);
    a->NeedGeomExElem(1, 0);
    a->NeedGeomNbElemSprites(ch);
    a->NeedGeomNbElemSprites(ch);
    a->NeedGeomNbElemSprites(ch + 3);
    a->NeedGeomElemNbSprites(0, t + 1);
    a->NeedGeomElemNbSprites(ch, t);
    a->NeedGeomElemNbSprites(ch + 99, t);
    /* a second geometry wide enough that the per-chunk calls below
       address a chunk that exists -- without it every NeedGeomExElem
       falls out at the bound and the chunk capacities are untested */
    a->NeedGeomEx(8, 8, 4, 6, colors);
    a->NeedGeomExElem(2, t + 11);
    a->NeedGeomExElem(2, t + 11);
    a->NeedGeomExElem(2, 0);
    a->NeedGeomExElem(3, 0);
    a->NeedGeomExElem(1, 1);
    a->NeedGeomExElem(1, t);
    a->NeedGeomNbElemSprites(4);
    a->NeedGeomElemNbSprites(2, t + 3);
    a->NeedGeomElemNbSprites(2, 0);
    a->SetQuadIndices();
    a->SetQuadUVs();
    a->RenderGeom(0x10000u);
    a->SetRenderState();
    a->End();
    a->End();
}

/* --- group B: the element list --- */

#define B_N (23 * 8)

static void caseList(const tApi* a, int c)
{
    int type = c % 23;
    int shape = c / 23;
    tGfxElem* e;
    int id1, id2, id3;

    id1 = a->Add(&gList, (char)type, Gao(0));
    Rec(id1);
    id2 = a->Add(&gList, (char)((type + 1) % 24), Gao(0x4000000));
    Rec(id2);
    id3 = a->AddSorted(&gList, (char)type, (uint16_t)(shape * 100), Gao(0x8));
    Rec(id3);
    Rec(a->AddSorted(&gList, (char)type, (uint16_t)(shape * 100), 0));
    Rec(a->AddSorted(&gList, (char)type, 1024, 0));
    Rec(a->AddSorted(&gList, (char)type, (uint16_t)(shape * 50), 0));

    RecList(gList);
    a->SetCurrent(gList, (int16_t)id1);
    Rec((int)(GFX_gpst_Current != 0));
    a->SetCurrent(gList, 31337);
    Rec((int)(GFX_gpst_Current != 0));

    e = a->FindById(&gList, (int16_t)id2);
    Rec(e ? e->type : -99);
    e = a->FindById(&gList, 31337);
    Rec(e ? 1 : 0);

    a->FlagSet(gList, (int16_t)id1, (char)(shape & 7), 1);
    a->FlagSet(gList, (int16_t)id1, 2, 1);
    a->FlagSet(gList, (int16_t)id1, (char)(shape & 7), 0);
    a->FlagSet(gList, (int16_t)id2, 0, 1);
    a->SetMaterial(gList, (int16_t)id2, (struct sGEOMAT*)0x40);
    a->SetMaterial(gList, 31337, (struct sGEOMAT*)0x40);

    Rec((int)(a->Attached(gList) != 0));
    a->Test(gList);

    Rec(a->SetDeath(&gList, a->FindById(&gList, (int16_t)id1)));
    Rec(a->SetDeath(&gList, 0));

    a->Del(&gList, (int16_t)id2);
    a->Del(&gList, 31337);
    Rec(a->Request(shape * 500));

    a->Free(&gList, shape & 1 ? 20 : 0);
    Rec((int)(gList != 0));
    a->Free(&gList, 20);
}

/* --- group C: the parameter tables --- */

#define C_SPAN 28
#define C_N (23 * C_SPAN)

static void caseParams(const tApi* a, int c)
{
    int type = c / C_SPAN;
    int d = (c % C_SPAN) - 2;
    tGfxElem* e;
    Vec3 v;
    Vec3* got;
    int pf, pi, pv;
    int k;

    e = (tGfxElem*)MEM_p_Alloc(sizeof(tGfxElem));
    e->type = (int8_t)type;
    e->id = 777;
    e->prevGfxElem = 0;
    e->privateData = MEM_p_Alloc(4096);
    for (k = 0; k < 4096; ++k)
        ((unsigned char*)e->privateData)[k] = (unsigned char)(k * 7 + 3);
    gList = e;

    pf = NEW_ga_Paramf_First[type] + d;
    pi = NEW_ga_Parami_First[type] + d;
    pv = NEW_ga_Paramv_First[type] + d;

    GFX_gpst_Current = 0;
    RecF(a->Getf(e, 777, pf));
    GFX_gpst_Current = 0;
    Rec(a->Geti(e, 777, pi));
    GFX_gpst_Current = 0;
    got = a->Getv(e, 777, pv);
    Rec(got ? (int)((unsigned char*)got - (unsigned char*)e->privateData) : -1);

    GFX_gpst_Current = 0;
    a->Setf(e, 777, pf, 1.5f + (float)d);
    GFX_gpst_Current = 0;
    a->Seti(e, 777, pi, 0x11223344 + d);
    v.x = 1.0f + d;
    v.y = 2.0f + d;
    v.z = 3.0f + d;
    GFX_gpst_Current = 0;
    a->Setv(e, 777, pv, &v);

    /* and once more through the cached-current path */
    a->Setf(e, 777, pf, -2.25f);
    a->Seti(e, 777, pi, -7);
    a->Setv(e, 777, pv, &v);

    /* no element with this id at all */
    GFX_gpst_Current = 0;
    RecF(a->Getf(e, 4, pf));
    GFX_gpst_Current = 0;
    Rec(a->Geti(e, 4, pi));
    GFX_gpst_Current = 0;
    Rec(a->Getv(e, 4, pv) ? 1 : 0);
    GFX_gpst_Current = 0;
    a->Setf(e, 4, pf, 9.0f);
    GFX_gpst_Current = 0;
    a->Seti(e, 4, pi, 9);
    GFX_gpst_Current = 0;
    a->Setv(e, 4, pv, &v);
}

/* --- group D: GFX_Render --- */

static const unsigned char D_flags[] = { 0, 1, 3, 5, 7, 9, 0x81, 0x89, 0x85, 0x0D };
#define D_NF ((int)(sizeof(D_flags)/sizeof(D_flags[0])))
#define D_N (23 * D_NF * 4)

static void caseRender(const tApi* a, int c)
{
    int type = c % 23;
    int fi = (c / 23) % D_NF;
    int mode = (c / (23 * D_NF)) % 4;
    int idA, idB;
    tGfxElem* e;

    idA = a->Add(&gList, (char)type, Gao(0));
    idB = a->Add(&gList, (char)((type + 5) % 23), Gao(0x4000000));
    Rec(idA);
    Rec(idB);

    e = a->FindById(&gList, (int16_t)idA);
    if (e)
    {
        e->gfxFlags = D_flags[fi];
        e->lifeTime = (mode & 1) ? 1.0f : 0.0f;
        e->material = (struct sGEOMAT*)0x200;
    }
    e = a->FindById(&gList, (int16_t)idB);
    if (e)
    {
        e->gfxFlags = D_flags[(fi + 3) % D_NF];
        e->lifeTime = (mode & 1) ? 0.0f : 2.0f;
    }

    RecList(gList);
    TIM_gf_dt = 0.5f;
    GFXC_renderResult = (mode & 2) ? 1 : 0;
    a->Render(&gList, mode & 1);
    GFXC_renderResult = (mode & 2) ? 0 : 1;
    a->Render(&gList, (mode >> 1) & 1);
    a->Render(&gList, 256);
    Rec((int)(gList != 0));
    a->Free(&gList, 0);
}

/* --- group E: GFX_CreateExplosion --- */

static const float E_size[] = { 1.0f, 0.5f, 12.75f, 1e-3f, 250.0f };
static const float E_dur[] = { 1.0f, 0.25f, 3.0f, 100.0f, 0.0f };
#define E_N (5 * 5 * 2)

static void caseExplosion(const tApi* a, int c)
{
    Vec3 pos;
    int s = c % 5;
    int d = (c / 5) % 5;
    int withGeom = (c / 25) & 1;

    if (withGeom)
        a->NeedGeom(4, 4, 2, 1);
    pos.x = 3.5f;
    pos.y = -2.25f;
    pos.z = 7.125f;
    a->CreateExplosion(E_size[s], E_dur[d], &pos, Gao(0), (struct sGEOMAT*)0x44);
    RecF(pos.x);
    RecF(pos.y);
    RecF(pos.z);
    gList = GFXC_world.gfxCurrentList;
    Rec((int)(gList != 0));
    a->Render(&gList, 0);
    GFXC_world.gfxCurrentList = gList;
    a->Free(&GFXC_world.gfxCurrentList, 0);
    gList = GFXC_world.gfxCurrentList;
}

/* --- group F: the cached current element ---
 *
 * GFX_SetCurrent short-circuits when the element it is already holding
 * carries the id being asked for, whichever list that element came
 * from.  Two lists that share an id are the only thing that can see
 * the short-circuit, and without them a defect that drops it reads as
 * equivalent. */

#define F_N 12

static int ArenaOff(const void* p)
{
    if (!p)
        return -1;
    return (int)((const unsigned char*)p - GFXC_arena);
}

static tGfxElem* MakeElem(int type, int id, tGfxElem* prev)
{
    tGfxElem* e = (tGfxElem*)MEM_p_Alloc(sizeof(tGfxElem));
    e->type = (int8_t)type;
    e->gfxFlags = 1;
    e->id = (int16_t)id;
    e->privateData = MEM_p_Alloc(4096);
    memset(e->privateData, 0, 4096);
    e->material = 0;
    e->aiGameObject = 0;
    e->prevGfxElem = prev;
    e->lifeTime = 100.0f;
    return e;
}

static void caseCache(const tApi* a, int c)
{
    tGfxElem* listA;
    tGfxElem* listB;
    int type = c % 6;
    int shared = 500;
    Vec3 v;

    listA = MakeElem(type, 501, 0);
    listA = MakeElem(type, shared, listA);
    listB = MakeElem(type, 502, 0);
    listB = MakeElem(type, shared, listB);

    v.x = 1.0f;
    v.y = 2.0f;
    v.z = 3.0f;

    GFX_gpst_Current = 0;
    a->SetCurrent(listA, (int16_t)shared);
    Rec(ArenaOff(GFX_gpst_Current));
    /* the same id, a different list: the cache decides the answer */
    a->SetCurrent(listB, (int16_t)shared);
    Rec(ArenaOff(GFX_gpst_Current));
    a->Seti(listB, (int16_t)shared, NEW_ga_Parami_First[type] + (c / 6), 0x6B6B6B6B);
    a->Setf(listB, (int16_t)shared, NEW_ga_Paramf_First[type] + (c / 6), 4.5f);
    a->Setv(listB, (int16_t)shared, NEW_ga_Paramv_First[type] + (c / 6), &v);
    Rec(ArenaOff(a->FindById(&listB, (int16_t)shared)));
    Rec(ArenaOff(a->Getv(listA, (int16_t)shared, NEW_ga_Paramv_First[type] + (c / 6))));

    /* and a different id on the other list, which must miss the cache */
    a->SetCurrent(listB, 502);
    Rec(ArenaOff(GFX_gpst_Current));
    a->SetCurrent(listA, 502);
    Rec(ArenaOff(GFX_gpst_Current));
    a->FlagSet(listA, 502, 1, 1);
    a->Del(&listB, 502);
    Rec(ArenaOff(listB));
}

/* ---- main ---- */

static void TableCheck(void)
{
    int bad = 0;
    bad |= memcmp(BASE_ga_Paramf_First, NEW_ga_Paramf_First, 23 * sizeof(int)) != 0;
    bad |= memcmp(BASE_ga_Paramf_Number, NEW_ga_Paramf_Number, 23 * sizeof(int)) != 0;
    bad |= memcmp(BASE_ga_Parami_First, NEW_ga_Parami_First, 23 * sizeof(int)) != 0;
    bad |= memcmp(BASE_ga_Parami_Number, NEW_ga_Parami_Number, 23 * sizeof(int)) != 0;
    bad |= memcmp(BASE_ga_Paramv_First, NEW_ga_Paramv_First, 23 * sizeof(int)) != 0;
    bad |= memcmp(BASE_ga_Paramv_Number, NEW_ga_Paramv_Number, 23 * sizeof(int)) != 0;
    bad |= memcmp(BASE_gaa_Paramf_Offset, NEW_gaa_Paramf_Offset, 23 * 21 * sizeof(int16_t)) != 0;
    bad |= memcmp(BASE_gaa_Parami_Offset, NEW_gaa_Parami_Offset, 23 * 20 * sizeof(int16_t)) != 0;
    bad |= memcmp(BASE_gaa_Paramv_Offset, NEW_gaa_Paramv_Offset, 23 * 20 * sizeof(int16_t)) != 0;
    if (bad)
    {
        fprintf(stderr, "  parameter tables differ between the two copies\n");
        ++gFail;
    }
}

int main(void)
{
    printf("=== gfx.c ===\n");
    TableCheck();
    Run("buffer", caseBuffer, A_N);
    Run("list", caseList, B_N);
    Run("params", caseParams, C_N);
    Run("render", caseRender, D_N);
    Run("explosion", caseExplosion, E_N);
    Run("cache", caseCache, F_N);
    printf("  %d cases compared, %d differing\n", gCases, gFail);
    if (gFail)
        printf("  FAIL\n");
    else
        printf("  all equivalent\n");
    return gFail ? 1 : 0;
}
