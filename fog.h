#pragma once

/* The engine's fog, with no renderer in it.
 *
 * The engine hands a renderer a curve, a range, a density and a
 * colour, and each material then says whether fog applies to it and
 * which of two colours it uses. One renderer turns that into
 * fixed-function state and lets the card do the arithmetic; the other
 * has to do the arithmetic itself. What is the same either way is
 * which curve the engine meant, which colour, and whether fog applies
 * at all, so that is what lives here.
 */

/* The curve, named rather than numbered: the engine's own numbering is
   an index into a table of another renderer's constants and says
   nothing on its own.

   FOG_OFF is the engine's fourth number and does not mean fog is off.
   It names no distance curve, and the renderer that has fog leaves fog
   enabled for it -- which in that renderer's terms means the factor
   comes off the vertices rather than off the distance. A renderer with
   no per-vertex fog source cannot honour it and should say so rather
   than quietly drawing no fog. */
enum
{
    FOG_OFF    = 0,
    FOG_LINEAR = 1,
    FOG_EXP    = 2,
    FOG_EXP2   = 3
};

/* What the engine asked for, field by field, so that nothing here has
   to know the shape of the engine's own request. */
typedef struct sFog_Source
{
    int          b_enabled;    /* the request's low flag */
    int          engineCurve;  /* 0..3, the engine's own numbering */
    float        start;
    float        end;
    float        density;
    unsigned int engineColor;  /* in the engine's byte order */
} tFog_Source;

typedef struct sFog_State
{
    int          b_on;
    int          curve;        /* FOG_* */
    float        start;
    float        end;
    float        density;
    /* Both in A8R8G8B8, which is the order a renderer wants and not
       the order the engine keeps: the engine's word has red and blue
       the other way round, exactly as its vertex colours do.

       The second colour is black, and not by coincidence -- the
       renderer that has fog today sets it to zero every time fog
       parameters arrive and never sets it to anything else. So the
       variant that uses it is a variant that fogs to black. */
    unsigned int color;
    unsigned int color2;
} tFog_State;

void FOG_Decode(const tFog_Source* src, tFog_State* out);

/* Does fog apply to a pass? The engine has to have asked for it and
   the pass's draw mask has to permit it. Which variant the material
   names does not come into it -- a variant only chooses the colour --
   and neither does the curve, which says where the factor comes from
   and not whether there is one. */
int FOG_b_Applies(const tFog_State* st, int b_fogAllowed);

/* Which colour a variant fogs to. The caller has already resolved
   RST_FOG_KEEP into whichever variant was last named, because keeping
   is what that one means. */
unsigned int FOG_i_Color(const tFog_State* st, int b_useSecond);

/* How much of the pixel survives at a camera-space depth: 1 is no fog
   and 0 is all fog, which is the sense the hardware's own fog factor
   has. The colour is then `mix(fogColor, pixel, factor)`. */
float FOG_f_Factor(const tFog_State* st, float depth);
