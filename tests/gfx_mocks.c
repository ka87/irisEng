#include <math.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "gfx_mocks.h"
#include "../lights.h"

tGfxMockCall GFXMOCK_log[GFXMOCK_LOG_MAX];
int GFXMOCK_logLen;
int GFXMOCK_logOverflow;

tGfxCapture GFXMOCK_caps[GFXMOCK_CAP_MAX];
int GFXMOCK_capCount;

tGfxElem* GFXMOCK_elem;

void GFXMOCK_Record(const char* fn, int nargs, ...)
{
    va_list ap;
    int i;

    if (GFXMOCK_logLen >= GFXMOCK_LOG_MAX)
    {
        GFXMOCK_logOverflow = 1;
        return;
    }

    GFXMOCK_log[GFXMOCK_logLen].fn = fn;
    GFXMOCK_log[GFXMOCK_logLen].nargs = nargs > GFXMOCK_CALL_ARGS ? GFXMOCK_CALL_ARGS : nargs;
    va_start(ap, nargs);
    for (i = 0; i < GFXMOCK_log[GFXMOCK_logLen].nargs; i++)
        GFXMOCK_log[GFXMOCK_logLen].arg[i] = (float)va_arg(ap, double);
    va_end(ap);
    GFXMOCK_logLen++;
}

/* ------------------------------------------------------------------ */
/* The pseudo-random sequence. The engine scales rand() by 1/SHRT_MAX  */
/* and expects a 15-bit result; a 31-bit one puts every effect that    */
/* draws a random number into a range it never sees in the real build, */
/* which drowns the comparison in meaningless magnitudes. So the test  */
/* supplies the 15-bit generator the real build is compiled against.   */
/* ------------------------------------------------------------------ */

static unsigned long GFXMOCK_randState = 1;

int rand(void)
{
    GFXMOCK_randState = GFXMOCK_randState * 214013ul + 2531011ul;
    return (int)((GFXMOCK_randState >> 16) & 0x7FFF);
}

void srand(unsigned int seed)
{
    GFXMOCK_randState = seed;
}

/* ------------------------------------------------------------------ */
/* The allocator. Every block is remembered so a reset can hand the    */
/* second side of a case the same addresses the first side saw, which  */
/* matters because some effects store pointers into their own state.   */
/* ------------------------------------------------------------------ */

#define GFXMOCK_ALLOC_MAX 1024

static void* GFXMOCK_blocks[GFXMOCK_ALLOC_MAX];
static int GFXMOCK_blockCount;

static void GFXMOCK_l_Remember(void* ptr)
{
    if (GFXMOCK_blockCount < GFXMOCK_ALLOC_MAX)
        GFXMOCK_blocks[GFXMOCK_blockCount++] = ptr;
}

static void GFXMOCK_l_Forget(void* ptr)
{
    int i;

    for (i = 0; i < GFXMOCK_blockCount; i++)
        if (GFXMOCK_blocks[i] == ptr)
        {
            GFXMOCK_blocks[i] = NULL;
            return;
        }
}

void* MEM_p_Alloc(unsigned int size)
{
    void* ptr;

    ptr = calloc(1, size ? size : 1);
    GFXMOCK_l_Remember(ptr);
    GFXMOCK_Record("MEM_p_Alloc", 1, (double)size);
    return ptr;
}

void* MEM_p_Realloc(void* ptr, unsigned int size)
{
    void* out;

    GFXMOCK_l_Forget(ptr);
    out = realloc(ptr, size ? size : 1);
    GFXMOCK_l_Remember(out);
    GFXMOCK_Record("MEM_p_Realloc", 1, (double)size);
    return out;
}

void MEM_Free(void* ptr)
{
    GFXMOCK_l_Forget(ptr);
    free(ptr);
    GFXMOCK_Record("MEM_Free", 0);
}

void* MEM_p_AllocAligned(int size, int alignment)
{
    void* ptr;

    ptr = calloc(1, size > 0 ? (size_t)size : 1);
    GFXMOCK_l_Remember(ptr);
    return ptr;
}

void* MEM_p_ReallocAligned(void* ptr, int size, int alignment)
{
    void* out;

    GFXMOCK_l_Forget(ptr);
    out = realloc(ptr, size > 0 ? (size_t)size : 1);
    GFXMOCK_l_Remember(out);
    return out;
}

void MEM_FreeAlign(void* ptr)
{
    GFXMOCK_l_Forget(ptr);
    free(ptr);
}

/* ------------------------------------------------------------------ */
/* The shared geometry buffer and the display data.                    */
/* ------------------------------------------------------------------ */

