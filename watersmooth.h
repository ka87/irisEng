#pragma once

/* The water's surface smoothing, with no renderer in it.
 *
 * The last of the water's three pieces. Before the surface samples the
 * reflection, the reflection is blurred -- twice, out into a scratch
 * target and back again -- so that what the water mirrors is softer
 * than what the world drew.
 *
 * Three things are worth knowing before reading any of it, and all
 * three are read out of the renderer that already has this rather than
 * decided here:
 *
 * **It is off unless somebody turns it on.** The two amounts are debug
 * tweaks and both start at nought, so by default every tap lands in
 * the same place and the two passes are two plain copies. Writing it
 * faithfully therefore changes nothing on a default run, which is also
 * what makes it safe to add.
 *
 * **Half of the original is unreachable.** The blur chooses between a
 * four-unit path and a two-unit one by the card's simultaneous-texture
 * count, but the only thing that calls it requires four units first --
 * so the two-unit arm cannot run from the water, ever. Only the
 * four-unit arithmetic is here, and the other arm is noted rather than
 * reproduced.
 *
 * **The vertical spread is not uniform.** The horizontal tap offsets
 * are the same across the whole rectangle; the vertical ones are
 * **nought at the top edge and grow to three steps at the bottom**,
 * interpolated between. So the blur is weakest at the far edge of the
 * reflection and strongest at the near one -- which is what a surface
 * seen in perspective wants, and is spelled out vertex by vertex in
 * the original rather than falling out of anything.
 */

#define WSM_TAPS        4

/* Where the reflection is blurred from and to, in pixels, as the
   renderer this came from passes them around: a rectangle given by its
   first and last row and column, and the size of the surface holding
   it. Last, not one-past -- a 512-wide surface covered edge to edge has
   `last` of 511. */
typedef struct sWsm_Rect
{
    int firstColumn;
    int lastColumn;
    int firstRow;
    int lastRow;
    int width;
    int height;
} tWsm_Rect;

/* One tap: where it samples, relative to the point being written.
 *
 * `u` applies everywhere. `vAtBottom` applies at the rectangle's
 * bottom edge and nought applies at its top, with the space between
 * interpolated -- see the note above. Both are in texture coordinates
 * already, so a caller does not scale them again. */
typedef struct sWsm_Tap
{
    float u;
    float vAtBottom;
    float weight;
} tWsm_Tap;

/* The whole blur, as the two passes it is. The first reads the
   reflection and writes the scratch, the second reads the scratch and
   writes the reflection back -- so the reflection ends up holding the
   result and the surface needs to know nothing about any of it. */
typedef struct sWsm_Pass
{
    tWsm_Rect from;
    tWsm_Rect to;
} tWsm_Pass;

/* The scratch rectangle for a target of this size.
 *
 * The reflection is always the whole of its own 512-square surface.
 * The scratch is whatever the renderer has to hand, so the reflection
 * is **centred** in it when it is the larger and clipped to it when it
 * is not -- and "clipped" here means the rectangle is simply the whole
 * target, with no attempt to keep the middle. That asymmetry is the
 * original's.
 *
 * The comparison that picks between the two arms is "no larger than",
 * and whether it is written that way or strictly is **not
 * observable**: at a target of exactly 512 the two arms compute the
 * same rectangle -- nought and 511 either way, because the centring
 * offset is nought and the span is the full 512 -- and there is no
 * other size where they differ. Both spellings were tried against the
 * other renderer over targets of 256, 512 and 1024 and neither moves
 * anything. So the test below is not missing a case; there is no case
 * to add. */
void WSM_Scratch(tWsm_Rect* out, int width, int height);

/* The reflection's own rectangle: the whole of a square of this size,
   given as first and last. */
void WSM_Reflection(tWsm_Rect* out, int size);

/* The two passes, in order, for a reflection of `size` square and a
 * scratch target of `width` by `height`. `out` takes two. */
void WSM_Passes(tWsm_Pass* out, int size, int width, int height);

/* How far one tap step is in texture coordinates, from the amount the
 * tweak holds. The divisor is the rectangle's **last** index and not
 * its size, which is a pixel's difference at this scale and is kept
 * because it is what the original divides by. */
float WSM_f_Step(float amount, int last);

/* The four taps, with their offsets and their weights.
 *
 * The weights are the same chain the depth blur's taps are -- a run of
 * hardware stages each mixing its own sample into what came before by
 * a fixed factor -- so they come from the same place rather than being
 * written out twice. `uStep` and `vStep` are WSM_f_Step's answers.
 *
 * The offsets are nought, minus a step, plus a step and nought across;
 * and nought, one, two and three steps down. The horizontal pattern
 * returning to nought on the last tap is not a mistake in the reading:
 * two of the four taps sample the same column. */
void WSM_Taps(tWsm_Tap* out, float uStep, float vStep);
