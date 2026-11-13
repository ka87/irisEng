/* The differential test for ok3.c.
 *
 * The frozen pre-rewrite copy (tests/baseline_ok3.c) and the rewrite go
 * into one binary with their symbols prefixed BASE_ and NEW_, run over
 * identical state, and are compared on the values they return, every
 * byte the allocator handed out, and the ordered call sequence.
 *
 * The real math.c is linked, so MATH_SubstractVec, MATH_CrossProduct
 * and MATH_VecMultScalar are the engine's own.
 *
 * One limitation, stated rather than papered over: ok3.c writes the
 * sizes of two of its own structs as the literals 8 and 32, which are
 * their sizes in a 32-bit build.  On this 64-bit host they are 16 and
 * 40, so every memcpy length and block size computed from those
 * literals covers fewer elements here than it does on the engine's
 * target.  Both copies compute the same wrong length, so the
 * comparison is sound; what the test does not reproduce is how far
 * those copies reach on Win32.  The functions that index properly --
 * every one of the geometry primitives, TriangleTouchBox,
 * OptimizeBoxBV and GetNumberOfFacesInBox -- are unaffected. */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "ok3_mocks.h"

typedef struct sApi
{
    int (*FacePlane)(Vec3*, Vec3*, Vec3*);
    int (*Bevel2D)(Vec3*, Vec3*, Vec3*);
    int (*Bevel3D)(Vec3*, Vec3*, Vec3*);
    int (*CheckPoint)(Vec3*, Vec3*, float, int, Vec3*, Vec3*);
    BOOL (*CheckLine)(Vec3*, Vec3*, char, Vec3*, Vec3*);
    unsigned int (*FacesInBox)(tCobOK3AABBoxElem*);
    void (*GetTriangle)(void*, uint16_t, uint16_t, Vec3**, Vec3**, Vec3**, int);
    void (*OptimizeBoxBV)(tCobOK3AABBoxElem*, void*, int);
    BOOL (*TouchBox)(Vec3*, Vec3*, Vec3*, Vec3*, Vec3*);
    void (*Optimize)(tCobOK3*, void*, int);
    int (*Subdivide)(void*, unsigned int, int*, int);
    void (*Build)(void*, int, int);
} tApi;

#define DECL(p) \
    int p##Face_Plane(Vec3*, Vec3*, Vec3*); \
    int p##bevel_2D(Vec3*, Vec3*, Vec3*); \
    int p##bevel_3D(Vec3*, Vec3*, Vec3*); \
    int p##check_point(Vec3*, Vec3*, float, int, Vec3*, Vec3*); \
    BOOL p##check_line(Vec3*, Vec3*, char, Vec3*, Vec3*); \
    unsigned int p##COL_OK3_GetNumberOfFacesInBox(tCobOK3AABBoxElem*); \
    void p##COL_pst_OK3_GetTriangle(void*, uint16_t, uint16_t, Vec3**, Vec3**, Vec3**, int); \
    void p##COL_OK3_OptimizeBoxBV(tCobOK3AABBoxElem*, void*, int); \
    BOOL p##COL_OK3_TriangleTouchBox(Vec3*, Vec3*, Vec3*, Vec3*, Vec3*); \
    void p##COL_OK3_Optimize(tCobOK3*, void*, int); \
    int p##COL_OK3_SubdivideBox(void*, unsigned int, int*, int); \
    void p##COL_OK3_Build(void*, int, int);

DECL(BASE_)
DECL(NEW_)

#define FILL(p) { \
    p##Face_Plane, p##bevel_2D, p##bevel_3D, p##check_point, p##check_line, \
    p##COL_OK3_GetNumberOfFacesInBox, p##COL_pst_OK3_GetTriangle, \
    p##COL_OK3_OptimizeBoxBV, p##COL_OK3_TriangleTouchBox, p##COL_OK3_Optimize, \
    p##COL_OK3_SubdivideBox, p##COL_OK3_Build }

static const tApi API_base = FILL(BASE_);
static const tApi API_new = FILL(NEW_);

/* ---- snapshots ---- */

#define SNAP_ARENA (8 * 1024 * 1024)
#define SNAP_LOG 200000
#define SNAP_RET 4096