static tGEOGRO GFXMOCK_geo;
static tGroIntf GFXMOCK_intf;
static tGeoTriangleChunks GFXMOCK_firstChunk;
static tGdi_DisplayData GFXMOCK_dd;
static tGameObjectMain GFXMOCK_gao;
static tGameObjectBasic GFXMOCK_gaoBasic;
static tGfxElem GFXMOCK_elemStorage;
static Mtx4x4 GFXMOCK_gaoMatrix;

#define GFXMOCK_RASTERS 64
static profilerRasterStructElem GFXMOCK_rasters[GFXMOCK_RASTERS];

static void GFXMOCK_l_Capture(struct sGameObjectMain* go);

static void GFXMOCK_l_SetProjectionMatrix(tDDCamStruct* cam)
{
    GFXMOCK_Record("SetProjectionMatrix", 1, (double)cam->focale);
}

void GFXMOCK_SetDistance(int distance)
{
    GFXMOCK_gao.distance = (uint8_t)distance;
}

void GFXMOCK_SetCameraPos(float x, float y, float z)
{
    GFXMOCK_dd.ct.cam.cameraMtx.Pos.x = x;
    GFXMOCK_dd.ct.cam.cameraMtx.Pos.y = y;
    GFXMOCK_dd.ct.cam.cameraMtx.Pos.z = z;
}

void GFXMOCK_Init(void)
{
    MATH_InitModule();

    GFXMOCK_intf.Render = GFXMOCK_l_Capture;
    GFXMOCK_geo.intf = &GFXMOCK_intf;
    GFXMOCK_geo.triangleChunks = &GFXMOCK_firstChunk;

    GFXMOCK_gaoBasic.actorObjectOrVisualObject = NULL;
    GFXMOCK_gao.basic = &GFXMOCK_gaoBasic;
    GFXMOCK_gao.matrices = &GFXMOCK_gaoMatrix;
    MATH_SetIdentityMatrix(&GFXMOCK_gaoMatrix);
    GFXMOCK_gao.distance = 1;

    GFXMOCK_elemStorage.aiGameObject = &GFXMOCK_gao;
    GFXMOCK_elem = &GFXMOCK_elemStorage;

    MATH_SetIdentityMatrix(&GFXMOCK_dd.ct.cam.cameraMtx);
    MATH_SetIdentityMatrix(&GFXMOCK_dd.ct.cam.objectMtx);
    MATH_SetIdentityMatrix(&GFXMOCK_dd.ct.cam.projectionMtx);
    GFXMOCK_dd.ct.cam.focale = 1.5f;
    GFXMOCK_dd.ct.cam.width = 640.0f;
    GFXMOCK_dd.ct.cam.height = 480.0f;
    GFXMOCK_dd.ct.cam.nearPlane = 0.1f;
    GFXMOCK_dd.ct.cam.farPlane = 1000.0f;
    GFXMOCK_dd.SetProjectionMatrix = GFXMOCK_l_SetProjectionMatrix;
    GFXMOCK_dd.drawMaskAllowedStuffCurrent = 0x7FFFFFFF;

    GFXMOCK_dd.rasters = GFXMOCK_rasters;
    gpst_DX_stDD = &GFXMOCK_dd;
    GFX_gpst_Geo = &GFXMOCK_geo;
    GFX_gpst_GO = &GFXMOCK_gao;
    GFX_gpst_Current = GFXMOCK_elem;

    {
        int i;

        for (i = 0; i < 256; i++)
            LIGHT_gaul_LinearTable[i] = i;
        LIGHT_gpul_CurrentTable = LIGHT_gaul_LinearTable;
    }
}

void GFXMOCK_Reset(void)
{
    int i;

    for (i = 0; i < GFXMOCK_capCount; i++)
    {
        free(GFXMOCK_caps[i].verts);
        free(GFXMOCK_caps[i].uvs);
        free(GFXMOCK_caps[i].tris);
        free(GFXMOCK_caps[i].colors);
    }
    memset(GFXMOCK_caps, 0, sizeof(GFXMOCK_caps));
    GFXMOCK_capCount = 0;

    GFXMOCK_logLen = 0;
    GFXMOCK_logOverflow = 0;

    for (i = 0; i < GFXMOCK_blockCount; i++)
        free(GFXMOCK_blocks[i]);
    memset(GFXMOCK_blocks, 0, sizeof(GFXMOCK_blocks));
    GFXMOCK_blockCount = 0;

    memset(&GFXMOCK_geo, 0, sizeof(GFXMOCK_geo));
    memset(&GFXMOCK_firstChunk, 0, sizeof(GFXMOCK_firstChunk));
    GFXMOCK_geo.intf = &GFXMOCK_intf;
    GFXMOCK_geo.triangleChunks = &GFXMOCK_firstChunk;
    GFX_gpst_Geo = &GFXMOCK_geo;

    GFX_pul_RLI = NULL;
    GFX_ul_MaxRLI = 0;
    GFX_ul_MaxPoints = 0;
    GFX_ul_MaxUVs = 0;
    GFX_ul_MaxElements = 0;
    GFX_ul_MaxElemSprites = 0;
    memset(GFX_aul_MaxTriangles, 0, sizeof(GFX_aul_MaxTriangles));
    memset(GFX_aul_MaxSprites, 0, sizeof(GFX_aul_MaxSprites));

    GFXMOCK_dd.drawMaskAllowedStuffCurrent = 0x7FFFFFFF;
    memset(GFXMOCK_rasters, 0, sizeof(GFXMOCK_rasters));
    GFXMOCK_dd.rasters = GFXMOCK_rasters;
    GFXMOCK_gao.distance = 1;
    MATH_SetIdentityMatrix(&GFXMOCK_gaoMatrix);
    MATH_SetIdentityMatrix(&GFXMOCK_dd.ct.cam.cameraMtx);
    MATH_SetIdentityMatrix(&GFXMOCK_dd.ct.cam.objectMtx);
    MATH_SetIdentityMatrix(&GFXMOCK_dd.ct.cam.projectionMtx);
    GFXMOCK_dd.ct.cam.focale = 1.5f;

    srand(12345);
}

