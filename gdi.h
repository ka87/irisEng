#pragma once
#include "common.h"
#include "math.h"
#include "graphicsobjects/geo.h"
#include "graphicsobjects/geomaterial.h"
#include "graphicsobjects/shadow.h"
#include "graphicsobjects/light.h"
#include "graphicsobjects/staticlod.h"
#include "graphicsobjects/camera.h"
#include "graphicsobjects/way.h"
#include "graphicsobjects/particle.h"
#include "cameraengine.h"
#include "profiler.h"
#include "lights.h"
#include "shadows.h"
#include "soft.h"
#include "graphicsobjects/geostr.h"
#include "sprites.h"


/* The renderer's own private state, whatever renderer that is. The
   engine holds it as an opaque pointer and never looks inside: the
   Direct3D one is tGdi_D3DRenderParams in win/wingdi.h, which is
   where it belongs and where it used to not be. */

typedef struct sTexManagerStruct
{
    int flags;
    int i2;
    int i3;
    int texMemoryNeededCompressedCase2Backup;
    int texMemoryNeededCompressedCase2;
    int texMemoryNeededUncompressedCase2;
    int texMemoryNeededCompressedCase1Backup;
    int texMemoryNeededUncompressedCase1;
    int texMemoryNeededCompressedCase1;
    int paletteUsedMemory;
    int timsIdx;
    int elemsFlagsWithoutFlag2;
    int elemsFlagsWithFlag2;
    char keyLoadOrderIDs[4];
} tTexManagerStruct;

typedef struct sTexUploadRequest {
    uint32_t* dataPtr;
    uint16_t idx;
} tTexUploadRequest;

typedef struct sFogParams
{
    char fogVertexMode;
    char flags;
    float fogStart;
    float fogEnd;
    int fogColor;
    float density;
} tFogParams;


/* A vertex of flat 2D geometry: pixel-space position and a colour.
   Laid out to match what the Direct3D renderer already drew the
   overlay's shapes with, so adopting it moved no bytes -- the test
   checks the size rather than trusting that. */
typedef struct sGdi_Vertex
{
    Vec3     pos;
    uint32_t color;
} tGdi_Vertex;

/* A vertex of textured 2D geometry: pixel-space position, a colour
   the texture is modulated by, and one texture coordinate. This is
   the untextured overlay vertex with a UV added, and both are laid
   out so a fixed-function renderer can describe them without
   reordering anything. */
typedef struct sGdi_TexturedVertex
{
    Vec3     pos;
    uint32_t color;
    float    u;
    float    v;
} tGdi_TexturedVertex;

/* One sprite, as four corners and a colour. The corners are in world
   space and in the order the renderer expands them: the first and the
   third are opposite, which is what lets two triangles share them. */
typedef struct sGdi_SpriteQuad
{
    Vec3 v1;
    Vec3 v2;
    Vec3 v3;
    Vec3 v4;
    int  color;
} tGdi_SpriteQuad;

struct sWaterParamsGlobalStruct;

