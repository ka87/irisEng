/* collision.c functions 64-97 (the middle third), before the rewrite
 * against after.
 *
 * What is covered here is the geometry: the five INT_ intersection
 * routines in the range and COL_ComputeRejection, which are pure over
 * plain structs, plus the three COL_ZDM_* wrappers, which are reachable
 * with a hand-built collision object and a one-record world.
 *
 * What is NOT covered is the pipeline -- COL_MainCall, COL_Compute,
 * COL_OneCall, COL_PreComputeReports, COL_ComputeReports, the REC_
 * response and the mesh walk. Standing one of those up means standing
 * up a world with its sector tables, a game-object graph with matrices,
 * bounding volumes, dynamics and animation, a loaded collision set with
 * its OK3 acceleration tree, and the sweep-and-prune list. Every value
 * those functions compute would then come out of the fixture rather
 * than out of the engine, so the test would be measuring the mocks.
 *
 * Every comparison below is BITWISE on the raw bytes of the results and
 * on the ordered transcript of engine calls each side made. The rewrite
 * is renames, goto removal and seven extracted helpers, so anything but
 * bit-identical is a regression.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "../gameobject.h"
#include "../world.h"
#include "../view.h"
#include "../math.h"
#include "../graphicsobjects/geo.h"
#include "../modifiers.h"
#include "../modifiers/otcmodifier.h"
#include "../profiler.h"
#include "../collision.h"
#include "collision_b_mocks.h"

/* ---- the two copies ---------------------------------------------- */

#define D(ret, name, args) ret NEW_##name args; ret BASE_##name args;

D(BOOL, INT_SphereAABBox, (Vec3*, float, Vec3*, Vec3*))
D(BOOL, INT_SphereOBBox, (Vec3*, float, Vec3*, Vec3*, Vec3*))
D(int, INT_FullSphereOBBox, (Vec3*, float, Vec3*, Vec3*, Vec3*, Vec3*, Vec3*, Vec3*, int*))
D(int, INT_FullSphereCylinder, (Vec3*, float, Vec3*, float, float, Vec3*, Vec3*, Vec3*, Vec3*, int*))
D(int, INT_FullAABBoxTriangle, (Vec3*, Vec3*, Vec3*, Vec3*, Vec3*, Vec3*, Vec3*, uint32_t*, uint32_t*))
D(void, COL_ComputeRejection, (tColGlobal*, Vec3*, Vec3*, int))
D(int, COL_ZDM_Box, (tColGlobal*))
D(int, COL_ZDM_Sphere, (tColGlobal*))
D(int, COL_ZDM_Cylinder, (tColGlobal*))
D(void, COL_UpdateCobFromVisualRealTime, (tGameObjectMain*, tColCollisionObject*, tGEOGRO*))

#undef D

/* ---- harness ------------------------------------------------------ */

static long gCases;
static long gTrue;      /* cases where the routine reported a hit */
static long gCalls;     /* cases where the routine called out at all */
static long gDiffResult;
static long gDiffCalls;
static const char* gWhere;
static int gReported;

#define SIDE_BASE 0
#define SIDE_NEW  1

typedef struct
{
    unsigned char bytes[512];
    int len;
    char calls[COLB_LOG_MAX][64];
    int callLen;
    int overflow;
} Capture;

static void CaptureCalls(Capture* c)
{
    int i;
    c->callLen = COLB_logLen;
    c->overflow = COLB_logOverflow;
    for (i = 0; i < COLB_logLen; i++)
        memcpy(c->calls[i], COLB_log[i], sizeof(c->calls[0]));
}

static void Compare(const char* what, const Capture* a, const Capture* b, const char* detail)
{
    int i;
    gCases++;
    if (a->bytes[0] || a->bytes[1] || a->bytes[2] || a->bytes[3]) gTrue++;
    if (a->callLen) gCalls++;
    if (a->len != b->len || memcmp(a->bytes, b->bytes, (size_t)a->len) != 0)
    {
        gDiffResult++;
        if (gReported < 12)
        {
            printf("  DIFF result  %s  %s\n", what, detail);
            printf("    base:");
            for (i = 0; i < a->len; i++) printf(" %02x", a->bytes[i]);
            printf("\n    new :");
            for (i = 0; i < b->len; i++) printf(" %02x", b->bytes[i]);
            printf("\n");
            gReported++;
        }
        return;
    }
    if (a->callLen != b->callLen || a->overflow != b->overflow)
    {
        gDiffCalls++;
        if (gReported < 12)
        {
            printf("  DIFF calls   %s  %s  base %d new %d\n",
                   what, detail, a->callLen, b->callLen);
            gReported++;
        }
        return;
    }
    for (i = 0; i < a->callLen; i++)
    {
        if (strcmp(a->calls[i], b->calls[i]) != 0)
        {
            gDiffCalls++;
            if (gReported < 12)
            {
                printf("  DIFF calls   %s  %s  #%d base '%s' new '%s'\n",
                       what, detail, i, a->calls[i], b->calls[i]);
                gReported++;
            }
            return;
        }
    }
}

/* A recognisable seed so an output the callee never writes still shows
   up as equal on both sides rather than as uninitialised stack. */
static void Poison(void* p, size_t n)
{
    memset(p, 0x5A, n);
}

/* ---- value sets --------------------------------------------------- */

