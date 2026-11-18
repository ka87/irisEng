#include "cinebands.h"

#include <string.h>

/* The engine's own switch, the tweak that overrides it, and how much
   of the screen one band covers.
 *
 * The first and the third were in the Direct3D renderer's files --
 * the switch in the after-effects, the height in the renderer -- and
 * both are the engine's: the switch is set by a script, the height is
 * a constant about the picture, and both renderers read them. They
 * are here now, with the one function that sets the switch.

   The tweak stays where the other tweaks are. */
extern int tweak_ae_nobands;

/* On while a cutscene is playing, which is what confines the picture
   to the strip. Set to one at startup, as the engine has it. */
int CIN_Bands = 1;

/* Whether a script may change that. The two together are the
   engine's: a script asks for cutscene mode and gets it only where
   this says it may. */
int CIN_NoBands = 0;

/* How much of the screen's height one band covers. */
float CIN_gf_BandHeight = 0.10416666f;

void CIN_CutsceneMode(int on)
{
    if (CIN_NoBands)
        CIN_Bands = on ? 1 : 0;
}

/* How far past the strip the effects reach. */
static const float CBD_MARGIN = 0.020833334f;

void CBD_Decide(tCbd_Bands* out)
{
    if (!out)
        return;

    memset(out, 0, sizeof(*out));
    out->bottom = 1.0f;
    out->reachBottom = 1.0f;

    if (tweak_ae_nobands || !CIN_Bands)
        return;

    out->b_on = 1;
    out->top = CIN_gf_BandHeight;
    out->bottom = 1.0f - CIN_gf_BandHeight;
    out->reachTop = out->top > CBD_MARGIN ? out->top - CBD_MARGIN : 0.0f;
    out->reachBottom = out->bottom + CBD_MARGIN < 1.0f
                           ? out->bottom + CBD_MARGIN : 1.0f;
}