typedef struct sSnap
{
    unsigned int arenaUsed;
    unsigned char arena[SNAP_ARENA];
    int realloc;
    int logLen;
    int logOver;
    tOk3Call log[SNAP_LOG];
    unsigned int ret[SNAP_RET];
    int retCount;
} tSnap;

static tSnap* SNAP[2];
static unsigned int gRet[SNAP_RET];
static int gRetN;
static int gFail;
static int gCases;
static long gValues;
static const char* gGroup;
static int gCaseId;

static void Rec(unsigned int v)
{
    if (gRetN < SNAP_RET)
        gRet[gRetN++] = v;
    ++gValues;
}

static void RecF(float f)
{
    unsigned int bits;
    memcpy(&bits, &f, 4);
    Rec(bits);
}

static void RecV(const Vec3* v)
{
    RecF(v->x);
    RecF(v->y);
    RecF(v->z);
}

static void Fail(const char* what, const char* detail)
{
    if (gFail < 12)
        fprintf(stderr, "  %s case %d: %s %s\n", gGroup, gCaseId, what, detail ? detail : "");
    ++gFail;
}

static void Snapshot(tSnap* s)
{
    s->arenaUsed = OK3M_arenaUsed;
    if (s->arenaUsed > SNAP_ARENA)
    {
        fprintf(stderr, "snapshot arena too small (%u)\n", s->arenaUsed);
        exit(2);
    }
    memcpy(s->arena, OK3M_arena, s->arenaUsed);
    s->realloc = COL_OK3_Realloc;
    s->logLen = OK3M_logLen;
    s->logOver = OK3M_logOverflow;
    if (s->logLen > SNAP_LOG)
    {
        fprintf(stderr, "snapshot log too small (%d)\n", s->logLen);
        exit(2);
    }
    memcpy(s->log, OK3M_log, (size_t)s->logLen * sizeof(tOk3Call));
    memcpy(s->ret, gRet, sizeof(gRet));
    s->retCount = gRetN;
}

static void Compare(void)
{
    int i, j;
    char buf[200];

    if (SNAP[0]->retCount != SNAP[1]->retCount)
        Fail("value count differs", 0);
    else
    {
        for (i = 0; i < SNAP[0]->retCount; ++i)
            if (SNAP[0]->ret[i] != SNAP[1]->ret[i])
            {
                sprintf(buf, "value %d: %08x vs %08x", i, SNAP[0]->ret[i], SNAP[1]->ret[i]);
                Fail("result differs,", buf);
                break;
            }
    }
    if (SNAP[0]->arenaUsed != SNAP[1]->arenaUsed)
    {
        sprintf(buf, "%u vs %u", SNAP[0]->arenaUsed, SNAP[1]->arenaUsed);
        Fail("arena use differs,", buf);
    }
    else if (memcmp(SNAP[0]->arena, SNAP[1]->arena, SNAP[0]->arenaUsed) != 0)
    {
        for (i = 0; i < (int)SNAP[0]->arenaUsed; ++i)
            if (SNAP[0]->arena[i] != SNAP[1]->arena[i]) break;
        sprintf(buf, "first differing byte at arena offset %d (%02x vs %02x)",
                i, SNAP[0]->arena[i], SNAP[1]->arena[i]);
        Fail("arena differs,", buf);
    }
    if (SNAP[0]->realloc != SNAP[1]->realloc)
        Fail("realloc counter differs", 0);
    if (SNAP[0]->logLen != SNAP[1]->logLen)
    {
        sprintf(buf, "%d vs %d calls", SNAP[0]->logLen, SNAP[1]->logLen);
        Fail("call count differs,", buf);
    }
    else
    {
        for (i = 0; i < SNAP[0]->logLen; ++i)
        {
            if (SNAP[0]->log[i].fn != SNAP[1]->log[i].fn)
            {
                sprintf(buf, "call %d: %s vs %s", i, SNAP[0]->log[i].fn, SNAP[1]->log[i].fn);
                Fail("call sequence differs,", buf);
                break;
            }
            for (j = 0; j < OK3M_ARGS; ++j)
                if (SNAP[0]->log[i].arg[j] != SNAP[1]->log[i].arg[j])
                {
                    sprintf(buf, "call %d (%s) arg %d: %llx vs %llx", i,
                            SNAP[0]->log[i].fn, j, SNAP[0]->log[i].arg[j], SNAP[1]->log[i].arg[j]);
                    Fail("call argument differs,", buf);
                    i = SNAP[0]->logLen;
                    break;
                }
        }
    }
    ++gCases;
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
            OK3M_Reset();
            gRetN = 0;
            fn(side ? &API_new : &API_base, c);
            Snapshot(SNAP[side]);
        }
        Compare();
    }
}

