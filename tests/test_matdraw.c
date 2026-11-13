/* Differential test for materialanddraw.c.
 *
 * The frozen pre-pass copy and the rewrite go into one binary with their
 * 22 functions prefixed BASE_/NEW_. The real math.c is linked, with
 * MATH_InitModule called before the first case so the inverse table the
 * two z-sort walks index is the engine's own rather than zeros; the real
 * profiler.c is linked too, so the start/end blocks the rewrite calls are
 * the real ones and the frozen copy's inlined arithmetic is measured
 * against them.
 *
 * Compared per case: the return value, every field the case can write --
 * the display data, the big buffer's colour and UV state, the 47 profiler
 * rasters, the D3D render params, the texture stage config, the UV
 * destination buffer, the ZList nodes and their mesh vertices -- and the
 * ordered sequence of engine calls with their arguments.
 */

#include "matdraw_mocks.h"
#include "../math.h"
#include "../gameobject.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>

/* ---------- the two sides ---------- */

#define DECL(p) \
    BOOL p##MAT_IsMaterialTransparent(struct sGEOMAT*, int, int, void*); \
    void p##MAT_SetRenderState(tGEOMAT*, int); \
    void p##MAT_DrawIndexedTriangle_ST(tGdi_DisplayData*, tGEOGRO*, tGEOMAT*, tGeoTriangleChunks*); \
    int  p##MAT_IsObjectTransparent(tGEOMAT*); \
    BOOL p##MAT_MaterialsHaveAlphaBlendingType3And4(tGEOMAT*); \
    BOOL p##MAT_IsLastElemInMultiTexture(tGdi_DisplayData*, tMultiTextureElemStruct*); \
    void p##MAT_DrawIndexedTriangle_MT(tGdi_DisplayData*, tGEOGRO*, tGEOMAT*, tGeoTriangleChunks*); \
    void p##MAT_DrawIndexedTriangle(struct sGEOGRO*, struct sGEOMAT*, struct sGeoTriangleChunks*); \
    void p##GAO_XMN_Render_TransparentTriangles(struct sGEOGRO*, struct sGEOMAT*); \
    void p##GEO_Render_TransparentTriangles(struct sGdi_DisplayData*, struct sGEOGRO*, struct sGameObjectVisualObject*); \
    void p##MAT_DrawIndexedSprites_MT(tGdi_DisplayData*, tGEOGRO*, tGEOMAT*, tGeoSprites*); \
    void p##MAT_DrawIndexedSprites(struct sGEOGRO*, struct sGEOMAT*, struct sGeoSprites*); \
    unsigned int p##MAT_GetValidityMASK(struct sGEOMAT*, int, struct sGEOGRO*); \
    BOOL p##MAT_VUISIdentity(struct sMultiTextureElemStruct*); \
    void p##MAT_UV_COMPUTE_GetMAtrix(struct sGdi_DisplayData*, struct sGameObjectMain*, unsigned int, Mtx4x4*); \
    void p##MAT_SetScale(int*, float); \
    void p##MAT_SetSpeed(int*, float); \
    void p##MAT_SetPos(int*, float); \
    void p##MAT_SetRotation(int*, int*, float); \
    float p##MAT_GetSpeed(int); \
    float p##MAT_GetScale(int); \
    float p##MAT_GetRotation(int, unsigned int);

DECL(BASE_)
DECL(NEW_)

typedef struct sSide
{
    const char* name;
    BOOL (*IsMaterialTransparent)(struct sGEOMAT*, int, int, void*);
    void (*SetRenderState)(tGEOMAT*, int);
    void (*DrawIndexedTriangle_ST)(tGdi_DisplayData*, tGEOGRO*, tGEOMAT*, tGeoTriangleChunks*);
    int  (*IsObjectTransparent)(tGEOMAT*);
    BOOL (*MaterialsHaveAlphaBlending)(tGEOMAT*);
    BOOL (*IsLastElemInMultiTexture)(tGdi_DisplayData*, tMultiTextureElemStruct*);
    void (*DrawIndexedTriangle_MT)(tGdi_DisplayData*, tGEOGRO*, tGEOMAT*, tGeoTriangleChunks*);
    void (*DrawIndexedTriangle)(struct sGEOGRO*, struct sGEOMAT*, struct sGeoTriangleChunks*);
    void (*XmnRender)(struct sGEOGRO*, struct sGEOMAT*);
    void (*GeoRender)(struct sGdi_DisplayData*, struct sGEOGRO*, struct sGameObjectVisualObject*);
    void (*DrawIndexedSprites_MT)(tGdi_DisplayData*, tGEOGRO*, tGEOMAT*, tGeoSprites*);
    void (*DrawIndexedSprites)(struct sGEOGRO*, struct sGEOMAT*, struct sGeoSprites*);
    unsigned int (*GetValidityMASK)(struct sGEOMAT*, int, struct sGEOGRO*);
    BOOL (*VUISIdentity)(struct sMultiTextureElemStruct*);
    void (*UvGetMatrix)(struct sGdi_DisplayData*, struct sGameObjectMain*, unsigned int, Mtx4x4*);
    void (*SetScale)(int*, float);
    void (*SetSpeed)(int*, float);
    void (*SetPos)(int*, float);
    void (*SetRotation)(int*, int*, float);
    float (*GetSpeed)(int);
    float (*GetScale)(int);
    float (*GetRotation)(int, unsigned int);
} tSide;

#define SIDE(nm, p) { nm, \
    p##MAT_IsMaterialTransparent, p##MAT_SetRenderState, p##MAT_DrawIndexedTriangle_ST, \
    p##MAT_IsObjectTransparent, p##MAT_MaterialsHaveAlphaBlendingType3And4, \
    p##MAT_IsLastElemInMultiTexture, p##MAT_DrawIndexedTriangle_MT, p##MAT_DrawIndexedTriangle, \
    p##GAO_XMN_Render_TransparentTriangles, p##GEO_Render_TransparentTriangles, \
    p##MAT_DrawIndexedSprites_MT, p##MAT_DrawIndexedSprites, p##MAT_GetValidityMASK, \
    p##MAT_VUISIdentity, p##MAT_UV_COMPUTE_GetMAtrix, p##MAT_SetScale, p##MAT_SetSpeed, \
    p##MAT_SetPos, p##MAT_SetRotation, p##MAT_GetSpeed, p##MAT_GetScale, p##MAT_GetRotation }

