#include "depthblur.h"

#include <string.h>

/* The smallest positive float. The near plane is floored to it rather
   than to zero, because every distance above is built by adding to it
   and the ramp divides by two of them. */
#define DPB_TINY  1.1754944e-38f

/* The separation between one distance and the next. */
#define DPB_GAP   0.0099999998

/* A setting that does not clear the one below it by DPB_GAP is
   replaced by that floor, so the distances come out in order whatever
   goes in -- which is what keeps the ramp's divisions inside the
   table.

   In order, but not necessarily apart: the sum is formed in double and
   stored back into a float, and past about 2^23 the gap is below the
   float's resolution, so the result is the value it was meant to clear
   and the two come out equal. The ramp's arms are written for that.
   The arithmetic is in double because that is the width the comparison
   is made at; doing it in float moves the result. */
static float DPB_l_f_AtLeast(float value, double floorAt)
{
    return value <= floorAt ? (float)floorAt : value;
}

void DPB_Decode(const tDpb_Source* src, tDpb_State* out)
{
    if (!src || !out)
        return;

    memset(out, 0, sizeof(*out));

    /* The tweak wins over everything, and the sun's mode is the one
       that is left when no blur was asked for -- so a sun with the
       blur switched on elsewhere does not get a ramp of its own. */
    if (src->b_zonAlphaTweak)
        out->kind = DPB_OFF;
    else if ((src->aeMaster & 8) != 0 && (src->aeMaster & 0x1000) != 0)
        out->kind = DPB_BOTH;
    else if ((src->aeMaster & 8) != 0)
        out->kind = DPB_FAR;
    else if ((src->aeMaster & 0x1000) != 0)
        out->kind = DPB_NEAR;
    else
        out->kind = src->b_sunEffectOn ? DPB_SUN : DPB_OFF;

    out->nearClamped = src->nearPlane <= (double)DPB_TINY ? DPB_TINY
                                                          : src->nearPlane;
    out->near1 = DPB_l_f_AtLeast(src->depthBlurNear1,
                                 (double)out->nearClamped + DPB_GAP);
    out->type0 = DPB_l_f_AtLeast(src->depthBlurType0,
                                 (double)out->near1 + DPB_GAP);
    out->type1 = DPB_l_f_AtLeast(src->depthBlurType1,
                                 (double)out->type0 + DPB_GAP);
    /* The far plane is the one compared the other way round: it is
       pushed out to clear type1 rather than being a setting that has to
       clear the floor. */
    out->farUsed = (double)out->type1 + DPB_GAP >= src->farPlane
                   ? (float)((double)out->type1 + DPB_GAP)
                   : src->farPlane;

    /* How the coordinate is made. Each mode reaches the ramp by a
       different distance: the far blur starts at type0, the near one at
       near1, and the sun's spans the whole frustum. */
    switch (out->kind)
    {
    case DPB_OFF:
        out->b_projected = 0;
        break;
    case DPB_FAR:
        out->b_projected = 1;
        out->translateU = out->type0;
        out->scaleW = 1.0f;
        break;
    case DPB_NEAR:
    case DPB_BOTH:
        out->b_projected = 1;
        out->translateU = out->near1;
        out->scaleW = 1.0f;
        break;
    case DPB_SUN:
        out->b_projected = 0;
        out->scaleUfromZ = (float)(1.0 / out->farUsed);
        break;
    default:
        break;
    }
}

int DPB_b_Ordered(const tDpb_State* st)
{
    if (!st)
        return 0;
    return st->nearClamped > 0.0f
           && st->near1 >= st->nearClamped
           && st->type0 >= st->near1
           && st->type1 >= st->type0
           && st->farUsed >= st->type1;
}

float DPB_f_DepthAtStep(const tDpb_State* st, int step)
{
    /* 1/256 of the far plane per step, so the 256 entries span the
       frustum. */
    if (!st || st->kind != DPB_SUN)
        return 0.0f;
    return (float)((double)step * st->farUsed * 0.00390625);
}

