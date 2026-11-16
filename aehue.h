#pragma once

/* The hue sweep, with no renderer in it.
 *
 * One of the after-effects, and the one with the least to look at and
 * the most to get wrong. What it does is subtract a colour from the
 * picture -- a different amount from each channel, saturating at
 * black -- and the three amounts are **one wave sampled at three
 * phases a third of a cycle apart**. So at any moment one channel is
 * left alone while the other two are darkened, and as the phase
 * advances which channel that is sweeps round: the picture is tinted,
 * and the tint cycles.
 *
 * Two parameters, both moved by the scripts: how deep the tint goes,
 * and where in the cycle it is. There is a third slot the scripts can
 * set and read back, and **nothing anywhere reads it** -- it is not
 * modelled here because there is nothing to model.
 *
 * Everything below is arithmetic on a byte per channel, and the reason
 * it is worth a file is that none of it is visible in a picture. A
 * wrong wave is a tint of a slightly different colour; a wrong phase
 * offset is a tint of the wrong colour, which looks just as
 * deliberate.
 */

/* The effect's bit in the after-effects word. The effect runs on the
   bit alone -- there is no parameter to gate it, so a depth of nought
   is a draw that subtracts nothing rather than no draw at all. */
#define AHU_BIT         0x4u

int AHU_b_On(unsigned int aeMaster);

/* The three channels, in the order the packed word below holds them:
   red, green, blue. */
#define AHU_CHANNELS    3

/* The wave every channel samples: a flat-topped trapezoid of period
 * one, folded about the half.
 *
 * One for the first sixth, a straight ramp down to nothing over the
 * second sixth, nothing for the third -- and then the mirror of that,
 * because the fold makes the second half of the cycle the first half
 * backwards. What is subtracted from a channel is *one less* this, so
 * a channel is untouched where its own wave is on the flat top and
 * fully darkened where the wave has reached nothing.
 *
 * Three things about it are not tidy and are all kept:
 *
 * **The wrap is a single subtraction**, not a remainder. A phase above
 * one has one taken off it and no more, so a phase of five folds to
 * minus three and a half, lands under the first threshold, and returns
 * one -- which subtracts nothing. A phase below nought does the same
 * without even wrapping. So a phase outside the cycle does not wrap
 * into it: it switches the effect off, and does so lopsidedly.
 *
 * **A sixth and a third are spelled three different ways** -- the
 * threshold the ramp starts at is a hair *below* the value the ramp
 * measures from. Between the two the ramp returns slightly more than
 * one, which makes what is subtracted slightly less than nothing. It
 * is a window seven parts in a thousand million wide and a real phase
 * reaches it; see AHU_u_Channel, where it comes to nothing subtracted
 * either way.
 *
 * **The ramp's slope is six and the window is a sixth**, so the ramp
 * really does span exactly one sixth -- that much is consistent, and
 * it is worth saying because the two spellings above make it look as
 * though it might not be.
 *
 * **Neither threshold can ever be an equality**, so which way round the
 * two comparisons are written cannot be seen. The thresholds are
 * decimal *doubles* and the value compared against them is always a
 * float, or one minus a float, which is exact -- and neither decimal
 * is a float. So no input reaches the boundary, and a sweep confirms
 * that moving either comparison changes nothing.
 */
float AHU_f_Wave(float phase);

/* Where a channel samples the wave: its own phase, a third of a cycle
 * further on for each channel after the first.
 *
 * The thirds are spelled as a subtraction and then a whole cycle added
 * back, which is what keeps the argument positive for the single wrap
 * above to cope with.
 *
 * The two constants are written as different decimals and **are
 * exactly the same float as one another doubled** -- 0.66666663f and
 * 2.0f * 0.33333331f have identical bits. So carrying both decimals
 * rather than doubling one is a matter of spelling and not of value,
 * and a sweep cannot tell the two apart. The decimals are kept
 * because they are what is written; a first draft of this comment
 * claimed they differed, and the sweep is what caught that. */
float AHU_f_ChannelPhase(float phase, int channel);

/* One channel's amount, as the whole number out of 255 that the other
 * renderer puts in a byte of a colour.
 *
 * Truncated, so the amounts step rather than slide, and **not
 * clamped**: a depth above one gives a number past 255, and what
 * happens to the extra is AHU_u_Subtract's business rather than a
 * clamp here. The negative sliver the wave's untidy threshold can
 * produce truncates to nothing. */
unsigned int AHU_u_Channel(float amount, float phase);

/* The three channels packed as one colour, exactly as the other
 * renderer packs them -- and this is the function with the finding in
 * it.
 *
 * The packing is three bytes OR-ed into a word that starts as all
 * ones in its upper three bytes, shifted up a byte between each. That
 * mask is there to leave the colour's alpha at full without a fourth
 * constant, and it has a consequence nobody wrote down: **the three
 * channels overflow differently.**
 *
 * With a depth above one, a channel's amount passes 255 and does not
 * fit its byte. Then
 *
 *   - **red's overflow is swallowed.** Its bit above the byte lands in
 *     the mask's own ones, which are already set, so it is simply
 *     lost: red wraps modulo 256 and a channel that should be fully
 *     dark comes back part-lit.
 *   - **green's and blue's overflow carries upwards.** Each is OR-ed
 *     into a word whose byte above it is a real channel, so the bit
 *     past the byte sets the bottom bit of the channel above --
 *     green's into red, blue's into green.
 *
 * At a depth of one and a half this is plainly visible: a phase that
 * should give a fully dark red and blue gives red 50, green 1 and blue
 * 126 instead. The 1 in green is blue's overflow; the 50 is red's
 * amount of 306 with its high bit discarded.
 *
 * So the packing is the effect at depths above one, and a renderer
 * that computed three clamped channels instead would be right about
 * the arithmetic and wrong about the picture. That is why this returns
 * the word and not three numbers. */
unsigned int AHU_u_Subtract(float amount, float phase);

/* The packed word back out as the three fractions a shader wants, in
   red, green, blue order. `out` takes AHU_CHANNELS. */
void AHU_Channels(float* out, unsigned int packed);
