/* Differential test for collision.c functions 98-158 (the last third of
   the file), the frozen pre-rewrite copy against the rewrite, in one
   binary over identical state.

   What it covers, and why those:

   - the geometry -- INT_LowLevel_OBBoxOBBox, INT_HighLevel_OBBoxOBBox
     and INT_FullTriangleTriangle. These are pure arithmetic over their
     arguments and the rewrite moved locals between float, double and
     long double, which is exactly the class of change a reading cannot
     settle. Every comparison is bitwise.

   - the bounding-volume tests -- COL_b_SingleBVIn, COL_b_BVIn,
     COL_b_SingleBVOverlap, COL_b_BVOverlap. Hand-built game objects:
     a bounding volume and a matrix are plain structs and
     OBJ_pst_GetAbsoluteMatrix is a one-line getter.

   - the report accessors -- all eighteen of them, over a hand-built
     record list. These are what the AI layer actually calls, and the
     rewrite turned every one of their goto-driven scans into a for
     loop, so they are the ones most likely to go wrong.

   - the zone accessors and the four ZDE collide entry points.

   - the edge-search chain COL_FindEdgeInFollowDirection ->
     COL_GetZConnected -> COL_GetPointConnectedTriangleWithMuchHorizontalEdge
     -> COL_ComputeJumpPoint, over a hand-built collision mesh. That
     chain had the heaviest rewriting in the range: an unreachable
     v16, a two-pass inner loop and three inlined copies of one edge
     measurement.

   What it does not cover, and why: the four loader callbacks need
   BIG_pc_ReadFileTmp and a real .big file; COL_ColMap_RayCast and
   COL_Visual_RayCast_OneObject hang their results off the single
   shared COL_gst_Ray, which the two copies would have to take turns
   with, and need a loaded geometry object; COL_SwapToSpecific's
   result is a pointer difference across a realloc, so the two copies
   cannot be given the same answer; REC_MainCall drives the dynamics.

   Every value compared here is expected to be BIT-IDENTICAL. */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "../gameobject.h"
#include "../collision.h"
#include "../world.h"
#include "../tables.h"
#include "../snp.h"

/* ---- the two copies ---------------------------------------------- */

#define D(ret, name, args) ret NEW_##name args; ret BASE_##name args;

D(BOOL, INT_LowLevel_OBBoxOBBox, (float, float, float, float, float, float, Mtx4x4*))
D(BOOL, INT_HighLevel_OBBoxOBBox, (Vec3*, Vec3*, Vec3*, Vec3*, Mtx4x4*, Vec3*))
D(int, INT_FullTriangleTriangle, (Vec3*, Vec3*, Vec3*, Vec3*, Vec3*, Vec3*, Vec3*))
D(BOOL, COL_b_SingleBVIn, (tGameObjectMain*, tGameObjectMain*))
D(BOOL, COL_b_BVIn, (tGameObjectMain*, tGameObjectMain*))
D(BOOL, COL_b_SingleBVOverlap, (tGameObjectMain*, tGameObjectMain*))
D(BOOL, COL_b_BVOverlap, (tGameObjectMain*, tGameObjectMain*, BOOL, BOOL, BOOL))
D(void, COL_Zone_Transform, (tColInstanceElem*, tGameObjectMain*, Vec3*, Vec3*))
D(int, COL_GameMaterial_GetFlags, (tGameObjectMain*, tWorld*, int))
D(float, COL_GameMaterial_GetSlide, (tGameObjectMain*, tWorld*, int))
D(float, COL_GameMaterial_GetRebound, (tGameObjectMain*, tWorld*, int))
D(uint8_t, COL_GameMaterial_GetSound, (tGameObjectMain*, tWorld*, int))
D(int16_t, COL_GameMaterial_GetID, (tGameObjectMain*, tWorld*, uint16_t))
D(int, COL_Report_CollideType, (tGameObjectMain*, tWorld*, int))
D(void, COL_Report_GetNormal, (tGameObjectMain*, tWorld*, Vec3*, int))
D(tGameObjectMain*, COL_Report_GetCOBGameObject, (tGameObjectMain*, tWorld*, int))
D(Vec3*, COL_Report_GetCollidedPoint, (tGameObjectMain*, tWorld*, int))
D(Vec3*, COL_Report_GMat_GetCollidedPoint, (tGameObjectMain*, tWorld*, int, int*))
D(int, COL_Report_GetAllReportsNormal, (tGameObjectMain*, tWorld*, Vec3*, int))
D(int, COL_Report_GetNumberOfReports, (tGameObjectMain*, tWorld*, int))
D(void, COL_Report_ComputeResultingNormal, (tGameObjectMain*, tWorld*, int, Vec3*))
D(tGameObjectMain*, COL_Report_GetBestWallGOInDirection, (tGameObjectMain*, tWorld*, Vec3*, float, int*))
D(tGameObjectMain*, COL_Report_GetBestWallGOInDirectionSpecial, (tGameObjectMain*, tWorld*, Vec3*, float, int*))
D(int, COL_Report_GetNumberOfReportsAndRanks, (tGameObjectMain*, tWorld*, int, unsigned int, int*))
D(int, COL_Report_b_ObjectsAreColliding, (tGameObjectMain*, tGameObjectMain*, tWorld*))
D(int, COL_Report_ul_BestReportGet, (tGameObjectMain*, tWorld*, int, int))
D(BOOL, COL_Report_HasGMat, (tGameObjectMain*, tWorld*, int))
D(uint8_t, COL_GetDesign, (tGameObjectMain*, tWorld*, int32_t))
D(int, COL_Report_ul_GMatReportGet, (tGameObjectMain*, tWorld*, int))
D(int, COL_Report_GetZDMGameObjectList, (tGameObjectMain*, tWorld*, tGameObjectMain**, Vec3*, int))
D(void, COL_FindEdgeInFollowDirection, (Vec3*, Vec3**, Vec3**, Vec3**, Vec3*, Vec3*, int))
D(tCobMeshTriangleElem*, COL_GetZConnected, (Vec3*, Vec3*, tCobMeshObjectMesh*, tCobMeshChunkElem*, tCobMeshTriangleElem*))
D(tCobMeshTriangleElem*, COL_GetPointConnectedTriangleWithMuchHorizontalEdge, (Vec3*, Vec3*, Vec3*, tCobMeshObjectMesh*, Mtx4x4*, tCobMeshChunkElem*))
D(void, COL_ComputeJumpPoint, (tGameObjectMain*, Vec3*, float, int, int))
D(void, COL_ZDx_SpecificPosSet, (tGameObjectMain*, uint8_t, Vec3*))
D(void, COL_ZDx_SpecificSizeSet, (tGameObjectMain*, uint8_t, Vec3*))
D(float, COL_ZDx_SizeGet, (tGameObjectMain*, uint8_t))
D(void, COL_ZDx_PosGet, (tGameObjectMain*, uint8_t, Vec3*))
D(void, COL_GeoColMap_PosSet, (tGameObjectMain*, uint8_t, Vec3*))
D(int, COL_ZDE_PivotCollide, (tGameObjectMain*, uint8_t, tGameObjectMain*, int))
D(BOOL, COL_ZDE_BVCollide, (tGameObjectMain*, uint8_t, tGameObjectMain*, int))
D(int, COL_ZDE_BVCenterCollide, (tGameObjectMain*, uint8_t, tGameObjectMain*, int))
D(BOOL, COL_ZDE_ZDECollide, (tGameObjectMain*, uint8_t, uint8_t, tGameObjectMain*))
D(int, COL_VisibleObjectListCreate, (tWorld*, int, int, int, tGameObjectMain**))
D(tGameObjectMain*, COL_GetFirstObjectInRange, (tWorld*, Vec3*, float, int, int, int))
D(int, COL_ListCreateDistance, (int, int, int, tWorld*, tGameObjectMain**, Vec3*, float, char))
D(tGameObjectMain*, COL_RayBV, (tableDescStruct*, Vec3*, Vec3*, float, int, int, int, int))
D(int, COL_TriangleCollision, (Vec3*, Vec3*, Vec3*, Vec3*, tWorld*, int, int, int))
D(int, COL_ListCreate, (tGameObjectMain*, int, int, int, char, tWorld*, tGameObjectMain**, char))

void MATH_InitModule(void);

/* ---- bookkeeping -------------------------------------------------- */

static long gCases;
static long gFails;
static const char* gGroup = "";

static void failed(long caseNo, const char* what)
{
    ++gFails;
    if (gFails <= 20)
        printf("  MISMATCH %-34s case %ld: %s\n", gGroup, caseNo, what);
}

static void eq_i(long c, long a, long b, const char* what)
{
    ++gCases;
    if (a != b)
    {
        char msg[160];
        snprintf(msg, sizeof msg, "%s: base %ld new %ld", what, a, b);
        failed(c, msg);
    }
}

static void eq_f(long c, float a, float b, const char* what)
{
    ++gCases;
    if (memcmp(&a, &b, sizeof a) != 0)
    {
        char msg[160];
        snprintf(msg, sizeof msg, "%s: base %.9g (%08x) new %.9g (%08x)",
                 what, (double)a, *(unsigned*)&a, (double)b, *(unsigned*)&b);
        failed(c, msg);
    }
}

