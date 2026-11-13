/* Mocks for the gameobject.c differential test.

   Everything gameobject.c calls that is not math.c lives here. The
   subsystem entry points are logged and do nothing else; the ones whose
   result the code under test would have to interpret return a fixed
   value out of the scene. Anything on a path the test does not drive
   aborts, so a rewrite that reached one fails the test rather than
   passing on a stand-in.

   The allocator is a bump arena that never frees: a frozen copy and a
   rewrite have to allocate at the same addresses for the scene bytes to
   be comparable, and handing memory back between the two sides would
   break that. Blocks sit 64 bytes into a larger one for the same reason
   tests/gfx_mocks.c does it. */

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../common.h"
#include "../math.h"
#include "../tables.h"
#include "../gameobject.h"
#include "../groups.h"
#include "../modifiers.h"
#include "../dynamics.h"
#include "../collision.h"
#include "../events.h"
#include "../sound.h"
#include "../ways.h"
#include "../world.h"
#include "../loading.h"
#include "../bigfiles.h"
#include "../animation.h"
#include "../soft.h"
#include "../dbg.h"
#include "../ai.h"
#include "../graphicsobjects/geo.h"
#include "../graphicsobjects/staticlod.h"
#include "../graphicsobjects/gro_common.h"
#include "gameobject_mocks.h"

/* ------------------------------------------------------------ arena */

#define ARENA_BYTES (4 * 1024 * 1024)
static unsigned char g_arena[ARENA_BYTES];
static unsigned long g_used;

static unsigned char g_heapSnap[2][ARENA_BYTES];
static unsigned long g_heapSnapUsed[2];

const unsigned char* GMOCK_HeapBase(void) { return g_arena; }
unsigned long GMOCK_HeapUsed(void) { return g_used; }

static void* arena_alloc(unsigned long n, unsigned long align)
{
    unsigned long base;

    g_used += 64;                      /* guard band in front */
    base = (g_used + align - 1) & ~(align - 1);
    if (base + n + 64 > ARENA_BYTES)
    {
        fprintf(stderr, "mock arena exhausted\n");
        exit(2);
    }
    g_used = base + n;
    memset(g_arena + base, 0xA5, n);   /* MEM_p_Alloc does not zero */
    return g_arena + base;
}

/* ------------------------------------------------------------- log */

#define LOG_MAX 200000
#define LOG_LINE 160
static char g_log[LOG_MAX][LOG_LINE];
static int g_logN;
static char g_logSnap[2][4096][LOG_LINE];
static int g_logSnapN[2];

void GMOCK_Log(const char* fmt, ...)
{
    va_list ap;

    if (g_logN >= LOG_MAX)
        return;
    va_start(ap, fmt);
    vsnprintf(g_log[g_logN], LOG_LINE, fmt, ap);
    va_end(ap);
    ++g_logN;
}

int GMOCK_LogCount(void) { return g_logN; }
const char* GMOCK_LogLine(int i) { return g_log[i]; }

void GMOCK_Reset(void)
{
    g_used = 0;
    g_logN = 0;
    memset(g_arena, 0, ARENA_BYTES);
}

void GMOCK_SnapshotLog(int side)
{
    int i;
    int n = g_logN > 4096 ? 4096 : g_logN;

    g_logSnapN[side] = n;
    for (i = 0; i < n; ++i)
        memcpy(g_logSnap[side][i], g_log[i], LOG_LINE);
}

int GMOCK_CompareLogs(const char* what)
{
    int i;

    if (g_logSnapN[0] != g_logSnapN[1])
    {
        printf("  %s: call count %d vs %d\n", what, g_logSnapN[0], g_logSnapN[1]);
        for (i = 0; i < g_logSnapN[0] || i < g_logSnapN[1]; ++i)
            printf("    %-60s | %s\n",
                   i < g_logSnapN[0] ? g_logSnap[0][i] : "",
                   i < g_logSnapN[1] ? g_logSnap[1][i] : "");
        return 1;
    }
    for (i = 0; i < g_logSnapN[0]; ++i)
        if (strcmp(g_logSnap[0][i], g_logSnap[1][i]))
        {
            printf("  %s: call %d  %s | %s\n", what, i, g_logSnap[0][i], g_logSnap[1][i]);
            return 1;
        }
    return 0;
}

