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
#include "aftereffects/aemaster.h"

/* Named below in a parameter list and nowhere else, so without this
   each prototype would declare a type of its own, scoped to itself:
   two functions taking "the same" pointer would not agree, and
   assigning one to the other is a type error rather than the
   intended opaque handle. */
struct sEngPaletteElem;
struct sEngTexIntermediateStruct;
struct sEngTexListElemStruct;
struct sMultiTextureElemStruct;


/* The renderer's own private state, whatever renderer that is. The
   engine holds it as an opaque pointer and never looks inside: the
   Direct3D one is tGdi_D3DRenderParams in platform/d3drender.h, which is
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
    /* Where a texture's coordinates come from, what is done to them,
       and the transform itself. The first two take the engine's own
       GDI_UVSRC_ and GDI_UVXF_ values below, which a renderer turns
       into whatever it calls them; the engine says the same thing
       again in GDI_gi_TextureUVMode and the transform matrix, and a
       renderer that reads those for itself -- the OpenGL one does --
       fills all three with nothing. */
    void(* SetTexCoordIndex)(int stage, int source);
    void(* SetTextureTransformFlags)(int stage, int transform);
    void(* SetTexCoordMatrix)(int stage, const Mtx4x4* matrix);

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

    /* Build that cache for everything in a world that has just
       loaded, rather than one piece at a time as it is first drawn.
       The engine asks once, after the load; what gets built, and
       whether anything does, is the renderer's business.

       Through the interface because it is a renderer the engine is
       asking. It used to call the Direct3D one by name, which meant
       the call happened whichever renderer was running and the only
       right answer that renderer could give was to do nothing. */
    void(* BuildGeometryCaches)(struct sWorld* world);

    /* The depth blur's settings have moved, so whatever a renderer
       keeps them in wants rebuilding. The scripts set them one at a
       time and each change comes through here.

       What that costs is the renderer's business: one of them keeps
       the ramp in a texture it has to lock and copy, so it wants
       telling; the other compares the settings it drew with last
       frame and re-uploads when they differ, so being told is
       nothing it needs. Both answer the entry.

       Through the interface for the same reason as the line above:
       the scripts' way in used to call the Direct3D one by name,
       which meant the call happened whichever renderer was running
       and that renderer had to carry a guard against having no
       device. */
    void(* RebuildDepthBlurRamp)(void);

    /* A frame is about to be drawn. Called once at the top of it,
       before anything is cleared.

       What that means is the renderer's business: one of them has a
       scene to open and a render target to point at the back buffer
       again, and the other has nothing to do, because its own calls
       set up what they need as they come. Both answer the entry.

       Through the interface for the same reason as the two above: the
       engine used to call the Direct3D one by name, so the call
       happened whichever renderer was running and that renderer
       carried a guard against having no device to open a scene on. */
    void(* BeginScene)(void);

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

    /* Which renderer this is, for the overlay to say so. A short
       name, not the device's -- the driver and the card are named in
       the log where there is room for them. */
    const char*(* sz_Name)(void);

    /* The after-effects, run where the engine asks for them: after the
       world and before the list it keeps for what goes on top of them,
       which is the heads-up display. A renderer that ran them at the
       end of the frame instead would smear and blur the display along
       with the picture. */
    void(* AfterEffects)(struct sWorld* world);

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

/* Where a texture's coordinates come from, and what is done to them.
   The engine's own words, deliberately not any renderer's numbers:
   Direct3D's for these differ between a real d3d9.h and the stub the
   tests build against, so a value carried through here rather than
   translated inside the renderer would be right in one build and
   wrong in the other. Nought is the stored coordinate and no
   transform in every spelling, which is what the call sites that
   predate a second renderer pass. */
#define GDI_UVSRC_STORED    0   /* the vertex's own stored coordinate */
#define GDI_UVSRC_NORMAL    1   /* the camera-space normal */
#define GDI_UVSRC_POSITION  2   /* the camera-space position */
#define GDI_UVXF_NONE       0   /* no transform */
#define GDI_UVXF_2D         1   /* two coordinates out of the transform */
extern float MAT_C_f_8Bits;

int gMaxSimultaneousTextures;

void MAT_UVDecompress(struct sMultiTextureElemStruct* mte, struct sMatDecompressedStruct* matds);

/* The UV transform matrix out of three axes, a position and a scale.
   It is the engine's own arithmetic and every UV mode uses it. */
