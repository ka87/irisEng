#pragma once
#include "../common.h"
#include "../gdi.h"
#include "../gfx/gfxsun.h"
#include <d3dx9math.h>
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

typedef struct sRasterNames
{
	char name[32];
	int unk1;
} tRasterNames;

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

ID3DXFont* d3dxfontIF;
char* rastersDrawPrimitiveUPDataStart;
char* rastersDrawPrimitiveUPDataEnd;

//AE
int PC_AE_MASTER;
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

void MAT_ComputeUV_sub_4521A0(tGEOGRO* geogro, tGeoTriangleChunks* a2, tMultiTextureElemStruct* a3);
void MAT_UV_Compute_PLANAR_GIZMO_NOSTORE(tGdi_DisplayData* a1, tGEOGRO* a2, tGeoTriangleChunks* a3, unsigned int a4);
void MAT_UV_Compute_Chrome(tGdi_DisplayData* a1, tGEOGRO* a21, tGeoTriangleChunks* a31, unsigned int a41);
void D3D_SetProjectionMatrix(tDDCamStruct* a1);
void D3D_BeginSceneCall();
void D3D_SetVertexShader(IDirect3DVertexShader9* a1);

void D3D_CheckReturnAndRunSettingsApplicationIfProblem(int a2);

// temp
int D3D_CalculateStride(unsigned int a1);

void D3D_SaveAEParams();
void D3D_CreateBlurTextures();
void D3D_AETexturesInit();

void D3D_DoBlurWaterTexture();
void D3D_DrawWater_DrawVertexTable(struct sWaterParamsGlobalStruct* a1);

struct sGdi_D3DRenderParams* D3D_pst_CreateDevice();
void D3D_DestroyDevice(struct sGdi_D3DRenderParams* a1);
int D3D_l_Init(HWND hWnd, struct sGdi_DisplayData* a6);
int D3D_l_Close();
void D3D_Clear(int a1, int color);
void D3D_Flip();
int D3D_SetViewMatrix(Mtx4x4* a1);
void D3D_SetProjectionMatrix(tDDCamStruct* a1);
int D3D_l_DrawElementIndexedTriangles(struct sGeoTriangleChunks* triangleChunks, Vec3* verticesIN, struct sGeoUVElem* UVs, int);
int D3D_DrawSprites(struct sGeoSprites* a1, void* a2, int x);
int D3D_l_Request(int type, void* param);
void D3D_l_Texture_Init(int a1, int a2, char a3);
int D3D_l_Texture_Store(struct sGdi_DisplayData* a1, int a2);
void D3D_Palette_Load(struct sGdi_DisplayData* dd, struct sEngTexListElemStruct* tle, struct sEngTexIntermediateStruct* a3, int idx);
void D3D_ReleaseTextures(struct sGdi_DisplayData* a1);
void D3D_SetRenderTargetAndBackupPreviousSDW(int a1, int clear);
void D3D_SetViewMatrix_SDW(Mtx4x4* a1, float* x);
void D3D_Texture_Unload(struct sGdi_DisplayData* a1, struct sEngPaletteElem* a2, int a3);
int D3D_l_ReadaptDisplay(HWND hwnd, struct sGdi_DisplayData* gdd);
int  D3D_Set_Texture_Palette(struct sGdi_DisplayData* a1, struct sEngTexListElemStruct* a2, struct sEngTexIntermediateStruct* a3, int a4);
void D3D_SetTextureTarget(struct sGdi_DisplayData* a1, int a2, int a3, int a4);
int D3D_UpdateBlurTexture();
void D3D_CreateSunTextures(tGfxSunD3DSpecificData* a1);
uint32_t  D3D_DepthBlurBlurFuncCalc2(float a1);
void D3D_ReleaseSunRenderTargetAndTexture(struct sGfxSunD3DSpecificData* a1);
void D3D_PrepareWaterRendering();
void D3D_PC_AE(tWorld* param1);
void PC_FreeABackCache(struct sGEOGRO* a1);
void D3D_CreateGameObjectsD3DStuffForMultiStream(struct sWorld* a1);

void PC_EngineCheatFinal();
void s_HandleWinMessages();
void ENG_WinGameWindowInit(HINSTANCE a1);

void PC_AE_MASTER_CTRL(int a1, int a2, int a3, float a4);
void PC_CIN_CutsceneMode(int on);

// Aspect handling. With D3D_gi_Widescreen set, the projection is
// built from the shape of the window instead of a fixed ratio: the
// vertical field of view stays as authored and the horizontal one
// grows with the window, so nothing is stretched and a wider screen
// simply shows more at the sides.
//
// D3D_gf_RefYoverX is the height over width the scenes were laid
// out for. The cutscene bands take the picture back to that frame:
// the top and bottom keep the size they always had, and the sides
// cover whatever the wider screen added.
extern int   D3D_gi_Widescreen;
extern float D3D_gf_RefYoverX;

// How much of the height each cutscene band covers.
extern float D3D_gf_CutsceneBand;

// The width the flat layer is laid out across, given the size of
// the window. On a window wider than the scenes were laid out for
// this is narrower than the window itself.
int D3D_FlatLayerWidth(int w, int h);

// Set to log the framing once a frame through the debug output.
extern int D3D_gi_AspectTrace;
void D3D_AspectTrace(const char* where, int viewX, int viewY, int viewW, int viewH);

// Height over width of the area being presented.
float D3D_RealYoverX(void);

// Where the cutscene bands fall, as fractions of the back buffer.
// A window taller than the framed aspect gets bands at the top and
// bottom, a wider one gets them at the sides, and an ultra wide one
// gets both.
void D3D_GetBandFractions(float* top, float* bottom, float* left, float* right);

// Horizontal counterpart of bandsVal1 and bandsVal2.
extern float bandsValU1;
extern float bandsValU2;
float PC_AE_MASTER_GET_P1(int a1, int a2);