/* ---- fixtures ---- */

static const float BOXES[][6] = {
    { -1.0f, -1.0f, -1.0f,  1.0f,  1.0f,  1.0f },
    {  0.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.0f },   /* zero extent */
    {  0.0f,  0.0f,  0.0f, 10.0f,  0.5f,  0.25f },  /* flat in y and z */
    {  2.0f, -3.0f,  5.0f,  2.0f,  4.0f,  9.0f },   /* zero extent in x */
    {  1.0f,  1.0f,  1.0f, -1.0f, -1.0f, -1.0f },   /* inverted */
    { -100.0f, -0.125f, 7.5f, 100.0f, 0.125f, 7.5f },
    { -0.5f, -0.5f, -0.5f, 0.5f, 0.5f, 0.5f },
};
#define NBOX ((int)(sizeof(BOXES)/sizeof(BOXES[0])))

static const float PTS[][3] = {
    {  0.0f,  0.0f,  0.0f },
    {  1.0f,  1.0f,  1.0f },
    { -1.0f, -1.0f, -1.0f },
    {  5.0f, -2.0f,  0.25f },
    {  0.0f,  0.0f,  1000.0f },
    { -3.5f,  0.125f, -0.125f },
    {  2.0f,  4.0f,  9.0f },
    {  0.5f,  0.5f,  0.5f },
    { 1e20f, -1e20f, 0.0f },
    { 0.0f, 0.0f, 0.0f },
};
#define NPT ((int)(sizeof(PTS)/sizeof(PTS[0])))

static void LoadBox(int b, Vec3* lo, Vec3* hi)
{
    lo->x = BOXES[b][0]; lo->y = BOXES[b][1]; lo->z = BOXES[b][2];
    hi->x = BOXES[b][3]; hi->y = BOXES[b][4]; hi->z = BOXES[b][5];
}

static void LoadPt(int p, Vec3* v)
{
    v->x = PTS[p][0]; v->y = PTS[p][1]; v->z = PTS[p][2];
}

/* --- group 1: the outcode primitives --- */

#define G1_N (NBOX * NPT)

static void caseOutcodes(const tApi* a, int c)
{
    Vec3 lo, hi, p, q;
    int m;
    LoadBox(c / NPT, &lo, &hi);
    LoadPt(c % NPT, &p);

    Rec((unsigned int)a->FacePlane(&p, &lo, &hi));
    Rec((unsigned int)a->Bevel2D(&p, &lo, &hi));
    Rec((unsigned int)a->Bevel3D(&p, &lo, &hi));
    for (m = 0; m < NPT; ++m)
    {
        LoadPt(m, &q);
        Rec((unsigned int)a->CheckPoint(&p, &q, 0.0f, 63, &lo, &hi));
        Rec((unsigned int)a->CheckPoint(&p, &q, 0.5f, 63, &lo, &hi));
        Rec((unsigned int)a->CheckPoint(&p, &q, 1.0f, 31, &lo, &hi));
        Rec((unsigned int)a->CheckPoint(&p, &q, -2.5f, 0, &lo, &hi));
    }
    for (m = 0; m < 64; ++m)
    {
        LoadPt((c + m) % NPT, &q);
        Rec((unsigned int)(unsigned char)a->CheckLine(&p, &q, (char)m, &lo, &hi));
    }
}

/* --- group 2: triangle against box --- */

#define G2_N (NBOX * NPT * 4)

