/* A unit test for aemotion.c.
 *
 * The motion smooth's decision and its four samples. Two things here
 * cannot be seen in a picture at all and are the reason the file
 * exists:
 *
 *   - **the threshold**, which is not a round number and is a strict
 *     comparison, so the movement equal to it must not smear and the
 *     one a hair above it must. Either side of that the picture looks
 *     the same;
 *   - **the axis asymmetry**: the distance that decides is measured in
 *     three dimensions and the smear uses two of them, so a camera
 *     moving straight down its own line of sight crosses the threshold
 *     and then smears by nothing. A test that moved the camera
 *     diagonally would never notice.
 *
 * What is *not* here is the rule that last frame's point is replaced
 * whether or not the smear ran. That is the caller's to get right --
 * this module stores nothing -- so it is asserted in the renderer's
 * own harnesses instead.
 */

#include "../aemotion.h"

#include <math.h>
#include <stdio.h>

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
        printf("  FAIL %s: got %ld want %ld\n", what, got, want);
        fails++;
    }
}

static void okNear(double got, double want, double tol, const char* what)
{
    checks++;
    if (!(fabs(got - want) <= tol))
    {
        printf("  FAIL %s: got %.9g want %.9g\n", what, got, want);
        fails++;
    }
}

/* A pair of points a given distance apart along one axis.
 *
 * The base is the origin, and that is not laziness. The movement is
 * measured as the difference of two points, so at a base of thirty a
 * true movement of 0.02 comes back as 0.0200004578 -- the difference
 * of two nearby floats keeps only the bits they do not share. A
 * fixture with a base away from the origin therefore cannot pin the
 * gain or the threshold at all, and a first draft of this file had one
 * and failed on both counts.
 *
 * MovedAt below keeps a base on purpose, to say that the imprecision
 * is real rather than an artefact of the test. */
static int Moved(float* dx, float* dy, int axis, float d)
{
    float now[3] = { 0.0f, 0.0f, 0.0f };
    float before[3] = { 0.0f, 0.0f, 0.0f };

    before[axis] = before[axis] - d;
    return AMO_b_Decide(dx, dy, now, before);
}

static int MovedAt(float* dx, float* dy, int axis, float d, float base)
{
    float now[3] = { base, base, base };
    float before[3] = { base, base, base };

    before[axis] = before[axis] - d;
    return AMO_b_Decide(dx, dy, now, before);
}

static void caseProbe(void)
{
    float p[3] = { 9.0f, 9.0f, 9.0f };

    printf("-- the direction whose movement is watched\n");

    AMO_Probe(p);
    okNear((double)p[0], 1.0, 1e-7, "the unit corner, across");
    okNear((double)p[1], 1.0, 1e-7, "up");
    okNear((double)p[2], 1.0, 1e-7, "and away");

    AMO_Probe(NULL);
    ok(1, "and probing nowhere is not a crash");
}

static void caseThreshold(void)
{
    float dx;
    float dy;

    printf("-- the threshold, from both sides of it\n");

    okEq((long)Moved(&dx, &dy, 0, 0.0f), 0L, "no movement, no smear");
    okEq((long)Moved(&dx, &dy, 0, 0.004f), 0L, "too little, no smear");

    /* The comparison is strict, and the pair that straddles it is
       **adjacent floats**: the threshold is a decimal double, and the
       float nearest it falls just below, so the next float up is the
       smallest movement that smears. One step of a float apart, and
       nothing about the picture distinguishes them.
     *
       Worth knowing while reading the threshold: the decimals 0.005
       and 0.0049999999 are the *same float*, so a movement given as
       either is the same movement. The odd-looking constant only looks
       odd; the comparison that uses it is in double, which is why the
       boundary is approachable at all. */
    okEq((long)Moved(&dx, &dy, 0, 0.0049999999f), 0L,
         "the float at the threshold does not smear");
    okEq((long)Moved(&dx, &dy, 0, 0.005f), 0L,
         "nor does 0.005, which is the same float as it");
    okEq((long)Moved(&dx, &dy, 0, 0.0050000004f), 1L,
         "and the next float up does");
    okEq((long)Moved(&dx, &dy, 0, 1.0f), 1L, "as does a camera cut");

    /* Backwards counts as movement: the distance is a distance, so
       the same float that smears forwards smears backwards. Written
       as the same constant rather than a round one, because a round
       one here is below the threshold and that is how this assertion
       first went in wrong. */
    okEq((long)Moved(&dx, &dy, 0, -0.0050000004f), 1L,
         "and the same movement the other way counts the same");
    okEq((long)Moved(&dx, &dy, 0, -0.005f), 0L,
         "while the float at the threshold does not, either way");

    /* A null pair is not a crash and does not smear. */
    okEq((long)AMO_b_Decide(&dx, &dy, NULL, NULL), 0L,
         "no points, no smear");

    /* And the threshold really is the written constant and not the
       round number it nearly is.
     *
       A single-axis float movement cannot land between 0.0049999999
       and 0.005 -- the two are the same float -- but a movement on two
       axes can, because the distance is a double of two float squares.
       This pair's distance is 0.0049999999011, which is over the
       written threshold and under the round one. So it smears here and
       would not if the constant were tidied, and that is the only
       input that can tell the two apart. Found by search rather than
       by hand; a sweep mutant replacing the constant survived until
       this case existed. */
    {
        float now[3] = { 0.00104500004f, 0.0f, 0.00488957809f };
        float before[3] = { 0.0f, 0.0f, 0.0f };

        okEq((long)AMO_b_Decide(&dx, &dy, now, before), 1L,
             "a movement just over the written threshold smears");
    }
}