static void eq_v(long c, const Vec3* a, const Vec3* b, const char* what)
{
    eq_f(c, a->x, b->x, what);
    eq_f(c, a->y, b->y, what);
    eq_f(c, a->z, b->z, what);
}

static void eq_mem(long c, const void* a, const void* b, size_t n, const char* what)
{
    ++gCases;
    if (memcmp(a, b, n) != 0)
    {
        size_t i;
        char msg[160];
        for (i = 0; i < n; ++i)
            if (((const unsigned char*)a)[i] != ((const unsigned char*)b)[i])
                break;
        snprintf(msg, sizeof msg, "%s: first differing byte %lu of %lu",
                 what, (unsigned long)i, (unsigned long)n);
        failed(c, msg);
    }
}

/* ---- deterministic numbers ---------------------------------------- */

static unsigned gSeed;
static void seed(unsigned s) { gSeed = s; }
static unsigned nextu(void) { gSeed = gSeed * 1103515245u + 12345u; return gSeed >> 8; }
static int nexti(int lo, int hi) { return lo + (int)(nextu() % (unsigned)(hi - lo + 1)); }

/* A spread that reaches the degenerate values as well as ordinary ones. */
static float nextf(void)
{
    switch (nextu() % 16)
    {
    case 0:  return 0.0f;
    case 1:  return -0.0f;
    case 2:  return 1.0f;
    case 3:  return -1.0f;
    case 4:  return 1e-7f;
    case 5:  return -1e-7f;
    case 6:  return 1e7f;
    case 7:  return 0.5f;
    default: return (float)((int)(nextu() % 20001) - 10000) / 97.0f;
    }
}

static void randv(Vec3* v) { v->x = nextf(); v->y = nextf(); v->z = nextf(); }

static void randmtx(Mtx4x4* m)
{
    m->v1.x = nextf(); m->v1.y = nextf(); m->v1.z = nextf(); m->_m14 = nextf();
    m->v2.x = nextf(); m->v2.y = nextf(); m->v2.z = nextf(); m->_m24 = nextf();
    m->v3.x = nextf(); m->v3.y = nextf(); m->v3.z = nextf(); m->_m34 = nextf();
    m->Pos.x = nextf(); m->Pos.y = nextf(); m->Pos.z = nextf(); m->w = 1.0f;
    m->mtxflag = (nextu() & 1) ? 8u : 0u;
    m->unk1 = 0; m->unk2 = 0; m->unk3 = 0;
}

/* A matrix with the scale flag set but a sane rotation and scale, so
   the arms of COL_ZDE_ZDECollide that only run for a scaled matrix
   reach their intersecting case instead of always missing. */
static void scaledmtx(Mtx4x4* m);

static void identity(Mtx4x4* m)
{
    memset(m, 0, sizeof *m);
    m->v1.x = 1.0f; m->v2.y = 1.0f; m->v3.z = 1.0f;
    m->_m14 = 1.0f; m->_m24 = 1.0f; m->_m34 = 1.0f;
    m->w = 1.0f;
}

static void scaledmtx(Mtx4x4* m)
{
    memset(m, 0, sizeof *m);
    m->v1.x = 1.0f; m->v2.y = 1.0f; m->v3.z = 1.0f;
    m->_m14 = (float)nexti(1, 6) / 2.0f;
    m->_m24 = (float)nexti(1, 6) / 2.0f;
    m->_m34 = (float)nexti(1, 6) / 2.0f;
    m->Pos.x = (float)nexti(-4, 4);
    m->Pos.y = (float)nexti(-4, 4);
    m->Pos.z = (float)nexti(-4, 4);
    m->w = 1.0f;
    m->mtxflag = 8u;
}

static void make_sphere(tGameObjectBV* bv, const Vec3* centre, float radius)
{
    memset(bv, 0, sizeof *bv);
    bv->BVMin = *centre;
    bv->BVMax.x = radius;
    bv->BVMax.y = 0.0f;
    *(uint32_t*)&bv->BVMax.z = 0xffffffffu;      /* the sphere sentinel */
    bv->BVMinLocal = *centre;
    bv->BVMaxLocal.x = radius;
}

static void make_box(tGameObjectBV* bv, const Vec3* lo, const Vec3* hi)
{
    memset(bv, 0, sizeof *bv);
    bv->BVMin = *lo;
    bv->BVMax = *hi;
    if (*(uint32_t*)&bv->BVMax.z == 0xffffffffu)
        bv->BVMax.z = 0.0f;                       /* never a sphere by accident */
    bv->BVMinOriginal = *lo;
    bv->BVMaxOriginal = *hi;
    bv->BVMinLocal = *lo;
    bv->BVMaxLocal = *hi;
}

/* ---- 1. INT_LowLevel_OBBoxOBBox ----------------------------------- */

static void test_lowlevel(long n)
{
    long c;
    gGroup = "INT_LowLevel_OBBoxOBBox";
    seed(0x1111);
    for (c = 0; c < n; ++c)
    {
        Mtx4x4 m;
        float a1, a2, a3, b1, b2, b3;
        BOOL rb, rn;

        if ((c & 7) == 0) identity(&m); else randmtx(&m);
        if ((c & 15) == 0) { m.Pos.x = 0.0f; m.Pos.y = 0.0f; m.Pos.z = 0.0f; }
        a1 = nextf(); a2 = nextf(); a3 = nextf();
        b1 = nextf(); b2 = nextf(); b3 = nextf();
        if ((c & 31) == 0) { a1 = a2 = a3 = 0.0f; }           /* degenerate box */
        if ((c & 63) == 0) { b1 = fabsf(m.Pos.x); }           /* exactly touching */

        rb = BASE_INT_LowLevel_OBBoxOBBox(a1, a2, a3, b1, b2, b3, &m);
        rn = NEW_INT_LowLevel_OBBoxOBBox(a1, a2, a3, b1, b2, b3, &m);
        eq_i(c, rb, rn, "result");
    }
}

/* ---- 2. INT_HighLevel_OBBoxOBBox ---------------------------------- */

static void test_highlevel(long n)
{
    long c;
    gGroup = "INT_HighLevel_OBBoxOBBox";
    seed(0x2222);
    for (c = 0; c < n; ++c)
    {
        Mtx4x4 m;
        Vec3 amin, amax, bmin, bmax, scale;
        BOOL rb, rn;

        if ((c & 7) == 0) identity(&m); else randmtx(&m);
        randv(&amin); randv(&amax); randv(&bmin); randv(&bmax); randv(&scale);
        if ((c & 15) == 0) amax = amin;                        /* zero extent */
        if ((c & 15) == 1) bmax = bmin;
        if ((c & 15) == 2) { scale.x = 1.0f; scale.y = 1.0f; scale.z = 1.0f; }

        rb = BASE_INT_HighLevel_OBBoxOBBox(&amin, &amax, &bmin, &bmax, &m, (c & 1) ? &scale : NULL);
        rn = NEW_INT_HighLevel_OBBoxOBBox(&amin, &amax, &bmin, &bmax, &m, (c & 1) ? &scale : NULL);
        eq_i(c, rb, rn, "result");
    }
}

/* ---- 3. INT_FullTriangleTriangle ---------------------------------- */

static void test_tritri(long n)
{
    long c;
    gGroup = "INT_FullTriangleTriangle";
    seed(0x3333);
    for (c = 0; c < n; ++c)
    {
        Vec3 p[6], hb, hn;
        int rb, rn, i;

        for (i = 0; i < 6; ++i) randv(&p[i]);
        switch (c & 15)
        {
        case 0: p[1] = p[0]; break;                            /* degenerate T1 */
        case 1: p[4] = p[3]; break;                            /* degenerate T2 */
        case 2: p[3] = p[0]; break;                            /* shared corner */
        case 3: p[3] = p[0]; p[4] = p[1]; break;               /* shared edge */
        case 4: p[3].z = p[0].z; p[4].z = p[0].z; p[5].z = p[0].z;
                p[1].z = p[0].z; p[2].z = p[0].z; break;       /* coplanar */
        default: break;
        }
        memset(&hb, 0xA5, sizeof hb);
        memset(&hn, 0xA5, sizeof hn);
        rb = BASE_INT_FullTriangleTriangle(&p[0], &p[1], &p[2], &p[3], &p[4], &p[5], &hb);
        rn = NEW_INT_FullTriangleTriangle(&p[0], &p[1], &p[2], &p[3], &p[4], &p[5], &hn);
        eq_i(c, rb, rn, "result");
        eq_mem(c, &hb, &hn, sizeof hb, "hit point");
    }
}

/* ---- 4. the bounding-volume tests --------------------------------- */

