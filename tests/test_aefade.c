/* A unit test for aefade.c.
 *
 * The fade's timer. Everything here is arithmetic over a single byte
 * of opacity, and the two things that make it worth stating away from
 * a picture are both invisible in one:
 *
 *   - the step is **truncated** before it is taken, so what a duration
 *     actually comes to is not what it asked for, and the error is a
 *     frame or two that no frame of video shows;
 *   - the conversion that truncates it is where the other renderer has
 *     undefined behaviour. This is built with the undefined-behaviour
 *     sanitizer on, so a case that reaches an out-of-range conversion
 *     fails here rather than producing a number that happens to work
 *     on one compiler.
 *
 * What the differential against the other renderer cannot see is also
 * here: the blend share, which only this renderer computes, and the
 * defensive range guard, which nothing reachable can drive.
 */

#include "../aefade.h"

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
        printf("  FAIL %s: got %g want %g\n", what, got, want);
        fails++;
    }
}

/* How many frames a fade of this duration actually takes, by running
   it. Capped, and the cap is reported, so a fade that never finishes
   is a readable answer rather than a hang. */
static int FramesFor(float seconds, int* finished)
{
    float step = AFD_f_Step(seconds);
    int   opacity = AFD_FULL;
    int   frames;

    *finished = 0;
    for (frames = 0; frames < 4096; frames++)
    {
        if (AFD_b_Advance(&opacity, step))
        {
            *finished = 1;
            return frames + 1;
        }
    }
    return frames;
}

/* ---------------------------------------------------- the step ----- */

static void caseStepTable(void)
{
    /* asked, the step, and the whole number taken. The last column is
       what the duration comes to, and it is the point of the case: a
       fade is a count of truncated steps and not a length of time. */
    static const struct { float asked; double step; unsigned taken; int frames; }
    table[] = {
        { 0.50f,   8.5,     8u,  32 },
        { 1.00f,   4.25,    4u,  64 },
        { 2.00f,   2.125,   2u, 128 },
        { 4.00f,   1.0625,  1u, 255 },
        { 4.25f,   1.0,     1u, 255 },
        { 8.00f,   1.0,     1u, 255 },
    };
    size_t i;

    printf("-- the step, truncated, and what a duration comes to\n");

    for (i = 0; i < sizeof(table) / sizeof(table[0]); i++)
    {
        int finished = 0;
        int frames;

        okNear((double)AFD_f_Step(table[i].asked), table[i].step, 1e-6,
               "the step is the range over the span");
        okEq((long)AFD_u_Taken(AFD_f_Step(table[i].asked)),
             (long)table[i].taken, "truncated toward nothing");
        frames = FramesFor(table[i].asked, &finished);
        ok(finished, "the fade finishes");
        okEq(frames, table[i].frames, "in the frames the truncation gives");
    }

    /* And the ceiling, stated on its own: no duration can buy more
       than the range in frames, because the step cannot go below one.
       Swept rather than sampled, so a clamp that let a longer fade
       through anywhere shows. */
    {
        int worst = 0;
        int seconds;

        for (seconds = 1; seconds <= 600; seconds++)
        {
            int finished = 0;
            int frames = FramesFor((float)seconds * 0.125f, &finished);

            ok(finished, "every duration finishes");
            if (frames > worst)
                worst = frames;
        }
        okEq(worst, AFD_FULL, "and none of them lasts longer than the"
                              " range in frames");
    }
}

static void caseStepClamp(void)
{
    printf("-- the clamp at one, and where it can be seen at all\n");

    /* Above the range the step is one exactly, which is the clamp
       doing its job: the untruncated step there is below one, and a
       step below one truncates to nothing and never finishes. */
    okNear((double)AFD_f_Step(5.0f), 1.0, 1e-6, "a long fade steps by one");
    okEq((long)AFD_u_Taken(AFD_f_Step(5.0f)), 1L, "and takes one");

    /* The boundary is at the range in frames, and the direction of the
       comparison there cannot be seen: the division gives exactly one
       at that span. So the case that matters is not the boundary but
       the span where a clamp set *lower* would start lying -- half the
       range in frames, where the true step is two and a clamp would
       call it one. */
    okNear((double)AFD_f_Step((float)AFD_FULL / 60.0f), 1.0, 1e-6,
           "at the range exactly the step is one either way round");
    okEq((long)AFD_u_Taken(AFD_f_Step((float)AFD_FULL / 120.0f)), 2L,
         "and at half the range it is two, which a lower clamp would"
         " flatten to one");
}

