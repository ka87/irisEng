#pragma once

/* The after-effects that build a frame out of several samples of one,
 * with no renderer in them.
 *
 * Four of them, and what makes them a group is that each one's output
 * is a sum of frames rather than a function of a pixel:
 *
 *   motion blur   the frame now and the frame before, mixed
 *   remanence     the frame and a wide blur of it, added -- a bloom,
 *                 whatever its name suggests
 *   zoom smooth   the frame sampled at eight scales about the centre,
 *                 added
 *   screen blur   the frame blurred seven times over, each pass
 *                 reaching further than the last
 *
 * They are not one effect and they do not share a shader. What they
 * share is that the thing to get right is a schedule and a set of
 * weights, and that neither is visible in the picture: a wrong weight
 * is a slightly different smear and a wrong schedule is a smear of
 * the wrong thing.
 *
 * All four are switched on by a bit and scaled by a parameter, and for
 * all four the parameter's neutral is nought -- unlike the colour
 * effects, whose neutral is a half.
 */

/* What the decision is made from. Every field is a value the engine
   already holds. */
typedef struct sAef_Source
{
    unsigned int aeMaster;
    float        motionBlur;
    float        remanence;
    float        zoomSmooth;
    float        screenBlur;

    /* The bottom of the letterbox band, as a fraction from the top.
       Only the zoom reads it, and only because its horizontal step
       divides by it -- see AEF_i_ZoomTaps. */
    float        bandBottom;
} tAef_Source;


/* ------------------------------------------------------ motion blur
 *
 * One draw of the frame over whatever the target already holds, mixed:
 *
 *     out = now * (1 - keep) + before * keep
 *
 * "Whatever the target already holds" is the whole effect, and it is
 * the only after-effect that reads across frames. There is no separate
 * history buffer: the target is simply not cleared, so what is in it
 * is what this effect wrote into it last frame.
 *
 * That puts a condition on the renderer rather than on the arithmetic.
 * The target an effect writes has to be **the same target next
 * frame**, or what it reads as the frame before is some other effect's
 * leftovers. A chain that alternates between spares holds that only
 * while the set of effects running is unchanged -- turn another effect
 * on and every effect after it moves along one. Nothing here can fix
 * that; it is written down because the renderer has to know it.
 */
int   AEF_b_MotionBlur(const tAef_Source* src);

/* How much of the frame before survives, nought to one. Quantised to a
   byte, because the renderer that has this carries it as one. */
float AEF_f_MotionBlurKeep(const tAef_Source* src);


/* -------------------------------------------------------- remanence
 *
 * Four widening blurs of the frame and then one draw adding the result
 * to the sharp frame twice over:
 *
 *     out = sharp + 2 * opacity * blurred
 *
 * Twice because the renderer that has this adds the blurred copy in
 * two successive stages with the same coordinates and the same weight,
 * which is one addition said twice -- not two different samples.
 *
 * The blurs are the same pass the depth blur uses, which is why the
 * schedule comes back in the same type; the reach is different, 1, 3,
 * 5 and 7 pixels against that effect's 1, 2 and 3.
 */
int   AEF_b_Remanence(const tAef_Source* src);

#define AEF_REMANENCE_PASSES 4

/* How far each of the four blurs reaches, in order, as a fraction of
   the texture. Returns how many were written; either array may be
   null.
 *
   Reaches and not a schedule: which target each pass reads and writes
   is the renderer's to decide, because how many targets there are to
   go round is a fact about the renderer and not about the effect. The
   depth blur's schedule does name its slots, and can, because it is
   always the first effect in the chain and so always starts from the
   frame. This one can start from whatever the effects before it left,
   and in a renderer that cannot write the frame mid-chain the slots
   that follow depend on that. */
int   AEF_i_RemanenceReaches(float* uSteps, float* vSteps, int max);

/* What the blurred copy is multiplied by on its way into the sum --
   twice the opacity, with the opacity quantised to a byte. It can
   exceed one, and that is not a mistake: the sum saturates, which is
   what makes a bloom bloom. */
