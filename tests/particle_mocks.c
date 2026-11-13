#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <math.h>

#include "particle_mocks.h"
#include "../math.h"
#include "../memory.h"
#include "../soft.h"

tPMockCall PMOCK_log[PMOCK_LOG_MAX];
int PMOCK_logLen;
int PMOCK_logOverflow;
tPMockCapture PMOCK_capture;

int PMOCK_cases;
int PMOCK_failures;
long PMOCK_compares;
long PMOCK_floatsDiffering;
double PMOCK_worstAbs;
double PMOCK_worstRel;
double PMOCK_worstRelAbs;
float PMOCK_tolAbs = 0.0f;
float PMOCK_tolRel = 0.0f;

static const char* PMOCK_case = "?";
static int PMOCK_reported;

void PMOCK_SetCase(const char* name)
{
    PMOCK_case = name;
    PMOCK_reported = 0;
    PMOCK_cases++;
}

static void PMOCK_Fail(void)
{
    PMOCK_reported = 1;
    PMOCK_failures++;
}

static void PMOCK_Record(const char* fn, int ni, int nf, ...)
{
    va_list ap;
    int i;
    tPMockCall* c;

    if (PMOCK_logLen >= PMOCK_LOG_MAX)
    {
        PMOCK_logOverflow = 1;
        return;
    }
    c = &PMOCK_log[PMOCK_logLen++];
    c->fn = fn;
    c->ni = ni;
    c->nf = nf;
    va_start(ap, nf);
    for (i = 0; i < ni; i++)
        c->ia[i] = va_arg(ap, int);
    for (i = 0; i < nf; i++)
        c->fa[i] = (float)va_arg(ap, double);
    va_end(ap);
}

/* ---- the pseudo-random sequence -------------------------------------
   MATH_RandFloat scales rand() by 1/SHRT_MAX, which assumes a 15-bit
   rand; under glibc's 31-bit one every drawn value lands in a range
   the real build never reaches. */

static unsigned long PMOCK_randState = 1;

int rand(void)
{
    PMOCK_randState = PMOCK_randState * 214013ul + 2531011ul;
    PMOCK_Record("rand", 0, 0);
    return (int)((PMOCK_randState >> 16) & 0x7FFF);
}

void srand(unsigned int seed)
{
    PMOCK_randState = seed;
}

void PMOCK_Seed(unsigned int seed)
{
    PMOCK_randState = seed;
}

int RND_JADErand(void)
{
    PMOCK_randState = PMOCK_randState * 214013ul + 2531011ul;
    PMOCK_Record("RND_JADErand", 0, 0);
    return (int)((PMOCK_randState >> 16) & 0x7FFF);
}

/* ---- the allocator math.c's matrix helpers want ---------------------- */

void* MEM_p_Alloc(unsigned int size) { return calloc(1, size ? size : 1); }
void* MEM_p_Realloc(void* ptr, size_t size) { return realloc(ptr, size ? size : 1); }
void MEM_Free(void* ptr) { free(ptr); }
void* MEM_p_AllocAligned(int size, int alignment) { (void)alignment; return calloc(1, size ? size : 1); }
void* MEM_p_ReallocAligned(void* ptr, unsigned int size, int alignment) { (void)alignment; return realloc(ptr, size ? size : 1); }
void MEM_FreeAlign(void* ptr) { free(ptr); }

/* ---- the object tree ------------------------------------------------- */

tGameObjectMain PMOCK_gao;
tGameObjectMain PMOCK_genGao;
tGameObjectMain PMOCK_shapeGao;
Mtx4x4 PMOCK_gaoMtx;
Mtx4x4 PMOCK_genMtx;
Mtx4x4 PMOCK_shapeMtx;
tGEOGRO PMOCK_gro;
tGEOGRO* PMOCK_groForGao1;
int PMOCK_groId = 1;
int PMOCK_haveStp;
int PMOCK_stpHasStp;

static tGroIntf PMOCK_groIntf;
static tGameObjectBasic PMOCK_gaoBasic;
static tGameObjectVisualObject PMOCK_visual;
static tGameObjectBasic PMOCK_gfxBasic;
static tGameObjectVisualObject PMOCK_gfxVisual;
static tGameObjectMain PMOCK_gfxGao;
static tGroGeneric PMOCK_gromat;

static Vec3 PMOCK_groVerts[64];
static Vec3 PMOCK_groNormals[64];
static Vec3 PMOCK_stpVerts[64];
static tStpMdfStruct PMOCK_stp;
static tModifier PMOCK_stpMdf;

