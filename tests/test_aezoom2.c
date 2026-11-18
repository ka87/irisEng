/* A unit test for aftereffects/aezoom2.c.
 *
 * The second zoom smooth: eight samples of the frame, each turned
 * about the middle of the picture, summed.
 *
 * The case the file exists for is caseTransform. The effect's
 * transform is written as a rotation and **is not one** -- its second
 * row reads the first row's answer -- so its determinant is cos
 * squared and every sample is turned *and shrunk*. A tidied rotation
 * would pass every other test here and be a different effect with the
 * wrong name, so the determinant is asserted directly and the map is
 * pinned against points.
 *
 * Every expected number below came from the other renderer's own
 * expressions before they were moved out, not from this module.
 */

#include "../aftereffects/aezoom2.h"

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
        printf("  FAIL %s: got %ld want %ld\n", what, got, want);
        fails++;
    }
}

static void okNear(double got, double want, double tol, const char* what)
{
    checks++;
    if (!(fabs(got - want) <= tol))
    {
        printf("  FAIL %s: got %.17g want %.17g\n", what, got, want);
        fails++;
    }
}

static void caseGate(void)
{
    printf("-- the gate: a bit and a turn that is not nothing\n");

    okEq((long)AZ2_b_On(AZ2_BIT, 1.0f), 1L, "the bit and a turn run it");
    okEq((long)AZ2_b_On(AZ2_BIT, 0.0f), 0L, "no turn does not");
    okEq((long)AZ2_b_On(AZ2_BIT, -2.0f), 1L,
         "and a turn the other way does, the gate being a comparison"
         " against nought rather than a floor");
    okEq((long)AZ2_b_On(0u, 1.0f), 0L, "without the bit, nothing");
    okEq((long)AZ2_b_On(~AZ2_BIT, 1.0f), 0L, "and no other bit is it");
}

static void caseSplit(void)
{
    /* stages a card offers, the engine's setting, and what comes out.
       The awkward row is three stages with the setting at eight: the
       eight samples do not divide by three, so it takes **six** and
       the weight changes with it. */
    static const struct { int cardStages; int eng; int stages; int samples; }
    table[] = {
        { 1, 2, 1, 8 }, { 2, 2, 2, 8 }, { 3, 2, 2, 8 },
        { 4, 2, 2, 8 }, { 8, 2, 2, 8 },
        { 1, 8, 1, 8 }, { 2, 8, 2, 8 }, { 3, 8, 3, 6 },
        { 4, 8, 4, 8 }, { 8, 8, 4, 8 }
    };
    size_t i;

    printf("-- how the samples are split between stages and draws\n");

    for (i = 0; i < sizeof(table) / sizeof(table[0]); i++)
    {
        int stages = AZ2_i_Stages(table[i].cardStages, table[i].eng);

        okEq((long)stages, (long)table[i].stages, "the stage count");
        okEq((long)AZ2_i_Samples(stages), (long)table[i].samples,
             "and the sample count");
        okEq((long)(AZ2_i_Passes(stages) * stages),
             (long)table[i].samples, "with the draws covering them");
    }

    /* The clamp at four is what makes a card with eight stages behave
       like one with four, and the setting is what makes a card with
       four behave like one with two. Both directions matter and
       neither is the card alone. */
    okEq((long)AZ2_i_Stages(8, 8), 4L, "eight stages are clamped to four");
    okEq((long)AZ2_i_Stages(4, 2), 2L, "and the setting can ask for two");
}

static void caseWeight(void)
{
    printf("-- what each sample weighs\n");

    /* 256 over the count, less one, out of 255 -- the same deliberate
       undershoot the first zoom smooth carries. Eight samples come to
       248 of 255, so the smear is a shade dark rather than
       saturating. */
    okNear((double)AZ2_f_Weight(8), 31.0 / 255.0, 1e-7,
           "eight samples weigh 31 of 255 each");
    okNear((double)AZ2_f_Weight(8) * 8.0, 248.0 / 255.0, 1e-7,
           "and come to 248 of 255 together");
    ok((double)AZ2_f_Weight(8) * 8.0 < 1.0, "which is short of the whole");
    /* And six, which is what a card with exactly three stages gets. */
    okNear((double)AZ2_f_Weight(6), 41.0 / 255.0, 1e-7,
           "six samples weigh 41 of 255 each");
    okEq((long)(AZ2_f_Weight(0) == 0.0f), 1L, "and none weigh nothing");
}

