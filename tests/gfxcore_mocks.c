#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "gfxcore_mocks.h"

tGfxcCall GFXC_log[GFXC_LOG_MAX];
int GFXC_logLen;
int GFXC_logOverflow;
int GFXC_renderResult = 1;

unsigned char GFXC_arena[GFXC_ARENA_BYTES];
unsigned int GFXC_arenaUsed;

tWorld GFXC_world;

static profilerRasterStructElem GFXC_rasters[64];
static tGdi_DisplayData GFXC_dd;
static tGameObjectMain GFXC_gfxGao;
static tGameObjectBasic GFXC_gfxGaoBasic;
static tGameObjectVisualObject GFXC_gfxVisual;
static struct sGroIntf GFXC_intf;
static int GFXC_tick;

/* ---- the log ---- */

unsigned long long GFXC_F(float f)
{
    unsigned int bits;
    memcpy(&bits, &f, 4);
    return bits;
}

unsigned long long GFXC_P(const void* p)
{
    return (unsigned long long)(size_t)p;
}

void GFXC_Call(const char* fn, int n, ...)
{
    va_list ap;
    int i;
    if (GFXC_logLen >= GFXC_LOG_MAX)
    {
        GFXC_logOverflow = 1;
        return;
    }
    GFXC_log[GFXC_logLen].fn = fn;
    GFXC_log[GFXC_logLen].nargs = n;
    GFXC_log[GFXC_logLen].state = gpst_DX_stDD ? gpst_DX_stDD->flags : 0;
    GFXC_log[GFXC_logLen].drawMask = gpst_DX_stDD ? gpst_DX_stDD->drawMaskAllowedStuffCurrent : 0;
    va_start(ap, n);
    for (i = 0; i < n && i < GFXC_ARGS; ++i)
        GFXC_log[GFXC_logLen].arg[i] = va_arg(ap, unsigned long long);
    va_end(ap);
    for (; i < GFXC_ARGS; ++i)
        GFXC_log[GFXC_logLen].arg[i] = 0;
    ++GFXC_logLen;
}

void GFXC_Call0(const char* fn)
{
    GFXC_Call(fn, 0);
}

/* ---- the allocator ---- */

#define GFXC_BLOCKS 4096
static void* GFXC_blockPtr[GFXC_BLOCKS];
static unsigned int GFXC_blockSize[GFXC_BLOCKS];
static int GFXC_blockCount;

static void GFXC_Remember(void* p, unsigned int size)
{
    if (GFXC_blockCount < GFXC_BLOCKS)
    {
        GFXC_blockPtr[GFXC_blockCount] = p;
        GFXC_blockSize[GFXC_blockCount] = size;
        ++GFXC_blockCount;
    }
}

static unsigned int GFXC_SizeOf(void* p)
{
    int i;
    for (i = GFXC_blockCount - 1; i >= 0; --i)
        if (GFXC_blockPtr[i] == p)
            return GFXC_blockSize[i];
    return 0;
}

static void* GFXC_Bump(unsigned int size, unsigned int align)
{
    unsigned int base;
    if (align < 8)
        align = 8;
    GFXC_arenaUsed = (GFXC_arenaUsed + align - 1u) & ~(align - 1u);
    base = GFXC_arenaUsed;
    if (base + size + 16u > (unsigned int)GFXC_ARENA_BYTES)
    {
        fprintf(stderr, "gfx mock arena exhausted\n");
        exit(2);
    }
    GFXC_arenaUsed = base + size + 16u;
    memset(GFXC_arena + base, GFXC_POISON, size + 16u);
    GFXC_Remember(GFXC_arena + base, size);
    return GFXC_arena + base;
}

void* MEM_p_Alloc(unsigned int size)
{
    void* p = GFXC_Bump(size, 8);
    GFXC_Call("MEM_p_Alloc", 2, (unsigned long long)size, GFXC_P(p));
    return p;
}

void* MEM_p_AllocAligned(unsigned int size, int align)
{
    void* p = GFXC_Bump(size, (unsigned int)align);
    GFXC_Call("MEM_p_AllocAligned", 3, (unsigned long long)size,
              (unsigned long long)align, GFXC_P(p));
    return p;
}

