/* Differential test for win/wingdi.c, the Direct3D 9 renderer.
 *
 * The frozen pre-pass copy and the rewrite go into one binary with
 * every function they define prefixed BASE_/NEW_, and with the eleven
 * globals the file defines strongly prefixed too so each side has its
 * own.  Everything else the renderer keeps -- D3DRenderParams, the
 * render-state, sampler and texture-stage shadow copies and their
 * backups, the vertex-buffer ring, the water and sun tables -- is a
 * tentative definition in the headers and is deliberately shared, so
 * the two sides are driven over the same state and compared on it.
 *
 * Compared per case: the return value, the whole shared renderer
 * state, each side's own matrices and aspect globals, a hash of
 * everything written through a Lock() or a LockRect(), and the ordered
 * sequence of Direct3D and engine calls with their arguments.  Floats
 * go into the call log as bit patterns.
 *
 * The real math.c and profiler.c are linked rather than mocked, with
 * MATH_InitModule called before the first case so the trig and inverse
 * tables are the engine's own.
 */

#include "wingdi_mocks.h"
#include "../math.h"
#include "../cameraengine.h"
#include "../soft.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ---------- the two sides ---------- */

#define DECL(p) \
    void p##D3D_SetInitialRenderStates(void); \
    void p##D3D_SetInitialSamplerStates(void); \
    void p##D3D_SetInitialTextureStageStates(void); \
    void p##D3D_RenderStateRestore(tD3dRenderStateConfigStruct*); \
    void p##D3D_SamplerStateRestore(tD3dSamplerConfigStruct*); \
    void p##D3D_TextureStageStateRestore(tD3dTextureStageTypeStruct*); \
    void p##D3D_SetTexture(int, int); \
    void p##D3D_SetVertexShader(IDirect3DVertexShader9*); \
    void p##D3D_SaveEverythingToBackup(int); \
    void p##D3D_RestoreEverythingFromBackup(void); \
    void p##D3D_SetColorWriteEnable(char); \
    void p##D3D_SetCullingParams(void); \
    void p##D3D_SetTextureStageStates(int, int); \
    void p##D3D_SetAlphaTestParams(unsigned int, int16_t); \
    void p##D3D_SetAlphaBlendingOptions(unsigned int, char); \
    void p##D3D_SetFogParams(unsigned int); \
    void p##D3D_l_Texture_Init(int, int, char); \
    void p##D3D_Request_SetFogParams(tFogParams*); \
    int  p##D3D_CalculateStride(unsigned int); \
    int  p##D3D_SelectSupportedDeviceFormat(int); \
    void p##D3D_ApplyRenderingPreesetsForVariousGraphicsCards(void); \
    void p##D3D_SetViewPort(int, int, int, int); \
    void p##D3D_CalcAndSetProjection(float, float, float, float, float, float); \
    void p##D3D_SetProjectionMatrix(tDDCamStruct*); \
    float p##D3D_RealYoverX(void); \
    int  p##D3D_FlatLayerWidth(int, int); \
    int  p##D3D_FlatLayerOffsetX(int, int); \
    float p##D3D_FlatLayerToLayoutX(float, int, int); \
    float p##D3D_FlatLayerFromLayoutX(float, int, int); \
    void p##D3D_Clear(int, int); \
    void p##D3D_SetRenderTargetAndBackupPreviousSDW(int, int); \
    int  p##D3D_SetViewMatrix(Mtx4x4*); \
    void p##D3D_SetViewMatrix_SDW(Mtx4x4*, float*); \
    void p##D3D_SetTransformAndSaveMatrix(IDirect3DTexture9*, int); \
    int  p##D3D_setWaterRenderParamsIfWaterRenderingIsActive(void); \
    void p##D3D_RestoreRenderParamsAfterWaterRendering(void); \
    void p##D3D_PrepareWaterRendering(void); \
    tVertex_type_142_generic* p##D3D_LockCurrentVBO(int); \
    void p##D3D_FillCurrentVBO(int); \
    void p##D3D_Request_PrepareForDrawing(int); \
    void p##D3D_FillSelectedVBOWithSprite(tD3dSpriteData*); \
    void p##D3D_Request_SpriteDraw(void); \
    int  p##D3D_l_Request(int, void*); \
    void p##Rasters_Update(void); \
    void p##D3D_RastersSetAndDrawAQuad(int, float, float, float, float, int); \
    void p##D3D_DrawARaster(int, char*, float, float, float); \
    void p##D3D_WaterBuffersInit(void); \
    int  p##D3D_FillWaterStreamData(struct sWaterParamsGlobalStruct*); \
    void p##D3D_DrawWater_DrawVertexTable(struct sWaterParamsGlobalStruct*); \
    void p##MAT_ComputeUV_sub_4521A0(tGEOGRO*, tGeoTriangleChunks*, tMultiTextureElemStruct*); \
    void p##MAT_UV_Compute_Chrome(tGdi_DisplayData*, tGEOGRO*, tGeoTriangleChunks*, unsigned int); \
    void p##MAT_UV_Compute_PLANAR_GIZMO_NOSTORE(tGdi_DisplayData*, tGEOGRO*, tGeoTriangleChunks*, unsigned int); \
    int  p##D3D_DrawSprites(struct sGeoSprites*, void*, int); \
    int  p##D3D_l_DrawElementIndexedTriangles(struct sGeoTriangleChunks*, Vec3*, struct sGeoUVElem*, int); \
    void p##D3D_CreateIndexAndVertexBuffForElemsIfSuitable(tGameObjectMain*, tGEOGRO*); \
    void p##PC_FreeABackCache(struct sGEOGRO*); \
    void p##D3D_ReleaseMeshSpecificData(tGameObjectMain*); \
    void p##D3D_ReleaseMeshSpecificDataOfAWorld(tWorld*); \
    void p##D3D_ReleaseEveryCache(void); \
    void p##D3D_CreateGameObjectsD3DStuffForMultiStream(struct sWorld*); \
    void p##D3D_ReleaseTextures(struct sGdi_DisplayData*); \
    void p##D3D_ReleaseShadowTextures(void); \
    void p##D3D_ReleaseVBOs(void); \
    void p##D3D_ReleaseEverything(void); \
    void p##D3D_CreateVBOs(void); \
    void p##D3D_CreateAndCopySourceTexturesToThem(void); \
    int  p##D3D_l_Texture_Store(struct sGdi_DisplayData*, int); \
    void p##D3D_Palette_Load(struct sGdi_DisplayData*, struct sEngTexListElemStruct*, struct sEngTexIntermediateStruct*, int); \
    void p##D3D_TextureStore(tTexUploadRequest*); \
    void p##D3D_GetSnapShot(tD3dRequestSnapshot*); \
    int  p##PC_WNDMsgHandler(HWND, int, int, int, int*); \
    int  p##PC_HandleWindowCreateAndEraseBackgroundMessages(HWND, GLOBAL_MainHandles*, int, int, int, int*); \
    void p##PC_SetUVMatrix(Mtx4x4*, Vec3*, Vec3*, Vec3*, Vec3*, Vec3*, int, int); \
    extern tWorld* p##worldPtrIfMultiStream; \
    extern D3DMATRIX p##d3dProjectionMtx; \
    extern D3DMATRIX p##d3dTransformMatrix; \
    extern D3DMATRIX p##d3dTransformMatrix2; \
    extern int p##D3D_gi_Widescreen; \
    extern int p##D3D_gi_FlatLayerBox; \
    extern int p##D3D_gi_AspectTrace; \
    extern float p##D3D_gf_RefYoverX; \
    extern float p##D3D_gf_CutsceneBand;

DECL(BASE_)
DECL(NEW_)

