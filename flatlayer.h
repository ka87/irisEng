#pragma once

/* The box the flat layer is laid out in, with no renderer in it.
 *
 * The flat layer is the 2D one -- the strings, the interface, the
 * pivots the text is placed against. It was laid out for a window of
 * a particular shape, and on a wider one it is kept to a box of that
 * shape in the middle rather than stretched across the whole window.
 *
 * This is where that box is worked out. It is arithmetic over three
 * numbers the engine already holds -- whether the window is wide,
 * what shape the layout was made for, and whether the box is wanted
 * at all -- and it names no renderer and calls nothing.
 *
 * It was in the Direct3D renderer's file, which is why its four
 * functions still read D3D_ in the engine's history: both renderers
 * want the same answers, and the strings, the string geometry and the
 * view functions asked the Direct3D renderer for them whichever
 * renderer was running.
 */

/* Whether the box is wanted. Set, never cleared by the engine; here
   so that turning it off turns off all four answers at once. */
extern int FLY_gi_Box;

/* The width the flat layer is laid out across, given the size of the
   window. On a window wider than the scenes were laid out for this is
   narrower than the window itself. */
int FLY_i_Width(int w, int h);

/* How far in from the left that width starts, so what is laid out
   across it lands in the middle of the window. */
int FLY_i_OffsetX(int w, int h);

/* Between a fraction of the window and a fraction of the box. Both
   are the identity while the box spans the window. */
float FLY_f_ToLayoutX(float windowFraction, int w, int h);
float FLY_f_FromLayoutX(float layoutFraction, int w, int h);