static void GFXMOCK_l_Capture(struct sGameObjectMain* go)
{
    tGfxCapture* cap;
    unsigned int total;
    unsigned int at;
    unsigned int i;

    GFXMOCK_Record("Render", 0);
    if (GFXMOCK_capCount >= GFXMOCK_CAP_MAX)
        return;

    cap = &GFXMOCK_caps[GFXMOCK_capCount++];
    cap->vertexNum = GFX_gpst_Geo->vertexNum;
    cap->uvNum = GFX_gpst_Geo->uvNum;
    cap->chunkCount = GFX_gpst_Geo->triangleChunkCount;
    cap->spritesNum = GFX_gpst_Geo->spritesNum;
    cap->drawMask = gpst_DX_stDD->drawMaskAllowedStuffCurrent;
    cap->hasColors = GFX_gpst_Geo->vertexColors != NULL;

    total = 0;
    for (i = 0; i < cap->chunkCount && i < GFXMOCK_CAP_CHUNKS; i++)
    {
        cap->triangleNum[i] = GFX_gpst_Geo->triangleChunks[i].triangleNum;
        cap->materialNum[i] = GFX_gpst_Geo->triangleChunks[i].materialNum;
        total += cap->triangleNum[i];
    }
    cap->triangleTotal = total;

    if (cap->vertexNum && GFX_gpst_Geo->vertices)
    {
        cap->verts = (Vec3*)malloc(sizeof(Vec3) * cap->vertexNum);
        memcpy(cap->verts, GFX_gpst_Geo->vertices, sizeof(Vec3) * cap->vertexNum);
    }
    if (cap->uvNum && GFX_gpst_Geo->UVs)
    {
        cap->uvs = (tGeoUVElem*)malloc(sizeof(tGeoUVElem) * cap->uvNum);
        memcpy(cap->uvs, GFX_gpst_Geo->UVs, sizeof(tGeoUVElem) * cap->uvNum);
    }
    if (total)
    {
        cap->tris = (tGeoTriangleChunkElem*)malloc(sizeof(tGeoTriangleChunkElem) * total);
        at = 0;
        for (i = 0; i < cap->chunkCount && i < GFXMOCK_CAP_CHUNKS; i++)
        {
            if (cap->triangleNum[i] && GFX_gpst_Geo->triangleChunks[i].triangleChunkElems)
                memcpy(&cap->tris[at], GFX_gpst_Geo->triangleChunks[i].triangleChunkElems,
                       sizeof(tGeoTriangleChunkElem) * cap->triangleNum[i]);
            at += cap->triangleNum[i];
        }
    }
    if (cap->hasColors && cap->vertexNum)
    {
        cap->colors = (unsigned int*)malloc(sizeof(unsigned int) * (cap->vertexNum + 1));
        memcpy(cap->colors, GFX_gpst_Geo->vertexColors,
               sizeof(unsigned int) * (cap->vertexNum + 1));
    }
}

/* ------------------------------------------------------------------ */
/* Verbatim copies of gfx.c, because the pre-refactor code spells the  */
/* quad helpers out inline and the rewritten code calls them.          */
/* ------------------------------------------------------------------ */

