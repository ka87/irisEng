#pragma once

/* Why COL_BV_PivotCollide said no.
 *
 * The contextual-action behaviours all gate on this one test -- a
 * ramp offers its jump only while
 * `COL_BV_PivotCollide(Sally)` is true -- and it has three ways to
 * return FALSE that look identical from the outside: either object
 * missing bit 0 of flags2, a bounding volume that is degenerate, or
 * the pivot genuinely outside the volume. This prints which.
 *
 * Set COLCHK_ENABLED to 0 to compile it all away. */
#define COLCHK_ENABLED 1

/* Log at most this many reports, so a per-frame test cannot flood the
 * output. 0 means no limit. */
#define COLCHK_MAX_REPORTS 400

/* Log the calls that SUCCEED as well. Off by default: the interesting
 * case is the one that keeps saying no. */
#define COLCHK_REPORT_HITS 0

struct sGameObjectMain;

#if COLCHK_ENABLED
void COLCHK_PivotCollide(struct sGameObjectMain* gao1,
                         struct sGameObjectMain* gao2,
                         int reverse, int result);
#else
#define COLCHK_PivotCollide(a, b, c, d)   ((void)0)
#endif
