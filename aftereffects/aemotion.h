#pragma once

/* The motion smooth, with no renderer in it.
 *
 * A directional smear along whichever way the camera just moved. Four
 * samples of the frame, offset along that direction, summed -- so in
 * what it computes it is a fourth member of aeframe.h's family, and it
 * is not in that file because **nothing else about it is like them**:
 *
 * The other three are switched on by a bit and scaled by a parameter
 * a script sets. This one is switched on by a bit and scaled by **how
 * far the camera moved since last frame**, which means it carries
 * state across frames and has no parameter at all. A script can turn
 * it on and then has no further say in how strong it is.
 *
 * What it watches is one **direction** -- the unit corner, carried
 * through the camera's matrix. A direction and not a point, because
 * the transform that carries it applies the matrix's rotation and
 * scale and **not its translation**. So:
 *
 *   - a camera that turns smears, and the smear follows the turn;
 *   - a camera that flies in a straight line without turning smears
 *     **nothing at all**, however fast it travels.
 *
 * That is worth being sure of rather than assuming, and it was assumed
 * wrongly here first: a note in this file read "a point and not a
 * direction" and had the consequence backwards. Reading the transform
 * settled it -- it has no translation term -- and the renderer tests
 * now move the camera by rotating it, because translating it produces
 * no effect to test.
 *
 * Three things follow from the way the movement is measured, and all
 * three are the other renderer's:
 *
 * **It is per-frame and never accumulates.** Last frame's point is
 * replaced every frame whether or not the smear ran, so a camera
 * turning slowly enough never crosses the threshold and never smears,
 * however far round it eventually comes.
 *
 * **The test is in three dimensions and the smear is in two.** The
 * distance that decides whether to smear includes the movement towards
 * and away from the camera; the smear itself only uses the other two.
 * So a camera moving straight along its own line of sight crosses the
 * threshold and then smears by nothing.
 *
 * **The movement is a difference of two points, so it coarsens as the
 * camera travels.** Far from the origin the two points share most of
 * their bits and the difference keeps only what is left: at a base of
 * thirty a movement of 0.02 reads as 0.0200005, and a million units
 * out a movement of 0.006 does not read at all -- the two directions
 * come back equal and the camera looks still. So the threshold is not a
 * fixed sensitivity; it is a fixed number compared against a
 * measurement whose precision depends on where the camera is. Nothing
 * here can fix that, because it follows from watching a transformed
 * point rather than a velocity. It is written down because a test
 * fixture that put the point anywhere but the origin could not pin
 * either the threshold or the gain, and the first draft of this
 * module's test did exactly that and failed on both.
 *
 * **The step is a fixed fraction of the picture, not a count of
 * pixels.** The two scales are a six-hundred-and-fortieth and a
 * four-hundred-and-eightieth, written as constants rather than taken
 * from the frame's real size, so the smear covers the same fraction of
 * the picture whatever resolution it runs at.
 */

/* The effect's bit in the after-effects word. No parameter goes with
   it -- see above. */
#define AMO_BIT         0x80u

int AMO_b_On(unsigned int aeMaster);

/* The direction whose movement is watched, before the camera's matrix
 * is applied to it: the unit corner. `out` takes three.
 *
 * It is here rather than written at the call site because it is the
 * one number in the effect that says *what* is being watched, and a
 * different point would give a different smear for the same camera
 * move. */
void AMO_Probe(float* out);

/* One frame of the decision, from this frame's transformed point and
 * last frame's.
 *
 * Returns whether to smear and, if so, writes how far across and down.
 * Either output may be null. The caller keeps the two points, because
 * where last frame's is stored is the renderer's business -- and the
 * caller must store this frame's over it **whatever the answer**, or
 * the movement starts accumulating and a slow camera eventually
 * smears when it should not.
 *
 * The arithmetic inside is mixed on purpose and reproduces the other
 * renderer's exactly: the distance across is widened to double before
 * it is squared while the other two are squared narrow, so the three
 * terms are not computed alike and the sum is not the sum a tidier
 * spelling would give. That only shows within a hair of the threshold,
 * which is precisely where it decides something. */
int AMO_b_Decide(float* outDx, float* outDy, const float* now,
                 const float* before);

/* How many samples the smear takes. */
#define AMO_TAPS        4

/* Where the four samples sit, in texture coordinates, for a smear of
 * this size. `uOut` and `vOut` take AMO_TAPS each; either may be null.
 *
 * The four are at minus three, plus two, minus one and nothing times
 * the step -- in that order, which is the order the other renderer
 * binds them in and so the order they are summed in. It is not a
 * symmetric spread and it is not sorted: three steps back, two
 * forward, one back, and the sample itself. Whatever that was meant to
 * be, it is what the picture is made of. */
void AMO_Taps(float* uOut, float* vOut, float dx, float dy);

/* What each of the four samples is multiplied by on its way into the
 * sum.
 *
 * Not a quarter. The other renderer carries the weight in a vertex
 * colour of 63 in every byte, so each sample comes in at 63 of 255 and
 * the four together come to 252 of 255 -- the smeared picture is a
 * shade darker than the one it was made from, by about one part in
 * eighty. That is small, consistent, and real. */
float AMO_f_Weight(void);

/* Whether a renderer with this many texture units at once can take all
 * four samples in one pass.
 *
 * The other renderer has two ways of drawing this and they compute the
 * same picture: four samples in one draw where it can, and two draws
 * of two added together where it cannot. The split one rounds to eight
 * bits in the middle and so is a shade different, but it is the same
 * arithmetic, which is why only the number of units decides.
 *
 * It is here so that the fact is written down once. A renderer that
 * always has four units has no use for the answer. */
int AMO_b_OnePass(int simultaneousTextures);