typedef struct sGdi_DisplayData
{
    void*(* CreateDevice)(void);
    void (* DestroyDevice)(void* rendererState);
    int(* Init)(HWND hWnd, struct sGdi_DisplayData* a6);
    int(* Close)();
    int(* ReadaptDisplay)(HWND hwnd, struct sGdi_DisplayData* gdd);
    void(* Clear)(int a1, int color);
    void(* Flip)();
    int(* SetViewMatrix)(Mtx4x4* mtxptr);
    void(* SetProjectionMatrix)(tDDCamStruct* a1);
    int(* DrawElementIndexedTriangles)(struct sGeoTriangleChunks*, Vec3*, struct sGeoUVElem*, int);
    int(* Request)(int type, void* param); // tTexUploadRequest 12-esnél
    void(* TextureInit)(int a1, int a2, char a3);
    int(* DrawSprites)(tGeoSprites* a1, Vec3* a2, int num);
    int(* TextureStore)(struct sGdi_DisplayData* a1, int a2);
    void(* PaletteLoad)(struct sGdi_DisplayData* a1, struct sEngTexListElemStruct* a2, struct sEngTexIntermediateStruct* a3, int a4);
    int(* SetTexturePalette)(struct sGdi_DisplayData* a1, struct sEngTexListElemStruct* a2, struct sEngTexIntermediateStruct* a3, int a4);
    void(* TextureUnload)(struct sGdi_DisplayData* a1, struct sEngPaletteElem *a2, int a3);
    void(* SetTextureTarget)(struct sGdi_DisplayData* a1, int a2, int a3, int a4);
    void(* ReleaseTextures)(struct sGdi_DisplayData* dd);
    void(* SetRenderTargetAndBackupPreviousSDW)(int textureID, int clear);
    void(* SetViewMatrixSDW)(Mtx4x4* mtxptr, float* unused);

    /* Drawing state the engine asks for by what it means, not by any
       renderer's own names for it. A renderer is free to keep its own
       cache of what is already in force -- the engine does not, and
       does not need to know whether one exists. */
    void(* SetZEnable)(int on);
    int (* i_ZEnable)(void);
    void(* SetZWriteEnable)(int on);
    void(* SetTexCoordIndex)(int stage, int index);
    void(* SetTextureTransformFlags)(int stage, int flags);

    /* The fullscreen video display: a movie-sized picture the engine
       writes decoded frames into, which the renderer fits to the
       screen. The engine knows about files, decoding and the clock
       and does not know what a surface is. */
    int (* b_VideoDisplayCreate)(int width, int height);
    void(* VideoDisplayClose)(void);
    int (* b_VideoFrameLock)(void** bits, int* pitch, int* height);
    void(* VideoFrameUnlock)(void);
    void(* VideoFramePresent)(int width, int height, int fit);

    /* The sun's occlusion test. The engine projects the sun, clips
       the positions it wants looked at, and takes an answer whenever
       one is ready; how the looking is done, and how long it takes,
       is the renderer's own business. */
    void*(* p_SunSamplerCreate)(void);
    void (* SunSamplerDestroy)(void* sampler);
    int  (* b_SunSamplerReady)(void* sampler);
    int  (* b_SunSamplingNow)(void* sampler);
    void (* SunSampleQueue)(void* sampler, const int* x, const int* y, int num,
                            int pointMask, float cameraZ);
    int  (* b_SunSampleRead)(void* sampler, int* outBlocked);
    void (* SunEffectOnOff)(void* sampler, int on);

    /* What this renderer can do, as the engine needs to know it. Each
       is worked out by the renderer when it starts -- from the
       device's own capabilities, in Direct3D's case -- and each can
       be forced from the tweak console, so these are read every time
       rather than cached by the caller. */
    int (* b_CanDiffuse2x)(void);     /* can modulate colour by two */
    int (* b_WantsNoFlare)(void);     /* too few texture units for the flare */
    int (* b_NoZonAlpha)(void);
    int (* b_DepthBlurOn)(void);      /* the depth blur is running */

    /* How big the renderer's drawing surface came out, which only it
       knows -- the window it was asked for and what it ended up with
       are not always the same thing. */
    void(* GetDrawingSize)(int* width, int* height);

    /* The same capabilities the other way round, for the tweak
       console: it exists so a flag the renderer worked out can be
       forced. Nothing in the engine's drawing path writes these. */
    void(* SetCanDiffuse2x)(int on);
    void(* SetWantsNoFlare)(int on);
    void(* SetNoZonAlpha)(int on);

    /* What a renderer has cached for a piece of geometry -- vertex
       and index buffers, whatever they are for that renderer. The
       engine asks for it to be dropped when the geometry changes
       under it, and never looks at it. */
    int (* b_GeometryHasCache)(struct sGEOGRO* gro);
    void(* DiscardGeometryCache)(struct sGEOGRO* gro);

    /* A glyph atlas, and the quads that read from it. The engine
       rasterizes text itself and hands over 8-bit coverage -- one
       byte per pixel, row after row, no padding -- leaving the
       renderer to pick whatever texture format it can modulate by a
       vertex colour. The handle is the renderer's and opaque.

       The quads are tGdi_TexturedVertex triangles in the same
       pixel-space coordinates as the untextured overlay pass, and
       expect to be drawn inside it: it is what sets up the
       projection and the alpha blending. */
    /* Whether there is anything to draw through yet. The overlay asks
       before it starts a pass: it runs from the message loop and can
       be asked to draw before a device exists or after one has gone. */
    int (* b_Ready)(void);

    /* The 2D pass the debug overlay draws inside. Begin sets up a
       pixel-space projection, alpha blending and no depth test, and
       saves whatever the engine had set; End puts it back. The quads
       between them are tGdi_Vertex triangles, and the textured ones
       above go in the same pass. */
    void(* OverlayBegin)(void);
    void(* OverlayDrawQuads)(const void* vertices, int triangleCount);
    void(* OverlayEnd)(void);

    void*(* p_CreateAlphaTexture)(int width, int height, const uint8_t* coverage);
    void(* DestroyAlphaTexture)(void* texture);
    void(* OverlayDrawTexturedQuads)(void* texture, const void* vertices,
                                     int triangleCount);

    /* The water, which is two passes in one place.
     *
     * PrepareWaterRendering runs once a frame before the world is
     * drawn: it clears the reflection target to the colour the water
     * is looking down into and keeps the camera as it is at that
     * moment, which is what the mirrored second draw of each object
     * is measured against. It also decides, by whether the engine has
     * water parameters at all, whether there is any water this frame
     * -- everything downstream reads that one answer.
     *
     * DrawWater draws the surface, once, after the world: a grid the
     * simulation has already built, with the reflection projected
     * onto it and pushed about by the wave. The grid and the
     * projection are in watermesh.c, shared between renderers,
     * because neither has a device in it.
     *
     * Both were called straight into one renderer until lately, so a
     * machine running the other one got no water at all. */
    void(* PrepareWaterRendering)(void);
    void(* DrawWater)(struct sWaterParamsGlobalStruct* water);

    uint8_t gap1_unused[4];
    HWND hwnd;
    int32_t drawMaskAllowedStuff;
    int flags;
    Vec3* verticesCurrent;
    int32_t drawMaskAllowedStuffCurrent;
    uint8_t gap6C1_unused[4];
    float NormalizedWidthOrHeightMultBy255;
    int* RLIPtr;
    int* mrmColors;
    float mrmColorFactor;
    float RLIFactor;
    int RLIDestColor;
    uint16_t* MRMPtr;
    float lightAttenuationFactor;
    struct sGEOMAT* currGEOMAT;
    struct sGameObjectMain* currTargetGao;
    struct sGEOGRO* GROGEO;
    struct sGEOMAT* extraMaterialToGeo;
    int winWidth;
    int winHeight;
    struct sDDContainerStruct2ForIDA ddcs2;
    struct sDDContainerStructForIDA ct;
    struct sLightListStruct lightList;
    uint8_t gap400_unused[4];
    struct sMatrixStackStruct matrixStack;
    struct sBigBufferStruct* bigBuffer;
    struct sFogParams fog;
    struct sTexManagerStruct texManager;
    int32_t presentationFlags;
    /* The renderer's private state, from its own CreateDevice. */
    void* rendererState;
    struct sWorld* CurrentWorld;
    struct sShadowStruct* shadowStruct;
    struct profilerRasterStructElem* rasters;
} tGdi_DisplayData;