void GMOCK_SnapshotHeap(int side)
{
    g_heapSnapUsed[side] = g_used;
    memcpy(g_heapSnap[side], g_arena, g_used);
}

int GMOCK_CompareHeaps(const char* what)
{
    unsigned long i;

    if (g_heapSnapUsed[0] != g_heapSnapUsed[1])
    {
        printf("  %s: heap used %lu vs %lu\n", what, g_heapSnapUsed[0], g_heapSnapUsed[1]);
        return 1;
    }
    for (i = 0; i < g_heapSnapUsed[0]; ++i)
        if (g_heapSnap[0][i] != g_heapSnap[1][i])
        {
            printf("  %s: heap byte %lu differs (%02x vs %02x)\n",
                   what, i, g_heapSnap[0][i], g_heapSnap[1][i]);
            return 1;
        }
    return 0;
}

static void unreached(const char* name)
{
    fprintf(stderr, "mock reached: %s\n", name);
    exit(3);
}

#define OFF(p) ((p) ? (long)((unsigned char*)(p) - g_arena) : -1L)

/* ------------------------------------------------------- allocator */

void* MEM_p_Alloc(unsigned int size)
{
    void* p = arena_alloc(size, 8);
    GMOCK_Log("MEM_p_Alloc %u -> +%ld", size, (long)((unsigned char*)p - g_arena));
    return p;
}

void* MEM_p_AllocAligned(unsigned int size, int align)
{
    void* p = arena_alloc(size, align < 8 ? 8 : (unsigned long)align);
    GMOCK_Log("MEM_p_AllocAligned %u a=%d -> +%ld", size, align, (long)((unsigned char*)p - g_arena));
    return p;
}

void* MEM_p_Realloc(void* p, unsigned int size)
{
    void* q = arena_alloc(size, 8);
    GMOCK_Log("MEM_p_Realloc +%ld %u -> +%ld",
              p ? (long)((unsigned char*)p - g_arena) : -1L, size,
              (long)((unsigned char*)q - g_arena));
    if (p)
        memcpy(q, p, size);
    return q;
}

void MEM_Free(void* p)
{
    GMOCK_Log("MEM_Free +%ld", p ? (long)((unsigned char*)p - g_arena) : -1L);
}

void MEM_FreeAlign(void* p)
{
    GMOCK_Log("MEM_FreeAlign +%ld", p ? (long)((unsigned char*)p - g_arena) : -1L);
}

/* The matrix-array helpers are math.c's own; only the aligned
   reallocation under them is a mock. */

void* MEM_p_ReallocAligned(void* p, unsigned int size, int align)
{
    void* q = arena_alloc(size, align < 8 ? 8 : (unsigned long)align);
    GMOCK_Log("MEM_p_ReallocAligned +%ld %u a=%d -> +%ld",
              p ? (long)((unsigned char*)p - g_arena) : -1L, size, align,
              (long)((unsigned char*)q - g_arena));
    if (p)
        memcpy(q, p, size);
    return q;
}

/* ---------------------------------------------------- subsystem stubs */

/* Faithful enough to its real contract to matter: the real one
   allocates the collision block when the object has none, and three
   teardown paths in gameobject.c dereference it afterwards. */
void COL_AllocDetectionList(tGameObjectMain* gao)
{
    GMOCK_Log("COL_AllocDetectionList");
    if (!gao->extended)
        return;
    if (!gao->extended->coll)
    {
        gao->extended->coll = (tGameObjectCollMain*)arena_alloc(sizeof(tGameObjectCollMain), 8);
        gao->extended->coll->detectionList = 0;
        gao->extended->coll->colInstance = 0;
        gao->extended->coll->colMap = 0;
        GMOCK_Log("  coll -> +%ld", OFF(gao->extended->coll));
    }
    gao->extended->coll->detectionList =
        (tGameObjectCollDetectionList*)arena_alloc(sizeof(tGameObjectCollDetectionList), 8);
    gao->extended->coll->detectionList->overlapNum = 0;
    gao->extended->coll->detectionList->detectionListGaos = 0;
    GMOCK_Log("  detectionList -> +%ld", OFF(gao->extended->coll->detectionList));
}
void COL_FreeDetectionList(tGameObjectCollMain* c) { GMOCK_Log("COL_FreeDetectionList"); (void)c; }
int COL_AllocColMap(tGameObjectMain* gao) { GMOCK_Log("COL_AllocColMap"); (void)gao; return 1; }
void COL_FreeColMap(tGameObjectCollMain* c) { GMOCK_Log("COL_FreeColMap"); (void)c; }
void COL_FreeInstance(tGameObjectCollMain* c) { GMOCK_Log("COL_FreeInstance"); (void)c; }
void COL_Free(tGameObjectMain* gao) { GMOCK_Log("COL_Free"); (void)gao; }
void COL_DuplicateInstance(tGameObjectMain* d, tGameObjectMain* s) { GMOCK_Log("COL_DuplicateInstance"); (void)d; (void)s; }
void COL_DuplicateColMap(tGameObjectMain* d, tGameObjectMain* s) { GMOCK_Log("COL_DuplicateColMap"); (void)d; (void)s; }
struct sColInstance* COL_ul_CallBackLoadInstance(uint32_t o) { (void)o; unreached("COL_ul_CallBackLoadInstance"); return 0; }
tColMap* COL_ul_CallBackLoadColMap(uint32_t o) { (void)o; unreached("COL_ul_CallBackLoadColMap"); return 0; }