void GFX_NeedGeomEx(unsigned int pointsNum, unsigned int UVNum, unsigned int triangleChunkNum, unsigned int triangleNum, int useVertexColors)
{
    int* v5;
    int* v6;
    unsigned int v7;
    unsigned int v8;
    tGeoTriangleChunks* v9;
    int v10;

    if (pointsNum && pointsNum <= 5000)
    {
        if (useVertexColors)
        {
            if (GFX_ul_MaxRLI < pointsNum)
            {
                if (GFX_pul_RLI)
                    v5 = (int*)MEM_p_Realloc(GFX_pul_RLI, 4 * pointsNum + 4);
                else
                    v5 = (int*)MEM_p_Alloc(4 * pointsNum + 4);
                GFX_pul_RLI = v5;
                GFX_ul_MaxRLI = pointsNum;
            }
            *GFX_pul_RLI = pointsNum;
        }
        if (GFX_gpst_GO)
        {
            v6 = GFX_pul_RLI;
            GFX_gpst_Geo->spritesNum = 0;
            GFX_gpst_Geo->vertexColors = (unsigned int*)(useVertexColors != 0 ? (void*)v6 : (void*)0);
            if (pointsNum > GFX_ul_MaxPoints)
            {
                MEM_FreeAlign(GFX_gpst_Geo->vertices);
                GFX_gpst_Geo->vertices = (Vec3*)MEM_p_AllocAligned(sizeof(Vec3) * pointsNum, 16);
                MEM_FreeAlign(GFX_gpst_Geo->normals);
                GFX_gpst_Geo->normals = (Vec3*)MEM_p_AllocAligned(sizeof(Vec3) * pointsNum, 16);
                GFX_ul_MaxPoints = pointsNum;
            }
            v7 = GFX_ul_MaxUVs;
            GFX_gpst_Geo->vertexNum = pointsNum;
            if (UVNum > v7)
            {
                MEM_FreeAlign(GFX_gpst_Geo->UVs);
                GFX_gpst_Geo->UVs = (tGeoUVElem*)MEM_p_AllocAligned(sizeof(tGeoUVElem) * UVNum, 8);
                GFX_ul_MaxUVs = UVNum;
            }
            GFX_gpst_Geo->uvNum = UVNum;
            if (triangleNum > GFX_aul_MaxTriangles[0])
            {
                MEM_FreeAlign(GFX_gpst_Geo->triangleChunks->triangleChunkElems);
                GFX_gpst_Geo->triangleChunks->triangleChunkElems = (tGeoTriangleChunkElem*)MEM_p_AllocAligned(
                    sizeof(tGeoTriangleChunkElem) * triangleNum,
                    16);
                GFX_aul_MaxTriangles[0] = triangleNum;
            }
            v8 = GFX_ul_MaxElements;
            GFX_gpst_Geo->triangleChunks->triangleNum = triangleNum;
            if (triangleChunkNum > v8)
            {
                if (v8)
                {
                    v9 = (tGeoTriangleChunks*)MEM_p_Realloc(GFX_gpst_Geo->triangleChunks, sizeof(tGeoTriangleChunks) * triangleChunkNum);
                    v10 = GFX_ul_MaxElements;
                    GFX_gpst_Geo->triangleChunks = v9;
                    memset(&GFX_gpst_Geo->triangleChunks[v10], 0, sizeof(tGeoTriangleChunks) * (triangleChunkNum - v10));
                    GFX_ul_MaxElements = triangleChunkNum;
                    GFX_gpst_Geo->triangleChunkCount = triangleChunkNum;
                    return;
                }
                GFX_gpst_Geo->triangleChunks = (tGeoTriangleChunks*)MEM_p_Alloc(sizeof(tGeoTriangleChunks) * triangleChunkNum);
                memcpy(GFX_gpst_Geo->triangleChunks, &GFXMOCK_firstChunk, sizeof(tGeoTriangleChunks));
                memset(&GFX_gpst_Geo->triangleChunks[1], 0,
                       sizeof(tGeoTriangleChunks) * (triangleChunkNum - 1));
                GFX_ul_MaxElements = triangleChunkNum;
            }
            GFX_gpst_Geo->triangleChunkCount = triangleChunkNum;
        }
    }
}

void GFX_NeedGeom(unsigned int pointsNum, unsigned int UVNum, unsigned int triangleNum, int useVertexColors)
{
    GFX_NeedGeomEx(pointsNum, UVNum, 1, triangleNum, useVertexColors);
}

void GFX_SetQuadIndices(void)
{
    tGeoTriangleChunkElem* tris;

    tris = GFX_gpst_Geo->triangleChunks->triangleChunkElems;
    tris[0].v1 = 0;
    tris[0].v2 = 1;
    tris[0].v3 = 2;
    tris[0].uv1 = 0;
    tris[0].uv2 = 1;
    tris[0].uv3 = 2;
    tris[1].v1 = 2;
    tris[1].v2 = 3;
    tris[1].v3 = 0;
    tris[1].uv1 = 2;
    tris[1].uv2 = 3;
    tris[1].uv3 = 0;
}