typedef struct sSide
{
    const char* name;
    void (*SetInitialRenderStates)(void);
    void (*SetInitialSamplerStates)(void);
    void (*SetInitialTextureStageStates)(void);
    void (*RenderStateRestore)(tD3dRenderStateConfigStruct*);
    void (*SamplerStateRestore)(tD3dSamplerConfigStruct*);
    void (*TextureStageStateRestore)(tD3dTextureStageTypeStruct*);
    void (*SetTexture)(int, int);
    void (*SetVertexShader)(IDirect3DVertexShader9*);
    void (*SaveEverythingToBackup)(int);
    void (*RestoreEverythingFromBackup)(void);
    void (*SetColorWriteEnable)(char);
    void (*SetCullingParams)(void);
    void (*SetTextureStageStates)(int, int);
    void (*SetAlphaTestParams)(unsigned int, int16_t);
    void (*SetAlphaBlendingOptions)(unsigned int, char);
    void (*SetFogParams)(unsigned int);
    void (*TextureInit)(int, int, char);
    void (*RequestSetFogParams)(tFogParams*);
    int (*CalculateStride)(unsigned int);
    int (*SelectSupportedDeviceFormat)(int);
    void (*ApplyPresets)(void);
    void (*SetViewPort)(int, int, int, int);
    void (*CalcAndSetProjection)(float, float, float, float, float, float);
    void (*SetProjectionMatrix)(tDDCamStruct*);
    float (*RealYoverX)(void);
    int (*FlatLayerWidth)(int, int);
    int (*FlatLayerOffsetX)(int, int);
    float (*FlatLayerToLayoutX)(float, int, int);
    float (*FlatLayerFromLayoutX)(float, int, int);
    void (*Clear)(int, int);
    void (*SetRenderTargetSDW)(int, int);
    int (*SetViewMatrix)(Mtx4x4*);
    void (*SetViewMatrixSDW)(Mtx4x4*, float*);
    void (*SetTransformAndSaveMatrix)(IDirect3DTexture9*, int);
    int (*WaterParamsOn)(void);
    void (*WaterParamsOff)(void);
    void (*PrepareWaterRendering)(void);
    tVertex_type_142_generic* (*LockCurrentVBO)(int);
    void (*FillCurrentVBO)(int);
    void (*PrepareForDrawing)(int);
    void (*FillSelectedVBOWithSprite)(tD3dSpriteData*);
    void (*SpriteDraw)(void);
    int (*Request)(int, void*);
    void (*RastersUpdate)(void);
    void (*RastersQuad)(int, float, float, float, float, int);
    void (*DrawARaster)(int, char*, float, float, float);
    void (*WaterBuffersInit)(void);
    int (*FillWaterStreamData)(struct sWaterParamsGlobalStruct*);
    void (*DrawWater)(struct sWaterParamsGlobalStruct*);
    void (*ComputeUV)(tGEOGRO*, tGeoTriangleChunks*, tMultiTextureElemStruct*);
    void (*UvChrome)(tGdi_DisplayData*, tGEOGRO*, tGeoTriangleChunks*, unsigned int);
    void (*UvPlanar)(tGdi_DisplayData*, tGEOGRO*, tGeoTriangleChunks*, unsigned int);
    int (*DrawSprites)(struct sGeoSprites*, void*, int);
    int (*DrawElementIndexedTriangles)(struct sGeoTriangleChunks*, Vec3*, struct sGeoUVElem*, int);
    void (*CreateBuffers)(tGameObjectMain*, tGEOGRO*);
    void (*FreeBackCache)(struct sGEOGRO*);
    void (*ReleaseMeshData)(tGameObjectMain*);
    void (*ReleaseMeshDataOfWorld)(tWorld*);
    void (*ReleaseEveryCache)(void);
    void (*CreateGameObjectsD3DStuff)(struct sWorld*);
    void (*ReleaseTextures)(struct sGdi_DisplayData*);
    void (*ReleaseShadowTextures)(void);
    void (*ReleaseVBOs)(void);
    void (*ReleaseEverything)(void);
    void (*CreateVBOs)(void);
    void (*CreateAndCopySource)(void);
    int (*TextureStoreInit)(struct sGdi_DisplayData*, int);
    void (*PaletteLoad)(struct sGdi_DisplayData*, struct sEngTexListElemStruct*, struct sEngTexIntermediateStruct*, int);
    void (*TextureStore)(tTexUploadRequest*);
    void (*GetSnapShot)(tD3dRequestSnapshot*);
    int (*WndMsg)(HWND, int, int, int, int*);
    int (*WndCreate)(HWND, GLOBAL_MainHandles*, int, int, int, int*);
    void (*SetUVMatrix)(Mtx4x4*, Vec3*, Vec3*, Vec3*, Vec3*, Vec3*, int, int);
    tWorld** multiStreamWorld;
    D3DMATRIX* projMtx;
    D3DMATRIX* xformMtx;
    D3DMATRIX* xformMtx2;
    int* wide;
    int* flatBox;
    int* aspectTrace;
    float* refYoverX;
    float* band;
} tSide;

#define SIDE(nm, p) { nm, \
    p##D3D_SetInitialRenderStates, p##D3D_SetInitialSamplerStates, \
    p##D3D_SetInitialTextureStageStates, p##D3D_RenderStateRestore, \
    p##D3D_SamplerStateRestore, p##D3D_TextureStageStateRestore, \
    p##D3D_SetTexture, p##D3D_SetVertexShader, p##D3D_SaveEverythingToBackup, \
    p##D3D_RestoreEverythingFromBackup, p##D3D_SetColorWriteEnable, \
    p##D3D_SetCullingParams, p##D3D_SetTextureStageStates, \
    p##D3D_SetAlphaTestParams, p##D3D_SetAlphaBlendingOptions, \
    p##D3D_SetFogParams, p##D3D_l_Texture_Init, p##D3D_Request_SetFogParams, \
    p##D3D_CalculateStride, p##D3D_SelectSupportedDeviceFormat, \
    p##D3D_ApplyRenderingPreesetsForVariousGraphicsCards, p##D3D_SetViewPort, \
    p##D3D_CalcAndSetProjection, p##D3D_SetProjectionMatrix, p##D3D_RealYoverX, \
    p##D3D_FlatLayerWidth, p##D3D_FlatLayerOffsetX, p##D3D_FlatLayerToLayoutX, \
    p##D3D_FlatLayerFromLayoutX, p##D3D_Clear, \
    p##D3D_SetRenderTargetAndBackupPreviousSDW, p##D3D_SetViewMatrix, \
    p##D3D_SetViewMatrix_SDW, p##D3D_SetTransformAndSaveMatrix, \
    p##D3D_setWaterRenderParamsIfWaterRenderingIsActive, \
    p##D3D_RestoreRenderParamsAfterWaterRendering, p##D3D_PrepareWaterRendering, \
    p##D3D_LockCurrentVBO, p##D3D_FillCurrentVBO, p##D3D_Request_PrepareForDrawing, \
    p##D3D_FillSelectedVBOWithSprite, p##D3D_Request_SpriteDraw, p##D3D_l_Request, \
    p##Rasters_Update, p##D3D_RastersSetAndDrawAQuad, p##D3D_DrawARaster, \
    p##D3D_WaterBuffersInit, p##D3D_FillWaterStreamData, \
    p##D3D_DrawWater_DrawVertexTable, p##MAT_ComputeUV_sub_4521A0, \
    p##MAT_UV_Compute_Chrome, p##MAT_UV_Compute_PLANAR_GIZMO_NOSTORE, \
    p##D3D_DrawSprites, p##D3D_l_DrawElementIndexedTriangles, \
    p##D3D_CreateIndexAndVertexBuffForElemsIfSuitable, p##PC_FreeABackCache, \
    p##D3D_ReleaseMeshSpecificData, p##D3D_ReleaseMeshSpecificDataOfAWorld, \
    p##D3D_ReleaseEveryCache, p##D3D_CreateGameObjectsD3DStuffForMultiStream, \
    p##D3D_ReleaseTextures, p##D3D_ReleaseShadowTextures, p##D3D_ReleaseVBOs, \
    p##D3D_ReleaseEverything, p##D3D_CreateVBOs, p##D3D_CreateAndCopySourceTexturesToThem, \
    p##D3D_l_Texture_Store, p##D3D_Palette_Load, p##D3D_TextureStore, \
    p##D3D_GetSnapShot, p##PC_WNDMsgHandler, \
    p##PC_HandleWindowCreateAndEraseBackgroundMessages, p##PC_SetUVMatrix, \
    &p##worldPtrIfMultiStream, \
    &p##d3dProjectionMtx, &p##d3dTransformMatrix, &p##d3dTransformMatrix2, \
    &p##D3D_gi_Widescreen, &p##D3D_gi_FlatLayerBox, &p##D3D_gi_AspectTrace, \
    &p##D3D_gf_RefYoverX, &p##D3D_gf_CutsceneBand }

static tSide sides[2] = { SIDE("base", BASE_), SIDE("new", NEW_) };

/* shared tentative definitions the renderer carries between frames */
extern long long qword_990B48;
extern float TIM_SpentTimeWithHandlingWinMessages;

/* ---------- the fixture ---------- */

#define MAXV 64
#define MAXTRI 16
#define MAXCHUNK 3
#define MAXSTRIP 4
#define MAXSTRIPELEM 6
#define RASTERS 54
#define WTR_W 6
#define WTR_H 5

typedef struct sWgFixture
{
    tGdi_DisplayData dd;
    tBigBufferStruct bb;
    profilerRasterStructElem rasters[RASTERS];
    tGEOGRO geogro;
    tGroIntf intf;
    tGeoTriangleChunks chunks[MAXCHUNK];
    tGeoTriangleChunkElem tris[MAXCHUNK][MAXTRI];
    tGeoWholeTriangleStripBlock blocks[MAXCHUNK];
    tGeoTriangleStrip strips[MAXCHUNK][MAXSTRIP];
    tGeoTriangleStripElem stripElems[MAXCHUNK][MAXSTRIP][MAXSTRIPELEM];
    Vec3 verts[MAXV];
    Vec3 normals[MAXV];
    tGeoUVElem uvs[MAXV];
    int colorsA[MAXV];
    int colorsB[MAXV];
    tGeoSprites sprites;
    tGeoSpriteElem spriteElems[8];
    Vec3 spritePos[16];
    tSpriteC0DEData c0de[16];
    tGameObjectMain gao;
    tGameObjectBasic basic;
    tGameObjectVisualObject visual;
    tGameObjectBV bv;
    Mtx4x4 gaoMtx;
    Mtx4x4 uvViewMtx;
    tMultiTextureElemStruct texElem;
    tFogParams fog;
    tWaterParamsGlobalStruct water;
    Vec3 waterVerts[WTR_W * WTR_H];
    Vec3 waterUvVecs[WTR_W * WTR_H];
    int waterColors[WTR_W * WTR_H];
    tD3dSpriteData spriteData;
    tD3dRequestSnapshot snap;
    char snapData[4096];
    tTexUploadRequest upload;
    unsigned int uploadData[256];
    tEngTexListElemStruct texList[8];
    tEngTexIntermediateStruct inter;
    unsigned char imgData[64 * 64 * 4];
    GLOBAL_MainHandles handles;
    tWorld world;
    tableElemStructPF worldElems[4];
    tableElemStructPF boneElems[3];
    tGroup skeleton;
    tableDescStruct boneTable;
    tGameObjectMain gaos[4];
    tGameObjectBasic basics[4];
    tGameObjectActorObject actors[4];
    tGEOSTATICLOD lod;
    tGEOSTATICLOD otherLod;
    tGroIntf lodIntf;
    tGroIntf otherIntf;
    tD3dRenderStateConfigStruct rsIn;
    tD3dSamplerConfigStruct smIn[8];
    tD3dTextureStageTypeStruct tsIn[8];
    IDirect3DTexture9* texPtrs[8];
    IDirect3DTexture9* srcPtrs[8];
    IDirect3DVertexBuffer9* uvvbs[4];
    Mtx4x4 viewMtx;
    Vec3 uvRow[5];
    float unkVals[4];
} tWgFixture;