static void test_bv(long n)
{
    long c;
    gGroup = "COL_b_*BV*";
    seed(0x4444);
    for (c = 0; c < n; ++c)
    {
        tGameObjectMain g1, g2;
        tGameObjectBV bv1, bv2;
        Mtx4x4 m1, m2;
        Vec3 a, b;
        BOOL rb, rn;

        memset(&g1, 0, sizeof g1);
        memset(&g2, 0, sizeof g2);
        if ((c & 7) == 0) identity(&m1); else randmtx(&m1);
        if ((c & 7) == 1) identity(&m2); else randmtx(&m2);
        randv(&a); randv(&b);
        if (c & 1) make_sphere(&bv1, &a, nextf()); else make_box(&bv1, &a, &b);
        randv(&a); randv(&b);
        if (c & 2) make_sphere(&bv2, &a, nextf()); else make_box(&bv2, &a, &b);
        /* One case in four puts the two volumes exactly on top of each
           other with both matrices at the origin, so every >= and <=
           in the six-way bound test is answered at equality rather
           than being decided by a wide margin. */
        if ((c & 3) == 0)
        {
            identity(&m1); identity(&m2);
            m1.Pos.x = 0.0f; m1.Pos.y = 0.0f; m1.Pos.z = 0.0f;
            m2.Pos = m1.Pos;
            if ((c & 4) == 0) m2.Pos.x = nextf();
            bv2 = bv1;
        }
        g1.matrices = &m1; g1.boundingVolume = &bv1;
        g2.matrices = &m2; g2.boundingVolume = &bv2;
        g1.flags = (int)nextu(); g2.flags = (int)nextu();
        g1.flags2 = (int)nextu(); g2.flags2 = (int)nextu();

        rb = BASE_COL_b_SingleBVIn(&g1, &g2);
        rn = NEW_COL_b_SingleBVIn(&g1, &g2);
        eq_i(c, rb, rn, "SingleBVIn");

        rb = BASE_COL_b_BVIn(&g1, &g2);
        rn = NEW_COL_b_BVIn(&g1, &g2);
        eq_i(c, rb, rn, "BVIn");

        rb = BASE_COL_b_SingleBVOverlap(&g1, &g2);
        rn = NEW_COL_b_SingleBVOverlap(&g1, &g2);
        eq_i(c, rb, rn, "SingleBVOverlap");

        if ((c & 63) == 0)
        {
            eq_i(c, BASE_COL_b_SingleBVIn(NULL, &g2), NEW_COL_b_SingleBVIn(NULL, &g2), "SingleBVIn null1");
            eq_i(c, BASE_COL_b_BVIn(&g1, NULL), NEW_COL_b_BVIn(&g1, NULL), "BVIn null2");
            eq_i(c, BASE_COL_b_SingleBVOverlap(NULL, NULL), NEW_COL_b_SingleBVOverlap(NULL, NULL), "Overlap null");
        }
    }
}

static void test_bvoverlap(long n)
{
    long c;
    tWorld* w = (tWorld*)calloc(1, sizeof(tWorld));
    tSnp* snp = (tSnp*)calloc(1, sizeof(tSnp));
    tSnpAxis* ax = (tSnpAxis*)calloc(3, sizeof(tSnpAxis));

    snp->axisPtr[0] = &ax[0];
    snp->axisPtr[1] = &ax[1];
    snp->axisPtr[2] = &ax[2];
    w->snp = snp;
    WOR_gpst_CurrentWorld = w;

    gGroup = "COL_b_BVOverlap";
    seed(0x5555);
    for (c = 0; c < n; ++c)
    {
        tGameObjectMain g1, g2;
        tGameObjectBV bv1, bv2;
        Mtx4x4 m1, m2;
        Vec3 a, b;
        int ob1, ob2, snpOn;
        BOOL rb, rn;

        memset(&g1, 0, sizeof g1);
        memset(&g2, 0, sizeof g2);
        if ((c & 7) == 0) identity(&m1); else randmtx(&m1);
        if ((c & 7) == 1) identity(&m2); else randmtx(&m2);
        randv(&a); randv(&b);
        if (c & 1) make_sphere(&bv1, &a, nextf()); else make_box(&bv1, &a, &b);
        randv(&a); randv(&b);
        if (c & 2) make_sphere(&bv2, &a, nextf()); else make_box(&bv2, &a, &b);
        g1.matrices = &m1; g1.boundingVolume = &bv1;
        g2.matrices = &m2; g2.boundingVolume = &bv2;
        g1.flags = (int)(nextu() & 0x80000u) | (int)(nextu() & 0xFFu);
        g2.flags = (int)(nextu() & 0x80000u) | (int)(nextu() & 0xFFu);
        g1.flags2 = (int)(nextu() & 0xA0u);
        g2.flags2 = (int)(nextu() & 0xA0u);
        g1.snpIndex = (uint16_t)nexti(0, 7);
        g2.snpIndex = (uint16_t)nexti(0, 7);
        ob1 = nexti(0, 1); ob2 = nexti(0, 1); snpOn = nexti(0, 1);

        rb = BASE_COL_b_BVOverlap(&g1, &g2, ob1, ob2, snpOn);
        rn = NEW_COL_b_BVOverlap(&g1, &g2, ob1, ob2, snpOn);
        eq_i(c, rb, rn, "result");

        if ((c & 127) == 0)
        {
            eq_i(c, BASE_COL_b_BVOverlap(NULL, &g2, ob1, ob2, snpOn),
                    NEW_COL_b_BVOverlap(NULL, &g2, ob1, ob2, snpOn), "null1");
            eq_i(c, BASE_COL_b_BVOverlap(&g1, NULL, ob1, ob2, snpOn),
                    NEW_COL_b_BVOverlap(&g1, NULL, ob1, ob2, snpOn), "null2");
        }
    }
    WOR_gpst_CurrentWorld = NULL;
    free(w); free(snp); free(ax);
}

/* ---- 5. the zone transform and the ZDx accessors ------------------ */

typedef struct
{
    tGameObjectMain gao;
    tGameObjectExtended ext;
    tGameObjectCollMain coll;
    tColInstance inst;
    tColInstSet set;
    uint8_t zoneIDs[16];
    tColInstanceElem elems[4];
    tColInstanceElem* slots[4];
    tColBox boxes[4];
    tColSphere spheres[4];
    tGameObjectBV bv;
    Mtx4x4 mtx;
} ZoneGao;

/* mode 0: identity near the origin, 1: an arbitrary matrix,
   2: a sane scaled matrix with the scale flag set. */
static void build_zone_gao(ZoneGao* z, int mode)
{
    int i;
    Vec3 lo, hi, c;

    memset(z, 0, sizeof *z);
    if (mode == 1) randmtx(&z->mtx);
    else if (mode == 2) scaledmtx(&z->mtx);
    else identity(&z->mtx);
    if (mode == 0)
    {
        z->mtx.Pos.x = (float)nexti(-4, 4);
        z->mtx.Pos.y = (float)nexti(-4, 4);
        z->mtx.Pos.z = (float)nexti(-4, 4);
    }
    randv(&c);
    lo.x = (float)nexti(-8, -1); lo.y = (float)nexti(-8, -1); lo.z = (float)nexti(-8, -1);
    hi.x = (float)nexti(1, 8);   hi.y = (float)nexti(1, 8);   hi.z = (float)nexti(1, 8);
    make_box(&z->bv, &lo, &hi);
    for (i = 0; i < 4; ++i)
    {
        /* colFlags keeps bit 2 -- the "zone is live" bit every ZDE
           entry point tests -- set most of the time, and elemType
           stays in the box/sphere pair, so COL_ZDE_ZDECollide's
           switch is reached instead of being bailed out of first. */
        z->elems[i].colFlags = (uint8_t)((nextu() & 0x0Bu) | ((nextu() % 8) ? 4u : 0u));
        z->elems[i].elemType = (uint8_t)((nextu() % 8) ? nexti(1, 2) : nexti(0, 3));
        z->elems[i].unk1 = (uint8_t)nextu();
        z->elems[i].design = (uint8_t)nextu();
        /* Shapes near the origin at a comparable size, so the collide
           tests come out both ways instead of always apart. */
        z->boxes[i].pos1.x = (float)nexti(-6, 6);
        z->boxes[i].pos1.y = (float)nexti(-6, 6);
        z->boxes[i].pos1.z = (float)nexti(-6, 6);
        z->boxes[i].pos2.x = z->boxes[i].pos1.x - (float)nexti(1, 8);
        z->boxes[i].pos2.y = z->boxes[i].pos1.y - (float)nexti(1, 8);
        z->boxes[i].pos2.z = z->boxes[i].pos1.z - (float)nexti(1, 8);
        z->spheres[i].center.x = (float)nexti(-6, 6);
        z->spheres[i].center.y = (float)nexti(-6, 6);
        z->spheres[i].center.z = (float)nexti(-6, 6);
        z->spheres[i].radius = (float)nexti(0, 8);
        z->elems[i].elemShapePtr = (z->elems[i].elemType == 2)
            ? (void*)&z->spheres[i] : (void*)&z->boxes[i];
        z->slots[i] = &z->elems[i];
    }
    for (i = 0; i < 16; ++i)
        z->zoneIDs[i] = (uint8_t)((nextu() % 8) ? (unsigned)nexti(0, 3) : 0xFFu);
    z->set.zoneIDs = z->zoneIDs;
    z->set.instElemNum = 4;
    z->inst.colSet = &z->set;
    z->inst.zoneNum = 4;
    z->inst.selectedInstElemPtrs = z->slots;
    z->inst.colInstElems = z->elems;
    z->inst.activationFlags = (uint16_t)((nextu() % 8) ? 0xFFFFu : (unsigned)nextu());
    z->inst.instElemBitmask = (uint16_t)(nextu() & 0x0Fu);
    z->coll.colInstance = &z->inst;
    z->ext.coll = &z->coll;
    z->gao.extended = &z->ext;
    z->gao.matrices = &z->mtx;
    z->gao.boundingVolume = &z->bv;
    z->gao.flags = 0x600;
    z->gao.flags2 = 1;
}