static tSide sides[2] = { SIDE("base", BASE_), SIDE("new", NEW_) };

/* ---------- the world ---------- */

#define MAXV 48
#define MAXTRI 24
#define MAXCHUNK 4
#define MAXTEX 6
#define MAXMAT 6
#define RASTERS 47

typedef struct sMdWorld
{
    tGdi_DisplayData dd;
    tBigBufferStruct* bb;
    profilerRasterStructElem rasters[RASTERS];
    tGroIntf intf3, intf4, intf5, intf1;
    tGEOMAT mats[MAXMAT];
    tGEOMAT nullMaskMat;
    tGEOMAT* matPtrs[MAXMAT];
    tMultiTextureElemStruct* tex;
    tGroIntf intf4null;
    tGEOGRO geogro;
    tGeoTriangleChunks chunks[MAXCHUNK];
    tGeoTriangleChunkElem tris[MAXCHUNK][MAXTRI];
    tGeoMRMUVIndexElem mrmUvIdxElem[MAXCHUNK];
    uint16_t mrmUvIdx[MAXCHUNK][MAXTRI];
    tGeoMRM mrm;
    Vec3 verts[MAXV];
    tGeoUVElem uvs[MAXV];
    tGeoSprites sprites[2];
    tGameObjectVisualObject visual;
    tGameObjectMain gao;
    tGameObjectMain partGao;
    tGameObjectBasic basic;
    tGameObjectBasic partBasic;
    tGameObjectMultiPartData multiPart;
    tGameObjectMultiPartData partMultiPart;
    Mtx4x4 gaoMtx;
    Mtx4x4 partMtx;
    Mtx4x4 partMatrices[8];
    tMultiPartGameObjectLinks links[8];
    tEngTexListElemStruct texElems[8];
    IDirect3DDevice9 dev;
    IDirect3DDevice9Vtbl vtbl;
    Mtx4x4 outMtx;
    int transformA;
    int transformB;
} tMdWorld;

static tMdWorld W;

static int MASK_table[8] = { 0, 0x11, 0x40, 0x80, 0xC0, 0x41, 0x140, 0x1 };
static int MASK_cursor;

static int Mask_FromTable(void* mat, int idx)
{
    MDMOCK_Record("GetValidatedMask", 2, (long long)MDMOCK_Index(mat), (long long)idx);
    return MASK_table[(MASK_cursor++ + (idx & 7)) & 7];
}

static HRESULT Vt_SetTextureStageState(IDirect3DDevice9* dev, DWORD stage,
                                       D3DTEXTURESTAGESTATETYPE type, DWORD value)
{
    MDMOCK_Record("SetTextureStageState", 3, (long long)stage, (long long)type,
                  (long long)value);
    return 0;
}

static void RegisterPointers(void)
{
    int i;

    MDMOCK_RegisterPtr(&W.dd);
    MDMOCK_RegisterPtr(W.bb);
    MDMOCK_RegisterPtr(&W.geogro);
    MDMOCK_RegisterPtr(W.verts);
    MDMOCK_RegisterPtr(W.uvs);
    MDMOCK_RegisterPtr(&W.visual);
    MDMOCK_RegisterPtr(&W.gao);
    for (i = 0; i < MAXMAT; i++)
        MDMOCK_RegisterPtr(&W.mats[i]);
    for (i = 0; i < MAXTEX; i++)
        MDMOCK_RegisterPtr(&W.tex[i]);
    for (i = 0; i < MAXCHUNK; i++)
        MDMOCK_RegisterPtr(&W.chunks[i]);
    for (i = 0; i < 2; i++)
        MDMOCK_RegisterPtr(&W.sprites[i]);
    MDMOCK_RegisterPtr(W.bb->data7);
    MDMOCK_RegisterPtr(&MAT_gst_DefaultSingleMaterial);
    MDMOCK_RegisterPtr(NULL);
}

