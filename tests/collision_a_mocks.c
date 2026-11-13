/* Everything collision.c reaches outside math.c, for the range-A
   differential test. The allocator is a bump allocator that never
   frees, so the two copies' state stays separate and a pointer the
   base copy wrote is still readable while the new copy runs. Every
   other entry point aborts: the test drives the intersection geometry
   and a handful of table walks, and anything that reaches the world,
   the loader or the display device is a case the test should not be
   claiming to cover. */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../gameobject.h"
#include "../memory.h"
#include "../loading.h"
#include "../bigfiles.h"
#include "../graphicsobjects/geo.h"
#include "../graphicsobjects/staticlod.h"
#include "../dbg.h"
#include "../snp.h"
#include "../animation.h"
#include "../time.h"
#include "../collision.h"

static void unreached(const char* name)
{
    fprintf(stderr, "mock reached that should not be: %s\n", name);
    abort();
}

/* ---- allocator ---------------------------------------------------- */

#define ARENA_BYTES (8 * 1024 * 1024)
static unsigned char arena[ARENA_BYTES];
static size_t arenaUsed;

void COLA_MockResetArena(void) { arenaUsed = 0; }
size_t COLA_MockArenaUsed(void) { return arenaUsed; }

void* MEM_p_Alloc(unsigned int size)
{
    size_t need = (size_t)(size == 0 ? 1 : size);
    void* p;

    need = (need + 15u) & ~(size_t)15u;
    if (arenaUsed + need > ARENA_BYTES)
        unreached("MEM_p_Alloc: arena exhausted");
    p = arena + arenaUsed;
    arenaUsed += need;
    memset(p, 0, need);
    return p;
}

void* MEM_p_Realloc(void* old, size_t size)
{
    void* p = MEM_p_Alloc((unsigned int)size);
    (void)old;
    return p;
}

void MEM_Free(void* p) { (void)p; }

void* MEM_p_AllocAligned(int size, int align)
{
    (void)align;
    return MEM_p_Alloc((unsigned int)size);
}

void* MEM_p_ReallocAligned(void* old, unsigned int size, int align)
{
    (void)old;
    (void)align;
    return MEM_p_Alloc(size);
}

void MEM_FreeAlign(void* p) { (void)p; }

/* Globals the engine defines elsewhere that collision.c reads. */
float TIM_gf_SpeedFactor = 1.0f;
BOOL MEM_b_CheckBlockHasBeenFreed(void* p) { (void)p; return 0; }

/* ---- engine subsystems -------------------------------------------- */

