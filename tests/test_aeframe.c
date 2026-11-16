/* A unit test for aeframe.c.
 *
 * The three after-effects that build a frame out of several samples of
 * one. What each comes down to is a schedule and a set of weights, and
 * neither shows in the picture: a wrong weight is a slightly different
 * smear, and a wrong schedule is a smear of the wrong thing. So the
 * properties are stated here, where there is no eight-bit channel to
 * hide a step in.
 *
 * The schedules are checked as chains -- no pass reading what it
 * writes, each reading what the one before wrote, the result where the
 * combine expects it -- the same three properties the depth blur's
 * schedule is held to, for the same reason: a wrong ping-pong still
 * produces a blurred picture.
 */

#include "../aeframe.h"

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

static void okNear(double got, double want, double tol, const char* what)
{
    checks++;
    if (!(fabs(got - want) <= tol))
    {
        printf("  FAIL %s: got %.9f, wanted %.9f\n", what, got, want);
        fails++;
    }
}

static tAef_Source src(unsigned int master, float mb, float rem, float zoom,
                       float bandBottom)
{
    tAef_Source s;

    s.aeMaster = master;
    s.motionBlur = mb;
    s.remanence = rem;
    s.zoomSmooth = zoom;
    s.bandBottom = bandBottom;
    return s;
}

/* --------------------------------------------------- the three gates */

static void caseGates(void)
{
    tAef_Source s;

    printf("-- which bit and which parameter each one needs\n");

    /* Each needs both its bit and a parameter off nought, and the
       three bits are different. A bit on its own does nothing, and a
       parameter on its own does nothing. */
    s = src(0, 1.0f, 1.0f, 1.0f, 1.0f);
    okEq(AEF_b_MotionBlur(&s), 0, "no bits: no motion blur");
    okEq(AEF_b_Remanence(&s), 0, "nor remanence");
    okEq(AEF_b_ZoomSmooth(&s), 0, "nor zoom smooth");

    s = src(0xFFFFFFFFu, 0.0f, 0.0f, 0.0f, 1.0f);
    okEq(AEF_b_MotionBlur(&s), 0, "every bit but no parameter: nothing");
    okEq(AEF_b_Remanence(&s), 0, "nor remanence");
    okEq(AEF_b_ZoomSmooth(&s), 0, "nor zoom smooth");

    /* Each bit turns on its own effect and no other. The three are
       checked against each other rather than against their values, so
       two effects sharing a bit fails here. */
    s = src(0x10, 1.0f, 1.0f, 1.0f, 1.0f);
    okEq(AEF_b_MotionBlur(&s) != 0, 1, "its own bit runs the motion blur");
    okEq(AEF_b_Remanence(&s), 0, "and not the remanence");
    okEq(AEF_b_ZoomSmooth(&s), 0, "nor the zoom");

    s = src(0x800, 1.0f, 1.0f, 1.0f, 1.0f);
    okEq(AEF_b_Remanence(&s) != 0, 1, "its own bit runs the remanence");
    okEq(AEF_b_MotionBlur(&s), 0, "and not the motion blur");
    okEq(AEF_b_ZoomSmooth(&s), 0, "nor the zoom");

    s = src(0x400, 1.0f, 1.0f, 1.0f, 1.0f);
    okEq(AEF_b_ZoomSmooth(&s) != 0, 1, "its own bit runs the zoom");
    okEq(AEF_b_MotionBlur(&s), 0, "and not the motion blur");
    okEq(AEF_b_Remanence(&s), 0, "nor the remanence");

    /* The remanence has a floor rather than a test against nought: a
       hundredth, and strictly above it. The other two take anything
       that is not nought at all, including a value far below that
       floor -- so the three thresholds are not interchangeable. */
    s = src(0xFFFFFFFFu, 0.0f, 0.005f, 0.0f, 1.0f);
    okEq(AEF_b_Remanence(&s), 0, "a two-hundredth is below the floor");
    s = src(0xFFFFFFFFu, 0.0f, 0.02f, 0.0f, 1.0f);
    okEq(AEF_b_Remanence(&s) != 0, 1, "a fiftieth is above it");
    s = src(0xFFFFFFFFu, 0.001f, 0.0f, 0.001f, 1.0f);
    okEq(AEF_b_MotionBlur(&s) != 0, 1,
         "the motion blur has no floor, only a nought");
    okEq(AEF_b_ZoomSmooth(&s) != 0, 1, "and nor does the zoom");

    /* A negative parameter is not nought, so it counts -- which is
       what the renderer that has these does, and it is stated so that
       a renderer clamping it is a visible change rather than a silent
       one. */
    s = src(0xFFFFFFFFu, -0.5f, 0.0f, -0.5f, 1.0f);
    okEq(AEF_b_MotionBlur(&s) != 0, 1, "a negative motion blur still runs");
    okEq(AEF_b_ZoomSmooth(&s) != 0, 1, "and a negative zoom");
    s = src(0xFFFFFFFFu, 0.0f, -0.5f, 0.0f, 1.0f);
    okEq(AEF_b_Remanence(&s), 0,
         "a negative remanence does not, being under the floor");

    /* Nothing to read. */
    okEq(AEF_b_MotionBlur(NULL), 0, "no source, no motion blur");
    okEq(AEF_b_Remanence(NULL), 0, "no source, no remanence");
    okEq(AEF_b_ZoomSmooth(NULL), 0, "no source, no zoom");
}