static const float gCoord[] = {
    -4.0f, -2.5f, -1.0f, -0.5f, -0.0f, 0.0f, 0.5f, 1.0f, 2.5f, 4.0f
};
#define NCOORD ((int)(sizeof(gCoord) / sizeof(gCoord[0])))

static const float gRadius[] = { 0.0f, 0.25f, 0.5f, 1.0f, 1.5f, 3.0f, 8.0f };
#define NRADIUS ((int)(sizeof(gRadius) / sizeof(gRadius[0])))

typedef struct { Vec3 lo, hi; } BoxPair;

static const BoxPair gBoxes[] = {
    { { -1.0f, -1.0f, -1.0f }, {  1.0f,  1.0f,  1.0f } },   /* unit cube   */
    { {  0.0f,  0.0f,  0.0f }, {  2.0f,  0.5f,  3.0f } },   /* off-origin  */
    { { -1.0f, -1.0f, -1.0f }, { -1.0f, -1.0f, -1.0f } },   /* degenerate  */
    { {  1.0f,  1.0f,  1.0f }, { -1.0f, -1.0f, -1.0f } },   /* inverted    */
    { { -1.0f, -1.0f, -0.0f }, {  1.0f,  1.0f,  0.0f } }    /* flat at z=0 */
};
#define NBOXES ((int)(sizeof(gBoxes) / sizeof(gBoxes[0])))

static const Vec3 gDirs[] = {
    {  0.0f,  0.0f,  0.0f },
    {  1.0f,  0.0f,  0.0f },
    {  0.0f,  1.0f,  0.0f },
    {  0.0f,  0.0f,  1.0f },
    {  0.57735026f, 0.57735026f, 0.57735026f },
    { -0.70710677f, 0.70710677f, 0.0f },
    {  0.0f, -1.0f,  0.0f }
};
#define NDIRS ((int)(sizeof(gDirs) / sizeof(gDirs[0])))

/* ---- 1. INT_SphereAABBox ------------------------------------------ */

static void Run_SphereAABBox(void)
{
    Capture cap[2];
    int b, r, ix, iy, iz, side;
    char detail[128];
    Vec3 center;
    Vec3 lo, hi;
    BOOL rc;

    gWhere = "INT_SphereAABBox";
    for (b = 0; b < NBOXES; b++)
    for (r = 0; r < NRADIUS; r++)
    for (ix = 0; ix < NCOORD; ix++)
    for (iy = 0; iy < NCOORD; iy++)
    for (iz = 0; iz < NCOORD; iz++)
    {
        for (side = 0; side < 2; side++)
        {
            center.x = gCoord[ix]; center.y = gCoord[iy]; center.z = gCoord[iz];
            lo = gBoxes[b].lo; hi = gBoxes[b].hi;
            COLB_LogReset();
            rc = side ? NEW_INT_SphereAABBox(&center, gRadius[r], &lo, &hi)
                      : BASE_INT_SphereAABBox(&center, gRadius[r], &lo, &hi);
            Poison(cap[side].bytes, sizeof(cap[side].bytes));
            memcpy(cap[side].bytes, &rc, sizeof(rc));
            memcpy(cap[side].bytes + sizeof(rc), &center, sizeof(center));
            cap[side].len = (int)(sizeof(rc) + sizeof(center));
            CaptureCalls(&cap[side]);
        }
        sprintf(detail, "box %d r %g c %g,%g,%g", b, (double)gRadius[r],
                (double)gCoord[ix], (double)gCoord[iy], (double)gCoord[iz]);
        Compare(gWhere, &cap[0], &cap[1], detail);
    }
}

/* ---- 2. INT_SphereOBBox ------------------------------------------- */

static void Run_SphereOBBox(void)
{
    Capture cap[2];
    int b, r, ix, iy, iz, side, wantOut;
    char detail[128];
    Vec3 center, lo, hi, closest;
    BOOL rc;

    gWhere = "INT_SphereOBBox";
    for (b = 0; b < NBOXES; b++)
    for (r = 0; r < NRADIUS; r++)
    for (wantOut = 0; wantOut < 2; wantOut++)
    for (ix = 0; ix < NCOORD; ix++)
    for (iy = 0; iy < NCOORD; iy++)
    for (iz = 0; iz < NCOORD; iz++)
    {
        for (side = 0; side < 2; side++)
        {
            center.x = gCoord[ix]; center.y = gCoord[iy]; center.z = gCoord[iz];
            lo = gBoxes[b].lo; hi = gBoxes[b].hi;
            Poison(&closest, sizeof(closest));
            COLB_LogReset();
            rc = side ? NEW_INT_SphereOBBox(&center, gRadius[r], &lo, &hi, wantOut ? &closest : NULL)
                      : BASE_INT_SphereOBBox(&center, gRadius[r], &lo, &hi, wantOut ? &closest : NULL);
            Poison(cap[side].bytes, sizeof(cap[side].bytes));
            memcpy(cap[side].bytes, &rc, sizeof(rc));
            memcpy(cap[side].bytes + sizeof(rc), &closest, sizeof(closest));
            cap[side].len = (int)(sizeof(rc) + sizeof(closest));
            CaptureCalls(&cap[side]);
        }
        sprintf(detail, "box %d r %g out %d c %g,%g,%g", b, (double)gRadius[r], wantOut,
                (double)gCoord[ix], (double)gCoord[iy], (double)gCoord[iz]);
        Compare(gWhere, &cap[0], &cap[1], detail);
    }
}

