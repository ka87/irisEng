/* collision.c functions 1..63, before the rewrite against after.
 *
 * This range holds the file's intersection geometry -- ray against
 * triangle, box, sphere and cylinder; sphere against sphere and
 * triangle; segment against sphere and triangle; the closest-point
 * helpers; the box outcodes -- and those are pure arithmetic over
 * plain structs, so they need no world and no loader. The table walks
 * that are equally self-contained are driven too: the game-material
 * lookup, the proximity builder, the face tags, the zone activation
 * and the two bounding-volume ray tests.
 *
 * Every comparison is BITWISE. The rewrite was renames, gotos turned
 * into control flow and three helper restorations whose bodies were
 * read field for field first, so a differing bit is a regression and
 * not rounding.
 *
 * What is NOT covered, and why, is in docs/cleanup/root.md: anything
 * that walks a cob mesh out of a loaded world, the ray-cast entry
 * points that write through COL_gst_Ray.worldSetInColMapRayCast, and
 * COL_ComputeReport, which needs a tColGlobal wired to a world, an
 * instance and a collision map at once.
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#include "../gameobject.h"
#include "../collision.h"

/* ---- the two copies ---------------------------------------------- */

#define D(ret, name, args) ret NEW_##name args; ret BASE_##name args;

D(int, INT_PointInTriangle, (Vec3*, Vec3*, Vec3*, Vec3*, Vec3*, uint8_t*))
D(BOOL, INT_PointSphere, (Vec3*, float, Vec3*))
D(BOOL, INT_SphereSphere, (Vec3*, float, Vec3*, float))
D(BOOL, INT_FullSphereSphere, (Vec3*, float, Vec3*, float, Vec3*, Vec3*))
D(int, INT_FullSegmentSphere, (Vec3*, Vec3*, Vec3*, float, Vec3*))
D(int, INT_FullSegmentTriangle, (Vec3*, Vec3*, Vec3*, Vec3*, Vec3*, Vec3*, Vec3*, uint8_t*))
D(int, INT_FullSphereTriangle, (Vec3*, float, Vec3*, Vec3*, Vec3*, Vec3*, Vec3*, int*))
D(int, INT_FullRayTriangle, (Vec3*, Vec3*, Vec3*, Vec3*, Vec3*, float*, Vec3*))
D(int, INT_FullRayAABBox, (Vec3*, Vec3*, tColBox*, Vec3*, float, float*))
D(int, INT_FullRaySphere, (Vec3*, Vec3*, tColSphere*, Vec3*, float, float*))
D(int, INT_b_FullRayCylinder, (Vec3*, Vec3*, Vec3*, Vec3*, float, float, float, float*, Vec3*))
D(Vec3*, INT_PointLineClosestPoint, (Vec3*, Vec3*, Vec3*, Vec3*, Vec3*))
D(void, INT_PointTriangleClosestPointGet, (Vec3*, Vec3*, Vec3*, Vec3*, Vec3*, Vec3*, uint32_t*))
D(BOOL, INT_DynamicHit, (Vec3*, float, Vec3*, float, Vec3*, Vec3*, Vec3*, Vec3*, Vec3*, Vec3*, int*))
D(int, INT_ul_GetPositionPointWithBox, (Vec3*, Vec3*, Vec3*))
D(int, INT_ul_GetPositionPointWithEdgeBox, (Vec3*, Vec3*, Vec3*, Vec3*, Vec3*))
D(BOOL, INT_b_InYZRange, (Vec3*, Vec3*, Vec3*))
D(BOOL, INT_b_InXZRange, (Vec3*, Vec3*, Vec3*))
D(BOOL, INT_b_InXYRange, (Vec3*, Vec3*, Vec3*))
D(int, INT_b_IntersectEdgeWithBoxWithHit, (Vec3*, Vec3*, Vec3*, Vec3*, Vec3*, char, Vec3*))

D(tCobGameMaterialElems*, COL_pst_GMat_Get, (tColCollisionObject*, tCobMeshChunkElem*))
D(BOOL, COL_GMat_Skip, (tCobGameMaterialElems*, tColGlobal*))
D(void, COL_ComputeProximity, (tColCollisionObject*))
D(void, COL_SetFaceTag, (tCobOK3*, unsigned int))
D(BOOL, COL_b_TestFaceTag, (tCobOK3*, unsigned int))
D(int, COL_i_ZoneActivationGet, (tColInstance*, char))
D(int16_t, COL_w_TestInstanceFlag, (struct sColInstance*, int16_t))
D(BOOL, COL_NeedRecomputation, (tColGlobal*, tGameObjectMain*, int16_t))
D(int, COL_RaySphereBV, (Vec3*, Vec3*, tGameObjectMain*, float*, float))
D(int, COL_RayAABBox, (Vec3*, Vec3*, tGameObjectMain*, float*, float))
D(BOOL, COL_b_IsARealTopologicEdge, (tColGlobal*, Vec3*, uint8_t))

void COLA_MockResetArena(void);

/* ---- bookkeeping -------------------------------------------------- */

static long gCases;
static long gDiffs;
static const char* gGroup = "?";
static long gGroupCases;

static long gGroupDiffs;

static void group(const char* name)
{
    if (gGroupCases)
        printf("  %-34s %8ld cases %8ld differing\n", gGroup, gGroupCases, gGroupDiffs);
    gGroup = name;
    gGroupCases = 0;
    gGroupDiffs = 0;
}

static void endGroups(void)
{
    group("");
    gGroupCases = 0;
}

/* A case passes only when every captured byte matches. Floats are
   compared as bits, so a NaN payload or a signed zero that moved is a
   difference. */
static void check(const char* what, const void* a, const void* b, size_t n)
{
    ++gCases;
    ++gGroupCases;
    if (memcmp(a, b, n) == 0)
        return;
    ++gDiffs;
    ++gGroupDiffs;
    if (gDiffs <= 20)
    {
        size_t i;
        printf("  DIFF %s:\n    base", what);
        for (i = 0; i < n; i++) printf(" %02x", ((const unsigned char*)b)[i]);
        printf("\n    new ");
        for (i = 0; i < n; i++) printf(" %02x", ((const unsigned char*)a)[i]);
        printf("\n");
    }
}

/* ---- inputs ------------------------------------------------------- */

static unsigned int gSeed = 0x1234567u;

static unsigned int nextRand(void)
{
    gSeed = gSeed * 1103515245u + 12345u;
    return (gSeed >> 8) & 0x7fffffu;
}

static float randf(float lo, float hi)
{
    return lo + (hi - lo) * ((float)nextRand() / (float)0x800000u);
}

/* Values chosen so the degenerate cases are reached rather than hoped
   for: zero, the two signed zeroes, exact halves and the epsilon the
   file tests against. */
static const float kScalars[] = {
    0.0f, -0.0f, 1.0f, -1.0f, 0.5f, -0.5f, 2.0f, -2.0f,
    0.0000099999997f, -0.0000099999997f, 0.000001f, 1e-7f,
    3.0f, -3.0f, 100.0f, -100.0f, 0.57735026f, -0.57735026f
};
#define NSCALARS ((int)(sizeof(kScalars) / sizeof(kScalars[0])))

static void setv(Vec3* v, float x, float y, float z)
{
    v->x = x;
    v->y = y;
    v->z = z;
}

static void randv(Vec3* v, float r)
{
    setv(v, randf(-r, r), randf(-r, r), randf(-r, r));
}

static void randUnit(Vec3* v)
{
    float len;
    do
    {
        randv(v, 1.0f);
        len = (float)sqrt(v->x * v->x + v->y * v->y + v->z * v->z);
    } while (len < 0.01f);
    v->x /= len;
    v->y /= len;
    v->z /= len;
}

/* The unnormalised face normal of a triangle, which is what the file's
   own callers hand these functions. */
