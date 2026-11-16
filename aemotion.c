#include "aemotion.h"

#include <math.h>

/* How far the watched direction has to move before the frame is
   smeared. Not a round number, and read as written. */
#define AMO_THRESHOLD   0.0049999999

/* What the movement is multiplied by to become a smear. The movement
   is in the camera's own units and this turns it into steps. */
#define AMO_GAIN        50.0

/* One step, across and down, as a fraction of the picture. A
   six-hundred-and-fortieth and a four-hundred-and-eightieth, written
   as constants rather than taken from the frame -- see the header. The
   second is not exactly a four-hundred-and-eightieth; it is the
   decimal the other renderer carries. */
#define AMO_USCALE      0.0015625
#define AMO_VSCALE      0.0020833334

/* The weight each sample arrives with: 63 of 255, carried in a vertex
   colour whose every byte is 63. */
#define AMO_WEIGHT_BYTE 63.0f

int AMO_b_On(unsigned int aeMaster)
{
    return (aeMaster & AMO_BIT) != 0u;
}

void AMO_Probe(float* out)
{
    if (!out)
        return;
    out[0] = 1.0f;
    out[1] = 1.0f;
    out[2] = 1.0f;
}

int AMO_b_Decide(float* outDx, float* outDy, const float* now,
                 const float* before)
{
    double dx;
    float  dy;
    float  dz;

    if (!now || !before)
        return 0;

    /* Across is widened to double here and the other two are not,
       which is the other renderer's spelling and not an accident of
       this one: it keeps that difference in a double because it needs
       it again below, and squares the other two where they are. The
       three terms of the sum are therefore computed at two different
       precisions. */
    dx = now[0] - before[0];
    dy = now[1] - before[1];
    dz = now[2] - before[2];

    /* And the sum is in the order the other renderer writes it --
       across, then **towards**, then down. Not across, down, towards.
       Floating-point addition is not associative, so the order is part
       of the answer at the threshold.
     *
       The comparison is strict, and whether it is strict cannot be
       seen: it would take a distance exactly equal to the constant,
       which is a decimal double that the square root of a sum of float
       squares does not land on. The *value* of the constant can be
       seen, and the unit test pins it with a two-axis movement that
       falls between it and the round number it nearly is. */
    if (!(sqrt(dx * dx + dz * dz + dy * dy) > AMO_THRESHOLD))
        return 0;

    if (outDx)
        *outDx = (float)(dx * AMO_GAIN);
    if (outDy)
        *outDy = (float)((double)dy * AMO_GAIN);
    return 1;
}

void AMO_Taps(float* uOut, float* vOut, float dx, float dy)
{
    /* The four multipliers, in binding order -- see the header, they
       are neither symmetric nor sorted. */
    static const double at[AMO_TAPS] = { -3.0, 2.0, -1.0, 0.0 };

    float uStep;
    float vStep;
    int   i;

    uStep = (float)((double)dx * AMO_USCALE);
    vStep = (float)((double)dy * AMO_VSCALE);

    for (i = 0; i < AMO_TAPS; i++)
    {
        if (uOut)
            uOut[i] = (float)((double)uStep * at[i]);
        if (vOut)
            vOut[i] = (float)((double)vStep * at[i]);
    }
}

float AMO_f_Weight(void)
{
    return AMO_WEIGHT_BYTE / 255.0f;
}

int AMO_b_OnePass(int simultaneousTextures)
{
    /* Two and three take two draws; anything else takes one. Written
       the way the other renderer writes the test, which is the awkward
       way round: the *fewer* units arm is the middle of the range, and
       a card reporting one unit takes the four-sample path it cannot
       run. That is its arithmetic and not a reading of it. */
    return simultaneousTextures < 2 || simultaneousTextures > 3;
}
