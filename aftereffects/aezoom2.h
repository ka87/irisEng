#pragma once

/* The second zoom smooth, with no renderer in it.
 *
 * The last of the after-effects and the strangest. Eight samples of the
 * frame, each **turned about the middle of the picture** by a
 * different angle, summed -- so it is a rotational smear, which is not
 * what its name says.
 *
 * Except that the name is right, and the reason is the finding in this
 * file. The transform is not a rotation. It is written as one, in
 * place, with the second line reading the variable the first line has
 * already overwritten:
 *
 *     u = du * cos + dv * sin;
 *     v = dv * cos -  u * sin;      <- u, not du
 *
 * A rotation's second line takes `du`. Taking `u` instead gives
 *
 *     v = dv * cos - (du * cos + dv * sin) * sin
 *       = -du * cos * sin + dv * (cos - sin * sin)
 *
 * which is still a linear map -- so interpolating it across a quad is
 * exact, and a renderer may apply it per pixel instead -- but its
 * **determinant is cos squared**, not one. Every tap is therefore
 * turned *and shrunk*, by more the further it is turned, and the eight
 * together sweep a short spiral inwards. That is a zoom smear with a
 * twist in it, and it is why the effect is called what it is called.
 *
 * Whether the author meant it is not knowable from here and does not
 * matter: it is what the picture is made of, and a tidied rotation
 * would be a different effect with the wrong name.
 *
 * Two more things that are not what they look like:
 *
 * **The turn happens in pixels, not in texture space.** The sample
 * coordinates are built in a 640 by 480 frame, turned there, and only
 * then divided down to the unit square. So in texture coordinates the
 * map is the turn conjugated by the frame's aspect -- the off-diagonal
 * terms are scaled by four-thirds one way and three-quarters the
 * other, and a circle does not come back a circle.
 *
 * **The eight angles are not centred.** They run from four steps back
 * to three steps on, so there is one more sample behind the frame than
 * ahead of it -- the same sort of lopsidedness as the motion smooth's
 * taps, and not a rounding of a symmetric range.
 */

/* The effect's bit in the after-effects word. */
#define AZ2_BIT         0x8000u

/* Whether it runs: the bit, and a parameter that is not exactly
   nought. Not a floor -- a negative parameter turns the samples the
   other way and is legal. */
int AZ2_b_On(unsigned int aeMaster, float zoom);

/* How many samples the smear takes when the hardware can take them
   all. */
#define AZ2_TAPS        8

/* How the other renderer splits the samples between texture stages and
 * draws, from the stages a card offers and the engine's own setting.
 *
 * It is here because the split decides the **angles**, not just the
 * drawing: the angle walks forward one step per sample and is
 * re-accumulated once per draw, so four stages in two draws and two
 * stages in four draws round the same eight angles differently. Both
 * are reachable -- the engine setting chooses, not the card -- and
 * neither is wrong.
 *
 * `engStages` is that setting. A card offering three stages is the one
 * case where the eight do not divide: it takes six samples, not eight,
 * and the weight below changes with it. */
int   AZ2_i_Stages(int simultaneousTextures, int engStages);
int   AZ2_i_Passes(int stages);
int   AZ2_i_Samples(int stages);

/* How many angles a draw carries, which is how many coordinate sets
 * the quad has -- four -- and **not** how many of them are sampled.
 *
 * A draw with two stages still carries four coordinate sets and still
 * computes all four angles; the last two are written into the vertex
 * data and never read. That is not an oversight to tidy: the vertex
 * data is what it is, and a renderer comparing itself against this one
 * compares those too. */
#define AZ2_ANGLE_SLOTS 4

/* The angles, four to a draw, in draw order: slots 0 to 3 of draw
 * nought, then of draw one, and so on. So `out` takes
 * AZ2_i_Passes(stages) * AZ2_ANGLE_SLOTS, and the first `stages` of
 * each four are the ones a draw actually samples.
 *
 * `stages` is what AZ2_i_Stages answered, because the accumulation
 * depends on it -- see above. Returns how many were written.
 *
 * The walk is reproduced rather than computed from a closed form: the
 * angle is a float that is advanced once per draw and offset within a
 * draw, so a closed form would be right about the angles and wrong
 * about their last bits. */
int   AZ2_i_Angles(float* out, int max, float zoom, int stages);

/* The sampled angles alone, in sampling order -- the first `stages` of
 * each four above, run together. For a renderer that takes all the
 * samples in one draw and so has no use for the slots nobody reads.
 *
 * Returns how many were written, which is AZ2_i_Samples' answer
 * bounded by `max`. */
int   AZ2_i_SampledAngles(float* out, int max, float zoom, int stages);

/* What each sample is multiplied by on its way into the sum.
 *
 * `256 / samples - 1` of 255, which is the same deliberate undershoot
 * the first zoom smooth carries: eight samples come to 248 of 255 and
 * the smear is a shade dark rather than saturating. */
float AZ2_f_Weight(int samples);

/* One sample's transform, as the four entries of a two-by-two in row
 * order, mapping a texture coordinate measured from the middle of the
 * picture.
 *
 * Already conjugated by the frame's aspect, so a caller hands it
 * texture coordinates and not pixels -- and already with the vertical
 * measured **up** the picture, which is this interface's direction and
 * the opposite of the one the transform is written in. Both of those
 * change the off-diagonal terms, so neither is a caller's business to
 * get right.
 *
 * `out4` takes four. */
void  AZ2_Tap(float* out4, float angle);

/* The determinant of that transform, which is cos squared -- offered
   because it is the whole argument for the effect's name and a test
   should be able to ask for it rather than rebuild it. */
float AZ2_f_TapScale(float angle);
