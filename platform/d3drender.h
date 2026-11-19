#pragma once
#include "../common.h"
#include "../gdi.h"
#include "../gfx/gfxsun.h"
#include "d3dsun.h"

/* The Direct3D renderer's own private state. The engine holds this
   as an opaque pointer; nothing outside platform/ looks inside it. */
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
    /* The fade's timer, which this renderer no longer keeps: it is the
       engine's, behind GDI_i_AeFadeOpacity. The field stays because
       the frozen pre-rewrite copy in tests/ still writes it, and a
       frozen copy is not edited to follow a rewrite. */
    int opacity;
} tGdi_D3DRenderParams;

struct sWaterParamsGlobalStruct;
tGdi_D3DRenderParams D3DRenderParams;

#define MY_QS_ALLINPUT_COMPATIBLE \
                            (QS_MOUSE        | \
                            QS_KEY           | \
                            QS_POSTMESSAGE   | \
                            QS_TIMER         | \
                            QS_PAINT         | \
                            QS_HOTKEY        | \
                            QS_SENDMESSAGE)

/*
extern int d3dTexCoordIndex;
extern int d3dTextureTransformFlags;
extern int d3d_zEnable;
extern int d3d_zWriteEnable;
*/
double rastersUnkTimeFloats_dbl_990B58[54];

typedef struct sD3dSamplerConfigStruct
{
	int d3d_addressu;
	int d3d_addressv;
	int d3d_minfilter;
	int d3d_magfilter;
	int d3d_mipfilter;
	float d3d_mipmaplodbias;
} tD3dSamplerConfigStruct;

typedef struct sD3dTextureStageTypeStruct
{
	int d3dTextureID;
	int d3d_colorOp;
	int d3d_colorarg1;
	int d3d_colorarg2;
	int d3d_alphaOp;
	int d3d_alphaArg1;
	int d3d_alphaArg2;
	float d3d_bumpenvmat00;
	float d3d_bumpenvmat01;
	float d3d_bumpenvmat10;
	float d3d_bumpenvmat11;
	int d3dTexCoordIndex;
	float d3d_bumpEnvLScale;
	float d3d_bumpEnvLOffset;
	int d3dTextureTransformFlags;
	int d3d_colorArg0;
	int d3d_alphaArg0;
	int d3d_resultArg0;
	int d3d_constant;
} tD3dTextureStageTypeStruct;

typedef struct sD3dRenderStateConfigStruct
{
	int d3d_zEnable;
	int d3d_zFunc;
	int d3d_zWriteEnable;
	int d3dFogEnable;
	int d3dFogColor;
	int d3dFogTableMode;
	int d3dFogVertexMode;
	float d3dFogDensity;
	float d3dFogStart;
	float d3dFogEnd;
	int d3dAlphaTest;
	int d3dAlphaFunc;
	int d3dAlphaRef;
	int d3dAlphaBlendEnable;
	int d3dSrcBlend;
	int d3dDestBlend;
	int d3d_BlendOp;
	int d3dFillMode;
	int d3d_Lighting;
	int d3dCullMode;
	int d3dColorWriteEnable;
	int d3d_normalizeNormals;
	int d3dTextureFactor;
	int d3dClipPlaneEnable;
	IDirect3DVertexShader9* vertexShader;
	int fvf;
} tD3dRenderStateConfigStruct;

/* tRasterNames and the table itself are in profiler.h now: they name
   the profiler's rasters, and both renderers' performance displays
   read them. */

typedef struct sVertexAndColor {
	Vec3 v;
	int color;
} tVertexAndColor;

// temp for AE DepthBur & co
typedef struct sVertex_type_204
{
	Vec3 v1;
	float hw;
	tGeoUVElem uvSet1;
	tGeoUVElem uvSet2;
} tVertex_type_204;

typedef struct sVertex_type_404
{
	Vec3 v1;
	float hw;
	tGeoUVElem uvSet1;
	tGeoUVElem uvSet2;
	tGeoUVElem uvSet3;
	tGeoUVElem uvSet4;
} tVertex_type_404;

typedef struct sVertex_type_244
{
	Vec3 v1;
	float hw;
	unsigned int color;
	tGeoUVElem uvSet1;
	tGeoUVElem uvSet2;
} tVertex_type_244;

typedef struct sVertex_type_104
{
	Vec3 v1;
	float hw;
	tGeoUVElem uvSet1;
} tVertex_type_104;

typedef struct sVertex_type_144
{
	Vec3 v1;
	float hw;
	unsigned int color;
	tGeoUVElem uvSet1;
} tVertex_type_144;

// --

typedef struct sVertex_type_water
{
	Vec3 v;
	Vec3 normal;
	unsigned int diffuse;
	unsigned int specular;
} tVertex_type_water;