/* ---- 3. INT_FullSphereOBBox --------------------------------------- */

static void Run_FullSphereOBBox(void)
{
    Capture cap[2];
    int b, r, d, ix, iy, iz, side, wantFlags;
    char detail[160];
    Vec3 center, lo, hi, rayOrigin, rayDir, outN, outHit;
    int outFlags;
    int rc;

    gWhere = "INT_FullSphereOBBox";
    for (b = 0; b < NBOXES; b++)
    for (r = 0; r < NRADIUS; r++)
    for (d = 0; d < NDIRS; d++)
    for (wantFlags = 0; wantFlags < 2; wantFlags++)
    for (ix = 0; ix < NCOORD; ix += 2)
    for (iy = 0; iy < NCOORD; iy += 2)
    for (iz = 0; iz < NCOORD; iz += 2)
    {
        for (side = 0; side < 2; side++)
        {
            center.x = gCoord[ix]; center.y = gCoord[iy]; center.z = gCoord[iz];
            lo = gBoxes[b].lo; hi = gBoxes[b].hi;
            rayOrigin.x = gCoord[(ix + 3) % NCOORD];
            rayOrigin.y = gCoord[(iy + 5) % NCOORD];
            rayOrigin.z = gCoord[(iz + 7) % NCOORD];
            rayDir = gDirs[d];
            Poison(&outN, sizeof(outN));
            Poison(&outHit, sizeof(outHit));
            outFlags = 0x12345678;
            COLB_LogReset();
            rc = side
                ? NEW_INT_FullSphereOBBox(&center, gRadius[r], &lo, &hi, &rayOrigin,
                                          &rayDir, &outN, &outHit, wantFlags ? &outFlags : NULL)
                : BASE_INT_FullSphereOBBox(&center, gRadius[r], &lo, &hi, &rayOrigin,
                                           &rayDir, &outN, &outHit, wantFlags ? &outFlags : NULL);
            Poison(cap[side].bytes, sizeof(cap[side].bytes));
            memcpy(cap[side].bytes + 0, &rc, 4);
            memcpy(cap[side].bytes + 4, &outN, 12);
            memcpy(cap[side].bytes + 16, &outHit, 12);
            memcpy(cap[side].bytes + 28, &outFlags, 4);
            cap[side].len = 32;
            CaptureCalls(&cap[side]);
        }
        sprintf(detail, "box %d r %g dir %d flags %d c %g,%g,%g", b, (double)gRadius[r], d,
                wantFlags, (double)gCoord[ix], (double)gCoord[iy], (double)gCoord[iz]);
        Compare(gWhere, &cap[0], &cap[1], detail);
    }
}

/* ---- 4. INT_FullSphereCylinder ------------------------------------ */

static void Run_FullSphereCylinder(void)
{
    Capture cap[2];
    static const float cylR[] = { 0.0f, 0.5f, 2.0f };
    static const float cylH[] = { 0.0f, 1.0f, 4.0f };
    int cr, ch, r, d, ix, iy, iz, side;
    char detail[160];
    Vec3 center, cylCenter, rayOrigin, rayDir, outN, outHit;
    int outFlags;
    int rc;

    gWhere = "INT_FullSphereCylinder";
    for (cr = 0; cr < 3; cr++)
    for (ch = 0; ch < 3; ch++)
    for (r = 0; r < NRADIUS; r++)
    for (d = 0; d < NDIRS; d++)
    for (ix = 0; ix < NCOORD; ix += 2)
    for (iy = 0; iy < NCOORD; iy += 2)
    for (iz = 0; iz < NCOORD; iz += 2)
    {
        for (side = 0; side < 2; side++)
        {
            center.x = gCoord[ix]; center.y = gCoord[iy]; center.z = gCoord[iz];
            /* every fourth case puts the sphere centre exactly on the
               cylinder axis, which is the only way into the degenerate
               "radial offset is the null vector" arm */
            if (((ix + iy + iz) & 3) == 0)
            {
                cylCenter.x = center.x;
                cylCenter.y = center.y;
                cylCenter.z = gCoord[(iz + 2 * (d & 1)) % NCOORD];
            }
            else
            {
                cylCenter.x = 0.0f;
                cylCenter.y = gCoord[(iy + 4) % NCOORD];
                cylCenter.z = gCoord[(iz + 2) % NCOORD];
            }
            rayOrigin.x = gCoord[(ix + 3) % NCOORD];
            rayOrigin.y = gCoord[(iy + 5) % NCOORD];
            rayOrigin.z = gCoord[(iz + 7) % NCOORD];
            rayDir = gDirs[d];
            Poison(&outN, sizeof(outN));
            Poison(&outHit, sizeof(outHit));
            outFlags = 0x12345678;
            COLB_LogReset();
            rc = side
                ? NEW_INT_FullSphereCylinder(&center, gRadius[r], &cylCenter, cylR[cr], cylH[ch],
                                             &rayOrigin, &rayDir, &outN, &outHit, &outFlags)
                : BASE_INT_FullSphereCylinder(&center, gRadius[r], &cylCenter, cylR[cr], cylH[ch],
                                              &rayOrigin, &rayDir, &outN, &outHit, &outFlags);
            Poison(cap[side].bytes, sizeof(cap[side].bytes));
            memcpy(cap[side].bytes + 0, &rc, 4);
            memcpy(cap[side].bytes + 4, &outN, 12);
            memcpy(cap[side].bytes + 16, &outHit, 12);
            memcpy(cap[side].bytes + 28, &outFlags, 4);
            cap[side].len = 32;
            CaptureCalls(&cap[side]);
        }
        sprintf(detail, "cylR %g cylH %g r %g dir %d c %g,%g,%g",
                (double)cylR[cr], (double)cylH[ch], (double)gRadius[r], d,
                (double)gCoord[ix], (double)gCoord[iy], (double)gCoord[iz]);
        Compare(gWhere, &cap[0], &cap[1], detail);
    }
}