static void faceNormal(Vec3* n, const Vec3* a, const Vec3* b, const Vec3* c)
{
    float ux = b->x - a->x, uy = b->y - a->y, uz = b->z - a->z;
    float vx = c->x - a->x, vy = c->y - a->y, vz = c->z - a->z;
    float len;
    n->x = uy * vz - uz * vy;
    n->y = uz * vx - ux * vz;
    n->z = ux * vy - uy * vx;
    len = (float)sqrt(n->x * n->x + n->y * n->y + n->z * n->z);
    if (len > 0.0f)
    {
        n->x /= len;
        n->y /= len;
        n->z /= len;
    }
}

/* ---- 1. INT_PointInTriangle --------------------------------------- */

static void runPointInTriangle(void)
{
    Vec3 p1, p2, p3, n, pt;
    uint8_t ea, eb;
    int ra, rb;
    int i, j;
    struct { int r; uint8_t e; } A, B;

    group("INT_PointInTriangle");

    for (i = 0; i < 4000; i++)
    {
        randv(&p1, 10.0f);
        randv(&p2, 10.0f);
        randv(&p3, 10.0f);
        faceNormal(&n, &p1, &p2, &p3);

        /* inside, outside, on each vertex, on each edge midpoint, on
           the plane but far away, and a point with a degenerate normal */
        for (j = 0; j < 12; j++)
        {
            float u, v;
            switch (j)
            {
            case 0: u = 1.0f / 3.0f; v = 1.0f / 3.0f; break;
            case 1: u = 0.0f; v = 0.0f; break;               /* on p1 */
            case 2: u = 1.0f; v = 0.0f; break;               /* on p2 */
            case 3: u = 0.0f; v = 1.0f; break;               /* on p3 */
            case 4: u = 0.5f; v = 0.0f; break;               /* edge p1p2 */
            case 5: u = 0.0f; v = 0.5f; break;               /* edge p1p3 */
            case 6: u = 0.5f; v = 0.5f; break;               /* edge p2p3 */
            case 7: u = 2.0f; v = 0.0f; break;
            case 8: u = 0.0f; v = 2.0f; break;
            case 9: u = -1.0f; v = -1.0f; break;
            case 10: u = 0.9f; v = 0.09f; break;
            default: u = randf(-1.5f, 2.5f); v = randf(-1.5f, 2.5f); break;
            }
            pt.x = p1.x + u * (p2.x - p1.x) + v * (p3.x - p1.x);
            pt.y = p1.y + u * (p2.y - p1.y) + v * (p3.y - p1.y);
            pt.z = p1.z + u * (p2.z - p1.z) + v * (p3.z - p1.z);

            ea = 0xAA; eb = 0xAA;
            ra = NEW_INT_PointInTriangle(&pt, &p1, &p2, &p3, &n, &ea);
            rb = BASE_INT_PointInTriangle(&pt, &p1, &p2, &p3, &n, &eb);
            memset(&A, 0, sizeof(A)); memset(&B, 0, sizeof(B));
            A.r = ra; A.e = ea; B.r = rb; B.e = eb;
            check("INT_PointInTriangle", &A, &B, sizeof(A));

            /* and the null-edge-out path */
            ra = NEW_INT_PointInTriangle(&pt, &p1, &p2, &p3, &n, 0);
            rb = BASE_INT_PointInTriangle(&pt, &p1, &p2, &p3, &n, 0);
            check("INT_PointInTriangle/noedge", &ra, &rb, sizeof(ra));
        }
    }

    /* the dominant-axis pick is driven by |n| against 1/sqrt(3), so
       walk a normal across both thresholds explicitly */
    for (i = 0; i < NSCALARS; i++)
    {
        for (j = 0; j < NSCALARS; j++)
        {
            setv(&p1, 0.0f, 0.0f, 0.0f);
            setv(&p2, 1.0f, 0.0f, 0.0f);
            setv(&p3, 0.0f, 1.0f, 0.0f);
            setv(&n, kScalars[i], kScalars[j], 0.57735026f);
            setv(&pt, 0.25f, 0.25f, 0.0f);
            ea = 0xAA; eb = 0xAA;
            ra = NEW_INT_PointInTriangle(&pt, &p1, &p2, &p3, &n, &ea);
            rb = BASE_INT_PointInTriangle(&pt, &p1, &p2, &p3, &n, &eb);
            memset(&A, 0, sizeof(A)); memset(&B, 0, sizeof(B));
            A.r = ra; A.e = ea; B.r = rb; B.e = eb;
            check("INT_PointInTriangle/axis", &A, &B, sizeof(A));
        }
    }
}

/* ---- 2. the sphere tests ------------------------------------------ */

static void runSpheres(void)
{
    Vec3 c1, c2, na, nb, pa, pb;
    float r1, r2;
    int i, j;
    BOOL ra, rb;
    struct { BOOL r; Vec3 n; Vec3 p; } A, B;

    group("INT_PointSphere");
    for (i = 0; i < 2000; i++)
    {
        randv(&c1, 5.0f);
        randv(&c2, 5.0f);
        r1 = randf(-1.0f, 5.0f);
        ra = NEW_INT_PointSphere(&c1, r1, &c2);
        rb = BASE_INT_PointSphere(&c1, r1, &c2);
        check("INT_PointSphere", &ra, &rb, sizeof(ra));
    }
    /* exactly touching: radius equal to the distance, both signs of 0 */
    for (i = 0; i < NSCALARS; i++)
    {
        setv(&c1, 0.0f, 0.0f, 0.0f);
        setv(&c2, kScalars[i], 0.0f, 0.0f);
        r1 = kScalars[i] < 0.0f ? -kScalars[i] : kScalars[i];
        ra = NEW_INT_PointSphere(&c1, r1, &c2);
        rb = BASE_INT_PointSphere(&c1, r1, &c2);
        check("INT_PointSphere/touch", &ra, &rb, sizeof(ra));
    }

    group("INT_SphereSphere");
    for (i = 0; i < NSCALARS; i++)
    {
        for (j = 0; j < NSCALARS; j++)
        {
            setv(&c1, 0.0f, 0.0f, 0.0f);
            setv(&c2, 1.0f, 0.0f, 0.0f);
            ra = NEW_INT_SphereSphere(&c1, kScalars[i], &c2, kScalars[j]);
            rb = BASE_INT_SphereSphere(&c1, kScalars[i], &c2, kScalars[j]);
            check("INT_SphereSphere", &ra, &rb, sizeof(ra));
        }
    }
    for (i = 0; i < 2000; i++)
    {
        randv(&c1, 5.0f);
        randv(&c2, 5.0f);
        ra = NEW_INT_SphereSphere(&c1, randf(0.0f, 4.0f), &c2, randf(0.0f, 4.0f));
        rb = BASE_INT_SphereSphere(&c1, randf(0.0f, 4.0f), &c2, randf(0.0f, 4.0f));
        (void)ra; (void)rb;
    }
    /* the loop above would feed the two copies different randoms, so
       the real sweep draws first and then calls both */
    for (i = 0; i < 4000; i++)
    {
        randv(&c1, 5.0f);
        randv(&c2, 5.0f);
        r1 = randf(0.0f, 4.0f);
        r2 = randf(0.0f, 4.0f);
        ra = NEW_INT_SphereSphere(&c1, r1, &c2, r2);
        rb = BASE_INT_SphereSphere(&c1, r1, &c2, r2);
        check("INT_SphereSphere/rand", &ra, &rb, sizeof(ra));
    }

    group("INT_FullSphereSphere");
    for (i = 0; i < 6000; i++)
    {
        randv(&c1, 5.0f);
        randv(&c2, 5.0f);
        r1 = randf(0.0f, 5.0f);
        r2 = randf(0.0f, 5.0f);
        if ((i & 15) == 0)
            c2 = c1;                                  /* coincident centres */
        memset(&na, 0xA5, sizeof(na));
        memset(&nb, 0xA5, sizeof(nb));
        memset(&pa, 0xA5, sizeof(pa));
        memset(&pb, 0xA5, sizeof(pb));
        A.r = NEW_INT_FullSphereSphere(&c1, r1, &c2, r2, &na, &pa);
        B.r = BASE_INT_FullSphereSphere(&c1, r1, &c2, r2, &nb, &pb);
        A.n = na; A.p = pa;
        B.n = nb; B.p = pb;
        check("INT_FullSphereSphere", &A, &B, sizeof(A));
    }
}