void GFX_SetQuadUVs(void)
{
    tGeoUVElem* uvs;

    uvs = GFX_gpst_Geo->UVs;
    uvs[0].u = 0.0;
    uvs[0].v = 0.0;
    uvs[1].u = 1.0;
    uvs[1].v = 0.0;
    uvs[2].u = 1.0;
    uvs[2].v = 1.0;
    uvs[3].u = 0.0;
    uvs[3].v = 1.0;
}

void GFX_RenderGeom(unsigned int maskBitsToClear)
{
    int savedMask;

    savedMask = gpst_DX_stDD->drawMaskAllowedStuffCurrent;
    gpst_DX_stDD->drawMaskAllowedStuffCurrent = savedMask & ~maskBitsToClear;
    GFX_gpst_Geo->intf->Render(GFX_gpst_GO);
    gpst_DX_stDD->drawMaskAllowedStuffCurrent = savedMask;
}

void GFX_SetNumberOfSprites(unsigned int num)
{
    GFX_gpst_Geo->spritesNum = num;
    GFXMOCK_Record("GFX_SetNumberOfSprites", 1, (double)num);
}

void GFX_SetCurrent(struct sGfxElem* elem, int16_t id)
{
    GFXMOCK_Record("GFX_SetCurrent", 1, (double)id);
}

void GFX_Del(tGfxElem** list, int16_t id)
{
    GFXMOCK_Record("GFX_Del", 1, (double)id);
}

int GFX_i_Request(int type)
{
    GFXMOCK_Record("GFX_i_Request", 1, (double)type);
    return 1;
}

/* ------------------------------------------------------------------ */
/* Verbatim copies of lights.c: the colour blends are byte tricks on   */
/* the float bits, so a stand-in would not do.                         */
/* ------------------------------------------------------------------ */

unsigned int LIGHT_ul_Interpol2Colors(int color1, int color2, float factor)
{
    unsigned int tmpColor;
    unsigned int colorToAdd;
    float hax;

    if (factor >= 0.98000002f)
        return color2;
    if (factor <= 0.02f)
        return color1;

    hax = (float)(factor + 49152.0);
    tmpColor = color2;
    if (((*(char*)(&hax)) & 0x80) == 0)
        tmpColor = color1;
    colorToAdd = ((tmpColor >> 1) & 0x7f7f7f7f);
    tmpColor = color2;
    if (((*(char*)(&hax)) & 0x40) == 0)
        tmpColor = color1;
    colorToAdd = ((tmpColor >> 2) & 0x3f3f3f3f) + colorToAdd;
    tmpColor = color2;
    if (((*(char*)(&hax)) & 0x20) == 0)
        tmpColor = color1;
    colorToAdd = ((tmpColor >> 3) & 0x1f1f1f1f) + colorToAdd;
    tmpColor = color2;
    if (((*(char*)(&hax)) & 0x10) == 0)
        tmpColor = color1;
    colorToAdd = ((tmpColor >> 4) & 0x0f0f0f0f) + colorToAdd;
    tmpColor = color2;
    if (((*(char*)(&hax)) & 8) == 0)
        tmpColor = color1;
    colorToAdd = ((tmpColor >> 5) & 0x07070707) + colorToAdd;
    tmpColor = color2;
    if (((*(char*)(&hax)) & 4) == 0)
        tmpColor = color1;
    colorToAdd = ((tmpColor >> 6) & 0x03030303) + colorToAdd;
    if (((*(char*)(&hax)) & 2) != 0)
        tmpColor = ((unsigned int)color2 >> 7) & 0x01010101;
    else
        tmpColor = ((unsigned int)color1 >> 7) & 0x01010101;
    return tmpColor + colorToAdd;
}

int LIGHT_ul_MulColor(int color1, float factor)
{
    int ret;
    int colorFromTable;
    float hax;

    if (factor >= 0.98000002f)
        return color1;
    if (factor <= 0.02f)
        return 0;
    hax = (float)(factor + 49152.0);
    colorFromTable = LIGHT_gpul_CurrentTable[(*(char*)(&hax)) & 0xff];
    ret = 0;
    if ((colorFromTable & 0x80) != 0)
        ret = ((unsigned int)color1 >> 1) & 0x7f7f7f7f;
    if ((colorFromTable & 0x40) != 0)
        ret += ((unsigned int)color1 >> 2) & 0x3f3f3f3f;
    if ((colorFromTable & 0x20) != 0)
        ret += ((unsigned int)color1 >> 3) & 0x1f1f1f1f;
    if ((colorFromTable & 0x10) != 0)
        ret += ((unsigned int)color1 >> 4) & 0xf0f0f0f;
    if ((colorFromTable & 8) != 0)
        ret += ((unsigned int)color1 >> 5) & 0x7070707;
    return ret;
}

/* ------------------------------------------------------------------ */
/* Deterministic stand-ins. Both copies call these the same way, so    */
/* they only have to be reproducible, not real.                        */
/* ------------------------------------------------------------------ */