/* ---- 5. INT_FullAABBoxTriangle ------------------------------------ */

/* Vertices chosen so the sweep hits every degenerate case the routine
   can see: a face, an edge and a corner of the box exactly, a triangle
   wholly inside, wholly outside and straddling, and triangles with two
   or three coincident vertices. */
static const Vec3 gTriVerts[] = {
    {  0.0f,  0.0f,  0.0f },   /* box centre            */
    {  1.0f,  0.0f,  0.0f },   /* face centre           */
    {  1.0f,  1.0f,  0.0f },   /* edge midpoint         */
    {  1.0f,  1.0f,  1.0f },   /* corner                */
    { -1.0f, -1.0f, -1.0f },   /* opposite corner       */
    {  3.0f,  0.0f,  0.0f },   /* outside on one axis   */
    { -3.0f,  2.0f,  0.5f },   /* outside, oblique      */
    {  0.5f, -0.5f,  0.25f },  /* inside, off-grid      */
    {  0.0f,  0.0f,  1.0f },   /* on the top face       */
    {  2.0f,  2.0f, -2.0f }    /* outside, far corner   */
};
#define NTRI ((int)(sizeof(gTriVerts) / sizeof(gTriVerts[0])))

static const Vec3 gTriNormals[] = {
    {  0.0f,  0.0f,  1.0f },
    {  0.0f,  0.0f, -1.0f },
    {  1.0f,  0.0f,  0.0f },
    {  0.57735026f, 0.57735026f, 0.57735026f },
    {  0.0f,  0.0f,  0.0f }     /* degenerate normal     */
};
#define NTRINORM ((int)(sizeof(gTriNormals) / sizeof(gTriNormals[0])))

static void Run_FullAABBoxTriangle(void)
{
    Capture cap[2];
    int b, n, i, j, k, side, wantFlags;
    char detail[192];
    Vec3 p0, p1, p2, normal, lo, hi;
    Vec3 hits[8];
    uint32_t hitCount;
    uint32_t flags;
    int rc;

    gWhere = "INT_FullAABBoxTriangle";
    for (b = 0; b < NBOXES; b++)
    for (n = 0; n < NTRINORM; n++)
    for (wantFlags = 0; wantFlags < 2; wantFlags++)
    for (i = 0; i < NTRI; i++)
    for (j = 0; j < NTRI; j++)
    for (k = 0; k < NTRI; k++)
    {
        for (side = 0; side < 2; side++)
        {
            p0 = gTriVerts[i]; p1 = gTriVerts[j]; p2 = gTriVerts[k];
            normal = gTriNormals[n];
            lo = gBoxes[b].lo; hi = gBoxes[b].hi;
            Poison(hits, sizeof(hits));
            hitCount = 0xDEADBEEFu;
            flags = 0x4000u;
            COLB_LogReset();
            rc = side
                ? NEW_INT_FullAABBoxTriangle(&p0, &p1, &p2, &normal, &lo, &hi, hits,
                                             &hitCount, wantFlags ? &flags : NULL)
                : BASE_INT_FullAABBoxTriangle(&p0, &p1, &p2, &normal, &lo, &hi, hits,
                                              &hitCount, wantFlags ? &flags : NULL);
            Poison(cap[side].bytes, sizeof(cap[side].bytes));
            memcpy(cap[side].bytes + 0, &rc, 4);
            memcpy(cap[side].bytes + 4, &hitCount, 4);
            memcpy(cap[side].bytes + 8, &flags, 4);
            memcpy(cap[side].bytes + 12, hits, sizeof(hits));
            memcpy(cap[side].bytes + 12 + sizeof(hits), &p0, 12);
            memcpy(cap[side].bytes + 24 + sizeof(hits), &normal, 12);
            cap[side].len = (int)(36 + sizeof(hits));
            CaptureCalls(&cap[side]);
        }
        sprintf(detail, "box %d n %d flags %d tri %d,%d,%d", b, n, wantFlags, i, j, k);
        Compare(gWhere, &cap[0], &cap[1], detail);
    }
}

/* ---- 6. COL_ComputeRejection -------------------------------------- */