typedef struct sGdi_SnapshotRequest
{
    int16_t flags;
    uint8_t gap[2];
    int cutSizeFromTopAndBottom;
    int cutSizeFromSides;
    int width;
    int height;
    char* srcData;
} tGdi_SnapshotRequest;

typedef struct sMatDecompressedStruct
{
    float f1;
    float f2;
    float f3;
    float f4;
    float speedU;
    float speedV;
} tMatDecompressedStruct;


tGdi_DisplayData* gpst_DX_stDD;
tGdi_DisplayData* fbss;
BOOL GDI_gl_ReadaptOperation;

Mtx4x4 GLOBAL_UVTransformMatrix;

BOOL GDI_gb_WaveSprite;

char asz_PrevWorldName[100];

extern float GDI_gaf_ScreenRatio[4];

/* Aspect handling, and settings rather than renderer state: with
   GDI_gi_Widescreen set, a projection is built from the shape of the
   window instead of a fixed ratio -- the vertical field of view stays
   as authored and the horizontal one grows with the window, so
   nothing is stretched and a wider screen simply shows more at the
   sides.

   GDI_gf_RefYoverX is the height over width the scenes were laid out
   for.

   They were named after Direct3D and defined inside it, which was
   only ever where they happened to live: cameraengine.c reads them,
   and every renderer's projection depends on them. */