/* ---- 3. segment tests --------------------------------------------- */

static void runSegments(void)
{
    Vec3 s1, s2, c, p1, p2, p3, n, ha, hb;
    float r;
    int i, j;
    uint8_t ea, eb;
    int ra2, rb2;
    struct { int r; Vec3 h; } A, B;
    struct { int r; Vec3 h; uint8_t e; int t1, t2; } C, Dd;

    group("INT_FullSegmentSphere");
    for (i = 0; i < 8000; i++)
    {
        randv(&s1, 6.0f);
        randv(&s2, 6.0f);
        randv(&c, 4.0f);
        r = randf(0.0f, 4.0f);
        if ((i & 31) == 0) s2 = s1;                   /* zero-length segment */
        if ((i & 63) == 0) c = s1;                    /* centre on the start */
        memset(&ha, 0xA5, sizeof(ha));
        memset(&hb, 0xA5, sizeof(hb));
        A.r = NEW_INT_FullSegmentSphere(&s1, &s2, &c, r, &ha);
        B.r = BASE_INT_FullSegmentSphere(&s1, &s2, &c, r, &hb);
        A.h = ha; B.h = hb;
        check("INT_FullSegmentSphere", &A, &B, sizeof(A));
    }

    group("INT_FullSegmentTriangle");
    for (i = 0; i < 8000; i++)
    {
        randv(&p1, 8.0f);
        randv(&p2, 8.0f);
        randv(&p3, 8.0f);
        faceNormal(&n, &p1, &p2, &p3);
        randv(&s1, 8.0f);
        randv(&s2, 8.0f);
        if ((i & 15) == 0)
        {
            /* a segment that crosses the centroid, so the hit path runs */
            s1.x = (p1.x + p2.x + p3.x) / 3.0f + n.x;
            s1.y = (p1.y + p2.y + p3.y) / 3.0f + n.y;
            s1.z = (p1.z + p2.z + p3.z) / 3.0f + n.z;
            s2.x = s1.x - 2.0f * n.x;
            s2.y = s1.y - 2.0f * n.y;
            s2.z = s1.z - 2.0f * n.z;
        }
        if ((i & 255) == 0) s2 = s1;                  /* degenerate segment */
        memset(&ha, 0xA5, sizeof(ha));
        memset(&hb, 0xA5, sizeof(hb));
        ea = 0xAA; eb = 0xAA;
        ra2 = NEW_INT_FullSegmentTriangle(&s1, &s2, &p1, &p2, &p3, &n, &ha, &ea);
        rb2 = BASE_INT_FullSegmentTriangle(&s1, &s2, &p1, &p2, &p3, &n, &hb, &eb);
        memset(&C, 0, sizeof(C)); memset(&Dd, 0, sizeof(Dd));
        C.r = ra2; Dd.r = rb2;
        C.h = ha; C.e = ea; C.t1 = 0; C.t2 = 0;
        Dd.h = hb; Dd.e = eb; Dd.t1 = 0; Dd.t2 = 0;
        check("INT_FullSegmentTriangle", &C, &Dd, sizeof(C));
    }

    group("INT_FullSphereTriangle");
    for (i = 0; i < 8000; i++)
    {
        int ta = -1, tb = -1;
        randv(&p1, 6.0f);
        randv(&p2, 6.0f);
        randv(&p3, 6.0f);
        faceNormal(&n, &p1, &p2, &p3);
        r = randf(0.0f, 4.0f);
        randv(&c, 6.0f);
        if ((i & 7) == 0)
        {
            /* centre just off the centroid along the normal, which is
               the configuration that reaches every edge arm */
            float t = randf(-2.0f, 2.0f);
            c.x = (p1.x + p2.x + p3.x) / 3.0f + t * n.x;
            c.y = (p1.y + p2.y + p3.y) / 3.0f + t * n.y;
            c.z = (p1.z + p2.z + p3.z) / 3.0f + t * n.z;
        }
        if ((i & 63) == 0)
        {
            /* centre sitting exactly on a vertex */
            c = (i & 128) ? p2 : p1;
        }
        memset(&ha, 0xA5, sizeof(ha));
        memset(&hb, 0xA5, sizeof(hb));
        ra2 = NEW_INT_FullSphereTriangle(&c, r, &p1, &p2, &p3, &n, &ha, &ta);
        rb2 = BASE_INT_FullSphereTriangle(&c, r, &p1, &p2, &p3, &n, &hb, &tb);
        memset(&C, 0, sizeof(C)); memset(&Dd, 0, sizeof(Dd));
        C.r = ra2; Dd.r = rb2;
        C.h = ha; C.e = 0; C.t1 = ta; C.t2 = 0;
        Dd.h = hb; Dd.e = 0; Dd.t1 = tb; Dd.t2 = 0;
        check("INT_FullSphereTriangle", &C, &Dd, sizeof(C));
    }

    (void)j;
}

/* ---- 4. the ray tests --------------------------------------------- */

