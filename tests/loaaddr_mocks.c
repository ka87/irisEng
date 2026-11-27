/* Everything loading.c refers to and the address store does not need.
 *
 * The store is a small part of a large file, so most of this list is
 * the rest of that file's business rather than the store's. The two
 * that matter are the allocator, which is the C library's here so that
 * this test does not depend on the engine's own, and the big-file key
 * lookup, which is what the reference resolver asks before it reaches
 * the store -- so the driver drives it.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../bigfiles.h"
#include "../basic_array.h"

/* --- the allocator, which the key/value arrays use ----------------- */

void* MEM_p_Alloc(unsigned int size) { return malloc(size ? size : 1u); }
/* The allocator's own account of an address. The engine's version
   reads the block header the pool writes; this heap has none, so the
   mock says only that it cannot tell -- which is all the resolver's
   report needs of it, and that report is reached only on a fault
   these cases do not produce. */
void MEM_DescribeBlock(const void* ptr, char* out, unsigned int n)
{
    snprintf(out, n, ptr ? "not an arena this harness keeps" : "null");
}

/* Named by the resolver's report, which compares it against the
   reference's own callback and never calls it. */
struct sGroGeneric* GEO_ul_Load_ObjectCallback(uint32_t offset)
{
    (void)offset;
    return 0;
}

void* MEM_p_Realloc(void* src, size_t size) { return realloc(src, size ? size : 1u); }
void MEM_Free(void* ptr) { free(ptr); }
void MEM_FreeFromEnd(void* ptr) { free(ptr); }

/* --- the big-file key lookup, which the driver sets up ------------- */

/* key -> position, as the driver wants it answered. A key that is not
   in this table answers -1, which is what a real miss answers. */
#define LOAMOCK_KEYS 64

static struct { uint32_t key; int pos; } loamock_keys[LOAMOCK_KEYS];
static int loamock_keyNum;

void LOAMOCK_SetKeyPos(uint32_t key, int pos)
{
    if (loamock_keyNum < LOAMOCK_KEYS)
    {
        loamock_keys[loamock_keyNum].key = key;
        loamock_keys[loamock_keyNum].pos = pos;
        ++loamock_keyNum;
    }
}

void LOAMOCK_ForgetKeys(void)
{
    loamock_keyNum = 0;
}

int BIG_ul_SearchKeyToPos(uint32_t key)
{
    int i;
    for (i = 0; i < loamock_keyNum; ++i)
        if (loamock_keys[i].key == key)
            return loamock_keys[i].pos;
    return -1;
}

/* --- the rest of loading.c's business, never reached here --------- */

static void loamock_unreachable(const char* what)
{
    printf("  FAIL a mock the driver should never reach was called: %s\n", what);
    exit(1);
}

/* Called at the end of every resolve pass; there is no cache here. */
void BIG_FreeGlobalBuffer() { }
int BIG_SpeedMode_fread(char** o, int s, FILE* f)
{
    (void)o; (void)s; (void)f; loamock_unreachable("BIG_SpeedMode_fread"); return 0;
}
int BIG_fseek(FILE* f, int o, int w)
{
    (void)f; (void)o; (void)w; loamock_unreachable("BIG_fseek"); return 0;
}
long long FIO_i64_Tell(FILE* f) { (void)f; loamock_unreachable("FIO_i64_Tell"); return 0; }
int DPT_b_ExePath(const char* n, char* o, size_t c)
{
    (void)n; (void)o; (void)c; return 0;      /* no special-key file */
}
const char* SET_pc_SpecialArray(void) { return "jade.spe"; }
void DBG_assert(int isError, char* where, int line, char* expr,
                const char* err, const char* extra, int box)
{
    (void)where; (void)line; (void)expr; (void)extra; (void)box;
    if (isError)
    {
        printf("  FAIL the code under test asserted: %s\n", err ? err : "(no text)");
        exit(1);
    }
}

