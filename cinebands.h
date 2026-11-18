#pragma once

/* The cutscene letterbox, with no renderer in it.
 *
 * During a cutscene the picture is confined to a strip down the middle
 * of the screen and the rest is left black. The bands are not drawn:
 * the after-effects copy their finished frame back to the screen and,
 * when the letterbox is on, copy only the strip -- so the bands are
 * whatever the screen already held, which is black.
 *
 * Both renderers need the same two answers -- whether it is on, and
 * where the strip is -- and the gate has two parts, the engine's own
 * switch and the tweak that turns the whole thing off. Forgetting the
 * second is the easy mistake, which is why this is a function rather
 * than two globals read at each site.
 */

/* The engine's cutscene switch, whether a script may change it, and
   how much of the screen one band covers. Here rather than in a
   renderer's file: a script sets the first, the third is a constant
   about the picture, and both renderers read them. */
extern int   CIN_Bands;
extern int   CIN_NoBands;
extern float CIN_gf_BandHeight;

/* What a script asks for when a cutscene starts and stops. Honoured
   only while CIN_NoBands says it may be. */
void CIN_CutsceneMode(int on);

typedef struct sCbd_Bands
{
    int   b_on;

    /* The strip the picture is confined to, as fractions of the
       screen's height from the top. With the letterbox off these are
       0 and 1 -- the whole screen -- so a caller can use them without
       asking whether it is on. */
    float top;
    float bottom;

    /* The same strip widened by a margin and clamped to the screen.
       The effects reach a little past the strip so that a filter
       kernel sampling across the edge has something to sample; the
       copy itself uses the strip above. */
    float reachTop;
    float reachBottom;
} tCbd_Bands;

void CBD_Decide(tCbd_Bands* out);