static void caseAngles(void)
{
    float out[AZ2_TAPS * AZ2_ANGLE_SLOTS];
    int   n;

    printf("-- the angle walk, four slots to a draw\n");

    /* Four stages in two draws, at a turn of one. The four numbers of
       a draw are all written, including the ones a draw with fewer
       stages would not sample -- see the header, they reach the vertex
       data either way. */
    n = AZ2_i_Angles(out, AZ2_TAPS * AZ2_ANGLE_SLOTS, 1.0f, 4);
    okEq((long)n, 8L, "two draws of four slots");
    okNear((double)out[0], -0.15999999642372131, 1e-17, "draw nought slot 0");
    okNear((double)out[1], -0.11999999731779099, 1e-17, "slot 1");
    okNear((double)out[2], -0.079999998211860657, 1e-17, "slot 2");
    okNear((double)out[3], -0.039999999105930328, 1e-17, "slot 3");
    okNear((double)out[4], 0.0, 1e-17, "draw one slot 0");
    okNear((double)out[5], 0.039999999105930328, 1e-17, "slot 1");
    okNear((double)out[6], 0.079999998211860657, 1e-17, "slot 2");
    okNear((double)out[7], 0.11999999731779099, 1e-17, "slot 3");

    /* The span is not centred: four steps back and three on. An even
       number of samples cannot straddle nought evenly, and this is
       which way the odd one falls. */
    ok(out[0] < 0.0f && out[7] > 0.0f, "the span straddles no turn");
    okNear((double)(-out[0]) / (double)out[7], 4.0 / 3.0, 1e-5,
           "with one more sample behind than ahead");

    /* Two stages in four draws: sixteen slots, and the first two of
       each four are the sampled ones. */
    n = AZ2_i_Angles(out, AZ2_TAPS * AZ2_ANGLE_SLOTS, 1.0f, 2);
    okEq((long)n, 16L, "four draws of four slots");
    okNear((double)out[4], -0.079999998211860657, 1e-17,
           "draw one starts two steps on");
    okNear((double)out[12], 0.079999998211860657, 1e-17,
           "and draw three six steps on");

    /* A turn the other way runs the samples backwards. */
    n = AZ2_i_Angles(out, AZ2_TAPS * AZ2_ANGLE_SLOTS, -2.0f, 4);
    okEq((long)n, 8L, "a turn the other way still fills two draws");
    okNear((double)out[0], 0.31999999284744263, 1e-17,
           "and starts four steps the other side");
    okNear((double)out[7], -0.23999999463558197, 1e-17, "ending the other");

    /* A room-bounded caller gets what fits, and is told. */
    okEq((long)AZ2_i_Angles(out, 3, 1.0f, 4), 3L, "three asked for");
    okEq((long)AZ2_i_Angles(out, 0, 1.0f, 4), 0L, "none asked for");
    okEq((long)AZ2_i_Angles(NULL, 8, 1.0f, 4), 0L, "nowhere to put them");
}

static void caseSampledAngles(void)
{
    float all4[AZ2_TAPS];
    float all2[AZ2_TAPS];
    int   i;
    int   differing;

    printf("-- the sampled angles, and where the two splits part\n");

    okEq((long)AZ2_i_SampledAngles(all4, AZ2_TAPS, 1.0f, 4), 8L,
         "four stages give eight sampled angles");
    okEq((long)AZ2_i_SampledAngles(all2, AZ2_TAPS, 1.0f, 2), 8L,
         "and so do two");

    /* At a turn of one the two splits sample the *same* eight angles,
       bit for bit -- which is worth asserting because it is the
       obvious assumption and it is only sometimes true. */
    okEq((long)memcmp(all4, all2, sizeof(all4)), 0L,
         "at a turn of one the two splits agree exactly");

    /* And over forty thousand turns they disagree at about two in
     * three -- always at the fourth sample, and always by one step of
     * a float.
     *
     * That is the reason the split is asked for rather than assumed:
     * the fourth sample is the one computed as three steps added to
     * the draw's angle in one split and as the next draw's angle plus
     * a step in the other, and those two round differently. Measured
     * rather than argued -- a first reading of this file assumed the
     * splits always agreed, and a turn of one is one of the cases
     * where they happen to. */
    differing = 0;
    for (i = -20000; i <= 20000; i++)
    {
        float zoom = (float)i * 0.001f;
        int   j;

        if (AZ2_i_SampledAngles(all4, AZ2_TAPS, zoom, 4) != 8
            || AZ2_i_SampledAngles(all2, AZ2_TAPS, zoom, 2) != 8)
        {
            ok(0, "both splits give eight at every turn");
            return;
        }
        for (j = 0; j < AZ2_TAPS; j++)
            if (all4[j] != all2[j])
            {
                /* Only ever the fourth. */
                if (j != 3)
                {
                    ok(0, "the splits differ only at the fourth sample");
                    return;
                }
                differing++;
                break;
            }
    }
    ok(differing > 20000 && differing < 30000,
       "the two splits part at about two turns in three");

    /* One of them, stated outright so that the sweep has something to
       bite on. */
    AZ2_i_SampledAngles(all4, AZ2_TAPS, -19.999f, 4);
    AZ2_i_SampledAngles(all2, AZ2_TAPS, -19.999f, 2);
    okNear((double)all4[3], 0.79996013641357422, 1e-17,
           "four stages at that turn");
    okNear((double)all2[3], 0.79996001720428467, 1e-17,
           "two stages at the same turn, a step of a float away");
    ok(all4[3] != all2[3], "and they really are different numbers");

    /* Three stages sample six and not eight. */
    okEq((long)AZ2_i_SampledAngles(all4, AZ2_TAPS, 1.0f, 3), 6L,
         "three stages sample six");
}

