/* Direct3D 9 constant values as the real SDK defines them, forced into
 * both sides of the wingdi_aftereffects differential test with -include.
 *
 * tests/stub/d3d9.h deliberately gives these names arbitrary distinct
 * values, because nothing in the engine may read meaning from the
 * numbers. The file under test is the one place where that gets in the
 * way: the pre-rewrite copy writes the bare SDK numbers at 150-odd
 * sites -- SetTextureStageState(dev, 0, 1, 2) for COLOROP = SELECTARG1
 * -- and the rewrite spells them with the names. The two only compare
 * equal if the names carry the numbers they carry in the shipping game,
 * so this header supplies them, identically on both sides.
 *
 * The one thing it gives up is telling apart two constants that are
 * equal in the SDK as well. In this file those are D3DTA_TFACTOR and
 * D3DTOP_SELECTARG2 (both 3) and D3DTA_DIFFUSE and D3DTTFF_DISABLE
 * (both 0). That is a property of Direct3D, not of this harness: the
 * shipping game cannot tell them apart either.
 *
 * Values from the DirectX 9 SDK headers (d3d9types.h).
 */
#pragma once
#include <d3d9.h>

/* D3DRENDERSTATETYPE */
#undef  D3DRS_ZENABLE
#define D3DRS_ZENABLE                 7
#undef  D3DRS_FILLMODE
#define D3DRS_FILLMODE                8
#undef  D3DRS_ZWRITEENABLE
#define D3DRS_ZWRITEENABLE            14
#undef  D3DRS_ALPHATESTENABLE
#define D3DRS_ALPHATESTENABLE         15
#undef  D3DRS_SRCBLEND
#define D3DRS_SRCBLEND                19
#undef  D3DRS_DESTBLEND
#define D3DRS_DESTBLEND               20
#undef  D3DRS_CULLMODE
#define D3DRS_CULLMODE                22
#undef  D3DRS_ZFUNC
#define D3DRS_ZFUNC                   23
#undef  D3DRS_ALPHAREF
#define D3DRS_ALPHAREF                24
#undef  D3DRS_ALPHAFUNC
#define D3DRS_ALPHAFUNC               25
#undef  D3DRS_ALPHABLENDENABLE
#define D3DRS_ALPHABLENDENABLE        27
#undef  D3DRS_FOGENABLE
#define D3DRS_FOGENABLE               28
#undef  D3DRS_FOGCOLOR
#define D3DRS_FOGCOLOR                34
#undef  D3DRS_FOGTABLEMODE
#define D3DRS_FOGTABLEMODE            35
#undef  D3DRS_FOGSTART
#define D3DRS_FOGSTART                36
#undef  D3DRS_FOGEND
#define D3DRS_FOGEND                  37
#undef  D3DRS_FOGDENSITY
#define D3DRS_FOGDENSITY              38
#undef  D3DRS_TEXTUREFACTOR
#define D3DRS_TEXTUREFACTOR           60
#undef  D3DRS_CLIPPLANEENABLE
#define D3DRS_CLIPPLANEENABLE         152
#undef  D3DRS_FOGVERTEXMODE
#define D3DRS_FOGVERTEXMODE           140
#undef  D3DRS_LIGHTING
#define D3DRS_LIGHTING                137
#undef  D3DRS_NORMALIZENORMALS
#define D3DRS_NORMALIZENORMALS        143
#undef  D3DRS_COLORWRITEENABLE
#define D3DRS_COLORWRITEENABLE        168
#undef  D3DRS_BLENDOP
#define D3DRS_BLENDOP                 171