struct sGroGeneric* OBJ_p_GetGro(tGameObjectMain* gao)
{
    PMOCK_Record("OBJ_p_GetGro", 1, 0, gao == &PMOCK_shapeGao ? 1 : 0);
    if (!PMOCK_groForGao1)
        return NULL;
    PMOCK_gro.intf = &PMOCK_groIntf;
    PMOCK_groIntf.id = PMOCK_groId;
    return (struct sGroGeneric*)PMOCK_groForGao1;
}

Mtx4x4* OBJ_pst_GetAbsoluteMatrix(tGameObjectMain* gao)
{
    int which = 0;

    if (gao == &PMOCK_genGao)
        which = 1;
    else if (gao == &PMOCK_shapeGao)
        which = 2;
    PMOCK_Record("OBJ_pst_GetAbsoluteMatrix", 1, 0, which);
    if (which == 1)
        return &PMOCK_genMtx;
    if (which == 2)
        return &PMOCK_shapeMtx;
    return &PMOCK_gaoMtx;
}

tModifier* MDF_pst_GetByType(tGameObjectMain* gao, int type)
{
    (void)gao;
    PMOCK_Record("MDF_pst_GetByType", 1, 0, type);
    if (!PMOCK_haveStp)
        return NULL;
    PMOCK_stp.hasStp = PMOCK_stpHasStp;
    PMOCK_stp.num = 64;
    PMOCK_stp.verticesStored = PMOCK_stpVerts;
    PMOCK_stpMdf.mdfPrivateDataPtr = &PMOCK_stp;
    return &PMOCK_stpMdf;
}

void GEO_UseNormals(struct sGEOGRO* geo)
{
    PMOCK_Record("GEO_UseNormals", 1, 0, geo != NULL);
}

/* COLOR_ul_Mul stands in for lights.c's own. It is not a copy of it:
   what the test needs is a deterministic result that moves whenever
   the factor moves, so any change to how the fade factor is computed
   reaches the colour array. The factor itself is compared exactly
   through the call log. */
uint32_t COLOR_ul_Mul(uint32_t color, float mult)
{
    PMOCK_Record("COLOR_ul_Mul", 1, 1, (int)color, (double)mult);
    return (color ^ (unsigned int)(mult * 16777216.0f)) & 0x00FFFFFFu;
}

/* ---- the display data ------------------------------------------------ */

static tGdi_DisplayData PMOCK_dd;
static profilerRasterStructElem PMOCK_rasters[16];
static Mtx4x4 PMOCK_viewMtx;
static Mtx4x4 PMOCK_stackMtx[8];

static int PMOCK_SetViewMatrix(Mtx4x4* mtx)
{
    PMOCK_Record("SetViewMatrix", 1, 0, mtx == &PMOCK_dd.ct.cam.cameraMtx ? 1 : 0);
    return 0;
}

void SOFT_AddCurrentObjectInZList(struct sGdi_DisplayData* dd, struct sGameObjectMain* gao)
{
    (void)dd;
    (void)gao;
    PMOCK_Record("SOFT_AddCurrentObjectInZList", 0, 0);
}

/* ---- the shared geometry buffer -------------------------------------- */

static tGEOGRO PMOCK_geo;
static tGeoTriangleChunks PMOCK_chunk;
static tGeoSprites PMOCK_sprites;
static Vec3 PMOCK_bufVerts[PMOCK_VERTS];
static Vec3 PMOCK_bufNormals[PMOCK_VERTS];
static tGeoUVElem PMOCK_bufUVs[PMOCK_UVS];
static tGeoTriangleChunkElem PMOCK_bufTris[PMOCK_TRIS];
static int PMOCK_bufColors[PMOCK_VERTS + 1];
static tGeoSpriteElem PMOCK_bufSprites[PMOCK_SPRITES];
static tGroIntf PMOCK_geoIntf;