static tWgFixture W;

static unsigned int rnd(unsigned int* s)
{
    *s = *s * 1103515245u + 12345u;
    return (*s >> 8) & 0xFFFFFF;
}

static float rf(unsigned int* s, float lo, float hi)
{
    return lo + (hi - lo) * (float)(rnd(s) % 1000) * 0.001f;
}

/* Everything the two sides share, snapshotted after each run. */
typedef struct sShot
{
    tGdi_D3DRenderParams rp;
    tD3dRenderStateConfigStruct rs;
    tD3dSamplerConfigStruct sm[8];
    tD3dTextureStageTypeStruct ts[8];
    tD3dRenderStateConfigStruct rsBak;
    tD3dSamplerConfigStruct smBak[8];
    tD3dTextureStageTypeStruct tsBak[8];
    int saveType;
    Mtx4x4WithoutAdditionalData xform[8];
    D3DMATRIX xformBak, xform2Bak;
    D3DMATRIX proj, view, world;
    int streamOffset, streamSize, currentVBO, alphaCase3, clipSet, waterOffset;
    int reqCount, reqCount2, cullBak;
    int wtrA, wtrB, wtrPrim;
    Mtx4x4 uvGlobal;
    Mtx4x4 camMtx;
    double rasterTimes[RASTERS];
    profilerRasterStructElem rasters[RASTERS];
    tGdi_DisplayData dd;
    tBigBufferStruct bbHead;      /* only the head: the data arrays are huge */
    tGEOGRO geogro;
    tGeoWholeTriangleStripBlock blocks[MAXCHUNK];
    tGeoTriangleStrip strips[MAXCHUNK][MAXSTRIP];
    unsigned long long lockHash;
    int isSingleStream, isDLBF, argb;
    int ret;
    float fret;
    int wide, flatBox, trace;
    float refYoverX, band;
} tShot;

static tShot shots[2];

static void Snapshot(const tSide* s, tShot* o, int ret, float fret)
{
    memset(o, 0, sizeof(*o));
    o->rp = D3DRenderParams;
    o->rs = renderStateConfig;
    memcpy(o->sm, samplerConfig, sizeof(o->sm));
    memcpy(o->ts, textureStageTypeConfig, sizeof(o->ts));
    o->rsBak = renderstateBackup;
    memcpy(o->smBak, samplerStateBackup, sizeof(o->smBak));
    memcpy(o->tsBak, texturestageBackup, sizeof(o->tsBak));
    o->saveType = d3dparamsSaveType;
    memcpy(o->xform, d3dTransformationMatrix, sizeof(o->xform));
    o->xformBak = d3dTransformMatrixBackup;
    o->xform2Bak = d3dTransformMatrix2Backup;
    o->proj = *s->projMtx;
    o->view = *s->xformMtx;
    o->world = *s->xformMtx2;
    o->streamOffset = d3dStreamCurrentOffset;
    o->streamSize = d3dStreamActDataSize;
    o->currentVBO = currentVBOSelected;
    o->alphaCase3 = d3d_alphablendIsCase3;
    o->clipSet = water_ClippingHasBeenSet;
    o->waterOffset = waterOffsetInBytes;
    o->reqCount = d3dRequestPrimitiveCount;
    o->reqCount2 = d3dRequestPrimitiveCount2;
    o->cullBak = d3dCullModeBackup;
    o->wtrA = wtr_NumA;
    o->wtrB = wtr_NumB;
    o->wtrPrim = wtr_primitiveCount;
    o->uvGlobal = GLOBAL_UVTransformMatrix;
    o->camMtx = d3dCameraMtx;
    memcpy(o->rasterTimes, rastersUnkTimeFloats_dbl_990B58, sizeof(o->rasterTimes));
    memcpy(o->rasters, W.rasters, sizeof(o->rasters));
    o->dd = W.dd;
    o->dd.Request = NULL;   /* the one side-dependent pointer */
    memcpy(&o->bbHead, &W.bb, sizeof(int) * 4);
    o->geogro = W.geogro;
    memcpy(o->blocks, W.blocks, sizeof(o->blocks));
    memcpy(o->strips, W.strips, sizeof(o->strips));
    o->lockHash = WGM_HashState();
    o->isSingleStream = ENG_IsSingleStream;
    o->isDLBF = ENG_IsDLBFEnabled;
    o->argb = D3DRenderParams.argbIsSupportedForTextures;
    o->ret = ret;
    o->fret = fret;
    o->wide = *s->wide;
    o->flatBox = *s->flatBox;
    o->trace = *s->aspectTrace;
    o->refYoverX = *s->refYoverX;
    o->band = *s->band;
}

/* ---------- guard-distribution bookkeeping ---------- */

#define GUARDS 30
static int guardHit[GUARDS][2];
static const char* guardName[GUARDS];

static void Guard(int id, const char* name, int value)
{
    guardName[id] = name;
    guardHit[id][value ? 1 : 0]++;
}

/* ---------- the cases ---------- */

enum {
    C_INITSTATES, C_RESTORE, C_SETTEXTURE, C_SAVERESTORE, C_COLORWRITE,
    C_CULLING, C_STAGESTATES, C_ALPHATEST, C_ALPHABLEND, C_FOG,
    C_TEXTUREINIT, C_REQFOG, C_STRIDE, C_SELECTFORMAT, C_PRESETS,
    C_VIEWPORT, C_PROJECTION, C_ASPECT, C_CLEAR, C_SDW,
    C_VIEWMATRIX, C_XFORMSAVE, C_WATERPARAMS, C_VBO, C_REQUEST,
    C_RASTERS, C_RASTERQUAD, C_WATERBUFFERS, C_WATERSTREAM, C_DRAWWATER,
    C_UVCOMPUTE, C_UVCHROME, C_UVPLANAR, C_SPRITES, C_DRAWTRIS,
    C_CREATEBUFFERS, C_RELEASE, C_RELEASEMESH, C_CREATE, C_PALETTE, C_SNAPSHOT,
    C_WNDMSG, C_SETUVMATRIX,
    C_COUNT
};

static const char* caseName[C_COUNT] = {
    "initial states", "state restore", "set texture", "save/restore",
    "colour write", "culling", "stage states", "alpha test", "alpha blend",
    "fog", "texture init", "request fog", "stride", "select format",
    "card presets", "viewport", "projection", "aspect", "clear", "shadow target",
    "view matrix", "transform save", "water params", "vbo ring", "request",
    "rasters", "raster quad", "water buffers", "water stream", "draw water",
    "uv compute", "uv chrome", "uv planar", "sprites", "draw triangles",
    "create buffers", "release", "release mesh data", "create", "palette load", "snapshot",
    "window message", "uv matrix"
};

static void ResetShared(void)
{
    memset(&D3DRenderParams, 0, sizeof(D3DRenderParams));
    memset(&renderStateConfig, 0, sizeof(renderStateConfig));
    memset(samplerConfig, 0, sizeof(samplerConfig));
    memset(textureStageTypeConfig, 0, sizeof(textureStageTypeConfig));
    memset(&renderstateBackup, 0, sizeof(renderstateBackup));
    memset(samplerStateBackup, 0, sizeof(samplerStateBackup));
    memset(texturestageBackup, 0, sizeof(texturestageBackup));
    memset(d3dTransformationMatrix, 0, sizeof(d3dTransformationMatrix));
    memset(&d3dTransformMatrixBackup, 0, sizeof(d3dTransformMatrixBackup));
    memset(&d3dTransformMatrix2Backup, 0, sizeof(d3dTransformMatrix2Backup));
    memset(VBOs, 0, sizeof(VBOs));
    memset(gD3dSunSurfaces, 0, sizeof(gD3dSunSurfaces));
    memset(rastersUnkTimeFloats_dbl_990B58, 0, sizeof(double) * RASTERS);
    memset(&GLOBAL_UVTransformMatrix, 0, sizeof(GLOBAL_UVTransformMatrix));
    memset(&d3dCameraMtx, 0, sizeof(d3dCameraMtx));
    memset(PerfCounterStruct, 0, sizeof(LARGE_INTEGER) * 54);
    memset(PerfCountertimeSumArray, 0, sizeof(long long) * 54);
    qword_990B48 = 0;
    TIM_SpentTimeWithHandlingWinMessages = 0.0f;
    d3dparamsSaveType = 0;
    d3dStreamCurrentOffset = 0;
    d3dStreamActDataSize = 0;
    currentVBOSelected = 0;
    d3d_alphablendIsCase3 = 0;
    water_ClippingHasBeenSet = 0;
    waterOffsetInBytes = 0;
    d3dRequestPrimitiveCount = 0;
    d3dRequestPrimitiveCount2 = 0;
    d3dCullModeBackup = 0;
    d3d_requestDrawSelectedVBO = NULL;
    d3dxfontIF = NULL;
    rastersDrawPrimitiveUPDataStart = NULL;
    rastersDrawPrimitiveUPDataEnd = NULL;
    waterTexture = NULL;
    waterStreamData = NULL;
    waterIndexData = NULL;
    d3dWaterRTtoSet = NULL;
    d3dDSSurfaceToSet = NULL;
    d3dRTToGet = NULL;
    d3dWaterDSSurfaceToGet = NULL;
    wtr_NumA = wtr_NumB = wtr_primitiveCount = 0;
    ENG_IsSingleStream = 0;
    ENG_IsDLBFEnabled = 0;
    ENG_gb_ExitApplication = 0;
    ENG_gb_ForceEndEngine = 0;
    ENG_gb_EngineRunning = 1;
    ENG_gb_ForcePauseEngine = 0;
    ENG_WIN_AutoPlaySupressorRegistered = 0;
    pst_GlobalsWaterParams = NULL;
    WOR_gpst_CurrentWorld = NULL;
    gMaxSimultaneousTextures = 4;
    GDI_gb_WaveSprite = 0;
    TIM_gul_TicksCount = 1234;
    waterHeight = 2.5f;
    memset(&MAI_gst_MainHandles, 0, sizeof(MAI_gst_MainHandles));
}

