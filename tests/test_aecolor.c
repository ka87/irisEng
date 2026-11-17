/* A unit test for aecolor.c.
 *
 * The three colour effects are each a gain, a bias and -- for the
 * grayscale -- three weights, worked out from what the renderer that
 * has them computes rather than chosen. So the numbers are the thing
 * to test, and the way to test a number that was derived is to derive
 * it the other way and see whether the two agree.
 *
 * That is what most of this file does. The grayscale is checked
 * against the two-stage form it came from, evaluated step by step;
 * the contrast is checked against the one formula both of its halves
 * collapse to; and the brightness is checked at the places its two
 * halves meet. A constant copied out of aecolor.c and compared with
 * itself would pass whatever either of them said.
 */

#include "../aecolor.h"

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

/* A step applied to one channel, the way a renderer has to apply it. */
static double apply(const tAec_Step* s, double r, double g, double b,
                    int channel)
{
    double v;

    if (s->kind == AEC_GRAYSCALE)
        v = (r * s->weights[0] + g * s->weights[1] + b * s->weights[2])
            * s->gain + s->bias;
    else
    {
        double in = channel == 0 ? r : (channel == 1 ? g : b);

        v = in * s->gain + s->bias;
    }
    if (v < 0.0)
        v = 0.0;
    if (v > 1.0)
        v = 1.0;
    return v;
}

static tAec_Source src(unsigned int master, float brightness, float contrast)
{
    tAec_Source s;

    s.aeMaster = master;
    s.brightness = brightness;
    s.contrast = contrast;
    /* The whole of the grayscale, which is what every case below was
       written against. The amount has a section of its own. */
    s.grayscale = 1.0f;
    return s;
}

static tAec_Source srcGray(float amount)
{
    tAec_Source s = src(1u, 0.5f, 0.5f);

    s.grayscale = amount;
    return s;
}

/* ------------------------------------------------------- the amount */

/* The grayscale is the one of the three with an amount of its own,
   and its bit is not the whole of its gate. The amount reaches the
   hardware as a vertex alpha -- a byte, truncated -- so the number
   here is quantised rather than the float that came in. */
static void caseGrayAmount(void)
{
    tAec_Step   st[AEC_MAX_STEPS];
    tAec_Source s;

    printf("-- how much of the grayscale\n");

    s = srcGray(0.0f);
    okEq(AEC_i_Steps(&s, st, AEC_MAX_STEPS), 0,
         "the bit set and no amount is no grayscale, not all of it");

    s = srcGray(1.0f);
    okEq(AEC_i_Steps(&s, st, AEC_MAX_STEPS), 1, "all of it is one step");
    okEq(st[0].kind, AEC_GRAYSCALE, "and it is the grayscale");
    okNear(st[0].blend, 1.0f, 1e-6, "which replaces the picture outright");

    s = srcGray(0.4f);
    okEq(AEC_i_Steps(&s, st, AEC_MAX_STEPS), 1, "and so is part of it");
    okNear(st[0].blend, 102.0f / 255.0f, 1e-6,
           "four tenths is 102 of 255, the byte the hardware carries");

    /* 0.4009 * 255 is 102.2, which rounds to 102 either way; 0.4019 is
       102.5 and separates the two. */
    s = srcGray(0.4019608f);
    okNear(st[0].blend, 102.0f / 255.0f, 1e-6, "and a half step is truncated, not rounded");
    okEq(AEC_i_Steps(&s, st, AEC_MAX_STEPS), 1, "still one step");
    okNear(st[0].blend, 102.0f / 255.0f, 1e-6, "still truncated");

    s = srcGray(2.0f);
    okEq(AEC_i_Steps(&s, st, AEC_MAX_STEPS), 1, "more than all of it is a step");
    okNear(st[0].blend, 1.0f, 1e-6, "and is all of it");

    /* The weights do not move with the amount: the amount mixes the
       result back, it does not weaken the sum. */
    s = srcGray(0.4f);
    AEC_i_Steps(&s, st, AEC_MAX_STEPS);
    {
        tAec_Step   full[AEC_MAX_STEPS];
        tAec_Source f = srcGray(1.0f);

        AEC_i_Steps(&f, full, AEC_MAX_STEPS);
        okNear(st[0].gain, full[0].gain, 1e-6, "the gain is the amount's business, not the sum's");
        okNear(st[0].weights[1], full[0].weights[1], 1e-6, "nor the weights'");
    }

    /* The other two have no amount, and say so rather than leaving it
       at zero -- a step with no blend would draw nothing. */
    s = src(0u, 0.7f, 0.5f);
    okEq(AEC_i_Steps(&s, st, AEC_MAX_STEPS), 1, "a brightness on its own");
    okNear(st[0].blend, 1.0f, 1e-6, "replaces the picture outright");
    s = src(0u, 0.5f, 0.8f);
    okEq(AEC_i_Steps(&s, st, AEC_MAX_STEPS), 1, "and a contrast");
    okNear(st[0].blend, 1.0f, 1e-6, "likewise");
}

