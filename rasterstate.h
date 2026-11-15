#pragma once

/* The per-draw raster state, read out of the two words the engine
   hands a renderer before every material: the material's own flags,
   and the draw mask that says what the current pass is allowed to
   touch.

   This was bit arithmetic spread through the Direct3D renderer, and
   none of it is Direct3D's. Which bit means "wrap" rather than
   "clamp", which one turns the alpha test on and where its reference
   value sits, which four bits choose the blend equation -- that is
   the engine's encoding, and a second renderer has to read exactly
   the same bits out of the same word. Reading them twice is how two
   renderers come to disagree about what a material meant.

   So the reading is here and it is pure: a flags word and a draw mask
   in, a description of the state out. Applying it -- naming the
   device's own constants, deciding what a renderer without an alpha
   test does instead -- stays with the renderer.

   The values below are this module's own and deliberately match no
   renderer's constants. A renderer therefore has to map them, and a
   mapping that swaps a pair shows up as a difference rather than
   working by accident. */

/* texture addressing, per axis */
enum { RST_CLAMP = 0, RST_WRAP = 1 };

/* sampling, both for the filter within a level and between levels */
enum { RST_POINT = 0, RST_LINEAR = 1 };

/* the depth comparison: the pass either draws what is nearer or
   draws only what is already exactly at that depth */
enum { RST_DEPTH_LESSEQUAL = 0, RST_DEPTH_EQUAL = 1 };

/* which way the alpha test keeps a fragment */
enum { RST_ALPHA_GREATER = 0, RST_ALPHA_LESS = 1 };

enum { RST_CULL_NONE = 0, RST_CULL_CW = 1, RST_CULL_CCW = 2 };

/* The blend equations the material's four-bit mode selects. Named for
   what they do rather than for either renderer's factor names, and
   ordered arbitrarily so that the order carries no meaning. */
typedef enum eRst_Blend
{
    RST_BLEND_KEEP = 0,       /* the mode names none; leave it alone */
    RST_BLEND_OFF,            /* src one, dst zero, blending disabled */
    RST_BLEND_SRC_ALPHA,      /* srcalpha, invsrcalpha */
    RST_BLEND_PREMULTIPLIED,  /* one, invsrcalpha */
    RST_BLEND_DEST_ALPHA,     /* destalpha, invdestalpha */
    RST_BLEND_DEST_ALPHA_ADD, /* destalpha, one */
    RST_BLEND_ADD,            /* one, one */
    RST_BLEND_INV_SRC_COLOR   /* zero, invsrccolor */
} tRst_Blend;

/* The fog variant the same four bits select. These are the engine's
   own tags, not a device's fog mode -- nothing is ever handed them;
   they pick which of the two fog colours the pass uses and they are
   what the renderer compares against to leave the colour alone. The
   values are the engine's so that a renderer caching on them does not
   have to translate. */
enum { RST_FOG_KEEP = 0, RST_FOG_A = 1, RST_FOG_B = 3, RST_FOG_C = 5 };

typedef struct sRst_State
{
    /* from the material's flags */
    int addressU;          /* RST_WRAP / RST_CLAMP */
    int addressV;
    int filter;            /* RST_LINEAR / RST_POINT, min and mag alike */
    int mipFilter;

    int depthCompare;      /* RST_DEPTH_* */
    int depthWrite;

    int writeRGB;
    int writeAlpha;

    int alphaTest;         /* whether to test at all */
    int alphaCompare;      /* RST_ALPHA_* */
    int alphaRef;          /* a multiple of four, 0..252 */

    /* The alpha the vertex colours are masked with downstream. It is
       either fully opaque or nothing, and the renderer combines it. */
    unsigned int colorMaskAlpha;

    tRst_Blend blend;
    /* Four of the blend equations come in two senses, and the same
       bit that picks the sense also picks the alpha comparison. */
    int blendInverted;

    int fogVariant;        /* RST_FOG_* */
    /* Fog variant B uses the second of the two fog colours the
       renderer keeps; the other two use the first. */
    int fogUsesSecondColor;

    /* from the draw mask */
    int solidFill;         /* otherwise wireframe */
    int depthTest;         /* whether the pass tests depth at all */
    int cull;              /* RST_CULL_* */
    int fogAllowed;        /* the pass permits fog; a renderer still
                              needs a fog variant to be in effect */
} tRst_State;

/* Reads both words. Nothing else is consulted and nothing is written
   but *out, so the same inputs always give the same answer. */
void RST_Decode(unsigned int flags, int drawMask, tRst_State* out);