static void test_zone(long n)
{
    long c;
    gGroup = "COL_Zone_Transform / COL_ZDx_*";
    seed(0x6666);
    for (c = 0; c < n; ++c)
    {
        ZoneGao zb, zn;
        Vec3 minb, maxb, minn, maxn, arg;
        int which = nexti(0, 3);
        uint8_t id = (uint8_t)nexti(0, 15);

        seed((unsigned)(0x6666 + c));
        build_zone_gao(&zb, (int)(c % 3));
        seed((unsigned)(0x6666 + c));
        build_zone_gao(&zn, (int)(c % 3));

        memset(&minb, 0xA5, sizeof minb); memset(&maxb, 0xA5, sizeof maxb);
        memset(&minn, 0xA5, sizeof minn); memset(&maxn, 0xA5, sizeof maxn);
        BASE_COL_Zone_Transform(&zb.elems[which], &zb.gao, &minb, &maxb);
        NEW_COL_Zone_Transform(&zn.elems[which], &zn.gao, &minn, &maxn);
        eq_mem(c, &minb, &minn, sizeof minb, "Zone_Transform min");
        eq_mem(c, &maxb, &maxn, sizeof maxb, "Zone_Transform max");

        /* The ZDx accessors only reach a slot whose zone id is not 0xFF. */
        if (zb.zoneIDs[id] != 0xFF)
        {
            Vec3 pb, pn;
            float fb, fn;

            randv(&arg);
            BASE_COL_ZDx_SpecificPosSet(&zb.gao, id, &arg);
            NEW_COL_ZDx_SpecificPosSet(&zn.gao, id, &arg);
            eq_mem(c, zb.boxes, zn.boxes, sizeof zb.boxes, "SpecificPosSet boxes");
            eq_mem(c, zb.spheres, zn.spheres, sizeof zb.spheres, "SpecificPosSet spheres");

            randv(&arg);
            BASE_COL_ZDx_SpecificSizeSet(&zb.gao, id, &arg);
            NEW_COL_ZDx_SpecificSizeSet(&zn.gao, id, &arg);
            eq_mem(c, zb.boxes, zn.boxes, sizeof zb.boxes, "SpecificSizeSet boxes");
            eq_mem(c, zb.spheres, zn.spheres, sizeof zb.spheres, "SpecificSizeSet spheres");

            fb = BASE_COL_ZDx_SizeGet(&zb.gao, id);
            fn = NEW_COL_ZDx_SizeGet(&zn.gao, id);
            eq_f(c, fb, fn, "SizeGet");

            memset(&pb, 0xA5, sizeof pb);
            memset(&pn, 0xA5, sizeof pn);
            BASE_COL_ZDx_PosGet(&zb.gao, id, &pb);
            NEW_COL_ZDx_PosGet(&zn.gao, id, &pn);
            eq_mem(c, &pb, &pn, sizeof pb, "PosGet");
        }
    }
}

static void test_zde(long n)
{
    long c;
    gGroup = "COL_ZDE_*Collide";
    seed(0x7777);
    for (c = 0; c < n; ++c)
    {
        ZoneGao za, zb2;
        uint8_t id1 = (uint8_t)nexti(0, 15);
        uint8_t id2 = (uint8_t)nexti(0, 15);
        int dir = nexti(0, 1);
        unsigned base = (unsigned)(0x7777 + c * 2);

        seed(base);      build_zone_gao(&za, (int)(c % 3));
        seed(base + 1);  build_zone_gao(&zb2, (int)((c / 3) % 3));

        eq_i(c, BASE_COL_ZDE_PivotCollide(&za.gao, id1, &zb2.gao, dir),
                NEW_COL_ZDE_PivotCollide(&za.gao, id1, &zb2.gao, dir), "PivotCollide");
        eq_i(c, BASE_COL_ZDE_BVCollide(&za.gao, id1, &zb2.gao, dir),
                NEW_COL_ZDE_BVCollide(&za.gao, id1, &zb2.gao, dir), "BVCollide");
        eq_i(c, BASE_COL_ZDE_BVCenterCollide(&za.gao, id1, &zb2.gao, dir),
                NEW_COL_ZDE_BVCenterCollide(&za.gao, id1, &zb2.gao, dir), "BVCenterCollide");
        eq_i(c, BASE_COL_ZDE_ZDECollide(&za.gao, id1, id2, &zb2.gao),
                NEW_COL_ZDE_ZDECollide(&za.gao, id1, id2, &zb2.gao), "ZDECollide");
    }
}

/* ---- 6. the report accessors over a hand-built record list -------- */

#define NGAO 6
#define NMAT 4

static tGameObjectMain gGao[NGAO];
static tGameObjectBV gGaoBV[NGAO];
static Mtx4x4 gGaoMtx[NGAO];
static tCobGameMaterialElems gMat[NMAT];

static void build_gaos(void)
{
    int i;
    seed(0x8101);
    for (i = 0; i < NGAO; ++i)
    {
        Vec3 lo, hi;
        memset(&gGao[i], 0, sizeof gGao[i]);
        identity(&gGaoMtx[i]);
        gGaoMtx[i].Pos.x = (float)(i * 6);
        gGaoMtx[i].Pos.y = (float)(i & 1);
        /* Boxes the ray and range tests can hit: a spread along x with
           a size the fixtures' rays actually cross. */
        lo.x = -2.0f; lo.y = -2.0f; lo.z = -2.0f;
        hi.x = 2.0f;  hi.y = 2.0f;  hi.z = 2.0f;
        if (i == 2) { hi.x = 0.25f; hi.y = 0.25f; hi.z = 0.25f; }
        make_box(&gGaoBV[i], &lo, &hi);
        if (i == 4) make_sphere(&gGaoBV[i], &lo, 3.0f);
        gGao[i].matrices = &gGaoMtx[i];
        gGao[i].boundingVolume = &gGaoBV[i];
        gGao[i].flags = 0x100 | (int)(nextu() & 0xFFu);
        gGao[i].flags2 = 1 | (int)(nextu() & 0xF0u);
        gGao[i].distance = (uint8_t)nexti(0, 2);
    }
    for (i = 0; i < NMAT; ++i)
    {
        gMat[i].flags = (int)nextu();
        gMat[i].slide = nextf();
        gMat[i].rebound = nextf();
        gMat[i].sound = (uint8_t)nextu();
        gMat[i].b2 = (uint8_t)nextu();
        gMat[i].id = (int16_t)nextu();
    }
}

static void build_world(tWorld* w, unsigned s, int nrec)
{
    int i;
    memset(w, 0, sizeof *w);
    seed(s);
    w->recordCount = nrec;
    for (i = 0; i < nrec; ++i)
    {
        tRecordElem* r = &w->records[i];
        r->collisionInstanceGameObject = &gGao[nexti(0, NGAO - 1)];
        r->collisionMapGameObject = (nextu() & 7) ? &gGao[nexti(0, NGAO - 1)] : NULL;
        randv(&r->collidedPoint);
        if ((nextu() & 7) == 0) { r->collidedPoint.x = 0; r->collidedPoint.y = 0; r->collidedPoint.z = 0; }
        randv(&r->recalVec);
        randv(&r->normalVec);
        r->gameMaterial = (nextu() & 3) ? &gMat[nexti(0, NMAT - 1)] : NULL;
        /* flag words that actually hit the masks the accessors test */
        r->flags = (int)(nextu() & 0xFFFFu) | (int)((nextu() & 7u) << 16)
                 | (int)((nextu() & 1u) << 30);
        r->triangleChunkElemID = nexti(0, 3);
        r->triangleChunkID = (int16_t)nexti(0, 3);
        r->designVal = (uint8_t)nextu();
    }
    w->rayCastCobGmat = (s & 1) ? &gMat[0] : NULL;
    w->rayCastCobMeshChunkElemsDesignVal = (uint8_t)(s & 0xFF);
}

static long offset_in(const tWorld* w, const void* p)
{
    if (!p) return -1;
    if ((const char*)p < (const char*)w || (const char*)p >= (const char*)(w + 1))
        return -2;                                  /* outside: e.g. MATH_gst_NulVector */
    return (const char*)p - (const char*)w;
}

