/* Engine stand-ins for the collision.c differential test.

   Only the calls the tested functions actually make are implemented.
   Everything else collision.c references aborts the process if it is
   reached, so a rewrite that starts calling something new fails the
   test instead of passing on a stand-in.

   The two copies in the binary share these mocks, so a mock only has
   to be deterministic, not faithful to the engine. The three that do
   have to be faithful are OBJ_BV_IsAABBox, OBJ_BV_IsSphere and
   OBJ_pst_GetAbsoluteMatrix: the tested code branches on them, and
   they are copied from gameobject.c. */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../gameobject.h"
#include "../collision.h"
#include "../loading.h"
#include "../bigfiles.h"
#include "../dbg.h"

static void unreachable(const char* name)
{
    fprintf(stderr, "FATAL: mock %s reached -- the test does not model it\n", name);
    abort();
}

/* ---- faithful, copied from gameobject.c -------------------------- */

BOOL OBJ_BV_IsSphere(tGameObjectBV* bv)
{
    return (*((uint32_t*)&(bv->BVMax.z))) == 0xffffffff;
}

BOOL OBJ_BV_IsAABBox(tGameObjectBV* bv)
{
    return (*((uint32_t*)&(bv->BVMax.z))) != 0xffffffff;
}

Mtx4x4* OBJ_pst_GetAbsoluteMatrix(tGameObjectMain* gao)
{
    return gao->matrices;
}

/* ---- deterministic stand-ins the tested paths need --------------- */

int OBJ_b_test_flag(struct sGameObjectMain* gao, int flagMustHave, int flagMustNothave, int whatToTest)
{
    if (!gao)
        return 0;
    if (whatToTest == 0)
        return (gao->flags & flagMustHave) == flagMustHave && (gao->flags & flagMustNothave) == 0;
    return (gao->flags2 & flagMustHave) == flagMustHave && (gao->flags2 & flagMustNothave) == 0;
}

void OBJ_BV_ComputeCenter(struct sGameObjectMain* gao, Vec3* out)
{
    tGameObjectBV* bv = gao->boundingVolume;

    out->x = (float)((bv->BVMin.x + bv->BVMax.x) * 0.5);
    out->y = (float)((bv->BVMin.y + bv->BVMax.y) * 0.5);
    out->z = (float)((bv->BVMin.z + bv->BVMax.z) * 0.5);
}

/* Sweep-and-prune: a fixed pattern, so the two copies see the same
   answers for the same object pair. */
int INT_SnP_FullGetFlag(int* flags, int a, int b, int stride, int* outIndex, int* outSpan)
{
    (void)flags; (void)stride;
    *outIndex = a;
    *outSpan = b;
    return ((a + b) & 1) == 0;
}

int INT_SnP_GetFlag(int* flags, char index, int span)
{
    (void)flags;
    return ((index + span) & 3) != 3;
}

/* The allocator. Nothing is freed: a leak keeps the two copies' state
   apart, and no tested function depends on reuse. */
void* MEM_p_Alloc(unsigned int size)
{
    void* p = calloc(1, size ? size : 1);
    if (!p) abort();
    return p;
}

void* MEM_p_Realloc(void* src, size_t size)
{
    void* p = realloc(src, size ? size : 1);
    if (!p) abort();
    return p;
}

void MEM_Free(void* p) { (void)p; }
BOOL MEM_b_CheckBlockHasBeenFreed(void* p) { (void)p; return 0; }

void DBG_assert(int isError, char* file, int line, char* text, const char* error, const char* extra, int box)
{
    (void)isError; (void)file; (void)line; (void)text; (void)error; (void)extra; (void)box;
}

unsigned int TIM_GetTicksAsInt32(void) { return 0; }

float TIM_gf_SpeedFactor = 1.0f;

/* ---- everything else: reached means the test is wrong ------------ */

