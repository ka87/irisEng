#pragma once

/* The depth blur's arithmetic, with no renderer in it.
 *
 * The blur is not a post-process pass over the finished frame. It is a
 * 256 by 1 ramp of blur-amount-per-depth that the world's geometry is
 * drawn through: the texture coordinate comes from the camera-space
 * position, so the coordinate is the depth, and only the alpha channel
 * is written -- what the pass leaves behind is a per-pixel blur amount
 * in the frame's alpha, which the after-effects then read.
 *
 * The sun's occlusion test reads the same alpha, which is why the two
 * are one piece of work and why there is only one blur mode in which
 * the sun has anything to read.
 *
 * Everything here is the decision and the table, which are the same
 * whichever renderer draws the pass. What stays with a renderer is how
 * the ramp becomes a texture, how the camera-space coordinate is
 * generated, and how the write is narrowed to alpha.
 */

/* Which blur is in force. These are the engine's own numbers, not a
   fresh set: other code tests them directly -- the sun samples only in
   mode 4 -- so renaming the values would be renaming them in one place
   of two. */
#define DPB_OFF        0   /* no blur; the ramp is solid */
#define DPB_FAR        1   /* far blur only */
#define DPB_NEAR       2   /* near blur only */
#define DPB_BOTH       3   /* near and far */
#define DPB_SUN        4   /* the depth ramp the sun's test reads */

/* What the decision is made from. Every field is a value the engine
   already holds; none of it is read from here. */
typedef struct sDpb_Source
{
    int          b_zonAlphaTweak;  /* the tweak that turns the blur off */
    unsigned int aeMaster;         /* the after-effects master word */
    int          b_sunEffectOn;    /* any sun wants its occlusion test */
    float        nearPlane;
    float        farPlane;
    float        depthBlurNear1;   /* the three AI-script settings */
    float        depthBlurType0;
    float        depthBlurType1;
} tDpb_Source;

/* The decision, and the four distances it is built on.
 *
 * The distances come out ordered, nearClamped <= near1 <= type0 <=
 * type1 <= farUsed, and that is what the ramp rests on: its two
 * divisions are of a smaller distance by a larger one, so neither
 * ratio exceeds one and neither index runs past the end of the table.
 *
 * Ordered, but not separated. Each distance is built by adding a
 * hundredth to the one below and storing the sum back into a float,
 * and past about 2^23 the hundredth is below the float's resolution --
 * so at a blur setting of a million the sum is the value it was meant
 * to clear and the two come out *equal*. The ratio is then exactly
 * one and the index exactly 256, one past the last entry, which every
 * arm of the ramp is written to tolerate. An earlier version of this
 * comment claimed strict separation; it is not true, and the ramp
 * would divide by zero if it were relied on.
 *
 * DPB_b_Ordered says whether a state holds, for a state that was built
 * by hand rather than decoded. */
typedef struct sDpb_State
{
    int   kind;          /* one of DPB_* above */
    float nearClamped;   /* the near plane, floored away from zero */
    float near1;
    float type0;
    float type1;
    float farUsed;     /* the far plane, pushed out past type1 */

    /* How the texture coordinate is made, named for what it does
       rather than for where a renderer puts it. With b_projected the
       generated coordinate is divided through by its third component;
       without it, it is taken as it is. */
    int   b_projected;
    float translateU;   /* added to u */
    float scaleW;       /* the third component, when it is divided through */
    float scaleUfromZ;  /* u per unit of camera-space depth */
} tDpb_State;

/* The whole decision, from the engine's state. */
void DPB_Decode(const tDpb_Source* src, tDpb_State* out);

/* Are the distances in order, so the ramp's divisions stay inside the
   table? Non-strict, because equal neighbours are reachable -- see
   above. True for anything DPB_Decode produced; it is here so a
   hand-built state can be checked rather than trusted. */
int DPB_b_Ordered(const tDpb_State* st);

/* The depth the ramp's step'th entry stands for, and the blur amount
   at a depth -- 0 to 255. Only DPB_SUN has a depth-to-blur relation at
   all; the other modes answer zero, which is what they did. */
float        DPB_f_DepthAtStep(const tDpb_State* st, int step);
unsigned int DPB_i_BlurAtDepth(const tDpb_State* st, float depth);

/* The ramp itself: 256 entries, each a blur amount in the top byte and
   nothing in the other three, which is the layout the texture wants.
   `table` must have room for 256. */
void DPB_BuildRamp(const tDpb_State* st, int* table);


/* ------------------------------------------------------------------
 * The after-effect that consumes the alpha the pass above wrote.
 *
 * Three widening blurs of the frame, ping-ponged between two spare
 * targets, and then one draw that mixes the sharp frame with the
 * blurred one using the sharp frame's own alpha as the mix. Alpha 255
 * takes the sharp pixel and 0 the blurred one, which is why the ramp
 * is solid 255 when no blur is in force.
 *
 * The schedule is the part worth stating away from a renderer: which
 * target each pass reads and writes, and how far it reaches. Getting
 * the ping-pong wrong gives a blurred picture either way, so nothing
 * about the look of it would say so.
 */

/* The slots a pass names. Three are enough because no pass reads and
   writes the same one. */
#define DPB_SLOT_FRAME  0   /* the frame as drawn, alpha and all */
#define DPB_SLOT_A      1
#define DPB_SLOT_B      2

#define DPB_BLUR_PASSES 3
#define DPB_BLUR_TAPS   4

typedef struct sDpb_BlurPass
{
    int   fromSlot;
    int   toSlot;
    float uStep;   /* in texture space, so a fraction of the width */
    float vStep;
} tDpb_BlurPass;

/* Does the after-effect run? Two bits ask for it and either will do.
   The master word is already zero when the effects are switched off
   wholesale, so that gate is not repeated here. */
int DPB_b_AfterEffect(unsigned int aeMaster);

/* The three passes, in order. Returns how many were written, so a
   caller that sized its array from DPB_BLUR_PASSES gets told rather
   than assuming. */
int DPB_i_BlurSchedule(tDpb_BlurPass* out, int max);

/* Which slot the last pass left the blurred frame in -- the one the
   final mix samples beside the frame. Derived from the schedule rather
   than written down twice. */
int DPB_i_BlurResultSlot(void);

/* Where one pass's four taps sit, relative to the pixel being written,
   and how much of each lands in the result.
 *
 * The offsets are in screen terms: du to the right, dv *down*, which
 * is the engine's own direction for a texture coordinate. A renderer
 * whose textures run the other way up has to flip dv, and it matters
 * -- the weights are not symmetric.
 *
 * They are not symmetric because of how the four taps are combined on
 * the hardware this was written for: each tap is mixed half-and-half
 * with everything accumulated so far, so the last tap keeps half the
 * result, the one before a quarter, and the first two split what is
 * left. They come to one, because every step of that is a mix rather
 * than an addition -- but they are only near a half, a quarter and
 * two eighths, because the factor the hardware mixes with is 128 of
 * 255 and not an exact half. That is also the whole reason the two
 * taps across weigh differently from each other: nothing separates
 * them but which one the mixing reached first.
 *
 * The weights are worked out from the mix rather than listed, so the
 * near-halves stay wherever that arithmetic puts them.
 *
 * `offsets` takes 2 * DPB_BLUR_TAPS floats and `weights` takes
 * DPB_BLUR_TAPS; either may be null. */
void DPB_BlurTaps(const tDpb_BlurPass* pass, float* offsets, float* weights);