static void caseAxes(void)
{
    float dx;
    float dy;

    printf("-- the test is in three dimensions, the smear in two\n");

    /* Across: both the test and the smear see it. The expected gain
       is against the float the movement actually is, not against the
       decimal it was written as -- see Moved. */
    dx = dy = 99.0f;
    okEq((long)Moved(&dx, &dy, 0, 0.02f), 1L, "moving across smears");
    okNear((double)dx, (double)0.02f * 50.0, 1e-6,
           "by the movement times fifty");
    okNear((double)dy, 0.0, 1e-7, "and not at all the other way");

    /* Up: the same. */
    dx = dy = 99.0f;
    okEq((long)Moved(&dx, &dy, 1, 0.02f), 1L, "moving up smears");
    okNear((double)dx, 0.0, 1e-7, "not across");
    okNear((double)dy, (double)0.02f * 50.0, 1e-6,
           "and by the movement times fifty");

    /* Away: the test sees it and the smear does not. So the effect
       runs, draws, and draws the picture it was given -- which is the
       asymmetry no diagonal test would find. */
    dx = dy = 99.0f;
    okEq((long)Moved(&dx, &dy, 2, 1.0f), 1L,
         "moving straight away still crosses the threshold");
    okNear((double)dx, 0.0, 1e-7, "and smears by nothing across");
    okNear((double)dy, 0.0, 1e-7, "nor down");

    /* Either output may be left out. */
    okEq((long)Moved(NULL, NULL, 0, 0.02f), 1L,
         "and the answer alone can be asked for");
}

static void casePrecisionFarFromTheOrigin(void)
{
    float dx;
    float dy;

    printf("-- the movement is a difference, and loses bits with"
           " distance\n");

    /* The same movement measured near the origin and far from it. Far
       out, the two points share most of their bits and the difference
       keeps only what is left -- so the camera's own idea of how far
       it moved coarsens as it travels, and with it the threshold.
     *
       This is the other renderer's behaviour too: it is a consequence
       of watching a transformed point rather than a velocity, and
       there is nothing here to fix. It is a case so that the next
       reader meets it as a fact rather than as a puzzle. */
    okEq((long)MovedAt(&dx, &dy, 0, 0.02f, 0.0f), 1L, "it smears near the"
                                                      " origin");
    okNear((double)dx, (double)0.02f * 50.0, 1e-6, "by the right amount");

    okEq((long)MovedAt(&dx, &dy, 0, 0.02f, 30.0f), 1L, "and far from it");
    ok(fabs((double)dx - (double)0.02f * 50.0) > 1e-5,
       "but by a measurably different amount");

    /* Far enough out and a movement smaller than the spacing of the
       floats there disappears entirely -- the two points come back
       equal and the camera looks still. */
    okEq((long)MovedAt(&dx, &dy, 0, 0.006f, 1.0e6f), 0L,
         "and a small movement a long way out is not seen at all");
}

