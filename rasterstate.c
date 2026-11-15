#include "rasterstate.h"

/* The material's flags, bit by bit. The names are what the bits do,
   read off the renderer that already acts on them.

   Several of these are inverted -- the bit turns something off rather
   than on -- and two of them share one bit, which is why reading the
   word twice in two renderers is a bad idea. */
#define RST_F_WRAP_U      0x00000001u
#define RST_F_WRAP_V      0x00000002u
#define RST_F_FILTER      0x00000004u  /* set: linear within a level */
#define RST_F_MIPFILTER   0x00000008u  /* set: linear between levels */
#define RST_F_ALPHA_TEST  0x00000010u
#define RST_F_NO_ALPHA    0x00000020u  /* set: do not write alpha */
#define RST_F_NO_COLOR    0x00000040u  /* set: do not write colour */
/* One bit, two meanings: it reverses the alpha comparison and it
   reverses the sense of the blend equation. */
#define RST_F_REVERSE     0x00000080u
#define RST_F_DEPTH_EQUAL 0x00000100u
#define RST_F_NO_DEPTH_W  0x00000200u  /* set: do not write depth */
#define RST_F_MASK_ALPHA  0x00000400u

/* The four bits that choose the blend equation and the fog variant --
   the same four for both. */
#define RST_F_MODE_SHIFT  16
#define RST_F_MODE_MASK   0xFu

/* The alpha reference value. Six bits, taken from 24..29 and landing
   at 2..7, so the value is a multiple of four between 0 and 252 and
   bits 22 and 23 of the word are discarded rather than read. */
#define RST_F_ALPHAREF_SHIFT 22
#define RST_F_ALPHAREF_MASK  0xFCu

/* The draw mask's bits. These say what the pass as a whole allows,
   not what one material wants. */
#define RST_D_SOLID       0x00000010
#define RST_D_CULL        0x00000800
#define RST_D_CULL_FRONT  0x00002000
#define RST_D_FOG         0x00004000
#define RST_D_DEPTH       0x01000000

/* mode -> blend equation. Two of the sixteen name none at all, and a
   renderer leaves its blending as it found it for those. */
static tRst_Blend RST_l_Blend(unsigned int mode)
{
    switch (mode)
    {
    case 0:
    case 7:
        return RST_BLEND_OFF;
    case 1:
        return RST_BLEND_SRC_ALPHA;
    case 2:
        return RST_BLEND_PREMULTIPLIED;
    case 3:
        return RST_BLEND_DEST_ALPHA;
    case 4:
        return RST_BLEND_DEST_ALPHA_ADD;
    case 5:
        return RST_BLEND_ADD;
    case 6:
    case 8:
        return RST_BLEND_INV_SRC_COLOR;
    default:
        return RST_BLEND_KEEP;
    }
}

/* mode -> fog variant, off the same four bits. The two tables do not
   line up: mode 8 names a blend equation and no fog variant, and
   modes 0 and 7 turn blending off while asking for the same fog as
   modes 1 and 3. */
static int RST_l_Fog(unsigned int mode)
{
    switch (mode)
    {
    case 0:
    case 1:
    case 3:
    case 7:
        return RST_FOG_A;
    case 2:
    case 4:
        return RST_FOG_C;
    case 5:
    case 6:
        return RST_FOG_B;
    default:
        return RST_FOG_KEEP;
    }
}

void RST_Decode(unsigned int flags, int drawMask, tRst_State* out)
{
    unsigned int mode;

    if (!out)
        return;

    mode = (flags >> RST_F_MODE_SHIFT) & RST_F_MODE_MASK;

    out->addressU = (flags & RST_F_WRAP_U) ? RST_WRAP : RST_CLAMP;
    out->addressV = (flags & RST_F_WRAP_V) ? RST_WRAP : RST_CLAMP;
    out->filter = (flags & RST_F_FILTER) ? RST_LINEAR : RST_POINT;
    out->mipFilter = (flags & RST_F_MIPFILTER) ? RST_LINEAR : RST_POINT;

    out->depthCompare = (flags & RST_F_DEPTH_EQUAL) ? RST_DEPTH_EQUAL
                                                    : RST_DEPTH_LESSEQUAL;
    out->depthWrite = (flags & RST_F_NO_DEPTH_W) ? 0 : 1;

    out->writeRGB = (flags & RST_F_NO_COLOR) ? 0 : 1;
    out->writeAlpha = (flags & RST_F_NO_ALPHA) ? 0 : 1;

    out->alphaTest = (flags & RST_F_ALPHA_TEST) ? 1 : 0;
    out->alphaCompare = (flags & RST_F_REVERSE) ? RST_ALPHA_LESS
                                                : RST_ALPHA_GREATER;
    out->alphaRef = (int)((flags >> RST_F_ALPHAREF_SHIFT) & RST_F_ALPHAREF_MASK);

    out->colorMaskAlpha = (flags & RST_F_MASK_ALPHA) ? 0xFF000000u : 0u;

    out->blend = RST_l_Blend(mode);
    out->blendInverted = (flags & RST_F_REVERSE) ? 1 : 0;

    out->fogVariant = RST_l_Fog(mode);
    out->fogUsesSecondColor = (out->fogVariant == RST_FOG_B) ? 1 : 0;

    out->solidFill = (drawMask & RST_D_SOLID) ? 1 : 0;
    out->depthTest = (drawMask & RST_D_DEPTH) ? 1 : 0;
    if (!(drawMask & RST_D_CULL))
        out->cull = RST_CULL_NONE;
    else
        out->cull = (drawMask & RST_D_CULL_FRONT) ? RST_CULL_CW : RST_CULL_CCW;
    out->fogAllowed = (drawMask & RST_D_FOG) ? 1 : 0;
}
