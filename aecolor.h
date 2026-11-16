#pragma once

/* The after-effects that are a colour transform and nothing else, with
 * no renderer in them.
 *
 * Three of them run at the end of the chain, each reading the frame
 * and writing a colour per pixel with no regard to any neighbour:
 * brightness, contrast and grayscale. None of them needs a target of
 * its own, none of them reads the frame's alpha, and none of them
 * writes it -- which is what makes them one group rather than three
 * unrelated effects.
 *
 * What is here is the arithmetic each one comes to. The renderer that
 * has them today spells all three as texture stages over a quad, and
 * the stages are a roundabout way of saying something short: the
 * numbers below are that short form, worked out from what the stages
 * compute rather than chosen.
 *
 * They are **clamped between steps**, and that is not a detail. Each
 * is a separate draw into an eight-bit target there, so a channel
 * pushed past white by the brightness is white before the contrast
 * sees it. Composing the three in one pass without clamping between
 * them gives a different and brighter picture at the extremes.
 */

#define AEC_MAX_STEPS   3

#define AEC_BRIGHTNESS  1   /* a grey added to or taken off every channel */
#define AEC_CONTRAST    2   /* a gain about mid grey */
#define AEC_GRAYSCALE   3   /* a weighted sum of the channels */

/* What the decision is made from. All three are values the engine
   already holds. The two parameters are neutral at a half rather than
   at zero, and the engine puts them back to a half itself when the
   effects are switched off. */
typedef struct sAec_Source
{
    unsigned int aeMaster;
    float        brightness;
    float        contrast;
} tAec_Source;

/* One step. `gain` and `bias` are a channel's multiply and add; the
   grayscale uses `weights` as well and applies the gain and the bias
   to the sum rather than to a channel.
 *
 * So every step is
 *
 *     brightness, contrast   out_c = clamp(in_c * gain + bias)
 *     grayscale              out_c = clamp(dot(in, weights) * gain
 *                                          + bias), all channels alike
 */
typedef struct sAec_Step
{
    int   kind;
    float gain;
    float bias;
    float weights[3];
} tAec_Step;

/* The steps that apply, in the order they run. Returns how many were
   written. `out` takes AEC_MAX_STEPS of them.
 *
 * The order is the one the chain runs them in and it matters: with the
 * clamp between them, brightness then contrast is not contrast then
 * brightness. */
int AEC_i_Steps(const tAec_Source* src, tAec_Step* out, int max);

/* Mid grey as the engine writes it -- 128 of 255, which is a shade
   above a half and is the value the contrast pivots on. */
float AEC_f_MidGrey(void);