static void runRays(void)
{
    Vec3 o, d, p1, p2, p3, ha, hb, axis, c;
    tColBox box;
    tColSphere sph;
    float da, db, maxDist;
    int i;
    struct { int r; Vec3 h; float d; } A, B;

    group("INT_FullRayTriangle");
    for (i = 0; i < 10000; i++)
    {
        randv(&p1, 8.0f);
        randv(&p2, 8.0f);
        randv(&p3, 8.0f);
        randv(&o, 8.0f);
        randUnit(&d);
        if ((i & 7) == 0)
        {
            /* aim at the centroid so the hit arm runs */
            float cx = (p1.x + p2.x + p3.x) / 3.0f - o.x;
            float cy = (p1.y + p2.y + p3.y) / 3.0f - o.y;
            float cz = (p1.z + p2.z + p3.z) / 3.0f - o.z;
            float l = (float)sqrt(cx * cx + cy * cy + cz * cz);
            if (l > 0.0001f) setv(&d, cx / l, cy / l, cz / l);
        }
        if ((i & 127) == 0) p3 = p2;                  /* degenerate triangle */
        if ((i & 255) == 0) setv(&d, 0.0f, 0.0f, 0.0f);

        memset(&ha, 0xA5, sizeof(ha));
        memset(&hb, 0xA5, sizeof(hb));
        da = -12345.0f; db = -12345.0f;
        A.r = NEW_INT_FullRayTriangle(&o, &d, &p1, &p2, &p3, &da, &ha);
        B.r = BASE_INT_FullRayTriangle(&o, &d, &p1, &p2, &p3, &db, &hb);
        A.h = ha; A.d = da;
        B.h = hb; B.d = db;
        check("INT_FullRayTriangle", &A, &B, sizeof(A));

        /* the hitOut == NULL arm */
        da = -12345.0f; db = -12345.0f;
        A.r = NEW_INT_FullRayTriangle(&o, &d, &p1, &p2, &p3, &da, 0);
        B.r = BASE_INT_FullRayTriangle(&o, &d, &p1, &p2, &p3, &db, 0);
        A.d = da; B.d = db;
        memset(&A.h, 0, sizeof(A.h));
        memset(&B.h, 0, sizeof(B.h));
        check("INT_FullRayTriangle/nohit", &A, &B, sizeof(A));
    }

    group("INT_FullRayAABBox");
    for (i = 0; i < 20000; i++)
    {
        Vec3 lo, hi;
        randv(&lo, 5.0f);
        randv(&hi, 5.0f);
        box.pos1.x = lo.x < hi.x ? lo.x : hi.x;
        box.pos1.y = lo.y < hi.y ? lo.y : hi.y;
        box.pos1.z = lo.z < hi.z ? lo.z : hi.z;
        box.pos2.x = lo.x < hi.x ? hi.x : lo.x;
        box.pos2.y = lo.y < hi.y ? hi.y : lo.y;
        box.pos2.z = lo.z < hi.z ? hi.z : lo.z;

        randv(&o, 8.0f);
        randUnit(&d);
        maxDist = randf(0.0f, 20.0f);

        /* every mix of inside/outside per axis, and a ray that is
           axis-parallel on one or more axes */
        if ((i & 3) == 0) { o = box.pos1; }
        if ((i & 7) == 0) { o = box.pos2; }
        if ((i & 15) == 0) { setv(&d, 1.0f, 0.0f, 0.0f); }
        if ((i & 31) == 0) { setv(&d, 0.0f, 1.0f, 0.0f); }
        if ((i & 63) == 0) { setv(&d, 0.0f, 0.0f, 1.0f); }
        if ((i & 127) == 0) { setv(&d, 0.0f, 0.0f, 0.0f); }
        if ((i & 255) == 0) { box.pos2 = box.pos1; }

        memset(&ha, 0xA5, sizeof(ha));
        memset(&hb, 0xA5, sizeof(hb));
        da = -12345.0f; db = -12345.0f;
        A.r = NEW_INT_FullRayAABBox(&o, &d, &box, &ha, maxDist, &da);
        B.r = BASE_INT_FullRayAABBox(&o, &d, &box, &hb, maxDist, &db);
        A.h = ha; A.d = da;
        B.h = hb; B.d = db;
        check("INT_FullRayAABBox", &A, &B, sizeof(A));

        /* hitOut NULL and distOut NULL, which are both live call forms
           inside this file */
        A.r = NEW_INT_FullRayAABBox(&o, &d, &box, 0, maxDist, 0);
        B.r = BASE_INT_FullRayAABBox(&o, &d, &box, 0, maxDist, 0);
        memset(&A.h, 0, sizeof(A.h)); A.d = 0;
        memset(&B.h, 0, sizeof(B.h)); B.d = 0;
        check("INT_FullRayAABBox/null", &A, &B, sizeof(A));
    }

    group("INT_FullRaySphere");
    for (i = 0; i < 10000; i++)
    {
        randv(&o, 8.0f);
        randUnit(&d);
        randv(&sph.center, 6.0f);
        sph.radius = randf(0.0f, 5.0f);
        maxDist = randf(0.0f, 20.0f);
        if ((i & 15) == 0) sph.center = o;            /* origin at the centre */
        if ((i & 31) == 0)
        {
            /* origin exactly on the surface */
            sph.center.x = o.x + sph.radius;
            sph.center.y = o.y;
            sph.center.z = o.z;
        }
        memset(&ha, 0xA5, sizeof(ha));
        memset(&hb, 0xA5, sizeof(hb));
        da = -12345.0f; db = -12345.0f;
        A.r = NEW_INT_FullRaySphere(&o, &d, &sph, &ha, maxDist, &da);
        B.r = BASE_INT_FullRaySphere(&o, &d, &sph, &hb, maxDist, &db);
        A.h = ha; A.d = da;
        B.h = hb; B.d = db;
        check("INT_FullRaySphere", &A, &B, sizeof(A));

        da = -12345.0f; db = -12345.0f;
        A.r = NEW_INT_FullRaySphere(&o, &d, &sph, 0, maxDist, &da);
        B.r = BASE_INT_FullRaySphere(&o, &d, &sph, 0, maxDist, &db);
        A.d = da; B.d = db;
        memset(&A.h, 0, sizeof(A.h));
        memset(&B.h, 0, sizeof(B.h));
        check("INT_FullRaySphere/nohit", &A, &B, sizeof(A));
    }

    group("INT_b_FullRayCylinder");
    for (i = 0; i < 10000; i++)
    {
        float radius, halfHeight;
        randv(&o, 8.0f);
        randUnit(&d);
        randv(&c, 5.0f);
        randUnit(&axis);
        if ((i & 3) == 0) setv(&axis, 0.0f, 0.0f, 1.0f);
        if ((i & 15) == 0) d = axis;                  /* ray along the axis */
        if ((i & 31) == 0) { d.x = -axis.x; d.y = -axis.y; d.z = -axis.z; }
        radius = randf(0.0f, 4.0f);
        halfHeight = randf(0.0f, 6.0f);
        maxDist = randf(0.0f, 20.0f);

        memset(&ha, 0xA5, sizeof(ha));
        memset(&hb, 0xA5, sizeof(hb));
        da = -12345.0f; db = -12345.0f;
        A.r = NEW_INT_b_FullRayCylinder(&o, &d, &c, &axis, radius, halfHeight, maxDist, &da, &ha);
        B.r = BASE_INT_b_FullRayCylinder(&o, &d, &c, &axis, radius, halfHeight, maxDist, &db, &hb);
        A.h = ha; A.d = da;
        B.h = hb; B.d = db;
        check("INT_b_FullRayCylinder", &A, &B, sizeof(A));

        memset(&ha, 0xA5, sizeof(ha));
        memset(&hb, 0xA5, sizeof(hb));
        A.r = NEW_INT_b_FullRayCylinder(&o, &d, &c, &axis, radius, halfHeight, maxDist, 0, &ha);
        B.r = BASE_INT_b_FullRayCylinder(&o, &d, &c, &axis, radius, halfHeight, maxDist, 0, &hb);
        A.h = ha; A.d = 0;
        B.h = hb; B.d = 0;
        check("INT_b_FullRayCylinder/nodist", &A, &B, sizeof(A));
    }
}

/* ---- 5. the closest-point helpers --------------------------------- */