void SND_HeaderLoadArray(BASStruct* a, int k) { (void)a; (void)k; loamock_unreachable("SND_HeaderLoadArray"); }
void SND_HeaderMakeArray(BASStruct* a) { (void)a; loamock_unreachable("SND_HeaderMakeArray"); }
void SND_LoadAllSounds() { loamock_unreachable("SND_LoadAllSounds"); }
void SND_NotifyNewLoading() { loamock_unreachable("SND_NotifyNewLoading"); }
int SND_ResolveStaticPrefetchForSpecialMdF() { loamock_unreachable("SND_ResolveStaticPrefetchForSpecialMdF"); return 0; }
void SND_StreamResolveSyncPrefetch() { loamock_unreachable("SND_StreamResolveSyncPrefetch"); }
void SND_StreamStopAll() { loamock_unreachable("SND_StreamStopAll"); }

int BIGcomp_M_CompressedSize;
int BIGcomp_M_BlockSize;
int gi_TestBin;
int ENG_PlatformID;

/* --- what ai_common.c refers to, for the two small functions in it
       that the driver actually calls -------------------------------- */

#include "../ai.h"
#include "../gameobject.h"
#include "../profiler.h"

/* The dispatch tables, except the type table: that one is the real
   ai/ai_types.c, because the stack driver copies values through its
   accessors. Zeroed is right for the rest -- nothing here runs a node. */
tOfc_scriptengine_functions AI_gast_Categs[16];
tOfc_scriptengine_functions AI_gast_Keywords[74];
tOfc_scriptengine_functions AI_gast_Functions[857];
tOfc_scriptengine_functions AI_gast_Fields[27];
struct sGameObjectMain** AI_MainActor;

unsigned int AI2C_i_SizeofArray() { return 0; }
void AI_EvalFunc_GRPSectUpdateMe_C(tGameObjectMain* g) { (void)g; loamock_unreachable("AI_EvalFunc_GRPSectUpdateMe_C"); }
char* BIG_pc_ReadFileTmp(int o, int* s) { (void)o; (void)s; loamock_unreachable("BIG_pc_ReadFileTmp"); return 0; }
void DBG_OutputDebugString(char* f, ...) { (void)f; }
void MATH_NormalizeEqualVector(Vec3* v) { (void)v; }
void MATH_SubstractVec(Vec3* d, Vec3* a, Vec3* b) { (void)d; (void)a; (void)b; }
void MATH_VecAddVec(Vec3* d, Vec3* a, Vec3* b) { (void)d; (void)a; (void)b; }
float MATH_VecLen(Vec3* v) { (void)v; return 0.0f; }
void MEM_FreeAlign(void* p) { free(p); }
void* MEM_p_AllocAligned(int s, int a) { (void)a; return malloc((size_t)(s > 0 ? s : 1)); }
void* MEM_p_AllocFromEnd(int s) { return malloc((size_t)(s > 0 ? s : 1)); }
void OBJ_SetProcess(tGameObjectMain* g, char c, char m) { (void)g; (void)c; (void)m; }
int OBJ_b_HasBeenProcess(tGameObjectMain* g, char c, int m) { (void)g; (void)c; (void)m; return 0; }
void PRO_QPC_RecordTimeDecRef(int id) { (void)id; }
void PRO_RecordTimeDecRef(profilerRasterStructElem* r) { (void)r; }
void SIDE_Free(char* b) { (void)b; }
BOOL SIDE_b_Overrides(uint32_t k) { (void)k; return 0; }
char* SIDE_pc_ReadByKey(uint32_t k, int* s) { (void)k; (void)s; return 0; }
BOOL TAB_TableElemIsInvalid(void* e) { (void)e; return 0; }
unsigned int TIM_GetTicksAsInt32() { return 0; }

/* The compiled-script table, which is a definition elsewhere and only
   its address is wanted here. */
#include "../ai/ai_compiledscripts.h"
tStruct_funclist AI2C_gat_fctdefs[1];

/* The timer the profiler reads, which the driver never gets near. */
#include "../platform/syscompat.h"
int QueryPerformanceCounter(LARGE_INTEGER* out)
{
    if (out)
        out->QuadPart = 0;
    return 1;
}
