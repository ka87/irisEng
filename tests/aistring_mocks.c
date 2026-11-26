/* Just enough of the engine for the AI model resolve and free.
 *
 * The five dispatch tables are the driver's own rather than the
 * engine's. That is deliberate: the real ones drag in every handler in
 * ai/, and what this check needs from them is only the funcids, which
 * AI_ResolveEnums turns into the AI_gaw_EnumLink entries the resolve
 * and the read side both compare against. The driver asserts those
 * entries come out as the engine's real tables would make them, so a
 * table here that drifts from the real one fails the run rather than
 * quietly changing what is being tested.
 */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "../common.h"
#include "../math.h"
#include "../ai.h"
#include "../handle.h"
#include "../profiler.h"
#include "../ai/ai_compiledscripts.h"

#include "aistring_mocks.h"

/* ------------------------------------------------------------ the heap */

int AISMOCK_liveBlocks;

static void* take(unsigned int size)
{
    ++AISMOCK_liveBlocks;
    return malloc(size ? size : 1);
}

static void give(void* p)
{
    if (p)
        --AISMOCK_liveBlocks;
    free(p);
}

void* MEM_p_Alloc(unsigned int size)        { return take(size); }
void* MEM_p_AllocFromEnd(int size)          { return take((unsigned)size); }
void* MEM_p_AllocAligned(int size, int a)   { (void)a; return take((unsigned)size); }
void MEM_Free(void* p)                      { give(p); }
void MEM_FreeAlign(void* p)                 { give(p); }
void MEM_FreeFromEnd(void* p)               { give(p); }

/* ------------------------------------------------- the dispatch tables */

/* Only the funcids matter here. Entry 0 of the categories is funcid 1,
   the type category, exactly as ai/ai_categories.c has it; the types
   carry 0x26, the string, at the same index the real table puts it. */

static tAINode* categ_noop(tAINode* node) { return node + 1; }

tOfc_scriptengine_functions AI_gast_Categs[16] = {
    { 0x1, 0, categ_noop },  { 0x2, 0, categ_noop },
    { 0x3, 0, categ_noop },  { 0x4, 0, categ_noop },
    { 0x5, 0, categ_noop },  { 0x6, 0, categ_noop },
    { 0x7, 0, categ_noop },  { 0x8, 0, categ_noop },
    { 0x9, 0, categ_noop },  { 0xA, 0, categ_noop },
    { 0xB, 0, categ_noop },  { 0xC, 0, categ_noop },
    { 0xD, 0, categ_noop },  { 0xE, 0, categ_noop },
    { 0xF, 0, categ_noop },  { 0x10, 0, categ_noop }
};

static void type_noop(void* a, void* b) { (void)a; (void)b; }

/* The first eleven of the real table's funcids, which is as far as the
   string at 0x26 and its neighbours go. */
tOfc_scriptengine_functions_types AI_gast_Types[23] = {
    { 0x1C, 4, type_noop }, { 0x1D, 4, type_noop }, { 0x1E, 4, type_noop },
    { 0x1F, 4, type_noop }, { 0x20, 4, type_noop }, { 0x21, 4, type_noop },
    { 0x22, 4, type_noop }, { 0x25, 12, type_noop },
    { 0x26, 4, type_noop },                         /* string */
    { 0x27, 4, type_noop }, { 0x28, 4, type_noop }, { 0x29, 4, type_noop },
    { 0x2A, 4, type_noop }, { 0x2B, 4, type_noop }, { 0x2C, 8, type_noop },
    { 0x2D, 4, type_noop }, { 0x2E, 4, type_noop }, { 0x7C, 12, type_noop },
    { 0x7D, 12, type_noop }, { 0x7E, 12, type_noop }, { 0x7F, 12, type_noop },
    { 0x80, 12, type_noop }, { 0x81, 12, type_noop }
};

tOfc_scriptengine_functions AI_gast_Keywords[AISMOCK_KEYWORDS];
tOfc_scriptengine_functions AI_gast_Fields[AISMOCK_FIELDS];
tOfc_scriptengine_functions AI_gast_Functions[AISMOCK_FUNCS];

