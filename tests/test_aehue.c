/* A unit test for aehue.c.
 *
 * The hue sweep's arithmetic: one wave, three phases a third of a
 * cycle apart, and three bytes packed into a colour.
 *
 * None of it is visible in a picture. A wrong wave gives a tint of a
 * slightly different colour and a wrong phase offset gives a tint of
 * the wrong colour, both of which look exactly as deliberate as the
 * right answer. So the values are stated here, and they are stated
 * **exactly** -- every expected number below was taken from the other
 * renderer's own expressions rather than worked out by hand, and the
 * differential is what says this module still agrees with them.
 *
 * The case that earns the file is casePackingOverflow. At a depth
 * above one the channels overflow their bytes and they do not all
 * overflow the same way, which is a property of the *packing* and not
 * of the arithmetic -- so a renderer computing three clamped channels
 * would pass every other case here and draw the wrong picture.
 *
 * Built with the undefined-behaviour sanitizer, because turning the
 * wave into a whole number is a float-to-integer conversion and the
 * wave can come out a hair above one.
 */

#include "../aehue.h"

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

static void okHex(unsigned long got, unsigned long want, const char* what)
{
    checks++;
    if (got != want)
    {
        printf("  FAIL %s: got %08lX want %08lX\n", what, got, want);
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

/* -------------------------------------------------- the wave ------- */

static void caseWaveShape(void)
{
    /* phase, and what the wave is there. A flat top, a straight ramp
       down over the next sixth, nothing for the sixth after that --
       and then the mirror of all of it, because the fold makes the
       back half of the cycle the front half backwards. */
    static const struct { float phase; double value; } table[] = {
        { 0.0f,         1.0 },
        { 0.1f,         1.0 },
        { 0.16f,        1.0 },
        { 0.1666666f,   1.0 },          /* still the flat top */
        { 0.2f,         0.800000012 },
        { 0.25f,        0.5 },
        { 0.3f,         0.199999943 },
        { 0.33333331f,  0.0 },
        { 0.4f,         0.0 },
        { 0.5f,         0.0 },
        { 0.6f,         0.0 },          /* the mirror begins */
        { 0.75f,        0.5 },
        { 0.9f,         1.0 },
        { 1.0f,         1.0 },
    };
    size_t i;

    printf("-- the wave: a flat top, a ramp, and the mirror of both\n");

    for (i = 0; i < sizeof(table) / sizeof(table[0]); i++)
        okNear((double)AHU_f_Wave(table[i].phase), table[i].value, 1e-7,
               "the wave at a phase");

    /* The fold stated as a property rather than as the table above:
       every phase and the same distance back from the end of the cycle
       give the same answer. A fold about the wrong point, or none,
       breaks this while leaving the ramp looking right. */
    for (i = 0; i <= 50; i++)
    {
        float p = (float)i / 100.0f;

        okNear((double)AHU_f_Wave(p), (double)AHU_f_Wave(1.0f - p), 1e-6,
               "the wave is its own mirror about the half");
    }

    /* And the ramp is straight, which the three sampled points above
       suggest and this says: equal steps in phase, equal steps down. */
    {
        double a = (double)AHU_f_Wave(0.20f);
        double b = (double)AHU_f_Wave(0.25f);
        double c = (double)AHU_f_Wave(0.30f);

        okNear(a - b, b - c, 1e-6, "the ramp falls at one rate");
    }
}

static void caseWaveUntidy(void)
{
    printf("-- the sliver where the wave comes out above one\n");

    /* A sixth is spelled two ways and the threshold is the smaller, so
       between them the ramp measures a negative distance and returns
       more than one. The window is about seven parts in a thousand
       million and this phase is inside it -- found rather than
       reasoned about, and kept rather than tidied, because tidying it
       would mean inventing a constant the other renderer does not
       have. */
    ok((double)AHU_f_Wave(0.8333333433f) > 1.0,
       "the wave really does exceed one somewhere");

    /* What matters is that it comes to nothing subtracted rather than
       to a conversion out of range -- which is what the sanitizer this
       is built with would catch. */
    okEq((long)AHU_u_Channel(1.0f, 0.8333333433f), 0L,
         "and a little less than nothing subtracted is nothing");
    okEq((long)AHU_u_Channel(1.5f, 0.8333333433f), 0L,
         "at any depth");
}

static void caseWaveOutsideTheCycle(void)
{
    printf("-- a phase outside the cycle does not wrap into it\n");

    /* The wrap is one subtraction and no remainder, so a phase well
       above the cycle folds to a large negative, lands under the first
       threshold and returns one -- which subtracts nothing. Stated as
       the packed words, because what a caller sees is the colour. */
    okHex(AHU_u_Subtract(1.0f, 2.0f), 0xFF000000uL,
          "two cycles on subtracts nothing");
    okHex(AHU_u_Subtract(1.0f, 3.0f), 0xFF000000uL, "nor three");
    okHex(AHU_u_Subtract(1.0f, 5.0f), 0xFF000000uL, "nor five");
    okHex(AHU_u_Subtract(1.0f, -1.0f), 0xFF000000uL,
          "and a whole cycle back subtracts nothing either");

    /* But not uniformly nothing, which is the part that would be easy
       to get wrong by guarding the phase instead of reproducing the
       wrap: just outside the cycle the three channels disagree. */
    okHex(AHU_u_Subtract(1.0f, -0.25f), 0xFF00FF00uL,
          "a quarter cycle back takes all of green and none of the"
          " others");
    okHex(AHU_u_Subtract(1.0f, 1.5f), 0xFFFF0000uL,
          "and half a cycle on takes all of red");
}

/* ------------------------------------------------ the phases ------- */

static void casePhases(void)
{
    printf("-- the three channels sample a third of a cycle apart\n");

    okNear((double)AHU_f_ChannelPhase(0.25f, 0), 0.25, 1e-7,
           "red samples at the phase itself");
    okNear((double)AHU_f_ChannelPhase(0.25f, 1), 0.25 - 0.33333331 + 1.0,
           1e-7, "green a third further on, with a cycle added back");
    okNear((double)AHU_f_ChannelPhase(0.25f, 2), 0.25 - 0.66666663 + 1.0,
           1e-7, "and blue two thirds");

    /* The gaps are a third each, which is what makes the tint sweep
       rather than pulse. Checked as a difference so that a wrong pair
       of constants shows even if each looks plausible alone. */
    okNear((double)AHU_f_ChannelPhase(0.25f, 0)
           - (double)AHU_f_ChannelPhase(0.25f, 1) + 1.0, 1.0 / 3.0, 1e-6,
           "red to green is a third");
    okNear((double)AHU_f_ChannelPhase(0.25f, 1)
           - (double)AHU_f_ChannelPhase(0.25f, 2), 1.0 / 3.0, 1e-6,
           "and green to blue is a third");

    /* A channel index that is not one of the three answers as the
       first does, which is what the other renderer's own code comes
       to -- it has no index at all, only three sites. */
    okNear((double)AHU_f_ChannelPhase(0.25f, 7), 0.25, 1e-7,
           "anything else is the phase itself");
}

/* ----------------------------------------------- the packing ------- */

static void casePacking(void)
{
    /* depth, phase, and the colour. Taken from the other renderer's
       expressions, so these pin the packing as well as the wave. */
    static const struct { float amount; float phase; unsigned long word; }
    table[] = {
        { 0.5f, 0.0f, 0xFF007F7FuL },
        { 0.5f, 0.2f, 0xFF19007FuL },
        { 0.5f, 0.5f, 0xFF7F0000uL },
        { 0.5f, 0.9f, 0xFF007F33uL },
        { 1.0f, 0.0f, 0xFF00FFFFuL },
        { 1.0f, 0.3f, 0xFFCC00FFuL },
        { 1.0f, 0.5f, 0xFFFF0000uL },
        { 1.0f, 0.7f, 0xFFCCFF00uL },
        /* The top of the cycle is one off the bottom of it, because
           the fold lands a hair the other side of the ramp's end. Not
           a rounding slip here: the other renderer gives the same. */
        { 1.0f, 1.0f, 0xFF00FEFFuL },
    };
    size_t i;

    printf("-- the three channels packed into one colour\n");

    for (i = 0; i < sizeof(table) / sizeof(table[0]); i++)
        okHex(AHU_u_Subtract(table[i].amount, table[i].phase),
              table[i].word, "the packed colour");

    /* The alpha byte is full, whatever the channels are. It is not
       used by anything -- the colour write is narrowed to the three
       colour channels -- and it is full because of how the packing
       fills the upper bytes, not because anyone chose it. */
    for (i = 0; i < sizeof(table) / sizeof(table[0]); i++)
        okEq((long)(AHU_u_Subtract(table[i].amount, table[i].phase) >> 24),
             0xFFL, "with the alpha byte full");

    /* A depth of nought takes nothing from anything, which is the
       state the effect starts life in -- and it still draws, because
       the bit is the whole gate. */
    okHex(AHU_u_Subtract(0.0f, 0.25f), 0xFF000000uL,
          "no depth takes nothing");
}

static void casePackingOverflow(void)
{
    unsigned long word;
    float         channels[AHU_CHANNELS];

    printf("-- what the channels do when they overflow their bytes\n");

    /* The case the file exists for. At a depth of one and a half, red
       wants 306 and blue wants 382 -- neither fits a byte -- and green
       wants nothing at all. What comes out is red 50, green 1, blue
       126, and all three of those numbers are a story:

         red   306 is 0x132. Its high bit is OR-ed into a mask whose
               bits are already set, so it is simply lost and red wraps
               to 0x32. A channel that should be fully dark comes back
               a fifth lit.
         green wanted nothing, and reads 1. That bit is *blue's*
               overflow carrying up into the byte above it.
         blue  382 is 0x17E and keeps 0x7E, its high bit having gone
               into green.

       So the overflow is lost at the top and carried everywhere else,
       and the difference is only that the mask sits under red. */
    word = AHU_u_Subtract(1.5f, 0.3f);
    okHex(word, 0xFF32017EuL, "the packed colour at a depth above one");
    okEq((long)((word >> 16) & 0xFFu), 50L,
         "red wraps, its overflow swallowed by the mask");
    okEq((long)((word >> 8) & 0xFFu), 1L,
         "green reads one, which is blue's overflow and not green's own");
    okEq((long)(word & 0xFFu), 126L, "and blue keeps the rest of itself");

    /* Green's own amount really is nothing there, which is what makes
       the 1 above a carry rather than a coincidence. */
    okEq((long)AHU_u_Channel(1.5f, AHU_f_ChannelPhase(0.3f, 1)), 0L,
         "green's own amount is nothing");
    /* And red's and blue's really are over a byte. */
    okEq((long)AHU_u_Channel(1.5f, AHU_f_ChannelPhase(0.3f, 0)), 306L,
         "red's own amount is over a byte");
    okEq((long)AHU_u_Channel(1.5f, AHU_f_ChannelPhase(0.3f, 2)), 382L,
         "and so is blue's");

    /* Four more, so that the one above is not the only shape of it. */
    okHex(AHU_u_Subtract(1.5f, 0.25f), 0xFFBF017EuL, "red just fits");
    okHex(AHU_u_Subtract(1.5f, 0.35f), 0xFF7E0158uL, "red wrapped flat");
    okHex(AHU_u_Subtract(1.5f, 0.4f), 0xFF7E00E5uL, "blue just fits");
    okHex(AHU_u_Subtract(1.5f, 0.45f), 0xFF7E0072uL, "and blue below it");

    /* Unpacking is the bytes as fractions, in red green blue order.
       The order matters and nothing about a tint says which way round
       it is: swap red and blue and the tint sweeps backwards, which
       looks just as deliberate. */
    AHU_Channels(channels, 0xFF32017EuL);
    okNear((double)channels[0], 50.0 / 255.0, 1e-6, "red unpacks first");
    okNear((double)channels[1], 1.0 / 255.0, 1e-6, "green second");
    okNear((double)channels[2], 126.0 / 255.0, 1e-6, "and blue last");

    /* A full byte is all of the channel, which is what makes the
       subtract reach black. */
    AHU_Channels(channels, 0xFFFFFFFFuL);
    okNear((double)channels[0], 1.0, 1e-7, "a full byte is all of it");
    okNear((double)channels[2], 1.0, 1e-7, "in every channel");

    AHU_Channels(NULL, 0u);
    ok(1, "and unpacking nowhere is not a crash");
}

/* --------------------------------------------------- the bit ------- */

static void caseBit(void)
{
    printf("-- the bit, which is the whole of the gate\n");

    okEq((long)AHU_b_On(AHU_BIT), 1L, "on when the bit is set");
    okEq((long)AHU_b_On(0u), 0L, "off when it is not");
    okEq((long)AHU_b_On(~AHU_BIT), 0L, "and no other bit is it");
}

int main(void)
{
    caseWaveShape();
    caseWaveUntidy();
    caseWaveOutsideTheCycle();
    casePhases();
    casePacking();
    casePackingOverflow();
    caseBit();

    printf("  %ld checks, %ld failure(s)\n", checks, fails);
    return fails ? 1 : 0;
}
