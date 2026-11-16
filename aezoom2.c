#include "aezoom2.h"

#include <math.h>

/* What the parameter is multiplied by to become one step of angle, in
   radians. The same decimal the screen blur scales by, and as there it
   is read as written rather than tidied to a twenty-fifth. */
#define AZ2_ANGLE_SCALE 0.039999999

/* The frame the sample coordinates are built in, and the middle of it.
   Written as the other renderer writes them -- the picture's real size
   does not come into the turn at all, only into the quad. */
#define AZ2_FRAME_W     640.0
#define AZ2_FRAME_H     480.0

int AZ2_b_On(unsigned int aeMaster, float zoom)
{
    return (aeMaster & AZ2_BIT) != 0u && zoom != 0.0f;
}

int AZ2_i_Stages(int simultaneousTextures, int engStages)
{
    int clamped;

    /* Clamped at four first -- the quad carries four coordinate sets
     * and no more -- and then the engine's setting is taken if the
     * card can reach it.
     *
     * The awkward shape is the other renderer's: the clamp is compared
     * against the setting, but the fallback below looks at the
     * *unclamped* count again. **That makes no difference**, and a
     * sweep says so rather than this comment guessing: the fallback is
     * only reached when the clamp is under the setting, and there the
     * clamped and unclamped counts agree about being four or more
     * either way. So it reads as a slip and is not one -- it is just
     * two spellings of the same test. */
    clamped = simultaneousTextures;
    if (simultaneousTextures >= 4)
        clamped = 4;

    if (clamped >= engStages)
        return engStages;
    if (simultaneousTextures >= 4)
        return 4;
    return simultaneousTextures;
}

int AZ2_i_Passes(int stages)
{
    if (stages <= 0)
        return 0;
    return AZ2_TAPS / stages;
}

int AZ2_i_Samples(int stages)
{
    /* Whole division twice over, so three stages take six samples and
       not eight -- the one count that does not divide. */
    return stages * AZ2_i_Passes(stages);
}

int AZ2_i_Angles(float* out, int max, float zoom, int stages)
{
    float step;
    float advance;
    float angle;
    int   passes;
    int   samples;
    int   written;
    int   pass;
    int   i;

    passes = AZ2_i_Passes(stages);
    samples = AZ2_i_Samples(stages);
    if (!out || max <= 0 || samples <= 0)
        return 0;

    step = (float)((double)zoom * AZ2_ANGLE_SCALE);

    /* The start, which is half the whole span behind nought -- so the
       span is centred on no turn at all, and the lopsidedness comes
       from there being an even number of samples rather than from
       this. */
    angle = (float)((double)passes * (double)stages * (double)step * -0.5);
    advance = (float)((double)stages * (double)step);

    written = 0;
    for (pass = 0; pass < passes; pass++)
    {
        /* All four slots, not just the sampled ones -- see the
           header. */
        for (i = 0; i < AZ2_ANGLE_SLOTS; i++)
        {
            float a;

            /* The four offsets within a draw, each spelled as the
               other renderer spells it. They are all the float sum of
               the angle and a multiple of the step, and the
               differences between the spellings are roundings -- but
               they are the roundings that renderer has. */
            if (i == 0)
                a = angle;
            else if (i == 1)
                a = angle + step;
            else if (i == 2)
                a = (step + step) + angle;
            else
                a = (float)((double)step * 3.0) + angle;

            if (written >= max)
                return written;
            out[written] = a;
            written++;
        }
        /* Re-accumulated once a draw, which is what makes the split
           visible in the last bits of the angles. */
        angle = advance + angle;
    }
    return written;
}

int AZ2_i_SampledAngles(float* out, int max, float zoom, int stages)
{
    float all[AZ2_TAPS * AZ2_ANGLE_SLOTS];
    int   passes;
    int   written;
    int   pass;
    int   i;

    passes = AZ2_i_Passes(stages);
    if (!out || max <= 0 || passes <= 0 || stages <= 0)
        return 0;
    if (passes * AZ2_ANGLE_SLOTS > (int)(sizeof(all) / sizeof(all[0])))
        return 0;
    if (AZ2_i_Angles(all, passes * AZ2_ANGLE_SLOTS, zoom, stages)
        != passes * AZ2_ANGLE_SLOTS)
        return 0;

    written = 0;
    for (pass = 0; pass < passes; pass++)
        for (i = 0; i < stages; i++)
        {
            if (written >= max)
                return written;
            out[written] = all[pass * AZ2_ANGLE_SLOTS + i];
            written++;
        }
    return written;
}

float AZ2_f_Weight(int samples)
{
    if (samples <= 0)
        return 0.0f;
    /* A step under a true share, as the first zoom smooth's weight is:
       256 over the count, less one, out of 255. */
    return (float)((double)(256 / samples - 1) / 255.0);
}

void AZ2_Tap(float* out4, float angle)
{
    double c;
    double s;
    double a;
    double b;
    double cc;
    double d;

    if (!out4)
        return;

    c = cos((double)angle);
    s = sin((double)angle);

    /* The map as the other renderer computes it, with the second line
       reading the first line's answer -- see the header, this is not a
       rotation and its determinant is cos squared. */
    a = c;
    b = s;
    cc = -c * s;
    d = c - s * s;

    /* Conjugated by the frame's aspect, because the turn is done in a
       640 by 480 frame and the caller works in the unit square; and
       then by a flip of the vertical, because this interface measures
       it up the picture and the turn is written measuring it down.
       The flip negates the two off-diagonal terms and leaves the
       diagonal alone. */
    out4[0] = (float)a;
    out4[1] = (float)(-b * (AZ2_FRAME_H / AZ2_FRAME_W));
    out4[2] = (float)(-cc * (AZ2_FRAME_W / AZ2_FRAME_H));
    out4[3] = (float)d;
}

float AZ2_f_TapScale(float angle)
{
    double c = cos((double)angle);

    return (float)(c * c);
}