static void caseTouch(const tApi* a, int c)
{
    Vec3 lo, hi, p1, p2, p3;
    int b = c % NBOX;
    int s = (c / NBOX) % NPT;
    int k = (c / (NBOX * NPT)) % 4;

    LoadBox(b, &lo, &hi);
    LoadPt(s, &p1);
    LoadPt((s + 1 + k) % NPT, &p2);
    LoadPt((s + 3 + 2 * k) % NPT, &p3);
    if (k == 3)
        p3 = p2;                              /* degenerate, zero area */
    Rec((unsigned int)(unsigned char)a->TouchBox(&p1, &p2, &p3, &lo, &hi));
    Rec((unsigned int)(unsigned char)a->TouchBox(&p3, &p2, &p1, &lo, &hi));
}

/* --- shared structure builders --- */

typedef struct sFixture
{
    tGEOGRO* geo;
    tCobMeshObjectMesh* mesh;
    tCobOK3* ok3;
} tFixture;

static Vec3* MakeVerts(int n, int seed, float scale)
{
    Vec3* v = (Vec3*)OK3M_Bump(sizeof(Vec3) * (n ? n : 1), 16);
    int i;
    for (i = 0; i < n; ++i)
    {
        v[i].x = (float)(((i * 7 + seed) % 23) - 11) * 0.5f * scale;
        v[i].y = (float)(((i * 11 + seed) % 17) - 8) * 0.75f * scale;
        v[i].z = (float)(((i * 5 + seed) % 29) - 14) * 0.25f * scale;
    }
    return v;
}

static tGEOGRO* MakeGeo(int vertexNum, int chunkNum, int trisPerChunk, int seed, float scale)
{
    tGEOGRO* g = (tGEOGRO*)OK3M_Bump(sizeof(tGEOGRO), 16);
    int i, j;
    memset(g, 0, sizeof(tGEOGRO));
    g->vertexNum = vertexNum;
    g->vertices = MakeVerts(vertexNum, seed, scale);
    g->triangleChunkCount = chunkNum;
    g->triangleChunks = (tGeoTriangleChunks*)OK3M_Bump(sizeof(tGeoTriangleChunks) * (chunkNum ? chunkNum : 1), 16);
    memset(g->triangleChunks, 0, sizeof(tGeoTriangleChunks) * (chunkNum ? chunkNum : 1));
    for (i = 0; i < chunkNum; ++i)
    {
        int n = trisPerChunk + (i % 3);
        g->triangleChunks[i].triangleNum = n;
        g->triangleChunks[i].triangleChunkElems =
            (tGeoTriangleChunkElem*)OK3M_Bump(sizeof(tGeoTriangleChunkElem) * (n ? n : 1), 16);
        memset(g->triangleChunks[i].triangleChunkElems, 0, sizeof(tGeoTriangleChunkElem) * (n ? n : 1));
        for (j = 0; j < n; ++j)
        {
            g->triangleChunks[i].triangleChunkElems[j].v1 = (uint16_t)((j * 3 + i) % vertexNum);
            g->triangleChunks[i].triangleChunkElems[j].v2 = (uint16_t)((j * 3 + 1 + i) % vertexNum);
            g->triangleChunks[i].triangleChunkElems[j].v3 = (uint16_t)((j * 3 + 2 + i) % vertexNum);
        }
    }
    return g;
}

static tCobMeshObjectMesh* MakeMesh(int vertexNum, int chunkNum, int trisPerChunk, int seed, float scale)
{
    tCobMeshObjectMesh* m = (tCobMeshObjectMesh*)OK3M_Bump(sizeof(tCobMeshObjectMesh), 16);
    int i, j, faces = 0;
    memset(m, 0, sizeof(tCobMeshObjectMesh));
    m->vertexCount = vertexNum;
    m->vertices = MakeVerts(vertexNum, seed, scale);
    m->meshChunkElemNum = chunkNum;
    m->meshChunkElems = (tCobMeshChunkElem*)OK3M_Bump(sizeof(tCobMeshChunkElem) * (chunkNum ? chunkNum : 1), 16);
    memset(m->meshChunkElems, 0, sizeof(tCobMeshChunkElem) * (chunkNum ? chunkNum : 1));
    for (i = 0; i < chunkNum; ++i)
    {
        int n = trisPerChunk + (i % 3);
        m->meshChunkElems[i].elemNum = (uint16_t)n;
        m->meshChunkElems[i].triangleElems =
            (tCobMeshTriangleElem*)OK3M_Bump(sizeof(tCobMeshTriangleElem) * (n ? n : 1), 16);
        memset(m->meshChunkElems[i].triangleElems, 0, sizeof(tCobMeshTriangleElem) * (n ? n : 1));
        for (j = 0; j < n; ++j)
        {
            m->meshChunkElems[i].triangleElems[j].v1 = (uint16_t)((j * 3 + i) % vertexNum);
            m->meshChunkElems[i].triangleElems[j].v2 = (uint16_t)((j * 3 + 1 + i) % vertexNum);
            m->meshChunkElems[i].triangleElems[j].v3 = (uint16_t)((j * 3 + 2 + i) % vertexNum);
        }
        faces += n;
    }
    m->faceCount = faces;
    return m;
}

