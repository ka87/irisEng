/* A unit test for depthblur.c.
 *
 * The extraction itself is already checked, and checked harder than
 * this file can manage: tests/run_wingdi_ae.sh runs the frozen
 * pre-rewrite copy of the after-effects beside the rewrite over the
 * same state and compares the whole 256-entry ramp, the five
 * distances, the mode and the coordinate transform, with every one of
 * the five modes driven on purpose. Anything this file duplicates of
 * that is weaker than the original.
 *
 * What it is for instead is the property the ramp rests on and the
 * differential comparison cannot state: that the distances always come
 * out ordered and separated, which is what keeps the ramp's two
 * divisions inside the table. The frozen copy divides without checking
 * and so does the extraction, so "it has never gone out of range" is
 * the whole argument -- and an argument like that is worth testing
 * rather than believing, especially now that a second renderer is
 * about to build a texture from the same table.
 *
 * It is also what makes the two clamps in DPB_BuildRamp provably dead
 * rather than merely untaken. They are a backstop for a hand-built
 * state; no state DPB_Decode produces can reach them, and the fuzz
 * below is what says so.
 */

#include "../depthblur.h"

#include <float.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

static long checks;
static long fails;

static void ok(int cond, const char* what)
{
    checks++;
    if (!cond)
    {
        printf("  FAIL %s\n", what);
        fails++;
    }
}

static void okEq(long got, long want, const char* what)
{
    checks++;
    if (got != want)
    {
        printf("  FAIL %s: got %ld, wanted %ld\n", what, got, want);
        fails++;
    }
}

/* ------------------------------------------------------- the inputs
 *
 * Deliberately hostile, and deliberately not a tidy grid: zero and
 * negative distances, a denormal near plane, a far plane in front of
 * the blur distances, settings far larger than the frustum, and
 * settings in the wrong order relative to each other. The engine
 * starts all three settings at zero, so that case is not a corner --
 * it is the one that runs at startup.
 */
static const float dpb_distances[] = {
    0.0f, -1.0f, -1e30f, FLT_MIN, 1e-20f, 0.001f, 0.01f, 0.1f,
    1.0f, 9.0f, 21.0f, 77.0f, 100.0f, 1000.0f, 1e6f, 1e20f
};
#define DPB_NDIST ((int)(sizeof(dpb_distances) / sizeof(dpb_distances[0])))

/* --------------------------------------------- the mode truth table */

static void caseKind(void)
{
    tDpb_Source src;
    tDpb_State  st;
    int         tweak, far, near, sun;

    printf("-- which blur is chosen\n");

    memset(&src, 0, sizeof(src));
    src.nearPlane = 0.1f;
    src.farPlane = 1000.0f;

    /* Every combination of the two master bits, the tweak and whether
       a sun asked, against what each one means. Bits outside the two
       are set as well, to show they are not read. */
    for (tweak = 0; tweak < 2; tweak++)
        for (far = 0; far < 2; far++)
            for (near = 0; near < 2; near++)
                for (sun = 0; sun < 2; sun++)
                {
                    int want;

                    src.b_zonAlphaTweak = tweak;
                    src.aeMaster = (unsigned int)(far ? 8 : 0)
                                   | (unsigned int)(near ? 0x1000 : 0)
                                   | 0x2466;   /* noise in the other bits */
                    src.b_sunEffectOn = sun;
                    DPB_Decode(&src, &st);

                    if (tweak)
                        want = DPB_OFF;
                    else if (far && near)
                        want = DPB_BOTH;
                    else if (far)
                        want = DPB_FAR;
                    else if (near)
                        want = DPB_NEAR;
                    else
                        want = sun ? DPB_SUN : DPB_OFF;

                    checks++;
                    if (st.kind != want)
                    {
                        printf("  FAIL kind: tweak=%d far=%d near=%d sun=%d"
                               " gave %d, wanted %d\n",
                               tweak, far, near, sun, st.kind, want);
                        fails++;
                    }
                }

    /* The tweak beats a sun that asked, and beats both master bits. */
    src.b_zonAlphaTweak = 1;
    src.aeMaster = 8 | 0x1000;
    src.b_sunEffectOn = 1;
    DPB_Decode(&src, &st);
    okEq(st.kind, DPB_OFF, "the tweak turns everything off");

    /* A sun only gets its mode when no blur was asked for. */
    src.b_zonAlphaTweak = 0;
    src.aeMaster = 8;
    src.b_sunEffectOn = 1;
    DPB_Decode(&src, &st);
    okEq(st.kind, DPB_FAR, "a blur that was asked for beats the sun's mode");
}