Mtx4x4* OBJ_pst_GetAbsoluteMatrix(tGameObjectMain* gao)
{
    return gao->matrices;
}

void CAM_2Dto3D(struct sDDCamStruct* cam, Vec3* out, Vec3* in)
{
    out->x = in->x * 0.5f + 1.0f;
    out->y = in->y * 0.25f - 2.0f;
    out->z = in->z * 2.0f + 0.125f;
    GFXMOCK_Record("CAM_2Dto3D", 3, (double)in->x, (double)in->y, (double)in->z);
}

void CAM_2Dto3DCamera2(struct sDDCamStruct* cam, Vec3* out, Vec3* in)
{
    out->x = in->x * 0.25f - 1.0f;
    out->y = in->y * 0.5f + 2.0f;
    out->z = in->z * 1.5f - 0.25f;
    GFXMOCK_Record("CAM_2Dto3DCamera2", 3, (double)in->x, (double)in->y, (double)in->z);
}

void CAM_AssignCameraToDevice(tDDContainerStruct2ForIDA* ct2, tDDContainerStruct* ct,
                              int winWidth, int winHeight)
{
    GFXMOCK_Record("CAM_AssignCameraToDevice", 2, (double)winWidth, (double)winHeight);
}

void SOFT_Project(Vec3* out, Vec3* in, int idx, tDDContainerStruct* ct)
{
    out->x = in->x * 0.75f + 320.0f;
    out->y = in->y * -0.75f + 240.0f;
    out->z = in->z * 0.5f;
    GFXMOCK_Record("SOFT_Project", 4, (double)in->x, (double)in->y, (double)in->z, (double)idx);
}

void SOFT_TransformAndProject(Vec3* out, Vec3* in, int idx, tDDCamStruct* cam)
{
    out->x = in->x * 0.5f + 160.0f;
    out->y = in->y * -0.5f + 120.0f;
    out->z = in->z * 0.25f + 1.0f;
    GFXMOCK_Record("SOFT_TransformAndProject", 4, (double)in->x, (double)in->y, (double)in->z, (double)idx);
}

/* ------------------------------------------------------------------ */
/* Comparison.                                                         */
/* ------------------------------------------------------------------ */

int GFXMOCK_fails;
int GFXMOCK_diffCount;

static char GFXMOCK_caseName[160];
static char GFXMOCK_worstAbsSite[160];
static char GFXMOCK_worstRelSite[160];
static double GFXMOCK_worstAbs;
static double GFXMOCK_worstAbsRel;
static double GFXMOCK_worstRel;
static double GFXMOCK_worstRelAbs;

static tGfxMockCall GFXMOCK_sideLog[2][GFXMOCK_LOG_MAX];
static int GFXMOCK_sideLogLen[2];
static tGfxCapture GFXMOCK_sideCaps[2][GFXMOCK_CAP_MAX];
static int GFXMOCK_sideCapCount[2];

void GFXMOCK_SetCase(const char* name)
{
    snprintf(GFXMOCK_caseName, sizeof(GFXMOCK_caseName), "%s", name);
}

void GFXMOCK_SetCameraBasis(float* v1, float* v2, float* v3)
{
    memcpy(&GFXMOCK_dd.ct.cam.objectMtx.v1, v1, sizeof(Vec3));
    memcpy(&GFXMOCK_dd.ct.cam.objectMtx.v2, v2, sizeof(Vec3));
    memcpy(&GFXMOCK_dd.ct.cam.objectMtx.v3, v3, sizeof(Vec3));
}

static void GFXMOCK_l_FreeSide(int side)
{
    int i;

    for (i = 0; i < GFXMOCK_sideCapCount[side]; i++)
    {
        free(GFXMOCK_sideCaps[side][i].verts);
        free(GFXMOCK_sideCaps[side][i].uvs);
        free(GFXMOCK_sideCaps[side][i].tris);
        free(GFXMOCK_sideCaps[side][i].colors);
    }
    memset(GFXMOCK_sideCaps[side], 0, sizeof(GFXMOCK_sideCaps[side]));
    GFXMOCK_sideCapCount[side] = 0;
    GFXMOCK_sideLogLen[side] = 0;
}

void GFXMOCK_SaveSide(int side)
{
    int i;

    GFXMOCK_l_FreeSide(side);

    memcpy(GFXMOCK_sideLog[side], GFXMOCK_log, sizeof(tGfxMockCall) * GFXMOCK_logLen);
    GFXMOCK_sideLogLen[side] = GFXMOCK_logLen;

    for (i = 0; i < GFXMOCK_capCount; i++)
    {
        GFXMOCK_sideCaps[side][i] = GFXMOCK_caps[i];
        GFXMOCK_caps[i].verts = NULL;
        GFXMOCK_caps[i].uvs = NULL;
        GFXMOCK_caps[i].tris = NULL;
        GFXMOCK_caps[i].colors = NULL;
    }
    GFXMOCK_sideCapCount[side] = GFXMOCK_capCount;
}