static void test_reports(long n)
{
    long c;
    tWorld* wb = (tWorld*)malloc(sizeof(tWorld));
    tWorld* wn = (tWorld*)malloc(sizeof(tWorld));

    gGroup = "report accessors";
    for (c = 0; c < n; ++c)
    {
        tGameObjectMain* gao;
        int filter, nrec;
        int rankb = -99, rankn = -99;
        Vec3 vb, vn;
        int listb[64], listn[64];
        tGameObjectMain* gb[64];
        tGameObjectMain* gn[64];
        Vec3 nb[64], nn[64];
        Vec3 dir;

        nrec = (int)(c % 24);
        build_world(wb, (unsigned)(0x9000 + c), nrec);
        build_world(wn, (unsigned)(0x9000 + c), nrec);
        seed((unsigned)(0xA000 + c));
        gao = &gGao[nexti(0, NGAO - 1)];
        switch (c % 6)
        {
        case 0:  filter = (int)(nextu() & 0xFFFFu); break;
        case 1:  filter = 0xFFFF; break;
        case 2:  filter = (int)(0x80000000u | (unsigned)nexti(0, nrec ? nrec - 1 : 0)); break;
        case 3:  filter = 0x2000 | (int)(nextu() & 0xFFu); break;
        case 4:  filter = 0x40000000 | (int)(nextu() & 0xFFFu); break;
        default: filter = (int)nextu(); break;
        }
        if ((c % 6) == 2 && nrec == 0) filter = 0;       /* no record to index */
        randv(&dir);
        if ((c & 7) == 0) { dir.x = 0; dir.y = 0; dir.z = 0; }

        eq_i(c, BASE_COL_Report_CollideType(gao, wb, filter),
                NEW_COL_Report_CollideType(gao, wn, filter), "CollideType");

        memset(&vb, 0xA5, sizeof vb); memset(&vn, 0xA5, sizeof vn);
        BASE_COL_Report_GetNormal(gao, wb, &vb, filter);
        NEW_COL_Report_GetNormal(gao, wn, &vn, filter);
        eq_mem(c, &vb, &vn, sizeof vb, "GetNormal");

        eq_i(c, (long)(BASE_COL_Report_GetCOBGameObject(gao, wb, filter) - gGao),
                (long)(NEW_COL_Report_GetCOBGameObject(gao, wn, filter) - gGao), "GetCOBGameObject");

        eq_i(c, BASE_COL_GameMaterial_GetFlags(gao, wb, filter),
                NEW_COL_GameMaterial_GetFlags(gao, wn, filter), "GMat flags");
        eq_f(c, BASE_COL_GameMaterial_GetSlide(gao, wb, filter),
                NEW_COL_GameMaterial_GetSlide(gao, wn, filter), "GMat slide");
        eq_i(c, BASE_COL_GameMaterial_GetSound(gao, wb, filter),
                NEW_COL_GameMaterial_GetSound(gao, wn, filter), "GMat sound");
        eq_i(c, BASE_COL_GameMaterial_GetID(gao, wb, (uint16_t)filter),
                NEW_COL_GameMaterial_GetID(gao, wn, (uint16_t)filter), "GMat id");
        /* GetRebound's tagged-rank arm indexes in front of the record
           array, so only the non-negative filters are comparable. */
        if (filter >= 0)
            eq_f(c, BASE_COL_GameMaterial_GetRebound(gao, wb, filter),
                    NEW_COL_GameMaterial_GetRebound(gao, wn, filter), "GMat rebound");

        eq_i(c, offset_in(wb, BASE_COL_Report_GetCollidedPoint(gao, wb, filter)),
                offset_in(wn, NEW_COL_Report_GetCollidedPoint(gao, wn, filter)), "GetCollidedPoint");

        eq_i(c, offset_in(wb, BASE_COL_Report_GMat_GetCollidedPoint(gao, wb, filter, &rankb)),
                offset_in(wn, NEW_COL_Report_GMat_GetCollidedPoint(gao, wn, filter, &rankn)), "GMat point");
        eq_i(c, rankb, rankn, "GMat point rank");

        memset(nb, 0xA5, sizeof nb); memset(nn, 0xA5, sizeof nn);
        eq_i(c, BASE_COL_Report_GetAllReportsNormal(gao, wb, nb, filter),
                NEW_COL_Report_GetAllReportsNormal(gao, wn, nn, filter), "AllReportsNormal");
        eq_mem(c, nb, nn, sizeof nb, "AllReportsNormal out");

        eq_i(c, BASE_COL_Report_GetNumberOfReports(gao, wb, filter),
                NEW_COL_Report_GetNumberOfReports(gao, wn, filter), "NumberOfReports");

        memset(&vb, 0xA5, sizeof vb); memset(&vn, 0xA5, sizeof vn);
        BASE_COL_Report_ComputeResultingNormal(gao, wb, filter, &vb);
        NEW_COL_Report_ComputeResultingNormal(gao, wn, filter, &vn);
        eq_mem(c, &vb, &vn, sizeof vb, "ResultingNormal");

        rankb = -99; rankn = -99;
        {
            float cosv = nextf();
            eq_i(c, (long)(BASE_COL_Report_GetBestWallGOInDirection(gao, wb, &dir, cosv, &rankb) - gGao),
                    (long)(NEW_COL_Report_GetBestWallGOInDirection(gao, wn, &dir, cosv, &rankn) - gGao), "BestWall");
            eq_i(c, rankb, rankn, "BestWall rank");
            rankb = -99; rankn = -99;
            eq_i(c, (long)(BASE_COL_Report_GetBestWallGOInDirectionSpecial(gao, wb, &dir, cosv, &rankb) - gGao),
                    (long)(NEW_COL_Report_GetBestWallGOInDirectionSpecial(gao, wn, &dir, cosv, &rankn) - gGao), "BestWallSpecial");
            eq_i(c, rankb, rankn, "BestWallSpecial rank");
        }

        memset(listb, 0x5A, sizeof listb); memset(listn, 0x5A, sizeof listn);
        eq_i(c, BASE_COL_Report_GetNumberOfReportsAndRanks(gao, wb, filter, 8u, listb),
                NEW_COL_Report_GetNumberOfReportsAndRanks(gao, wn, filter, 8u, listn), "ReportsAndRanks");
        eq_mem(c, listb, listn, sizeof listb, "ReportsAndRanks out");

        eq_i(c, BASE_COL_Report_b_ObjectsAreColliding(gao, &gGao[(c + 1) % NGAO], wb),
                NEW_COL_Report_b_ObjectsAreColliding(gao, &gGao[(c + 1) % NGAO], wn), "ObjectsAreColliding");

        {
            int rej = (int)nextu();
            eq_i(c, BASE_COL_Report_ul_BestReportGet(gao, wb, filter, rej),
                    NEW_COL_Report_ul_BestReportGet(gao, wn, filter, rej), "BestReportGet");
        }

        eq_i(c, BASE_COL_Report_HasGMat(gao, wb, filter),
                NEW_COL_Report_HasGMat(gao, wn, filter), "HasGMat");
        eq_i(c, BASE_COL_GetDesign(gao, wb, filter),
                NEW_COL_GetDesign(gao, wn, filter), "GetDesign");
        eq_i(c, BASE_COL_Report_ul_GMatReportGet(gao, wb, filter),
                NEW_COL_Report_ul_GMatReportGet(gao, wn, filter), "GMatReportGet");

        memset(gb, 0, sizeof gb); memset(gn, 0, sizeof gn);
        eq_i(c, BASE_COL_Report_GetZDMGameObjectList(gao, wb, gb, &dir, filter),
                NEW_COL_Report_GetZDMGameObjectList(gao, wn, gn, &dir, filter), "ZDMGameObjectList");
        eq_mem(c, gb, gn, sizeof gb, "ZDMGameObjectList out");

        eq_mem(c, wb, wn, sizeof *wb, "world untouched");
    }
    free(wb);
    free(wn);
}

/* ---- 7. the edge-search chain and COL_ComputeJumpPoint ------------ */

#define MESH_W 5
#define NVERT (MESH_W * MESH_W)
#define NTRI ((MESH_W - 1) * (MESH_W - 1) * 2)

typedef struct
{
    tCobMeshObjectMesh mesh;
    Vec3 verts[NVERT];
    Vec3 normals[NTRI];
    tCobMeshChunkElem chunk;
    tCobMeshTriangleElem tris[NTRI];
    tColCollisionObject cob;
    tColCollisionObject* cobPtr;
    tColMap colMap;
    tGameObjectCollMain coll;
    tGameObjectExtended ext;
    tGameObjectMain gao;
    tGameObjectBV bv;
    Mtx4x4 mtx;
} MeshGao;

static void build_mesh_gao(MeshGao* m, unsigned s)
{
    int x, y, t = 0, i;
    Vec3 lo, hi;

    memset(m, 0, sizeof *m);
    seed(s);
    for (y = 0; y < MESH_W; ++y)
        for (x = 0; x < MESH_W; ++x)
        {
            m->verts[y * MESH_W + x].x = (float)x * 10.0f + nextf() * 0.01f;
            m->verts[y * MESH_W + x].y = (float)y * 10.0f + nextf() * 0.01f;
            m->verts[y * MESH_W + x].z = nextf() * 0.5f;
        }
    for (y = 0; y < MESH_W - 1; ++y)
        for (x = 0; x < MESH_W - 1; ++x)
        {
            m->tris[t].v1 = (uint16_t)(y * MESH_W + x);
            m->tris[t].v2 = (uint16_t)(y * MESH_W + x + 1);
            m->tris[t].v3 = (uint16_t)((y + 1) * MESH_W + x);
            ++t;
            m->tris[t].v1 = (uint16_t)(y * MESH_W + x + 1);
            m->tris[t].v2 = (uint16_t)((y + 1) * MESH_W + x + 1);
            m->tris[t].v3 = (uint16_t)((y + 1) * MESH_W + x);
            ++t;
        }
    for (i = 0; i < NTRI; ++i) randv(&m->normals[i]);
    m->mesh.vertexCount = NVERT;
    m->mesh.faceCount = NTRI;
    m->mesh.meshChunkElemNum = 1;
    m->mesh.vertices = m->verts;
    m->mesh.normals = m->normals;
    m->mesh.meshChunkElems = &m->chunk;
    m->mesh.ok3 = NULL;
    m->chunk.elemNum = NTRI;
    m->chunk.triangleElems = m->tris;
    m->chunk.designVal = 3;
    m->cob.objectType = 5;
    m->cob.objectPtrByType = &m->mesh;
    m->cobPtr = &m->cob;
    m->colMap.colObjectID = 1;
    m->colMap.activation = 1;
    m->colMap.collisionObjectPtr = &m->cobPtr;
    m->coll.colMap = &m->colMap;
    m->ext.coll = &m->coll;
    identity(&m->mtx);
    m->mtx.Pos.x = 1.5f;
    m->mtx.Pos.y = -2.5f;
    /* A volume that actually covers the mesh, so COL_TriangleCollision
       gets past its bounding-box reject. */
    lo.x = -5.0f; lo.y = -5.0f; lo.z = -5.0f;
    hi.x = 45.0f; hi.y = 45.0f; hi.z = 5.0f;
    make_box(&m->bv, &lo, &hi);
    m->gao.extended = &m->ext;
    m->gao.matrices = &m->mtx;
    m->gao.boundingVolume = &m->bv;
    m->gao.flags = 0x100;
    m->gao.flags2 = 1;
}