/* seed-driven world, so each case drives a different shape on purpose */
static void BuildWorld(unsigned int seed)
{
    tBigBufferStruct* bb = W.bb;
    int i, c, t;

    memset(&W.dd, 0, sizeof(W.dd));
    memset(W.rasters, 0, sizeof(W.rasters));
    memset(bb, 0, sizeof(*bb));
    memset(W.mats, 0, sizeof(W.mats));
    memset(&W.nullMaskMat, 0, sizeof(W.nullMaskMat));
    memset(W.tex, 0, MAXTEX * sizeof(tMultiTextureElemStruct));
    memset(&W.geogro, 0, sizeof(W.geogro));
    memset(W.chunks, 0, sizeof(W.chunks));
    memset(W.tris, 0, sizeof(W.tris));
    memset(W.sprites, 0, sizeof(W.sprites));
    memset(&W.outMtx, 0, sizeof(W.outMtx));
    memset(&D3DRenderParams, 0, sizeof(D3DRenderParams));
    memset(textureStageTypeConfig, 0, sizeof(textureStageTypeConfig));
    memset(W.texElems, 0, sizeof(W.texElems));
    memset(&MAT_gst_DefaultSingleMaterial, 0, sizeof(MAT_gst_DefaultSingleMaterial));
    MASK_cursor = seed & 7;

    W.intf1.id = 1;
    W.intf3.id = 3;
    W.intf4.id = 4;
    W.intf5.id = 5;
    W.intf1.GetValidatedMask = Mask_FromTable;
    W.intf3.GetValidatedMask = Mask_FromTable;
    W.intf4.GetValidatedMask = Mask_FromTable;
    W.intf5.GetValidatedMask = Mask_FromTable;
    W.intf4null.id = 4;
    W.intf4null.GetValidatedMask = NULL;
    W.nullMaskMat.intf = &W.intf4null;
    W.nullMaskMat.multiMaterialNumIfType4ThenAmbiantColor = 2;
    W.nullMaskMat.multiMaterialPtrIfType4ThenDiffuseColor = W.matPtrs;
    W.nullMaskMat.idxOrPtr = (int)(intptr_t)&W.tex[0];

    /* materials: 0 is the multi-material head, 1..4 its children */
    W.mats[0].intf = &W.intf4;
    W.mats[0].multiMaterialNumIfType4ThenAmbiantColor = (seed & 3) + 1;
    W.mats[0].multiMaterialPtrIfType4ThenDiffuseColor = W.matPtrs;
    W.mats[0].idxOrPtr = (int)(intptr_t)&W.tex[0];
    for (i = 0; i < MAXMAT; i++)
        W.matPtrs[i] = &W.mats[i];
    if (seed & 0x20000)
        W.matPtrs[0] = NULL;
    for (i = 1; i < MAXMAT; i++)
    {
        W.mats[i].intf = (i & 1) ? &W.intf5 : &W.intf3;
        W.mats[i].solidVertexColor = 0x11223344 + i;
        W.mats[i].unkFloat2 = (i & 2) ? 0.5f : 0.995f;
        W.mats[i].idxOrPtr = (int)(intptr_t)&W.tex[i % MAXTEX];
    }
    W.mats[1].idxOrPtr = (int)(intptr_t)&W.tex[0];
    MAT_gst_DefaultSingleMaterial.intf = &W.intf3;
    MAT_gst_DefaultSingleMaterial.unkFloat2 = 0.1f;
    MAT_gst_DefaultSingleMaterial.idxOrPtr = 3;

    /* a four-long multi-texture chain with a different colour mode each */
    for (i = 0; i < MAXTEX; i++)
    {
        W.tex[i].texElemIdx = (int16_t)((seed + i) % 5 == 4 ? -1 : (i % 4));
        W.tex[i].phongOrPlanarParam = (uint16_t)(0x1234 + 0x1111 * i);
        W.tex[i].subTextureFlags = (unsigned int)(((seed >> (i * 3)) & 7) << 12)
                                 | (((seed >> i) & 1) ? 0x400u : 0u)
                                 | (((seed >> (i + 4)) & 1) ? 0x800u : 0u)
                                 | (((seed >> (i + 8)) & 1) ? 0x200u : 0u)
                                 | ((unsigned int)((seed >> (i + 12)) & 7) << 16);
        W.tex[i].transform1 = (seed & 1) ? 0x3F800000 : (int)(0x40000000 + i);
        W.tex[i].transform2 = (seed & 2) ? 0x3F800000 : (int)(0x40400000 + i);
        if (((seed >> 2) & 3) == 1 && (i & 1) == 0)
            W.tex[i].transform1 = 0;
        if (((seed >> 2) & 3) == 2 && (i & 1) == 0)
            W.tex[i].transform2 = 0;
        if (((seed >> 2) & 3) == 3 && (i & 1) == 1)
        {
            W.tex[i].transform1 = 0;
            W.tex[i].transform2 = 0;
        }
        W.tex[i].nextTextureKey = (i + 1 < 4) ? &W.tex[i + 1] : NULL;
    }
    if (seed & 0x40000)
        W.tex[3].nextTextureKey = (tMultiTextureElemStruct*)-1;

    for (i = 0; i < 8; i++)
        W.texElems[i].texElemFlag = (int16_t)(((seed >> i) & 1) ? 0x20 : 0x04);
    TEX_gst_GlobalList.texElems = W.texElems;
    TEX_gst_GlobalList.actTexElemNum = 8;

    /* geometry */
    for (i = 0; i < MAXV; i++)
    {
        W.verts[i].x = (float)((int)((seed + i * 7) % 17) - 8);
        W.verts[i].y = (float)((int)((seed + i * 13) % 19) - 9);
        W.verts[i].z = (float)((int)((seed + i * 5) % 23) - 11);
        W.uvs[i].u = (float)i * 0.125f;
        W.uvs[i].v = (float)i * 0.0625f;
    }
    W.geogro.intf = &W.intf1;
    W.geogro.vertexNum = MAXV;
    W.geogro.vertices = W.verts;
    W.geogro.uvNum = MAXV;
    W.geogro.UVs = W.uvs;
    W.geogro.triangleChunkCount = (seed % 3) + 1;
    W.geogro.triangleChunks = W.chunks;
    W.geogro.mrm = (seed & 4) ? &W.mrm : NULL;
    W.geogro.spritesNum = (seed & 8) ? 2 : 0;
    W.geogro.spritesPtr = W.sprites;
    for (c = 0; c < MAXCHUNK; c++)
    {
        W.chunks[c].triangleNum = (uint32_t)((seed + c) % 5) + 1;
        W.chunks[c].materialNum = (int)((seed + c) % 4);
        W.chunks[c].triangleChunkElems = W.tris[c];
        W.chunks[c].MRMUVIndicesNum = (uint32_t)(((seed >> 2) + c) % 6) + 1;
        W.chunks[c].MRMUVIndices = W.mrmUvIdx[c];
        W.mrmUvIdxElem[c].UVStartOffset = (int)((seed + c) % 4);
        W.chunks[c].MRMUVIndexElems = &W.mrmUvIdxElem[c];
        for (t = 0; t < MAXTRI; t++)
        {
            W.tris[c][t].v1 = (uint16_t)((seed + c * 3 + t * 5) % MAXV);
            W.tris[c][t].v2 = (uint16_t)((seed + c * 7 + t * 11) % MAXV);
            W.tris[c][t].v3 = (uint16_t)((seed + c * 13 + t * 3) % MAXV);
            W.tris[c][t].uv1 = (uint16_t)((seed + t) % MAXV);
            W.tris[c][t].uv2 = (uint16_t)((seed + t * 2) % MAXV);
            W.tris[c][t].uv3 = (uint16_t)((seed + t * 4) % MAXV);
            W.mrmUvIdx[c][t] = (uint16_t)((seed + c + t * 2) % 16);
        }
    }
    for (i = 0; i < 2; i++)
    {
        W.sprites[i].num = (uint32_t)((seed >> i) & 1) * 3;
        W.sprites[i].matIdx = (int)((seed + i) % 4);
    }

    /* display data */
    W.dd.bigBuffer = bb;
    W.dd.rasters = W.rasters;
    W.dd.verticesCurrent = W.verts;
    W.dd.drawMaskAllowedStuffCurrent = (int)(seed & 0xFF) | (int)((seed & 0x100) ? 1 : 0);
    W.dd.flags = (int)((seed >> 4) & 0xF) | (int)((seed & 0x20) ? 0x8000 : 0);
    W.dd.RLIPtr = (int*)bb->data1LightColors;
    W.dd.TextureInit = MDMOCK_TextureInit;
    W.dd.DrawElementIndexedTriangles = MDMOCK_DrawElementIndexedTriangles;
    W.dd.DrawSprites = MDMOCK_DrawSprites;
    W.dd.currTargetGao = &W.gao;
    W.dd.matrixStack.actMatrixElem = &W.gaoMtx;
    W.dd.ct.cam.factorX = 1.5f + (float)(seed % 5);
    W.dd.ct.cam.centerX = 2.5f + (float)(seed % 3);
    bb->flags = (int)(seed & 7);
    for (i = 0; i < (int)sizeof(bb->data3FinalColorWithShadow); i++)
        bb->data3FinalColorWithShadow[i] = (uint8_t)((seed + i * 31) & 0xFF);
    gpst_DX_stDD = &W.dd;

    D3DRenderParams.d3dDev = &W.dev;
    W.dev.lpVtbl = &W.vtbl;
    W.vtbl.SetTextureStageState = Vt_SetTextureStageState;
    D3DRenderParams.isDepthBlurOn = (int)((seed >> 5) & 1);
    D3DRenderParams.tweak_zonalpha = (int)((seed >> 6) & 1);
    D3DRenderParams.tweak_ae_d2x = (int)((seed >> 7) & 1);
    /* These were (seed >> 9) & 1 and (seed >> 10) & 1, and both are zero
       for every seed the sweep uses -- 0..95 never reaches bit 9, and
       0x10000..0x10040 has those bits clear too. So both stage-state
       blocks below them were dead, and swapping D3DTSS_TEXCOORDINDEX for
       D3DTSS_TEXTURETRANSFORMFLAGS scored clean. Drive them from the low
       bits instead, which vary across both ranges. */
    textureStageTypeConfig[0].d3dTexCoordIndex = (int)(seed % 5) < 2;
    textureStageTypeConfig[0].d3dTextureTransformFlags = (int)(seed % 7) < 3;

    /* game objects for the UV matrix */
    memset(&W.gao, 0, sizeof(W.gao));
    memset(&W.partGao, 0, sizeof(W.partGao));
    memset(&W.basic, 0, sizeof(W.basic));
    memset(&W.partBasic, 0, sizeof(W.partBasic));
    memset(W.links, 0, sizeof(W.links));
    for (i = 0; i < 8; i++)
    {
        int k;
        float* f = (float*)&W.partMatrices[i];
        for (k = 0; k < (int)(sizeof(Mtx4x4) / sizeof(float)); k++)
            f[k] = (float)(((int)(seed + i * 3 + k) % 11) - 5) * 0.25f;
    }
    {
        int k;
        float* f = (float*)&W.gaoMtx;
        for (k = 0; k < (int)(sizeof(Mtx4x4) / sizeof(float)); k++)
            f[k] = (float)(((int)(seed + k * 7) % 13) - 6) * 0.5f;
    }
    W.gao.matrices = &W.gaoMtx;
    W.gao.basic = &W.basic;
    W.gao.flags = (int)(((seed >> 11) & 1) ? 0x200000 : 0)
                | (int)(((seed >> 12) & 1) ? 0x1000000 : 0);
    W.basic.multiPartData = &W.multiPart;
    W.multiPart.elemNum = (int)((seed >> 13) & 7) + 1;
    W.multiPart.multipartGaoObjectPartsOrMatrices =
        ((seed >> 12) & 1) ? (void*)W.links : (void*)W.partMatrices;
    W.partGao.matrices = &W.partMtx;
    W.partGao.basic = &W.partBasic;
    W.partGao.flags = (int)(((seed >> 14) & 1) ? 0x200000 : 0)
                    | (int)(((seed >> 15) & 1) ? 0x1000000 : 0);
    W.partBasic.multiPartData = &W.partMultiPart;
    W.partMultiPart.elemNum = 8;
    W.partMultiPart.multipartGaoObjectPartsOrMatrices = (void*)W.links;
    {
        int k;
        float* f = (float*)&W.partMtx;
        for (k = 0; k < (int)(sizeof(Mtx4x4) / sizeof(float)); k++)
            f[k] = (float)(((int)(seed + k * 5) % 9) - 4) * 0.75f;
    }
    for (i = 0; i < 8; i++)
    {
        W.links[i].gao = ((seed >> i) & 1) ? &W.partGao : NULL;
        W.links[i].mtxPtr = ((seed >> (i + 1)) & 1) ? &W.partMatrices[i] : NULL;
        W.links[i].mtxIdx = i;
    }
    W.visual.GROMAT = (struct sGroGeneric*)&W.mats[0];
    for (i = 0; i < 8; i++)
        MDMOCK_geoMaterials[i] = (i < MAXMAT) ? &W.mats[i] : NULL;

    W.transformA = (int)seed;
    W.transformB = (int)(seed * 2654435761u);
}

