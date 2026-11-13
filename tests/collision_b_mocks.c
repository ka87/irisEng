/* Stand-ins for everything collision.c reaches outside itself and
   math.c, plus an ordered log of the calls made. Both copies under
   test call the same unprefixed mocks, so the driver clears the log
   before each side and compares the two transcripts.

   Nothing here does real work: the geometry under test calls none of
   it, and the few pipeline entry points that are reachable only need
   the calls to be counted and not to crash. */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../gameobject.h"
#include "../memory.h"
#include "../snp.h"
#include "../animation.h"
#include "../dynamics.h"
#include "../profiler.h"
#include "../collision.h"
#include "collision_b_mocks.h"

char COLB_log[COLB_LOG_MAX][64];
int COLB_logLen;
int COLB_logOverflow;

void COLB_LogReset(void)
{
    COLB_logLen = 0;
    COLB_logOverflow = 0;
    memset(COLB_log, 0, sizeof(COLB_log));
}

void COLB_Log(const char* fmt, ...)
{
    va_list ap;
    if (COLB_logLen >= COLB_LOG_MAX)
    {
        COLB_logOverflow = 1;
        return;
    }
    va_start(ap, fmt);
    vsnprintf(COLB_log[COLB_logLen], sizeof(COLB_log[0]), fmt, ap);
    va_end(ap);
    COLB_logLen++;
}

/* ---- allocator ---------------------------------------------------- */

void* MEM_p_Alloc(unsigned int size)
{
    COLB_Log("MEM_p_Alloc %u", size);
    return calloc(1, size ? size : 1);
}

void* MEM_p_Realloc(void* src, size_t size)
{
    COLB_Log("MEM_p_Realloc %u", (unsigned)size);
    return realloc(src, size ? size : 1);
}

void MEM_Free(void* ptr)
{
    COLB_Log("MEM_Free %d", ptr != NULL);
}

BOOL MEM_b_CheckBlockHasBeenFreed(void* ptr)
{
    COLB_Log("MEM_b_CheckBlockHasBeenFreed");
    return 0;
}

void* MEM_p_AllocAligned(int size, int align)
{
    void* p = NULL;
    if (align < (int)sizeof(void*)) align = sizeof(void*);
    if (posix_memalign(&p, (size_t)align, size ? size : 1) != 0) return NULL;
    memset(p, 0, size ? size : 1);
    return p;
}

void* MEM_p_ReallocAligned(void* old, unsigned int size, int align)
{
    (void)old;
    return MEM_p_AllocAligned(size, align);
}

void MEM_FreeAlign(void* p) { (void)p; }

/* ---- animation / game object -------------------------------------- */

void ANI_ApplyHierarchyOnFlash(struct sGameObjectMain* gao)
{
    COLB_Log("ANI_ApplyHierarchyOnFlash");
}

void ANI_RemoveHierarchyOnFlash(struct sGameObjectMain* gao)
{
    COLB_Log("ANI_RemoveHierarchyOnFlash");
}

void ANI_UpdateFlashAfterMagicBoxTranslation(struct sGameObjectMain* gao, Vec3* offset, char which)
{
    COLB_Log("ANI_Flash %d %.9g %.9g %.9g", (int)which,
             (double)offset->x, (double)offset->y, (double)offset->z);
}

void OBJ_BV_ComputeCenter(struct sGameObjectMain* a1, Vec3* a2)
{
    COLB_Log("OBJ_BV_ComputeCenter");
    a2->x = 0.0f; a2->y = 0.0f; a2->z = 0.0f;
}

BOOL OBJ_BV_IsAABBox(tGameObjectBV* a1)
{
    COLB_Log("OBJ_BV_IsAABBox");
    return 1;
}

BOOL OBJ_BV_IsSphere(tGameObjectBV* a1)
{
    COLB_Log("OBJ_BV_IsSphere");
    return 0;
}

void OBJ_ChangeIdentityFlags(tGameObjectMain* a1, unsigned int a2, int a3)
{
    COLB_Log("OBJ_ChangeIdentityFlags %u %d", a2, a3);
}

void OBJ_ComputeGlobalWhenHie(struct sGameObjectMain* a1)
{
    COLB_Log("OBJ_ComputeGlobalWhenHie");
}