static void PMOCK_Capture(struct sGameObjectMain* go)
{
    unsigned int n;

    (void)go;
    PMOCK_Record("Render", 0, 0);
    PMOCK_capture.taken = 1;
    PMOCK_capture.vertexNum = PMOCK_geo.vertexNum;
    PMOCK_capture.uvNum = PMOCK_geo.uvNum;
    PMOCK_capture.spritesNum = PMOCK_geo.spritesNum;
    PMOCK_capture.drawMask = PMOCK_dd.drawMaskAllowedStuffCurrent;
    PMOCK_capture.materialNum = PMOCK_chunk.materialNum;
    PMOCK_capture.triangleNum = PMOCK_chunk.triangleNum;

    n = PMOCK_geo.vertexNum;
    if (n > PMOCK_VERTS)
        n = PMOCK_VERTS;
    memcpy(PMOCK_capture.verts, PMOCK_bufVerts, sizeof(Vec3) * n);
    memcpy(PMOCK_capture.colors, PMOCK_bufColors, sizeof(int) * (n + 1));

    n = PMOCK_geo.uvNum;
    if (n > PMOCK_UVS)
        n = PMOCK_UVS;
    memcpy(PMOCK_capture.uvs, PMOCK_bufUVs, sizeof(tGeoUVElem) * n);

    n = PMOCK_chunk.triangleNum;
    if (n > PMOCK_TRIS)
        n = PMOCK_TRIS;
    memcpy(PMOCK_capture.tris, PMOCK_bufTris, sizeof(tGeoTriangleChunkElem) * n);

    n = PMOCK_sprites.num;
    if (n > PMOCK_SPRITES)
        n = PMOCK_SPRITES;
    memcpy(PMOCK_capture.sprites, PMOCK_bufSprites, sizeof(tGeoSpriteElem) * n);
}

void GFX_NeedGeomEx(unsigned int pointsNum, unsigned int UVNum, unsigned int triangleChunkNum,
                    unsigned int triangleNum, int useVertexColors)
{
    PMOCK_Record("GFX_NeedGeomEx", 5, 0, (int)pointsNum, (int)UVNum, (int)triangleChunkNum,
                 (int)triangleNum, useVertexColors);
    if (pointsNum > PMOCK_VERTS || UVNum > PMOCK_UVS || triangleNum > PMOCK_TRIS)
    {
        fprintf(stderr, "geometry request past the mock buffers\n");
        abort();
    }
    /* The engine keeps the point count in the first colour slot and
       hands the array out from there, which is why every writer
       indexes the colours from 1. */
    PMOCK_bufColors[0] = (int)pointsNum;
    PMOCK_geo.vertexColors = useVertexColors ? (unsigned int*)PMOCK_bufColors : NULL;
    PMOCK_geo.vertexNum = pointsNum;
    PMOCK_geo.uvNum = UVNum;
    PMOCK_geo.spritesNum = 0;
    PMOCK_geo.triangleChunkCount = triangleChunkNum;
    PMOCK_chunk.triangleNum = triangleNum;
}

void GFX_NeedGeom(unsigned int pointsNum, unsigned int UVNum, unsigned int triangleNum,
                  int useVertexColors)
{
    PMOCK_Record("GFX_NeedGeom", 4, 0, (int)pointsNum, (int)UVNum, (int)triangleNum,
                 useVertexColors);
    GFX_NeedGeomEx(pointsNum, UVNum, 1, triangleNum, useVertexColors);
}

void GFX_NeedGeomNbElemSprites(unsigned int num)
{
    PMOCK_Record("GFX_NeedGeomNbElemSprites", 1, 0, (int)num);
    PMOCK_geo.spritesNum = num;
    PMOCK_geo.spritesPtr = &PMOCK_sprites;
}

void GFX_NeedGeomElemNbSprites(unsigned int idx, unsigned int num)
{
    PMOCK_Record("GFX_NeedGeomElemNbSprites", 2, 0, (int)idx, (int)num);
    if (num > PMOCK_SPRITES)
    {
        fprintf(stderr, "sprite request past the mock buffer\n");
        abort();
    }
    PMOCK_sprites.num = num;
    PMOCK_sprites.spriteElems = PMOCK_bufSprites;
}

/* Not logged on purpose: the rewrite calls this where the frozen copy
   writes the four UVs out, so logging it would make the two sides
   differ by construction. What is compared is the UV array it left
   behind. */
void GFX_SetQuadUVs(void)
{
    tGeoUVElem* uvs = PMOCK_geo.UVs;

    uvs[0].u = 0.0;
    uvs[0].v = 0.0;
    uvs[1].u = 1.0;
    uvs[1].v = 0.0;
    uvs[2].u = 1.0;
    uvs[2].v = 1.0;
    uvs[3].u = 0.0;
    uvs[3].v = 1.0;
}

/* ---- setup and teardown ---------------------------------------------- */