static void Setup(const tSide* s, int kind, unsigned int seed)
{
    unsigned int r = seed * 2654435761u + 17u;
    int i, c, t, k;

    memset(&W, 0, sizeof(W));
    ResetShared();
    WGM_Reset();

    *s->wide = (int)(seed & 1);
    *s->flatBox = (int)((seed >> 1) & 1);
    *s->aspectTrace = 0;
    *s->refYoverX = 0.75f;
    *s->band = 0.10416666f;
    Guard(0, "widescreen", (int)(seed & 1));
    Guard(1, "flat layer box", (int)((seed >> 1) & 1));

    /* ---- the device ---- */
    D3DRenderParams.d3dDev = &WGM_dev;
    D3DRenderParams.d3d_if = &WGM_d3d;
    D3DRenderParams.d3dcaps.MaxTextureBlendStages = 2u + (seed % 7u);
    D3DRenderParams.d3dcaps.MaxSimultaneousTextures = 2u + (seed % 4u);
    D3DRenderParams.d3dcaps.TextureOpCaps = rnd(&r);
    D3DRenderParams.d3dcaps.RasterCaps = rnd(&r) & 0x1FF;
    D3DRenderParams.d3dcaps.Caps2 = rnd(&r);
    D3DRenderParams.presentParams.BackBufferWidth = 320u + (seed % 5u) * 160u;
    D3DRenderParams.presentParams.BackBufferHeight = 240u + (seed % 3u) * 120u;
    D3DRenderParams.presentParams.BackBufferFormat = rnd(&r) & 0xFF;
    D3DRenderParams.Rect.left = 0;
    D3DRenderParams.Rect.top = 0;
    D3DRenderParams.Rect.right = (int)(seed % 4u) * 200 + 200;
    D3DRenderParams.Rect.bottom = (int)(seed % 5u) * 100 + 150;
    D3DRenderParams.d3d_defaultZEnable = (int)((seed >> 2) & 1);
    D3DRenderParams.setsDefaultZEnableAtBeginScene = (int)((seed >> 3) & 1);
    D3DRenderParams.textureUVMode = (int)(seed % 6u);
    D3DRenderParams.textureUVIsProjected = (int)((seed >> 4) & 1);
    D3DRenderParams.diffuse2x = (int)((seed >> 5) & 1);
    D3DRenderParams.tweak_ae_d2x = (int)((seed >> 6) & 1);
    D3DRenderParams.Mipmapping = (int)((seed >> 7) & 1);
    D3DRenderParams.argbIsSupportedForTextures = (int)(seed % 3u) - 1;
    D3DRenderParams.d3dFogModeProbably = (int)(seed % 6u);
    D3DRenderParams.d3dFogColor = (int)rnd(&r);
    D3DRenderParams.d3dFogColor2Case5and6 = (int)rnd(&r);
    D3DRenderParams.colorMaskAlpha = (int)(((seed >> 8) & 1) ? 0xFF000000 : 0);
    D3DRenderParams.d3dNeedToCallEndScene = (int)((seed >> 9) & 1);
    D3DRenderParams.renderTargetBackup = ((seed >> 10) & 1) ? &WGM_surf[7] : NULL;
    D3DRenderParams.blurTex = &WGM_tex[20];
    D3DRenderParams.newZStencil = &WGM_surf[9];
    D3DRenderParams.pDestTexture = &WGM_tex[21];
    D3DRenderParams.pSourceTexture = &WGM_tex[22];
    D3DRenderParams.stride = 4;
    D3DRenderParams.stride2 = 12;
    D3DRenderParams.d3dStride = 12;
    D3DRenderParams.d3dXYZVB = &WGM_vb[10];
    D3DRenderParams.d3dNormalsBuffer = &WGM_vb[11];
    D3DRenderParams.d3dColorsVB = &WGM_vb[12];
    D3DRenderParams.vertexDeclarationIFd3dUVVBIsNull = &WGM_decl[1];
    D3DRenderParams.vertexDeclarationIFd3dUVVBISnotNull = &WGM_decl[2];
    D3DRenderParams.d3dVertexDeclaration = &WGM_decl[3];
    for (i = 0; i < 8; i++)
    {
        D3DRenderParams.d3dShadowTextures[i] = &WGM_tex[24 + i];
        W.texPtrs[i] = ((seed >> i) & 1) ? &WGM_tex[i] : NULL;
        W.srcPtrs[i] = ((seed >> (i + 1)) & 1) ? &WGM_tex[8 + i] : NULL;
    }
    D3DRenderParams.d3dTextures = W.texPtrs;
    D3DRenderParams.d3dSourceTextures = W.srcPtrs;
    D3DRenderParams.d3dTextureNum = 4 + (int)(seed % 5u);
    VBOs[0] = &WGM_vb[0];
    VBOs[1] = &WGM_vb[1];
    VBOs[2] = &WGM_vb[2];
    currentVBOSelected = (int)(seed % 3u);
    waterTexture = &WGM_tex[30];
    waterStreamData = ((seed >> 11) & 1) ? &WGM_vb[20] : NULL;
    waterIndexData = ((seed >> 11) & 1) ? &WGM_ib[1] : NULL;
    d3dWaterRTtoSet = &WGM_surf[11];
    d3dDSSurfaceToSet = &WGM_surf[12];
    WGM_vendorId = (seed % 3u) == 0 ? 0x1002u : ((seed % 3u) == 1 ? 4318u : 0x8086u);
    WGM_deviceId = (unsigned int)(seed * 97u) & 0xFFFFu;
    WGM_formatMask = seed * 2654435761u;
    WGM_descWidth = 16u + (seed % 4u) * 16u;
    WGM_descHeight = 8u + (seed % 3u) * 8u;
    WGM_descFormat = rnd(&r) & 0x3F;
    WGM_SetMatFunc((int)(seed % 4u));

    /* ---- the shadow state the setters compare against ---- */
    renderStateConfig.d3d_zEnable = (int)(seed % 3u);
    renderStateConfig.d3d_zFunc = (int)(seed % 5u);
    renderStateConfig.d3d_zWriteEnable = (int)((seed >> 1) & 1);
    renderStateConfig.d3dFogEnable = (int)((seed >> 2) & 1);
    renderStateConfig.d3dFogColor = (int)(seed * 7u);
    renderStateConfig.d3dFogTableMode = (int)(seed % 4u);
    renderStateConfig.d3dFogVertexMode = (int)(seed % 4u);
    renderStateConfig.d3dFogDensity = (float)(seed % 3u);
    renderStateConfig.d3dFogStart = (float)(seed % 5u);
    renderStateConfig.d3dFogEnd = (float)(seed % 7u);
    renderStateConfig.d3dAlphaTest = (int)((seed >> 3) & 1);
    renderStateConfig.d3dAlphaFunc = (int)(seed % 9u);
    renderStateConfig.d3dAlphaRef = (int)(seed % 255u);
    renderStateConfig.d3dAlphaBlendEnable = (int)((seed >> 4) & 1);
    renderStateConfig.d3dSrcBlend = (int)(seed % 8u);
    renderStateConfig.d3dDestBlend = (int)(seed % 6u);
    renderStateConfig.d3d_BlendOp = (int)(seed % 3u);
    renderStateConfig.d3dFillMode = (int)(seed % 4u);
    renderStateConfig.d3d_Lighting = (int)((seed >> 5) & 1);
    renderStateConfig.d3dCullMode = 1 + (int)(seed % 3u);
    renderStateConfig.d3dColorWriteEnable = (int)(seed % 16u);
    renderStateConfig.d3d_normalizeNormals = (int)((seed >> 6) & 1);
    renderStateConfig.d3dTextureFactor = (int)rnd(&r);
    renderStateConfig.d3dClipPlaneEnable = (int)((seed >> 7) & 1);
    renderStateConfig.fvf = (int)(seed % 4u);
    renderStateConfig.vertexShader = ((seed >> 8) & 1) ? (IDirect3DVertexShader9*)&WGM_decl[4] : NULL;
    Guard(2, "shader set", (int)((seed >> 8) & 1));
    for (i = 0; i < 8; i++)
    {
        samplerConfig[i].d3d_addressu = (int)((seed + i) % 4u);
        samplerConfig[i].d3d_addressv = (int)((seed + i * 2u) % 4u);
        samplerConfig[i].d3d_minfilter = (int)((seed + i) % 3u);
        samplerConfig[i].d3d_magfilter = (int)((seed + i * 3u) % 3u);
        samplerConfig[i].d3d_mipfilter = (int)((seed + i) % 2u);
        samplerConfig[i].d3d_mipmaplodbias = (float)((seed + i) % 5u) * 0.25f;
        textureStageTypeConfig[i].d3dTextureID = (int)((seed + i) % 6u) - 2;
        textureStageTypeConfig[i].d3d_colorOp = (int)((seed + i) % 5u);
        textureStageTypeConfig[i].d3d_colorarg1 = (int)((seed + i) % 4u);
        textureStageTypeConfig[i].d3d_colorarg2 = (int)((seed + i * 2u) % 3u);
        textureStageTypeConfig[i].d3d_alphaOp = (int)((seed + i) % 4u);
        textureStageTypeConfig[i].d3d_alphaArg1 = (int)((seed + i) % 3u);
        textureStageTypeConfig[i].d3d_alphaArg2 = (int)((seed + i * 3u) % 3u);
        textureStageTypeConfig[i].d3d_bumpenvmat00 = (float)((seed + i) % 4u);
        textureStageTypeConfig[i].d3d_bumpenvmat01 = (float)((seed + i * 2u) % 4u);
        textureStageTypeConfig[i].d3d_bumpenvmat10 = (float)((seed + i * 3u) % 4u);
        textureStageTypeConfig[i].d3d_bumpenvmat11 = (float)((seed + i * 5u) % 4u);
        textureStageTypeConfig[i].d3dTexCoordIndex = (int)((seed + i) % 5u);
        textureStageTypeConfig[i].d3d_bumpEnvLScale = (float)((seed + i) % 3u);
        textureStageTypeConfig[i].d3d_bumpEnvLOffset = (float)((seed + i * 2u) % 3u);
        textureStageTypeConfig[i].d3dTextureTransformFlags = (int)((seed + i) % 4u);
        textureStageTypeConfig[i].d3d_colorArg0 = (int)((seed + i) % 3u);
        textureStageTypeConfig[i].d3d_alphaArg0 = (int)((seed + i * 2u) % 3u);
        textureStageTypeConfig[i].d3d_resultArg0 = (int)((seed + i) % 2u);
        textureStageTypeConfig[i].d3d_constant = (int)((seed + i) % 3u);
        /* the configuration the restore functions are handed */
        W.smIn[i].d3d_addressu = (int)((seed + i * 7u) % 4u);
        W.smIn[i].d3d_addressv = (int)((seed + i * 11u) % 4u);
        W.smIn[i].d3d_minfilter = (int)((seed + i * 13u) % 3u);
        W.smIn[i].d3d_magfilter = (int)((seed + i * 5u) % 3u);
        W.smIn[i].d3d_mipfilter = (int)((seed + i * 3u) % 2u);
        W.smIn[i].d3d_mipmaplodbias = (float)((seed + i * 3u) % 5u) * 0.25f;
        W.tsIn[i].d3dTextureID = (int)((seed + i * 3u) % 6u) - 2;
        W.tsIn[i].d3d_colorOp = (int)((seed + i * 7u) % 5u);
        W.tsIn[i].d3d_colorarg1 = (int)((seed + i * 11u) % 4u);
        W.tsIn[i].d3d_colorarg2 = (int)((seed + i * 13u) % 3u);
        W.tsIn[i].d3d_alphaOp = (int)((seed + i * 5u) % 4u);
        W.tsIn[i].d3d_alphaArg1 = (int)((seed + i * 3u) % 3u);
        W.tsIn[i].d3d_alphaArg2 = (int)((seed + i * 17u) % 3u);
        W.tsIn[i].d3d_bumpenvmat00 = (float)((seed + i * 3u) % 4u);
        W.tsIn[i].d3d_bumpenvmat01 = (float)((seed + i * 5u) % 4u);
        W.tsIn[i].d3d_bumpenvmat10 = (float)((seed + i * 7u) % 4u);
        W.tsIn[i].d3d_bumpenvmat11 = (float)((seed + i * 11u) % 4u);
        W.tsIn[i].d3dTexCoordIndex = (int)((seed + i * 3u) % 5u);
        W.tsIn[i].d3d_bumpEnvLScale = (float)((seed + i * 5u) % 3u);
        W.tsIn[i].d3d_bumpEnvLOffset = (float)((seed + i * 7u) % 3u);
        W.tsIn[i].d3dTextureTransformFlags = (int)((seed + i * 3u) % 4u);
        W.tsIn[i].d3d_colorArg0 = (int)((seed + i * 5u) % 3u);
        W.tsIn[i].d3d_alphaArg0 = (int)((seed + i * 7u) % 3u);
        W.tsIn[i].d3d_resultArg0 = (int)((seed + i * 3u) % 2u);
        W.tsIn[i].d3d_constant = (int)((seed + i * 5u) % 3u);
    }
    W.rsIn = renderStateConfig;
    W.rsIn.d3d_zEnable = (int)((seed * 3u) % 3u);
    W.rsIn.d3d_zFunc = (int)((seed * 5u) % 5u);
    W.rsIn.d3dFogColor = (int)(seed * 11u);
    W.rsIn.d3dFogDensity = (float)((seed * 3u) % 3u);
    W.rsIn.d3dCullMode = 1 + (int)((seed * 7u) % 3u);
    W.rsIn.fvf = (int)((seed * 3u) % 4u);
    W.rsIn.vertexShader = ((seed >> 9) & 1) ? (IDirect3DVertexShader9*)&WGM_decl[5] : NULL;
    Guard(3, "restore shader", (int)((seed >> 9) & 1));

    /* ---- geometry ---- */
    for (i = 0; i < MAXV; i++)
    {
        W.verts[i].x = rf(&r, -8.0f, 8.0f);
        W.verts[i].y = rf(&r, -8.0f, 8.0f);
        W.verts[i].z = rf(&r, -8.0f, 8.0f);
        W.normals[i].x = rf(&r, -1.0f, 1.0f);
        W.normals[i].y = rf(&r, -1.0f, 1.0f);
        W.normals[i].z = rf(&r, -1.0f, 1.0f);
        W.uvs[i].u = rf(&r, 0.0f, 1.0f);
        W.uvs[i].v = rf(&r, 0.0f, 1.0f);
        W.colorsA[i] = (int)rnd(&r);
        W.colorsB[i] = (int)rnd(&r);
    }
    for (c = 0; c < MAXCHUNK; c++)
    {
        W.chunks[c].triangleNum = (unsigned int)(1 + (seed + (unsigned)c) % MAXTRI);
        W.chunks[c].materialNum = c;
        W.chunks[c].triangleChunkElems = W.tris[c];
        for (t = 0; t < MAXTRI; t++)
        {
            W.tris[c][t].v1 = (uint16_t)((seed + c * 3u + t * 5u) % MAXV);
            W.tris[c][t].v2 = (uint16_t)((seed + c * 7u + t * 11u) % MAXV);
            W.tris[c][t].v3 = (uint16_t)((seed + c * 13u + t * 3u) % MAXV);
            W.tris[c][t].uv1 = (uint16_t)((seed + t) % MAXV);
            W.tris[c][t].uv2 = (uint16_t)((seed + t * 2u) % MAXV);
            W.tris[c][t].uv3 = (uint16_t)((seed + t * 4u) % MAXV);
        }
        W.blocks[c].triangleStripNum = (unsigned int)(1 + (seed + (unsigned)c) % MAXSTRIP);
        W.blocks[c].triangleStrips = W.strips[c];
        W.blocks[c].streamsNum = 0;
        for (k = 0; k < MAXSTRIP; k++)
        {
            W.strips[c][k].triangleStripElemNum = 1 + (int)((seed + (unsigned)k) % MAXSTRIPELEM);
            W.strips[c][k].triangleStripElems = W.stripElems[c][k];
            W.strips[c][k].vbIdx = 0;
            for (i = 0; i < MAXSTRIPELEM; i++)
            {
                W.stripElems[c][k][i].triangleIdx =
                    (uint16_t)((seed + (unsigned)(c * 5 + k * 3 + i)) % 16u);
                W.stripElems[c][k][i].uvIdx =
                    (uint16_t)((seed + (unsigned)(c * 7 + k * 5 + i * 3)) % MAXV);
            }
        }
        W.chunks[c].triangleStripBlock = ((seed >> (unsigned)c) & 1) ? &W.blocks[c] : NULL;
    }
    Guard(4, "strip block", (int)(seed & 1));
    W.intf.id = 1;
    W.geogro.intf = &W.intf;
    W.geogro.vertexNum = 16u + (seed % 8u);
    W.geogro.vertices = W.verts;
    W.geogro.normals = W.normals;
    W.geogro.uvNum = ((seed >> 13) & 1) ? 16u : 0u;
    W.geogro.UVs = W.uvs;
    W.geogro.triangleChunkCount = (unsigned int)MAXCHUNK;
    W.geogro.triangleChunks = W.chunks;
    W.geogro.d3dvb = ((seed >> 14) & 1) ? &WGM_vb[30] : NULL;
    W.geogro.d3dvb2 = ((seed >> 15) & 1) ? &WGM_vb[31] : NULL;
    W.geogro.d3dUVVB = ((seed >> 16) & 1) ? W.uvvbs : NULL;
    W.geogro.d3dUVVBNum = ((seed >> 16) & 1) ? 2 : 0;
    W.uvvbs[0] = &WGM_vb[32];
    W.uvvbs[1] = &WGM_vb[33];
    W.geogro.d3dIndexBuf = ((seed >> 17) & 1) ? &WGM_ib[2] : NULL;
    Guard(5, "uv count", (int)((seed >> 13) & 1));
    Guard(6, "uv vertex buffers", (int)((seed >> 16) & 1));
    Guard(7, "index buffer", (int)((seed >> 17) & 1));

    /* ---- display data ---- */
    W.bb.flags = (int)(seed & 7u);
    W.bb.solidVertexColor = (int)rnd(&r);
    W.bb.colorMask = (int)rnd(&r);
    W.bb.vertexColorArray = ((seed >> 18) & 1) ? W.colorsA : NULL;
    W.bb.vertexColorArraySecond = ((seed >> 19) & 1) ? W.colorsB : NULL;
    W.bb.UVData = ((seed >> 20) & 1) ? W.bb.data7 : NULL;
    Guard(8, "vertex colours", (int)((seed >> 18) & 1));
    Guard(9, "second colours", (int)((seed >> 19) & 1));
    Guard(10, "uv data", (int)((seed >> 20) & 1));
    W.dd.bigBuffer = &W.bb;
    W.dd.rasters = W.rasters;
    W.dd.verticesCurrent = W.verts;
    W.dd.drawMaskAllowedStuffCurrent = (int)rnd(&r);
    W.dd.flags = (int)(seed & 0x7FFu);
    W.dd.GROGEO = ((seed >> 21) & 1) ? &W.geogro : NULL;
    W.dd.currTargetGao = &W.gao;
    W.dd.matrixStack.actMatrixElem = &W.gaoMtx;
    W.dd.pd3drp = &D3DRenderParams;
    W.dd.Request = s->Request;
    W.dd.texManager.paletteUsedMemory = 0;
    Guard(11, "geogro attached", (int)((seed >> 21) & 1));
    for (i = 0; i < RASTERS; i++)
    {
        W.rasters[i].insideCounter = (int16_t)((seed + (unsigned)i) % 2u);
        W.rasters[i].startTick = (int)((seed + (unsigned)i) * 3u);
        W.rasters[i].sumTime = (int)((seed + (unsigned)i) * 5u);
        W.rasters[i].unk1 = (int)((seed + (unsigned)i) % 7u);
    }
    gpst_DX_stDD = &W.dd;

    /* ---- camera ---- */
    W.dd.ct.cam.flags = (int)(seed & 0x401u);
    W.dd.ct.cam.nearPlane = rf(&r, 0.1f, 2.0f);
    W.dd.ct.cam.farPlane = rf(&r, 50.0f, 500.0f);
    W.dd.ct.cam.focale = rf(&r, 0.4f, 1.4f);
    W.dd.ct.cam.YoverX = rf(&r, 0.5f, 1.0f);
    W.dd.ct.cam.factorX = rf(&r, 0.5f, 3.0f);
    W.dd.ct.cam.factorY = rf(&r, 0.5f, 3.0f);
    W.dd.ct.cam.centerX = rf(&r, 1.0f, 4.0f);
    W.dd.ct.cam.centerY = rf(&r, 1.0f, 4.0f);
    W.dd.ct.cam.width = 320.0f + (float)(seed % 4u) * 160.0f;
    W.dd.ct.cam.height = 240.0f + (float)(seed % 3u) * 120.0f;
    W.dd.ct.cam.viewPortSizeWidth = ((seed >> 22) & 1) ? 0.0f : rf(&r, 0.3f, 1.0f);
    W.dd.ct.cam.viewPortSizeHeight = rf(&r, 0.3f, 1.0f);
    W.dd.ct.cam.viewPortPosX = rf(&r, 0.0f, 0.4f);
    W.dd.ct.cam.viewPortPosY = rf(&r, 0.0f, 0.4f);
    W.dd.ct.cam.isoFactor = ((seed >> 23) & 1) ? 0.0f : rf(&r, 0.2f, 2.0f);
    W.dd.ct.cam.isoZoom = rf(&r, 0.5f, 2.0f);
    Guard(12, "viewport unset", (int)((seed >> 22) & 1));
    Guard(13, "iso factor zero", (int)((seed >> 23) & 1));
    Guard(14, "ortho camera", (int)((seed >> 10) & 1));
    W.dd.ct.cam.flags = (int)(((seed >> 10) & 1) ? 0x400 : ((seed & 1) ? 1 : 0));
    W.dd.ddcs2.flags2 = (int)(seed & 7u);
    W.dd.ddcs2.pixelYoverX = rf(&r, 0.8f, 1.2f);
    W.dd.ddcs2.manualScreenRatio = rf(&r, 0.5f, 1.0f);
    W.dd.ddcs2.selectedScreenRatio = (int)(seed % 6u) - 1;
    for (i = 0; i < (int)(sizeof(Mtx4x4) / sizeof(float)); i++)
    {
        ((float*)&W.gaoMtx)[i] = rf(&r, -2.0f, 2.0f);
        ((float*)&W.viewMtx)[i] = rf(&r, -2.0f, 2.0f);
        ((float*)&W.uvViewMtx)[i] = rf(&r, -2.0f, 2.0f);
        ((float*)&W.dd.ct.cam.objectMtx)[i] = rf(&r, -4.0f, 4.0f);
        ((float*)&W.dd.ct.cam.cameraMtx)[i] = rf(&r, -2.0f, 2.0f);
    }
    W.uvViewMtx.mtxflag = (int)(seed & 0xF);
    W.dd.ct.cam.viewMtxPtr_setInGRORender = &W.uvViewMtx;
    W.gao.matrices = &W.gaoMtx;
    W.gao.basic = &W.basic;
    W.gao.flags = (int)(seed & 0x5204002u);
    W.gao.boundingVolume = &W.bv;
    W.basic.actorObjectOrVisualObject = (struct sGameObjectVisualObject*)&W.visual;
    W.visual.GROGEOOrSomethingElse = (struct sGroGeneric*)&W.geogro;
    W.bv.BVMin.x = rf(&r, -5.0f, 0.0f);
    W.bv.BVMin.y = rf(&r, -5.0f, 0.0f);
    W.bv.BVMax.x = rf(&r, 0.0f, 9.0f);
    W.bv.BVMax.y = rf(&r, 0.0f, 9.0f);

    /* ---- material ---- */
    W.texElem.phongOrPlanarParam = (int)(seed % 9u);
    W.texElem.subTextureFlags = (unsigned int)((seed % 16u) << 20) | (seed & 0xFFFFu);
    W.texElem.transform1 = ((seed >> 24) & 1) ? 0x3F800000 : (int)rnd(&r);
    W.texElem.transform2 = ((seed >> 25) & 1) ? 0x3F800000 : (int)rnd(&r);
    Guard(15, "uv transform identity", (int)((seed >> 24) & 1));

    /* ---- sprites ---- */
    W.sprites.num = 1u + (seed % 6u);
    W.sprites.spriteElems = W.spriteElems;
    W.sprites.globalSize = rf(&r, 0.2f, 3.0f);
    W.sprites.globalRatio = rf(&r, 0.2f, 3.0f);
    for (i = 0; i < 8; i++)
    {
        W.spriteElems[i].idx = (uint16_t)((seed + (unsigned)i) % 12u);
        W.spriteElems[i].size = rf(&r, 0.1f, 2.0f);
        W.spritePos[i].x = rf(&r, -5.0f, 5.0f);
        W.spritePos[i].y = rf(&r, -5.0f, 5.0f);
        W.spritePos[i].z = rf(&r, -5.0f, 5.0f);
        W.c0de[i].f1 = rf(&r, 0.1f, 2.0f);
        W.c0de[i].v1.x = rf(&r, -5.0f, 5.0f);
        W.c0de[i].v1.y = rf(&r, -5.0f, 5.0f);
        W.c0de[i].v1.z = rf(&r, -5.0f, 5.0f);
    }
    for (i = 8; i < 16; i++)
    {
        W.spritePos[i].x = rf(&r, -5.0f, 5.0f);
        W.c0de[i].f1 = rf(&r, 0.1f, 2.0f);
    }
    if ((seed >> 26) & 1)
    {
        W.spriteElems[0].idx = 0xC0DE;
        W.spriteElems[1].idx = (uint16_t)(seed % 4u);
    }
    Guard(16, "c0de sprites", (int)((seed >> 26) & 1));
    GDI_gb_WaveSprite = (int)((seed >> 27) & 1);
    Guard(17, "wave sprite", (int)((seed >> 27) & 1));

    /* ---- water ---- */
    W.water.horizontalVertexNum = WTR_W;
    W.water.verticallVertexNum = WTR_H;
    W.water.vertices = W.waterVerts;
    W.water.vecsForUVs = W.waterUvVecs;
    W.water.colors = (unsigned int*)W.waterColors;
    W.water.waterEngine.bottomColor = (int)rnd(&r);
    for (i = 0; i < WTR_W * WTR_H; i++)
    {
        W.waterVerts[i].x = rf(&r, -9.0f, 9.0f);
        W.waterVerts[i].y = rf(&r, -9.0f, 9.0f);
        W.waterVerts[i].z = rf(&r, -9.0f, 9.0f);
        W.waterUvVecs[i].x = rf(&r, -1.0f, 1.0f);
        W.waterUvVecs[i].y = rf(&r, -1.0f, 1.0f);
        W.waterColors[i] = (int)rnd(&r);
    }
    tweak_water_blur_normal = (int)((seed >> 28) & 1);
    tweak_water_clip = (int)((seed >> 29) & 1);
    tweak_water_smooth = (int)((seed >> 30) & 1);
    tweak_water_min_bv = rf(&r, 0.0f, 4.0f);
    tweak_water_off = rf(&r, -1.0f, 1.0f);
    tweak_mipmaplodbias = (float)(seed % 5u) * 0.25f;
    Guard(18, "water clip", (int)((seed >> 29) & 1));
    pst_GlobalsWaterParams = ((seed >> 31) & 1) ? &W.water : NULL;
    Guard(19, "water params", (int)((seed >> 31) & 1));

    /* ---- rasters ---- */
    tweak_perf = (int)(seed % 4u);
    tweak_perf_fps_min = rf(&r, 10.0f, 60.0f);
    tweak_perf_fps_target = rf(&r, 1000.0f, 90000.0f);
    for (i = 0; i < RASTERS; i++)
    {
        PerfCountertimeSumArray[i] = (long long)((seed + (unsigned)i * 37u) % 4096u);
        PerfCounterStruct[i].QuadPart = (long long)((seed + (unsigned)i) % 3u);
        rastersUnkTimeFloats_dbl_990B58[i] = (double)((seed * 7u + (unsigned)i * 13u) % 997u);
    }
    Guard(20, "perf raster mode", tweak_perf != 0);
    Guard(21, "perf detail", tweak_perf == 1);

    /* ---- misc ---- */
    W.fog.flags = (char)(seed & 3u);
    W.fog.fogVertexMode = (char)(seed % 4u);
    W.fog.fogStart = rf(&r, 0.0f, 10.0f);
    W.fog.fogEnd = rf(&r, 10.0f, 100.0f);
    W.fog.fogColor = (int)rnd(&r);
    W.fog.density = rf(&r, 0.0f, 1.0f);
    W.spriteData.color = (int)rnd(&r);
    for (i = 0; i < 4; i++)
    {
        ((Vec3*)&W.spriteData)[i].x = rf(&r, -3.0f, 3.0f);
        ((Vec3*)&W.spriteData)[i].y = rf(&r, -3.0f, 3.0f);
        ((Vec3*)&W.spriteData)[i].z = rf(&r, -3.0f, 3.0f);
    }
    W.snap.flags = (int16_t)(seed & 0x303u);
    W.snap.width = 4 + (int)(seed % 5u);
    W.snap.height = 2 + (int)(seed % 3u);
    W.snap.cutSizeFromTopAndBottom = (int)(seed % 3u);
    W.snap.cutSizeFromSides = (int)(seed % 2u);
    W.snap.srcData = W.snapData;
    Guard(22, "snapshot taken", (W.snap.flags & 0xFF) == 0 && ((W.snap.flags >> 8) & 0xFF) != 2);
    W.upload.idx = (uint16_t)(seed % 4u);
    W.upload.dataPtr = W.uploadData;
    TEX_gst_GlobalList.texElems = W.texList;
    W.texList[W.upload.idx].normalData.width = (int16_t)(4 + seed % 5u);
    W.texList[W.upload.idx].normalData.height = (int16_t)(2 + seed % 3u);
    W.inter.colorDepth2 = (seed % 3u) == 0 ? 32 : ((seed % 3u) == 1 ? 24 : 16);
    W.inter.imgDataPtr = (char*)W.imgData;
    W.inter.bmpUnkb1 = (int)(seed & 0x3Fu);
    W.inter.tp.flagsUnkAndProceduralUnkb1 = (int16_t)(seed & 0x241u);
    W.inter.tp.type = (int)(seed % 7u);
    Guard(23, "palette 32 bit", (seed % 3u) == 0);
    for (i = 0; i < (int)sizeof(W.imgData); i++)
        W.imgData[i] = (unsigned char)((seed + (unsigned)i * 31u) & 0xFF);
    /* a small world: four objects, one of them a skeleton owner */
    W.lodIntf.id = 8;
    W.lod.intf = &W.lodIntf;
    W.lod.lodNum = 2;
    W.lod.geo[0] = &W.geogro;
    W.lod.geo[1] = &W.geogro;
    W.otherIntf.id = 3;
    W.otherLod.intf = &W.otherIntf;
    for (i = 0; i < 4; i++)
    {
        W.gaos[i].basic = &W.basics[i];
        W.gaos[i].flags = (int)((seed >> i) & 1) * 0x4000
                        | (int)((seed >> (i + 1)) & 1) * 2;
        W.basics[i].actorObjectOrVisualObject =
            (struct sGameObjectVisualObject*)&W.actors[i];
        /* one of each kind the dispatcher knows, plus a kind it does
           not and a null one */
        switch (((int)(seed >> i) + i) % 4)
        {
        case 0:
            ((tGameObjectVisualObject*)&W.actors[i])->GROGEOOrSomethingElse =
                (struct sGroGeneric*)&W.geogro;
            break;
        case 1:
            ((tGameObjectVisualObject*)&W.actors[i])->GROGEOOrSomethingElse =
                (struct sGroGeneric*)&W.lod;
            break;
        case 2:
            ((tGameObjectVisualObject*)&W.actors[i])->GROGEOOrSomethingElse =
                (struct sGroGeneric*)&W.otherLod;
            break;
        default:
            ((tGameObjectVisualObject*)&W.actors[i])->GROGEOOrSomethingElse = NULL;
            break;
        }

        W.actors[i].skeletonGroup = ((seed >> i) & 1) ? &W.skeleton : NULL;
        W.worldElems[i].objectPtr = ((seed >> (i + 2)) & 1)
            ? (void*)&W.gaos[i] : NULL;
    }
    for (i = 0; i < 4; i++)
    {
        static const char* kindName[4] = { "mesh geogro", "mesh lod",
                                           "mesh other kind", "mesh none" };
        int k;
        for (k = 0; k < 4; k++)
            Guard(26 + k, kindName[k], (((int)(seed >> i) + i) % 4) == k);
    }
    W.worldElems[3].objectPtr = (void*)((size_t)&W.gaos[3] | 1u);
    W.world.objectTablePF.actElemNum = (seed & 1) ? 4u : 0u;
    W.world.objectTablePF.firstElemPtr = W.worldElems;
    W.world.objectTablePF.nextElemPtr = &W.worldElems[4];
    W.skeleton.tablePF = &W.boneTable;
    W.boneTable.firstElemPtr = W.boneElems;
    W.boneTable.nextElemPtr = &W.boneElems[3];
    for (i = 0; i < 3; i++)
        W.boneElems[i].objectPtr = ((seed >> i) & 1) ? (void*)&W.gaos[i] : NULL;
    Guard(24, "world table populated", (int)(seed & 1));
    Guard(25, "lod geometry", (int)(seed & 1) == 0);

    W.handles.hwnd = (HWND)0x100;
    W.unkVals[0] = rf(&r, -1.0f, 1.0f);
    (void)kind;
}