/* An OK3 with `boxCount` boxes, each with `chunkCount` chunk entries;
   `emptyMask` says which of those entries get a count of zero. */
static tCobOK3* MakeOk3(int boxCount, int chunkCount, unsigned int emptyMask, int idsPerChunk)
{
    tCobOK3* o = (tCobOK3*)OK3M_Bump(sizeof(tCobOK3), 16);
    int b, c, i;
    memset(o, 0, sizeof(tCobOK3));
    o->num = boxCount;
    o->faceTagInts = 0;
    /* one box past the count, with a splittable extent and no chunks.
       COL_OK3_SubdivideBox admits boxIdx == num, so without a defined
       box there the two copies would be compared on whatever the
       allocator left behind. */
    o->aaBoxes = (tCobOK3AABBoxElem*)OK3M_Bump(sizeof(tCobOK3AABBoxElem) * (boxCount + 1), 16);
    memset(o->aaBoxes, 0, sizeof(tCobOK3AABBoxElem) * (boxCount + 1));
    o->aaBoxes[boxCount].vec2.x = 8.0f;
    o->aaBoxes[boxCount].vec2.y = 8.0f;
    o->aaBoxes[boxCount].vec2.z = 8.0f;
    o->aaBoxes[boxCount].chunkElems =
        (tCobOK3AABBoxElemChunkElem*)OK3M_Bump(sizeof(tCobOK3AABBoxElemChunkElem), 16);
    memset(o->aaBoxes[boxCount].chunkElems, 0, sizeof(tCobOK3AABBoxElemChunkElem));
    for (b = 0; b < boxCount; ++b)
    {
        o->aaBoxes[b].vec1.x = -4.0f - b;
        o->aaBoxes[b].vec1.y = -3.0f - b;
        o->aaBoxes[b].vec1.z = -2.0f - b;
        o->aaBoxes[b].vec2.x = 4.0f + b;
        o->aaBoxes[b].vec2.y = 3.0f + b;
        o->aaBoxes[b].vec2.z = 2.0f + b;
        o->aaBoxes[b].num = chunkCount;
        o->aaBoxes[b].chunkElems =
            (tCobOK3AABBoxElemChunkElem*)OK3M_Bump(sizeof(tCobOK3AABBoxElemChunkElem) * (chunkCount ? chunkCount : 1), 16);
        memset(o->aaBoxes[b].chunkElems, 0,
               sizeof(tCobOK3AABBoxElemChunkElem) * (chunkCount ? chunkCount : 1));
        for (c = 0; c < chunkCount; ++c)
        {
            int n = (emptyMask >> ((b * chunkCount + c) % 32)) & 1 ? 0 : idsPerChunk;
            uint16_t* ids = (uint16_t*)OK3M_Bump(2 * (idsPerChunk + 8), 8);
            o->aaBoxes[b].chunkElems[c].triangleChunkID = (uint16_t)c;
            o->aaBoxes[b].chunkElems[c].num = (uint16_t)n;
            /* the id array is always allocated, even for an empty
               chunk: compaction moves a chunk entry with an 8-byte
               memcpy, which on this host carries the count across but
               not the pointer, so a null here faults on the next
               pass -- see the note at the top of this file */
            o->aaBoxes[b].chunkElems[c].triangleChunkElemIDs = ids;
            for (i = 0; i < idsPerChunk + 8; ++i)
                ids[i] = (uint16_t)i;
        }
    }
    return o;
}