/* ---------- snapshot ---------- */

typedef struct sShot
{
    tGdi_DisplayData dd;
    profilerRasterStructElem rasters[RASTERS];
    int bbFlags, bbSolid, bbColorMask;
    int bbVertexColorArray, bbVertexColorArraySecond, bbUVData;
    uint8_t uvDest[256];
    tGEOMAT mats[MAXMAT];
    tMultiTextureElemStruct tex[MAXTEX];
    int diffuse2x, texUVMode, texCoordIdx, texTransformFlags;
    Mtx4x4 outMtx;
    int transformA, transformB;
    int nodeCount;
    tZListMeshElem mesh[MDMOCK_NODES][MDMOCK_NODE_MESH];
    int nodeGeomat[MDMOCK_NODES];
    int nodeFlags[MDMOCK_NODES];
    unsigned int ret[8192];
    int nret;
} tShot;

static tShot shot[2];
static tMdMockCall logCopy[2][MDMOCK_LOG_MAX];
static int logLen[2];

static void TakeShot(tShot* s)
{
    int i;

    memcpy(&s->dd, &W.dd, sizeof(s->dd));
    s->dd.bigBuffer = NULL;
    s->dd.rasters = NULL;
    s->dd.verticesCurrent = NULL;
    s->dd.RLIPtr = NULL;
    s->dd.currTargetGao = NULL;
    s->dd.matrixStack.actMatrixElem = NULL;
    s->dd.TextureInit = NULL;
    s->dd.DrawElementIndexedTriangles = NULL;
    s->dd.DrawSprites = NULL;
    s->dd.currGEOMAT = (struct sGEOMAT*)(intptr_t)MDMOCK_Index(W.dd.currGEOMAT);
    memcpy(s->rasters, W.rasters, sizeof(s->rasters));
    s->bbFlags = W.bb->flags;
    s->bbSolid = W.bb->solidVertexColor;
    s->bbColorMask = W.bb->colorMask;
    s->bbVertexColorArray = MDMOCK_Index(W.bb->vertexColorArray);
    if (W.bb->vertexColorArray == (int*)W.bb->data3FinalColorWithShadow)
        s->bbVertexColorArray = 1001;
    else if (W.bb->vertexColorArray == (int*)W.bb->data2SpecularColors)
        s->bbVertexColorArray = 1002;
    else if (W.bb->vertexColorArray == W.dd.RLIPtr)
        s->bbVertexColorArray = 1003;
    s->bbVertexColorArraySecond = W.bb->vertexColorArraySecond ? 1 : 0;
    s->bbUVData = 0;
    if (W.bb->UVData == (uint8_t*)W.uvs)
        s->bbUVData = 1;
    else if (W.bb->UVData == W.bb->data7)
        s->bbUVData = 2;
    else if (W.bb->UVData)
        s->bbUVData = 3;
    memcpy(s->uvDest, W.bb->data7, sizeof(s->uvDest));
    memcpy(s->mats, W.mats, sizeof(s->mats));
    memcpy(s->tex, W.tex, MAXTEX * sizeof(tMultiTextureElemStruct));
    for (i = 0; i < MAXMAT; i++)
    {
        s->mats[i].intf = NULL;
        s->mats[i].multiMaterialPtrIfType4ThenDiffuseColor = NULL;
        s->mats[i].idxOrPtr = 0;
    }
    for (i = 0; i < MAXTEX; i++)
        s->tex[i].nextTextureKey = NULL;
    s->diffuse2x = D3DRenderParams.diffuse2x;
    s->texUVMode = D3DRenderParams.textureUVMode;
    s->texCoordIdx = textureStageTypeConfig[0].d3dTexCoordIndex;
    s->texTransformFlags = textureStageTypeConfig[0].d3dTextureTransformFlags;
    s->outMtx = W.outMtx;
    s->transformA = W.transformA;
    s->transformB = W.transformB;
    s->nodeCount = MDMOCK_nodeCount;
    memcpy(s->mesh, MDMOCK_mesh, sizeof(s->mesh));
    for (i = 0; i < MDMOCK_NODES; i++)
    {
        s->nodeGeomat[i] = MDMOCK_Index(MDMOCK_nodes[i].geomat);
        s->nodeFlags[i] = MDMOCK_nodes[i].renderingFlags;
    }
}

