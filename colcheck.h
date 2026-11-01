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

/* The test runs every frame against every collidable pair, so
 * reporting each negative buries the one that matters -- a first run
 * spent its whole budget on distant objects before the player reached
 * the ramp. Only two kinds of call are reported in detail now:
 *
 *   - a DEFECT: either object missing flags2 bit 0, a missing bounding
 *     volume, or one that is degenerate on an axis. These mean the test
 *     never looked at the geometry.
 *   - a NEAR MISS: the pivot is outside, but within COLCHK_NEAR of the
 *     volume on every axis, which is what standing next to a ramp looks
 *     like.
 *
 * Everything else is only counted, and the counts are printed every
 * COLCHK_SUMMARY_EVERY calls so nothing is dropped silently. */
#define COLCHK_MAX_REPORTS     2000
#define COLCHK_SUMMARY_EVERY   20000
#define COLCHK_NEAR            3.0f

/* Log the calls that SUCCEED as well. Worth turning on once a near
 * miss shows the right pair is being tested: a success here means the
 * gate passed and the behaviour's own later conditions are what fail. */
#define COLCHK_REPORT_HITS 0

struct sGameObjectMain;

#if COLCHK_ENABLED
void COLCHK_PivotCollide(struct sGameObjectMain* gao1,
                         struct sGameObjectMain* gao2,
                         int reverse, int result);
#else
#define COLCHK_PivotCollide(a, b, c, d)   ((void)0)
#endif