/* ------------------------------------------------------- which apply */

static void caseWhichApply(void)
{
    tAec_Step   st[AEC_MAX_STEPS];
    tAec_Source s;

    printf("-- which of the three apply, and in what order\n");

    s = src(0, 0.5f, 0.5f);
    okEq(AEC_i_Steps(&s, st, AEC_MAX_STEPS), 0,
         "both parameters neutral and no bit: nothing to do");

    /* Each on its own. */
    s = src(0, 0.6f, 0.5f);
    okEq(AEC_i_Steps(&s, st, AEC_MAX_STEPS), 1, "brightness alone");
    okEq(st[0].kind, AEC_BRIGHTNESS, "and it is the brightness");
    s = src(0, 0.5f, 0.6f);
    okEq(AEC_i_Steps(&s, st, AEC_MAX_STEPS), 1, "contrast alone");
    okEq(st[0].kind, AEC_CONTRAST, "and it is the contrast");
    s = src(1, 0.5f, 0.5f);
    okEq(AEC_i_Steps(&s, st, AEC_MAX_STEPS), 1, "the grayscale alone");
    okEq(st[0].kind, AEC_GRAYSCALE, "and it is the grayscale");

    /* Either side of neutral counts, so a parameter below a half is
       not mistaken for one that was never set. */
    s = src(0, 0.4f, 0.4f);
    okEq(AEC_i_Steps(&s, st, AEC_MAX_STEPS), 2,
         "both parameters below neutral still apply");

    /* All three, in the order the chain runs them. The order is part
       of the answer: with a clamp between the steps, brightening then
       stretching is not stretching then brightening. */
    s = src(1, 0.7f, 0.3f);
    okEq(AEC_i_Steps(&s, st, AEC_MAX_STEPS), 3, "all three");
    okEq(st[0].kind, AEC_BRIGHTNESS, "brightness first");
    okEq(st[1].kind, AEC_CONTRAST, "contrast second");
    okEq(st[2].kind, AEC_GRAYSCALE, "grayscale last");

    /* Only the one bit turns the grayscale on. */
    s = src(0xFFFFFFFEu, 0.5f, 0.5f);
    okEq(AEC_i_Steps(&s, st, AEC_MAX_STEPS), 0,
         "every bit but the first leaves the grayscale off");

    /* Refusals. */
    s = src(1, 0.7f, 0.3f);
    okEq(AEC_i_Steps(&s, st, 0), 0, "no room");
    okEq(AEC_i_Steps(&s, st, -1), 0, "less than no room");
    okEq(AEC_i_Steps(&s, NULL, AEC_MAX_STEPS), 0, "nowhere to write");
    okEq(AEC_i_Steps(NULL, st, AEC_MAX_STEPS), 0, "nothing to read");
    okEq(AEC_i_Steps(&s, st, 2), 2, "as many as there is room for");
    okEq(st[0].kind, AEC_BRIGHTNESS, "and they are the first two");
    okEq(st[1].kind, AEC_CONTRAST, "in order");
}

/* ------------------------------------------------------- brightness */