static void Run_ComputeRejection(void)
{
    Capture cap[2];
    static tColGlobal cg;
    int r, ix, iy, iz, useDyn, side;
    char detail[160];
    Vec3 lo, hi;

    gWhere = "COL_ComputeRejection";
    for (useDyn = 0; useDyn < 2; useDyn++)
    for (r = 0; r < NRADIUS; r++)
    for (ix = 0; ix < NCOORD; ix++)
    for (iy = 0; iy < NCOORD; iy++)
    for (iz = 0; iz < NCOORD; iz++)
    {
        for (side = 0; side < 2; side++)
        {
            memset(&cg, 0, sizeof(cg));
            cg.radius = gRadius[r];
            cg.dynRadius = gRadius[(r + 3) % NRADIUS];
            cg.centerPos.x = gCoord[ix];
            cg.centerPos.y = gCoord[iy];
            cg.centerPos.z = gCoord[iz];
            cg.dynCenterPos.x = gCoord[(ix + 1) % NCOORD];
            cg.dynCenterPos.y = gCoord[(iy + 2) % NCOORD];
            cg.dynCenterPos.z = gCoord[(iz + 3) % NCOORD];
            Poison(&lo, sizeof(lo));
            Poison(&hi, sizeof(hi));
            COLB_LogReset();
            if (side) NEW_COL_ComputeRejection(&cg, &lo, &hi, useDyn);
            else      BASE_COL_ComputeRejection(&cg, &lo, &hi, useDyn);
            Poison(cap[side].bytes, sizeof(cap[side].bytes));
            memcpy(cap[side].bytes + 0, &lo, 12);
            memcpy(cap[side].bytes + 12, &hi, 12);
            cap[side].len = 24;
            CaptureCalls(&cap[side]);
        }
        sprintf(detail, "dyn %d r %g c %g,%g,%g", useDyn, (double)gRadius[r],
                (double)gCoord[ix], (double)gCoord[iy], (double)gCoord[iz]);
        Compare(gWhere, &cap[0], &cap[1], detail);
    }
}

/* ---- 7. COL_ZDM_Box / Sphere / Cylinder ---------------------------- */

/* The smallest world the three zone-against-zone wrappers will run in:
   one collision object holding one enclosure, one instance element, a
   game object on each side with a matrix, and a record array for the
   report to land in. Everything the wrappers reach is in this fixture;
   nothing is read back out of the engine. */

typedef struct
{
    tColGlobal cg;
    tWorld world;
    tColCollisionObject colObj;
    tCobNormalObjectGeneric cob;
    tCobGameMaterial gmat;
    tCobGameMaterialElems gmatElems[2];
    int gmatIds[2];
    tColInstanceElem instElem;
    tColInstance colInstance;
    tColInstSet instSet;
    uint8_t zoneIds[16];
    tGameObjectMain instGao;
    tGameObjectMain mapGao;
    tGameObjectExtended instExt;
    tGameObjectExtended mapExt;
    tGameObjectCollMain instColl;
    tGameObjectCollMain mapColl;
    Mtx4x4 instMtx;
    Mtx4x4 mapMtx;
    tColBox box;
    tColSphere sphere;
    tColCylinder cylinder;
} ZdmFixture;

static void ZdmBuild(ZdmFixture* f, int shape, int scaled, int colFlags,
                     const Vec3* center, float radius, const Vec3* prev,
                     const Vec3* moveDir)
{
    memset(f, 0, sizeof(*f));

    f->gmatIds[0] = 0;
    f->gmatElems[0].flags = 0;
    f->gmatElems[0].slide = 1.0f;
    f->gmatElems[0].rebound = 0.0f;
    f->gmat.num = 1;
    f->gmat.ptrInts = f->gmatIds;
    f->gmat.elems = f->gmatElems;

    f->box.pos1.x = 1.0f;  f->box.pos1.y = 2.0f;  f->box.pos1.z = 0.5f;
    f->box.pos2.x = -1.0f; f->box.pos2.y = -0.5f; f->box.pos2.z = -2.0f;
    f->sphere.center.x = 0.25f; f->sphere.center.y = -0.5f; f->sphere.center.z = 1.0f;
    f->sphere.radius = 1.25f;
    f->cylinder.center.x = 0.0f; f->cylinder.center.y = 0.5f; f->cylinder.center.z = -1.0f;
    f->cylinder.height = 1.5f;   /* the field named height is the radius     */
    f->cylinder.radius = 2.0f;   /* the field named radius is the half-height */

    f->cob.gameMaterialElemID = 0;
    if (shape == 0)      { f->colObj.objectType = 1; f->cob.enclosurePtr = &f->box; }
    else if (shape == 1) { f->colObj.objectType = 2; f->cob.enclosurePtr = &f->sphere; }
    else                 { f->colObj.objectType = 3; f->cob.enclosurePtr = &f->cylinder; }
    f->colObj.flags = 0;
    f->colObj.gameMaterial = &f->gmat;
    f->colObj.objectPtrByType = &f->cob;

    f->instElem.colFlags = (uint8_t)colFlags;
    f->instElem.elemType = 2;
    f->instElem.elemShapePtr = &f->box;

    f->zoneIds[0] = 0;
    f->instSet.instElemNum = 1;
    f->instSet.instanceNum = 1;
    f->instSet.colInstElems = &f->instElem;
    f->instSet.zoneIDs = f->zoneIds;
    f->colInstance.colSet = &f->instSet;
    f->colInstance.zoneNum = 1;
    f->colInstance.colInstElems = &f->instElem;
    f->colInstance.activationFlags = 1;
    f->colInstance.priority = 1;
    f->colInstance.mtx = &f->instMtx;

    MATH_SetIdentityMatrix(&f->instMtx);
    MATH_SetIdentityMatrix(&f->mapMtx);
    if (scaled)
    {
        f->mapMtx.mtxflag |= 8;
        f->mapMtx._m14 = 2.0f;
        f->mapMtx._m24 = 0.5f;
        f->mapMtx._m34 = 1.5f;
        f->mapMtx.v1.x = 2.0f;
        f->mapMtx.v2.y = 0.5f;
        f->mapMtx.v3.z = 1.5f;
    }

    f->instColl.colInstance = &f->colInstance;
    f->instExt.coll = &f->instColl;
    f->instGao.extended = &f->instExt;
    f->instGao.matrices = &f->instMtx;
    f->instGao.flags = 0x200;
    f->mapColl.colInstance = &f->colInstance;
    f->mapExt.coll = &f->mapColl;
    f->mapGao.extended = &f->mapExt;
    f->mapGao.matrices = &f->mapMtx;
    f->mapGao.flags = 0x100;

    f->world.recordCount = 0;

    f->cg.colWorld = &f->world;
    f->cg.colInstanceGao = &f->instGao;
    f->cg.colMapGao = &f->mapGao;
    f->cg.collInstanceElem = &f->instElem;
    f->cg.colInstance = &f->colInstance;
    f->cg.colInstMatrix = &f->instMtx;
    f->cg.colInstGaoMatrices = &f->instMtx;
    f->cg.colMapGaoMatrices = &f->mapMtx;
    f->cg.collisionObjectPtr = &f->colObj;
    f->cg.colMap = NULL;
    f->cg.centerPos = *center;
    f->cg.centerPosPrevious = *prev;
    f->cg.dynCenterPos = *prev;
    f->cg.radius = radius;
    f->cg.dynRadius = radius;
    f->cg.moveVecUnit = *moveDir;
    f->cg.moveVec = *moveDir;
    f->cg.wallCosAngle = 0.5f;
    f->cg.maxStepSize = -1.0f;
    f->cg.ptrSetToVec6InZDMZDRGravityNeg = &f->cg.vec6InZDMZDRGravityNeg;
    f->cg.vec6InZDMZDRGravityNeg.z = -1.0f;
    MATH_SetIdentityMatrix(&f->cg.colMapGaoInvMatrix);
    f->cg.num = 0;
}