float AEF_f_RemanenceWeight(const tAef_Source* src);


/* ------------------------------------------------------ zoom smooth
 *
 * The frame added to itself eight times, each at a slightly tighter
 * crop about the centre of the band -- so a pixel is smeared along the
 * line from the centre through it, which is what a zoom looks like.
 *
 * Every tap weighs the same, and the weight is just under an eighth:
 * `256 / 8 - 1` of 255 rather than `255 / 8`. So the eight together
 * come to 248 of 255 and the result is a shade dark rather than
 * saturating -- the same flavour of deliberate undershoot as the
 * grayscale's weights.
 */
int   AEF_b_ZoomSmooth(const tAef_Source* src);

#define AEF_ZOOM_TAPS 8

/* One tap: how far in from each side of the band to sample, as a
   fraction of the texture. The crop is symmetric, so this is the inset
   on the left and on the right alike, and likewise top and bottom. */
typedef struct sAef_ZoomTap
{
    float insetU;
    float insetV;
} tAef_ZoomTap;

/* The eight taps, from the untouched frame inwards. Returns how many
   were written. */
int   AEF_i_ZoomTaps(const tAef_Source* src, tAef_ZoomTap* out, int max);

/* The weight every tap carries. */
float AEF_f_ZoomWeight(void);

/* ------------------------------------------------------ screen blur
 *
 * The frame blurred seven times over, each pass reaching one step
 * further than the last, every pass being the same four-tap blur the
 * depth blur is built from. So the result is a wide, even blur of the
 * whole picture -- and it is the only one of the four whose parameter
 * scales the *reach* rather than a weight.
 *
 * Two things are worth knowing before reading the reaches.
 *
 * **The parameter is a reach and not a fraction.** A step is the
 * parameter times about a twenty-fifth, and a pass's reach is that
 * times the pass number, in pixels of a 640 by 480 picture. So a
 * parameter of 25 puts the last pass seven pixels out, and a parameter
 * of one puts it under a third of a pixel out.
 *
 * **A reach under a pixel still costs brightness**, which is the part
 * that surprises. The passes do not move anything anywhere -- every
 * tap lands on the pixel it started from -- but a tap a fraction of a
 * pixel out is *interpolated*, so it brings back a mixture of its
 * pixel and the next one, and seven passes of that compound. At a
 * reach of one a lone bright row comes back at about half its
 * brightness without having spread at all. So "too small to see" is
 * not the same as "no effect", and the gate below does not stop a
 * reach that small being asked for.
 *
 * **The gate is a comparison against nought and not a floor.** Unlike
 * the remanence, which skips anything under a hundredth, this runs for
 * any value that is not exactly nought -- including a **negative**
 * one, which steps the taps the other way. The blur's four taps are
 * not symmetric, so a negative reach is not the same picture as a
 * positive one; it is a different blur, and a legal one.
 */
int   AEF_b_ScreenBlur(const tAef_Source* src);

#define AEF_SCREENBLUR_PASSES 7

/* The seven reaches, in order, for a parameter of `strength`. Returns
 * how many were written. Either array may be null.
 *
 * Reaches and not a schedule, for the reason AEF_i_RemanenceReaches
 * gives: which target a pass reads and writes is the renderer's to
 * decide.
 *
 * **The first pass's vertical reach is computed differently from the
 * other six**, and that is not a slip in the reading. The step is a
 * double that is then narrowed to a float; the first pass's horizontal
 * reach comes from the narrowed step and its vertical reach from the
 * **unnarrowed** product, while all six that follow take both from the
 * narrowed one. The difference is a single rounding and no picture
 * could show it, so it is here because it is what the other renderer
 * computes and for no other reason.
 *
 * The two scales are this effect's own doubles and **not** the family's
 * AEF_STEP_U and AEF_STEP_V, which are floats. The horizontal one is a
 * power of two and comes to the same number either way; the vertical
 * one does not, and taking it from the float would move every reach
 * here. */
int   AEF_i_ScreenBlurReaches(float* uSteps, float* vSteps, int max,
                              float strength);