void OBJ_GameObject_CreateExtended(tGameObjectMain* a1)
{
    COLB_Log("OBJ_GameObject_CreateExtended");
}

int OBJ_b_test_flag(struct sGameObjectMain* gao, int a, int b, int c)
{
    COLB_Log("OBJ_b_test_flag");
    return 1;
}

struct sGEOGRO* OBJ_p_GetCurrentGeo(struct sGameObjectMain* a1)
{
    COLB_Log("OBJ_p_GetCurrentGeo");
    return NULL;
}

Mtx4x4* OBJ_pst_GetAbsoluteMatrix(tGameObjectMain* a1)
{
    COLB_Log("OBJ_pst_GetAbsoluteMatrix");
    return a1 ? a1->matrices : NULL;
}

struct sGameObjectMain* get_reference_point_gao_2_from_gao_sub_40BEA0(struct sGameObjectMain* a1)
{
    COLB_Log("get_ref_gao2");
    if (a1 && a1->basic && a1->basic->mtx)
        return a1->basic->mtx->refGao2;
    return NULL;
}

/* ---- dynamics ------------------------------------------------------ */

void DYN_ApplySpeedLimit(struct sDyn* dyn)
{
    COLB_Log("DYN_ApplySpeedLimit");
}

void DYN_CalcReboundVector(Vec3* out, Vec3* normal, Vec3* speed, float scale)
{
    COLB_Log("DYN_CalcReboundVector %.9g", (double)scale);
    out->x = normal->x * scale;
    out->y = normal->y * scale;
    out->z = normal->z * scale;
}

void DYN_CalcSlideVector(Vec3* out, Vec3* normal, Vec3* speed, float scale)
{
    COLB_Log("DYN_CalcSlideVector %.9g", (double)scale);
    out->x = speed->x * scale;
    out->y = speed->y * scale;
    out->z = speed->z * scale;
}

/* ---- the rest ------------------------------------------------------ */

char* BIG_pc_ReadFileTmp(int Offset, int* size) { COLB_Log("BIG_pc_ReadFileTmp"); if (size) *size = 0; return NULL; }
void COL_OK3_Build(void* aa, int type, int a3) { COLB_Log("COL_OK3_Build"); }
void DBG_assert(int a, char* b, int c, char* d, const char* e, const char* f, int g) { COLB_Log("DBG_assert"); }
void GEO_MRM_SetNumberOfPoints_Percent(struct sGEOGRO* a1, float a2) { COLB_Log("GEO_MRM"); }
void INT_SnP_DetachObject(struct sGameObjectMain* a1, struct sWorld* a2) { COLB_Log("SnP_Detach"); }
int INT_SnP_FullGetFlag(int* a1, int a2, int a3, int a4, int* a5, int* a6) { COLB_Log("SnP_FullGetFlag"); return 0; }
int INT_SnP_GetFlag(int* a1, char offset, int idx) { COLB_Log("SnP_GetFlag"); return 0; }
void INT_SnP_InsertionSort(struct sSnp* a1, int a2) { COLB_Log("SnP_InsertionSort"); }
void LOA_DeleteAddress(int pos) { COLB_Log("LOA_DeleteAddress"); }
void LOA_MakeFileRef(int key, void** retVal, void* (*func)(uint32_t), char flags) { COLB_Log("LOA_MakeFileRef"); }
char LOA_ReadChar(char** o, char* r, char m) { COLB_Log("LOA_ReadChar"); return 0; }
float LOA_ReadFloat(char** o, float* r, char m) { COLB_Log("LOA_ReadFloat"); return 0.0f; }
int LOA_ReadLong(char** o, uint32_t* r, char m) { COLB_Log("LOA_ReadLong"); return 0; }
uint16_t LOA_ReadShort(char** o, uint16_t* r, char m) { COLB_Log("LOA_ReadShort"); return 0; }
void LOA_ReadString(char** o, char* r, int l, char m) { COLB_Log("LOA_ReadString"); }
void LOA_ReadVector(char** o, Vec3* r, int m) { COLB_Log("LOA_ReadVector"); }
void PRO_PerfCounterStartBlock(profilerRasterStructElem* a1) { COLB_Log("PRO_Start"); if (a1) a1->insideCounter++; }
unsigned int TIM_GetTicksAsInt32(void) { return 0; }

float TIM_gf_SpeedFactor = 1.0f;