/* ---------- comparison ---------- */

static int cases, failures, valuesCompared, callsCompared;

static void Fail(const char* what, const char* detail)
{
    if (failures < 25)
        printf("  DIFF %-26s %s\n", what, detail);
    failures++;
}

static void CompareCase(const char* name)
{
    char buf[256];
    int i, j;

    cases++;
    valuesCompared += 20 + RASTERS * 4 + MAXMAT * 4 + MAXTEX * 4;
    if (shot[0].nret > (int)(sizeof(shot[0].ret) / sizeof(shot[0].ret[0])))
        Fail("return buffer overflowed", name);
    if (shot[0].nret != shot[1].nret)
    {
        sprintf(buf, "%s: %d vs %d", name, shot[0].nret, shot[1].nret);
        Fail("return count", buf);
    }
    else
    {
        for (i = 0; i < shot[0].nret; i++)
        {
            valuesCompared++;
            if (shot[0].ret[i] != shot[1].ret[i])
            {
                sprintf(buf, "%s ret[%d]: %08x vs %08x", name, i,
                        shot[0].ret[i], shot[1].ret[i]);
                Fail("return value", buf);
                break;
            }
        }
    }
    if (memcmp(&shot[0].dd, &shot[1].dd, sizeof(tGdi_DisplayData)) != 0)
    {
        sprintf(buf, "%s: mask %d/%d flags %d/%d currGEOMAT %d/%d", name,
                shot[0].dd.drawMaskAllowedStuffCurrent, shot[1].dd.drawMaskAllowedStuffCurrent,
                shot[0].dd.flags, shot[1].dd.flags,
                (int)(intptr_t)shot[0].dd.currGEOMAT, (int)(intptr_t)shot[1].dd.currGEOMAT);
        Fail("display data", buf);
    }
    for (i = 0; i < RASTERS; i++)
    {
        profilerRasterStructElem* a = &shot[0].rasters[i];
        profilerRasterStructElem* b = &shot[1].rasters[i];
        if (a->insideCounter != b->insideCounter || a->startTick != b->startTick
            || a->sumTime != b->sumTime || memcmp(&a->unk2, &b->unk2, 4) != 0)
        {
            sprintf(buf, "%s raster %d: cnt %d/%d start %d/%d sum %d/%d val %08x/%08x",
                    name, i, a->insideCounter, b->insideCounter, a->startTick, b->startTick,
                    a->sumTime, b->sumTime, *(unsigned int*)&a->unk2, *(unsigned int*)&b->unk2);
            Fail("raster", buf);
            break;
        }
    }
    if (shot[0].bbFlags != shot[1].bbFlags || shot[0].bbSolid != shot[1].bbSolid
        || shot[0].bbColorMask != shot[1].bbColorMask
        || shot[0].bbVertexColorArray != shot[1].bbVertexColorArray
        || shot[0].bbVertexColorArraySecond != shot[1].bbVertexColorArraySecond
        || shot[0].bbUVData != shot[1].bbUVData)
    {
        sprintf(buf, "%s: solid %08x/%08x mask %08x/%08x vca %d/%d vca2 %d/%d uv %d/%d",
                name, shot[0].bbSolid, shot[1].bbSolid, shot[0].bbColorMask,
                shot[1].bbColorMask, shot[0].bbVertexColorArray, shot[1].bbVertexColorArray,
                shot[0].bbVertexColorArraySecond, shot[1].bbVertexColorArraySecond,
                shot[0].bbUVData, shot[1].bbUVData);
        Fail("big buffer", buf);
    }
    if (memcmp(shot[0].uvDest, shot[1].uvDest, sizeof(shot[0].uvDest)) != 0)
    {
        sprintf(buf, "%s: MRM UV destination differs", name);
        Fail("uv buffer", buf);
    }
    if (memcmp(shot[0].mats, shot[1].mats, sizeof(shot[0].mats)) != 0)
        Fail("materials", name);
    if (memcmp(shot[0].tex, shot[1].tex, sizeof(shot[0].tex)) != 0)
        Fail("texture elems", name);
    if (shot[0].diffuse2x != shot[1].diffuse2x || shot[0].texUVMode != shot[1].texUVMode
        || shot[0].texCoordIdx != shot[1].texCoordIdx
        || shot[0].texTransformFlags != shot[1].texTransformFlags)
    {
        sprintf(buf, "%s: d2x %d/%d uvmode %d/%d coord %d/%d tflags %d/%d", name,
                shot[0].diffuse2x, shot[1].diffuse2x, shot[0].texUVMode, shot[1].texUVMode,
                shot[0].texCoordIdx, shot[1].texCoordIdx,
                shot[0].texTransformFlags, shot[1].texTransformFlags);
        Fail("render params", buf);
    }
    if (memcmp(&shot[0].outMtx, &shot[1].outMtx, sizeof(Mtx4x4)) != 0)
    {
        float* a = (float*)&shot[0].outMtx;
        float* b = (float*)&shot[1].outMtx;
        for (i = 0; i < (int)(sizeof(Mtx4x4) / sizeof(float)); i++)
            if (memcmp(&a[i], &b[i], 4) != 0)
            {
                sprintf(buf, "%s mtx[%d]: %.9g vs %.9g", name, i, a[i], b[i]);
                Fail("uv matrix", buf);
                break;
            }
    }
    if (shot[0].transformA != shot[1].transformA || shot[0].transformB != shot[1].transformB)
    {
        sprintf(buf, "%s: %08x/%08x %08x/%08x", name, shot[0].transformA,
                shot[1].transformA, shot[0].transformB, shot[1].transformB);
        Fail("packed transform", buf);
    }
    if (shot[0].nodeCount != shot[1].nodeCount)
    {
        sprintf(buf, "%s: %d vs %d", name, shot[0].nodeCount, shot[1].nodeCount);
        Fail("zlist node count", buf);
    }
    else
    {
        valuesCompared += shot[0].nodeCount * MDMOCK_NODE_MESH * 6;
        if (memcmp(shot[0].mesh, shot[1].mesh, sizeof(shot[0].mesh)) != 0)
        {
            for (i = 0; i < MDMOCK_NODES && failures < 25; i++)
                for (j = 0; j < MDMOCK_NODE_MESH; j++)
                    if (memcmp(&shot[0].mesh[i][j], &shot[1].mesh[i][j],
                               sizeof(tZListMeshElem)) != 0)
                    {
                        sprintf(buf, "%s node %d vtx %d: (%g %g %g|%08x) vs (%g %g %g|%08x)",
                                name, i, j,
                                shot[0].mesh[i][j].point.x, shot[0].mesh[i][j].point.y,
                                shot[0].mesh[i][j].point.z, shot[0].mesh[i][j].vertexColor,
                                shot[1].mesh[i][j].point.x, shot[1].mesh[i][j].point.y,
                                shot[1].mesh[i][j].point.z, shot[1].mesh[i][j].vertexColor);
                        Fail("zlist mesh", buf);
                        i = MDMOCK_NODES;
                        break;
                    }
        }
        if (memcmp(shot[0].nodeGeomat, shot[1].nodeGeomat,
                   sizeof(shot[0].nodeGeomat)) != 0)
            Fail("zlist material", name);
        if (memcmp(shot[0].nodeFlags, shot[1].nodeFlags, sizeof(shot[0].nodeFlags)) != 0)
            Fail("zlist flags", name);
    }
    if (logLen[0] != logLen[1])
    {
        sprintf(buf, "%s: %d vs %d calls", name, logLen[0], logLen[1]);
        Fail("call sequence", buf);
        return;
    }
    for (i = 0; i < logLen[0]; i++)
    {
        tMdMockCall* a = &logCopy[0][i];
        tMdMockCall* b = &logCopy[1][i];
        callsCompared++;
        if (strcmp(a->fn, b->fn) || a->nargs != b->nargs)
        {
            sprintf(buf, "%s call %d: %s vs %s", name, i, a->fn, b->fn);
            Fail("call sequence", buf);
            return;
        }
        for (j = 0; j < a->nargs; j++)
            if (a->arg[j] != b->arg[j])
            {
                sprintf(buf, "%s call %d (%s) arg %d: %lld vs %lld", name, i, a->fn, j,
                        a->arg[j], b->arg[j]);
                Fail("call sequence", buf);
                return;
            }
    }
}