typedef struct sVertex_type_444
{
	Vec3 v1;
	float hw;
	unsigned int color;
	tGeoUVElem uvSet1;
	tGeoUVElem uvSet2;
	tGeoUVElem uvSet3;
	tGeoUVElem uvSet4;
} tVertex_type_444;


// main
typedef struct sVertex_type_142_generic
{
	Vec3 v1;
	unsigned int color;
	tGeoUVElem uvSet1;
} tVertex_type_142_generic;


int waterOffsetInBytes;

typedef struct sUnkWaterBlurStruct
{
	int i1;
	int i2;
	int i3;
	int i4;
	int i5;
	int i6;
} tUnkWaterBlurStruct;

typedef struct sD3dSpriteData
{
	Vec3 v1;
	Vec3 v2;
	Vec3 v3;
	Vec3 v4;
	int color;
} tD3dSpriteData;


tD3dTextureStageTypeStruct textureStageTypeConfig[8];
tD3dSamplerConfigStruct samplerConfig[8];
tD3dRenderStateConfigStruct renderStateConfig;

tD3dSamplerConfigStruct samplerStateBackup[8];
tD3dTextureStageTypeStruct texturestageBackup[8];
tD3dRenderStateConfigStruct renderstateBackup;
int d3dparamsSaveType;

IDirect3DVertexBuffer9* VBOs[3];
int d3dStreamCurrentOffset; // todo kinullazni nem kene?
IDirect3DSurface9* d3dWaterRTtoSet;
IDirect3DTexture9* waterTexture;
IDirect3DVertexBuffer9* waterStreamData;
IDirect3DIndexBuffer9* waterIndexData;
IDirect3DSurface9* d3dRTToGet; // water
IDirect3DSurface9* d3dWaterDSSurfaceToGet; // water
D3DVIEWPORT9 d3dViewPort; // water
Mtx4x4 d3dCameraMtx; // water
int d3dCullModeBackup;
IDirect3DSurface9* d3dDSSurfaceToSet;
tGfxSunD3DSpecificData* gD3dSunSurfaces[16];
int d3dStreamActDataSize;
tVertex_type_142_generic* d3d_requestDrawSelectedVBO; // sprite request
int d3dRequestPrimitiveCount; // sprite request
int d3dRequestPrimitiveCount2; // sprite request

int water_ClippingHasBeenSet;


Mtx4x4WithoutAdditionalData d3dTransformationMatrix[8]; // TODO IFXME? van egy hasonló sak kis t-vel, átnevezni

D3DMATRIX d3dTransformMatrixBackup;
D3DMATRIX d3dTransformMatrix2Backup;

HWND MAI_gh_MainWindow;
int ENG_WIN_AutoPlaySupressorRegistered;


int currentVBOSelected;
int d3d_alphablendIsCase3;

char* rastersDrawPrimitiveUPDataStart;
char* rastersDrawPrimitiveUPDataEnd;

//AE
extern int CIN_Bands;
extern int CIN_NoBands;
int depthblur_Type;
float camUnkf1Save;
float deptBlurNear1Save;
float deptBlurType0Save;
float deptBlur1TypeSave;
float depthBlur1TypeOrcamUnkf2Save;
Vec3 AE_MS_CurrVec;
Vec3 AE_MS_PrevVec;

void MAT_UV_ComputeAndSetTransform(tGEOGRO* geogro, tGeoTriangleChunks* chunks, tMultiTextureElemStruct* texElem);
void MAT_UV_Compute_PLANAR_GIZMO_NOSTORE(tGdi_DisplayData* dd, tGEOGRO* geogro, tGeoTriangleChunks* chunks, unsigned int type);
void MAT_UV_Compute_Chrome(tGdi_DisplayData* dd, tGEOGRO* geogro, tGeoTriangleChunks* chunks, unsigned int type);
// the overlay's screen-space pass, used by platform/overlay.c
void D3D_OverlayBegin(void);
void D3D_OverlayDrawQuads(const void* verts, int triangleCount);
int  D3D_b_Ready(void);
const char* D3D_sz_Name(void);
void D3D_OverlayEnd(void);

void D3D_SetProjectionMatrix(tDDCamStruct* cam);
void D3D_BeginSceneCall(void);
void D3D_SetVertexShader(IDirect3DVertexShader9* shader);

/* The engine's drawing-state interface, filled into tGdi_DisplayData. */
void D3D_SetCanDiffuse2x(int on);
void D3D_SetWantsNoFlare(int on);
void D3D_SetNoZonAlpha(int on);
void D3D_GetDrawingSize(int* width, int* height);
int  D3D_b_CanDiffuse2x(void);
int  D3D_b_WantsNoFlare(void);
int  D3D_b_NoZonAlpha(void);
int  D3D_b_DepthBlurOn(void);
void D3D_SetZEnable(int on);
int  D3D_i_ZEnable(void);
void D3D_SetZWriteEnable(int on);
void D3D_SetTexCoordIndex(int stage, int source);
void D3D_SetTexCoordMatrix(int stage, const Mtx4x4* matrix);
void D3D_SetTextureTransformFlags(int stage, int transform);