void PC_SetUVMatrix(Mtx4x4* dest, Vec3* axisU, Vec3* axisV, Vec3* axisW, Vec3* pos, Vec3* scale, int unused, int flags);

void MAT_UV_Compute_OBJ1(tGdi_DisplayData* dd, tGEOGRO* geogro, tGeoTriangleChunks* gtc, unsigned int unk);
void MAT_UV_Compute_OBJ2(tGdi_DisplayData* dd, tGEOGRO* geogro, tGeoTriangleChunks* gtc, unsigned int unk);
void MAT_UV_Compute_DF_GIZMO(tGdi_DisplayData* dd, tGEOGRO* geogro, tGeoTriangleChunks* gtc, unsigned int unk);
void MAT_UV_Compute_PHONG_GIZMO(tGdi_DisplayData* dd, tGEOGRO* geogro, tGeoTriangleChunks* gtc, unsigned int type);
void MAT_UV_Compute_Previous(tGdi_DisplayData* dd, tGEOGRO* geogro, tGeoTriangleChunks* gtc, unsigned int unk);
void MAT_UV_Compute_FACE_MAP(tGdi_DisplayData* dd, tGEOGRO* geogro, tGeoTriangleChunks* gtc, unsigned int unk);
void MAT_UV_Compute_FOGZZ(tGdi_DisplayData* dd, tGEOGRO* geogro, tGeoTriangleChunks* gtc, unsigned int unk);
void MAT_UV_Compute_Default(tGdi_DisplayData* dd, tGEOGRO* geogro, tGeoTriangleChunks* gtc, unsigned int unk);

/* The three that are computed in materialuv.c rather than gdi.c,
   because they are the modes that want the device told about the
   projection as well as the engine. The arithmetic is the engine's
   like the rest of them, so they are declared here with their
   siblings; what a renderer does about the result goes through the
   renderer's own table.

   They were declared in the Direct3D renderer's header, which is why
   materialanddraw.c -- which calls the first of them straight -- had
   to include that header to draw anything. */
void MAT_UV_ComputeAndSetTransform(tGEOGRO* geogro, tGeoTriangleChunks* chunks, struct sMultiTextureElemStruct* texElem);
void MAT_UV_Compute_Chrome(tGdi_DisplayData* dd, tGEOGRO* geogro, tGeoTriangleChunks* chunks, unsigned int type);
void MAT_UV_Compute_PLANAR_GIZMO_NOSTORE(tGdi_DisplayData* dd, tGEOGRO* geogro, tGeoTriangleChunks* chunks, unsigned int type);

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

/* The after-effects state the AI scripts move -- which effects are
   running and how hard -- used to be declared here. It is in
   aftereffects/aemaster.h now, which this header includes, and the
   storage is in one file rather than one copy per includer. */

/* The texture-coordinate transform the material path computes, one per
   stage, with stage nought the one the world's geometry uses. Declared
   here because both renderers read it and neither owns it: it is
   written by MAT_UV_ComputeAndSetTransform from the engine's own
   GLOBAL_UVTransformMatrix, whether or not there is a device to hand
   it to. The name is the one it has always had and says d3d for no
   good reason. */
Mtx4x4WithoutAdditionalData d3dTransformationMatrix[8];
/* The fade's remaining opacity: the timer that effect steps itself,
 * from the whole range down to nothing.
 *
 * The storage is in gdi.c, which is state the engine keeps and not a
 * renderer's: the scripts' way in starts the timer running before any
 * device is known, and whichever renderer is in force then reads it,
 * steps it and writes it back. It used to sit in the Direct3D
 * renderer's own parameter block -- that renderer was the only one
 * when the scripts' call was written -- which left the engine's state
 * inside a renderer that may not be the one running.
 *
 * It is one number and not one each on purpose. A renderer keeping its
 * own copy would miss the start of a fade: the only thing that sets
 * the timer running is that call, and it cannot know which renderer
 * will come to read it. */
int  GDI_i_AeFadeOpacity(void);
void GDI_AeFadeOpacitySet(int opacity);

/* The engine's side of the entry above: the scripts' control call says
   a depth-blur setting moved and this passes that to whichever
   renderer is in force, or to none if there is not one yet. Here with
   the two above, and defined in gdi.c with them, for the same reason
   -- it is the one file that knows both renderers. */
void GDI_DepthBlurRampRebuild(void);

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
