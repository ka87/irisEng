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
#include <d3d9.h>


typedef struct sGdi_D3DRenderParams
{
    int d3d_Adapter;
    int d3d_AdapterFormat;
    int ZBufferDepth;
    int hwnd2_dword_B3162C;
    IDirect3D9* d3d_if;
    D3DPRESENT_PARAMETERS presentParams;
    LPDIRECT3DDEVICE9 d3dDev;
    D3DCAPS9 d3dcaps;
    int dword_B317A0;
    int setsDefaultZEnableAtBeginScene;
    int d3d_defaultZEnable;
    Mtx4x4* projectionMatrix;
    struct tagRECT Rect;
    D3DVIEWPORT9 d3dViewPort;
    D3DMATRIX projectionMatrixBackup;
    IDirect3DSurface9* renderTargetBackup;
    uint8_t d3dUnkTextures[4]; // ez a joegtudja mi, a jatke nem csinal vele semmit, legalábbis futtatáskor
    IDirect3DTexture9* d3dShadowTextures[8];
    D3DVIEWPORT9 pViewPort;
    IDirect3DTexture9* blurTex;
    IDirect3DSurface9* newZStencil;
    IDirect3DSurface9* pBackBuffers[2];
    IDirect3DSurface9* pDepthStencilSurface;
    IDirect3DTexture9** d3dTextures;
    IDirect3DTexture9** d3dSourceTextures;
    int d3dTextureNum;
    IDirect3DTexture9* pDestTexture; // blorolt textura (lasd D3D_UpdateBlurTexture)
    IDirect3DTexture9* pSourceTexture; // orig textura
    int d3dNeedToCallEndScene;
    /* Dead in the live build: the two below are GDI_gi_TextureUVMode
       and GDI_gi_Diffuse2x now, because the engine writes them as
       well as reading them and they are no renderer's to own. They
       stay here because the frozen copies in tests/ still name them,
       and a struct member cannot be mapped the way a renamed symbol
       can. Nothing live should read or write either. */
    int textureUVMode;
    Mtx4x4WithoutAdditionalData transformationMatrix;
    int textureUVIsProjected;
    int isDepthBlurOn;
    int tweak_zonalpha;
    int sunEffectIsOn;
    int tweak_ae_noflare;
    int diffuse2x;
    int tweak_ae_d2x;
    int Mipmapping;
    int argbIsSupportedForTextures;
    int d3dFogModeProbably;
    int d3dFogColor;
    int d3dFogColor2Case5and6;
    IDirect3DVertexBuffer9* d3dXYZVB;
    int stride2;
    int xyzFVF;
    int prevVertexElemIdx2;
    int startVertexElemIdx2;
    struct sGameObjectMain* currTargetGao;
    uint8_t gap16[16];
    IDirect3DVertexBuffer9* d3dNormalsBuffer;
    int d3dStride;
    int normalsFVF;
    int sumVertexNumInDrawElementIndexedTriangle;
    int currVertexStartOffset;
    struct sGameObjectMain* currTargetGaoInDrawElementIndexedTriangle;
    uint8_t gap17[16];
    IDirect3DVertexBuffer9* d3dColorsVB;
    int stride;
    int colorsFVF;
    int prevVertexElemIdx;
    int startVertexElemIdx;
    struct sGameObjectMain* currTargetGaoInDrawElementIndexedTriangle2;
    int *vertexColorArray;
    int colorMask;
    int colorMaskAlpha2;
    int solidVertexColor;
    IDirect3DVertexDeclaration9* vertexDeclarationIFd3dUVVBIsNull;
    IDirect3DVertexDeclaration9* vertexDeclarationIFd3dUVVBISnotNull;
    IDirect3DVertexDeclaration9* d3dVertexDeclaration;
    int colorMaskAlpha;
    IDirect3DTexture9* blurRenderTarget2;
    IDirect3DTexture9* blurRenderTarget;
    IDirect3DTexture9* unktex1_dword_B31988;
    IDirect3DTexture9* unktex2_dword_B3198C;
    IDirect3DTexture9* tmpRenderTarget;
    int opacity;
} tGdi_D3DRenderParams;

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


typedef struct sGdi_DisplayData
{
    struct sGdi_D3DRenderParams* (* CreateDevice)();
    void(* DestroyDevice)(struct sGdi_D3DRenderParams* x);
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
    void(* D3D_ReleaseTextures)(struct sGdi_DisplayData* a1);
    void(* D3D_SetRenderTargetAndBackupPreviousSDW)(int, int);
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
    struct sGdi_D3DRenderParams* pd3drp;
    struct sWorld* CurrentWorld;
    struct sShadowStruct* shadowStruct;
    struct profilerRasterStructElem* rasters;
} tGdi_DisplayData;

typedef struct sD3dRequestSnapshot
{
    int16_t flags;
    uint8_t gap[2];
    int cutSizeFromTopAndBottom;
    int cutSizeFromSides;
    int width;
    int height;
    char* srcData;
} tD3dRequestSnapshot;

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