void* MEM_p_Realloc(void* old, unsigned int size)
{
    void* p;
    unsigned int oldSize;
    if (!old)
    {
        p = GFXC_Bump(size, 8);
    }
    else
    {
        oldSize = GFXC_SizeOf(old);
        p = GFXC_Bump(size, 8);
        memcpy(p, old, oldSize < size ? oldSize : size);
    }
    GFXC_Call("MEM_p_Realloc", 3, GFXC_P(old), (unsigned long long)size, GFXC_P(p));
    return p;
}

void* MEM_p_ReallocAligned(void* old, unsigned int size, int align)
{
    void* p;
    unsigned int oldSize;
    if (!old)
    {
        p = GFXC_Bump(size, (unsigned int)align);
    }
    else
    {
        oldSize = GFXC_SizeOf(old);
        p = GFXC_Bump(size, (unsigned int)align);
        memcpy(p, old, oldSize < size ? oldSize : size);
    }
    GFXC_Call("MEM_p_ReallocAligned", 4, GFXC_P(old), (unsigned long long)size,
              (unsigned long long)align, GFXC_P(p));
    return p;
}

/* A freed block is scrubbed rather than left readable.  Nothing is
   handed back -- both sides have to see the same addresses -- but a
   read through a pointer that has just been freed has to be visible,
   or a loop that captures its successor one statement too late reads
   the right answer and the test says nothing. */
static void Scrub(void* p)
{
    unsigned int size = GFXC_SizeOf(p);
    if (p && size)
        memset(p, 0xDE, size);
}

void MEM_Free(void* p)
{
    GFXC_Call("MEM_Free", 1, GFXC_P(p));
    Scrub(p);
}

void MEM_FreeAlign(void* p)
{
    GFXC_Call("MEM_FreeAlign", 1, GFXC_P(p));
    Scrub(p);
}

/* ---- the geometry object ---- */

static int GFXC_GroRender(struct sGameObjectMain* gao)
{
    GFXC_Call("GRO_Render", 1, GFXC_P(gao));
    return 0;
}

struct sGEOGRO* GEO_pst_Create(int vertexNum, int uvNum, int triangleChunkNum, int* vertexColors)
{
    tGEOGRO* geo = (tGEOGRO*)GFXC_Bump(sizeof(tGEOGRO), 8);
    memset(geo, 0, sizeof(tGEOGRO));
    GFXC_Call("GEO_pst_Create", 4, (unsigned long long)vertexNum,
              (unsigned long long)uvNum, (unsigned long long)triangleChunkNum,
              GFXC_P(vertexColors));
    geo->intf = &GFXC_intf;
    geo->vertexNum = vertexNum;
    geo->vertices = (Vec3*)GFXC_Bump(sizeof(Vec3) * (vertexNum ? vertexNum : 1), 16);
    geo->normals = (Vec3*)GFXC_Bump(sizeof(Vec3) * (vertexNum ? vertexNum : 1), 16);
    geo->uvNum = uvNum;
    geo->UVs = (tGeoUVElem*)GFXC_Bump(sizeof(tGeoUVElem) * (uvNum ? uvNum : 1), 8);
    geo->triangleChunkCount = triangleChunkNum;
    geo->triangleChunks = (tGeoTriangleChunks*)GFXC_Bump(
        sizeof(tGeoTriangleChunks) * (triangleChunkNum ? triangleChunkNum : 1), 8);
    geo->vertexColors = (unsigned int*)vertexColors;
    return geo;
}

void GEO_AllocElementContent(struct sGeoTriangleChunks* tc)
{
    GFXC_Call("GEO_AllocElementContent", 2, GFXC_P(tc),
              (unsigned long long)tc->triangleNum);
    tc->triangleChunkElems = (struct sGeoTriangleChunkElem*)GFXC_Bump(
        sizeof(tGeoTriangleChunkElem) * (tc->triangleNum ? tc->triangleNum : 1), 16);
}

void GEO_Free(struct sGroGeneric* geo)
{
    GFXC_Call("GEO_Free", 1, GFXC_P(geo));
}

struct sGameObjectMain* OBJ_GameObject_Create(int flags)
{
    GFXC_Call("OBJ_GameObject_Create", 1, (unsigned long long)flags);
    GFXC_gfxGao.basic = &GFXC_gfxGaoBasic;
    GFXC_gfxGaoBasic.actorObjectOrVisualObject = (struct sGameObjectActorObject*)&GFXC_gfxVisual;
    return &GFXC_gfxGao;
}