static void caseStepNoLength(void)
{
    printf("-- a duration that cannot be stepped\n");

    /* Nought, and a span so short the step will not fit in a whole
       number. Both give a step of nothing, which takes nothing -- so
       the fade never finishes and its bit never clears. That is the
       other renderer's behaviour and it is kept deliberately; see the
       pick in the notes. */
    okEq((long)(AFD_f_Step(0.0f) == 0.0f), 1L, "no length gives no step");
    okEq((long)AFD_u_Taken(AFD_f_Step(0.0f)), 0L, "and takes nothing");
    okEq(AFD_i_Next(AFD_FULL, AFD_f_Step(0.0f)), AFD_FULL,
         "so the timer does not move");
    {
        int finished = 0;
        int frames = FramesFor(0.0f, &finished);

        okEq((long)finished, 0L, "and the fade never finishes");
        ok(frames > AFD_FULL, "running on past the range");
    }

    okEq((long)(AFD_f_Step(1.0e-30f) == 0.0f), 1L,
         "a span too short to step gives no step either");

    /* A negative duration is different: the step is an ordinary
       negative number, and the fade runs *backwards* past the top of
       the range with no ceiling. Reproduced rather than guarded, and
       the two spellings of the subtraction agree about it. */
    okNear((double)AFD_f_Step(-1.0f), -4.25, 1e-6,
           "a negative duration gives a negative step");
    okEq(AFD_i_Next(AFD_FULL, AFD_f_Step(-1.0f)), AFD_FULL + 4,
         "which puts the timer above the top of the range");
    okEq(AFD_i_Next(AFD_FULL + 4, AFD_f_Step(-1.0f)), AFD_FULL + 8,
         "and keeps going");
}

static void caseTakenRange(void)
{
    printf("-- the guard on a step that cannot be a whole number\n");

    /* Nothing reachable drives this: every step the step function
       returns is either in range or exactly nought. It is a guard on a
       public function, and the sanitizer this is built with is the
       reason it is a guard and not a cast -- the conversion it stands
       in front of is the one the other renderer leaves undefined. */
    okEq((long)AFD_u_Taken(1.0e30f), 0L, "a step far too large takes"
                                         " nothing");
    okEq((long)AFD_u_Taken(-1.0e30f), 0L, "and so does one far too small");
    okEq((long)AFD_u_Taken(0.0f), 0L, "and nought takes nothing");
}

/* ------------------------------------------------- the timer ------- */

static void caseNextSaturates(void)
{
    printf("-- the timer stops at nothing rather than wrapping\n");

    /* A short fade's step is larger than the opacity it is taken from,
       which is the case the clamp exists for: without it the timer
       wraps to the top of an unsigned word and the fade runs for
       billions of frames. */
    okEq(AFD_i_Next(4, 8.5f), 0, "a step past the end lands on nothing");
    okEq(AFD_i_Next(0, 8.5f), 0, "and nothing stays nothing");
    okEq(AFD_i_Next(9, 8.5f), 1, "a step that just fits leaves the rest");
    okEq(AFD_i_Next(8, 8.5f), 0, "and one that just does not leaves"
                                 " nothing");
}