static void Run_Zdm(void)
{
    /* One fixture, rebuilt from scratch between the two sides, so every
       pointer inside it is the same address for both and the mutated
       state can be compared as raw bytes. */
    static ZdmFixture fix;
    Capture cap[2];
    int shape, scaled, cf, r, d, ix, iz, side, n;
    static const int colFlagSet[] = { 0x01, 0x11, 0x81 };
    char detail[192];
    Vec3 center, prev, dir;
    int rc;
    unsigned char* w;

    gWhere = "COL_ZDM_*";
    for (shape = 0; shape < 3; shape++)
    for (scaled = 0; scaled < 2; scaled++)
    for (cf = 0; cf < 3; cf++)
    for (r = 0; r < NRADIUS; r++)
    for (d = 0; d < NDIRS; d++)
    for (ix = 0; ix < NCOORD; ix++)
    for (iz = 0; iz < NCOORD; iz++)
    {
        center.x = gCoord[ix];
        center.y = gCoord[(ix + iz) % NCOORD];
        center.z = gCoord[iz];
        prev.x = gCoord[(ix + 2) % NCOORD];
        prev.y = gCoord[(iz + 4) % NCOORD];
        prev.z = gCoord[(ix + iz + 1) % NCOORD];
        dir = gDirs[d];
        for (side = 0; side < 2; side++)
        {
            ZdmBuild(&fix, shape, scaled, colFlagSet[cf],
                     &center, gRadius[r], &prev, &dir);
            COLB_LogReset();
            if (shape == 0)
                rc = side ? NEW_COL_ZDM_Box(&fix.cg) : BASE_COL_ZDM_Box(&fix.cg);
            else if (shape == 1)
                rc = side ? NEW_COL_ZDM_Sphere(&fix.cg) : BASE_COL_ZDM_Sphere(&fix.cg);
            else
                rc = side ? NEW_COL_ZDM_Cylinder(&fix.cg) : BASE_COL_ZDM_Cylinder(&fix.cg);

            Poison(cap[side].bytes, sizeof(cap[side].bytes));
            w = cap[side].bytes;
            memcpy(w +  0, &rc, 4);
            memcpy(w +  4, &fix.cg.num, 4);
            memcpy(w +  8, &fix.world.recordCount, 4);
            memcpy(w + 12, &fix.cg.recordCountBackup, 4);
            memcpy(w + 16, &fix.cg.centerPos, 12);
            memcpy(w + 28, &fix.cg.accelVecInZDMZDR, 12);
            memcpy(w + 40, &fix.cg.vec6InZDMZDRGravityNeg, 12);
            memcpy(w + 52, &fix.colInstance.flag, 2);
            memcpy(w + 54, &fix.colInstance.wallPush, 4);
            memcpy(w + 58, &fix.colObj.flags, 1);
            memcpy(w + 59, &fix.gmatElems[0].id, 2);
            /* the first two records, pointers folded to small ids */
            for (n = 0; n < 2; n++)
            {
                tRecordElem* re = &fix.world.records[n];
                unsigned char* q = w + 64 + n * 56;
                int instId = re->collisionInstanceGameObject == NULL ? 0
                           : re->collisionInstanceGameObject == &fix.instGao ? 1
                           : re->collisionInstanceGameObject == &fix.mapGao ? 2 : 3;
                int mapId  = re->collisionMapGameObject == NULL ? 0
                           : re->collisionMapGameObject == &fix.instGao ? 1
                           : re->collisionMapGameObject == &fix.mapGao ? 2 : 3;
                int gmatId = re->gameMaterial == NULL ? 0
                           : re->gameMaterial == &fix.gmatElems[0] ? 1 : 2;
                memcpy(q +  0, &re->collidedPoint, 12);
                memcpy(q + 12, &re->recalVec, 12);
                memcpy(q + 24, &re->normalVec, 12);
                memcpy(q + 36, &re->flags, 4);
                memcpy(q + 40, &re->triangleChunkElemID, 4);
                memcpy(q + 44, &re->triangleChunkID, 2);
                memcpy(q + 46, &re->designVal, 1);
                memcpy(q + 48, &instId, 4);
                memcpy(q + 52, &mapId, 4);
                q[47] = (unsigned char)gmatId;
            }
            cap[side].len = 64 + 2 * 56;
            CaptureCalls(&cap[side]);
        }
        sprintf(detail, "shape %d scaled %d cf %x r %g dir %d c %g,%g,%g",
                shape, scaled, colFlagSet[cf], (double)gRadius[r], d,
                (double)center.x, (double)center.y, (double)center.z);
        Compare(gWhere, &cap[0], &cap[1], detail);
    }
}

