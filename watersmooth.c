#include "watersmooth.h"

#include "depthblur.h"

#include <string.h>

/* The reflection's size, which is also the size the scratch rectangle
   is centred at. Written once here; the renderers have their own name
   for the same number. */
#define WSM_SIZE        512

void WSM_Reflection(tWsm_Rect* out, int size)
{
    if (!out)
        return;
    out->firstColumn = 0;
    out->firstRow = 0;
    out->lastColumn = size - 1;
    out->lastRow = size - 1;
    out->width = size;
    out->height = size;
}

void WSM_Scratch(tWsm_Rect* out, int width, int height)
{
    if (!out)
        return;

    /* Centred when the target is the larger, and the whole of the
       target when it is not. The two arms are not symmetric: the
       smaller case does not keep the middle of anything, it simply
       takes the lot, so a target under 512 wide blurs the reflection
       into a squashed rectangle rather than cropping it. That is the
       original's arithmetic and the comparison against it is what
       keeps it that way. */
    if (width <= WSM_SIZE)
    {
        out->firstColumn = 0;
        out->lastColumn = width - 1;
    }
    else
    {
        out->firstColumn = (width - WSM_SIZE) >> 1;
        out->lastColumn = out->firstColumn + WSM_SIZE - 1;
    }

    if (height <= WSM_SIZE)
    {
        out->firstRow = 0;
        out->lastRow = height - 1;
    }
    else
    {
        out->firstRow = (height - WSM_SIZE) >> 1;
        out->lastRow = out->firstRow + WSM_SIZE - 1;
    }

    out->width = width;
    out->height = height;
}

void WSM_Passes(tWsm_Pass* out, int size, int width, int height)
{
    tWsm_Rect reflection;
    tWsm_Rect scratch;

    if (!out)
        return;

    WSM_Reflection(&reflection, size);
    WSM_Scratch(&scratch, width, height);

    /* Out and back. The second pass is the first with its ends
       exchanged, which is what leaves the result in the reflection
       itself -- so nothing downstream has to know the blur happened. */
    out[0].from = reflection;
    out[0].to = scratch;
    out[1].from = scratch;
    out[1].to = reflection;
}

float WSM_f_Step(float amount, int last)
{
    if (last <= 0)
        return 0.0f;
    /* The last index, not the size. One pixel's difference at this
       scale, and it is what the original divides by. */
    return (float)((double)amount / (double)(unsigned int)last);
}

void WSM_Taps(tWsm_Tap* out, float uStep, float vStep)
{
    float weights[DPB_BLUR_TAPS];
    int   i;

    if (!out)
        return;
    memset(out, 0, sizeof(*out) * WSM_TAPS);

    /* Across: nothing, one step back, one step on, nothing again. Two
       of the four sample the same column, which is worth saying out
       loud because it reads like a transcription slip and is not. */
    out[0].u = 0.0f;
    out[1].u = -uStep;
    out[2].u = uStep;
    out[3].u = 0.0f;

    /* Down: nought, one, two, three steps -- but only at the bottom
       edge of the rectangle. At the top edge all four are nought, and
       between them it is interpolated, so the blur opens out towards
       the near edge of the water. */
    for (i = 0; i < WSM_TAPS; i++)
        out[i].vAtBottom = (float)i * vStep;

    /* The same chain the depth blur's taps are weighed by, asked for
       rather than written out again: both are a run of hardware stages
       mixing each sample into what came before by one fixed factor. */
    DPB_ChainWeights(weights);
    for (i = 0; i < WSM_TAPS && i < DPB_BLUR_TAPS; i++)
        out[i].weight = weights[i];
}
