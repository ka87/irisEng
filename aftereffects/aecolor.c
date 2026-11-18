#include "aecolor.h"

#include <string.h>

/* The bit that asks for the grayscale. The other two effects have no
   bit: they run whenever their parameter is off neutral. */
#define AEC_GRAY_BIT    0x1u

/* Neutral for both parameters. Compared exactly, as the engine
   compares it: these are set by script to a half and left there, not
   arrived at by arithmetic, so there is nothing for a tolerance to
   absorb. */
#define AEC_NEUTRAL     0.5f

/* Mid grey, 128 of 255. A shade above a half, and the shade matters:
   it is what the contrast pivots on at both ends, so using a half
   would move every pixel of a frame the effect is meant to leave
   alone. */
#define AEC_MID         (128.0 / 255.0)

/* The grayscale's own grey, 127 of 255 -- one less than the mid grey
   above, and a different number for a different job. It is the bias
   the hardware's dot product needs rather than a pivot; see
   AEC_l_Grayscale. */
#define AEC_GRAY_D      (127.0 / 255.0)

float AEC_f_MidGrey(void)
{
    return (float)AEC_MID;
}

/* A parameter's magnitude as the hardware receives it: scaled to a
   byte and **truncated**, not rounded. Reproduced because it is
   visible -- a gain of 127/255 where the arithmetic wanted 128/255 is
   a step of one in eight bits, and two of the three steps below
   quantise their numbers twice over. */
static double AEC_l_f_Byte(double scaled)
{
    unsigned int b;

    if (scaled < 0.0)
        scaled = 0.0;
    if (scaled > 255.0)
        scaled = 255.0;
    b = (unsigned int)scaled;
    return (double)b / 255.0;
}

/* Brightness: a grey added to every channel, or taken off it.
 *
 * The parameter's distance from neutral is the magnitude and its
 * direction is the sign -- the renderer that has this picks between an
 * adding and a subtracting stage rather than carrying a signed
 * number, which is the same thing said with two ops. */
static void AEC_l_Brightness(float balance, tAec_Step* out)
{
    double offset;

    out->kind = AEC_BRIGHTNESS;
    out->gain = 1.0f;
    out->blend = 1.0f;

    if (balance >= AEC_NEUTRAL)
    {
        offset = AEC_l_f_Byte(((double)balance - 0.5) * 255.0);
        out->bias = (float)offset;
    }
    else
    {
        offset = AEC_l_f_Byte((0.5 - (double)balance) * 255.0);
        out->bias = (float)-offset;
    }
}

/* Contrast: a gain about mid grey.
 *
 * The renderer that has this writes the two halves as two different
 * effects -- one blends the frame towards a flat mid grey, the other
 * doubles it and subtracts a grey back off -- and they come to the
 * same function:
 *
 *     out = (in - mid) * 2 * balance + mid
 *
 * which is a half at neutral giving a gain of one, nought giving flat
 * grey, and one giving twice the contrast. Worth writing down because
 * it is not what either half looks like on its own.
 *
 * The halves are kept apart here all the same, because they do not
 * round alike, and one of them is not as close to that formula as it
 * looks. The lower half quantises the gain once and lands within half
 * a byte of it. The upper half quantises the gain and the offset
 * separately, both truncating downwards, and the offset's truncation
 * goes straight to the output -- so it can sit a **whole** byte off
 * the formula, which is a visible step and not a rounding detail.
 * Measured across the range at a tenth of a thousandth, the worst
 * misses are 0.50 of a byte and 1.00 of a byte.
 *
 * Off, not below: both numbers truncate downwards, but a smaller gain
 * moves a pixel towards the pivot, so the miss lands above the
 * formula below mid grey and below it above. */