static void runClosest(void)
{
    Vec3 pt, ls, le, toPt, oa, ob, p1, p2, p3, n, c1, c2, dir;
    uint32_t ta, tb;
    int ea, eb;
    int i;
    struct { Vec3 o; uint32_t t; int r; } A, B;

    group("INT_PointLineClosestPoint");
    for (i = 0; i < 10000; i++)
    {
        randv(&pt, 8.0f);
        randv(&ls, 8.0f);
        randv(&le, 8.0f);
        if ((i & 31) == 0) le = ls;                   /* zero-length line */
        if ((i & 63) == 0) pt = ls;
        toPt.x = ls.x - pt.x;
        toPt.y = ls.y - pt.y;
        toPt.z = ls.z - pt.z;
        memset(&oa, 0xA5, sizeof(oa));
        memset(&ob, 0xA5, sizeof(ob));
        NEW_INT_PointLineClosestPoint(&pt, &ls, &le, &toPt, &oa);
        BASE_INT_PointLineClosestPoint(&pt, &ls, &le, &toPt, &ob);
        check("INT_PointLineClosestPoint", &oa, &ob, sizeof(oa));
    }

    group("INT_PointTriangleClosestPointGet");
    for (i = 0; i < 10000; i++)
    {
        randv(&p1, 8.0f);
        randv(&p2, 8.0f);
        randv(&p3, 8.0f);
        faceNormal(&n, &p1, &p2, &p3);
        randv(&pt, 8.0f);
        if ((i & 7) == 0)
        {
            pt.x = (p1.x + p2.x + p3.x) / 3.0f;
            pt.y = (p1.y + p2.y + p3.y) / 3.0f;
            pt.z = (p1.z + p2.z + p3.z) / 3.0f;
        }
        if ((i & 63) == 0) pt = p1;                   /* equidistant tie */
        memset(&oa, 0xA5, sizeof(oa));
        memset(&ob, 0xA5, sizeof(ob));
        ta = 0xDEADBEEFu; tb = 0xDEADBEEFu;
        NEW_INT_PointTriangleClosestPointGet(&p1, &p2, &p3, &n, &pt, &oa, &ta);
        BASE_INT_PointTriangleClosestPointGet(&p1, &p2, &p3, &n, &pt, &ob, &tb);
        A.o = oa; A.t = ta; A.r = 0;
        B.o = ob; B.t = tb; B.r = 0;
        check("INT_PointTriangleClosestPointGet", &A, &B, sizeof(A));
    }

    group("INT_DynamicHit");
    for (i = 0; i < 12000; i++)
    {
        float r1, r2;
        randv(&p1, 6.0f);
        randv(&p2, 6.0f);
        randv(&p3, 6.0f);
        faceNormal(&n, &p1, &p2, &p3);
        randv(&c1, 6.0f);
        randv(&c2, 6.0f);
        randUnit(&dir);
        r1 = randf(0.0f, 5.0f);
        r2 = randf(0.0f, 5.0f);
        if ((i & 7) == 0)
        {
            /* a swept sphere aimed through the centroid */
            c2.x = (p1.x + p2.x + p3.x) / 3.0f;
            c2.y = (p1.y + p2.y + p3.y) / 3.0f;
            c2.z = (p1.z + p2.z + p3.z) / 3.0f;
            c1.x = c2.x + 2.0f * n.x;
            c1.y = c2.y + 2.0f * n.y;
            c1.z = c2.z + 2.0f * n.z;
            dir = n;
        }
        if ((i & 63) == 0) dir = n;                   /* sweep along the normal */
        if ((i & 127) == 0)
        {
            /* sweep inside the plane, which makes the dot zero */
            dir.x = p2.x - p1.x;
            dir.y = p2.y - p1.y;
            dir.z = p2.z - p1.z;
        }
        memset(&oa, 0xA5, sizeof(oa));
        memset(&ob, 0xA5, sizeof(ob));
        ea = -1; eb = -1;
        A.r = NEW_INT_DynamicHit(&c1, r1, &c2, r2, &dir, &p1, &p2, &p3, &n, &oa, &ea);
        B.r = BASE_INT_DynamicHit(&c1, r1, &c2, r2, &dir, &p1, &p2, &p3, &n, &ob, &eb);
        A.o = oa; A.t = (uint32_t)ea;
        B.o = ob; B.t = (uint32_t)eb;
        check("INT_DynamicHit", &A, &B, sizeof(A));
    }
}

/* ---- 6. the box outcodes ------------------------------------------ */

static void runOutcodes(void)
{
    Vec3 pt, lo, hi, extent, negExtent, ha, hb, e1, e2, dir;
    int i, k;
    int ra, rb;
    BOOL ba, bb;
    struct { int r; Vec3 h; } A, B;

    group("INT_ul_GetPositionPointWithBox");
    for (i = 0; i < 20000; i++)
    {
        randv(&lo, 4.0f);
        randv(&hi, 4.0f);
        randv(&pt, 6.0f);
        if ((i & 7) == 0) pt = lo;                    /* exactly on the min */
        if ((i & 15) == 0) pt = hi;                   /* exactly on the max */
        ra = NEW_INT_ul_GetPositionPointWithBox(&pt, &lo, &hi);
        rb = BASE_INT_ul_GetPositionPointWithBox(&pt, &lo, &hi);
        check("INT_ul_GetPositionPointWithBox", &ra, &rb, sizeof(ra));
    }

    group("INT_ul_GetPositionPointWithEdgeBox");
    for (i = 0; i < 20000; i++)
    {
        randv(&lo, 4.0f);
        randv(&hi, 4.0f);
        randv(&pt, 6.0f);
        /* the caller builds these two out of the box extent */
        setv(&extent, hi.x - lo.x, hi.y - lo.y, hi.z - lo.z);
        setv(&negExtent, -extent.x, -extent.y, -extent.z);
        if ((i & 15) == 0) pt = lo;
        if ((i & 31) == 0) pt = hi;
        if ((i & 63) == 0) { hi = lo; setv(&extent, 0.0f, 0.0f, 0.0f); setv(&negExtent, -0.0f, -0.0f, -0.0f); }
        ra = NEW_INT_ul_GetPositionPointWithEdgeBox(&pt, &lo, &hi, &extent, &negExtent);
        rb = BASE_INT_ul_GetPositionPointWithEdgeBox(&pt, &lo, &hi, &extent, &negExtent);
        check("INT_ul_GetPositionPointWithEdgeBox", &ra, &rb, sizeof(ra));
    }

    group("INT_b_In*Range");
    for (i = 0; i < 10000; i++)
    {
        randv(&lo, 4.0f);
        randv(&hi, 4.0f);
        randv(&pt, 5.0f);
        if ((i & 3) == 0) pt = lo;
        if ((i & 7) == 0) pt = hi;
        ba = NEW_INT_b_InYZRange(&pt, &lo, &hi);
        bb = BASE_INT_b_InYZRange(&pt, &lo, &hi);
        check("INT_b_InYZRange", &ba, &bb, sizeof(ba));
        ba = NEW_INT_b_InXZRange(&pt, &lo, &hi);
        bb = BASE_INT_b_InXZRange(&pt, &lo, &hi);
        check("INT_b_InXZRange", &ba, &bb, sizeof(ba));
        ba = NEW_INT_b_InXYRange(&pt, &lo, &hi);
        bb = BASE_INT_b_InXYRange(&pt, &lo, &hi);
        check("INT_b_InXYRange", &ba, &bb, sizeof(ba));
    }

    group("INT_b_IntersectEdgeWithBoxWithHit");
    for (i = 0; i < 4000; i++)
    {
        randv(&lo, 4.0f);
        randv(&hi, 4.0f);
        if (hi.x < lo.x) { float t = hi.x; hi.x = lo.x; lo.x = t; }
        if (hi.y < lo.y) { float t = hi.y; hi.y = lo.y; lo.y = t; }
        if (hi.z < lo.z) { float t = hi.z; hi.z = lo.z; lo.z = t; }
        randv(&e1, 6.0f);
        randv(&e2, 6.0f);
        setv(&dir, e2.x - e1.x, e2.y - e1.y, e2.z - e1.z);
        if ((i & 31) == 0) setv(&dir, 0.0f, 0.0f, 0.0f);
        /* every one of the 64 plane masks, so no arm is left untried */
        for (k = 0; k < 64; k++)
        {
            memset(&ha, 0xA5, sizeof(ha));
            memset(&hb, 0xA5, sizeof(hb));
            A.r = NEW_INT_b_IntersectEdgeWithBoxWithHit(&e1, &e2, &dir, &lo, &hi, (char)k, &ha);
            B.r = BASE_INT_b_IntersectEdgeWithBoxWithHit(&e1, &e2, &dir, &lo, &hi, (char)k, &hb);
            A.h = ha; B.h = hb;
            check("INT_b_IntersectEdgeWithBoxWithHit", &A, &B, sizeof(A));
        }
    }
}

/* ---- 7. the self-contained table walks ----------------------------- */