void D3D_CheckReturnAndRunSettingsApplicationIfProblem(int result);

// temp
int D3D_CalculateStride(unsigned int fvf);

void D3D_SaveAEParams();
void D3D_CreateBlurTextures();
void D3D_AETexturesInit();

void D3D_DoBlurWaterTexture();
void D3D_DrawWater_DrawVertexTable(struct sWaterParamsGlobalStruct* water);

struct sGdi_D3DRenderParams* D3D_pst_CreateDevice();
void D3D_DestroyDevice(struct sGdi_D3DRenderParams* renderParams);
int D3D_l_Init(HWND hWnd, struct sGdi_DisplayData* dd);
int D3D_l_Close();
void D3D_Clear(int flags, int color);
void D3D_Flip();
int D3D_SetViewMatrix(Mtx4x4* mtx);
void D3D_SetProjectionMatrix(tDDCamStruct* cam);
int D3D_l_DrawElementIndexedTriangles(struct sGeoTriangleChunks* triangleChunks, Vec3* verticesIN, struct sGeoUVElem* UVs, int);
int D3D_DrawSprites(struct sGeoSprites* sprites, void* positions, int num);
int D3D_l_Request(int type, void* param);
void D3D_l_Texture_Init(int textureID, int flags, char mode);
int D3D_l_Texture_Store(struct sGdi_DisplayData* dd, int textureNum);
void D3D_Palette_Load(struct sGdi_DisplayData* dd, struct sEngTexListElemStruct* tle, struct sEngTexIntermediateStruct* inter, int idx);
void D3D_ReleaseTextures(struct sGdi_DisplayData* dd);
void D3D_SetRenderTargetAndBackupPreviousSDW(int textureID, int clear);
void D3D_SetViewMatrix_SDW(Mtx4x4* mtx, float* unkVals);
void D3D_Texture_Unload(struct sGdi_DisplayData* dd, struct sEngPaletteElem* palette, int idx);
int D3D_l_ReadaptDisplay(HWND hwnd, struct sGdi_DisplayData* gdd);
int  D3D_Set_Texture_Palette(struct sGdi_DisplayData* dd, struct sEngTexListElemStruct* tle, struct sEngTexIntermediateStruct* inter, int idx);
void D3D_SetTextureTarget(struct sGdi_DisplayData* dd, int target, int width, int height);
int D3D_UpdateBlurTexture();
void D3D_CreateSunTextures(tGfxSunD3DSpecificData* sun);
uint32_t  D3D_DepthBlurBlurFuncCalc2(float depth);
void D3D_ReleaseSunRenderTargetAndTexture(struct sGfxSunD3DSpecificData* sun);
void D3D_PrepareWaterRendering(void);
void D3D_PC_AE(tWorld* param1);
/* A geometry's buffers are opaque handles in geo.h, because they
   belong to whichever renderer made them. These are this
   renderer's view of its own. */
#define D3D_GEO_VB(g)     ((IDirect3DVertexBuffer9*)(g)->vertexBuffer)
#define D3D_GEO_VB2(g)    ((IDirect3DVertexBuffer9*)(g)->vertexBuffer2)
#define D3D_GEO_UVVB(g)   ((IDirect3DVertexBuffer9**)(g)->uvBuffers)
#define D3D_GEO_IB(g)     ((IDirect3DIndexBuffer9*)(g)->indexBuffer)

void* D3D_p_CreateAlphaTexture(int width, int height, const uint8_t* coverage);
void D3D_DestroyAlphaTexture(void* texture);
void D3D_OverlayDrawTexturedQuads(void* texture, const void* vertices,
                                  int triangleCount);
int  D3D_b_GeometryHasCache(struct sGEOGRO* gro);
void D3D_DiscardGeometryCache(struct sGEOGRO* gro);
void PC_FreeABackCache(struct sGEOGRO* geogro);
void D3D_CreateGameObjectsD3DStuffForMultiStream(struct sWorld* world);

void D3D_SaveScreenshot(void);

/* Told that a depth-blur setting moved: the ramp this renderer keeps
   in a texture is rebuilt from the settings as they now stand. */
void D3D_RebuildDepthBlurRamp(void);

// Aspect handling. The two settings every renderer needs are in
// gdi.h, because cameraengine.c reads them as well and a second
// renderer's projection depends on them; this one is about the flat
// layer box, which only this renderer draws.

// How much of the height each cutscene band covers.


// Set to log the framing once a frame through the debug output.
extern int D3D_gi_AspectTrace;
void D3D_AspectTrace(const char* where, int viewX, int viewY, int viewW, int viewH);

// Height over width of the area being presented.
float D3D_RealYoverX(void);

// Where the cutscene bands fall, as fractions of the back buffer.
void D3D_GetBandFractions(float* top, float* bottom);

