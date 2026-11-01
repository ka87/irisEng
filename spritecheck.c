#include "spritecheck.h"

#if SPGCHK_ENABLED

#include "math.h"
#include "sprites.h"
#include "common.h"
#include "dbg.h"

static int SPGCHK_gi_Reported;

static int SPGCHK_NotFinite(float f)
{
    /* no isnan/isfinite here: read the bits instead, so this works the
       same whatever the compiler does with floating point */
    const unsigned int bits = *(const unsigned int*)&f;
    return ((bits >> 23) & 0xFF) == 0xFF;      /* infinity or not a number */
}

static int SPGCHK_Impossible(const Vec3* v)
{
    if (SPGCHK_NotFinite(v->x) || SPGCHK_NotFinite(v->y) || SPGCHK_NotFinite(v->z))
        return 1;
    return v->x > SPGCHK_SANE_RANGE || v->x < -SPGCHK_SANE_RANGE
        || v->y > SPGCHK_SANE_RANGE || v->y < -SPGCHK_SANE_RANGE
        || v->z > SPGCHK_SANE_RANGE || v->z < -SPGCHK_SANE_RANGE;
}

/* The two noise vectors and a few random table entries: these are the
   multipliers every sprite vertex is built from, so a bad vertex
   almost always shows up here first. */
void SPGCHK_ReportInputs(const char* where)
{
    if (SPGCHK_LIMIT && SPGCHK_gi_Reported >= SPGCHK_LIMIT)
        return;

    DBG_OutputDebugString("SPGCHK %s noise1 %g %g %g noise2 %g %g %g\n", where,
        SPGNoise1.x, SPGNoise1.y, SPGNoise1.z,
        SPGNoise2.x, SPGNoise2.y, SPGNoise2.z);
    DBG_OutputDebugString("SPGCHK %s random[0,1,2,255] %g %g %g %g\n", where,
        f_RandomTable[0], f_RandomTable[1], f_RandomTable[2], f_RandomTable[255]);

    if (SPGCHK_Impossible(&SPGNoise1) || SPGCHK_Impossible(&SPGNoise2))
    {
        DBG_OutputDebugString("SPGCHK %s NOISE IS ALREADY IMPOSSIBLE"
                              " -- the fault is upstream of the vertices\n", where);
        ++SPGCHK_gi_Reported;
    }
}

/* Scan one batch of sprite vertices just before it is handed to the
   renderer. Reports the first bad vertex and how many of the batch are
   bad, not every one, so one broken batch is one line. */
void SPGCHK_CheckBatch(const char* where, int num)
{
    int i, bad = 0, first = -1;
    int notFinite = 0, outOfRange = 0;

    if (num <= 0 || num > 256)
    {
        DBG_OutputDebugString("SPGCHK %s batch count %d is outside the stack\n", where, num);
        return;
    }

    for (i = 0; i < num; i++)
    {
        if (SPGCHK_Impossible(&SPG_gvStack[i]))
        {
            if (first < 0)
                first = i;
            ++bad;
            if (SPGCHK_NotFinite(SPG_gvStack[i].x)
                || SPGCHK_NotFinite(SPG_gvStack[i].y)
                || SPGCHK_NotFinite(SPG_gvStack[i].z))
                ++notFinite;
            else
                ++outOfRange;
        }
    }

    if (!bad)
        return;

    if (SPGCHK_LIMIT && SPGCHK_gi_Reported >= SPGCHK_LIMIT)
        return;
    ++SPGCHK_gi_Reported;

    DBG_OutputDebugString("SPGCHK %s %d of %d vertices impossible"
                          " (%d not finite, %d out of range), first at %d: %g %g %g\n",
        where, bad, num, notFinite, outOfRange, first,
        SPG_gvStack[first].x, SPG_gvStack[first].y, SPG_gvStack[first].z);

    SPGCHK_ReportInputs(where);
}

#endif