/* --- group 3: the box queries --- */

#define G3_N 48

static void caseBoxQueries(const tApi* a, int c)
{
    tGEOGRO* geo = MakeGeo(40, 3, 2, c, 1.0f);
    tCobOK3* ok3 = MakeOk3(2, 3, (unsigned int)c, 1 + (c % 4));
    Vec3 *p1, *p2, *p3;
    int b, i;

    for (b = 0; b < 2; ++b)
        Rec(a->FacesInBox(&ok3->aaBoxes[b]));

    for (i = 0; i < 6; ++i)
    {
        a->GetTriangle(geo, (uint16_t)(i % 3), (uint16_t)(i % 2), &p1, &p2, &p3, 0);
        RecV(p1);
        RecV(p2);
        RecV(p3);
    }

    a->OptimizeBoxBV(0, geo, 0);
    a->OptimizeBoxBV(&ok3->aaBoxes[0], 0, 0);
    for (b = 0; b < 2; ++b)
    {
        a->OptimizeBoxBV(&ok3->aaBoxes[b], geo, 0);
        RecV(&ok3->aaBoxes[b].vec1);
        RecV(&ok3->aaBoxes[b].vec2);
    }
}

/* --- group 4: the chunk and box compaction --- */

#define G4_N 64

static void caseOptimize(const tApi* a, int c)
{
    int boxCount = 1 + (c % 4);
    int chunkCount = 1 + ((c / 4) % 4);
    tGEOGRO* geo = MakeGeo(40, 4, 2, c, 1.0f);
    tCobOK3* ok3 = MakeOk3(boxCount, chunkCount, (unsigned int)(c * 2654435761u), 2);
    unsigned int b;
    int j;

    /* Every box but the last keeps one live chunk, so the box that
       gets removed is always the tail one.  Removing a box in the
       middle copies the box after it with a 32-byte memcpy, which on
       this host drops the chunk pointer out of the struct and faults
       on the next pass -- see the note at the top of this file.  The
       tail removal moves nothing and exercises the bookkeeping. */
    for (b = 0; b + 1 < (unsigned int)boxCount; ++b)
        ok3->aaBoxes[b].chunkElems[0].num = 2;
    if ((c / 16) & 1)
        for (j = 0; j < chunkCount; ++j)
            ok3->aaBoxes[boxCount - 1].chunkElems[j].num = 0;

    a->Optimize(ok3, geo, 0);
    Rec(ok3->num);
    for (b = 0; b < ok3->num && b < 8; ++b)
    {
        Rec(ok3->aaBoxes[b].num);
        RecV(&ok3->aaBoxes[b].vec1);
        RecV(&ok3->aaBoxes[b].vec2);
        Rec((unsigned int)(ok3->aaBoxes[b].chunkElems != 0));
    }
}

/* --- group 5: the split --- */

static const float SPLIT_BOX[][6] = {
    { 0.0f, 0.0f, 0.0f, 8.0f, 8.0f, 8.0f },       /* cube, all three axes */
    { 0.0f, 0.0f, 0.0f, 8.0f, 0.5f, 0.5f },       /* long in x */
    { 0.0f, 0.0f, 0.0f, 0.5f, 8.0f, 0.5f },       /* long in y */
    { 0.0f, 0.0f, 0.0f, 0.5f, 0.5f, 8.0f },       /* long in z */
    { 0.0f, 0.0f, 0.0f, 0.5f, 0.5f, 0.5f },       /* too small to split */
    { 0.0f, 0.0f, 0.0f, 8.0f, 8.0f, 0.25f },      /* flat */
    { 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f },       /* zero extent */
    { 0.0f, 0.0f, 0.0f, 0.6f, 0.6f, 1.5f },       /* z between the 1.0 and 2.0 cuts */
    { 0.0f, 0.0f, 0.0f, 1.5f, 1.5f, 1.5f },       /* every axis between the cuts */
};
#define NSPLIT ((int)(sizeof(SPLIT_BOX)/sizeof(SPLIT_BOX[0])))
#define G5_N (NSPLIT * 10)

