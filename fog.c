#include "fog.h"

#include <math.h>
#include <string.h>

/* The engine's four curve numbers. The order is the engine's, not a
   choice made here: it is the order the one renderer with fog indexes
   its own constants with. */
static int FOG_l_i_Curve(int engineCurve)
{
    switch (engineCurve)
    {
    case 0:  return FOG_LINEAR;
    case 1:  return FOG_EXP;
    case 2:  return FOG_EXP2;
    case 3:  return FOG_OFF;
    default: return FOG_OFF;
    }
}

/* Red and blue the other way round, alpha and green where they were.
   The engine keeps its colours that way and every renderer has to
   undo it -- the vertex colours take the same swap. */
static unsigned int FOG_l_i_Swap(unsigned int c)
{
    return (c & 0xFF00FF00u)
           | ((c >> 16) & 0xFFu)
           | ((c & 0xFFu) << 16);
}

void FOG_Decode(const tFog_Source* src, tFog_State* out)
{
    if (!src || !out)
        return;

    memset(out, 0, sizeof(*out));
    if (!src->b_enabled)
        return;

    /* The flag is what says fog is on. The curve can still come out
       FOG_OFF, and that is not the same thing: the engine's fourth
       curve number names no distance curve, and the renderer that has
       fog today leaves fog *enabled* with that number -- which in its
       own terms means the factor comes off the vertices instead of
       off the distance. So the colour and the range are carried
       either way, and it is the curve that says where the factor
       comes from.

       Collapsing the two was the first draft of this, and the
       differential test against that renderer reported 44 differences
       for it: with the fourth number the colour came out black
       instead of the engine's. */
    out->b_on = 1;
    out->curve = FOG_l_i_Curve(src->engineCurve);
    out->start = src->start;
    out->end = src->end;
    out->density = src->density;
    out->color = FOG_l_i_Swap(src->engineColor);
    out->color2 = 0;
}

int FOG_b_Applies(const tFog_State* st, int b_fogAllowed)
{
    return st && st->b_on && b_fogAllowed;
}

unsigned int FOG_i_Color(const tFog_State* st, int b_useSecond)
{
    if (!st)
        return 0;
    return b_useSecond ? st->color2 : st->color;
}

float FOG_f_Factor(const tFog_State* st, float depth)
{
    double f;

    if (!st || !st->b_on)
        return 1.0f;

    switch (st->curve)
    {
    case FOG_LINEAR:
        /* The whole range at one distance is no range at all; the
           hardware's own answer there is undefined, so this gives the
           far end of the ramp rather than a division by zero. */
        if (st->end == st->start)
            f = depth < st->end ? 1.0 : 0.0;
        else
            f = ((double)st->end - depth) / ((double)st->end - st->start);
        break;
    case FOG_EXP:
        f = exp(-((double)depth * st->density));
        break;
    case FOG_EXP2:
    {
        double d = (double)depth * st->density;

        f = exp(-(d * d));
        break;
    }
    default:
        return 1.0f;
    }

    if (f < 0.0)
        f = 0.0;
    if (f > 1.0)
        f = 1.0;
    return (float)f;
}