/* ------------------------------ the ordering, over the hostile grid */

static void caseOrdering(void)
{
    tDpb_Source src;
    tDpb_State  st;
    int         a, b, c, d, e;
    long        states = 0;
    long        unordered = 0;
    long        outOfRange = 0;
    long        worstIdx = -1;

    printf("-- the distances come out ordered, whatever goes in\n");

    memset(&src, 0, sizeof(src));

    for (a = 0; a < DPB_NDIST; a++)
        for (b = 0; b < DPB_NDIST; b++)
            for (c = 0; c < DPB_NDIST; c++)
                for (d = 0; d < DPB_NDIST; d++)
                    for (e = 0; e < 5; e++)
                    {
                        int i0, i1, i2;

                        src.nearPlane = dpb_distances[a];
                        src.farPlane = dpb_distances[b];
                        src.depthBlurNear1 = dpb_distances[c];
                        src.depthBlurType0 = dpb_distances[d];
                        src.depthBlurType1 = dpb_distances[(d + a) % DPB_NDIST];
                        src.b_zonAlphaTweak = (e == 1);
                        src.aeMaster = (e == 2) ? 8u
                                       : (e == 3) ? 0x1000u
                                       : (e == 4) ? (8u | 0x1000u) : 0u;
                        src.b_sunEffectOn = (e == 0);

                        DPB_Decode(&src, &st);
                        states++;

                        if (!DPB_b_Ordered(&st))
                        {
                            if (unordered == 0)
                                printf("  FAIL unordered from near=%g far=%g"
                                       " n1=%g t0=%g t1=%g -> %g %g %g %g %g\n",
                                       src.nearPlane, src.farPlane,
                                       src.depthBlurNear1, src.depthBlurType0,
                                       src.depthBlurType1,
                                       st.nearClamped, st.near1, st.type0,
                                       st.type1, st.farUsed);
                            unordered++;
                        }

                        /* The three indices the ramp computes. Every
                           one is a smaller distance over a larger one,
                           so every one has to land inside the table --
                           and that is what makes the clamps dead. */
                        i0 = (int)(unsigned int)(st.type0 / st.type1 * 256.0);
                        i1 = (int)(unsigned int)(st.near1 / st.type1 * 256.0);
                        i2 = (int)(unsigned int)(st.near1 / st.type0 * 256.0);
                        /* 256 is the reachable extreme, not an
                           error: at a large enough setting two
                           distances come out equal and the ratio is
                           exactly one. Past 256 is what would run off
                           the table. */
                        if (i0 < 0 || i0 > 256 || i1 < 0 || i1 > 256
                            || i2 < 0 || i2 > 256)
                        {
                            outOfRange++;
                            if (i0 > worstIdx) worstIdx = i0;
                            if (i1 > worstIdx) worstIdx = i1;
                            if (i2 > worstIdx) worstIdx = i2;
                        }
                    }

    okEq(unordered, 0, "every decoded state is ordered");
    okEq(outOfRange, 0, "and every ramp index it produces is at most 256");
    if (outOfRange)
        printf("        worst index seen: %ld\n", worstIdx);
    printf("  %ld states decoded\n", states);

    /* And the check itself says no to a state that is not ordered, or
       it would be agreeing with anything. */
    memset(&st, 0, sizeof(st));
    ok(!DPB_b_Ordered(&st), "an all-zero state is not ordered");
    st.nearClamped = 1.0f;
    st.near1 = 2.0f;
    st.type0 = 3.0f;
    st.type1 = 4.0f;
    st.farUsed = 5.0f;
    ok(DPB_b_Ordered(&st), "an ordered one is");
    st.type0 = 2.0f;
    ok(DPB_b_Ordered(&st), "two equal neighbours are still in order, which"
                           " is the case a large setting produces");
    st.type0 = 1.5f;
    ok(!DPB_b_Ordered(&st), "one that goes backwards is not");
    st.type0 = 3.0f;
    st.farUsed = 3.5f;
    ok(!DPB_b_Ordered(&st), "nor is a far plane inside type1");
}

/* ------------------------------------------- the ramp stays in bounds */

