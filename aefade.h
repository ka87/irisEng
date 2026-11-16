#pragma once

/* The fade, with no renderer in it.
 *
 * One of the after-effects, and the odd one out in three ways that all
 * matter to whoever wires it up:
 *
 * **It freezes a frame and dissolves out of it.** On the frame it
 * starts, the picture is copied aside; from then on what is shown is
 * that frozen copy mixed with the live frame, and the frozen copy's
 * share falls to nothing. So the first frame of a fade is identical to
 * no fade at all -- the copy was just taken from the live picture --
 * and the last is the live picture again. In between the world moves
 * underneath a still image of itself.
 *
 * **It carries a timer.** Every other effect is a function of its
 * parameter and this frame's picture. This one has a remaining
 * opacity that it steps down itself, which is why it is the only
 * effect whose arithmetic has a *next* as well as a *now*.
 *
 * **It switches itself off.** When the opacity reaches nothing the
 * effect clears its own bit in the after-effects word, so the scripts
 * can poll it and wait. Nothing else in the chain does that, and it
 * means the fade's bit is script-visible state rather than a request.
 *
 * The one thing to know before reading any of it: the timer is kept as
 * a **byte of opacity**, not as a time, and the step comes out of a
 * division that is then **truncated to a whole number**. Both of those
 * shape what durations are actually achievable -- see AFD_f_Step.
 */

/* The opacity a fade starts at, and the width of the whole range: the
   timer counts down from this to nought. */
#define AFD_FULL        255

/* The effect's bit in the after-effects word.
 *
 * Spelled once, here, because three places need it and one of them is
 * unlike anything else in the chain: this is the only effect that
 * *writes* the word, clearing its own bit when the timer runs out. The
 * scripts read that back, so the bit is the fade's answer to "are you
 * still going" and not just a request to run. */
#define AFD_BIT         0x40000u

/* Whether the fade is running, given the after-effects word. */
int AFD_b_On(unsigned int aeMaster);

/* The same word with the fade switched off -- what to write back when
   the timer has run out. */
unsigned int AFD_u_Cleared(unsigned int aeMaster);

/* How much comes off the opacity each frame, for a fade asked to last
 * `seconds`.
 *
 * The span is the duration in frames at sixty a second, and the step
 * is the whole range over that span -- so a half-second fade steps by
 * 8.5 and a four-second one by 1.06.
 *
 * **The clamp at one is load-bearing, and does not read that way.**
 * The step is truncated to a whole number before it is taken (see
 * AFD_u_Taken), so a step below one takes *nothing* and the fade never
 * finishes: the picture stays frozen and the bit never clears. Every
 * span longer than the range itself would produce such a step, and the
 * clamp is the only thing standing in front of it. So it is not a
 * tidy-up -- without it, asking for a fade longer than 255 frames
 * freezes the screen for good.
 *
 * The same truncation means a fade **always runs a little long** and
 * only lands exactly where asked when the step divides the range:
 *
 *     asked   span    step   taken   actual
 *     0.50s     30    8.50       8    0.53s
 *     1.00s     60    4.25       4    1.07s
 *     2.00s    120    2.13       2    2.13s
 *     4.00s    240    1.06       1    4.25s
 *     8.00s    480    1.00       1    4.25s
 *
 * and that last row is the ceiling: **no fade can last longer than
 * 4.25 seconds**, however long it is asked for.
 *
 * A span of nought cannot be divided by, and the answer here is a step
 * of nought -- which takes nothing, so such a fade never finishes.
 * That is the other renderer's behaviour rather than a choice made
 * here, and there is a pick in the notes recording why it was kept.
 * The same answer is given for a span so small that the step will not
 * fit in a whole number, and for a span that is not a number at all --
 * the range check catches both, since a comparison against a number
 * that is not one is false either way round. A
 * *negative* span is left to divide, because the step it produces is
 * an ordinary negative number and the other renderer runs the fade
 * backwards on it; AFD_u_Taken reproduces that.
 */
float AFD_f_Step(float seconds);

/* The step as the whole number that is actually taken off the opacity.
 *
 * Truncated toward nought, and **modular** below it: a negative step
 * comes out as a very large amount, which the subtraction in
 * AFD_i_Next then wraps back into a small *increase*. That is not a
 * guard being skipped -- it is what the other renderer does, and it is
 * how a negative duration runs the fade backwards there, with no
 * ceiling at the top of the range. */
unsigned int AFD_u_Taken(float step);

/* The opacity after one frame's step, clamped at nought from below and
   not clamped above -- see AFD_u_Taken. */
int AFD_i_Next(int opacity, float step);

/* One frame of the timer: steps `opacity` and answers whether the
 * fade has finished, which is the signal to clear the effect's bit.
 *
 * The other renderer spells this as a test before the step and the
 * same test again after it, with the step in between skipped when the
 * opacity is already nothing. The two collapse: the answer is always
 * whether the opacity is nothing *after* this frame, because an
 * opacity of nothing cannot step anywhere else. Written collapsed
 * here, and the chunk of the other renderer that spells it out long
 * is noted rather than copied. */
int AFD_b_Advance(int* opacity, float step);

/* Whether this is the frame that freezes the picture: the first one,
   where the opacity is still the whole range. */
int AFD_b_Freeze(int opacity);

/* The frozen copy's share of the picture, between nought and one. The
   live frame takes the rest. */
float AFD_f_Blend(int opacity);

/* The same share as the other renderer carries it: an opacity byte at
   the top of a word whose colour is left white, so that the one
   constant hands the hardware both a neutral colour to multiply by
   and the blend's factor in its alpha. */
unsigned int AFD_u_TextureFactor(int opacity);