static void caseTransform(void)
{
    static const float angles[3] = { 0.16f, -0.32f, 0.0f };
    float              m[4];
    int                i;

    printf("-- the transform, which is not the rotation it is written"
           " as\n");

    /* The map, pinned. Already conjugated by the frame's aspect and
       already with the vertical measured up the picture, so these are
       not the cosines and sines they look like. */
    AZ2_Tap(m, 0.16f);
    okNear((double)m[0], 0.98722726106643677, 1e-15, "the map's a");
    okNear((double)m[1], -0.11948864907026291, 1e-15, "b");
    okNear((double)m[2], 0.20971103012561798, 1e-15, "c");
    okNear((double)m[3], 0.96184498071670532, 1e-15, "d");

    AZ2_Tap(m, -0.32f);
    okNear((double)m[0], 0.94923543930053711, 1e-15, "a, turned back");
    okNear((double)m[1], 0.23592491447925568, 1e-15, "b");
    okNear((double)m[2], -0.39813029766082764, 1e-15, "c");
    okNear((double)m[3], 0.85028332471847534, 1e-15, "d");

    /* No turn is the identity, which is what makes a parameter of
       nought a no-op and is why the gate can be a plain comparison. */
    AZ2_Tap(m, 0.0f);
    okNear((double)m[0], 1.0, 1e-9, "no turn leaves a at one");
    okNear((double)fabs((double)m[1]), 0.0, 1e-9, "b at nothing");
    okNear((double)fabs((double)m[2]), 0.0, 1e-9, "c at nothing");
    okNear((double)m[3], 1.0, 1e-9, "and d at one");

    /* **The determinant is cos squared and not one.** This is the
       finding: a rotation preserves area, and this does not -- it
       shrinks by more the further it turns, which is what makes a
       rotational smear look like a zoom and is why the effect carries
       the name it does. Asserted at several angles, and against the
       cosine rather than against a number, so that a tidied rotation
       cannot pass. */
    for (i = 0; i < 3; i++)
    {
        double c = cos((double)angles[i]);
        double det;

        AZ2_Tap(m, angles[i]);
        det = (double)m[0] * (double)m[3] - (double)m[1] * (double)m[2];
        okNear(det, c * c, 1e-6, "the determinant is cos squared");
        okNear((double)AZ2_f_TapScale(angles[i]), c * c, 1e-6,
               "and the module says so too");
    }
    /* Which is to say it is less than one wherever there is a turn at
       all -- so a tidied rotation is distinguishable from this by one
       reading. */
    ok((double)AZ2_f_TapScale(0.32f) < 0.95,
       "a third of a radian shrinks by five parts in a hundred");
    okNear((double)AZ2_f_TapScale(0.0f), 1.0, 1e-9,
           "and no turn shrinks by nothing");

    /* The off-diagonal terms, which carry the same finding from
     * another direction.
     *
     * A rotation's two off-diagonal entries are each other's
     * negatives, so after the frame's aspect is conjugated in their
     * ratio would be exactly the aspect squared -- sixteen ninths.
     * This map's are **not**: its lower entry is minus cos times sin
     * where a rotation's would be minus sin, so the ratio comes out
     * the aspect squared times **cos**.
     *
     * So the departure from sixteen ninths is itself a measure of the
     * bend, and a tidied rotation would land on sixteen ninths exactly.
     * A first draft of this case asserted that it would, and the
     * reading is what corrected it. */
    AZ2_Tap(m, 0.16f);
    okNear((double)m[2] / (double)(-m[1]),
           (640.0 / 480.0) * (640.0 / 480.0) * cos(0.16), 1e-5,
           "the off-diagonal terms differ by the aspect squared times"
           " the cosine");
    ok((double)m[2] / (double)(-m[1])
       < (640.0 / 480.0) * (640.0 / 480.0) - 0.01,
       "which is measurably short of what a rotation would give");

    AZ2_Tap(NULL, 0.16f);
    ok(1, "and asking for nowhere is not a crash");
}

int main(void)
{
    caseGate();
    caseSplit();
    caseWeight();
    caseAngles();
    caseSampledAngles();
    caseTransform();

    printf("  %ld checks, %ld failure(s)\n", checks, fails);
    return fails ? 1 : 0;
}