static void caseRampBounds(void)
{
    tDpb_Source src;
    tDpb_State  st;
    int         a, b, kind;
    long        built = 0;
    long        gapsSeen = 0;

    printf("-- the ramp writes 256 entries and not one more\n");

    memset(&src, 0, sizeof(src));

    for (kind = 0; kind < 5; kind++)
        for (a = 0; a < DPB_NDIST; a++)
            for (b = 0; b < DPB_NDIST; b++)
            {
                /* Guard words either side, so a write past either end
                   is caught here as well as by the sanitiser. */
                struct { int before[8]; int table[256]; int after[8]; } buf;
                int i;
                int bad = 0;

                src.nearPlane = dpb_distances[a];
                src.farPlane = dpb_distances[b];
                src.depthBlurNear1 = dpb_distances[(a + 3) % DPB_NDIST];
                src.depthBlurType0 = dpb_distances[(b + 5) % DPB_NDIST];
                src.depthBlurType1 = dpb_distances[(a + b) % DPB_NDIST];
                src.b_zonAlphaTweak = (kind == DPB_OFF);
                src.aeMaster = (kind == DPB_FAR) ? 8u
                               : (kind == DPB_NEAR) ? 0x1000u
                               : (kind == DPB_BOTH) ? (8u | 0x1000u) : 0u;
                src.b_sunEffectOn = (kind == DPB_SUN);

                DPB_Decode(&src, &st);
                if (st.kind != kind)
                    continue;

                for (i = 0; i < 8; i++)
                    buf.before[i] = buf.after[i] = 0x5A5A5A5A;
                memset(buf.table, 0xCD, sizeof(buf.table));

                DPB_BuildRamp(&st, buf.table);
                built++;

                for (i = 0; i < 8; i++)
                    if (buf.before[i] != 0x5A5A5A5A
                        || buf.after[i] != 0x5A5A5A5A)
                        bad++;
                if (bad)
                {
                    printf("  FAIL mode %d wrote outside the table\n", kind);
                    checks++;
                    fails++;
                    return;
                }

                /* How much of the table each mode writes, which is
                   not all of it. Mode DPB_BOTH's solid run uses a
                   count where an index belongs, so it stops short and
                   leaves a gap -- the quirk preserved in depthblur.c,
                   stated here as the number of entries it skips so a
                   change to it fails rather than passing quietly.
                   Those entries keep whatever the ramp held before,
                   because the game fills this table in place over the
                   locked texture. */
                {
                    int left = 0;
                    int want = 0;

                    for (i = 0; i < 256; i++)
                        if (buf.table[i] == (int)0xCDCDCDCD)
                            left++;

                    if (kind == DPB_BOTH)
                    {
                        int ce = (int)(unsigned int)(st.near1 / st.type1 * 256.0);
                        int re = (int)(unsigned int)(st.near1 / st.type0 * 256.0);
                        int at = ce > re ? ce : re;

                        /* The solid run is `for (i = at; i <= 255 - at;
                           i++)`, a count where an index belongs, so it
                           is mirrored about the middle of the table.
                           Below the middle it overshoots and leaves the
                           far end short; at or past the middle its
                           condition is false at once and it writes
                           nothing, leaving everything from `at` to 254.
                           Entry 255 is always written. */
                        if (at <= 127)
                            want = at >= 2 ? at - 1 : 0;
                        else if (at <= 254)
                            want = 255 - at;
                        else
                            want = 0;
                    }

                    checks++;
                    if (left != want)
                    {
                        printf("  FAIL mode %d left %d entr(ies) unwritten,"
                               " expected %d\n", kind, left, want);
                        fails++;
                        return;
                    }
                    if (kind == DPB_BOTH && want > 0)
                        gapsSeen++;
                }

                /* Only the top byte carries anything, which is what
                   the texture's alpha channel is built from. Entries
                   the mode leaves alone still hold the fill, so this
                   asks only about the ones it wrote -- and a written
                   entry can never look like the fill, because the
                   fill's low bytes are not zero. */
                for (i = 0; i < 256; i++)
                    if (buf.table[i] != (int)0xCDCDCDCD
                        && (buf.table[i] & 0x00FFFFFF) != 0)
                        bad++;
                if (bad)
                {
                    printf("  FAIL mode %d put something in the low three"
                           " bytes of %d entr(ies)\n", kind, bad);
                    checks++;
                    fails++;
                    return;
                }
            }

    checks++;
    printf("  %ld ramps built across all five modes, %ld of them with the"
           " near-and-far gap\n", built, gapsSeen);
    ok(built >= 5, "every mode was reached at least once");
    checks++;
    if (gapsSeen == 0)
    {
        printf("  FAIL the near-and-far gap was never produced, so the"
               " quirk is untested\n");
        fails++;
    }
}

/* --------------------------------------------- the sun's depth ramp */