extern int   GDI_gi_Widescreen;
extern float GDI_gf_RefYoverX;
extern void(*MAT_FuncTable[16])(tGdi_DisplayData*, tGEOGRO*, tGeoTriangleChunks*, unsigned int);
extern float MAT_C_f_8Bits;

int gMaxSimultaneousTextures;

void MAT_UVDecompress(struct sMultiTextureElemStruct* mte, struct sMatDecompressedStruct* matds);

void MAT_UV_Compute_OBJ1(tGdi_DisplayData* dd, tGEOGRO* geogro, tGeoTriangleChunks* gtc, unsigned int unk);
void MAT_UV_Compute_OBJ2(tGdi_DisplayData* dd, tGEOGRO* geogro, tGeoTriangleChunks* gtc, unsigned int unk);
void MAT_UV_Compute_DF_GIZMO(tGdi_DisplayData* dd, tGEOGRO* geogro, tGeoTriangleChunks* gtc, unsigned int unk);
void MAT_UV_Compute_PHONG_GIZMO(tGdi_DisplayData* dd, tGEOGRO* geogro, tGeoTriangleChunks* gtc, unsigned int type);
void MAT_UV_Compute_Previous(tGdi_DisplayData* dd, tGEOGRO* geogro, tGeoTriangleChunks* gtc, unsigned int unk);
void MAT_UV_Compute_FACE_MAP(tGdi_DisplayData* dd, tGEOGRO* geogro, tGeoTriangleChunks* gtc, unsigned int unk);
void MAT_UV_Compute_FOGZZ(tGdi_DisplayData* dd, tGEOGRO* geogro, tGeoTriangleChunks* gtc, unsigned int unk);
void MAT_UV_Compute_Default(tGdi_DisplayData* dd, tGEOGRO* geogro, tGeoTriangleChunks* gtc, unsigned int unk);

/* How a video frame is fitted to the screen. The engine says which
   it wants in these terms; what a renderer does about it, and what
   it calls its filters, is the renderer's business. */
#define GDI_VIDEOFIT_ACTUALSIZE  0      /* centred, a pixel for a pixel */
#define GDI_VIDEOFIT_SHARP       1      /* scaled to the screen, unsmoothed */
#define GDI_VIDEOFIT_SMOOTH      2      /* scaled to the screen, smoothed */

/* The renderer in force, set by GDI_fnl_InitInterface. NULL before
   it has run, and NULL if no renderer could run at all. The type and
   the picking live in renderers.h. */
struct sGdi_Renderer;
extern const struct sGdi_Renderer* GDI_gpst_Renderer;

/* Drawing state that both the engine and the renderer write, so it
   belongs to neither of them. The renderer clears them when it starts
   and reads them while it draws; the engine sets them as it decides
   what a material needs. */
int GDI_gi_TextureUVMode;
int GDI_gi_Diffuse2x;

/* The after-effects state the AI scripts move, declared here because
   both renderers read it and neither owns it. The scripts' way in is
   PC_AE_MASTER_CTRL, which sets these before it reaches a device and
   so keeps them whichever renderer is in force; the definitions still
   sit with the one renderer that has an after-effects pass. The depth
   blur's decision is built from them -- see depthblur.h. */
int   PC_AE_MASTER;
/* The two after-effect parameters that have no bit in the word above:
   they run whenever they are off neutral, which for both of them is a
   half. The scripts move them the same way and the engine puts them
   back to a half itself when the effects are switched off. The colour
   effects built from them are in aecolor.h.
 *
   Note that nought is **not** neutral for either of them, unlike the
   word above. A tentative definition cannot carry an initialiser, so
   the half comes from the definitions that sit with the renderer
   whose pass has always owned them -- which is linked whichever
   renderer is in force. Were it ever not, these would start at
   nought, and nought means a frame flattened to mid grey on the
   contrast and darkened by half on the brightness. Anything that
   links one renderer alone has to set them. */
/* The brightness after-effect's parameter: a flat grey added to the
   picture above a half and subtracted below it, which is what makes
   the effect brightness and not something else. A half leaves the
   picture alone and is what being off looks like -- this effect has
   no bit in PC_AE_MASTER. Its id is 8 for the parameter getters.

   The frozen copies in tests/ still spell it unkAEFunc2Param; the
   differential maps that spelling onto this one for its side, which
   is how the two functions renamed before it are handled. */