static void caseBrightness(void)
{
    tAec_Step   st[AEC_MAX_STEPS];
    tAec_Source s;
    int         i;

    printf("-- the brightness, both directions\n");

    /* A gain of one, always: it adds and takes away, it does not
       scale. A gain of anything else would also change the contrast,
       which is the next effect's job. */
    for (i = 0; i <= 20; i++)
    {
        float p = (float)i / 20.0f;

        if (p == 0.5f)
            continue;
        s = src(0, p, 0.5f);
        AEC_i_Steps(&s, st, AEC_MAX_STEPS);
        okNear(st[0].gain, 1.0, 1e-7, "the brightness never scales");
    }

    /* Above neutral it adds and below it takes away, by the distance
       from neutral. The two are checked as a pair at the same
       distance, so a sign applied to the wrong half shows up as the
       two no longer mirroring. */
    s = src(0, 0.75f, 0.5f);
    AEC_i_Steps(&s, st, AEC_MAX_STEPS);
    ok(st[0].bias > 0.0f, "above neutral it adds");
    okNear(st[0].bias, 0.25, 0.004, "a quarter of the way up, a quarter");
    {
        float up = st[0].bias;

        s = src(0, 0.25f, 0.5f);
        AEC_i_Steps(&s, st, AEC_MAX_STEPS);
        ok(st[0].bias < 0.0f, "below neutral it takes away");
        okNear(st[0].bias, -up, 1e-7,
               "and by the same amount at the same distance");
    }

    /* The far ends. A half either way is the most there is. */
    s = src(0, 1.0f, 0.5f);
    AEC_i_Steps(&s, st, AEC_MAX_STEPS);
    okNear(st[0].bias, 0.5, 0.004, "all the way up is half a channel");
    s = src(0, 0.0f, 0.5f);
    AEC_i_Steps(&s, st, AEC_MAX_STEPS);
    okNear(st[0].bias, -0.5, 0.004, "and all the way down");

    /* Quantised to a byte and truncated, not rounded: a parameter a
       whisker above neutral asks for a fraction of a byte and gets
       none of it. That is the renderer's own arithmetic and it is
       what keeps a nearly-neutral setting from showing a step. */
    s = src(0, 0.5019f, 0.5f);
    AEC_i_Steps(&s, st, AEC_MAX_STEPS);
    okEq((long)(st[0].bias == 0.0f), 1,
         "a fraction of a byte above neutral rounds down to nothing");
    s = src(0, 0.5059f, 0.5f);
    AEC_i_Steps(&s, st, AEC_MAX_STEPS);
    okNear(st[0].bias, 1.0 / 255.0, 1e-7, "and a whole byte is one byte");
}

/* --------------------------------------------------------- contrast */