static void caseAdvance(void)
{
    int opacity;

    printf("-- one frame of the timer, and when the fade is over\n");

    opacity = AFD_FULL;
    okEq((long)AFD_b_Advance(&opacity, 4.25f), 0L, "a fade with room to go"
                                                   " is not over");
    okEq(opacity, AFD_FULL - 4, "and the timer moved by the truncated step");

    opacity = 3;
    okEq((long)AFD_b_Advance(&opacity, 4.25f), 1L,
         "the frame that runs out is the frame it is over");
    okEq(opacity, 0, "with the timer at nothing");

    /* An opacity already at nothing is over at once, and stepping it
       first gives the same answer -- which is why the two-stage test
       the other renderer writes collapses into one. */
    opacity = 0;
    okEq((long)AFD_b_Advance(&opacity, 4.25f), 1L, "nothing left is over");
    okEq(opacity, 0, "and stays nothing");

    /* A null timer is not a crash. */
    okEq((long)AFD_b_Advance(NULL, 4.25f), 0L, "no timer, nothing to say");
}

static void caseFreeze(void)
{
    printf("-- the one frame the picture is put aside on\n");

    okEq((long)AFD_b_Freeze(AFD_FULL), 1L, "the first frame freezes");
    okEq((long)AFD_b_Freeze(AFD_FULL - 1), 0L, "the next one does not");
    okEq((long)AFD_b_Freeze(0), 0L, "nor the last");
    /* And not above the range either, which a backwards fade reaches:
       a freeze there would take a fresh copy part way through. */
    okEq((long)AFD_b_Freeze(AFD_FULL + 4), 0L,
         "nor a timer that has run backwards past the top");
}

/* -------------------------------------------------- the share ------ */

static void caseBlend(void)
{
    printf("-- the frozen copy's share of the picture\n");

    /* The ends exactly, because they are what make a fade begin
       invisibly and end on the live picture. */
    okNear((double)AFD_f_Blend(AFD_FULL), 1.0, 1e-7,
           "all of the frozen copy at the top");
    okNear((double)AFD_f_Blend(0), 0.0, 1e-7, "and none of it at the end");
    /* And the middle over the range and not over a round number: the
       timer counts to 255, so halfway is 128 of 255 and not a half. */
    okNear((double)AFD_f_Blend(128), 128.0 / 255.0, 1e-7,
           "halfway is over the range, not over a round number");
    ok((double)AFD_f_Blend(128) > 0.5, "which is just over a half");
}

static void caseTextureFactor(void)
{
    printf("-- the share as the other renderer carries it\n");

    /* One constant doing two jobs: a neutral colour to multiply the
       picture by, and the share in its alpha. */
    okEq((long)(AFD_u_TextureFactor(AFD_FULL) >> 24), 255L,
         "the share sits in the top byte");
    okEq((long)(AFD_u_TextureFactor(0) >> 24), 0L, "at both ends");
    okEq((long)(AFD_u_TextureFactor(128) & 0xFFFFFFu), 0xFFFFFFL,
         "and the colour is left white");
    okEq((long)AFD_u_TextureFactor(128), (long)0x80FFFFFFL,
         "which is one word for the two of them");
}

/* ---------------------------------------------------- the bit ------ */

static void caseBit(void)
{
    printf("-- the bit the effect reads and writes\n");

    okEq((long)AFD_b_On(AFD_BIT), 1L, "on when the bit is set");
    okEq((long)AFD_b_On(0u), 0L, "off when it is not");
    okEq((long)AFD_b_On(~AFD_BIT), 0L, "and every other bit is not it");

    /* Clearing leaves the rest of the word alone, which matters: the
       word is shared with every other effect and this is the only one
       that writes it. */
    okEq((long)AFD_u_Cleared(AFD_BIT | 0x10u), 0x10L,
         "clearing takes out the fade and nothing else");
    okEq((long)AFD_u_Cleared(0x10u), 0x10L, "and is safe to do twice");
    okEq((long)AFD_b_On(AFD_u_Cleared(AFD_BIT)), 0L, "after which it is off");
}

int main(void)
{
    caseStepTable();
    caseStepClamp();
    caseStepNoLength();
    caseTakenRange();
    caseNextSaturates();
    caseAdvance();
    caseFreeze();
    caseBlend();
    caseTextureFactor();
    caseBit();

    printf("  %ld checks, %ld failure(s)\n", checks, fails);
    return fails ? 1 : 0;
}