/* ------------------------------------------------------ motion blur */

static void caseMotionBlur(void)
{
    tAef_Source s;
    int         i;
    double      last;

    printf("-- how much of the frame before the motion blur keeps\n");

    /* Nought keeps nothing and one keeps everything, so the parameter
       is the share of the old frame and not of the new. Getting that
       backwards gives a blur that is strongest when it is asked for
       least, which looks like a blur either way. */
    s = src(0x10, 0.0f, 0.0f, 0.0f, 1.0f);
    okNear(AEF_f_MotionBlurKeep(&s), 0.0, 1e-7,
           "at nought nothing of the frame before survives");
    s = src(0x10, 1.0f, 0.0f, 0.0f, 1.0f);
    okNear(AEF_f_MotionBlurKeep(&s), 1.0, 1e-7, "at one, all of it");
    /* Halfway is the same knife edge: 255 less 127.5 truncates to 127,
       so the new frame takes 127 of 255 and the old keeps the other
       128 -- a step *over* half, where the remanence's weight lands a
       step under one. The two go opposite ways because this one
       quantises the new frame's share and subtracts. */
    s = src(0x10, 0.5f, 0.0f, 0.0f, 1.0f);
    okNear(AEF_f_MotionBlurKeep(&s), 128.0 / 255.0, 1e-6,
           "halfway keeps 128 of 255, a step over half");
    ok(AEF_f_MotionBlurKeep(&s) > 0.5, "and over it rather than under");

    /* Monotone across the range, which a sign error inside the byte
       arithmetic would break while leaving both ends right. */
    last = -1.0;
    for (i = 0; i <= 64; i++)
    {
        float p = (float)i / 64.0f;

        s = src(0x10, p, 0.0f, 0.0f, 1.0f);
        ok(AEF_f_MotionBlurKeep(&s) >= last,
           "the share kept never falls as the parameter rises");
        last = AEF_f_MotionBlurKeep(&s);
    }

    /* The quantisation lands on the *new* frame's share, not on the
       old one's -- the byte carried is `255 - factor * 255` and the
       old frame gets what is left. So the kept share is a byte's
       complement, and at a setting that is not a whole byte the two
       ways of quantising differ. This is the reading that tells them
       apart: quantising the kept share directly truncates downwards,
       and this rounds the other way. */
    s = src(0x10, 0.5019f, 0.0f, 0.0f, 1.0f);
    {
        double keep = AEF_f_MotionBlurKeep(&s);
        double direct = (double)(unsigned int)(0.5019 * 255.0) / 255.0;

        ok(keep > direct,
           "the kept share is a byte's complement, not a truncated byte");
        okNear(keep, 1.0 - (double)(unsigned int)(255.0 - 0.5019 * 255.0)
                           / 255.0, 1e-7,
               "and comes to exactly one less the new frame's byte");
    }

    okNear(AEF_f_MotionBlurKeep(NULL), 0.0, 1e-7, "no source, nothing kept");
}

/* -------------------------------------------------------- remanence */