static void caseSubdivide(const tApi* a, int c)
{
    int s = c % NSPLIT;
    int f = c / NSPLIT;             /* 0..8 forced, 9 = natural */
    tGEOGRO* geo = MakeGeo(40, 2, 3, c, 1.0f);
    tCobOK3* ok3 = MakeOk3(1, 2, 0, 3);
    int forced = f;
    unsigned int b;

    geo->collisionPtr = ok3;
    ok3->aaBoxes[0].vec1.x = SPLIT_BOX[s][0];
    ok3->aaBoxes[0].vec1.y = SPLIT_BOX[s][1];
    ok3->aaBoxes[0].vec1.z = SPLIT_BOX[s][2];
    ok3->aaBoxes[0].vec2.x = SPLIT_BOX[s][3];
    ok3->aaBoxes[0].vec2.y = SPLIT_BOX[s][4];
    ok3->aaBoxes[0].vec2.z = SPLIT_BOX[s][5];

    Rec((unsigned int)a->Subdivide(0, 0, 0, 0));
    /* boxIdx exactly at the count: the guard admits it */
    {
        tGEOGRO* probeGeo = MakeGeo(40, 2, 3, c, 1.0f);
        tCobOK3* probe = MakeOk3(1, 2, 0, 3);
        unsigned int q;
        probeGeo->collisionPtr = probe;
        Rec((unsigned int)a->Subdivide(probeGeo, probe->num, 0, 0));
        Rec(probeGeo->collisionPtr->num);
        for (q = 0; q < probeGeo->collisionPtr->num && q < 16; ++q)
            RecV(&probeGeo->collisionPtr->aaBoxes[q].vec1);
    }
    /* fifty boxes, so that (num + subCount) % 50 == subCount and the
       realloc trigger's <= is separable from < */
    if ((c % 3) == 0)
    {
        tGEOGRO* bigGeo = MakeGeo(40, 2, 3, c, 1.0f);
        tCobOK3* big = MakeOk3(50, 1, 0, 2);
        unsigned int q;
        bigGeo->collisionPtr = big;
        big->aaBoxes[0].vec2.x = 8.0f;
        big->aaBoxes[0].vec2.y = 8.0f;
        big->aaBoxes[0].vec2.z = 8.0f;
        big->aaBoxes[0].vec1.x = 0.0f;
        big->aaBoxes[0].vec1.y = 0.0f;
        big->aaBoxes[0].vec1.z = 0.0f;
        Rec((unsigned int)a->Subdivide(bigGeo, 0, 0, 0));
        Rec(bigGeo->collisionPtr->num);
        Rec((unsigned int)COL_OK3_Realloc);
        for (q = 0; q < bigGeo->collisionPtr->num && q < 60; ++q)
            Rec(bigGeo->collisionPtr->aaBoxes[q].num);
    }
    Rec((unsigned int)a->Subdivide(geo, 0, f == 9 ? 0 : &forced, 0));
    Rec(ok3->num);
    for (b = 0; b < geo->collisionPtr->num && b < 16; ++b)
    {
        Rec(geo->collisionPtr->aaBoxes[b].num);
        RecV(&geo->collisionPtr->aaBoxes[b].vec1);
        RecV(&geo->collisionPtr->aaBoxes[b].vec2);
    }
    /* and once more, out of range */
    Rec((unsigned int)a->Subdivide(geo, geo->collisionPtr->num + 5, 0, 0));
}

/* --- group 6: the whole build --- */

#define G6_N 36