void ANI_ApplyHierarchyOnFlash(struct sGameObjectMain* g) { (void)g; unreachable("ANI_ApplyHierarchyOnFlash"); }
void ANI_RemoveHierarchyOnFlash(struct sGameObjectMain* g) { (void)g; unreachable("ANI_RemoveHierarchyOnFlash"); }
void ANI_UpdateFlashAfterMagicBoxTranslation(struct sGameObjectMain* g, Vec3* o, char w) { (void)g; (void)o; (void)w; unreachable("ANI_UpdateFlashAfterMagicBoxTranslation"); }
char* BIG_pc_ReadFileTmp(int offset, int* size) { (void)offset; (void)size; unreachable("BIG_pc_ReadFileTmp"); return 0; }
void COL_OK3_Build(void* a, int t, int b) { (void)a; (void)t; (void)b; unreachable("COL_OK3_Build"); }
void DYN_ApplySpeedLimit(struct sDyn* d) { (void)d; unreachable("DYN_ApplySpeedLimit"); }
void DYN_CalcReboundVector(Vec3* r, Vec3* n, Vec3* s, float f) { (void)r; (void)n; (void)s; (void)f; unreachable("DYN_CalcReboundVector"); }
void DYN_CalcSlideVector(Vec3* r, Vec3* n, Vec3* s, float f) { (void)r; (void)n; (void)s; (void)f; unreachable("DYN_CalcSlideVector"); }
void GEO_MRM_SetNumberOfPoints_Percent(struct sGEOGRO* g, float p) { (void)g; (void)p; unreachable("GEO_MRM_SetNumberOfPoints_Percent"); }
void INT_SnP_DetachObject(struct sGameObjectMain* g, struct sWorld* w) { (void)g; (void)w; unreachable("INT_SnP_DetachObject"); }
void INT_SnP_InsertionSort(struct sSnp* s, int a) { (void)s; (void)a; unreachable("INT_SnP_InsertionSort"); }
void LOA_DeleteAddress(int pos) { (void)pos; unreachable("LOA_DeleteAddress"); }
void LOA_MakeFileRef(int key, void** out, void* (*fn)(uint32_t), char flags) { (void)key; (void)out; (void)fn; (void)flags; unreachable("LOA_MakeFileRef"); }
char LOA_ReadChar(char** p, char* r, char m) { (void)p; (void)r; (void)m; unreachable("LOA_ReadChar"); return 0; }
float LOA_ReadFloat(char** p, float* r, char m) { (void)p; (void)r; (void)m; unreachable("LOA_ReadFloat"); return 0; }
int LOA_ReadLong(char** p, uint32_t* r, char m) { (void)p; (void)r; (void)m; unreachable("LOA_ReadLong"); return 0; }
uint16_t LOA_ReadShort(char** p, uint16_t* r, char m) { (void)p; (void)r; (void)m; unreachable("LOA_ReadShort"); return 0; }
void LOA_ReadString(char** p, char* r, int l, char m) { (void)p; (void)r; (void)l; (void)m; unreachable("LOA_ReadString"); }
void LOA_ReadVector(char** p, Vec3* r, int m) { (void)p; (void)r; (void)m; unreachable("LOA_ReadVector"); }
void OBJ_ChangeIdentityFlags(tGameObjectMain* g, unsigned int f, int s) { (void)g; (void)f; (void)s; unreachable("OBJ_ChangeIdentityFlags"); }
void OBJ_ComputeGlobalWhenHie(struct sGameObjectMain* g) { (void)g; unreachable("OBJ_ComputeGlobalWhenHie"); }
void OBJ_GameObject_CreateExtended(tGameObjectMain* g) { (void)g; unreachable("OBJ_GameObject_CreateExtended"); }
struct sGEOGRO* OBJ_p_GetCurrentGeo(struct sGameObjectMain* g) { (void)g; unreachable("OBJ_p_GetCurrentGeo"); return 0; }
void PRO_PerfCounterStartBlock(profilerRasterStructElem* e) { (void)e; unreachable("PRO_PerfCounterStartBlock"); }
struct sGameObjectMain* get_reference_point_gao_2_from_gao_sub_40BEA0(struct sGameObjectMain* g) { (void)g; unreachable("get_reference_point_gao_2_from_gao_sub_40BEA0"); return 0; }