unsigned int DPB_i_BlurAtDepth(const tDpb_State* st, float depth)
{
    if (!st || st->kind != DPB_SUN)
        return 0;
    /* Fully blurred behind the camera, none at the far plane, and
       nothing at all past it. */
    if (depth < 0.0)
        return 255;
    if (depth <= (double)st->farUsed)
        return (unsigned int)((1.0 - depth / st->farUsed) * 255.0);
    return 0;
}

/* One rising run of the ramp: `from` to `to`, going from no blur to
   full across however many entries that is. The last entry of the run
   is one short of full, because the step count starts at zero. */
static void DPB_l_Ramp(int* table, int from, int to, int span)
{
    int idx;
    int step;

    for (idx = from, step = 0; idx < to; idx++, step++)
        table[idx] = (unsigned int)((double)step * (255.0 / (double)span)) << 24;
}

void DPB_BuildRamp(const tDpb_State* st, int* table)
{
    int clearEnd;
    int rampEnd;
    int idx;
    int i;

    if (!st || !table)
        return;

    switch (st->kind)
    {
    case DPB_OFF:
        /* Solid, so the pass writes full alpha everywhere and the
           after-effects read no blur. */
        for (i = 0; i < 256; i++)
            table[i] = 0xFF000000;
        break;

    case DPB_FAR:
        /* Nothing until type0, then a rise to the far end. */
        /* type0 <= type1, so this is at most 256 -- and 256 exactly
           is reachable, at a setting large enough that the two came
           out equal. The memset below is then the whole table and the
           guard after it is what stops the division by zero. */
        clearEnd = (unsigned int)(st->type0 / st->type1 * 256.0);
        if (clearEnd > 256)
            clearEnd = 256;      /* only for a state built by hand */
        idx = 0;
        if (clearEnd > 0)
        {
            memset(table, 0, 4 * (size_t)clearEnd);
            idx = clearEnd;
        }
        if (clearEnd < 256)
            DPB_l_Ramp(table, idx, 256, 256 - clearEnd);
        table[255] = 0xFF000000;
        break;

    case DPB_NEAR:
        /* Full everywhere but the last entry, which the near blur uses
           as its own. */
        for (i = 0; i < 255; i++)
            table[i] = 0xFF000000;
        table[255] = 0;
        break;

    case DPB_BOTH:
        clearEnd = (unsigned int)(st->near1 / st->type1 * 256.0);
        rampEnd = (unsigned int)(st->near1 / st->type0 * 256.0);
        /* near1 <= type0 <= type1, so clearEnd <= rampEnd <= 256, and
           both reach 256 at a large enough setting. */
        if (clearEnd > 256)
            clearEnd = 256;      /* only for a state built by hand */
        if (rampEnd > 256)
            rampEnd = 256;
        idx = 0;
        if (clearEnd > 0)
        {
            memset(table, 0, 4 * (size_t)clearEnd);
            idx = clearEnd;
        }
        if (idx < rampEnd)
        {
            DPB_l_Ramp(table, idx, rampEnd, rampEnd - clearEnd);
            idx = rampEnd;
        }
        /* Preserved: a count used where an index belongs, so the solid
           run is mirrored about the middle of the table and stops short
           of 254 instead of reaching it. Writing `i < 255` here is the
           obvious reading and it is not what the game does. */
        if (idx < 255)
        {
            for (i = idx; i <= 255 - idx; i++)
                table[i] = 0xFF000000;
        }
        table[255] = 0;
        break;

    case DPB_SUN:
        for (i = 0; i < 256; i++)
        {
            /* Shifted as unsigned: the top byte's 255 does not fit the
               24 places in a signed int, which is undefined rather
               than merely wrapping. Same bits either way. */
            unsigned int blur;

            blur = (unsigned char)DPB_i_BlurAtDepth(st,
                                                    DPB_f_DepthAtStep(st, i));
            table[i] = (int)(blur << 24);
        }
        table[255] = 0;
        break;

    default:
        break;
    }
}