/* ---- 8. COL_UpdateCobFromVisualRealTime ---------------------------- */

/* The 14 unrolled copies of the wave loop and the face-normal pass are
   the largest restructuring in the range, and they need nothing but
   plain structs: a visual geometry, a collision mesh of the same vertex
   count, and the wave modifier's own parameters. */

#define COB_NVERT 9
#define COB_NCHUNK 3
#define COB_NTRI 4

typedef struct
{
    tGEOGRO geogro;
    Vec3 geoVerts[COB_NVERT];
    tCobMeshObjectMesh mesh;
    Vec3 meshVerts[COB_NVERT];
    Vec3 meshNormals[COB_NCHUNK * COB_NTRI];
    tCobMeshChunkElem chunks[COB_NCHUNK];
    tCobMeshTriangleElem tris[COB_NCHUNK][COB_NTRI];
    tColCollisionObject colObj;
    tGameObjectMain gao;
    tGameObjectExtended ext;
    tGameObjectBasic basic;
    tGameObjectVisualObject visual;
    tModifier mdf;
    tModifierInterface mdfIntf;
    tOtcMdfStruct otc;
    int rli[COB_NVERT + 1];
} CobFixture;

static void CobBuild(CobFixture* f, int otcFlags, int withRli, int vertexMismatch,
                     float offset, float intensity, float speed, int mdfId)
{
    int i, c, t;

    memset(f, 0, sizeof(*f));

    for (i = 0; i < COB_NVERT; i++)
    {
        f->geoVerts[i].x = (float)(i - 4) * 0.75f;
        f->geoVerts[i].y = (float)((i * 3) % 7) - 3.0f;
        f->geoVerts[i].z = (float)((i * 5) % 5) * 0.5f - 1.0f;
    }
    f->geogro.vertexNum = COB_NVERT;
    f->geogro.vertices = f->geoVerts;

    f->mesh.vertexCount = vertexMismatch ? COB_NVERT - 1 : COB_NVERT;
    f->mesh.faceCount = COB_NCHUNK * COB_NTRI;
    f->mesh.meshChunkElemNum = COB_NCHUNK;
    f->mesh.vertices = f->meshVerts;
    f->mesh.normals = f->meshNormals;
    f->mesh.meshChunkElems = f->chunks;
    f->mesh.ok3 = NULL;
    for (c = 0; c < COB_NCHUNK; c++)
    {
        /* the middle chunk is empty, which is the path that leaves the
           running face index where it was */
        f->chunks[c].elemNum = (uint16_t)(c == 1 ? 0 : COB_NTRI);
        f->chunks[c].triangleElems = f->tris[c];
        for (t = 0; t < COB_NTRI; t++)
        {
            f->tris[c][t].v1 = (uint16_t)((c * 3 + t) % COB_NVERT);
            f->tris[c][t].v2 = (uint16_t)((c * 3 + t + 1) % COB_NVERT);
            /* one triangle per chunk is degenerate, so the zero-normal
               arm is exercised too */
            f->tris[c][t].v3 = (uint16_t)(t == 2 ? f->tris[c][t].v1
                                                 : (c * 3 + t + 4) % COB_NVERT);
        }
    }

    f->colObj.objectType = 5;
    f->colObj.flags = 0x40;
    f->colObj.objectPtrByType = &f->mesh;

    f->otc.flags = otcFlags;
    f->otc.offset = offset;
    f->otc.intensity = intensity;
    f->otc.speed = speed;
    f->otc.timeMultiplier = 1.0f;

    f->mdfIntf.mdfId = mdfId;
    f->mdf.modifierIntf = &f->mdfIntf;
    f->mdf.modifierFlags = 0;
    f->mdf.mdfPrivateDataPtr = &f->otc;
    f->mdf.nextElem = NULL;

    for (i = 0; i < COB_NVERT + 1; i++)
        f->rli[i] = (int)((unsigned)((i * 37 + 11) & 0xFF) << 24) | 0x00112233;
    f->visual.RLIVertexColors = withRli ? f->rli : NULL;

    f->basic.actorObjectOrVisualObject = &f->visual;
    f->ext.mdf = &f->mdf;
    f->gao.extended = &f->ext;
    f->gao.basic = &f->basic;
    f->gao.flags = 0x2000;
}