void OBJ_GameObject_Remove(struct sGameObjectMain* gao, int flag)
{
    GFXC_Call("OBJ_GameObject_Remove", 2, GFXC_P(gao), (unsigned long long)flag);
}

/* ---- timing and profiling ---- */

unsigned int TIM_GetTicksAsInt32(void)
{
    return (unsigned int)++GFXC_tick;
}

int QueryPerformanceCounter(LARGE_INTEGER* out)
{
    GFXC_Call("QueryPerformanceCounter", 1, GFXC_P(out));
    if (out)
        out->QuadPart = ++GFXC_tick;
    return 1;
}

void PRO_QPC_RecordTimeDecRef(int idx)
{
    GFXC_Call("PRO_QPC_RecordTimeDecRef", 1, (unsigned long long)idx);
}

void SOFT_ZList_Lock(void)
{
    GFXC_Call0("SOFT_ZList_Lock");
}

void SOFT_ZList_Unlock(void)
{
    GFXC_Call0("SOFT_ZList_Unlock");
}

/* ---- the effects ---- */

static void* GFXC_Blob(const char* fn)
{
    void* p = GFXC_Bump(512, 16);
    GFXC_Call(fn, 1, GFXC_P(p));
    return p;
}

#define GFXC_CREATE0(name) void* name(void) { return GFXC_Blob(#name); }
#define GFXC_CREATE1(name, t) void* name(t a) { GFXC_Call(#name "_arg", 1, GFXC_P((const void*)(size_t)a)); return GFXC_Blob(#name); }

GFXC_CREATE0(GFX_Line_Create)
GFXC_CREATE0(GFX_Lightning_Create)
GFXC_CREATE0(GFX_Smoke_Create)
GFXC_CREATE0(GFX_Table_Create)
GFXC_CREATE0(GFX_Speed_Create)
GFXC_CREATE0(GFX_Spark_Create)
GFXC_CREATE0(GFX_Water2_Create)
GFXC_CREATE0(GFX_Fade_Create)
GFXC_CREATE0(GFX_Profiler_Create)
GFXC_CREATE0(GFX_BorderFade_Create)
GFXC_CREATE0(GFX_Diaphragm_Create)
GFXC_CREATE0(GFX_Carte_Create)
GFXC_CREATE0(GFX_Sun_Create)

void* GFX_Explode_Create(void) { return GFXC_Blob("GFX_Explode_Create"); }
void* GFX_Desintegrate_Create(struct sGameObjectMain* gao)
{
    GFXC_Call("GFX_Desintegrate_Create_arg", 1, GFXC_P(gao));
    return GFXC_Blob("GFX_Desintegrate_Create");
}
void* GFX_Gao_Create(struct sGameObjectMain* gao)
{
    GFXC_Call("GFX_Gao_Create_arg", 1, GFXC_P(gao));
    return GFXC_Blob("GFX_Gao_Create");
}
void* GFX_Str_Create(struct sGameObjectMain* gao)
{
    GFXC_Call("GFX_Str_Create_arg", 1, GFXC_P(gao));
    return GFXC_Blob("GFX_Str_Create");
}
void* GFX_Ripple_Create(struct sGameObjectMain* gao)
{
    GFXC_Call("GFX_Ripple_Create_arg", 1, GFXC_P(gao));
    return GFXC_Blob("GFX_Ripple_Create");
}
void* GFX_AnotherFlare_Create(struct sGameObjectMain* gao)
{
    GFXC_Call("GFX_AnotherFlare_Create_arg", 1, GFXC_P(gao));
    return GFXC_Blob("GFX_AnotherFlare_Create");
}
void* GFX_Flare_Create(int kind)
{
    GFXC_Call("GFX_Flare_Create_arg", 1, (unsigned long long)kind);
    return GFXC_Blob("GFX_Flare_Create");
}