/* ---------- cases ---------- */

static void Case_BitPacking(const tSide* s, tShot* o, unsigned int seed)
{
    static const float vals[8] = { 0.0f, 1.0f, -1.0f, 0.5f, 123.75f, -0.03125f,
                                   1e-7f, 65535.5f };
    static const int packs[8] = { 0, 0x8001000, 1, -1, 0x3F800000, 0x40490FDB,
                                  0x1FFFF, (int)0xFFFE0000 };
    int i, j;

    o->nret = 0;
    for (i = 0; i < 8; i++)
    {
        int a = packs[i] ^ (int)seed;
        int b = packs[(i + 3) & 7] ^ (int)(seed * 3);
        float f = vals[(i + (int)seed) & 7];

        s->SetScale(&a, f);
        o->ret[o->nret++] = (unsigned int)a;
        s->SetSpeed(&a, f);
        o->ret[o->nret++] = (unsigned int)a;
        s->SetPos(&b, f);
        o->ret[o->nret++] = (unsigned int)b;
        s->SetRotation(&a, &b, f);
        o->ret[o->nret++] = (unsigned int)a;
        o->ret[o->nret++] = (unsigned int)b;
        for (j = 0; j < 8; j++)
        {
            float g;
            g = s->GetSpeed(packs[j] ^ (int)seed);
            o->ret[o->nret++] = *(unsigned int*)&g;
            g = s->GetScale(packs[j] ^ (int)seed);
            o->ret[o->nret++] = *(unsigned int*)&g;
            g = s->GetRotation(packs[j] ^ (int)seed, (unsigned int)packs[(j + 5) & 7]);
            o->ret[o->nret++] = *(unsigned int*)&g;
        }
    }
    W.transformA = packs[seed & 7];
    W.transformB = packs[(seed + 1) & 7];
    s->SetRotation(&W.transformA, &W.transformB, vals[seed & 7]);
}

