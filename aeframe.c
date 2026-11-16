#include "aeframe.h"

#include <string.h>

/* The bits that ask for each of the four. */
#define AEF_BIT_MOTIONBLUR  0x10u
#define AEF_BIT_SCREENBLUR  0x20u
#define AEF_BIT_REMANENCE   0x800u
#define AEF_BIT_ZOOMSMOOTH  0x400u

/* A parameter scaled to a byte and truncated, which is how each of
   these reaches the hardware. Reproduced rather than rounded off,
   because a weight one byte out is a visibly different smear at the
   large settings these effects are used at. */
static double AEF_l_f_Byte(double scaled)
{
    unsigned int b;

    if (scaled < 0.0)
        scaled = 0.0;
    if (scaled > 255.0)
        scaled = 255.0;
    b = (unsigned int)scaled;
    return (double)b / 255.0;
}

/* ------------------------------------------------------ motion blur */

int AEF_b_MotionBlur(const tAef_Source* src)
{
    /* A bit and a parameter, and the parameter is tested against zero
       exactly -- the engine sets it rather than arriving at it. */
    return src && (src->aeMaster & AEF_BIT_MOTIONBLUR) != 0
           && src->motionBlur != 0.0f;
}

float AEF_f_MotionBlurKeep(const tAef_Source* src)
{
    double now;

    if (!src)
        return 0.0f;

    /* The renderer that has this carries the *new* frame's weight as
       the byte `255 - factor * 255` and lets the hardware's blend take
       one minus it for the old. So the quantisation lands on the new
       frame's share and the old frame's is what is left of a byte --
       which is not the same as quantising the old frame's directly,
       and is a step out at some settings. */
    now = AEF_l_f_Byte(255.0 - (double)src->motionBlur * 255.0);
    return (float)(1.0 - now);
}

/* -------------------------------------------------------- remanence */

/* The threshold the renderer that has this uses, spelled as it spells
   it: a hundredth, as the nearest float below it. Below this the
   effect is skipped rather than run with a weight of nearly nothing. */
#define AEF_REMANENCE_FLOOR  0.0099999998f

int AEF_b_Remanence(const tAef_Source* src)
{
    return src && (src->aeMaster & AEF_BIT_REMANENCE) != 0
           && src->remanence > AEF_REMANENCE_FLOOR;
}

/* One pixel of a 640 by 480 picture, as the depth blur's steps are.
   The reach here goes up in twos. */
#define AEF_STEP_U   0.0015625f
#define AEF_STEP_V   0.0020833334f

int AEF_i_RemanenceReaches(float* uSteps, float* vSteps, int max)
{
    int i;
    int n;

    n = max < AEF_REMANENCE_PASSES ? max : AEF_REMANENCE_PASSES;
    if (n <= 0 || (!uSteps && !vSteps))
        return 0;

    for (i = 0; i < n; i++)
    {
        /* One, three, five, seven -- the odd numbers, so the four
           blurs reach twice as far as the depth blur's three do while
           costing one more pass. */
        if (uSteps)
            uSteps[i] = AEF_STEP_U * (float)(2 * i + 1);
        if (vSteps)
            vSteps[i] = AEF_STEP_V * (float)(2 * i + 1);
    }
    return n;
}

float AEF_f_RemanenceWeight(const tAef_Source* src)
{
    if (!src)
        return 0.0f;
    /* Twice, because the blurred copy goes into the sum in two
       successive stages carrying the same weight and the same
       coordinates. */
    return (float)(2.0 * AEF_l_f_Byte((double)src->remanence * 255.0));
}

/* ------------------------------------------------------ zoom smooth */

int AEF_b_ZoomSmooth(const tAef_Source* src)
{
    return src && (src->aeMaster & AEF_BIT_ZOOMSMOOTH) != 0
           && src->zoomSmooth != 0.0f;
}

float AEF_f_ZoomWeight(void)
{
    /* `256 / 8 - 1` of 255, which is 31 and not the 32 an eighth of
       256 would be, nor the 31.875 an eighth of 255 would be. Eight of
       them come to 248 of 255, so the sum lands a little under rather
       than a little over -- which is the point of the minus one. */
    return (float)((256 / AEF_ZOOM_TAPS - 1) / 255.0);
}