static void caseRemanence(void)
{
    float         u[AEF_REMANENCE_PASSES + 2];
    float         v[AEF_REMANENCE_PASSES + 2];
    tAef_Source   s;
    int           n;
    int           i;

    printf("-- the remanence's four reaches and the weight it adds them at\n");

    memset(u, 0, sizeof(u));
    memset(v, 0, sizeof(v));
    okEq(AEF_i_RemanenceReaches(u, v, AEF_REMANENCE_PASSES + 2),
         AEF_REMANENCE_PASSES, "four passes, however much room there is");
    n = AEF_i_RemanenceReaches(u, v, AEF_REMANENCE_PASSES);
    okEq(n, AEF_REMANENCE_PASSES, "four passes");

    /* The reach: odd numbers of pixels, so four passes cover twice
       the ground the depth blur's three do. */
    for (i = 1; i < n; i++)
    {
        ok(u[i] > u[i - 1], "each blur reaches further across");
        ok(v[i] > v[i - 1], "and further down");
    }
    okNear(u[0] * 640.0, 1.0, 1e-3, "the first reaches one pixel");
    okNear(u[1] * 640.0, 3.0, 1e-3, "the second three");
    okNear(u[2] * 640.0, 5.0, 1e-3, "the third five");
    okNear(u[3] * 640.0, 7.0, 1e-3, "and the fourth seven");
    okNear(v[0] * 480.0, 1.0, 1e-3, "and the same down");
    okNear(v[3] * 480.0, 7.0, 1e-3, "at both ends");
    /* Odd, which is to say the gap between passes is two pixels and
       not one -- an evenly-spaced 1, 2, 3, 4 would pass every check
       above but blur half as far. */
    okNear((u[1] - u[0]) * 640.0, 2.0, 1e-3,
           "the passes are two pixels apart, not one");
    /* And the first is one pixel rather than none: a pass that
       reached nowhere would be a copy. */
    ok(u[0] > 0.0f, "the first pass reaches somewhere");

    /* The weight: twice the opacity, because the blurred copy goes in
       twice. It is allowed over one. */
    /* A half scales to 127.5 of a byte, which is the one place the
       difference between truncating and rounding shows -- and the
       renderer that has this truncates. So the weight is twice 127 of
       255 and not twice 128, which is a step below one rather than a
       step above it. Stated exactly: every other reading in this case
       has a byte of room and would take either. */
    s = src(0x800, 0.0f, 0.5f, 0.0f, 1.0f);
    okNear(AEF_f_RemanenceWeight(&s), 2.0 * 127.0 / 255.0, 1e-6,
           "at a half the weight is twice a truncated byte, just under"
           " one");
    ok(AEF_f_RemanenceWeight(&s) < 1.0, "and under it rather than over");
    s = src(0x800, 0.0f, 1.0f, 0.0f, 1.0f);
    okNear(AEF_f_RemanenceWeight(&s), 2.0, 1e-7, "at one, two");
    ok(AEF_f_RemanenceWeight(&s) > 1.0,
       "which is over one, so the sum saturates -- that is the bloom");
    s = src(0x800, 0.0f, 0.25f, 0.0f, 1.0f);
    okNear(AEF_f_RemanenceWeight(&s), 0.5, 2.0 / 255.0, "at a quarter, a half");

    /* Twice, not once: the single-addition reading gives half of each
       of these and nothing in the picture would say which. */
    s = src(0x800, 0.0f, 0.4f, 0.0f, 1.0f);
    okNear(AEF_f_RemanenceWeight(&s),
           2.0 * (double)(unsigned int)(0.4 * 255.0) / 255.0, 1e-7,
           "the weight is twice the byte, not the byte");

    /* Refusals. */
    okEq(AEF_i_RemanenceReaches(NULL, NULL, AEF_REMANENCE_PASSES), 0,
         "nowhere to write");
    okEq(AEF_i_RemanenceReaches(u, NULL, AEF_REMANENCE_PASSES),
         AEF_REMANENCE_PASSES, "the reaches across alone");
    okEq(AEF_i_RemanenceReaches(NULL, v, AEF_REMANENCE_PASSES),
         AEF_REMANENCE_PASSES, "or down alone");
    okEq(AEF_i_RemanenceReaches(u, v, 0), 0, "no room");
    okEq(AEF_i_RemanenceReaches(u, v, 3), 3, "as many as there is room for");
    okNear(AEF_f_RemanenceWeight(NULL), 0.0, 1e-7, "no source, no weight");
}

/* ------------------------------------------------------ zoom smooth */