/* D3DTEXTURESTAGESTATETYPE */
#undef  D3DTSS_COLOROP
#define D3DTSS_COLOROP                1
#undef  D3DTSS_COLORARG1
#define D3DTSS_COLORARG1              2
#undef  D3DTSS_COLORARG2
#define D3DTSS_COLORARG2              3
#undef  D3DTSS_ALPHAOP
#define D3DTSS_ALPHAOP                4
#undef  D3DTSS_ALPHAARG1
#define D3DTSS_ALPHAARG1              5
#undef  D3DTSS_ALPHAARG2
#define D3DTSS_ALPHAARG2              6
#undef  D3DTSS_BUMPENVMAT00
#define D3DTSS_BUMPENVMAT00           7
#undef  D3DTSS_BUMPENVMAT01
#define D3DTSS_BUMPENVMAT01           8
#undef  D3DTSS_BUMPENVMAT10
#define D3DTSS_BUMPENVMAT10           9
#undef  D3DTSS_BUMPENVMAT11
#define D3DTSS_BUMPENVMAT11           10
#undef  D3DTSS_TEXCOORDINDEX
#define D3DTSS_TEXCOORDINDEX          11
#undef  D3DTSS_BUMPENVLSCALE
#define D3DTSS_BUMPENVLSCALE          22
#undef  D3DTSS_BUMPENVLOFFSET
#define D3DTSS_BUMPENVLOFFSET         23
#undef  D3DTSS_TEXTURETRANSFORMFLAGS
#define D3DTSS_TEXTURETRANSFORMFLAGS  24
#undef  D3DTSS_COLORARG0
#define D3DTSS_COLORARG0              26
#undef  D3DTSS_ALPHAARG0
#define D3DTSS_ALPHAARG0              27
#undef  D3DTSS_RESULTARG
#define D3DTSS_RESULTARG              28
#undef  D3DTSS_CONSTANT
#define D3DTSS_CONSTANT               32

/* D3DSAMPLERSTATETYPE */
#undef  D3DSAMP_ADDRESSU
#define D3DSAMP_ADDRESSU              1
#undef  D3DSAMP_ADDRESSV
#define D3DSAMP_ADDRESSV              2
#undef  D3DSAMP_MAGFILTER
#define D3DSAMP_MAGFILTER             5
#undef  D3DSAMP_MINFILTER
#define D3DSAMP_MINFILTER             6
#undef  D3DSAMP_MIPFILTER
#define D3DSAMP_MIPFILTER             7
#undef  D3DSAMP_MIPMAPLODBIAS
#define D3DSAMP_MIPMAPLODBIAS         8

/* D3DTEXTUREOP */
#undef  D3DTOP_DISABLE
#define D3DTOP_DISABLE                1
#undef  D3DTOP_SELECTARG1
#define D3DTOP_SELECTARG1             2
#undef  D3DTOP_SELECTARG2
#define D3DTOP_SELECTARG2             3
#undef  D3DTOP_MODULATE
#define D3DTOP_MODULATE               4
#undef  D3DTOP_MODULATE2X
#define D3DTOP_MODULATE2X             5
#undef  D3DTOP_ADD
#define D3DTOP_ADD                    7
#undef  D3DTOP_SUBTRACT
#define D3DTOP_SUBTRACT               10
#undef  D3DTOP_BLENDDIFFUSEALPHA
#define D3DTOP_BLENDDIFFUSEALPHA      12
#undef  D3DTOP_BLENDFACTORALPHA
#define D3DTOP_BLENDFACTORALPHA       14
#undef  D3DTOP_BLENDCURRENTALPHA
#define D3DTOP_BLENDCURRENTALPHA      16
#undef  D3DTOP_MODULATEALPHA_ADDCOLOR
#define D3DTOP_MODULATEALPHA_ADDCOLOR 18
#undef  D3DTOP_DOTPRODUCT3
#define D3DTOP_DOTPRODUCT3            24

/* D3DTA */
#undef  D3DTA_DIFFUSE
#define D3DTA_DIFFUSE                 0x00000000
#undef  D3DTA_CURRENT
#define D3DTA_CURRENT                 0x00000001
#undef  D3DTA_TEXTURE
#define D3DTA_TEXTURE                 0x00000002
#undef  D3DTA_TFACTOR
#define D3DTA_TFACTOR                 0x00000003

/* D3DTEXTURETRANSFORMFLAGS */
#undef  D3DTTFF_DISABLE
#define D3DTTFF_DISABLE               0
#undef  D3DTTFF_COUNT2
#define D3DTTFF_COUNT2                2
#undef  D3DTTFF_COUNT3
#define D3DTTFF_COUNT3                3
#undef  D3DTTFF_PROJECTED
#define D3DTTFF_PROJECTED             256