int AEF_i_ZoomTaps(const tAef_Source* src, tAef_ZoomTap* out, int max)
{
    double uStep;
    double vStep;
    int    i;
    int    n;

    n = max < AEF_ZOOM_TAPS ? max : AEF_ZOOM_TAPS;
    if (!src || !out || n <= 0)
        return 0;

    /* A forty-eighth of the texture per unit of the parameter, per
       tap -- ten rows of a 480-row picture.
     *
       The same fraction applies across, which is not the same number
       of pixels: on a 640 by 480 frame a forty-eighth is thirteen and
       a third pixels wide against ten high. So the crop is square in
       texture space and four-to-three in pixels, and a pixel smears
       further sideways than up. That is what the picture does, and a
       first draft of this comment claimed ten pixels both ways.
     *
       And a letterbox tilts it further, because only the horizontal
       step divides by the band's bottom edge: at a three-quarter band
       the horizontal rate is a third again and the vertical rate is
       unchanged. Nothing suggests that is deliberate; it is
       reproduced rather than tidied because it is what the picture
       does. */
    vStep = (double)src->zoomSmooth * 10.0 * (double)AEF_STEP_V;
    if (src->bandBottom > 0.0f)
        /* Spelled as the renderer that has it spells it -- a count of
           480ths of the band divided into the width -- rather than as
           the vStep over the band's bottom that it works out to. The
           two agree to the last bit or so and the long form is the one
           to check against. */
        uStep = (double)src->zoomSmooth * 6400.0
                / ((double)src->bandBottom * 480.0) * (double)AEF_STEP_U;
    else
        /* No band at all leaves nothing to divide by. The renderer
           that has this would divide by zero; this gives the answer
           an unletterboxed frame has, which is the same step as the
           vertical one. */
        uStep = vStep;

    for (i = 0; i < n; i++)
    {
        out[i].insetU = (float)(uStep * (double)i);
        out[i].insetV = (float)(vStep * (double)i);
    }
    return n;
}

/* ----------------------------------------------------- screen blur */

/* What the parameter is multiplied by to become a step. About a
   twenty-fifth, and read as written. */
#define AEF_SCREEN_SCALE    0.039999999

/* One pixel of a 640 by 480 picture, as doubles.
 *
 * Deliberately not the family's AEF_STEP_U and AEF_STEP_V, which are
 * floats: the horizontal one is a power of two and comes to the same
 * number either way, but the vertical float is 0.00208333344 where
 * this double is 0.0020833334, and taking it from the float would move
 * every reach this effect computes. */
#define AEF_SCREEN_U        0.0015625
#define AEF_SCREEN_V        0.0020833334

int AEF_b_ScreenBlur(const tAef_Source* src)
{
    /* Not a floor, unlike the remanence: anything that is not exactly
       nought runs, negative values included. */
    return src && (src->aeMaster & AEF_BIT_SCREENBLUR) != 0
           && src->screenBlur != 0.0f;
}

int AEF_i_ScreenBlurReaches(float* uSteps, float* vSteps, int max,
                            float strength)
{
    double scaled;
    float  step;
    int    i;
    int    n;

    n = max < AEF_SCREENBLUR_PASSES ? max : AEF_SCREENBLUR_PASSES;
    if (n <= 0 || (!uSteps && !vSteps))
        return 0;

    /* The product, and then the product narrowed. Both are used, and
       which one each reach comes from is the whole of the oddity this
       function reproduces -- see the header. */
    scaled = (double)strength * AEF_SCREEN_SCALE;
    step = (float)scaled;

    for (i = 0; i < n; i++)
    {
        /* The reach goes up one step a pass: one, two, three and so on
           to seven, rather than the remanence's odd numbers. */
        double span = (double)step * (double)(i + 1);

        if (uSteps)
            uSteps[i] = (float)(span * AEF_SCREEN_U);
        if (vSteps)
        {
            /* The first pass alone takes its vertical reach from the
               unnarrowed product. One rounding's difference, kept
               because it is what the other renderer computes. */
            if (i == 0)
                vSteps[i] = (float)(scaled * AEF_SCREEN_V);
            else
                vSteps[i] = (float)(AEF_SCREEN_V * span);
        }
    }
    return n;
}