static void test_edges(long n)
{
    long c;
    MeshGao* mb = (MeshGao*)malloc(sizeof(MeshGao));
    MeshGao* mn = (MeshGao*)malloc(sizeof(MeshGao));

    gGroup = "edge search";
    for (c = 0; c < n; ++c)
    {
        Vec3 hit, right, normal;
        Vec3 *p1b, *p2b, *p3b, *p1n, *p2n, *p3n;
        Vec3 ab, bb, an, bn;
        tCobMeshTriangleElem *rb, *rn;
        int down = (int)(c & 1);
        int ti;

        build_mesh_gao(mb, (unsigned)(0xB000 + c));
        build_mesh_gao(mn, (unsigned)(0xB000 + c));
        seed((unsigned)(0xC000 + c));
        randv(&hit); randv(&right); randv(&normal);
        ti = nexti(0, NTRI - 1);

        /* COL_FindEdgeInFollowDirection is pure: same three vertices,
           two cursor triples, compare which vertices come back. */
        p1b = &mb->verts[mb->tris[ti].v1];
        p2b = &mb->verts[mb->tris[ti].v2];
        p3b = &mb->verts[mb->tris[ti].v3];
        p1n = &mn->verts[mn->tris[ti].v1];
        p2n = &mn->verts[mn->tris[ti].v2];
        p3n = &mn->verts[mn->tris[ti].v3];
        BASE_COL_FindEdgeInFollowDirection(&hit, &p1b, &p2b, &p3b, &right, &normal, down);
        NEW_COL_FindEdgeInFollowDirection(&hit, &p1n, &p2n, &p3n, &right, &normal, down);
        eq_i(c, p1b - mb->verts, p1n - mn->verts, "FindEdge point1");
        eq_i(c, p2b - mb->verts, p2n - mn->verts, "FindEdge point2");
        eq_i(c, p3b - mb->verts, p3n - mn->verts, "FindEdge point3");

        /* COL_GetZConnected over every edge of the chosen triangle. */
        {
            int e;
            for (e = 0; e < 3; ++e)
            {
                int i1 = (e == 0) ? mb->tris[ti].v1 : (e == 1) ? mb->tris[ti].v2 : mb->tris[ti].v1;
                int i2 = (e == 0) ? mb->tris[ti].v2 : (e == 1) ? mb->tris[ti].v3 : mb->tris[ti].v3;
                rb = BASE_COL_GetZConnected(&mb->verts[i1], &mb->verts[i2], &mb->mesh, &mb->chunk, &mb->tris[ti]);
                rn = NEW_COL_GetZConnected(&mn->verts[i1], &mn->verts[i2], &mn->mesh, &mn->chunk, &mn->tris[ti]);
                eq_i(c, rb ? rb - mb->tris : -1, rn ? rn - mn->tris : -1, "GetZConnected");
            }
        }

        /* COL_GetPointConnectedTriangleWithMuchHorizontalEdge. */
        memset(&ab, 0xA5, sizeof ab); memset(&bb, 0xA5, sizeof bb);
        memset(&an, 0xA5, sizeof an); memset(&bn, 0xA5, sizeof bn);
        {
            int vi = nexti(0, NVERT - 1);
            rb = BASE_COL_GetPointConnectedTriangleWithMuchHorizontalEdge(
                     &mb->verts[vi], &ab, &bb, &mb->mesh, &mb->mtx, &mb->chunk);
            rn = NEW_COL_GetPointConnectedTriangleWithMuchHorizontalEdge(
                     &mn->verts[vi], &an, &bn, &mn->mesh, &mn->mtx, &mn->chunk);
            eq_i(c, rb ? rb - mb->tris : -1, rn ? rn - mn->tris : -1, "PointConnected tri");
            eq_mem(c, &ab, &an, sizeof ab, "PointConnected shared");
            eq_mem(c, &bb, &bn, sizeof bb, "PointConnected other");
        }
        eq_mem(c, mb->verts, mn->verts, sizeof mb->verts, "mesh untouched");
    }
    free(mb);
    free(mn);
}

static void test_jumppoint(long n)
{
    long c;
    MeshGao* mb = (MeshGao*)malloc(sizeof(MeshGao));
    MeshGao* mn = (MeshGao*)malloc(sizeof(MeshGao));
    tWorld* wb = (tWorld*)malloc(sizeof(tWorld));
    tWorld* wn = (tWorld*)malloc(sizeof(tWorld));

    gGroup = "COL_ComputeJumpPoint";
    for (c = 0; c < n; ++c)
    {
        Vec3 pb, pn;
        float margin;
        int down = (int)(c & 1);
        int ti;

        build_mesh_gao(mb, (unsigned)(0xD000 + c));
        build_mesh_gao(mn, (unsigned)(0xD000 + c));
        memset(wb, 0, sizeof *wb);
        memset(wn, 0, sizeof *wn);
        seed((unsigned)(0xE000 + c));
        ti = nexti(0, NTRI - 1);
        margin = (float)nexti(1, 40) / 4.0f;

        /* Drive the useLastRayHit arm: it is the one the game uses. */
        wb->rayCastGao = &mb->gao;
        wb->rayCastTriangleChunkID = 0;
        wb->rayCastTriangleChunkElemID = ti;
        randv(&wb->colRayHitPos);
        wn->rayCastGao = &mn->gao;
        wn->rayCastTriangleChunkID = 0;
        wn->rayCastTriangleChunkElemID = ti;
        wn->colRayHitPos = wb->colRayHitPos;

        memset(&pb, 0xA5, sizeof pb);
        memset(&pn, 0xA5, sizeof pn);
        WOR_gpst_CurrentWorld = wb;
        BASE_COL_ComputeJumpPoint(&mb->gao, &pb, margin, down, 1);
        WOR_gpst_CurrentWorld = wn;
        NEW_COL_ComputeJumpPoint(&mn->gao, &pn, margin, down, 1);
        WOR_gpst_CurrentWorld = NULL;

        eq_mem(c, &pb, &pn, sizeof pb, "jump point");
        eq_mem(c, &wb->rayCastNormalVec, &wn->rayCastNormalVec, sizeof wb->rayCastNormalVec, "normal");
        eq_mem(c, mb->verts, mn->verts, sizeof mb->verts, "mesh untouched");

        /* The other arm scans the collision records instead. It leaves
           the world pointer it writes the normal through null, so only
           the cases that return before that are safe to drive: a scan
           that matches nothing, which is also the arm that reads one
           record past the end, and a match whose map object carries no
           collision map. */
        {
            int k;
            int matching = (int)(c & 1);

            wb->recordCount = 4;
            wn->recordCount = 4;
            for (k = 0; k < 4; ++k)
            {
                memset(&wb->records[k], 0, sizeof wb->records[k]);
                wb->records[k].collisionInstanceGameObject =
                    (k == 2 && matching) ? &mb->gao : &gGao[k % NGAO];
                wb->records[k].collisionMapGameObject = &gGao[k % NGAO];
                wb->records[k].flags = matching
                    ? (int)(0x80u | (unsigned)nexti(1, 15))
                    : (int)(nextu() & 0x7Fu);
                wb->records[k].triangleChunkID = 0;
                wb->records[k].triangleChunkElemID = ti;
                randv(&wb->records[k].collidedPoint);
                wn->records[k] = wb->records[k];
                if (wn->records[k].collisionInstanceGameObject == &mb->gao)
                    wn->records[k].collisionInstanceGameObject = &mn->gao;
            }
            memset(&wb->records[4], 0, sizeof wb->records[4]);
            memset(&wn->records[4], 0, sizeof wn->records[4]);

            memset(&pb, 0xA5, sizeof pb);
            memset(&pn, 0xA5, sizeof pn);
            WOR_gpst_CurrentWorld = wb;
            BASE_COL_ComputeJumpPoint(&mb->gao, &pb, margin, down, 0);
            WOR_gpst_CurrentWorld = wn;
            NEW_COL_ComputeJumpPoint(&mn->gao, &pn, margin, down, 0);
            WOR_gpst_CurrentWorld = NULL;
            eq_mem(c, &pb, &pn, sizeof pb, "jump point from a report");
        }
    }
    free(mb); free(mn); free(wb); free(wn);
}

/* ---- 8. the list builders and the triangle sweep ------------------ */