/* D3DTEXTUREADDRESS */
#undef  D3DTADDRESS_WRAP
#define D3DTADDRESS_WRAP              1
#define D3DTADDRESS_MIRROR            2
#undef  D3DTADDRESS_CLAMP
#define D3DTADDRESS_CLAMP             3

/* D3DTEXTUREFILTERTYPE */
#undef  D3DTEXF_NONE
#define D3DTEXF_NONE                  0
#undef  D3DTEXF_POINT
#define D3DTEXF_POINT                 1
#undef  D3DTEXF_LINEAR
#define D3DTEXF_LINEAR                2

/* D3DBLEND */
#undef  D3DBLEND_ZERO
#define D3DBLEND_ZERO                 1
#undef  D3DBLEND_ONE
#define D3DBLEND_ONE                  2
#undef  D3DBLEND_SRCALPHA
#define D3DBLEND_SRCALPHA             5
#undef  D3DBLEND_INVSRCALPHA
#define D3DBLEND_INVSRCALPHA          6
#undef  D3DBLEND_DESTALPHA
#define D3DBLEND_DESTALPHA            7
#undef  D3DBLEND_INVDESTALPHA
#define D3DBLEND_INVDESTALPHA         8
#undef  D3DBLEND_INVSRCCOLOR
#define D3DBLEND_INVSRCCOLOR          4

/* D3DBLENDOP / D3DCMPFUNC / D3DCULL / D3DFILLMODE / D3DZBUFFERTYPE */
#undef  D3DBLENDOP_ADD
#define D3DBLENDOP_ADD                1
#undef  D3DCMP_LESS
#define D3DCMP_LESS                   2
#undef  D3DCMP_LESSEQUAL
#define D3DCMP_LESSEQUAL              4
#undef  D3DCMP_GREATER
#define D3DCMP_GREATER                5
#undef  D3DCMP_ALWAYS
#define D3DCMP_ALWAYS                 8
#undef  D3DCULL_NONE
#define D3DCULL_NONE                  1
#undef  D3DCULL_CW
#define D3DCULL_CW                    2
#undef  D3DCULL_CCW
#define D3DCULL_CCW                   3
#undef  D3DFILL_WIREFRAME
#define D3DFILL_WIREFRAME             2
#undef  D3DFILL_SOLID
#define D3DFILL_SOLID                 3
#undef  D3DZB_FALSE
#define D3DZB_FALSE                   0
#undef  D3DZB_TRUE
#define D3DZB_TRUE                    1

/* D3DCOLORWRITEENABLE */
#undef  D3DCOLORWRITEENABLE_RED
#define D3DCOLORWRITEENABLE_RED       1
#undef  D3DCOLORWRITEENABLE_GREEN
#define D3DCOLORWRITEENABLE_GREEN     2
#undef  D3DCOLORWRITEENABLE_BLUE
#define D3DCOLORWRITEENABLE_BLUE      4
#undef  D3DCOLORWRITEENABLE_ALPHA
#define D3DCOLORWRITEENABLE_ALPHA     8

/* D3DPRIMITIVETYPE */
#undef  D3DPT_TRIANGLELIST
#define D3DPT_TRIANGLELIST            4
#undef  D3DPT_TRIANGLESTRIP
#define D3DPT_TRIANGLESTRIP           5

/* D3DFVF */
#undef  D3DFVF_XYZ
#define D3DFVF_XYZ                    0x002
#undef  D3DFVF_XYZRHW
#define D3DFVF_XYZRHW                 0x004
#undef  D3DFVF_NORMAL
#define D3DFVF_NORMAL                 0x010
#undef  D3DFVF_DIFFUSE
#define D3DFVF_DIFFUSE                0x040
#undef  D3DFVF_SPECULAR
#define D3DFVF_SPECULAR               0x080
#undef  D3DFVF_TEX1
#define D3DFVF_TEX1                   0x100
#undef  D3DFVF_TEX2
#define D3DFVF_TEX2                   0x200
#undef  D3DFVF_TEX4
#define D3DFVF_TEX4                   0x400