static void runTables(void)
{
    int i, j;
    group("COL_pst_GMat_Get");
    for (i = 0; i < 400; i++)
    {
        /* a material table of i entries with ascending ids, driven
           through both the linear and the binary-search arms */
        static int ids[64];
        static tCobGameMaterialElems elemsA[64];
        static tCobGameMaterialElems elemsB[64];
        tCobGameMaterial gmA, gmB;
        tColCollisionObject cobA, cobB;
        tCobMeshChunkElem chunk;
        tCobNormalObjectGeneric shape;
        tCobGameMaterialElems* ra;
        tCobGameMaterialElems* rb;
        long offA, offB;
        int num = i % 40;

        for (j = 0; j < num; j++)
            ids[j] = j * 3 + 1;
        memset(elemsA, 0, sizeof(elemsA));
        memset(elemsB, 0, sizeof(elemsB));

        gmA.num = num; gmA.ptrInts = ids; gmA.elems = elemsA; gmA.gameMaterialUsageCnt = 1;
        gmB.num = num; gmB.ptrInts = ids; gmB.elems = elemsB; gmB.gameMaterialUsageCnt = 1;
        shape.enclosurePtr = 0;
        shape.gameMaterialElemID = (i * 7) % 130;
        cobA.objectType = (uint8_t)((i & 1) ? 5 : 2);
        cobA.flags = 0;
        cobA.usageCount = 1;
        cobA.gameMaterial = &gmA;
        cobA.objectPtrByType = &shape;
        cobB = cobA;
        cobB.gameMaterial = &gmB;

        chunk.elemNum = 1;
        chunk.designVal = 0;
        chunk.flags = 0;
        chunk.gameMaterialElemID = (i * 11) % 130;
        chunk.triangleElems = 0;

        ra = NEW_COL_pst_GMat_Get(&cobA, &chunk);
        rb = BASE_COL_pst_GMat_Get(&cobB, &chunk);
        offA = ra ? (long)(ra - elemsA) : -1;
        offB = rb ? (long)(rb - elemsB) : -1;
        check("COL_pst_GMat_Get/chunk", &offA, &offB, sizeof(offA));
        check("COL_pst_GMat_Get/chunk-table", elemsA, elemsB, sizeof(elemsA));

        ra = NEW_COL_pst_GMat_Get(&cobA, 0);
        rb = BASE_COL_pst_GMat_Get(&cobB, 0);
        offA = ra ? (long)(ra - elemsA) : -1;
        offB = rb ? (long)(rb - elemsB) : -1;
        check("COL_pst_GMat_Get/shape", &offA, &offB, sizeof(offA));
        check("COL_pst_GMat_Get/shape-table", elemsA, elemsB, sizeof(elemsA));

        cobA.gameMaterial = 0;
        cobB.gameMaterial = 0;
        ra = NEW_COL_pst_GMat_Get(&cobA, &chunk);
        rb = BASE_COL_pst_GMat_Get(&cobB, &chunk);
        offA = ra ? 1 : 0;
        offB = rb ? 1 : 0;
        check("COL_pst_GMat_Get/nogmat", &offA, &offB, sizeof(offA));
    }

    group("COL_GMat_Skip");
    for (i = 0; i < 4096; i++)
    {
        tCobGameMaterialElems gm;
        tColCollisionObject cob;
        tColInstanceElem elem;
        tColGlobal cg;
        BOOL ra, rb;

        memset(&cg, 0, sizeof(cg));
        gm.flags = ((i & 1) ? 0x200000 : 0) | ((i & 2) ? 0x1000 : 0) | ((i & 4) ? 0x40 : 0);
        gm.slide = 0.0f;
        gm.rebound = 0.0f;
        gm.sound = 0;
        gm.b2 = 0;
        gm.id = 0;
        cob.objectType = 5;
        cob.flags = (uint8_t)((i & 8) ? 2 : 0);
        cob.usageCount = 1;
        cob.gameMaterial = 0;
        cob.objectPtrByType = 0;
        elem.colFlags = (uint8_t)(((i >> 4) & 0x10) | ((i >> 2) & 0x80));
        elem.elemType = 2;
        elem.unk1 = 0;
        elem.design = 0;
        elem.elemShapePtr = 0;
        cg.collInstanceElem = &elem;
        cg.collisionObjectPtr = &cob;

        ra = NEW_COL_GMat_Skip(&gm, &cg);
        rb = BASE_COL_GMat_Skip(&gm, &cg);
        check("COL_GMat_Skip", &ra, &rb, sizeof(ra));

        ra = NEW_COL_GMat_Skip(0, &cg);
        rb = BASE_COL_GMat_Skip(0, &cg);
        check("COL_GMat_Skip/null", &ra, &rb, sizeof(ra));
    }

    group("COL_ComputeProximity");
    for (i = 0; i < 300; i++)
    {
        /* a small indexed mesh in two chunks, so the neighbour index
           has to run across the chunk boundary */
        static tCobMeshTriangleElem triA[2][24];
        static tCobMeshTriangleElem triB[2][24];
        tCobMeshChunkElem chunkA[2];
        tCobMeshChunkElem chunkB[2];
        tCobMeshObjectMesh meshA, meshB;
        tColCollisionObject cobA, cobB;
        int c, t;

        for (c = 0; c < 2; c++)
        {
            int n = 1 + (i + c) % 12;
            for (t = 0; t < n; t++)
            {
                uint16_t a = (uint16_t)((i + t * 2 + c) % 9);
                uint16_t b = (uint16_t)((i + t * 2 + 1 + c) % 9);
                uint16_t cc = (uint16_t)((i + t + 5) % 9);
                triA[c][t].v1 = a; triA[c][t].v2 = b; triA[c][t].v3 = cc;
                triA[c][t].proximity1 = 0; triA[c][t].proximity2 = 0; triA[c][t].proximity3 = 0;
                triB[c][t] = triA[c][t];
            }
            chunkA[c].elemNum = (uint16_t)n;
            chunkA[c].designVal = 0;
            chunkA[c].flags = 0;
            chunkA[c].gameMaterialElemID = 0;
            chunkA[c].triangleElems = triA[c];
            chunkB[c] = chunkA[c];
            chunkB[c].triangleElems = triB[c];
        }
        meshA.vertexCount = 9;
        meshA.faceCount = 0;
        meshA.meshChunkElemNum = 2;
        meshA.vertices = 0;
        meshA.normals = 0;
        meshA.meshChunkElems = chunkA;
        meshA.ok3 = 0;
        meshB = meshA;
        meshB.meshChunkElems = chunkB;

        memset(&cobA, 0, sizeof(cobA));
        cobA.objectType = 5;
        cobA.objectPtrByType = &meshA;
        cobB = cobA;
        cobB.objectPtrByType = &meshB;

        NEW_COL_ComputeProximity(&cobA);
        BASE_COL_ComputeProximity(&cobB);
        check("COL_ComputeProximity", triA, triB, sizeof(triA));
    }

    group("COL_SetFaceTag / COL_b_TestFaceTag");
    for (i = 0; i < 2000; i++)
    {
        int tagsA[8];
        int tagsB[8];
        tCobOK3 okA, okB;
        BOOL ra, rb;
        unsigned int face;

        memset(tagsA, 0, sizeof(tagsA));
        memset(tagsB, 0, sizeof(tagsB));
        okA.num = 0; okA.aaBoxes = 0; okA.faceTagInts = tagsA;
        okB.num = 0; okB.aaBoxes = 0; okB.faceTagInts = tagsB;

        for (face = 0; face < 128u; face++)
        {
            if (((face * 7u + (unsigned int)i) & 3u) == 0u)
            {
                NEW_COL_SetFaceTag(&okA, face);
                BASE_COL_SetFaceTag(&okB, face);
            }
        }
        check("COL_SetFaceTag", tagsA, tagsB, sizeof(tagsA));
        for (face = 0; face < 128u; face++)
        {
            ra = NEW_COL_b_TestFaceTag(&okA, face);
            rb = BASE_COL_b_TestFaceTag(&okB, face);
            check("COL_b_TestFaceTag", &ra, &rb, sizeof(ra));
        }
        okA.faceTagInts = 0;
        okB.faceTagInts = 0;
        ra = NEW_COL_b_TestFaceTag(&okA, 3);
        rb = BASE_COL_b_TestFaceTag(&okB, 3);
        check("COL_b_TestFaceTag/notags", &ra, &rb, sizeof(ra));
        ra = NEW_COL_b_TestFaceTag(0, 3);
        rb = BASE_COL_b_TestFaceTag(0, 3);
        check("COL_b_TestFaceTag/null", &ra, &rb, sizeof(ra));
        NEW_COL_SetFaceTag(0, 3);
        BASE_COL_SetFaceTag(0, 3);
    }

    group("COL_i_ZoneActivationGet");
    for (i = 0; i < 4000; i++)
    {
        uint8_t zones[16];
        tColInstSet set;
        tColInstance inst;
        int ra, rb;
        int z;

        for (z = 0; z < 16; z++)
            zones[z] = (uint8_t)((i + z * 5) % 20);
        memset(&set, 0, sizeof(set));
        memset(&inst, 0, sizeof(inst));
        set.zoneIDs = zones;
        set.instElemNum = 16;
        inst.colSet = &set;
        inst.activationFlags = (uint16_t)(i * 37);

        ra = NEW_COL_i_ZoneActivationGet(&inst, (char)(i % 24));
        rb = BASE_COL_i_ZoneActivationGet(&inst, (char)(i % 24));
        check("COL_i_ZoneActivationGet", &ra, &rb, sizeof(ra));
    }

    group("COL_w_TestInstanceFlag");
    for (i = 0; i < 2000; i++)
    {
        tColInstance inst;
        int16_t ra, rb;
        memset(&inst, 0, sizeof(inst));
        inst.flag = (int16_t)(i * 13);
        ra = NEW_COL_w_TestInstanceFlag(&inst, (int16_t)i);
        rb = BASE_COL_w_TestInstanceFlag(&inst, (int16_t)i);
        check("COL_w_TestInstanceFlag", &ra, &rb, sizeof(ra));
        ra = NEW_COL_w_TestInstanceFlag(0, (int16_t)i);
        rb = BASE_COL_w_TestInstanceFlag(0, (int16_t)i);
        check("COL_w_TestInstanceFlag/null", &ra, &rb, sizeof(ra));
    }

    group("COL_NeedRecomputation");
    for (i = 0; i < 3000; i++)
    {
        tGameObjectMain gaos[8];
        tGameObjectExtended exts[8];
        tGameObjectCollMain colls[8];
        tColInstance insts[8];
        tColGlobal cgA, cgB;
        BOOL ra, rb;
        int g, which;
        struct { BOOL r; uint8_t num; int16_t flag; } A, B;

        memset(gaos, 0, sizeof(gaos));
        memset(exts, 0, sizeof(exts));
        memset(colls, 0, sizeof(colls));
        memset(insts, 0, sizeof(insts));
        for (g = 0; g < 8; g++)
        {
            insts[g].flag = (int16_t)((i + g) % 7 == 0 ? -1 : (i + g) & 0x7f);
            colls[g].colInstance = &insts[g];
            exts[g].coll = &colls[g];
            gaos[g].extended = &exts[g];
            gaos[g].flags2 = ((i + g) & 3) ? 0x20 : 0;
        }

        memset(&cgA, 0, sizeof(cgA));
        cgA.gaosNeededToBeRecomputedNum = (uint8_t)(i % 52);
        for (g = 0; g < cgA.gaosNeededToBeRecomputedNum && g < 50; g++)
            cgA.gaosNeededToBeRecomputed[g] = &gaos[(i + g) % 8];
        cgB = cgA;

        which = i % 8;
        memset(&A, 0, sizeof(A));
        memset(&B, 0, sizeof(B));
        ra = NEW_COL_NeedRecomputation(&cgA, &gaos[which], (int16_t)(1 << (i % 15)));
        A.r = ra;
        A.num = cgA.gaosNeededToBeRecomputedNum;
        A.flag = insts[which].flag;

        /* the two copies share the fixture, so it is rebuilt between
           them rather than left carrying the first run's writes */
        for (g = 0; g < 8; g++)
            insts[g].flag = (int16_t)((i + g) % 7 == 0 ? -1 : (i + g) & 0x7f);

        rb = BASE_COL_NeedRecomputation(&cgB, &gaos[which], (int16_t)(1 << (i % 15)));
        B.r = rb;
        B.num = cgB.gaosNeededToBeRecomputedNum;
        B.flag = insts[which].flag;
        check("COL_NeedRecomputation", &A, &B, sizeof(A));
        check("COL_NeedRecomputation/list", cgA.gaosNeededToBeRecomputed,
              cgB.gaosNeededToBeRecomputed, sizeof(cgA.gaosNeededToBeRecomputed));
    }
}