static int RunSide(const tSide* s, int kind, unsigned int seed, float* fret)
{
    int ret = 0;
    int extra = 0;

    *fret = 0.0f;
    switch (kind)
    {
    case C_INITSTATES:
        s->SetInitialRenderStates();
        s->SetInitialSamplerStates();
        s->SetInitialTextureStageStates();
        break;
    case C_RESTORE:
        s->RenderStateRestore(&W.rsIn);
        s->SamplerStateRestore(W.smIn);
        s->TextureStageStateRestore(W.tsIn);
        break;
    case C_SETTEXTURE:
        /* only -1, -2 and 0x80000000|n are in range: any other
           negative id indexes d3dShadowTextures with the sign bit
           masked off, which is 0x7FFFFFxx entries past the array */
        s->SetTexture((int)(seed % 4u), (int)(seed % 9u) - 2);
        s->SetTexture((int)((seed >> 2) % 4u), -2);
        s->SetTexture(0, (int)(0x80000000u | (seed % 8u)));
        break;
    case C_SAVERESTORE:
        s->SaveEverythingToBackup((int)(seed % 4u));
        s->SetInitialRenderStates();
        s->RestoreEverythingFromBackup();
        break;
    case C_COLORWRITE:
        s->SetColorWriteEnable((char)(seed & 0xFF));
        break;
    case C_CULLING:
        s->SetCullingParams();
        break;
    case C_STAGESTATES:
        s->SetTextureStageStates((int)(seed % 4u), (int)(seed % 7u) - 3);
        break;
    case C_ALPHATEST:
        s->SetAlphaTestParams(seed * 2654435761u, (int16_t)(seed & 0xFFFu));
        break;
    case C_ALPHABLEND:
        s->SetAlphaBlendingOptions((unsigned int)((seed % 10u) << 16) | (seed & 0xFFFFu),
                                   (char)((seed & 1) ? 0x80 : 0x01));
        break;
    case C_FOG:
        s->SetFogParams((unsigned int)((seed % 9u) << 16) | (seed & 0xFFFFu));
        break;
    case C_TEXTUREINIT:
        s->TextureInit((int)(seed % 7u) - 2, (int)(seed * 37u), (char)(seed & 3u));
        break;
    case C_REQFOG:
        s->RequestSetFogParams(&W.fog);
        break;
    case C_STRIDE:
        ret = s->CalculateStride(seed * 2654435761u);
        break;
    case C_SELECTFORMAT:
        ret = s->SelectSupportedDeviceFormat((int)(seed % 5u));
        break;
    case C_PRESETS:
        s->ApplyPresets();
        break;
    case C_VIEWPORT:
        s->SetViewPort((int)(seed % 9u) - 4, (int)((seed >> 2) % 9u) - 4,
                       (int)(seed % 900u), (int)(seed % 700u));
        break;
    case C_PROJECTION:
        s->SetProjectionMatrix(&W.dd.ct.cam);
        break;
    case C_ASPECT:
        *fret = s->RealYoverX();
        ret = s->FlatLayerWidth((int)(seed % 900u), (int)(seed % 700u));
        extra = s->FlatLayerOffsetX((int)(seed % 900u), (int)(seed % 700u));
        *fret += s->FlatLayerToLayoutX((float)(seed % 100u) * 0.01f,
                                       (int)(seed % 900u), (int)(seed % 700u));
        *fret += s->FlatLayerFromLayoutX((float)(seed % 100u) * 0.01f,
                                         (int)(seed % 900u), (int)(seed % 700u));
        ret = ret * 1000 + extra;
        break;
    case C_CLEAR:
        s->Clear((int)(seed % 4u), (int)(seed * 2654435761u));
        break;
    case C_SDW:
        s->SetRenderTargetSDW((int)(seed % 9u) - 1, (int)(seed & 1));
        s->SetRenderTargetSDW(-1, 0);
        break;
    case C_VIEWMATRIX:
        ret = s->SetViewMatrix(&W.viewMtx);
        s->SetViewMatrixSDW(&W.viewMtx, W.unkVals);
        break;
    case C_XFORMSAVE:
        s->SetTransformAndSaveMatrix(&WGM_tex[3], (int)(seed % 8u));
        break;
    case C_WATERPARAMS:
        water_ClippingHasBeenSet = (int)(seed & 1);
        ret = s->WaterParamsOn();
        if (ret)
            s->WaterParamsOff();
        break;
    case C_VBO:
        s->PrepareForDrawing((int)(seed % 5u));
        s->FillSelectedVBOWithSprite(&W.spriteData);
        s->FillSelectedVBOWithSprite(&W.spriteData);
        s->SpriteDraw();
        break;
    case C_REQUEST:
    {
        static const int types[] = { 1, 12, 18, 19, 20, 24, 25, 33, 7, 99 };
        int type = types[seed % 10u];
        void* param;
        switch (type)
        {
        case 1:  param = &W.fog; break;
        case 12: param = &W.upload; break;
        case 19:
        case 33: param = &W.spriteData; break;
        case 24: param = &W.snap; break;
        default: param = (void*)(size_t)(seed % 5u); break;
        }
        if (type == 19 || type == 33)
            s->PrepareForDrawing(2);
        ret = s->Request(type, param);
        break;
    }
    case C_RASTERS:
        s->RastersUpdate();
        break;
    case C_RASTERQUAD:
        s->RastersQuad((int)(seed * 7u), (float)(seed % 50u), (float)(seed % 40u),
                       (float)(seed % 30u), (float)(seed % 20u), (int)(seed & 1));
        s->DrawARaster((int)(seed * 11u), "r", (float)(seed % 9u) * 0.5f,
                       (float)(seed % 30u), 100.0f + (float)(seed % 400u));
        break;
    case C_WATERBUFFERS:
        wtr_NumA = (int)(2u + seed % 7u);
        wtr_NumB = (int)(1u + seed % 5u);
        waterStreamData = NULL;
        s->WaterBuffersInit();
        break;
    case C_WATERSTREAM:
        waterStreamData = &WGM_vb[20];
        ret = s->FillWaterStreamData(&W.water);
        break;
    case C_DRAWWATER:
        waterStreamData = &WGM_vb[20];
        waterIndexData = &WGM_ib[1];
        s->DrawWater(&W.water);
        break;
    case C_UVCOMPUTE:
        s->ComputeUV(&W.geogro, &W.chunks[0], &W.texElem);
        break;
    case C_UVCHROME:
        s->UvChrome(&W.dd, &W.geogro, &W.chunks[0], seed);
        break;
    case C_UVPLANAR:
        s->UvPlanar(&W.dd, &W.geogro, &W.chunks[0], seed);
        break;
    case C_SPRITES:
        ret = s->DrawSprites(&W.sprites,
                             ((seed >> 26) & 1) ? (void*)W.c0de : (void*)W.spritePos,
                             (int)(seed % 3u));
        break;
    case C_DRAWTRIS:
        ret = s->DrawElementIndexedTriangles(&W.chunks[0], W.verts,
                                             ((seed >> 13) & 1) ? W.uvs : NULL,
                                             (int)W.geogro.vertexNum);
        break;
    case C_CREATEBUFFERS:
        *s->multiStreamWorld = (tWorld*)(size_t)1;
        s->CreateBuffers(&W.gao, &W.geogro);
        *s->multiStreamWorld = NULL;
        break;
    case C_RELEASEMESH:
        s->ReleaseMeshData(&W.gaos[(seed % 4u)]);
        WOR_gpst_CurrentWorld = &W.world;
        s->ReleaseMeshDataOfWorld(&W.world);
        s->ReleaseEveryCache();
        break;
    case C_RELEASE:
        s->FreeBackCache(&W.geogro);
        s->ReleaseTextures(&W.dd);
        s->ReleaseShadowTextures();
        s->ReleaseVBOs();
        s->ReleaseEverything();
        break;
    case C_CREATE:
        s->CreateVBOs();
        ret = s->TextureStoreInit(&W.dd, (int)(seed % 5u));
        s->CreateAndCopySource();
        break;
    case C_PALETTE:
        ret = s->TextureStoreInit(&W.dd, 6);
        s->PaletteLoad(&W.dd, &W.texList[W.upload.idx], &W.inter,
                       (int)(seed % 4u));
        s->TextureStore(&W.upload);
        break;
    case C_SNAPSHOT:
        s->GetSnapShot(&W.snap);
        break;
    case C_WNDMSG:
    {
        static const int msgs[] = { 0x0001, 0x0002, 0x0010, 0x0012, 0x0014,
                                    0x001C, 0x0112, 0x00A1, 0x0200, 0xC123 };
        int lp = (int)seed;
        ret = s->WndMsg((HWND)0x100, msgs[seed % 10u], (int)(seed % 0xF060u),
                        (int)seed, &lp);
        ret = ret * 7 + lp;
        extra = s->WndCreate((HWND)0x100, &W.handles, msgs[(seed + 1u) % 10u],
                             0, 0, &lp);
        ret = ret * 7 + extra + lp;
        break;
    }
    case C_SETUVMATRIX:
        s->SetUVMatrix(&W.viewMtx, &W.verts[0], &W.verts[1], &W.verts[2],
                       &W.verts[3], &W.verts[4], (int)seed, (int)(seed % 9u));
        break;
    default:
        break;
    }
    return ret;
}