static void test_lists(long n)
{
    long c;
    tWorld* wb = (tWorld*)malloc(sizeof(tWorld));
    tWorld* wn = (tWorld*)malloc(sizeof(tWorld));
    static tableElemStructPF elems[NGAO + 2];

    gGroup = "list builders";
    for (c = 0; c < n; ++c)
    {
        tGameObjectMain* lb[64];
        tGameObjectMain* ln[64];
        Vec3 pos, dir;
        float range;
        int mustHave, mustNot, what, i, cnt;

        memset(wb, 0, sizeof *wb);
        memset(wn, 0, sizeof *wn);
        seed((unsigned)(0xF000 + c));
        cnt = nexti(1, NGAO);
        for (i = 0; i < cnt; ++i)
        {
            elems[i].objectPtr = (nextu() & 7) ? (void*)&gGao[i % NGAO] : NULL;
            elems[i].objectFlags = (int)nextu();
        }
        wb->activatedObjectsTable4PF.firstElemPtr = elems;
        wb->activatedObjectsTable4PF.nextElemPtr = &elems[cnt];
        wb->visibleGameObjectsPF = wb->activatedObjectsTable4PF;
        wn->activatedObjectsTable4PF = wb->activatedObjectsTable4PF;
        wn->visibleGameObjectsPF = wb->visibleGameObjectsPF;

        /* Most cases ask for nothing in particular, so the body of
           each list builder is actually entered; the rest keep the
           flag filtering under test. */
        if (c & 1) { mustHave = 0; mustNot = 0; }
        else { mustHave = (int)(nextu() & 0x1FFu); mustNot = (int)(nextu() & 0x1FFu); }
        what = nexti(0, 1);
        pos.x = (float)nexti(-10, 40);
        pos.y = (float)nexti(-4, 4);
        pos.z = (float)nexti(-4, 4);
        dir.x = 1.0f; dir.y = 0.0f; dir.z = 0.0f;
        if ((c & 6) == 2) { dir.x = 0.0f; dir.y = 1.0f; }
        if ((c & 6) == 4) { randv(&dir); }
        range = (float)nexti(1, 60);

        memset(lb, 0, sizeof lb); memset(ln, 0, sizeof ln);
        eq_i(c, BASE_COL_VisibleObjectListCreate(wb, mustHave, mustNot, what, lb),
                NEW_COL_VisibleObjectListCreate(wn, mustHave, mustNot, what, ln), "VisibleObjectListCreate");
        eq_mem(c, lb, ln, sizeof lb, "VisibleObjectListCreate out");

        eq_i(c, (long)(BASE_COL_GetFirstObjectInRange(wb, &pos, range, mustHave, mustNot, what) - gGao),
                (long)(NEW_COL_GetFirstObjectInRange(wn, &pos, range, mustHave, mustNot, what) - gGao), "GetFirstObjectInRange");

        {
            char cap = (char)nexti(1, 8);
            memset(lb, 0, sizeof lb); memset(ln, 0, sizeof ln);
            eq_i(c, BASE_COL_ListCreateDistance(mustHave, mustNot, what, wb, lb, &pos, range, cap),
                    NEW_COL_ListCreateDistance(mustHave, mustNot, what, wn, ln, &pos, range, cap), "ListCreateDistance");
            eq_mem(c, lb, ln, sizeof lb, "ListCreateDistance out");
        }

        eq_i(c, (long)(BASE_COL_RayBV(&wb->activatedObjectsTable4PF, &pos, &dir, range, mustHave, mustNot, what, (int)(c & 1)) - gGao),
                (long)(NEW_COL_RayBV(&wn->activatedObjectsTable4PF, &pos, &dir, range, mustHave, mustNot, what, (int)(c & 1)) - gGao), "RayBV");
    }
    free(wb);
    free(wn);
}

static void test_trisweep(long n)
{
    long c;
    MeshGao* mb = (MeshGao*)malloc(sizeof(MeshGao));
    MeshGao* mn = (MeshGao*)malloc(sizeof(MeshGao));
    tWorld* wb = (tWorld*)malloc(sizeof(tWorld));
    tWorld* wn = (tWorld*)malloc(sizeof(tWorld));
    static tableElemStructPF eb[1], en[1];

    gGroup = "COL_TriangleCollision";
    for (c = 0; c < n; ++c)
    {
        Vec3 t1, t2, t3, outb, outn;
        int rb, rn;

        build_mesh_gao(mb, (unsigned)(0x11000 + c));
        build_mesh_gao(mn, (unsigned)(0x11000 + c));
        memset(wb, 0, sizeof *wb);
        memset(wn, 0, sizeof *wn);
        eb[0].objectPtr = &mb->gao; eb[0].objectFlags = mb->gao.flags;
        en[0].objectPtr = &mn->gao; en[0].objectFlags = mn->gao.flags;
        wb->eot.collMapObjectsTablePF.firstElemPtr = eb;
        wb->eot.collMapObjectsTablePF.nextElemPtr = &eb[1];
        wn->eot.collMapObjectsTablePF.firstElemPtr = en;
        wn->eot.collMapObjectsTablePF.nextElemPtr = &en[1];

        seed((unsigned)(0x12000 + c));
        /* A triangle standing on edge across the sheet, so the
           tri/tri test comes out both ways rather than always missing;
           one case in eight lies flat and one is well clear. */
        t1.x = (float)nexti(0, 40); t1.y = (float)nexti(0, 40); t1.z = -3.0f;
        t2.x = t1.x + (float)nexti(-8, 8); t2.y = t1.y + (float)nexti(-8, 8); t2.z = 3.0f;
        t3.x = t1.x + (float)nexti(-8, 8); t3.y = t1.y + (float)nexti(-8, 8); t3.z = 3.0f;
        if ((c & 7) == 0) { t1.z = (float)nexti(-3, 3); t2.z = t1.z; t3.z = t1.z; }
        if ((c & 7) == 1) { t1.z = 9.0f; t2.z = 9.0f; t3.z = 9.0f; }
        if ((c & 7) == 2) { t2.z = 20.0f; t3.z = 20.0f; }

        memset(&outb, 0xA5, sizeof outb);
        memset(&outn, 0xA5, sizeof outn);
        rb = BASE_COL_TriangleCollision(&t1, &t2, &t3, &outb, wb, 0, 0, 0);
        rn = NEW_COL_TriangleCollision(&t1, &t2, &t3, &outn, wn, 0, 0, 0);
        eq_i(c, rb, rn, "result");
        eq_mem(c, &outb, &outn, sizeof outb, "out point");
    }
    free(mb); free(mn); free(wb); free(wn);
}


/* ---- 9. COL_ListCreate's dispatch and COL_SwapToSpecific ---------- */

#define NZONE 5

static void test_listcreate(long n)
{
    long c;
    static ZoneGao zb[NZONE], zn[NZONE];
    static tableElemStructPF eb[NZONE], en[NZONE];
    tWorld* wb = (tWorld*)malloc(sizeof(tWorld));
    tWorld* wn = (tWorld*)malloc(sizeof(tWorld));

    gGroup = "COL_ListCreate";
    for (c = 0; c < n; ++c)
    {
        tGameObjectMain* lb[64];
        tGameObjectMain* ln[64];
        long ib[64], in_[64];
        int i, rb, rn;
        char mode;
        char id;

        for (i = 0; i < NZONE; ++i)
        {
            seed((unsigned)(0x13000 + c * NZONE + i));
            build_zone_gao(&zb[i], (int)((c + i) % 3));
            seed((unsigned)(0x13000 + c * NZONE + i));
            build_zone_gao(&zn[i], (int)((c + i) % 3));
            zb[i].gao.flags |= 0x400;
            zn[i].gao.flags |= 0x400;
            eb[i].objectPtr = &zb[i].gao;
            en[i].objectPtr = &zn[i].gao;
            eb[i].objectFlags = zb[i].gao.flags;
            en[i].objectFlags = zn[i].gao.flags;
        }
        memset(wb, 0, sizeof *wb);
        memset(wn, 0, sizeof *wn);
        /* The two tables hold different spans of the same objects, so
           which one the sentinel set selects is visible. */
        wb->activatedObjectsTable4PF.firstElemPtr = eb;
        wb->activatedObjectsTable4PF.nextElemPtr = &eb[NZONE];
        wb->eot.collInstanceZDEGameObjectsPF.firstElemPtr = &eb[1];
        wb->eot.collInstanceZDEGameObjectsPF.nextElemPtr = &eb[NZONE - 1];
        wn->activatedObjectsTable4PF.firstElemPtr = en;
        wn->activatedObjectsTable4PF.nextElemPtr = &en[NZONE];
        wn->eot.collInstanceZDEGameObjectsPF.firstElemPtr = &en[1];
        wn->eot.collInstanceZDEGameObjectsPF.nextElemPtr = &en[NZONE - 1];

        seed((unsigned)(0x14000 + c));
        id = (char)nexti(0, 15);
        /* the eight sentinels, and a plain zone number for the
           zone-against-zone arm the default case reaches */
        switch (c % 9)
        {
        case 0: mode = -1; break;
        case 1: mode = -2; break;
        case 2: mode = -3; break;
        case 3: mode = -4; break;
        case 4: mode = -5; break;
        case 5: mode = -6; break;
        case 6: mode = -17; break;
        case 7: mode = -18; break;
        default: mode = (char)nexti(0, 15); break;
        }

        memset(lb, 0, sizeof lb); memset(ln, 0, sizeof ln);
        rb = BASE_COL_ListCreate(&zb[0].gao, 0, 0, 0, id, wb, lb, mode);
        rn = NEW_COL_ListCreate(&zn[0].gao, 0, 0, 0, id, wn, ln, mode);
        eq_i(c, rb, rn, "result");
        /* the two copies hold different objects, so compare which
           object of the set each entry names */
        for (i = 0; i < 64; ++i)
        {
            ib[i] = lb[i] ? ((char*)lb[i] - (char*)zb) / (long)sizeof(ZoneGao) : -1;
            in_[i] = ln[i] ? ((char*)ln[i] - (char*)zn) / (long)sizeof(ZoneGao) : -1;
        }
        eq_mem(c, ib, in_, sizeof ib, "list");
    }
    free(wb); free(wn);
}