static void Case_Predicates(const tSide* s, tShot* o, unsigned int seed)
{
    int i;

    o->nret = 0;
    for (i = 0; i < MAXMAT; i++)
    {
        o->ret[o->nret++] = (unsigned int)s->IsMaterialTransparent(&W.mats[i], (int)seed % 5,
                                                                  W.dd.drawMaskAllowedStuffCurrent, NULL);
        o->ret[o->nret++] = (unsigned int)s->IsObjectTransparent(&W.mats[i]);
        o->ret[o->nret++] = (unsigned int)s->MaterialsHaveAlphaBlending(&W.mats[i]);
    }
    o->ret[o->nret++] = (unsigned int)s->IsMaterialTransparent(NULL, 0, 0, NULL);
    for (i = 0; i < MAXTEX; i++)
    {
        o->ret[o->nret++] = (unsigned int)s->IsLastElemInMultiTexture(&W.dd, &W.tex[i]);
        o->ret[o->nret++] = (unsigned int)s->VUISIdentity(&W.tex[i]);
    }
    for (i = 0; i < MAXMAT; i++)
    {
        o->ret[o->nret++] = s->GetValidityMASK(&W.mats[i], -1, &W.geogro);
        o->ret[o->nret++] = s->GetValidityMASK(&W.mats[i], (int)(seed % 4), &W.geogro);
        o->ret[o->nret++] = s->GetValidityMASK(&W.mats[i], -1, NULL);
    }
    o->ret[o->nret++] = s->GetValidityMASK(NULL, -1, &W.geogro);
    o->ret[o->nret++] = s->GetValidityMASK(&W.nullMaskMat, -1, &W.geogro);
}

static void Case_SetRenderState(const tSide* s, tShot* o, unsigned int seed)
{
    int i;

    o->nret = 0;
    for (i = 0; i < MAXMAT; i++)
        s->SetRenderState(&W.mats[i], (int)((seed + i) % 5));
    s->SetRenderState(NULL, (int)(seed % 3));
}

static void Case_DrawTriangleST(const tSide* s, tShot* o, unsigned int seed)
{
    o->nret = 0;
    s->DrawIndexedTriangle_ST(&W.dd, &W.geogro, &W.mats[1 + (seed % 3)],
                              &W.chunks[seed % MAXCHUNK]);
}

static void Case_DrawTriangleMT(const tSide* s, tShot* o, unsigned int seed)
{
    o->nret = 0;
    s->DrawIndexedTriangle_MT(&W.dd, &W.geogro, &W.mats[1 + (seed % 3)],
                              &W.chunks[seed % MAXCHUNK]);
}

static void Case_DrawTriangle(const tSide* s, tShot* o, unsigned int seed)
{
    o->nret = 0;
    s->DrawIndexedTriangle(&W.geogro, &W.mats[seed % MAXMAT], &W.chunks[seed % MAXCHUNK]);
    s->DrawIndexedTriangle(&W.geogro, NULL, &W.chunks[(seed + 1) % MAXCHUNK]);
}

static void Case_DrawSprites(const tSide* s, tShot* o, unsigned int seed)
{
    o->nret = 0;
    W.sprites[0].num = 3;
    s->DrawIndexedSprites(&W.geogro, &W.mats[seed % MAXMAT], &W.sprites[0]);
    s->DrawIndexedSprites_MT(&W.dd, &W.geogro, &W.mats[1 + (seed % 3)], &W.sprites[0]);
    s->DrawIndexedSprites(&W.geogro, NULL, &W.sprites[1]);
}

static void Case_XmnRender(const tSide* s, tShot* o, unsigned int seed)
{
    o->nret = 0;
    MDMOCK_nodeFailEvery = (seed & 1) ? 5 : 0;
    if (seed & 0x80000)
        W.geogro.UVs = NULL;
    s->XmnRender(&W.geogro, &W.mats[seed % MAXMAT]);
}

static void Case_GeoRender(const tSide* s, tShot* o, unsigned int seed)
{
    o->nret = 0;
    MDMOCK_nodeFailEvery = (seed & 2) ? 4 : 0;
    if (seed & 0x80000)
        W.geogro.UVs = NULL;
    s->GeoRender(&W.dd, &W.geogro, &W.visual);
    if (seed & 4)
    {
        W.geogro.vertexNum = 5001;
        s->GeoRender(&W.dd, &W.geogro, &W.visual);
        W.geogro.vertexNum = MAXV;
    }
}