/* ---------- comparison ---------- */

static int diffs;
static int casesRun;
static long long callsCompared;

static void Report(const char* what, int kind, unsigned int seed)
{
    if (diffs < 12)
        printf("  DIFF  %-18s case %-20s seed %u\n", what, caseName[kind], seed);
    diffs++;
}

static void CompareLogs(int kind, unsigned int seed, tWgmCall* a, int na)
{
    int i, j;

    if (na != WGM_logLen)
    {
        Report("call count", kind, seed);
        if (diffs < 12)
            printf("        base %d, new %d\n", na, WGM_logLen);
        return;
    }
    for (i = 0; i < na; i++)
    {
        if (strcmp(a[i].fn, WGM_log[i].fn) != 0 || a[i].nargs != WGM_log[i].nargs)
        {
            Report("call", kind, seed);
            if (diffs < 12)
                printf("        [%d] base %s, new %s\n", i, a[i].fn, WGM_log[i].fn);
            return;
        }
        for (j = 0; j < a[i].nargs && j < WGM_ARGS; j++)
            if (a[i].arg[j] != WGM_log[i].arg[j])
            {
                Report("call argument", kind, seed);
                if (diffs < 12)
                    printf("        [%d] %s arg %d: base %lld, new %lld\n",
                           i, a[i].fn, j, a[i].arg[j], WGM_log[i].arg[j]);
                return;
            }
    }
    callsCompared += na;
}

