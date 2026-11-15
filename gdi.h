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
    int  (* b_SunSampleRead)(void* sampler, int* outPassed);
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
    void*(* p_CreateAlphaTexture)(int width, int height, const uint8_t* coverage);
    void(* DestroyAlphaTexture)(void* texture);
    void(* OverlayDrawTexturedQuads)(void* texture, const void* vertices,
                                     int triangleCount);
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
