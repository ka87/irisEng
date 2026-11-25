/* Just enough of the engine for graphicsobjects/geo.c to load a
 * geometry from a buffer.
 *
 * Most of what geo.c references belongs to its render paths, which this
 * driver does not go near; those are stubs. The allocators and the four
 * buffer reads are real, and the allocators are the C library's so that
 * --asan beside this sees the sizes geo.c actually asks for.
 */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "../common.h"
#include "../math.h"
#include "../loading.h"
#include "../graphicsobjects/gro_common.h"
#include "../graphicsobjects/geo.h"
#include "../graphicsobjects/geomaterial.h"
#include "../profiler.h"

#include "geo_mocks.h"

/* ------------------------------------------------------------ the heap */

int GEOMOCK_liveBlocks;

static void* take(unsigned int size)
{
    ++GEOMOCK_liveBlocks;
    return malloc(size ? size : 1);
}

static void give(void* ptr)
{
    if (ptr)
        --GEOMOCK_liveBlocks;
    free(ptr);
}

void* MEM_p_Alloc(unsigned int size)          { return take(size); }
void* MEM_GEO_p_Alloc(unsigned int size)      { return take(size); }
void* MEM_p_AllocFromEnd(int size)            { return take((unsigned)size); }
void* MEM_GEO_p_AllocAlign(int size, int a)   { (void)a; return take((unsigned)size); }
void* MEM_p_AllocAligned(int size, int a)     { (void)a; return take((unsigned)size); }
void MEM_Free(void* p)                        { give(p); }
void MEM_FreeAlign(void* p)                   { give(p); }
void MEM_FreeFromEnd(void* p)                 { give(p); }

/* ------------------------------------------------------- the read head */

int LOA_ReadLong(char** offset, uint32_t* retVal, char readMode)
{
    int32_t v;

    memcpy(&v, *offset, 4);
    *offset += 4;
    if (readMode == 1 && retVal)
        *retVal = (uint32_t)v;
    return v;
}

uint16_t LOA_ReadShort(char** offset, uint16_t* retVal, char readMode)
{
    uint16_t v;

    memcpy(&v, *offset, 2);
    *offset += 2;
    if (readMode == 1 && retVal)
        *retVal = v;
    return v;
}

float LOA_ReadFloat(char** offset, float* retVal, char readMode)
{
    float v;

    memcpy(&v, *offset, 4);
    *offset += 4;
    if (readMode == 1 && retVal)
        *retVal = v;
    return v;
}

void LOA_ReadVector(char** offset, Vec3* retVal, int readMode)
{
    Vec3 v;

    memcpy(&v, *offset, 12);
    *offset += 12;
    if (readMode == 1 && retVal)
        *retVal = v;
}

void LOA_ReadMatrix(char** offset, Mtx4x4* retVal, int readMode)
{
    Mtx4x4 m;

    memcpy(&m, *offset, sizeof(m));
    *offset += sizeof(m);
    if (readMode == 1 && retVal)
        *retVal = m;
}

void LOA_DeleteAddress(int pos)             { (void)pos; }

/* ------------------------------------------------- the object interface */

tGroIntf GRO_gast_Interface[12];

void GRO_Struct_Init(tGroGeneric* gro, int type)
{
    gro->intf = &GRO_gast_Interface[type];
    gro->usageCount = 0;
}

tGEOMAT MAT_gst_DefaultSingleMaterial;

/* ------------------------------------------- the render side, unreached */

void AI_ExecCallback(struct sGameObjectMain* gao, int id) { (void)gao; (void)id; }
struct sGameObjectMain* ANI_pst_GetReferenceInit(struct sGameObjectMain* gao)
{ return gao; }
uint32_t COLOR_ul_Mul(uint32_t c, float m) { (void)m; return c; }
void GAO_ModifierLazy_Apply(tModifier* mdf, void* u) { (void)mdf; (void)u; }
void GEO_Render_TransparentTriangles(struct sGdi_DisplayData* dd,
                                     struct sGEOGRO* g,
                                     struct sGameObjectVisualObject* v)
{ (void)dd; (void)g; (void)v; }
uint32_t HDL_ul_Handle(void* p) { return (uint32_t)(uintptr_t)p; }
void LIGHT_ComputeVertexColor(int n) { (void)n; }
void LIGHT_SendObjectToLight(struct sLightListStruct* l,
                             struct sGameObjectMain* g) { (void)l; (void)g; }
void LIGHT_SubColor(unsigned int* r, int c) { (void)r; (void)c; }
unsigned int LIGHT_ul_Interpol2Colors(int a, int b, float f)
{ (void)b; (void)f; return (unsigned int)a; }
void MATH_CrossProduct(Vec3* d, Vec3* a, Vec3* b) { (void)d; (void)a; (void)b; }
void MATH_InvertMatrix(Mtx4x4* d, Mtx4x4* s) { (void)d; (void)s; }
void MATH_MulMatrixMatrix(Mtx4x4* d, Mtx4x4* a, Mtx4x4* b)
{ (void)d; (void)a; (void)b; }
void MATH_NormalizeEqualVector(Vec3* v) { (void)v; }
void MATH_SetIdentityMatrix(Mtx4x4* m) { (void)m; }
void MATH_SubstractVec(Vec3* d, Vec3* a, Vec3* b) { (void)d; (void)a; (void)b; }
void MATH_TransformVertex(Vec3* d, Mtx4x4* m, Vec3* s)
{ (void)d; (void)m; (void)s; }
void MATH_VecAddVecWithoutBuffer(Vec3* d, Vec3* s) { (void)d; (void)s; }
void MAT_DrawIndexedSprites(struct sGEOGRO* g, struct sGEOMAT* m,
                            struct sGeoSprites* s) { (void)g; (void)m; (void)s; }
void MAT_DrawIndexedTriangle(struct sGEOGRO* g, struct sGEOMAT* m,
                             struct sGeoTriangleChunks* t)
{ (void)g; (void)m; (void)t; }
unsigned int MAT_GetValidityMASK(struct sGEOMAT* m, int i, struct sGEOGRO* g)
{ (void)m; (void)i; (void)g; return 0; }
BOOL MAT_IsMaterialTransparent(struct sGEOMAT* m, int i, int f, void* u)
{ (void)m; (void)i; (void)f; (void)u; return 0; }
void MDF_ApplyAll(struct sGameObjectMain* g) { (void)g; }
void MDF_UnApplyAll(struct sGameObjectMain* g) { (void)g; }
void PROTEX_Disturb(struct sGameObjectMain* g, struct sGEOGRO* o)
{ (void)g; (void)o; }
void PRO_PerfCounterEndBlock(profilerRasterStructElem* r) { (void)r; }
void PRO_PerfCounterStartBlock(profilerRasterStructElem* r) { (void)r; }
void PRO_QPC_RecordTimeDecRef(int id) { (void)id; }
int QueryPerformanceCounter(LARGE_INTEGER* out)
{
    if (out)
        out->QuadPart = 0;
    return 1;
}
void SDW_DisplayShadowsOnCurrentObject(struct sGEOGRO* g) { (void)g; }
void SOFT_AddCurrentObjectInZList(struct sGdi_DisplayData* dd,
                                  struct sGameObjectMain* g)
{ (void)dd; (void)g; }

void GEOMOCK_Reset(void)
{
    GEOMOCK_liveBlocks = 0;
}