void AI_FreeInstance(tAI* ai) { GMOCK_Log("AI_FreeInstance"); (void)ai; }
void AI_DuplicateInstance(tGameObjectMain* s, tGameObjectMain* d) { GMOCK_Log("AI_DuplicateInstance"); (void)s; (void)d; }
void AI_ResolveInstanceRef(tAI* ai) { GMOCK_Log("AI_ResolveInstanceRef"); (void)ai; }
void AI_ReinitInstance(tAI* ai) { GMOCK_Log("AI_ReinitInstance"); (void)ai; }
tAI* AI_ul_CallbackLoadInstance(uint32_t o) { (void)o; unreached("AI_ul_CallbackLoadInstance"); return 0; }

void ANI_Free(tGameObjectActorObject* a) { GMOCK_Log("ANI_Free"); (void)a; }
tGameObjectActorObject* ANI_pst_CreateGameObjectAnim()
{
    tGameObjectActorObject* p = (tGameObjectActorObject*)arena_alloc(sizeof(tGameObjectActorObject), 8);
    GMOCK_Log("ANI_pst_CreateGameObjectAnim -> +%ld", OFF(p));
    return p;
}
void ANI_pst_DuplicateGameObjectAnim(tWorld* w, tGameObjectMain* d, tGameObjectMain* s)
{ GMOCK_Log("ANI_pst_DuplicateGameObjectAnim"); (void)w; (void)d; (void)s; }

/* The three tracked bones, picked out of the scene by the driver. */
tGameObjectMain* GMOCK_canal[16];
tGameObjectMain* ANI_pst_GetObjectByAICanal(tGameObjectMain* gao, uint8_t canal)
{
    GMOCK_Log("ANI_pst_GetObjectByAICanal %u", (unsigned)canal);
    (void)gao;
    return canal < 16 ? GMOCK_canal[canal] : 0;
}

tDyn* DYN_pst_Dyna_Create(void)
{
    tDyn* p = (tDyn*)arena_alloc(sizeof(tDyn), 8);
    GMOCK_Log("DYN_pst_Dyna_Create -> +%ld", OFF(p));
    return p;
}
void DYN_Dyna_Init(tDyn* d, int a, float b, float c, float e, Vec3* f, Vec3* g)
{ GMOCK_Log("DYN_Dyna_Init %d %.6g", a, (double)b); (void)d; (void)c; (void)e; (void)f; (void)g; }
void DYN_Dyna_Close(tDyn* d) { GMOCK_Log("DYN_Dyna_Close"); (void)d; }

tEventData* EVE_AllocateMainStruct(tGameObjectMain* g) { GMOCK_Log("EVE_AllocateMainStruct"); (void)g; return 0; }
void EVE_FreeMainStruct(tGameObjectMain* g) { GMOCK_Log("EVE_FreeMainStruct"); (void)g; }
void EVE_Duplicate(tGameObjectMain* s, tGameObjectMain* d, int f) { GMOCK_Log("EVE_Duplicate %d", f); (void)s; (void)d; }
void EVE_ResolveRefs(tGameObjectMain* g, int f) { GMOCK_Log("EVE_ResolveRefs %d", f); (void)g; }
void EVE_ResolveListTrackRef(tGameObjectEventsList* l) { GMOCK_Log("EVE_ResolveListTrackRef %ld", OFF(l)); }
struct sGameObjectEventsList* EVE_ul_CallbackLoadListTracks(uint32_t o) { (void)o; unreached("EVE_ul_CallbackLoadListTracks"); return 0; }