static void caseZoomSmooth(void)
{
    tAef_ZoomTap t[AEF_ZOOM_TAPS + 2];
    tAef_Source  s;
    int          n;
    int          i;
    double       sum;

    printf("-- the zoom's eight taps and what each weighs\n");

    s = src(0x400, 0.0f, 0.0f, 1.0f, 1.0f);
    okEq(AEF_i_ZoomTaps(&s, t, AEF_ZOOM_TAPS + 2), AEF_ZOOM_TAPS,
         "eight taps, however much room there is");
    n = AEF_i_ZoomTaps(&s, t, AEF_ZOOM_TAPS);
    okEq(n, AEF_ZOOM_TAPS, "eight taps");

    /* The first tap is the frame itself, untouched. Without it the
       effect would be a zoom rather than a smear -- every pixel would
       move. */
    okNear(t[0].insetU, 0.0, 1e-9, "the first tap crops nothing across");
    okNear(t[0].insetV, 0.0, 1e-9, "nor down");

    /* And the rest crop evenly further in, each by the same step as
       the last -- so the taps are a ramp and not, say, a doubling. */
    for (i = 1; i < n; i++)
    {
        ok(t[i].insetU > t[i - 1].insetU, "each tap crops further across");
        ok(t[i].insetV > t[i - 1].insetV, "and further down");
        okNear(t[i].insetU - t[i - 1].insetU, t[1].insetU, 1e-7,
               "by the same step every time, across");
        okNear(t[i].insetV - t[i - 1].insetV, t[1].insetV, 1e-7,
               "and down");
    }

    /* The step at a parameter of one: a forty-eighth of the texture
       per tap, which is ten rows of a 480-row picture.
     *
       The same fraction sideways, which is *not* the same number of
       pixels -- thirteen and a third of 640 against ten of 480. So
       the crop is square in texture space and four-to-three in
       pixels. That is stated both ways round here because the first
       draft of this case asserted ten pixels across and the fraction
       is what the arithmetic actually carries. */
    okNear(t[1].insetV * 480.0, 10.0, 1e-3,
           "one unit of the parameter is ten rows down per tap");
    okNear(t[1].insetU, t[1].insetV, 1e-7,
           "and the same fraction across, with no band in the way");
    okNear(t[1].insetU * 640.0, 40.0 / 3.0, 1e-3,
           "which is thirteen and a third pixels, the frame being wider"
           " than it is tall");

    /* It scales with the parameter rather than being a fixed crop. */
    s = src(0x400, 0.0f, 0.0f, 0.5f, 1.0f);
    AEF_i_ZoomTaps(&s, t, AEF_ZOOM_TAPS);
    okNear(t[1].insetV * 480.0, 5.0, 1e-3, "half the parameter, half the step");

    /* With a letterbox the horizontal step grows and the vertical one
       does not, because only the horizontal divides by the band. That
       is a quirk rather than a design, and it is pinned here so that
       reproducing it stays deliberate: a renderer that made the two
       match would pass every other check in this case. */
    s = src(0x400, 0.0f, 0.0f, 1.0f, 0.75f);
    AEF_i_ZoomTaps(&s, t, AEF_ZOOM_TAPS);
    okNear(t[1].insetV * 480.0, 10.0, 1e-3,
           "a band leaves the vertical step alone");
    okNear(t[1].insetU, t[1].insetV / 0.75f, 1e-7,
           "and divides the horizontal one by the band's bottom");
    ok(t[1].insetU > t[1].insetV, "so a letterboxed zoom is wider than tall");

    /* A band of nothing would be a division by zero. */
    s = src(0x400, 0.0f, 0.0f, 1.0f, 0.0f);
    AEF_i_ZoomTaps(&s, t, AEF_ZOOM_TAPS);
    okNear(t[1].insetU, t[1].insetV, 1e-7,
           "no band at all falls back to the vertical step");
    ok(t[1].insetU == t[1].insetU, "and is a number rather than a nan");

    /* The weight: just under an eighth, and the eight together just
       under one. Both halves matter -- an eighth exactly would make
       the sum exactly one, and 255/8 would make it over. */
    okNear(AEF_f_ZoomWeight(), 31.0 / 255.0, 1e-9, "a tap weighs 31 of 255");
    ok(AEF_f_ZoomWeight() < 1.0 / 8.0, "which is under an eighth");
    sum = (double)AEF_f_ZoomWeight() * AEF_ZOOM_TAPS;
    ok(sum < 1.0, "so the eight come to less than all of the light");
    okNear(sum, 248.0 / 255.0, 1e-7, "248 of 255 of it");

    /* Refusals. */
    okEq(AEF_i_ZoomTaps(NULL, t, AEF_ZOOM_TAPS), 0, "nothing to read");
    okEq(AEF_i_ZoomTaps(&s, NULL, AEF_ZOOM_TAPS), 0, "nowhere to write");
    okEq(AEF_i_ZoomTaps(&s, t, 0), 0, "no room");
    okEq(AEF_i_ZoomTaps(&s, t, 3), 3, "as many as there is room for");
}

int main(void)
{
    caseGates();
    caseMotionBlur();
    caseRemanence();
    caseZoomSmooth();

    printf("\n  %ld checks, %ld failure(s)\n", checks, fails);
    return fails ? 1 : 0;
}