int GFXMOCK_Close(float base, float New, const char* what, int idx)
{
    double d, r, m;

    if (memcmp(&base, &New, sizeof(float)) == 0)
        return 1;

    d = fabs((double)base - (double)New);
    m = fabs((double)base) > fabs((double)New) ? fabs((double)base) : fabs((double)New);
    r = m > 0.0 ? d / m : d;

    /* A NaN on one side only is a real difference, never a rounding one. */
    if ((base != base) != (New != New))
    {
        printf("    DIFF %s | %s[%d]: old %.9g  new %.9g  (NaN on one side)\n",
               GFXMOCK_caseName, what, idx, (double)base, (double)New);
        return 0;
    }
    if (base != base)
        return 1;

    GFXMOCK_diffCount++;
    if (d > GFXMOCK_worstAbs)
    {
        GFXMOCK_worstAbs = d;
        GFXMOCK_worstAbsRel = r;
        snprintf(GFXMOCK_worstAbsSite, sizeof(GFXMOCK_worstAbsSite),
                 "%s %s[%d]", GFXMOCK_caseName, what, idx);
    }
    if (r > GFXMOCK_worstRel)
    {
        GFXMOCK_worstRel = r;
        GFXMOCK_worstRelAbs = d;
        snprintf(GFXMOCK_worstRelSite, sizeof(GFXMOCK_worstRelSite),
                 "%s %s[%d]", GFXMOCK_caseName, what, idx);
    }
    if (r > 1e-5 && d > 1e-5)
    {
        printf("    DIFF %s | %s[%d]: old %.9g  new %.9g  (rel %.3g)\n",
               GFXMOCK_caseName, what, idx, (double)base, (double)New, r);
        return 0;
    }
    return 1;
}

int GFXMOCK_CloseVec(Vec3* base, Vec3* New, const char* what)
{
    int ok = 1;

    ok &= GFXMOCK_Close(base->x, New->x, what, 0);
    ok &= GFXMOCK_Close(base->y, New->y, what, 1);
    ok &= GFXMOCK_Close(base->z, New->z, what, 2);
    return ok;
}

int GFXMOCK_SameInt(int base, int New, const char* what, int idx)
{
    if (base == New)
        return 1;
    printf("    DIFF %s | %s[%d]: old %d (0x%08X)  new %d (0x%08X)\n",
           GFXMOCK_caseName, what, idx, base, (unsigned)base, New, (unsigned)New);
    return 0;
}

static int GFXMOCK_l_CompareCapture(int which, tGfxCapture* a, tGfxCapture* b)
{
    char what[96];
    unsigned int i;
    int ok = 1;

    snprintf(what, sizeof(what), "cap%d.vertexNum", which);
    ok &= GFXMOCK_SameInt((int)a->vertexNum, (int)b->vertexNum, what, 0);
    snprintf(what, sizeof(what), "cap%d.uvNum", which);
    ok &= GFXMOCK_SameInt((int)a->uvNum, (int)b->uvNum, what, 0);
    snprintf(what, sizeof(what), "cap%d.chunkCount", which);
    ok &= GFXMOCK_SameInt((int)a->chunkCount, (int)b->chunkCount, what, 0);
    snprintf(what, sizeof(what), "cap%d.spritesNum", which);
    ok &= GFXMOCK_SameInt((int)a->spritesNum, (int)b->spritesNum, what, 0);
    snprintf(what, sizeof(what), "cap%d.drawMask", which);
    ok &= GFXMOCK_SameInt((int)a->drawMask, (int)b->drawMask, what, 0);
    snprintf(what, sizeof(what), "cap%d.hasColors", which);
    ok &= GFXMOCK_SameInt(a->hasColors, b->hasColors, what, 0);
    if (!ok)
        return 0;

    for (i = 0; i < a->chunkCount && i < GFXMOCK_CAP_CHUNKS; i++)
    {
        snprintf(what, sizeof(what), "cap%d.triangleNum", which);
        ok &= GFXMOCK_SameInt((int)a->triangleNum[i], (int)b->triangleNum[i], what, (int)i);
        snprintf(what, sizeof(what), "cap%d.materialNum", which);
        ok &= GFXMOCK_SameInt(a->materialNum[i], b->materialNum[i], what, (int)i);
    }
    if (!ok)
        return 0;

    if (a->verts && b->verts)
        for (i = 0; i < a->vertexNum; i++)
        {
            snprintf(what, sizeof(what), "cap%d.vert%u", which, i);
            ok &= GFXMOCK_CloseVec(&a->verts[i], &b->verts[i], what);
        }
    if (a->uvs && b->uvs)
        for (i = 0; i < a->uvNum; i++)
        {
            snprintf(what, sizeof(what), "cap%d.uv%u.u", which, i);
            ok &= GFXMOCK_Close(a->uvs[i].u, b->uvs[i].u, what, 0);
            snprintf(what, sizeof(what), "cap%d.uv%u.v", which, i);
            ok &= GFXMOCK_Close(a->uvs[i].v, b->uvs[i].v, what, 0);
        }
    if (a->tris && b->tris)
        for (i = 0; i < a->triangleTotal; i++)
        {
            snprintf(what, sizeof(what), "cap%d.tri%u", which, i);
            if (memcmp(&a->tris[i], &b->tris[i], sizeof(tGeoTriangleChunkElem)) != 0)
            {
                printf("    DIFF %s | %s: old (%u,%u,%u / %u,%u,%u)  new (%u,%u,%u / %u,%u,%u)\n",
                       GFXMOCK_caseName, what,
                       a->tris[i].v1, a->tris[i].v2, a->tris[i].v3,
                       a->tris[i].uv1, a->tris[i].uv2, a->tris[i].uv3,
                       b->tris[i].v1, b->tris[i].v2, b->tris[i].v3,
                       b->tris[i].uv1, b->tris[i].uv2, b->tris[i].uv3);
                ok = 0;
            }
        }
    if (a->colors && b->colors)
        for (i = 0; i <= a->vertexNum; i++)
        {
            snprintf(what, sizeof(what), "cap%d.color", which);
            ok &= GFXMOCK_SameInt((int)a->colors[i], (int)b->colors[i], what, (int)i);
        }
    return ok;
}