tStruct_funclist AI2C_gat_fctdefs[1];

/* gameobject.h keeps the array itself as a tentative definition and
   the pointer as an extern, so only the pointer needs a home. The
   driver points it at its own storage rather than at index -1 of
   whatever follows. */
static struct sGameObjectMain* aismock_actors[4];
struct sGameObjectMain** AI_MainActor = &aismock_actors[1];

unsigned int AI2C_i_SizeofArray()
{
    return 0;
}


/* ------------------------------------------------- the rest, unreached */

void AI_EvalFunc_GRPSectUpdateMe_C(struct sGameObjectMain* g) { (void)g; }
char* BIG_pc_ReadFileTmp(int pos, int* len) { (void)pos; (void)len; return NULL; }
int BIG_ul_SearchKeyToPos(uint32_t key) { (void)key; return -1; }
void DBG_OutputDebugString(char* fmt, ...) { (void)fmt; }
/* The address tables are not wanted here, but the last line of the real
   one is: it gives back the handle it was passed. Without that the two
   AI_FreeModel takes for the model and the function stay live and the
   leak check below reads them as the resolve's. */
void LOA_DeleteAddress(int pos) { HDL_Release(HDL_p_Pointer((uint32_t)pos)); }
void LOA_MakeFileRef(int key, void* retVal, void* (*f)(uint32_t), char fl)
{ (void)key; (void)f; (void)fl; if (retVal) *(void**)retVal = NULL; }
void LOA_ReadAILocal(char** o, void* d, char m) { (void)o; (void)d; (void)m; }
void LOA_ReadAINode(char** o, void* d, char m) { (void)o; (void)d; (void)m; }
char** LOA_ReadCharArray(char** o, char* d, unsigned int n, char m)
{ (void)d; (void)n; (void)m; return o; }
float LOA_ReadFloat(char** o, float* r, char m)
{ (void)o; (void)r; (void)m; return 0.0f; }
int LOA_ReadLong(char** o, uint32_t* r, char m)
{ (void)o; (void)r; (void)m; return 0; }
uint16_t LOA_ReadShort(char** o, uint16_t* r, char m)
{ (void)o; (void)r; (void)m; return 0; }
void LOA_ReadString(char** o, char* d, int n, char m)
{ (void)o; (void)d; (void)n; (void)m; }
void LOA_ReadVector(char** o, Vec3* r, int m) { (void)o; (void)r; (void)m; }
void LOA_Resolve() { }
uint32_t LOA_ul_GetCurrentKey(void) { return 0; }
int LOA_ul_SearchAddress(int pos) { (void)pos; return 0; }
void MATH_NormalizeEqualVector(Vec3* v) { (void)v; }
void MATH_SubstractVec(Vec3* d, Vec3* a, Vec3* b) { (void)d; (void)a; (void)b; }
void MATH_VecAddVec(Vec3* d, Vec3* a, Vec3* b) { (void)d; (void)a; (void)b; }
float MATH_VecLen(Vec3* v) { (void)v; return 0.0f; }
void OBJ_SetProcess(struct sGameObjectMain* g, char a, char b)
{ (void)g; (void)a; (void)b; }
int OBJ_b_HasBeenProcess(struct sGameObjectMain* g, char n, int mask)
{ (void)g; (void)n; (void)mask; return 0; }
void PRO_QPC_RecordTimeDecRef(int id) { (void)id; }
void PRO_RecordTimeDecRef(profilerRasterStructElem* r) { (void)r; }
void SIDE_Free(char* p) { (void)p; }
BOOL SIDE_b_Overrides(uint32_t key) { (void)key; return 0; }
char* SIDE_pc_ReadByKey(uint32_t key, int* len)
{ (void)key; if (len) *len = 0; return NULL; }
BOOL TAB_TableElemIsInvalid(void* e) { (void)e; return 0; }
unsigned int TIM_GetTicksAsInt32() { return 0; }

int QueryPerformanceCounter(LARGE_INTEGER* out)
{
    if (out)
        out->QuadPart = 0;
    return 1;
}

void AISMOCK_Reset(void)
{
    AISMOCK_liveBlocks = 0;
}
