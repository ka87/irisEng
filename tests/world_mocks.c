/* Stand-ins for everything world.c calls, for the differential test.

   Both copies of world.c run over one scene buffer at one address with
   the allocator rewound between them, so the blocks handed out are the
   same and every pointer either copy writes can be compared directly.
   Every observable call is recorded: comparing the final state alone
   would miss a dropped or reordered engine call that happens not to
   change the numbers.

   The real tables.c is linked rather than mocked. world.c's whole job
   is moving game objects between those tables, and the rewrite restored
   nine inlined copies of TAB_AddElemToTablePF and one of
   TAB_PTable_AddElemToTable, so the comparison is only worth anything
   if the table primitives are the engine's own.

   Anything world.c calls that belongs to the loader, the renderer, the
   sound engine or Direct3D aborts instead of returning: those paths
   cannot be stood up here, and a rewrite that reached one of them by
   accident should fail the test rather than pass on a stand-in. */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <math.h>

#include "../tables.h"
#include "../memory.h"
#include "../loading.h"
#include "../bigfiles.h"
#include "../view.h"
#include "../snp.h"
#include "../profiler.h"
#include "../gameobject.h"
#include "../time.h"
#include "../graphicsobjects/geo.h"
#include "../graphicsobjects/geomaterial.h"
#include "../modifiers/xmenmodifier.h"
#include "../animation.h"
#include "../engine.h"
#include "../water.h"
#include "../ai/ai_worldfunctions.h"
#include "../ai/ai_viewfunctions.h"
#include "../ai/ai_gfxfunctions.h"
#include "../grid.h"
#include "../ways.h"
#include "../strings.h"
#include "../graphicsobjects/staticlod.h"
#include "../textget.h"
#include "../cameraengine.h"
#include "../gfx.h"
#include "../modifiers/lazymodifier.h"
#include "../modifiers/protexmodifier.h"
#include "../gdi.h"
#include "../win/wingdi.h"
#include "../soft.h"
#include "../inputoutput.h"
#include "../textures.h"
#include "../sound.h"
#include "../sectors.h"
#include "../win/wininputoutput.h"
#include "../ai/ai_debugfunctions.h"
#include "../dbg.h"
#include "../world.h"
#include "world_mocks.h"

/* globals world.c reads that the engine headers declare extern */
int ENG_gb_ActiveSectorization;
int GFX_gl_FS_VideoPlaying;
float AI_gf_Epsilon;
struct sGameObjectMain** AI_MainActor;

/* ---------------------------------------------------------------- log */

static char WMOCK_log[WMOCK_LOG_MAX][WMOCK_LOG_LINE];
static int WMOCK_logNum;
static int WMOCK_logOverflow;

void WMOCK_LogReset(void)
{
    int i;

    for (i = 0; i < WMOCK_logNum && i < WMOCK_LOG_MAX; ++i)
        WMOCK_log[i][0] = '\0';
    WMOCK_logNum = 0;
    WMOCK_logOverflow = 0;
}

int WMOCK_LogNum(void) { return WMOCK_logNum; }
const char* WMOCK_LogLine(int i) { return WMOCK_log[i]; }
int WMOCK_LogOverflowed(void) { return WMOCK_logOverflow; }

void WMOCK_Logf(const char* fmt, ...)
{
    va_list ap;

    if (WMOCK_logNum >= WMOCK_LOG_MAX)
    {
        WMOCK_logOverflow = 1;
        return;
    }
    va_start(ap, fmt);
    vsnprintf(WMOCK_log[WMOCK_logNum], WMOCK_LOG_LINE, fmt, ap);
    va_end(ap);
    ++WMOCK_logNum;
}

static void WMOCK_Unreachable(const char* name)
{
    fprintf(stderr, "world_mocks: %s was called; this path is not stood up\n", name);
    abort();
}

/* ------------------------------------------------------------ the heap

   Nothing is ever handed back: the first side's blocks have to stay
   readable while the second side runs, and the allocation sequence is
   in the call log instead. Each block sits 32 bytes into a larger one
   so a copy that writes a little way in front of its block does not
   corrupt the allocator's own bookkeeping. */