static void caseSunRamp(void)
{
    tDpb_Source src;
    tDpb_State  st;
    int         table[256];
    int         i;
    int         bad;

    printf("-- the sun's ramp, which is the one its test reads\n");

    memset(&src, 0, sizeof(src));
    src.nearPlane = 0.1f;
    src.farPlane = 1000.0f;
    src.b_sunEffectOn = 1;
    DPB_Decode(&src, &st);
    okEq(st.kind, DPB_SUN, "the sun's mode");

    /* The coordinate is the depth over the far plane, so a point at
       the far plane lands at the end of the table. */
    /* The stored value is a float, so the comparison is to a float's
       precision and not a double's. */
    ok(fabs((double)st.scaleUfromZ - 1.0 / st.farUsed)
           <= 1e-6 * (1.0 / st.farUsed),
       "one unit of u per far plane of depth");

    /* Blur falls off with depth: full at the camera, none at the far
       plane, and nothing at all past it. */
    okEq((long)DPB_i_BlurAtDepth(&st, -1.0f), 255, "behind the camera is full");
    okEq((long)DPB_i_BlurAtDepth(&st, 0.0f), 255, "and so is the camera itself");
    okEq((long)DPB_i_BlurAtDepth(&st, st.farUsed), 0, "the far plane is none");
    okEq((long)DPB_i_BlurAtDepth(&st, st.farUsed * 2.0f), 0,
         "and past it is none either");
    okEq((long)DPB_i_BlurAtDepth(&st, st.farUsed * 0.5f), 127,
         "halfway is half, rounded down");

    /* The step-to-depth mapping and the depth-to-blur one are each
       other's, so walking the table has to walk the frustum. */
    okEq((long)(DPB_f_DepthAtStep(&st, 0) == 0.0f), 1, "step 0 is the camera");
    ok(fabs((double)DPB_f_DepthAtStep(&st, 256) - st.farUsed) < 1e-3,
       "and step 256 would be the far plane");

    DPB_BuildRamp(&st, table);
    okEq((long)((unsigned int)table[0] >> 24), 255, "the ramp starts at full");
    okEq(table[255], 0, "and ends at nothing");

    bad = 0;
    for (i = 1; i < 255; i++)
        if (((unsigned int)table[i] >> 24) > ((unsigned int)table[i - 1] >> 24))
            bad++;
    okEq(bad, 0, "and never rises along the way");

    /* The other modes have no depth relation at all, which is why the
       sun samples in this one only. */
    src.b_sunEffectOn = 0;
    src.aeMaster = 8;
    DPB_Decode(&src, &st);
    okEq(st.kind, DPB_FAR, "the far blur");
    okEq((long)DPB_i_BlurAtDepth(&st, 1.0f), 0, "answers no blur at any depth");
    okEq((long)(DPB_f_DepthAtStep(&st, 100) == 0.0f), 1,
         "and no depth at any step");
}

/* ------------------------------------------------ what it declines to do */

static void caseRefusals(void)
{
    tDpb_State  st;
    tDpb_Source src;
    int         table[256];
    int         i;
    int         bad = 0;

    printf("-- what it declines to do\n");

    memset(&src, 0, sizeof(src));
    src.nearPlane = 0.1f;
    src.farPlane = 100.0f;
    src.b_sunEffectOn = 1;
    DPB_Decode(&src, &st);

    /* A missing pointer is a return, not a crash: these are called
       from two renderers now and one of them may not be the one in
       force. */
    DPB_Decode(NULL, &st);
    DPB_Decode(&src, NULL);
    DPB_BuildRamp(NULL, table);
    DPB_BuildRamp(&st, NULL);
    okEq((long)DPB_i_BlurAtDepth(NULL, 1.0f), 0, "no state, no blur");
    okEq((long)(DPB_f_DepthAtStep(NULL, 1) == 0.0f), 1, "no state, no depth");
    ok(!DPB_b_Ordered(NULL), "no state is not ordered");

    /* A mode outside the five leaves the table alone rather than
       writing something arbitrary into it. */
    memset(table, 0xCD, sizeof(table));
    st.kind = 99;
    DPB_BuildRamp(&st, table);
    for (i = 0; i < 256; i++)
        if (table[i] != (int)0xCDCDCDCD)
            bad++;
    okEq(bad, 0, "a mode it does not know leaves the table untouched");
}

int main(void)
{
    caseKind();
    caseOrdering();
    caseRampBounds();
    caseSunRamp();
    caseRefusals();

    printf("\n  %ld checks, %ld failure(s)\n", checks, fails);
    return fails ? 1 : 0;
}