void PMOCK_Init(void)
{
    int i;

    MATH_InitModule();

    PMOCK_groIntf.id = 1;
    PMOCK_gro.intf = &PMOCK_groIntf;
    PMOCK_gro.vertexNum = 64;
    PMOCK_gro.vertices = PMOCK_groVerts;
    PMOCK_gro.normals = PMOCK_groNormals;

    for (i = 0; i < 64; i++)
    {
        PMOCK_groVerts[i].x = (float)(i % 7) - 3.0f;
        PMOCK_groVerts[i].y = (float)(i % 5) - 2.0f;
        PMOCK_groVerts[i].z = (float)(i % 11) - 5.0f;
        PMOCK_stpVerts[i].x = PMOCK_groVerts[i].x + 0.25f;
        PMOCK_stpVerts[i].y = PMOCK_groVerts[i].y - 0.5f;
        PMOCK_stpVerts[i].z = PMOCK_groVerts[i].z + 0.125f;
        PMOCK_groNormals[i].x = (float)((i % 3) - 1) * 0.5f;
        PMOCK_groNormals[i].y = (float)((i % 4) - 2) * 0.25f;
        PMOCK_groNormals[i].z = 1.0f - (float)(i % 2);
    }

    MATH_SetIdentityMatrix(&PMOCK_gaoMtx);
    MATH_SetIdentityMatrix(&PMOCK_genMtx);
    MATH_SetIdentityMatrix(&PMOCK_shapeMtx);
    MATH_SetIdentityMatrix(&PMOCK_viewMtx);

    PMOCK_visual.GROMAT = &PMOCK_gromat;
    PMOCK_gaoBasic.actorObjectOrVisualObject = &PMOCK_visual;
    PMOCK_gao.basic = &PMOCK_gaoBasic;
    PMOCK_gao.matrices = &PMOCK_gaoMtx;
    PMOCK_gao.distance = 1;
    PMOCK_genGao.matrices = &PMOCK_genMtx;
    PMOCK_shapeGao.matrices = &PMOCK_shapeMtx;

    PMOCK_gfxBasic.actorObjectOrVisualObject = &PMOCK_gfxVisual;
    PMOCK_gfxGao.basic = &PMOCK_gfxBasic;

    MATH_SetIdentityMatrix(&PMOCK_dd.ct.cam.cameraMtx);
    MATH_SetIdentityMatrix(&PMOCK_dd.ct.cam.objectMtx);
    PMOCK_dd.rasters = PMOCK_rasters;
    PMOCK_dd.SetViewMatrix = PMOCK_SetViewMatrix;
    PMOCK_dd.ct.cam.viewMtxPtr_setInGRORender = &PMOCK_viewMtx;
    PMOCK_dd.matrixStack.elems = PMOCK_stackMtx;
    PMOCK_dd.matrixStack.MaxElemNum = 8;

    PMOCK_geoIntf.id = 1;
    PMOCK_geoIntf.Render = PMOCK_Capture;
    PMOCK_geo.intf = &PMOCK_geoIntf;
    PMOCK_geo.triangleChunks = &PMOCK_chunk;
    PMOCK_geo.vertices = PMOCK_bufVerts;
    PMOCK_geo.normals = PMOCK_bufNormals;
    PMOCK_geo.UVs = PMOCK_bufUVs;
    PMOCK_geo.spritesPtr = &PMOCK_sprites;
    PMOCK_chunk.triangleChunkElems = PMOCK_bufTris;
    PMOCK_sprites.spriteElems = PMOCK_bufSprites;

    gpst_DX_stDD = &PMOCK_dd;
    GFX_gpst_Geo = &PMOCK_geo;
    GFX_gpst_GO = &PMOCK_gfxGao;
    TIM_gf_dt = 0.0f;
}

void PMOCK_Reset(void)
{
    PMOCK_logLen = 0;
    PMOCK_logOverflow = 0;
    memset(&PMOCK_capture, 0, sizeof(PMOCK_capture));

    memset(PMOCK_bufVerts, 0, sizeof(PMOCK_bufVerts));
    memset(PMOCK_bufUVs, 0, sizeof(PMOCK_bufUVs));
    memset(PMOCK_bufTris, 0, sizeof(PMOCK_bufTris));
    memset(PMOCK_bufColors, 0, sizeof(PMOCK_bufColors));
    memset(PMOCK_bufSprites, 0, sizeof(PMOCK_bufSprites));

    PMOCK_geo.vertexNum = 0;
    PMOCK_geo.uvNum = 0;
    PMOCK_geo.spritesNum = 0;
    PMOCK_geo.vertexColors = NULL;
    PMOCK_chunk.materialNum = 0;
    PMOCK_chunk.triangleNum = 0;
    PMOCK_sprites.num = 0;
    PMOCK_sprites.matIdx = 0;
    PMOCK_sprites.globalSize = 0.0f;
    PMOCK_sprites.globalRatio = 0.0f;

    PMOCK_dd.flags = 0;
    PMOCK_dd.drawMaskAllowedStuffCurrent = 0x7FFFFFFF;
    PMOCK_dd.matrixStack.currElemNum = 3;
    PMOCK_dd.matrixStack.actMatrixElem = &PMOCK_viewMtx;
    PMOCK_dd.ct.cam.viewMtxPtr_setInGRORender = &PMOCK_viewMtx;
    memset(PMOCK_rasters, 0, sizeof(PMOCK_rasters));
    /* Not zero, so a render whose generator has not run this frame
       takes the PAG_Update/PAG_Generate path rather than skipping it. */
    PMOCK_rasters[4].unk1 = 7;
    PMOCK_gfxVisual.GROMAT = NULL;
}