#define WMOCK_HEAP_SIZE (4u * 1024u * 1024u)
#define WMOCK_GUARD 32u

static unsigned char WMOCK_heap[WMOCK_HEAP_SIZE];
static unsigned int WMOCK_heapUsed;

void WMOCK_HeapReset(void)
{
    memset(WMOCK_heap, 0, WMOCK_heapUsed + WMOCK_GUARD);
    WMOCK_heapUsed = 0;
}

void* WMOCK_HeapBase(void) { return WMOCK_heap; }
unsigned int WMOCK_HeapUsed(void) { return WMOCK_heapUsed; }

static void* WMOCK_Take(unsigned int size)
{
    unsigned char* p;
    unsigned int step = (size + WMOCK_GUARD + 15u) & ~15u;

    if (WMOCK_heapUsed + step + WMOCK_GUARD > WMOCK_HEAP_SIZE)
    {
        fprintf(stderr, "world_mocks: heap exhausted\n");
        abort();
    }
    p = WMOCK_heap + WMOCK_heapUsed + WMOCK_GUARD;
    WMOCK_heapUsed += step;
    memset(p, 0, size);
    return p;
}

void* MEM_p_Alloc(unsigned int size)
{
    void* p = WMOCK_Take(size);
    WMOCK_Logf("MEM_p_Alloc %u -> +%u", size, (unsigned int)((unsigned char*)p - WMOCK_heap));
    return p;
}

void* MEM_p_AllocTmp(int size)
{
    void* p = WMOCK_Take(size);
    WMOCK_Logf("MEM_p_AllocTmp %u -> +%u", size, (unsigned int)((unsigned char*)p - WMOCK_heap));
    return p;
}

void* MEM_p_Realloc(void* old, size_t size)
{
    void* p = WMOCK_Take(size);
    WMOCK_Logf("MEM_p_Realloc +%d %u -> +%u",
               old ? (int)((unsigned char*)old - WMOCK_heap) : -1,
               size, (unsigned int)((unsigned char*)p - WMOCK_heap));
    if (old)
        memcpy(p, old, size);
    return p;
}

void MEM_Free(void* p)
{
    WMOCK_Logf("MEM_Free +%d", p ? (int)((unsigned char*)p - WMOCK_heap) : -1);
}

/* ---------------------------------------------------------- the clock */

static int WMOCK_ticks;

void WMOCK_ClockReset(void) { WMOCK_ticks = 0; }

unsigned int TIM_GetTicksAsInt32()
{
    WMOCK_ticks += 7;
    return (unsigned int)WMOCK_ticks;
}

float TIM_f_Clock_TrueRead()
{
    WMOCK_ticks += 3;
    return (float)WMOCK_ticks;
}

/* ------------------------------------------------- the game object API */

int WMOCK_cullSeed = 0;

/* A real half-space test, so the mask arithmetic the rewrite folded into
   a loop actually decides something: a corner is inside when it is on the
   non-negative side of each plane the flag word enables. */
BOOL OBJ_CullingPointInside(Vec3* p, Vec3* left, Vec3* up, Vec3* right, Vec3* down, char flags)
{
    Vec3* planes[4];
    int i;
    int inside = 1;

    planes[0] = left;
    planes[1] = up;
    planes[2] = right;
    planes[3] = down;
    for (i = 0; i < 4; ++i)
    {
        if ((flags & (1 << i)) == 0)
            continue;
        if (p->x * planes[i]->x + p->y * planes[i]->y + p->z * planes[i]->z < 0.0f)
            inside = 0;
    }
    WMOCK_Logf("OBJ_CullingPointInside %.6g %.6g %.6g f=%d -> %d",
               p->x, p->y, p->z, flags, inside);
    return inside;
}

