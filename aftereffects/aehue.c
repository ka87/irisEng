#include "aehue.h"

/* The wave's own numbers, each spelled the way the other renderer
 * spells it rather than derived from one another.
 *
 * AHU_RAMP_AT and AHU_RAMP_FROM are both a sixth, and they are **not
 * the same number**: the threshold the ramp begins at sits a hair
 * below the value the ramp measures its distance from. See the header
 * -- between the two the wave comes out above one, which is kept
 * rather than tidied because tidying it would be inventing a constant.
 */
#define AHU_RAMP_AT     0.1666666       /* where the flat top ends */
#define AHU_RAMP_FROM   0.16666667      /* what the ramp measures from */
#define AHU_RAMP_TO     0.33333331      /* where the ramp reaches nothing */
#define AHU_RAMP_SLOPE  6.0

/* A third of a cycle, and two thirds, as written. The two decimals
   are **the same float as one another doubled** -- identical bits --
   so carrying both is spelling rather than value, and nothing can
   tell this from doubling the first. See the header. */
#define AHU_THIRD       0.33333331f
#define AHU_TWO_THIRDS  0.66666663f

/* What a channel's amount is scaled by: a half of twice the range, so
   the range -- written the long way round because that is the way the
   other renderer writes it, and the two halves do not cancel in the
   order they are applied to a float depth. */
#define AHU_HALF        0.5
#define AHU_TWICE_FULL  510.0

/* The widest and narrowest an amount may be before converting it to a
   whole number stops being defined. Everything the wave can produce
   is well inside this; the test is for the depth a script could pass
   that the other renderer would convert out of range. */
#define AHU_MIN         (-1.0)
#define AHU_MAX         4294967296.0

int AHU_b_On(unsigned int aeMaster)
{
    return (aeMaster & AHU_BIT) != 0u;
}

float AHU_f_Wave(float phase)
{
    double wrapped;

    wrapped = phase;

    /* One subtraction and no more -- see the header. A phase well
       outside the cycle does not come back into it. */
    if (phase > 1.0)
        wrapped = wrapped - 1.0;

    /* Folded about the half, which is what makes the second half of
       the cycle the first half backwards. */
    if (wrapped > 0.5)
        wrapped = 1.0 - wrapped;

    /* Neither of these two can be an equality: the thresholds are
       decimal doubles, the value is always a float or one minus a
       float, and neither decimal is a float. So which way round the
       comparisons are written is not observable, and a sweep says so
       rather than this comment guessing. */
    if (wrapped < AHU_RAMP_AT)
        return 1.0f;
    if (wrapped <= AHU_RAMP_TO)
        return (float)(1.0 - (wrapped - AHU_RAMP_FROM) * AHU_RAMP_SLOPE);
    return 0.0f;
}

float AHU_f_ChannelPhase(float phase, int channel)
{
    /* The whole cycle added back is what keeps the argument positive,
       so the single wrap above has something it can cope with. */
    if (channel == 1)
        return phase - AHU_THIRD + 1.0f;
    if (channel == 2)
        return phase - AHU_TWO_THIRDS + 1.0f;
    return phase;
}

unsigned int AHU_u_Channel(float amount, float phase)
{
    double v;

    v = (1.0 - (double)AHU_f_Wave(phase)) * (double)amount
        * AHU_HALF * AHU_TWICE_FULL;

    /* The guard covers exactly the values the conversion is not
       defined for, so every amount a wave and a sane depth can produce
       goes through it unchanged -- including the negative sliver the
       untidy threshold makes, which truncates to nothing on both
       sides. */
    if (!(v > AHU_MIN && v < AHU_MAX))
        return 0u;
    return (unsigned int)v;
}

unsigned int AHU_u_Subtract(float amount, float phase)
{
    unsigned int red;
    unsigned int green;
    unsigned int blue;

    red = AHU_u_Channel(amount, AHU_f_ChannelPhase(phase, 0));
    green = AHU_u_Channel(amount, AHU_f_ChannelPhase(phase, 1));
    blue = AHU_u_Channel(amount, AHU_f_ChannelPhase(phase, 2));

    /* The packing, and the asymmetry the header describes: the mask's
       ones swallow red's overflow and carry green's and blue's into
       the channel above. Written out as the other renderer writes it,
       because at a depth above one the packing *is* the effect. */
    return ((((red | 0xFFFFFF00u) << 8) | green) << 8) | blue;
}

void AHU_Channels(float* out, unsigned int packed)
{
    if (!out)
        return;
    out[0] = (float)((packed >> 16) & 0xFFu) / 255.0f;
    out[1] = (float)((packed >> 8) & 0xFFu) / 255.0f;
    out[2] = (float)(packed & 0xFFu) / 255.0f;
}