int GFXMOCK_CompareSides(void)
{
    int ok = 1;
    int i, j, n;

    if (GFXMOCK_logOverflow)
        printf("    NOTE %s: call log overflowed\n", GFXMOCK_caseName);

    if (GFXMOCK_sideLogLen[0] != GFXMOCK_sideLogLen[1])
    {
        printf("    DIFF %s: call count old %d new %d\n", GFXMOCK_caseName,
               GFXMOCK_sideLogLen[0], GFXMOCK_sideLogLen[1]);
        ok = 0;
    }
    n = GFXMOCK_sideLogLen[0] < GFXMOCK_sideLogLen[1]
        ? GFXMOCK_sideLogLen[0] : GFXMOCK_sideLogLen[1];
    for (i = 0; i < n; i++)
    {
        tGfxMockCall* a = &GFXMOCK_sideLog[0][i];
        tGfxMockCall* b = &GFXMOCK_sideLog[1][i];

        if (strcmp(a->fn, b->fn) != 0 || a->nargs != b->nargs)
        {
            printf("    DIFF %s: call %d old %s/%d new %s/%d\n", GFXMOCK_caseName,
                   i, a->fn, a->nargs, b->fn, b->nargs);
            ok = 0;
            continue;
        }
        for (j = 0; j < a->nargs; j++)
        {
            char what[96];

            snprintf(what, sizeof(what), "call%d %s arg", i, a->fn);
            ok &= GFXMOCK_Close(a->arg[j], b->arg[j], what, j);
        }
    }

    if (GFXMOCK_sideCapCount[0] != GFXMOCK_sideCapCount[1])
    {
        printf("    DIFF %s: render count old %d new %d\n", GFXMOCK_caseName,
               GFXMOCK_sideCapCount[0], GFXMOCK_sideCapCount[1]);
        ok = 0;
    }
    n = GFXMOCK_sideCapCount[0] < GFXMOCK_sideCapCount[1]
        ? GFXMOCK_sideCapCount[0] : GFXMOCK_sideCapCount[1];
    for (i = 0; i < n; i++)
        ok &= GFXMOCK_l_CompareCapture(i, &GFXMOCK_sideCaps[0][i], &GFXMOCK_sideCaps[1][i]);

    if (!ok)
    {
        printf("  FAIL %s\n", GFXMOCK_caseName);
        GFXMOCK_fails++;
    }
    return ok;
}

void GFXMOCK_Summary(const char* title)
{
    printf("%s\n", title);
    if (GFXMOCK_diffCount)
    {
        printf("  %d float(s) differ within tolerance\n", GFXMOCK_diffCount);
        printf("    worst abs %g (rel %.3g) at %s\n",
               GFXMOCK_worstAbs, GFXMOCK_worstAbsRel, GFXMOCK_worstAbsSite);
        printf("    worst rel %g (abs %.3g) at %s\n",
               GFXMOCK_worstRel, GFXMOCK_worstRelAbs, GFXMOCK_worstRelSite);
    }
    else
        printf("  every value bit-identical\n");

    if (GFXMOCK_fails)
        printf("  %d case(s) differ\n", GFXMOCK_fails);
    else
        printf("  no behavioural differences\n");
}