BOOL OBJ_CullingObject(struct sGameObjectMain* gao)
{
    int r = (int)(((gao->flags >> 3) + WMOCK_cullSeed) & 1);
    WMOCK_Logf("OBJ_CullingObject fl=%08x -> %d", gao->flags, r);
    return r;
}

char OBJ_uc_CalcLODVis(struct sGameObjectMain* gao, struct sViewElem* view)
{
    char r = (char)((gao->flags2 + WMOCK_cullSeed * 3 + 11) & 0xFF);
    WMOCK_Logf("OBJ_uc_CalcLODVis fl2=%08x -> %u", gao->flags2, (unsigned)r);
    return r;
}

void OBJ_ComputeGlobalWhenHie(struct sGameObjectMain* gao)
{
    WMOCK_Logf("OBJ_ComputeGlobalWhenHie fl=%08x", gao->flags);
}

void OBJ_ResetProcess(tGameObjectMain* gao, char callCount, char mask)
{
    WMOCK_Logf("OBJ_ResetProcess fl=%08x n=%d m=%x", gao ? gao->flags : 0, callCount, mask);
    if (gao)
        gao->process = (char)(gao->process & ~mask);
}

void OBJ_ResolveAIRef(struct sGameObjectMain* gao)
{
    WMOCK_Logf("OBJ_ResolveAIRef fl=%08x", gao->flags);
}

void OBJ_Reinit(struct sGameObjectMain* gao)
{
    WMOCK_Logf("OBJ_Reinit fl=%08x", gao->flags);
}

void OBJ_Gizmo_UpdateGizmoPtr(struct sGameObjectMain* gao)
{
    WMOCK_Logf("OBJ_Gizmo_UpdateGizmoPtr fl=%08x", gao->flags);
}

void OBJ_ChangeIdentityFlags(tGameObjectMain* gao, unsigned int flags, int flags2)
{
    WMOCK_Logf("OBJ_ChangeIdentityFlags %08x %08x", flags, flags2);
    gao->flags = (int)flags;
}

void OBJ_GameObject_Remove(tGameObjectMain* gao, BOOL refCountDown)
{
    WMOCK_Logf("OBJ_GameObject_Remove fl=%08x %d", gao->flags, refCountDown);
}

void OBJ_UpdateCullingVars(struct sDDCamStruct* cam) { WMOCK_Unreachable("OBJ_UpdateCullingVars"); }
struct sGameObjectMain* OBJ_ul_GameObjectCallback(uint32_t o) { WMOCK_Unreachable("OBJ_ul_GameObjectCallback"); return 0; }

/* ------------------------------------------------ actions and animation */

void ACT_SetUsageCounts(struct sActionKit* kit)
{
    WMOCK_Logf("ACT_SetUsageCounts %d", kit ? 1 : 0);
}

void ACT_SetAction(struct sGameObjectMain* gao, int a, int b)
{
    WMOCK_Logf("ACT_SetAction fl=%08x %d %d", gao->flags, a, b);
}

void ANI_PlayGameObjectAnimation(tGameObjectMain* gao, tGameObjectActorObject* actor, float t, Vec3* speed)
{
    WMOCK_Logf("ANI_PlayGameObjectAnimation fl=%08x %.6g %d", gao->flags, t, speed ? 1 : 0);
}

void ANI_ResolveRefs(struct sGameObjectMain* gao)
{
    WMOCK_Logf("ANI_ResolveRefs fl=%08x", gao->flags);
}

void ANI_Render(struct sGameObjectMain* gao) { WMOCK_Unreachable("ANI_Render"); }

void EVE_ResolveListTrackRef(tGameObjectEventsList* list)
{
    WMOCK_Logf("EVE_ResolveListTrackRef %d", list ? 1 : 0);
}

void SND_ResolveRefsForMdF(struct sModifier* mdf)
{
    WMOCK_Logf("SND_ResolveRefsForMdF %d", mdf ? 1 : 0);
}

void SND_ResolveRefs(struct sGameObjectMain* gao)
{
    WMOCK_Logf("SND_ResolveRefs fl=%08x", gao->flags);
}