/* ---- 8. the bounding-volume ray tests ------------------------------ */

static void runBVRays(void)
{
    int i;
    group("COL_RaySphereBV / COL_RayAABBox");
    for (i = 0; i < 20000; i++)
    {
        tGameObjectBV bv;
        Mtx4x4 mtx;
        tGameObjectMain gao;
        Vec3 o, d;
        float da, db, maxDist;
        int ra, rb;
        struct { int r; float d; } A, B;

        memset(&bv, 0, sizeof(bv));
        memset(&mtx, 0, sizeof(mtx));
        memset(&gao, 0, sizeof(gao));
        randv(&bv.BVMin, 4.0f);
        randv(&bv.BVMax, 4.0f);
        if (i & 1)
            *(uint32_t*)&bv.BVMax.z = 0xffffffffu;    /* a sphere BV */
        randv(&mtx.Pos, 3.0f);
        mtx.mtxflag = 0;
        gao.boundingVolume = &bv;
        gao.matrices = &mtx;

        randv(&o, 8.0f);
        randUnit(&d);
        if ((i & 7) == 0) setv(&d, 0.0f, 0.0f, 0.0f);
        maxDist = randf(0.0f, 20.0f);

        da = -12345.0f; db = -12345.0f;
        ra = NEW_COL_RaySphereBV(&o, &d, &gao, &da, maxDist);
        rb = BASE_COL_RaySphereBV(&o, &d, &gao, &db, maxDist);
        A.r = ra; A.d = da;
        B.r = rb; B.d = db;
        check("COL_RaySphereBV", &A, &B, sizeof(A));

        da = -12345.0f; db = -12345.0f;
        ra = NEW_COL_RayAABBox(&o, &d, &gao, &da, maxDist);
        rb = BASE_COL_RayAABBox(&o, &d, &gao, &db, maxDist);
        A.r = ra; A.d = da;
        B.r = rb; B.d = db;
        check("COL_RayAABBox", &A, &B, sizeof(A));
    }
}

/* ---- 9. the topologic-edge test ------------------------------------ */

static void runTopologicEdge(void)
{
    int i;
    group("COL_b_IsARealTopologicEdge");
    for (i = 0; i < 20000; i++)
    {
        static Vec3 normals[16];
        tCobMeshObjectMesh mesh;
        tColCollisionObject cob;
        tCobMeshTriangleElem tri;
        Mtx4x4 instMtx, mapMtx;
        tColGlobal cgA, cgB;
        Vec3 gravityNeg, faceNormal_;
        BOOL ra, rb;
        int k;
        struct { BOOL r; Vec3 e; } A, B;

        for (k = 0; k < 16; k++)
            randUnit(&normals[k]);
        memset(&mesh, 0, sizeof(mesh));
        mesh.normals = normals;
        memset(&cob, 0, sizeof(cob));
        cob.objectType = 5;
        cob.objectPtrByType = &mesh;

        tri.v1 = 0; tri.v2 = 1; tri.v3 = 2;
        tri.proximity1 = (uint16_t)((i & 7) == 0 ? 0xFFFF : (i % 16));
        tri.proximity2 = (uint16_t)((i & 15) == 1 ? 0xFFFF : ((i + 3) % 16));
        tri.proximity3 = (uint16_t)((i & 31) == 2 ? 0xFFFF : ((i + 7) % 16));

        memset(&instMtx, 0, sizeof(instMtx));
        memset(&mapMtx, 0, sizeof(mapMtx));
        instMtx.v1.x = 1.0f; instMtx.v2.y = 1.0f;
        instMtx.v3.z = (i & 1) ? 1.0f : 0.5f;         /* upright or not */
        mapMtx.v1.x = 1.0f; mapMtx.v2.y = 1.0f; mapMtx.v3.z = 1.0f;
        randv(&mapMtx.v1, 1.0f);
        randv(&mapMtx.v2, 1.0f);
        randv(&mapMtx.v3, 1.0f);
        randv(&mapMtx.Pos, 2.0f);
        mapMtx.mtxflag = 0;

        randUnit(&gravityNeg);
        randUnit(&faceNormal_);

        memset(&cgA, 0, sizeof(cgA));
        cgA.collisionObjectPtr = &cob;
        cgA.cobMeshTriangleElem = &tri;
        cgA.colInstGaoMatrices = &instMtx;
        cgA.colMapGaoMatrices = &mapMtx;
        cgA.vec6InZDMZDRGravityNeg = gravityNeg;
        cgA.wallCosAngle = randf(-1.0f, 1.0f);
        cgB = cgA;
        cgA.ptrSetToVec6InZDMZDRGravityNeg = &cgA.vec6InZDMZDRGravityNeg;
        cgB.ptrSetToVec6InZDMZDRGravityNeg = &cgB.vec6InZDMZDRGravityNeg;

        ra = NEW_COL_b_IsARealTopologicEdge(&cgA, &faceNormal_, (uint8_t)(i & 7));
        rb = BASE_COL_b_IsARealTopologicEdge(&cgB, &faceNormal_, (uint8_t)(i & 7));
        A.r = ra; A.e = cgA.colMapEdgeNormalVec;
        B.r = rb; B.e = cgB.colMapEdgeNormalVec;
        check("COL_b_IsARealTopologicEdge", &A, &B, sizeof(A));
    }
}