/* ---- comparison ------------------------------------------------------- */

void PMOCK_SaveLog(tPMockCall* dest, int* destLen)
{
    memcpy(dest, PMOCK_log, sizeof(tPMockCall) * PMOCK_logLen);
    *destLen = PMOCK_logLen;
}

void PMOCK_SaveCapture(tPMockCapture* dest)
{
    memcpy(dest, &PMOCK_capture, sizeof(tPMockCapture));
}

int PMOCK_SameInt(int base, int New, const char* what, const char* where)
{
    PMOCK_compares++;
    if (base == New)
        return 0;
    if (!PMOCK_reported)
    {
        PMOCK_Fail();
        printf("  [%s] %s %s: base %d new %d\n", PMOCK_case, where, what, base, New);
    }
    return 1;
}

int PMOCK_CloseFloat(float base, float New, const char* what, const char* where)
{
    double diff;
    double rel;
    double mag;

    PMOCK_compares++;
    if (memcmp(&base, &New, sizeof(float)) == 0)
        return 0;
    if (isnan(base) && isnan(New))
        return 0;
    PMOCK_floatsDiffering++;
    diff = fabs((double)base - (double)New);
    mag = fabs((double)base) > fabs((double)New) ? fabs((double)base) : fabs((double)New);
    rel = mag > 0.0 ? diff / mag : 0.0;
    if (diff > PMOCK_worstAbs)
        PMOCK_worstAbs = diff;
    if (rel > PMOCK_worstRel)
    {
        PMOCK_worstRel = rel;
        PMOCK_worstRelAbs = diff;
    }
    if (diff <= PMOCK_tolAbs || rel <= PMOCK_tolRel)
        return 0;
    if (!PMOCK_reported)
    {
        PMOCK_Fail();
        printf("  [%s] %s %s: base %.9g new %.9g (abs %.3g rel %.3g)\n",
               PMOCK_case, where, what, (double)base, (double)New, diff, rel);
    }
    return 1;
}

int PMOCK_CompareLog(const tPMockCall* a, int alen, const tPMockCall* b, int blen,
                     const char* where)
{
    int i;
    int j;

    PMOCK_compares++;
    if (alen != blen)
    {
        if (!PMOCK_reported)
        {
            PMOCK_Fail();
            printf("  [%s] %s call count: base %d new %d\n", PMOCK_case, where, alen, blen);
            for (i = 0; i < alen && i < blen; i++)
                if (a[i].fn != b[i].fn)
                {
                    printf("      first differing call %d: base %s new %s\n", i, a[i].fn, b[i].fn);
                    break;
                }
        }
        return 1;
    }
    for (i = 0; i < alen; i++)
    {
        if (a[i].fn != b[i].fn || a[i].ni != b[i].ni || a[i].nf != b[i].nf)
        {
            if (!PMOCK_reported)
            {
                PMOCK_Fail();
                printf("  [%s] %s call %d: base %s new %s\n", PMOCK_case, where, i,
                       a[i].fn, b[i].fn);
            }
            return 1;
        }
        for (j = 0; j < a[i].ni; j++)
            if (a[i].ia[j] != b[i].ia[j])
            {
                if (!PMOCK_reported)
                {
                    PMOCK_Fail();
                    printf("  [%s] %s call %d (%s) int arg %d: base %d new %d\n",
                           PMOCK_case, where, i, a[i].fn, j, a[i].ia[j], b[i].ia[j]);
                }
                return 1;
            }
        for (j = 0; j < a[i].nf; j++)
            if (PMOCK_CloseFloat(a[i].fa[j], b[i].fa[j], "float arg", where))
                return 1;
    }
    return 0;
}

void PMOCK_Summary(const char* title)
{
    printf("%s: %d cases, %ld comparisons, %d failing\n",
           title, PMOCK_cases, PMOCK_compares, PMOCK_failures);
    printf("  floats differing: %ld", PMOCK_floatsDiffering);
    if (PMOCK_floatsDiffering)
        printf("   worst absolute %.3g   worst relative %.3g (abs %.3g)",
               PMOCK_worstAbs, PMOCK_worstRel, PMOCK_worstRelAbs);
    printf("\n");
}
