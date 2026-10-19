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
    byte d3dUnkTextures[4]; // ez a joegtudja mi, a jatke nem csinal vele semmit, legalábbis futtatáskor
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
    byte gap16[16];
    IDirect3DVertexBuffer9* d3dNormalsBuffer;
    int d3dStride;
    int normalsFVF;
    int sumVertexNumInDrawElementIndexedTriangle;
    int currVertexStartOffset;
    struct sGameObjectMain* currTargetGaoInDrawElementIndexedTriangle;
    byte gap17[16];
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
    byte gap1_unused[4];
    HWND hwnd;
    int32_t drawMaskAllowedStuff;
    int flags;
    Vec3* verticesCurrent;
    int32_t drawMaskAllowedStuffCurrent;
    byte gap6C1_unused[4];
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
    byte gap400_unused[4];
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
    __int16 flags;
    byte gap[2];
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

void MAT_UVDecompress(struct sMultiTextureElemStruct* a1, struct sMatDecompressedStruct* a2);

void MAT_UV_Compute_OBJ1(tGdi_DisplayData* a1, tGEOGRO* a2, tGeoTriangleChunks* gtc, unsigned int unk);
void MAT_UV_Compute_OBJ2(tGdi_DisplayData* a1, tGEOGRO* a2, tGeoTriangleChunks* a3, unsigned int unk);
void MAT_UV_Compute_DF_GIZMO(tGdi_DisplayData* a1, tGEOGRO* a2, tGeoTriangleChunks* a3a, unsigned int unk);
void MAT_UV_Compute_PHONG_GIZMO(tGdi_DisplayData* a1, tGEOGRO* a2, tGeoTriangleChunks* a3, unsigned int a4);
void MAT_UV_Compute_Previous(tGdi_DisplayData* a1, tGEOGRO* a2, tGeoTriangleChunks* a3, unsigned int unk);
void MAT_UV_Compute_FACE_MAP(tGdi_DisplayData* a1, tGEOGRO* a2, tGeoTriangleChunks* a3, unsigned int unk);
void MAT_UV_Compute_FOGZZ(tGdi_DisplayData* a1, tGEOGRO* a2, tGeoTriangleChunks* a3, unsigned int unk);
void MAT_UV_Compute_Default(tGdi_DisplayData* a1, tGEOGRO* a2, tGeoTriangleChunks* a3, unsigned int unk);

void GDI_fnl_InitInterface(struct sGdi_DisplayData* a1);
struct sGdi_DisplayData* GDI_fnpst_CreateDisplayData();
BOOL GDI_AttachDisplayAndReadaptDisplay(struct sGdi_DisplayData* gdd, HWND hwnd);
void GDI_DetachDisplay(struct sGdi_DisplayData* a1);
void GDI_DestroyDisplayData(struct sGdi_DisplayData* a1);
void s_Display(HWND a1, tGdi_DisplayData* dxdd);
void GDI_InitModule();
void GDI_CloseModule();
void DisplayAttach(struct sGdi_DisplayData* a1);
void GDI_l_AttachWorld(struct sGdi_DisplayData* dxdd, struct sWorld* world);
BOOL GDI_l_DetachWorld(struct sGdi_DisplayData* a1);