void COL_ResolveRefs(struct sGameObjectMain* gao)
{
    WMOCK_Logf("COL_ResolveRefs fl=%08x", gao->flags);
}

/* ------------------------------------- paths that cannot be stood up here */

void ENG_ReinitOneObject(struct sGameObjectMain* g, int n) { WMOCK_Unreachable("ENG_ReinitOneObject"); }
unsigned int WOR_View_Create(struct sWorld* w) { WMOCK_Unreachable("WOR_View_Create"); return 0; }
void WOR_View_Close(struct sViewElem* v) { WMOCK_Unreachable("WOR_View_Close"); }
void INT_SnP_Alloc(struct sWorld* w) { WMOCK_Unreachable("INT_SnP_Alloc"); }
void INT_SnP_DesAlloc(struct sWorld* w) { WMOCK_Unreachable("INT_SnP_DesAlloc"); }
void GRO_Struct_DestroyTable(tableDescStruct* t) { WMOCK_Unreachable("GRO_Struct_DestroyTable"); }
void GRO_Render(struct sGameObjectMain* g) { WMOCK_Unreachable("GRO_Render"); }
void GRI_Destroy(tWorldGridStruct* g) { WMOCK_Unreachable("GRI_Destroy"); }
void GRI_Reinit(struct sWorldGridStruct* g) { WMOCK_Unreachable("GRI_Reinit"); }
tWorldGridStruct* GRID_ul_CallbackWorldGrid(uint32_t o) { WMOCK_Unreachable("GRID_ul_CallbackWorldGrid"); return 0; }
void WAY_RemoveAllNetworks(tWayAllNetworks* w) { WMOCK_Unreachable("WAY_RemoveAllNetworks"); }
void WAY_ComputePoundNetworks(struct sWorld* w) { WMOCK_Unreachable("WAY_ComputePoundNetworks"); }
tWayAllNetworks* WAY_ul_AllNetWorksCallback(uint32_t o) { WMOCK_Unreachable("WAY_ul_AllNetWorksCallback"); return 0; }
void MDF_ApplyAllGao(struct sGameObjectMain* g) { WMOCK_Unreachable("MDF_ApplyAllGao"); }
void MDF_UnApplyAllGao(struct sGameObjectMain* g) { WMOCK_Unreachable("MDF_UnApplyAllGao"); }
void GFX_Free(struct sGfxElem** l, int n) { WMOCK_Unreachable("GFX_Free"); }
struct sGfxElem* GFX_Attached(struct sGfxElem* l) { WMOCK_Unreachable("GFX_Attached"); return 0; }
void GFX_Render(tGfxElem** l, int n) { WMOCK_Unreachable("GFX_Render"); }
void GFX_End() { WMOCK_Unreachable("GFX_End"); }
void GFX_FS_VideoUpdate() { WMOCK_Unreachable("GFX_FS_VideoUpdate"); }
void GFX_NeedGeomEx(unsigned int a, unsigned int b, unsigned int c, unsigned int d, int e) { WMOCK_Unreachable("GFX_NeedGeomEx"); }
void GFX_NeedGeomExElem(unsigned int a, unsigned int b) { WMOCK_Unreachable("GFX_NeedGeomExElem"); }
void LIGHT_List_Reset(struct sLightListStruct* l) { WMOCK_Unreachable("LIGHT_List_Reset"); }
void LIGHT_List_AddLight(struct sLightListStruct* l, struct sGameObjectMain* g) { WMOCK_Unreachable("LIGHT_List_AddLight"); }
int CAM_SetCameraMatrix(tDDContainerStruct* a, Mtx4x4* b) { WMOCK_Unreachable("CAM_SetCameraMatrix"); return 0; }
void CAM_Inverse(tDDContainerStruct* a) { WMOCK_Unreachable("CAM_Inverse"); }
void CAM_Engine_ComputePlans(tDDContainerStruct* a) { WMOCK_Unreachable("CAM_Engine_ComputePlans"); }
void SOFT_MatrixStack_Reset(tMatrixStackStruct* a, Mtx4x4* b) { WMOCK_Unreachable("SOFT_MatrixStack_Reset"); }
void SOFT_ZList_Send() { WMOCK_Unreachable("SOFT_ZList_Send"); }
void SOFT_ZList_Clear() { WMOCK_Unreachable("SOFT_ZList_Clear"); }
void D3D_PrepareWaterRendering() { WMOCK_Unreachable("D3D_PrepareWaterRendering"); }
void D3D_PC_AE(tWorld* w) { WMOCK_Unreachable("D3D_PC_AE"); }
void D3D_CreateGameObjectsD3DStuffForMultiStream(struct sWorld* w) { WMOCK_Unreachable("D3D_Create"); }
void SDW_UpdatePositions() { WMOCK_Unreachable("SDW_UpdatePositions"); }
void SDW_Destroy() { WMOCK_Unreachable("SDW_Destroy"); }
void PROTEX_BeforeDraw(void) { WMOCK_Unreachable("PROTEX_BeforeDraw"); }
void GAO_ModifierLazy_UnapplyAll(void) { WMOCK_Unreachable("GAO_ModifierLazy_UnapplyAll"); }
void GDI_l_AttachWorld(struct sGdi_DisplayData* d, struct sWorld* w) { WMOCK_Unreachable("GDI_l_AttachWorld"); }
BOOL GDI_l_DetachWorld(struct sGdi_DisplayData* d) { WMOCK_Unreachable("GDI_l_DetachWorld"); return 0; }
void DisplayAttach(struct sGdi_DisplayData* d) { WMOCK_Unreachable("DisplayAttach"); }
void DINPUT_InitVariables(int n) { WMOCK_Unreachable("DINPUT_InitVariables"); }
void* GRPSectClearAll() { WMOCK_Unreachable("GRPSectClearAll"); return 0; }
void WTR_BeginLoadWorldHook() { WMOCK_Unreachable("WTR_BeginLoadWorldHook"); }
void STR_3DStringList_Clear() { WMOCK_Unreachable("STR_3DStringList_Clear"); }
void STR_ReinitAll() { WMOCK_Unreachable("STR_ReinitAll"); }
void TEXT_FreeAll() { WMOCK_Unreachable("TEXT_FreeAll"); }
void TEXT_LoadWorldText(int k) { WMOCK_Unreachable("TEXT_LoadWorldText"); }
void TEX_List_Free(tEngTexListStruct* l) { WMOCK_Unreachable("TEX_List_Free"); }
void MAT_SpriteGen_Close() { WMOCK_Unreachable("MAT_SpriteGen_Close"); }
void SND_FreeHeaderArray(BASStruct* a) { WMOCK_Unreachable("SND_FreeHeaderArray"); }
void SND_MetaBankFreeList() { WMOCK_Unreachable("SND_MetaBankFreeList"); }
void SND_ResetAllMute() { WMOCK_Unreachable("SND_ResetAllMute"); }
void SND_StreamPrefetchFlushAll() { WMOCK_Unreachable("SND_StreamPrefetchFlushAll"); }
void INO_Reinit() { WMOCK_Unreachable("INO_Reinit"); }
void SAV_PhotoClose() { WMOCK_Unreachable("SAV_PhotoClose"); }
void SAV_PhotoInit() { WMOCK_Unreachable("SAV_PhotoInit"); }
BOOL MEM_Defrag(int n) { WMOCK_Unreachable("MEM_Defrag"); return 0; }
void MEM_ResetTmpMemory() { WMOCK_Unreachable("MEM_ResetTmpMemory"); }
void PreLoadTex_Finished(int* p) { WMOCK_Unreachable("PreLoadTex_Finished"); }
void PreLoadMoveBlocks(int k) { WMOCK_Unreachable("PreLoadMoveBlocks"); }
void ResetPreloadTexAll() { WMOCK_Unreachable("ResetPreloadTexAll"); }
void AI_EvalFunc_DbgTraceInt_C() { WMOCK_Unreachable("AI_EvalFunc_DbgTraceInt_C"); }
void AI_EvalFunc_VIEWSnapshot_DoIt() { WMOCK_Unreachable("AI_EvalFunc_VIEWSnapshot_DoIt"); }
void AI_EvalFunc_WORPreLoadTex_C() { WMOCK_Unreachable("AI_EvalFunc_WORPreLoadTex_C"); }
void AI_ExecCallback(struct sGameObjectMain* g, int n) { WMOCK_Unreachable("AI_ExecCallback"); }
void AI_FreeInstance(tAI* a) { WMOCK_Unreachable("AI_FreeInstance"); }
struct sAI* AI_pst_LoadInstance(int k) { WMOCK_Unreachable("AI_pst_LoadInstance"); return 0; }
void BIG_FreeGlobalBuffer() { WMOCK_Unreachable("BIG_FreeGlobalBuffer"); }
char* BIG_pc_ReadFileTmp(int o, int* s) { WMOCK_Unreachable("BIG_pc_ReadFileTmp"); return 0; }
void LOA_AddPosAddress(unsigned int p, unsigned int a) { WMOCK_Unreachable("LOA_AddPosAddress"); }
void LOA_BeginSpeedMode(int k) { WMOCK_Unreachable("LOA_BeginSpeedMode"); }
void LOA_EndSpeedMode() { WMOCK_Unreachable("LOA_EndSpeedMode"); }
void LOA_DeleteAddress(int a) { WMOCK_Unreachable("LOA_DeleteAddress"); }
void LOA_MakeFileRef(int k, void** d, void* (*cb)(uint32_t), char n) { WMOCK_Unreachable("LOA_MakeFileRef"); }
char** LOA_ReadCharArray(char** p, char* d, unsigned int n, char m) { WMOCK_Unreachable("LOA_ReadCharArray"); return 0; }
float LOA_ReadFloat(char** p, float* o, char m) { WMOCK_Unreachable("LOA_ReadFloat"); return 0; }
int LOA_ReadLong(char** p, uint32_t* r, char m) { WMOCK_Unreachable("LOA_ReadLong"); return 0; }
void LOA_ReadMatrix(char** p, Mtx4x4* m, int n) { WMOCK_Unreachable("LOA_ReadMatrix"); }
void LOA_ReadString(char** p, char* d, int n, char m) { WMOCK_Unreachable("LOA_ReadString"); }
void LOA_Resolve() { WMOCK_Unreachable("LOA_Resolve"); }
int LOA_ul_GetCurrentKey() { WMOCK_Unreachable("LOA_ul_GetCurrentKey"); return 0; }
void MATH_SetIdentityMatrix(Mtx4x4* m) { WMOCK_Unreachable("MATH_SetIdentityMatrix"); }
BOOL MATH_b_TypeIsCorrect(Mtx4x4* m) { WMOCK_Unreachable("MATH_b_TypeIsCorrect"); return 0; }
void DBG_OutputDebugString(char* f, ...) { WMOCK_Unreachable("DBG_OutputDebugString"); }
int QueryPerformanceCounter(LARGE_INTEGER* out) { WMOCK_Unreachable("QueryPerformanceCounter"); return 0; }

/* The two lookups WOR_World_ResolveAIRef needs are pure, so they are
   deterministic stand-ins rather than aborts: the function resolves four
   near-identical design fields and is worth driving. Keys that hash to -1
   exercise the miss arm. */
int BIG_ul_SearchKeyToPos(uint32_t key)
{
    int r = (key == 0 || (key & 7) == 3) ? -1 : (int)(key * 3 + 1);
    WMOCK_Logf("BIG_ul_SearchKeyToPos %u -> %d", key, r);
    return r;
}

int LOA_ul_SearchAddress(int pos)
{
    int r = pos * 5 + 17;
    WMOCK_Logf("LOA_ul_SearchAddress %d -> %d", pos, r);
    return r;
}