static tWgmCall savedLog[WGM_LOG_MAX];

static void RunCase(int kind, unsigned int seed)
{
    int na, ret0, ret1;
    float f0, f1;

    Setup(&sides[0], kind, seed);
    ret0 = RunSide(&sides[0], kind, seed, &f0);
    Snapshot(&sides[0], &shots[0], ret0, f0);
    na = WGM_logLen;
    memcpy(savedLog, WGM_log, sizeof(tWgmCall) * (size_t)na);

    Setup(&sides[1], kind, seed);
    ret1 = RunSide(&sides[1], kind, seed, &f1);
    Snapshot(&sides[1], &shots[1], ret1, f1);

    casesRun++;
    if (memcmp(&shots[0], &shots[1], sizeof(tShot)) != 0)
    {
        unsigned char* a = (unsigned char*)&shots[0];
        unsigned char* b = (unsigned char*)&shots[1];
        size_t i;
        Report("state", kind, seed);
        for (i = 0; i < sizeof(tShot); i++)
            if (a[i] != b[i])
            {
                if (diffs < 12)
                    printf("        first differing byte at offset %zu\n", i);
                break;
            }
    }
    CompareLogs(kind, seed, savedLog, na);
}

int main(void)
{
    int kind;
    unsigned int seed;
    int i;

    MATH_InitModule();
    WGM_InstallDevice();

    for (kind = 0; kind < C_COUNT; kind++)
    {
        for (seed = 0; seed < 64; seed++)
            RunCase(kind, seed);
        for (seed = 0; seed < 32; seed++)
            RunCase(kind, 0x10000u + seed * 7919u);
        for (seed = 0; seed < 16; seed++)
            RunCase(kind, 0xFFFF0000u + seed * 65537u);
    }

    printf("  %d cases, %lld engine calls compared\n", casesRun, callsCompared);
    printf("  guard distribution:\n");
    for (i = 0; i < GUARDS; i++)
        if (guardName[i])
        {
            printf("    %-24s false %6d  true %6d%s\n", guardName[i],
                   guardHit[i][0], guardHit[i][1],
                   (guardHit[i][0] == 0 || guardHit[i][1] == 0) ? "   <-- DEAD" : "");
            if (guardHit[i][0] == 0 || guardHit[i][1] == 0)
                diffs++;
        }
    if (WGM_overflow)
    {
        printf("  call log overflowed\n");
        diffs++;
    }
    if (diffs)
    {
        printf("  %d difference(s)\n", diffs);
        return 1;
    }
    printf("  no differences\n");
    return 0;
}