/* ---- 10. non-finite torture --------------------------------------
 *
 * Several relations were rewritten the other way round (an `if`/`else`
 * turned into an early return), which is equivalent for every ordered
 * pair but not for a NaN operand. Rather than argue those sites are
 * unreachable, this group feeds NaN and both infinities straight in.
 */

static const char* kNonFiniteNames[5] = {
    "non-finite INT_FullRayTriangle", "non-finite INT_FullRayAABBox",
    "non-finite INT_FullRaySphere", "non-finite INT_b_FullRayCylinder",
    "non-finite INT_FullSegmentSphere"
};

static const float kWeird[] = { 0.0f, -0.0f, 1.0f, -1.0f, 1e30f, -1e30f, 1e-38f, -1e-38f };
#define NWEIRD ((int)(sizeof(kWeird) / sizeof(kWeird[0])))

static void runNonFinite(void)
{
    float nan = (float)(0.0 / 0.0);
    float inf = (float)(1.0 / 0.0);
    float pool[11];
    int np;
    int i, j, k, m;

    group("non-finite inputs");

    pool[0] = nan; pool[1] = inf; pool[2] = -inf;
    for (i = 0; i < NWEIRD; i++) pool[3 + i] = kWeird[i];
    np = 11;

    for (i = 0; i < np; i++)
    {
        for (j = 0; j < np; j++)
        {
            for (k = 0; k < np; k++)
            {
                Vec3 o, d, p1, p2, p3, ha, hb, axis, c, lo, hi, toPt;
                tColBox box;
                tColSphere sph;
                float da, db;
                struct { int r; Vec3 h; float d; } A, B;

                setv(&o, pool[i], pool[j], pool[k]);
                setv(&d, pool[j], pool[k], pool[i]);
                setv(&p1, pool[k], pool[i], pool[j]);
                setv(&p2, pool[(i + 1) % np], pool[j], pool[k]);
                setv(&p3, pool[i], pool[(j + 1) % np], pool[k]);
                setv(&lo, pool[i], pool[j], pool[k]);
                setv(&hi, pool[(i + 2) % np], pool[(j + 2) % np], pool[(k + 2) % np]);
                box.pos1 = lo;
                box.pos2 = hi;
                sph.center = p1;
                sph.radius = pool[j];
                setv(&axis, 0.0f, 0.0f, 1.0f);
                c = p2;

                for (m = 0; m < 5; m++)
                {
                    float maxDist = pool[(i + j + m) % np];
                    memset(&A, 0, sizeof(A));
                    memset(&B, 0, sizeof(B));
                    memset(&ha, 0xA5, sizeof(ha));
                    memset(&hb, 0xA5, sizeof(hb));
                    da = -12345.0f; db = -12345.0f;
                    switch (m)
                    {
                    case 0:
                        A.r = NEW_INT_FullRayTriangle(&o, &d, &p1, &p2, &p3, &da, &ha);
                        B.r = BASE_INT_FullRayTriangle(&o, &d, &p1, &p2, &p3, &db, &hb);
                        break;
                    case 1:
                        A.r = NEW_INT_FullRayAABBox(&o, &d, &box, &ha, maxDist, &da);
                        B.r = BASE_INT_FullRayAABBox(&o, &d, &box, &hb, maxDist, &db);
                        break;
                    case 2:
                        A.r = NEW_INT_FullRaySphere(&o, &d, &sph, &ha, maxDist, &da);
                        B.r = BASE_INT_FullRaySphere(&o, &d, &sph, &hb, maxDist, &db);
                        break;
                    case 3:
                        A.r = NEW_INT_b_FullRayCylinder(&o, &d, &c, &axis, pool[i], pool[k], maxDist, &da, &ha);
                        B.r = BASE_INT_b_FullRayCylinder(&o, &d, &c, &axis, pool[i], pool[k], maxDist, &db, &hb);
                        break;
                    default:
                        A.r = NEW_INT_FullSegmentSphere(&o, &d, &p1, pool[j], &ha);
                        B.r = BASE_INT_FullSegmentSphere(&o, &d, &p1, pool[j], &hb);
                        break;
                    }
                    A.h = ha; A.d = da;
                    B.h = hb; B.d = db;
                    check(kNonFiniteNames[m], &A, &B, sizeof(A));
                }

                /* the closest-point helpers, where the zero-length line
                   really does make a NaN out of finite inputs */
                setv(&toPt, pool[i] - pool[j], pool[j] - pool[k], pool[k] - pool[i]);
                memset(&ha, 0xA5, sizeof(ha));
                memset(&hb, 0xA5, sizeof(hb));
                NEW_INT_PointLineClosestPoint(&o, &p1, &p2, &toPt, &ha);
                BASE_INT_PointLineClosestPoint(&o, &p1, &p2, &toPt, &hb);
                check("non-finite closest point", &ha, &hb, sizeof(ha));

                {
                    uint8_t ea = 0xAA, eb = 0xAA;
                    int ra, rb;
                    struct { int r; uint8_t e; } E, F;
                    ra = NEW_INT_PointInTriangle(&o, &p1, &p2, &p3, &d, &ea);
                    rb = BASE_INT_PointInTriangle(&o, &p1, &p2, &p3, &d, &eb);
                    memset(&E, 0, sizeof(E));
                    memset(&F, 0, sizeof(F));
                    E.r = ra; E.e = ea;
                    F.r = rb; F.e = eb;
                    check("non-finite point in triangle", &E, &F, sizeof(E));

                    ra = NEW_INT_ul_GetPositionPointWithBox(&o, &lo, &hi);
                    rb = BASE_INT_ul_GetPositionPointWithBox(&o, &lo, &hi);
                    check("non-finite outcode", &ra, &rb, sizeof(ra));
                }
            }
        }
    }
}

int main(void)
{
    printf("collision.c range A: frozen copy against the rewrite\n");
    COLA_MockResetArena();

    runPointInTriangle();
    runSpheres();
    runSegments();
    runRays();
    runClosest();
    runOutcodes();
    runTables();
    runBVRays();
    runTopologicEdge();
    runNonFinite();
    endGroups();

    printf("\n%ld cases, %ld differing\n", gCases, gDiffs);
    if (gDiffs)
    {
        printf("FAIL\n");
        return 1;
    }
    printf("OK\n");
    return 0;
}