static void AEC_l_Contrast(float balance, tAec_Step* out)
{
    out->kind = AEC_CONTRAST;
    out->blend = 1.0f;

    if (balance < AEC_NEUTRAL)
    {
        /* Twice the parameter, as a byte: the frame is mixed with a
           flat mid grey by that much. */
        double gain = AEC_l_f_Byte((double)balance * 510.0);

        out->gain = (float)gain;
        out->bias = (float)(AEC_MID * (1.0 - gain));
    }
    else
    {
        double amount = (double)balance + (double)balance - 1.0;
        double bright = AEC_l_f_Byte(255.0 * amount);
        double dark = AEC_l_f_Byte(128.0 * amount);

        /* One plus the amount, because the frame is added to itself
           scaled -- and then a grey of a little over half the amount
           comes off, which is what keeps mid grey where it was. */
        out->gain = (float)(1.0 + bright);
        out->bias = (float)-dark;
    }
}

/* Grayscale: a weighted sum of the three channels, the same in all
 * three.
 *
 * The weights are not a luminance anyone would write down -- they are
 * heavy on green, light on blue, and they do not come to one. They are
 * what the renderer that has this uses, and they arrive through a dot
 * product that treats both of its arguments as **signed**: a channel
 * is read as twice itself less one, so a weight of w in the table is
 * 2w - 1 in the sum.
 *
 * That signed reading is also why there is a stage ahead of it adding
 * a grey of 127/255 to a channel scaled by the same: it exists so that
 * the signed reading comes back out as very nearly the channel it
 * started as. "Very nearly", because 127 is not half of 255, and what
 * is left over is the gain and the bias below:
 *
 *     in'_c   = D + D * in_c                       (the stage ahead)
 *     signed  = 2 * in'_c - 1 = 2D * in_c + 2D - 1
 *     sum     = dot(signed, w) = 2D * dot(in, w) + (2D - 1) * sum(w)
 *
 * so the gain is 2D and the bias is (2D - 1) times the weights' own
 * sum. Written out that way the whole effect is one dot product, which
 * is what this fills in -- the two-stage shape is the hardware's, not
 * the effect's. */
static void AEC_l_Grayscale(float amount, tAec_Step* out)
{
    /* The weights as the hardware holds them, before the signed
       reading: 154, 218 and 136 of 255. */
    static const double packed[3] = { 154.0 / 255.0, 218.0 / 255.0,
                                      136.0 / 255.0 };
    double sum = 0.0;
    int    i;

    out->kind = AEC_GRAYSCALE;
    for (i = 0; i < 3; i++)
    {
        double w = 2.0 * packed[i] - 1.0;

        out->weights[i] = (float)w;
        sum += w;
    }
    out->gain = (float)(2.0 * AEC_GRAY_D);
    out->bias = (float)((2.0 * AEC_GRAY_D - 1.0) * sum);

    /* The amount, quantised the way the renderer that has this
       quantises it: it carries the amount as a vertex alpha, so a
       byte, truncated -- the same treatment and the same reason as
       the two parameters above. */
    out->blend = (float)AEC_l_f_Byte((double)amount * 255.0);
}

int AEC_i_Steps(const tAec_Source* src, tAec_Step* out, int max)
{
    int n = 0;

    if (!src || !out || max <= 0)
        return 0;

    /* The order the chain runs them in. With a clamp between each, the
       order is part of the answer rather than a convenience. */
    if (src->brightness != AEC_NEUTRAL && n < max)
    {
        memset(&out[n], 0, sizeof(out[n]));
        AEC_l_Brightness(src->brightness, &out[n]);
        n++;
    }
    if (src->contrast != AEC_NEUTRAL && n < max)
    {
        memset(&out[n], 0, sizeof(out[n]));
        AEC_l_Contrast(src->contrast, &out[n]);
        n++;
    }
    /* The bit is not the whole of this one's gate: the amount is
       neutral at nought, and the bit set with no amount is no
       grayscale rather than all of it. */
    if ((src->aeMaster & AEC_GRAY_BIT) != 0 && src->grayscale != 0.0f
        && n < max)
    {
        memset(&out[n], 0, sizeof(out[n]));
        AEC_l_Grayscale(src->grayscale, &out[n]);
        n++;
    }
    return n;
}