static void caseBuild(const tApi* a, int c)
{
    int kind = c % 6;
    int seed = c / 6;
    unsigned int b;

    if (kind < 3)
    {
        /* a geometry object: vertexNum under and over the 100 bound,
           and one whose extent sits between the 1.0 and 2.0 cuts */
        tGEOGRO* geo = MakeGeo(kind ? 120 : 60, 2, 3, seed, kind == 2 ? 0.15f : 1.0f);
        a->Build(geo, 0, 0);
        Rec((unsigned int)(geo->collisionPtr != 0));
        if (geo->collisionPtr)
        {
            Rec(geo->collisionPtr->num);
            for (b = 0; b < geo->collisionPtr->num && b < 24; ++b)
            {
                Rec(geo->collisionPtr->aaBoxes[b].num);
                RecV(&geo->collisionPtr->aaBoxes[b].vec1);
                RecV(&geo->collisionPtr->aaBoxes[b].vec2);
            }
        }
        /* a second call: no rebuild, then with rebuild */
        a->Build(geo, 0, 0);
        Rec((unsigned int)(geo->collisionPtr != 0));
        a->Build(geo, 0, 1);
        Rec((unsigned int)(geo->collisionPtr != 0));
        a->Build(0, 0, 0);
    }
    else
    {
        tCobMeshObjectMesh* mesh = MakeMesh(kind == 5 ? 120 : 60, 3, kind == 4 ? 30 : 2, seed,
                                            kind == 5 ? 0.15f : 1.0f);
        tGameObjectMain* gao = (tGameObjectMain*)OK3M_Bump(sizeof(tGameObjectMain), 16);
        tGameObjectExtended* ext = (tGameObjectExtended*)OK3M_Bump(sizeof(tGameObjectExtended), 16);
        tGameObjectCollMain* coll = (tGameObjectCollMain*)OK3M_Bump(sizeof(tGameObjectCollMain), 16);
        tColMap* colMap = (tColMap*)OK3M_Bump(sizeof(tColMap), 16);
        tColCollisionObject* obj = (tColCollisionObject*)OK3M_Bump(sizeof(tColCollisionObject), 16);
        tColCollisionObject** objPtr = (tColCollisionObject**)OK3M_Bump(sizeof(void*), 16);

        memset(gao, 0, sizeof(*gao));
        memset(ext, 0, sizeof(*ext));
        memset(coll, 0, sizeof(*coll));
        memset(colMap, 0, sizeof(*colMap));
        memset(obj, 0, sizeof(*obj));
        gao->flags = (seed & 1) ? 0x100 : 0;
        gao->extended = ext;
        ext->coll = coll;
        coll->colMap = colMap;
        colMap->collisionObjectPtr = (struct sColCollisionObject**)objPtr;
        *objPtr = obj;
        obj->objectType = (seed & 2) ? 5 : 4;
        obj->objectPtrByType = mesh;

        a->Build(gao, 1, 0);
        Rec((unsigned int)(mesh->ok3 != 0));
        Rec(obj->flags);
        if (mesh->ok3)
        {
            Rec(mesh->ok3->num);
            for (b = 0; b < mesh->ok3->num && b < 24; ++b)
            {
                Rec(mesh->ok3->aaBoxes[b].num);
                RecV(&mesh->ok3->aaBoxes[b].vec1);
                RecV(&mesh->ok3->aaBoxes[b].vec2);
            }
        }
        a->Build(gao, 1, 0);
        Rec((unsigned int)(mesh->ok3 != 0));
        a->Build(gao, 1, 1);
        Rec((unsigned int)(mesh->ok3 != 0));
        /* the chain broken one link at a time */
        coll->colMap = 0;
        a->Build(gao, 1, 0);
        ext->coll = 0;
        a->Build(gao, 1, 0);
        gao->extended = 0;
        a->Build(gao, 1, 0);
    }
}

int main(void)
{
    printf("=== ok3.c ===\n");
    printf("  host struct sizes: box %u, chunk entry %u (the file writes 32 and 8)\n",
           (unsigned int)sizeof(tCobOK3AABBoxElem),
           (unsigned int)sizeof(tCobOK3AABBoxElemChunkElem));
    SNAP[0] = (tSnap*)malloc(sizeof(tSnap));
    SNAP[1] = (tSnap*)malloc(sizeof(tSnap));
    if (!SNAP[0] || !SNAP[1])
        return 2;
    Run("outcodes", caseOutcodes, G1_N);
    Run("touch", caseTouch, G2_N);
    Run("boxqueries", caseBoxQueries, G3_N);
    Run("optimize", caseOptimize, G4_N);
    Run("subdivide", caseSubdivide, G5_N);
    Run("build", caseBuild, G6_N);
    printf("  %d cases compared, %ld values, %d differing\n", gCases, gValues / 2, gFail);
    if (gFail)
        printf("  FAIL\n");
    else
        printf("  all equivalent\n");
    return gFail ? 1 : 0;
}