static void caseContrast(void)
{
    tAec_Step   st[AEC_MAX_STEPS];
    tAec_Source s;
    double      mid = AEC_f_MidGrey();
    int         i;

    printf("-- the contrast, and the one formula both halves are\n");

    /* Neutral is not reachable through AEC_i_Steps -- it is the one
       value that means "do not run" -- so the two ends are checked
       approaching it. Just below neutral the gain is nearly one and
       the bias nearly nothing. */
    s = src(0, 0.5f, 0.499f);
    AEC_i_Steps(&s, st, AEC_MAX_STEPS);
    okNear(st[0].gain, 1.0, 0.005, "just under neutral, a gain of one");
    okNear(st[0].bias, 0.0, 0.005, "and no bias");
    s = src(0, 0.5f, 0.501f);
    AEC_i_Steps(&s, st, AEC_MAX_STEPS);
    okNear(st[0].gain, 1.0, 0.005, "just over neutral, the same");
    okNear(st[0].bias, 0.0, 0.005, "and the same");

    /* Nought is a flat mid grey: no gain at all and the bias carrying
       the whole picture. */
    s = src(0, 0.5f, 0.0f);
    AEC_i_Steps(&s, st, AEC_MAX_STEPS);
    okNear(st[0].gain, 0.0, 1e-7, "at nought the frame is thrown away");
    okNear(st[0].bias, mid, 1e-7, "and replaced by mid grey");
    okNear(apply(&st[0], 0.0, 0.0, 0.0, 0), mid, 1e-7, "black becomes grey");
    okNear(apply(&st[0], 1.0, 1.0, 1.0, 0), mid, 1e-7, "and so does white");

    /* One is twice the contrast about mid grey: black and white stay
       where they are and mid grey does too. */
    s = src(0, 0.5f, 1.0f);
    AEC_i_Steps(&s, st, AEC_MAX_STEPS);
    okNear(st[0].gain, 2.0, 0.005, "at one the gain is two");
    okNear(apply(&st[0], mid, mid, mid, 0), mid, 0.005,
           "mid grey is where it was");
    okNear(apply(&st[0], 0.0, 0.0, 0.0, 0), 0.0, 1e-7, "black is still black");
    okNear(apply(&st[0], 1.0, 1.0, 1.0, 0), 1.0, 1e-7, "white still white");

    /* And the claim the two halves make together: whichever side of
       neutral the parameter is on, the step is a gain of twice it
       about mid grey.
     *
       How close it comes is the interesting part, and the two halves
       differ. Both truncate their numbers to a byte, and the upper
       half truncates two of them -- the gain and the offset -- with
       the offset's loss going straight to the output. So the two
       bounds below are different on purpose, and they are tight: a
       whole byte for the upper half and half of one for the lower.
       Loosening either hides the thing being measured, and the
       looser bound applied to both halves would let the lower one
       drift a byte without anything saying so.
     *
       The bound is on the size of the miss and not on its direction,
       and that is deliberate: both halves truncate their numbers
       downwards, but a smaller gain moves a pixel *towards* the
       pivot, so the output lands above the formula below mid grey
       and below it above. A first draft of this asserted the
       direction and was wrong in five places. */
    for (i = 0; i <= 40; i++)
    {
        float  p = (float)i / 40.0f;
        double want;
        double got;
        double slack;

        if (p == 0.5f)
            continue;
        s = src(0, 0.5f, p);
        AEC_i_Steps(&s, st, AEC_MAX_STEPS);

        /* A quarter-grey pixel, which is neither the pivot nor an end
           -- at the pivot every gain agrees and at an end the clamp
           hides the difference. */
        want = (0.25 - mid) * 2.0 * (double)p + mid;
        if (want < 0.0)
            want = 0.0;
        if (want > 1.0)
            want = 1.0;
        got = apply(&st[0], 0.25, 0.25, 0.25, 0);
        slack = (p < 0.5f ? 0.5 : 1.0) / 255.0;

        ok(fabs(got - want) <= slack + 1e-9,
           p < 0.5f ? "within half a byte of the formula, below neutral"
                    : "within a byte of it, above neutral");
    }

    /* And the bound is reached, so the two numbers above are a
       measurement and not a guess that happens to hold. A parameter
       just short of the top leaves the upper half's offset nearly a
       whole byte short. */
    s = src(0, 0.5f, 0.99609f);
    AEC_i_Steps(&s, st, AEC_MAX_STEPS);
    {
        double want = (0.25 - mid) * 2.0 * 0.99609 + mid;
        double got = apply(&st[0], 0.25, 0.25, 0.25, 0);

        if (want < 0.0)
            want = 0.0;
        ok(fabs(want - got) > 0.9 / 255.0,
           "the upper half really does miss by a byte somewhere");
    }

    /* The gain rises with the parameter the whole way across, with no
       step at the join where the two halves meet. */
    {
        double last = -1.0;

        for (i = 0; i <= 100; i++)
        {
            float p = (float)i / 100.0f;

            if (p == 0.5f)
                continue;
            s = src(0, 0.5f, p);
            AEC_i_Steps(&s, st, AEC_MAX_STEPS);
            ok(st[0].gain >= last, "the gain never falls as the"
                                   " parameter rises");
            last = st[0].gain;
        }
    }
}

/* -------------------------------------------------------- grayscale */

