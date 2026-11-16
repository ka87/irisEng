#include "cinebands.h"

#include <string.h>

/* The engine's own switch, the tweak that overrides it, and how much
   of the screen one band covers. The names say cin and d3d for no
   better reason than where they started; all three are the engine's
   and both renderers read them. */
extern int   CIN_Bands;
extern int   tweak_ae_nobands;
extern float D3D_gf_CutsceneBand;

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
    out->top = D3D_gf_CutsceneBand;
    out->bottom = 1.0f - D3D_gf_CutsceneBand;
    out->reachTop = out->top > CBD_MARGIN ? out->top - CBD_MARGIN : 0.0f;
    out->reachBottom = out->bottom + CBD_MARGIN < 1.0f
                           ? out->bottom + CBD_MARGIN : 1.0f;
}