void LOA_DeleteAddress(int a) { (void)a; }
void COL_OK3_Build(void* a, int b, int c) { (void)a; (void)b; (void)c; unreached("COL_OK3_Build"); }
void DBG_assert(int a, char* b, int c, char* d, const char* e, const char* f, int g)
{
    (void)a; (void)b; (void)c; (void)d; (void)e; (void)f; (void)g;
    unreached("DBG_assert");
}
void GEO_MRM_SetNumberOfPoints_Percent(struct sGEOGRO* a, float b) { (void)a; (void)b; }
char LOA_ReadChar(char** offset, char* retVal, char readMode) { (void)offset; (void)retVal; (void)readMode; unreached("LOA_ReadChar"); return 0; }
char* BIG_pc_ReadFileTmp(int Offset, int* size) { (void)Offset; (void)size; unreached("BIG_pc_ReadFileTmp"); return 0; }
float LOA_ReadFloat(char** offset, float* out, char readMode) { (void)offset; (void)out; (void)readMode; unreached("LOA_ReadFloat"); return 0; }
int INT_SnP_FullGetFlag(int* a1, int a2, int a3, int a4, int* a5, int* a6) { (void)a1; (void)a2; (void)a3; (void)a4; (void)a5; (void)a6; unreached("INT_SnP_FullGetFlag"); return 0; }
int INT_SnP_GetFlag(int* a1, char offset, int idx) { (void)a1; (void)offset; (void)idx; unreached("INT_SnP_GetFlag"); return 0; }
int LOA_ReadLong(char** offset, uint32_t* retVal, char readMode) { (void)offset; (void)retVal; (void)readMode; unreached("LOA_ReadLong"); return 0; }
int OBJ_b_test_flag(struct sGameObjectMain* gao, int flagMustHave, int flagMustNothave, int whatToTest) { (void)gao; (void)flagMustHave; (void)flagMustNothave; (void)whatToTest; unreached("OBJ_b_test_flag"); return 0; }
struct sGEOGRO* OBJ_p_GetCurrentGeo(struct sGameObjectMain* a1) { (void)a1; unreached("OBJ_p_GetCurrentGeo"); return 0; }
struct sGameObjectMain* get_reference_point_gao_2_from_gao_sub_40BEA0(struct sGameObjectMain* a1) { (void)a1; unreached("get_reference_point_gao_2_from_gao_sub_40BEA0"); return 0; }
uint16_t LOA_ReadShort(char** offset, uint16_t* retVal, char readMode) { (void)offset; (void)retVal; (void)readMode; unreached("LOA_ReadShort"); return 0; }
unsigned int TIM_GetTicksAsInt32(void) {  unreached("TIM_GetTicksAsInt32"); return 0; }
void ANI_ApplyHierarchyOnFlash(struct sGameObjectMain* gao) { (void)gao; unreached("ANI_ApplyHierarchyOnFlash"); }
void ANI_RemoveHierarchyOnFlash(struct sGameObjectMain* gao) { (void)gao; unreached("ANI_RemoveHierarchyOnFlash"); }
void ANI_UpdateFlashAfterMagicBoxTranslation(struct sGameObjectMain* gao, Vec3* offset, char which) { (void)gao; (void)offset; (void)which; unreached("ANI_UpdateFlashAfterMagicBoxTranslation"); }
void DYN_ApplySpeedLimit(struct sDyn* dyn) { (void)dyn; unreached("DYN_ApplySpeedLimit"); }
void DYN_CalcReboundVector(Vec3* reboundVec, Vec3* normal, Vec3* speed, float scale) { (void)reboundVec; (void)normal; (void)speed; (void)scale; unreached("DYN_CalcReboundVector"); }
void DYN_CalcSlideVector(Vec3* slideVec, Vec3* normal, Vec3* speed, float scale) { (void)slideVec; (void)normal; (void)speed; (void)scale; unreached("DYN_CalcSlideVector"); }
void INT_SnP_DetachObject(struct sGameObjectMain* a1, struct sWorld* a2) { (void)a1; (void)a2; unreached("INT_SnP_DetachObject"); }
void INT_SnP_InsertionSort(struct sSnp* a1, int a2) { (void)a1; (void)a2; unreached("INT_SnP_InsertionSort"); }
void LOA_MakeFileRef(int key, void** retVal, void* (*func)(uint32_t), char flags) { (void)key; (void)retVal; (void)func; (void)flags; unreached("LOA_MakeFileRef"); }
void LOA_ReadString(char** offset, char* retVal, int len, char readMode) { (void)offset; (void)retVal; (void)len; (void)readMode; unreached("LOA_ReadString"); }
void LOA_ReadVector(char** offset, Vec3* retVal, int readMode) { (void)offset; (void)retVal; (void)readMode; unreached("LOA_ReadVector"); }
void OBJ_BV_ComputeCenter(struct sGameObjectMain* a1, Vec3* a2) { (void)a1; (void)a2; unreached("OBJ_BV_ComputeCenter"); }
void OBJ_ChangeIdentityFlags(tGameObjectMain* a1, unsigned int a2, int a3) { (void)a1; (void)a2; (void)a3; unreached("OBJ_ChangeIdentityFlags"); }
void OBJ_ComputeGlobalWhenHie(struct sGameObjectMain* a1) { (void)a1; unreached("OBJ_ComputeGlobalWhenHie"); }
void OBJ_GameObject_CreateExtended(tGameObjectMain* a1) { (void)a1; unreached("OBJ_GameObject_CreateExtended"); }
void PRO_PerfCounterStartBlock(profilerRasterStructElem* a1) { (void)a1; unreached("PRO_PerfCounterStartBlock"); }

/* OBJ_BV_IsSphere / OBJ_BV_IsAABBox / OBJ_pst_GetAbsoluteMatrix are
   driven by the test, so they read the fixture rather than abort. */

BOOL OBJ_BV_IsSphere(tGameObjectBV* a1)
{
    return (*((uint32_t *)&(a1->BVMax.z))) == 0xffffffff;
}

BOOL OBJ_BV_IsAABBox(tGameObjectBV* a1)
{
    return (*((uint32_t*)&(a1->BVMax.z))) != 0xffffffff;
}

Mtx4x4* OBJ_pst_GetAbsoluteMatrix(tGameObjectMain* a1)
{
    return a1->matrices;
}