static void caseGrayscale(void)
{
    tAec_Step   st[AEC_MAX_STEPS];
    tAec_Source s;
    int         i;
    int         j;

    printf("-- the grayscale, against the two stages it came from\n");

    s = src(1, 0.5f, 0.5f);
    AEC_i_Steps(&s, st, AEC_MAX_STEPS);

    /* Against the hardware's own two-step form, evaluated here from
       the packed bytes rather than from anything aecolor.c exposes:
       a stage that adds a grey of 127/255 to the channel scaled by
       the same, and then a dot product that reads both sides as
       signed. If the collapsed gain, bias or weights are wrong, the
       two disagree. */
    for (i = 0; i <= 8; i++)
        for (j = 0; j <= 8; j++)
        {
            double in[3];
            double d = 127.0 / 255.0;
            double packed[3] = { 154.0 / 255.0, 218.0 / 255.0,
                                 136.0 / 255.0 };
            double staged = 0.0;
            int    c;

            in[0] = (double)i / 8.0;
            in[1] = (double)j / 8.0;
            in[2] = (double)((i + j) % 9) / 8.0;

            for (c = 0; c < 3; c++)
            {
                double biased = d + d * in[c];

                staged += (2.0 * biased - 1.0) * (2.0 * packed[c] - 1.0);
            }
            if (staged < 0.0)
                staged = 0.0;
            if (staged > 1.0)
                staged = 1.0;

            okNear(apply(&st[0], in[0], in[1], in[2], 0), staged, 1e-6,
                   "the collapsed form matches the two stages");
        }

    /* All three channels come out the same, which is what makes it a
       grayscale rather than a channel mixer. */
    okNear(apply(&st[0], 0.2, 0.7, 0.4, 0), apply(&st[0], 0.2, 0.7, 0.4, 1),
           1e-7, "red and green come out alike");
    okNear(apply(&st[0], 0.2, 0.7, 0.4, 1), apply(&st[0], 0.2, 0.7, 0.4, 2),
           1e-7, "and blue");

    /* The weights are green-heavy and blue-light, and they are not a
       luminance anyone would write down -- stated as the ordering
       rather than as three decimals, so the check says what is being
       relied on. */
    ok(st[0].weights[1] > st[0].weights[0], "green weighs more than red");
    ok(st[0].weights[0] > st[0].weights[2], "and red more than blue");
    ok(st[0].weights[1] > 3.0f * st[0].weights[0],
       "green by more than three times");

    /* They do not come to one, so white does not come out white. That
       is the renderer's own arithmetic and the obvious thing to
       "correct". */
    {
        double sum = st[0].weights[0] + st[0].weights[1] + st[0].weights[2];

        ok(sum < 1.0, "the weights come to less than one");
        ok(sum > 0.95, "but not far short");
        ok(apply(&st[0], 1.0, 1.0, 1.0, 0) < 1.0,
           "so white comes out a little under white");
        okNear(apply(&st[0], 1.0, 1.0, 1.0, 0), sum * st[0].gain + st[0].bias,
               1e-7, "by exactly what the weights and the bias say");
    }

    /* Black comes out at nought rather than below it: the bias is
       negative and the clamp is what catches it. */
    ok(st[0].bias < 0.0f, "the bias is below nought");
    okNear(apply(&st[0], 0.0, 0.0, 0.0, 0), 0.0, 1e-7,
           "so black comes out black and not less");

    /* And it is monotone in every channel, which a wrong sign on a
       weight or on the gain would break. */
    for (i = 0; i < 3; i++)
    {
        double lo[3] = { 0.1, 0.1, 0.1 };
        double hi[3] = { 0.1, 0.1, 0.1 };

        hi[i] = 0.9;
        ok(apply(&st[0], hi[0], hi[1], hi[2], 0)
           > apply(&st[0], lo[0], lo[1], lo[2], 0),
           "more of any channel is more grey");
    }
}

/* ------------------------------------------- the three together */

static void caseClampBetween(void)
{
    tAec_Step   st[AEC_MAX_STEPS];
    tAec_Source s;
    int         n;
    double      clamped;
    double      unclamped;
    int         i;

    printf("-- the clamp between the steps is part of the answer\n");

    /* Brightness all the way up and then contrast all the way up. A
       channel the brightness pushes past white is white before the
       contrast sees it, so the two orders of doing the arithmetic
       disagree -- which is why the renderer may not compose these
       without clamping between them. */
    s = src(0, 1.0f, 1.0f);
    n = AEC_i_Steps(&s, st, AEC_MAX_STEPS);
    okEq(n, 2, "two steps");

    clamped = 0.7;
    unclamped = 0.7;
    for (i = 0; i < n; i++)
    {
        clamped = apply(&st[i], clamped, clamped, clamped, 0);
        unclamped = unclamped * st[i].gain + st[i].bias;
    }
    ok(unclamped > 1.0, "without the clamp the channel runs past white");
    okNear(clamped, 1.0, 1e-7, "with it, white is as far as it goes");

    /* And a case where the clamp bites at the bottom instead. */
    s = src(0, 0.0f, 1.0f);
    n = AEC_i_Steps(&s, st, AEC_MAX_STEPS);
    clamped = 0.3;
    unclamped = 0.3;
    for (i = 0; i < n; i++)
    {
        clamped = apply(&st[i], clamped, clamped, clamped, 0);
        unclamped = unclamped * st[i].gain + st[i].bias;
    }
    ok(unclamped < 0.0, "without the clamp it runs below black");
    okNear(clamped, 0.0, 1e-7, "with it, black is as far as it goes");
}

int main(void)
{
    caseWhichApply();
    caseGrayAmount();
    caseBrightness();
    caseContrast();
    caseGrayscale();
    caseClampBetween();

    printf("\n  %ld checks, %ld failure(s)\n", checks, fails);
    return fails ? 1 : 0;
}