#define GFXC_DESTROY(name, t) void name(t p) { GFXC_Call(#name, 1, GFXC_P(p)); }
GFXC_DESTROY(GFX_Lightning_Destroy, void*)
GFXC_DESTROY(GFX_Desintegrate_Destroy, void*)
GFXC_DESTROY(GFX_Smoke_Destroy, void*)
GFXC_DESTROY(GFX_Spark_Destroy, void*)
GFXC_DESTROY(GFX_Water2_Destroy, void*)
GFXC_DESTROY(GFX_Profiler_Destroy, void*)
GFXC_DESTROY(GFX_Explode_Destroy, void*)
GFXC_DESTROY(GFX_Gao_Destroy, void*)
GFXC_DESTROY(GFX_Str_Destroy, void*)
GFXC_DESTROY(GFX_Ripple_Destroy, void*)
GFXC_DESTROY(GFX_Sun_Destroy, void*)

void GFX_Carte_Destroy(void* data, void* mat)
{
    GFXC_Call("GFX_Carte_Destroy", 2, GFXC_P(data), GFXC_P(mat));
}

#define GFXC_RENDER_I(name) int name(void* p) { GFXC_Call(#name, 1, GFXC_P(p)); return GFXC_renderResult; }
GFXC_RENDER_I(GFX_i_Lightning_Render)
GFXC_RENDER_I(GFX_i_Desintegrate_Render)
GFXC_RENDER_I(GFX_i_Smoke_Render)
GFXC_RENDER_I(GFX_i_Table_Render)
GFXC_RENDER_I(GFX_i_Speed_Render)
GFXC_RENDER_I(GFX_i_Spark_Render)
GFXC_RENDER_I(GFX_i_Water2_Render)
GFXC_RENDER_I(GFX_i_Fade_Render)
GFXC_RENDER_I(GFX_i_Profiler_Render)
GFXC_RENDER_I(GFX_i_BorderFade_Render)
GFXC_RENDER_I(GFX_i_Diaphragm_Render)
GFXC_RENDER_I(GFX_i_Str_Render)
GFXC_RENDER_I(GFX_i_Ripple_Render)
GFXC_RENDER_I(GFX_i_AnotherFlare_Render)

void GFX_Line_Render(void* p) { GFXC_Call("GFX_Line_Render", 1, GFXC_P(p)); }
void GFX_Flare_Render(void* p) { GFXC_Call("GFX_Flare_Render", 1, GFXC_P(p)); }
void GFX_Flare_Test(void* p) { GFXC_Call("GFX_Flare_Test", 1, GFXC_P(p)); }
void GFX_Sun_Render(void* p) { GFXC_Call("GFX_Sun_Render", 1, GFXC_P(p)); }
void draw_game_object_in_GFX_sub_45DBB0(void* p)
{
    GFXC_Call("draw_game_object_in_GFX_sub_45DBB0", 1, GFXC_P(p));
}
void GFX_SetRenderStateForExplode(void)
{
    GFXC_Call0("GFX_SetRenderStateForExplode");
}

int GFX_i_Explode_Render(void* p, void* mat)
{
    GFXC_Call("GFX_i_Explode_Render", 2, GFXC_P(p), GFXC_P(mat));
    return GFXC_renderResult;
}

int GFX_i_Carte_Render(void* p, void* mat, void* gao)
{
    GFXC_Call("GFX_i_Carte_Render", 3, GFXC_P(p), GFXC_P(mat), GFXC_P(gao));
    return GFXC_renderResult;
}

int GFX_i_Explode_Geti(void* p, int param)
{
    GFXC_Call("GFX_i_Explode_Geti", 2, GFXC_P(p), (unsigned long long)(unsigned int)param);
    return 0x5A000000 + param;
}

#define GFXC_SETI(name) void name(void* p, int param, int value) \
    { GFXC_Call(#name, 3, GFXC_P(p), (unsigned long long)(unsigned int)param, \
                (unsigned long long)(unsigned int)value); }
GFXC_SETI(GFX_Smoke_Seti)
GFXC_SETI(GFX_Profiler_Seti)
GFXC_SETI(GFX_Explode_Seti)
GFXC_SETI(GFX_Carte_Seti)
GFXC_SETI(GFX_Str_Seti)
GFXC_SETI(GFX_Ripple_Seti)

void GFX_Sun_Seti(void* p, int param)
{
    GFXC_Call("GFX_Sun_Seti", 2, GFXC_P(p), (unsigned long long)(unsigned int)param);
}