static void Case_UvMatrix(const tSide* s, tShot* o, unsigned int seed)
{
    unsigned int mode;
    int partIdx, elemNum, shape;

    o->nret = 0;
    for (mode = 0; mode < 4; mode++)
    {
        unsigned int uv = (mode << 6) | (((seed >> 16) & 7) << 8);
        memset(&W.outMtx, 0, sizeof(W.outMtx));
        s->UvGetMatrix(&W.dd, &W.gao, uv, &W.outMtx);
        o->ret[o->nret++] = *(unsigned int*)&W.outMtx.v1.x;
        o->ret[o->nret++] = *(unsigned int*)&W.outMtx.v2.y;
        o->ret[o->nret++] = *(unsigned int*)&W.outMtx.v3.z;
        o->ret[o->nret++] = *(unsigned int*)&W.outMtx.Pos.x;
    }
    /* mode 3 reaches the multipart arms only while the target object
       carries 0x200000, which the seed-built world never sets, so every
       shape below is set here rather than drawn from the seed */
    for (shape = 0; shape < 32; shape++)
    for (partIdx = 0; partIdx < 8; partIdx++)
    for (elemNum = partIdx; elemNum <= partIdx + 2; elemNum++)
    {
        unsigned int uv = (3u << 6) | ((unsigned int)partIdx << 8);
        int i;

        W.gao.flags = 0x200000 | ((shape & 1) ? 0x1000000 : 0);
        W.partGao.flags = (((shape >> 4) & 1) ? 0x200000 : 0)
                        | (((shape >> 1) & 1) ? 0x1000000 : 0);
        W.multiPart.elemNum = elemNum;
        W.multiPart.multipartGaoObjectPartsOrMatrices =
            (shape & 1) ? (void*)W.links : (void*)W.partMatrices;
        for (i = 0; i < 8; i++)
        {
            W.links[i].gao = ((shape >> 2) & 1) ? &W.partGao : NULL;
            W.links[i].mtxPtr = ((shape >> 3) & 1) ? &W.partMatrices[i] : NULL;
        }
        memset(&W.outMtx, 0, sizeof(W.outMtx));
        s->UvGetMatrix(&W.dd, &W.gao, uv, &W.outMtx);
        o->ret[o->nret++] = *(unsigned int*)&W.outMtx.v1.x;
        o->ret[o->nret++] = *(unsigned int*)&W.outMtx.v2.y;
        o->ret[o->nret++] = *(unsigned int*)&W.outMtx.v3.z;
        o->ret[o->nret++] = *(unsigned int*)&W.outMtx.Pos.x;
        o->ret[o->nret++] = *(unsigned int*)&W.outMtx.v1.z;
    }
}

/* ---------- driver ---------- */

typedef void (*fnCase)(const tSide*, tShot*, unsigned int);

#ifdef MD_COVERAGE
#define MD_COVN 32
static const char* covName[MD_COVN];
static long covCount[MD_COVN];
static void MD_Tally(void)
{
    int i, k;
    for (i = 0; i < logLen[0]; i++)
    {
        for (k = 0; k < MD_COVN; k++)
        {
            if (!covName[k]) { covName[k] = logCopy[0][i].fn; covCount[k] = 1; break; }
            if (!strcmp(covName[k], logCopy[0][i].fn)) { covCount[k]++; break; }
        }
    }
}
static void MD_DumpCoverage(void)
{
    int k;
    for (k = 0; k < MD_COVN && covName[k]; k++)
        printf("  %-24s %ld\n", covName[k], covCount[k]);
}
#endif

static void Run(const char* label, fnCase fn, unsigned int seed)
{
    char name[96];
    int side;

    sprintf(name, "%s/%u", label, seed);
    for (side = 0; side < 2; side++)
    {
        BuildWorld(seed);
        MDMOCK_Reset();
        fn(&sides[side], &shot[side], seed);
        TakeShot(&shot[side]);
        memcpy(logCopy[side], MDMOCK_log, sizeof(MDMOCK_log));
        logLen[side] = MDMOCK_logLen;
    }
#ifdef MD_COVERAGE
    MD_Tally();
#endif
    CompareCase(name);
}

int main(void)
{
    unsigned int seed;

    W.bb = (tBigBufferStruct*)calloc(1, sizeof(tBigBufferStruct));
    if (!W.bb)
        return 2;
    /* the engine keeps a tMultiTextureElemStruct* in an int field, so the
       pool has to sit where a 32-bit truncation round-trips */
    W.tex = (tMultiTextureElemStruct*)mmap(NULL, 4096, PROT_READ | PROT_WRITE,
                                           MAP_PRIVATE | MAP_ANONYMOUS | MAP_32BIT, -1, 0);
    if (W.tex == MAP_FAILED)
    {
        printf("matdraw: no low memory for the texture pool\n");
        return 2;
    }
    MATH_InitModule();
    BuildWorld(1);
    RegisterPointers();

    for (seed = 0; seed < 96; seed++)
    {
        Run("bits", Case_BitPacking, seed);
        Run("predicates", Case_Predicates, seed);
        Run("renderstate", Case_SetRenderState, seed);
        Run("draw_st", Case_DrawTriangleST, seed);
        Run("draw_mt", Case_DrawTriangleMT, seed);
        Run("draw", Case_DrawTriangle, seed);
        Run("sprites", Case_DrawSprites, seed);
        Run("xmn", Case_XmnRender, seed);
        Run("geo", Case_GeoRender, seed);
        Run("uvmtx", Case_UvMatrix, seed);
    }
    /* seeds above everything used by the sweep, driving the bits the
       sweep's low-order seed never reaches */
    for (seed = 0x10000; seed < 0x10000 + 64; seed++)
    {
        unsigned int s2 = seed | ((seed & 0x3F) << 12) | 0x20000 | 0x40000;
        Run("draw_mt_hi", Case_DrawTriangleMT, s2);
        Run("draw_hi", Case_DrawTriangle, s2);
        Run("sprites_hi", Case_DrawSprites, s2);
        Run("uvmtx_hi", Case_UvMatrix, s2 | 0x80000);
        Run("predicates_hi", Case_Predicates, s2);
    }

#ifdef MD_COVERAGE
    MD_DumpCoverage();
#endif
    printf("matdraw: %d cases, %d values, %d engine calls compared, %d differing\n",
           cases, valuesCompared, callsCompared, failures);
    if (MDMOCK_overflow)
    {
        printf("matdraw: CALL LOG OVERFLOWED -- raise MDMOCK_LOG_MAX\n");
        return 1;
    }
    return failures ? 1 : 0;
}