void SND_Duplicate(tGameObjectMain* s, tGameObjectMain* d) { GMOCK_Log("SND_Duplicate"); (void)s; (void)d; }
void SND_FreeMainStruct(tGameObjectMain* g) { GMOCK_Log("SND_FreeMainStruct"); (void)g; }
tSndBank* SND_ul_CallbackLoad(uint32_t o) { (void)o; unreached("SND_ul_CallbackLoad"); return 0; }

void WAY_FreeStruct(tWay* w) { GMOCK_Log("WAY_FreeStruct"); (void)w; }
tWay* WAY_ul_AllLinkListsCallback(uint32_t o) { (void)o; unreached("WAY_ul_AllLinkListsCallback"); return 0; }

void MDF_Reinit(tGameObjectMain* g) { GMOCK_Log("MDF_Reinit"); (void)g; }
void MDF_Modifier_Destroy(tModifier* m) { GMOCK_Log("MDF_Modifier_Destroy %ld", OFF(m)); }
void MDF_Modifier_DuplicateList(tGameObjectMain* g, tModifier* m) { GMOCK_Log("MDF_Modifier_DuplicateList"); (void)g; (void)m; }
tModifier* MDF_pst_GetByType(tGameObjectMain* g, int t)
{
    GMOCK_Log("MDF_pst_GetByType %d", t);
    (void)g;
    return t == 7 && g && g->extended ? g->extended->mdf : 0;
}
int MDF_ul_Modifier_Load(tModifier** m, char* p, int n, tGameObjectMain* g)
{ (void)m; (void)p; (void)n; (void)g; unreached("MDF_ul_Modifier_Load"); return 0; }

void GRPSectDelObj(tGameObjectMain* g) { GMOCK_Log("GRPSectDelObj"); (void)g; }
void GRP_ObjDetachGroup(tGameObjectMain* g) { GMOCK_Log("GRP_ObjDetachGroup"); (void)g; }
tGroup* GRP_ul_LoadGrpCallback(uint32_t o) { (void)o; unreached("GRP_ul_LoadGrpCallback"); return 0; }

void EOT_UpdateIdentityFlagsOfTable(tableDescStruct* t) { GMOCK_Log("EOT_UpdateIdentityFlagsOfTable %ld", OFF(t)); }
void EOT_RemoveObjectFromTableIfBadFlag(tableDescStruct* t, int f) { GMOCK_Log("EOT_RemoveObjectFromTableIfBadFlag %d", f); (void)t; }

void INT_SnP_AddObject(tGameObjectMain* g, tSnp* s) { GMOCK_Log("INT_SnP_AddObject"); (void)g; (void)s; }
void INT_SnP_InsertionSort(tSnp* s, int f) { GMOCK_Log("INT_SnP_InsertionSort %d", f); (void)s; }

void WOR_ActivateObjectIfFlagsAllow(tGameObjectMain* g, tWorld* w) { GMOCK_Log("WOR_ActivateObjectIfFlagsAllow"); (void)g; (void)w; }
void WOR_World_AttachGameObject(tWorld* w, tGameObjectMain* g) { GMOCK_Log("WOR_World_AttachGameObject"); (void)w; (void)g; }

void TAB_PFTable_AddElemToTable(tableDescStruct* t, void* o, int f) { GMOCK_Log("TAB_PFTable_AddElemToTable %d", f); (void)t; (void)o; }
void TAB_PTable_AddElemToTable(tableDescStruct* t, void* o) { GMOCK_Log("TAB_PTable_AddElemToTable"); (void)t; (void)o; }
void TAB_PFtable_Close(tableDescStruct* t) { GMOCK_Log("TAB_PFtable_Close"); (void)t; }
void TAB_PFtable_Init(tableDescStruct* t, unsigned int n, float r) { (void)t; (void)n; (void)r; unreached("TAB_PFtable_Init"); }
int TAB_ul_Ptable_GetElemIndexWithPointer(tableDescStruct* t, void* o)
{ GMOCK_Log("TAB_ul_Ptable_GetElemIndexWithPointer"); (void)t; (void)o; return -1; }
BOOL TAB_TableElemIsInvalid(void* p) { return p == 0 || p == (void*)-1; }