static void caseTaps(void)
{
    float u[AMO_TAPS];
    float v[AMO_TAPS];
    /* The four multipliers, in binding order: three back, two
       forward, one back, and the sample itself. Neither symmetric nor
       sorted, which is the thing to pin. */
    static const double at[AMO_TAPS] = { -3.0, 2.0, -1.0, 0.0 };
    int i;

    printf("-- where the four samples sit\n");

    /* A smear of one unit, so a step is exactly the two scales and
       every tap is a clean multiple of them. */
    AMO_Taps(u, v, 1.0f, 1.0f);
    for (i = 0; i < AMO_TAPS; i++)
    {
        okNear((double)u[i], at[i] * 0.0015625, 1e-9,
               "the tap is its multiple of the step across");
        okNear((double)v[i], at[i] * 0.0020833334, 1e-9,
               "and of the step down");
    }

    /* The spread is lopsided, said directly: the furthest tap one way
       is three steps and the other way two, so the four do not average
       to the pixel they are written to. */
    okNear((double)(u[0] + u[1] + u[2] + u[3]),
           -2.0 * 0.0015625, 1e-9,
           "the four taps do not centre on the pixel");

    /* No smear, no offsets -- which is what makes a frame where the
       camera crossed the threshold only along its line of sight come
       back unchanged. */
    AMO_Taps(u, v, 0.0f, 0.0f);
    for (i = 0; i < AMO_TAPS; i++)
    {
        okNear((double)u[i], 0.0, 1e-9, "no movement, no offset across");
        okNear((double)v[i], 0.0, 1e-9, "nor down");
    }

    /* The two scales are different from one another, which is what
       makes the step a fraction of the picture rather than a count of
       pixels: a six-hundred-and-fortieth across and about a
       four-hundred-and-eightieth down. Pinned as a ratio so that
       swapping the two constants shows. */
    AMO_Taps(u, v, 1.0f, 1.0f);
    ok((double)v[0] / (double)u[0] > 1.3,
       "the step down is the larger, as the picture is the wider");

    /* And the vertical scale is the written decimal and not a
       four-hundred-and-eightieth, which takes a smear large enough for
       the two to round apart.
     *
       As floats the two constants are the *same* number, so no small
       smear can tell them apart -- but the step is a double product
       cast to a float, and at a smear of twenty-one the two products
       straddle a float boundary. The figures below are the written
       constant's; a four-hundred-and-eightieth gives -0.131249994,
       fifteen parts in a thousand million away, which is why the
       tolerance here is tighter than anywhere else in this file.
       Found by search, after a sweep mutant swapping the constant
       survived. */
    AMO_Taps(u, v, 0.0f, 21.0f);
    okNear((double)v[0], -0.13125000894069671631, 1.0e-10,
           "the vertical step is the written decimal, not a"
           " four-hundred-and-eightieth");
    okNear((double)v[1], 0.0875000059604644775, 1.0e-10,
           "at the second tap as well");

    AMO_Taps(NULL, NULL, 1.0f, 1.0f);
    ok(1, "and asking for neither is not a crash");
}

static void caseWeight(void)
{
    printf("-- what each sample weighs\n");

    /* Not a quarter. Four of these come to 252 of 255, so the smeared
       picture is about one part in eighty darker than the one it was
       made from -- small, consistent, and real. */
    okNear((double)AMO_f_Weight(), 63.0 / 255.0, 1e-7,
           "63 of 255, not a quarter");
    ok((double)AMO_f_Weight() * 4.0 < 1.0,
       "so the four together fall short of the whole picture");
    okNear((double)AMO_f_Weight() * 4.0, 252.0 / 255.0, 1e-7,
           "by three parts in 255");
}

static void caseOnePass(void)
{
    printf("-- which way the other renderer draws it\n");

    /* Two and three units take two draws; everything else takes one.
       The awkward part is the bottom: a card reporting one unit is
       given the four-sample path it cannot run. That is the other
       renderer's own test and not a misreading of it. */
    okEq((long)AMO_b_OnePass(1), 1L, "one unit takes the four-sample path");
    okEq((long)AMO_b_OnePass(2), 0L, "two units take two draws");
    okEq((long)AMO_b_OnePass(3), 0L, "and so do three");
    okEq((long)AMO_b_OnePass(4), 1L, "four take one draw");
    okEq((long)AMO_b_OnePass(8), 1L, "and so do eight");
    okEq((long)AMO_b_OnePass(0), 1L, "and a card reporting none");
}

static void caseBit(void)
{
    printf("-- the bit, which is the whole of the gate\n");

    okEq((long)AMO_b_On(AMO_BIT), 1L, "on when the bit is set");
    okEq((long)AMO_b_On(0u), 0L, "off when it is not");
    okEq((long)AMO_b_On(~AMO_BIT), 0L, "and no other bit is it");
}

int main(void)
{
    caseProbe();
    caseThreshold();
    caseAxes();
    casePrecisionFarFromTheOrigin();
    caseTaps();
    caseWeight();
    caseOnePass();
    caseBit();

    printf("  %ld checks, %ld failure(s)\n", checks, fails);
    return fails ? 1 : 0;
}