static void Run_CobFromVisual(void)
{
    static CobFixture fix;
    Capture cap[2];
    static const int flagSet[] = {
        0x00, 0x01, 0x02, 0x04, 0x09, 0x0A, 0x0C,
        0x11, 0x19, 0x1A, 0x1C, 0x12, 0x14,
        0x31, 0x39, 0x3A, 0x3C, 0x32, 0x34, 0x08, 0x38
    };
    static const float offsets[] = { 0.0f, 0.25f, 1.5f };
    static const float speeds[] = { 0.0f, 0.5f, 2.0f };
    static const float intensities[] = { 0.0f, 0.75f };
    int fl, rli, mism, o, sp, inten, mdfId, side;
    char detail[192];
    unsigned char* w;

    gWhere = "COL_UpdateCobFromVisualRealTime";
    for (fl = 0; fl < (int)(sizeof(flagSet) / sizeof(flagSet[0])); fl++)
    for (rli = 0; rli < 2; rli++)
    for (mism = 0; mism < 2; mism++)
    for (mdfId = 1; mdfId <= 2; mdfId++)
    for (o = 0; o < 3; o++)
    for (sp = 0; sp < 3; sp++)
    for (inten = 0; inten < 2; inten++)
    {
        for (side = 0; side < 2; side++)
        {
            CobBuild(&fix, flagSet[fl], rli, mism, offsets[o],
                     intensities[inten], speeds[sp], mdfId);
            memset(&PRO_RasterGroup.PRO_COBFromVisual, 0,
                   sizeof(PRO_RasterGroup.PRO_COBFromVisual));
            COLB_LogReset();
            if (side) NEW_COL_UpdateCobFromVisualRealTime(&fix.gao, &fix.colObj, &fix.geogro);
            else      BASE_COL_UpdateCobFromVisualRealTime(&fix.gao, &fix.colObj, &fix.geogro);

            Poison(cap[side].bytes, sizeof(cap[side].bytes));
            w = cap[side].bytes;
            /* leading word: did the routine do any work at all, i.e. are
               the collision vertices now the visual ones or better */
            { static const Vec3 zeroes[COB_NVERT];
              int touched = memcmp(fix.meshVerts, zeroes, sizeof(zeroes)) != 0;
              memcpy(w, &touched, 4); }
            w += 4;
            memcpy(w, fix.meshVerts, sizeof(fix.meshVerts));
            memcpy(w + sizeof(fix.meshVerts), fix.meshNormals, sizeof(fix.meshNormals));
            w += sizeof(fix.meshVerts) + sizeof(fix.meshNormals);
            memcpy(w + 0, &fix.colObj.flags, 1);
            memcpy(w + 1, &fix.otc.offset, 4);
            memcpy(w + 5, &PRO_RasterGroup.PRO_COBFromVisual.insideCounter, 2);
            memcpy(w + 7, &PRO_RasterGroup.PRO_COBFromVisual.sumTime, 4);
            cap[side].len = (int)(4 + sizeof(fix.meshVerts) + sizeof(fix.meshNormals) + 11);
            CaptureCalls(&cap[side]);
        }
        sprintf(detail, "otcflags %02x rli %d mismatch %d mdfId %d off %g speed %g int %g",
                flagSet[fl], rli, mism, mdfId, (double)offsets[o],
                (double)speeds[sp], (double)intensities[inten]);
        Compare(gWhere, &cap[0], &cap[1], detail);
    }
}

/* ---- main ---------------------------------------------------------- */

static tWorld gCurrentWorld;
static tViewElem gCurrentView;

int main(void)
{
    MATH_InitModule();

    /* COL_ComputeReport asks the current world which object the camera
       is attached to; nothing in the fixture is that object. */
    memset(&gCurrentWorld, 0, sizeof(gCurrentWorld));
    memset(&gCurrentView, 0, sizeof(gCurrentView));
    gCurrentWorld.viewPtr = &gCurrentView;
    WOR_gpst_CurrentWorld = &gCurrentWorld;

    printf("collision.c functions 64-97: frozen copy against the rewrite\n\n");

#define STAGE(fn, nm) do { long c0 = gCases, t0 = gTrue, k0 = gCalls; fn(); \
        printf("  %-24s %7ld cases, %7ld reported a hit, %7ld made engine calls\n", \
               nm, gCases - c0, gTrue - t0, gCalls - k0); } while (0)

    STAGE(Run_SphereAABBox, "INT_SphereAABBox");
    STAGE(Run_SphereOBBox, "INT_SphereOBBox");
    STAGE(Run_FullSphereOBBox, "INT_FullSphereOBBox");
    STAGE(Run_FullSphereCylinder, "INT_FullSphereCylinder");
    STAGE(Run_FullAABBoxTriangle, "INT_FullAABBoxTriangle");
    STAGE(Run_ComputeRejection, "COL_ComputeRejection");
    STAGE(Run_Zdm, "COL_ZDM_*");
    STAGE(Run_CobFromVisual, "COL_UpdateCobFromVisual");

    printf("\n%ld cases, %ld result differences, %ld call-sequence differences\n",
           gCases, gDiffResult, gDiffCalls);
    if (gDiffResult || gDiffCalls)
    {
        printf("FAIL\n");
        return 1;
    }
    printf("no differences\n");
    return 0;
}