void LOA_DeleteAddress(int a) { GMOCK_Log("LOA_DeleteAddress"); (void)a; }
void LOA_MakeFileRef(int k, void** slot, void* (*cb)(uint32_t), char n) { (void)k; (void)slot; (void)cb; (void)n; unreached("LOA_MakeFileRef"); }
int LOA_ReadLong(char** p, uint32_t* d, char m) { (void)p; (void)d; (void)m; unreached("LOA_ReadLong"); return 0; }
char LOA_ReadChar(char** p, char* d, char m) { (void)p; (void)d; (void)m; unreached("LOA_ReadChar"); return 0; }
uint16_t LOA_ReadShort(char** p, uint16_t* d, char m) { (void)p; (void)d; (void)m; unreached("LOA_ReadShort"); return 0; }
float LOA_ReadFloat(char** p, float* d, char m) { (void)p; (void)d; (void)m; unreached("LOA_ReadFloat"); return 0; }
void LOA_ReadVector(char** p, Vec3* d, int m) { (void)p; (void)d; (void)m; unreached("LOA_ReadVector"); }
void LOA_ReadMatrix(char** p, Mtx4x4* d, int m) { (void)p; (void)d; (void)m; unreached("LOA_ReadMatrix"); }
void LOA_ReadString(char** p, char* d, int n, char m) { (void)p; (void)d; (void)n; (void)m; unreached("LOA_ReadString"); }
char** LOA_ReadCharArray(char** p, char* d, unsigned int n, char m) { (void)p; (void)d; (void)n; (void)m; unreached("LOA_ReadCharArray"); return 0; }
char* BIG_pc_ReadFileTmp(int o, int* size) { (void)o; (void)size; unreached("BIG_pc_ReadFileTmp"); return 0; }

tGroGeneric* GRO_pst_Struct_Create(int idx) { (void)idx; unreached("GRO_pst_Struct_Create"); return 0; }
tGroGeneric* GEO_ul_Load_ObjectCallback(uint32_t o) { (void)o; unreached("GEO_ul_Load_ObjectCallback"); return 0; }
tActionKit* ACT_ul_ActionKitCallback(uint32_t o) { (void)o; unreached("ACT_ul_ActionKitCallback"); return 0; }

/* The static-LOD pick the geo getter makes: the driver sets this. */
tGEOGRO* GMOCK_lodPick;
tGEOGRO* GEO_pst_StaticLOD_GetLOD(tGEOSTATICLOD* lod, uint8_t distance)
{
    GMOCK_Log("GEO_pst_StaticLOD_GetLOD %u", distance);
    (void)lod;
    return GMOCK_lodPick;
}

/* A deterministic projection: the real one needs a display device. The
   sentinel 0x4F000000 (2147483648.0f) is produced for points behind the
   camera, because both culling frames branch on exactly that word.

   The two cross terms are not decoration. Without them the projected x
   depends only on (x, z) and the projected y only on (y, z), so each of
   the eight box corners shares its x with one sibling and its y with
   another -- and moving a corner onto another corner's position becomes
   invisible. With the shear every corner is its own point, which is
   what the real projection matrix gives. */
void SOFT_TransformAndProject(Vec3* dest, Vec3* src, int count, tDDCamStruct* cam)
{
    int i;
    float w;
    float px;
    float py;

    GMOCK_Log("SOFT_TransformAndProject %d", count);
    for (i = 0; i < count; ++i)
    {
        w = src[i].z - cam->objectMtx.Pos.z;
        if (w <= 0.0f)
        {
            dest[i].x = 2147483648.0f;
            dest[i].y = 2147483648.0f;
            dest[i].z = 0.0f;
        }
        else
        {
            px = src[i].x - cam->objectMtx.Pos.x + 0.3f * w;
            py = src[i].y - cam->objectMtx.Pos.y - 0.2f * w;
            dest[i].x = cam->centerX + (px + 0.25f * py) * cam->factorX / w;
            dest[i].y = cam->centerY + (py - 0.125f * px) * cam->factorY / w;
            dest[i].z = w;
        }
    }
}

void DBG_OutputDebugString(char* fmt, ...) { (void)fmt; }