/* Where a zone slot points after the swap, as something that does not
   depend on the allocator: the instance's own element block moves
   under a realloc, so the two copies hold different addresses and only
   the index within whichever block a slot names can be compared. */
static long slot_tag(ZoneGao* z, int i)
{
    tColInstanceElem* p = z->inst.selectedInstElemPtrs[i];
    long k;

    if (!p) return -1;
    k = p - z->inst.colInstElems;
    if (k >= 0 && k < z->inst.instElemNum) return 100 + k;
    k = p - z->elems;
    if (k >= 0 && k < 4) return k;
    return -2;
}

static void test_swap(long n)
{
    long c;
    static ZoneGao zb, zn;

    gGroup = "COL_SwapToSpecific";
    for (c = 0; c < n; ++c)
    {
        uint8_t id;
        int i;
        long sb[4], sn[4];

        seed((unsigned)(0x15000 + c));
        id = (uint8_t)((c & 31) == 0 ? 0xFFu : (unsigned)nexti(0, 15));
        build_zone_gao(&zb, (int)(c % 3));
        seed((unsigned)(0x15000 + c));
        (void)nexti(0, 15);
        build_zone_gao(&zn, (int)(c % 3));

        /* Two elements of the instance's own in a heap block, so the
           realloc arm runs rather than the first-element arm. */
        if (c & 1)
        {
            zb.inst.colInstElems = (tColInstanceElem*)malloc(2 * sizeof(tColInstanceElem));
            zn.inst.colInstElems = (tColInstanceElem*)malloc(2 * sizeof(tColInstanceElem));
            memcpy(zb.inst.colInstElems, zb.elems, 2 * sizeof(tColInstanceElem));
            memcpy(zn.inst.colInstElems, zn.elems, 2 * sizeof(tColInstanceElem));
            zb.inst.instElemNum = 2;
            zn.inst.instElemNum = 2;
            zb.inst.instElemBitmask = 3;
            zn.inst.instElemBitmask = 3;
            zb.slots[0] = &zb.inst.colInstElems[0];
            zb.slots[1] = &zb.inst.colInstElems[1];
            zn.slots[0] = &zn.inst.colInstElems[0];
            zn.slots[1] = &zn.inst.colInstElems[1];
        }
        else
        {
            zb.inst.instElemNum = 0;
            zn.inst.instElemNum = 0;
            zb.inst.instElemBitmask = 0;
            zn.inst.instElemBitmask = 0;
            zb.inst.colInstElems = zb.elems;
            zn.inst.colInstElems = zn.elems;
        }
        zb.inst.sharedZoneNum = 4;
        zn.inst.sharedZoneNum = 4;

        BASE_COL_SwapToSpecific(&zb.gao, id);
        NEW_COL_SwapToSpecific(&zn.gao, id);

        eq_i(c, zb.inst.instElemNum, zn.inst.instElemNum, "instElemNum");
        eq_i(c, zb.inst.sharedZoneNum, zn.inst.sharedZoneNum, "sharedZoneNum");
        eq_i(c, zb.inst.instElemBitmask, zn.inst.instElemBitmask, "instElemBitmask");
        for (i = 0; i < 4; ++i)
        {
            sb[i] = slot_tag(&zb, i);
            sn[i] = slot_tag(&zn, i);
        }
        eq_mem(c, sb, sn, sizeof sb, "zone slots");
        if (zb.inst.instElemNum && zn.inst.instElemNum)
        {
            tColInstanceElem* ab = &zb.inst.colInstElems[zb.inst.instElemNum - 1];
            tColInstanceElem* an = &zn.inst.colInstElems[zn.inst.instElemNum - 1];
            eq_i(c, ab->elemType, an->elemType, "new elem type");
            eq_i(c, ab->colFlags, an->colFlags, "new elem colFlags");
            eq_i(c, ab->unk1, an->unk1, "new elem unk1");
            eq_i(c, ab->design, an->design, "new elem design");
            if (ab->elemShapePtr && an->elemShapePtr && ab->elemType == an->elemType)
                eq_mem(c, ab->elemShapePtr, an->elemShapePtr,
                       ab->elemType == 2 ? sizeof(tColSphere) : sizeof(tColBox), "new elem shape");
        }
    }
}

/* ---- 10. COL_GeoColMap_PosSet ------------------------------------ */

static void test_geocolmap(long n)
{
    long c;
    gGroup = "COL_GeoColMap_PosSet";
    for (c = 0; c < n; ++c)
    {
        tGameObjectMain gb, gn;
        tGameObjectExtended eb, en;
        tGameObjectCollMain cb, cn;
        tColMap mb, mn;
        tColCollisionObject ob[3], on[3];
        tColCollisionObject* pb[3];
        tColCollisionObject* pn[3];
        tCobNormalObjectGeneric gnb[3], gnn[3];
        tColSphere sb[3], sn[3];
        tColCylinder yb[3], yn[3];
        Vec3 pos;
        uint8_t idx;
        int i;

        seed((unsigned)(0x16000 + c));
        memset(&gb, 0, sizeof gb); memset(&gn, 0, sizeof gn);
        memset(&eb, 0, sizeof eb); memset(&en, 0, sizeof en);
        memset(&cb, 0, sizeof cb); memset(&cn, 0, sizeof cn);
        memset(&mb, 0, sizeof mb); memset(&mn, 0, sizeof mn);
        for (i = 0; i < 3; ++i)
        {
            memset(&ob[i], 0, sizeof ob[i]); memset(&on[i], 0, sizeof on[i]);
            ob[i].objectType = (uint8_t)nexti(1, 5);
            on[i].objectType = ob[i].objectType;
            randv(&sb[i].center); sb[i].radius = nextf();
            sn[i] = sb[i];
            randv(&yb[i].center); yb[i].radius = nextf(); yb[i].height = nextf();
            yn[i] = yb[i];
            gnb[i].gameMaterialElemID = 0;
            gnn[i].gameMaterialElemID = 0;
            gnb[i].enclosurePtr = (ob[i].objectType == 3) ? (void*)&yb[i] : (void*)&sb[i];
            gnn[i].enclosurePtr = (on[i].objectType == 3) ? (void*)&yn[i] : (void*)&sn[i];
            ob[i].objectPtrByType = &gnb[i];
            on[i].objectPtrByType = &gnn[i];
            pb[i] = &ob[i];
            pn[i] = &on[i];
        }
        mb.colObjectID = (uint8_t)nexti(0, 3);
        mn.colObjectID = mb.colObjectID;
        mb.collisionObjectPtr = pb;
        mn.collisionObjectPtr = pn;
        cb.colMap = &mb; cn.colMap = &mn;
        eb.coll = &cb; en.coll = &cn;
        gb.extended = &eb; gn.extended = &en;
        gb.flags = (nextu() & 1) ? 0x100 : 0;
        gn.flags = gb.flags;
        idx = (uint8_t)nexti(0, 3);
        randv(&pos);

        BASE_COL_GeoColMap_PosSet(&gb, idx, &pos);
        NEW_COL_GeoColMap_PosSet(&gn, idx, &pos);
        eq_mem(c, sb, sn, sizeof sb, "spheres");
        eq_mem(c, yb, yn, sizeof yb, "cylinders");

        if ((c & 15) == 0)
        {
            BASE_COL_GeoColMap_PosSet(NULL, idx, &pos);
            NEW_COL_GeoColMap_PosSet(NULL, idx, &pos);
            ++gCases;
        }
    }
}

/* ---- main --------------------------------------------------------- */

int main(void)
{
    printf("collision.c functions 98-158: frozen copy against the rewrite\n\n");

    /* The engine's base vectors and lookup tables. Without this
       MATH_gst_BaseVectorK is all zeroes and every dot product the
       edge search takes against it comes out zero. */
    MATH_InitModule();
    build_gaos();

    test_lowlevel(60000);
    test_highlevel(60000);
    test_tritri(60000);
    test_bv(20000);
    test_bvoverlap(20000);
    test_zone(6000);
    test_zde(12000);
    test_reports(3000);
    test_edges(500);
    test_jumppoint(500);
    test_lists(3000);
    test_trisweep(1200);
    test_listcreate(2000);
    test_swap(2000);
    test_geocolmap(4000);

    printf("\n%ld comparisons, %ld differing\n", gCases, gFails);
    if (gFails)
        printf("FAIL\n");
    else
        printf("all equivalent\n");
    return gFails != 0;
}