float brightnessParam;
float colorBalanceParam1;
/* How much of the grayscale, 0 to 1, with 0 meaning none of it. */
float blackAndWhiteParam1;
/* What an effect's first parameter currently is, by the id the comment
   above uses. Declared here rather than beside the renderer that
   defines it, because the overlay reads it and cannot see a header
   that needs the device types. */
float PC_AE_MASTER_GET_P1(int effect, int slot);
/* And the four that scale the effects building a frame out of several
   samples of one. Neutral for all four is nought, so unlike the two
   above a tentative definition left to itself is the right value. The
   effects built from them are in aeframe.h.

   The last one is the odd one: it scales how far the blur *reaches*
   rather than how much of it is mixed in, so a value under a pixel
   spreads nothing -- and still costs brightness, because the taps are
   interpolated. aeframe.h has what that comes to. */
float motionBlurFactor;
float remenanceFactor;
float zoomSmoothFactor;
float blurFactor;
/* And the second zoom smooth's, which is a fifth of that family rather
   than a member of it: the effect it scales is a *rotational* smear
   about the middle of the picture, not a sum of scaled samples, and it
   is built in aezoom2.h. Neutral is nought here too, and a negative
   value is legal -- it turns the samples the other way. */
float zoomSmoothFactor2;
/* The texture-coordinate transform the material path computes, one per
   stage, with stage nought the one the world's geometry uses. Declared
   here because both renderers read it and neither owns it: it is
   written by MAT_UV_ComputeAndSetTransform from the engine's own
   GLOBAL_UVTransformMatrix, whether or not there is a device to hand
   it to. The name is the one it has always had and says d3d for no
   good reason. */
Mtx4x4WithoutAdditionalData d3dTransformationMatrix[8];
float depthBlurNear1;
float depthBlurType0;
float depthBlurType1;
/* The fade's duration, in seconds, as a script asks for it. Neutral is
   nought, and nought is also the one value a fade cannot be given --
   see aefade.h, where what it comes to is written down. */
float gAEOpacity;

/* The hue sweep's two, and the third the scripts can set and nothing
 * reads.
 *
 * The names are the ones they have always had and neither says what it
 * is: the first is how deep the tint goes and the second is where in
 * the cycle it sits -- a phase, not a texture factor. Neutral for the
 * depth is nought, which leaves a draw that takes nothing off, because
 * the effect is gated by its bit alone. See aehue.h.
 *
 * The third is set and read back by the scripts' own calls and is
 * never looked at by anything that draws. It is here so that reading
 * it back gives what was written. */
float unknownAEFlag4GlobalVar;
float texFactorManual;
float texFactorProbablyUnunsedType2;

/* The fade's remaining opacity: the timer that effect steps itself,
 * from the whole range down to nothing.
 *
 * Unlike everything above it, this is not a tentative definition here.
 * The storage sits in the one renderer's own parameter block, because
 * that renderer was the only one when the scripts' way in was written
 * and PC_AE_MASTER_CTRL starts the timer there before any device is
 * known. These two reach it from either renderer without either one
 * including the other's header, and gdi.c -- which already knows both
 * -- is where they are defined.
 *
 * It is one number and not one each on purpose. A renderer keeping its
 * own copy would miss the start of a fade: the only thing that sets
 * the timer running is that call, and it cannot know which renderer
 * will come to read it. */
int  GDI_i_AeFadeOpacity(void);
void GDI_AeFadeOpacitySet(int opacity);

void GDI_fnl_InitInterface(struct sGdi_DisplayData* dd);
struct sGdi_DisplayData* GDI_fnpst_CreateDisplayData();
BOOL GDI_AttachDisplayAndReadaptDisplay(struct sGdi_DisplayData* gdd, HWND hwnd);
void GDI_DetachDisplay(struct sGdi_DisplayData* dd);
void GDI_DestroyDisplayData(struct sGdi_DisplayData* dd);
void s_Display(HWND hwnd, tGdi_DisplayData* dxdd);
void GDI_InitModule();
void GDI_CloseModule();
void DisplayAttach(struct sGdi_DisplayData* dd);
void GDI_l_AttachWorld(struct sGdi_DisplayData* dxdd, struct sWorld* world);
BOOL GDI_l_DetachWorld(struct sGdi_DisplayData* dd);
