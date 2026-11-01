#include "colcheck.h"

#if COLCHK_ENABLED

#include "common.h"
#include "gameobject.h"
#include "collision.h"
#include "math.h"
#include "dbg.h"

static unsigned int COLCHK_gul_Reports;
static unsigned int COLCHK_gul_Calls;
static unsigned int COLCHK_gul_Hits;
static unsigned int COLCHK_gul_Gated;      /* flags2 bit 0 clear */
static unsigned int COLCHK_gul_NoVolume;   /* missing or degenerate */
static unsigned int COLCHK_gul_Outside;    /* the ordinary negative */

/* True when the pivot is outside the box but close to it on every
   axis -- what standing beside a ramp looks like, as opposed to the
   thousands of distant pairs tested every frame. */
static int COLCHK_NearBox(const Vec3* lo, const Vec3* hi, const Vec3* p)
{
    return p->x > lo->x - COLCHK_NEAR && p->x < hi->x + COLCHK_NEAR
        && p->y > lo->y - COLCHK_NEAR && p->y < hi->y + COLCHK_NEAR
        && p->z > lo->z - COLCHK_NEAR && p->z < hi->z + COLCHK_NEAR;
}

static void COLCHK_Summary(void)
{
    DBG_OutputDebugString("COLCHK after %u calls: %u passed, %u gated on"
                          " flags2, %u with no usable volume, %u simply"
                          " outside\n",
                          COLCHK_gul_Calls, COLCHK_gul_Hits,
                          COLCHK_gul_Gated, COLCHK_gul_NoVolume,
                          COLCHK_gul_Outside);
}

/* The function swaps its two arguments on `reverse`, and the test is
   always "is firstGao's pivot inside secondGao's bounding volume". The
   caller's own names for them are the other way round from the names
   this prints, so both are given explicitly. */
void COLCHK_PivotCollide(struct sGameObjectMain* gao1,
                         struct sGameObjectMain* gao2,
                         int reverse, int result)
{
    struct sGameObjectMain* pivotOf;
    struct sGameObjectMain* volumeOf;
    const char* why = 0;

    ++COLCHK_gul_Calls;
    if (result)
        ++COLCHK_gul_Hits;
    if (COLCHK_SUMMARY_EVERY
        && (COLCHK_gul_Calls % COLCHK_SUMMARY_EVERY) == 0)
        COLCHK_Summary();

    if (result && !COLCHK_REPORT_HITS)
        return;
    if (COLCHK_MAX_REPORTS && COLCHK_gul_Reports >= COLCHK_MAX_REPORTS)
        return;

    if (!gao1 || !gao2)
    {
        ++COLCHK_gul_Reports;
        DBG_OutputDebugString("COLCHK pivot/BV: a null object (%p, %p)\n",
                              (void*)gao1, (void*)gao2);
        return;
    }

    pivotOf  = reverse ? gao1 : gao2;
    volumeOf = reverse ? gao2 : gao1;

    /* The gate the function applies before looking at any geometry.
       Both objects need bit 0 of flags2, which is what makes an
       object collidable at all. */
    if ((gao1->flags2 & 1) == 0 || (gao2->flags2 & 1) == 0)
    {
        ++COLCHK_gul_Gated;
        why = "one of them has flags2 bit 0 clear, so the test never"
              " looks at the geometry";
    }

    ++COLCHK_gul_Reports;
    DBG_OutputDebugString("COLCHK pivot/BV %u: result %d, reverse %d."
                          " pivot from %p (flags %08x, flags2 %08x),"
                          " volume from %p (flags %08x, flags2 %08x)\n",
                          COLCHK_gul_Reports, result, reverse,
                          (void*)pivotOf, pivotOf->flags, pivotOf->flags2,
                          (void*)volumeOf, volumeOf->flags, volumeOf->flags2);

    if (why)
    {
        DBG_OutputDebugString("COLCHK   %s\n", why);
        return;
    }

    if (!volumeOf->boundingVolume)
    {
        ++COLCHK_gul_NoVolume;
        DBG_OutputDebugString("COLCHK   the volume object has no bounding"
                              " volume at all\n");
        return;
    }

    if (OBJ_BV_IsSphere(volumeOf->boundingVolume))
    {
        Vec3 d;
        Mtx4x4* m = OBJ_pst_GetAbsoluteMatrix(pivotOf);

        MATH_SubstractVec(&d, &m->Pos, &volumeOf->boundingVolume->BVMin);
        DBG_OutputDebugString("COLCHK   sphere: radius field %f%s,"
                              " centre (%f %f %f), pivot (%f %f %f),"
                              " distance %f against radius %f\n",
                              volumeOf->boundingVolume->BVMax.x,
                              volumeOf->boundingVolume->BVMax.x == 0.0f
                                ? " -- zero, so the test is skipped" : "",
                              volumeOf->boundingVolume->BVMin.x,
                              volumeOf->boundingVolume->BVMin.y,
                              volumeOf->boundingVolume->BVMin.z,
                              m->Pos.x, m->Pos.y, m->Pos.z,
                              MATH_VecLenSquared(&d),
                              volumeOf->boundingVolume->BVMax.x
                                * volumeOf->boundingVolume->BVMax.x);
        return;
    }

    {
        Vec3 lo, hi;
        Mtx4x4* m = OBJ_pst_GetAbsoluteMatrix(pivotOf);

        MATH_VecAddVec(&lo, &volumeOf->boundingVolume->BVMin,
                       &volumeOf->matrices->Pos);
        MATH_VecAddVec(&hi, &volumeOf->boundingVolume->BVMax,
                       &volumeOf->matrices->Pos);

        if (lo.x == hi.x || lo.y == hi.y || lo.z == hi.z)
            ++COLCHK_gul_NoVolume;
        else
        {
            ++COLCHK_gul_Outside;
            /* Report only the near miss; a distant pair says nothing
               and there are thousands of them every frame. */
            if (!result && !COLCHK_NearBox(&lo, &hi, &m->Pos))
            {
                --COLCHK_gul_Reports;
                return;
            }
        }

        DBG_OutputDebugString("COLCHK   box: lo (%f %f %f) hi (%f %f %f)"
                              " pivot (%f %f %f)%s\n",
                              lo.x, lo.y, lo.z, hi.x, hi.y, hi.z,
                              m->Pos.x, m->Pos.y, m->Pos.z,
                              (lo.x == hi.x || lo.y == hi.y || lo.z == hi.z)
                                ? " -- degenerate on an axis, so the test"
                                  " is skipped" : "");
        if (lo.x != hi.x && lo.y != hi.y && lo.z != hi.z)
            DBG_OutputDebugString("COLCHK   outside on:%s%s%s%s%s%s\n",
                                  m->Pos.x <= lo.x ? " x-low"  : "",
                                  m->Pos.x >= hi.x ? " x-high" : "",
                                  m->Pos.y <= lo.y ? " y-low"  : "",
                                  m->Pos.y >= hi.y ? " y-high" : "",
                                  m->Pos.z <= lo.z ? " z-low"  : "",
                                  m->Pos.z >= hi.z ? " z-high" : "");
    }
}

#endif  /* COLCHK_ENABLED */