void GFX_Water2_Seti(void* p, int param, unsigned int offset, int value)
{
    GFXC_Call("GFX_Water2_Seti", 4, GFXC_P(p), (unsigned long long)(unsigned int)param,
              (unsigned long long)offset, (unsigned long long)(unsigned int)value);
}

#define GFXC_SETF(name) void name(void* p, int param, float value) \
    { GFXC_Call(#name, 3, GFXC_P(p), (unsigned long long)(unsigned int)param, GFXC_F(value)); }
GFXC_SETF(GFX_Profiler_Setf)
GFXC_SETF(GFX_Explode_Setf)
GFXC_SETF(GFX_Ripple_Setf)

void GFX_Water2_Setf(void* p, int param, int offset, float value)
{
    GFXC_Call("GFX_Water2_Setf", 4, GFXC_P(p), (unsigned long long)(unsigned int)param,
              (unsigned long long)(unsigned int)offset, GFXC_F(value));
}

#define GFXC_SETV(name) void name(void* p, int param, Vec3* v) \
    { GFXC_Call(#name, 5, GFXC_P(p), (unsigned long long)(unsigned int)param, \
                GFXC_F(v->x), GFXC_F(v->y), GFXC_F(v->z)); }
GFXC_SETV(GFX_Profiler_Setv)
GFXC_SETV(GFX_Explode_Setv)
GFXC_SETV(GFX_Table_Setv)

void GFX_Water2_Setv(void* p, int param, unsigned int offset, Vec3* v)
{
    GFXC_Call("GFX_Water2_Setv", 6, GFXC_P(p), (unsigned long long)(unsigned int)param,
              (unsigned long long)offset, GFXC_F(v->x), GFXC_F(v->y), GFXC_F(v->z));
}

/* ---- the reset ---- */

static int GFXC_SetViewMatrix(Mtx4x4* m)
{
    GFXC_Call("SetViewMatrix", 1, GFXC_P(m));
    return 0;
}

void GFXC_Reset(void)
{
    memset(GFXC_arena, 0, sizeof(GFXC_arena));
    /* the untouched tail stays zero so the two sides' snapshots of it
       agree; only what the allocator hands out is poisoned */
    GFXC_arenaUsed = 0;
    GFXC_blockCount = 0;
    GFXC_logLen = 0;
    GFXC_logOverflow = 0;
    GFXC_tick = 0;
    GFXC_renderResult = 1;

    memset(&GFXC_dd, 0, sizeof(GFXC_dd));
    memset(GFXC_rasters, 0, sizeof(GFXC_rasters));
    memset(&GFXC_gfxGao, 0, sizeof(GFXC_gfxGao));
    memset(&GFXC_gfxGaoBasic, 0, sizeof(GFXC_gfxGaoBasic));
    memset(&GFXC_gfxVisual, 0, sizeof(GFXC_gfxVisual));
    memset(&GFXC_world, 0, sizeof(GFXC_world));
    memset(PerfCounterStruct, 0, sizeof(PerfCounterStruct));

    GFXC_intf.Render = GFXC_GroRender;
    GFXC_dd.rasters = GFXC_rasters;
    GFXC_dd.SetViewMatrix = GFXC_SetViewMatrix;
    GFXC_dd.drawMaskAllowedStuff = 0x00ABCDEF;
    GFXC_dd.drawMaskAllowedStuffCurrent = 0x00123456;
    GFXC_dd.flags = 0x00000121;
    gpst_DX_stDD = &GFXC_dd;
    WOR_gpst_CurrentWorld = &GFXC_world;

    TIM_gf_dt = 0.0f;
    GEO_b_IsInFix = 0;

    GFX_pul_RLI = 0;
    GFX_ul_MaxRLI = 0;
    GFX_gpst_GO = 0;
    GFX_gpst_Geo = 0;
    GFX_ul_MaxPoints = 0;
    GFX_ul_MaxUVs = 0;
    memset(GFX_aul_MaxTriangles, 0, sizeof(GFX_aul_MaxTriangles));
    GFX_ul_MaxElements = 0;
    GFX_ul_MaxElemSprites = 0;
    GFX_gpst_Current = 0;
    GFX_ppst_CurrentList = 0;
    memset(GFX_aul_MaxSprites, 0, sizeof(GFX_aul_MaxSprites));
    GFX_i_GlobalID = 0;
}
