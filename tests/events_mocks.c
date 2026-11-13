/* Stand-ins for everything events.c calls, for the differential test.

   Both copies of events.c run over the same scene buffer at the same
   addresses, one after the other, so the allocator hands out the same
   blocks to each run and pointers can be compared directly. Every
   observable call is recorded, because comparing the final state alone
   would miss a dropped or reordered engine call that happens not to
   change the numbers.

   The real math.c is linked rather than mocked: the rewrite replaced
   inlined arithmetic with MATH_VecMultScalar and MATH_VecAddVec calls,
   so the comparison is only worth anything if those are the engine's
   own. */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../memory.h"
#include "../loading.h"
#include "../bigfiles.h"
#include "../gameobject.h"
#include "../animation.h"
#include "../modifiers/leglinkmodifier.h"
#include "../dbg.h"
#include "events_mocks.h"

/* ---------------------------------------------------------------- log */

static char MOCK_log[MOCK_LOG_MAX][MOCK_LOG_LINE];
static int MOCK_logNum;
static int MOCK_logOverflow;

void MOCK_LogReset(void)
{
    int i;

    /* only what the last run filled: clearing the whole table costs more
       than everything else the test does */
    for (i = 0; i < MOCK_logNum && i < MOCK_LOG_MAX; ++i)
        MOCK_log[i][0] = '\0';
    MOCK_logNum = 0;
    MOCK_logOverflow = 0;
}

int MOCK_LogNum(void)
{
    return MOCK_logNum;
}

const char* MOCK_LogLine(int i)
{
    return MOCK_log[i];
}

int MOCK_LogOverflowed(void)
{
    return MOCK_logOverflow;
}

static void MOCK_Logf(const char* fmt, ...)
{
    va_list ap;

    if (MOCK_logNum >= MOCK_LOG_MAX)
    {
        MOCK_logOverflow = 1;
        return;
    }
    va_start(ap, fmt);
    vsnprintf(MOCK_log[MOCK_logNum], MOCK_LOG_LINE, fmt, ap);
    va_end(ap);
    ++MOCK_logNum;
}

/* ---------------------------------------------------------- allocator */

static uint8_t MOCK_heap[512 * 1024];
static unsigned int MOCK_heapUsed;

const void* MOCK_HeapBase(void)
{
    return MOCK_heap;
}

unsigned int MOCK_HeapUsed(void)
{
    return MOCK_heapUsed;
}

void MOCK_HeapReset(void)
{
    MOCK_heapUsed = 0;
    memset(MOCK_heap, 0xCD, sizeof(MOCK_heap));
}

void* MEM_p_Alloc(unsigned int size)
{
    void* p;

    if (MOCK_heapUsed + size + 16u > sizeof(MOCK_heap))
    {
        printf("the events test ran out of mock heap\n");
        abort();
    }
    p = MOCK_heap + MOCK_heapUsed;
    MOCK_heapUsed += (size + 15u) & ~15u;
    MOCK_Logf("MEM_p_Alloc(%u) -> +%u", size, (unsigned int)((uint8_t*)p - MOCK_heap));
    return p;
}

void* MEM_p_Realloc(void* Src, size_t Size)
{
    void* p;

    p = MEM_p_Alloc((unsigned int)Size);
    if (Src)
        memcpy(p, Src, Size);
    MOCK_Logf("MEM_p_Realloc(+%d, %u)",
              Src ? (int)((uint8_t*)Src - MOCK_heap) : -1, (unsigned int)Size);
    return p;
}

void MEM_Free(void* ptr)
{
    MOCK_Logf("MEM_Free(+%d)", ptr ? (int)((uint8_t*)ptr - MOCK_heap) : -1);
}

/* Declared extern by the engine headers, so defined here. */
int BIG_gi_SpecialRead;

void* MEM_p_AllocAligned(int size, int alignment) { return MEM_p_Alloc((unsigned int)size); }
void* MEM_p_ReallocAligned(void* ptr, unsigned int size, int alignment) { return MEM_p_Realloc(ptr, size); }
void MEM_FreeAlign(void* ptr) { MEM_Free(ptr); }

/* ------------------------------------------------------------ pointers

   A pointer the engine is handed is logged by where it lives, not by
   its value: the two copies have different frame layouts, so a matrix
   that is a caller's local sits at a different address in each run.
   Scene and heap pointers are logged as offsets; anything else gets a
   number in the order it is first seen, which is stable because the two
   runs make the same calls in the same order. */

static const uint8_t* MOCK_sceneBase;
static unsigned int MOCK_sceneSize;
static const void* MOCK_frames[64];
static int MOCK_frameNum;

void MOCK_RegisterScene(const void* base, unsigned int size)
{
    MOCK_sceneBase = (const uint8_t*)base;
    MOCK_sceneSize = size;
}

void MOCK_FramesReset(void)
{
    MOCK_frameNum = 0;
    memset(MOCK_frames, 0, sizeof(MOCK_frames));
}

static const char* MOCK_Ptr(const void* p)
{
    static char buf[4][40];
    static int turn;
    char* out;
    int i;

    out = buf[turn++ & 3];
    if (!p)
    {
        strcpy(out, "null");
        return out;
    }
    if (MOCK_sceneBase
        && (const uint8_t*)p >= MOCK_sceneBase
        && (const uint8_t*)p < MOCK_sceneBase + MOCK_sceneSize)
    {
        sprintf(out, "S+%u", (unsigned int)((const uint8_t*)p - MOCK_sceneBase));
        return out;
    }
    if ((const uint8_t*)p >= MOCK_heap && (const uint8_t*)p < MOCK_heap + sizeof(MOCK_heap))
    {
        sprintf(out, "H+%u", (unsigned int)((const uint8_t*)p - MOCK_heap));
        return out;
    }
    for (i = 0; i < MOCK_frameNum; ++i)
        if (MOCK_frames[i] == p)
        {
            sprintf(out, "F%d", i);
            return out;
        }
    if (MOCK_frameNum < 64)
    {
        MOCK_frames[MOCK_frameNum] = p;
        sprintf(out, "F%d", MOCK_frameNum);
        ++MOCK_frameNum;
        return out;
    }
    strcpy(out, "F?");
    return out;
}

/* ------------------------------------------------------------- loader */

/* One deterministic byte stream, rewound between the two runs. */
static char MOCK_file[4096];
static int MOCK_fileSize;

void MOCK_FileSet(const unsigned char* bytes, int num)
{
    memset(MOCK_file, 0, sizeof(MOCK_file));
    if (num > (int)sizeof(MOCK_file))
        num = (int)sizeof(MOCK_file);
    memcpy(MOCK_file, bytes, num);
    MOCK_fileSize = num;
}

char* MOCK_FileBase(void)
{
    return MOCK_file;
}

char* BIG_pc_ReadFileTmp(int Offset, int* size)
{
    *size = MOCK_fileSize;
    MOCK_Logf("BIG_pc_ReadFileTmp(%d)", Offset);
    return MOCK_file;
}

char LOA_ReadChar(char** offset, char* retVal, char readMode)
{
    char v;

    v = **offset;
    *offset += 1;
    if (retVal)
        *retVal = v;
    MOCK_Logf("LOA_ReadChar(mode %d) -> %d", (int)readMode, (int)v);
    return v;
}

uint16_t LOA_ReadShort(char** offset, uint16_t* retVal, char readMode)
{
    uint16_t v;

    memcpy(&v, *offset, sizeof(v));
    *offset += 2;
    if (retVal)
        *retVal = v;
    MOCK_Logf("LOA_ReadShort(mode %d) -> %u", (int)readMode, (unsigned int)v);
    return v;
}

int LOA_ReadLong(char** offset, uint32_t* retVal, char readMode)
{
    uint32_t v;

    memcpy(&v, *offset, sizeof(v));
    *offset += 4;
    if (retVal)
        *retVal = v;
    MOCK_Logf("LOA_ReadLong(mode %d) -> %u", (int)readMode, v);
    return (int)v;
}

float LOA_ReadFloat(char** offset, float* a2, char readMode)
{
    float v;

    memcpy(&v, *offset, sizeof(v));
    *offset += 4;
    if (a2)
        *a2 = v;
    MOCK_Logf("LOA_ReadFloat(mode %d) -> %.9g", (int)readMode, (double)v);
    return v;
}

void LOA_ReadString(char** offset, char* retVal, int len, char readMode)
{
    if (retVal)
        memcpy(retVal, *offset, len);
    *offset += len;
    MOCK_Logf("LOA_ReadString(%d, mode %d)", len, (int)readMode);
}

void LOA_ReadVector(char** offset, Vec3* retVal, int readMode)
{
    if (retVal)
        memcpy(retVal, *offset, sizeof(Vec3));
    *offset += sizeof(Vec3);
    MOCK_Logf("LOA_ReadVector(mode %d)", readMode);
}

void LOA_ReadMatrix(char** offset, Mtx4x4* retVal, int readMode)
{
    /* The engine's reader takes all 80 bytes off the stream. */
    if (retVal)
        memcpy(retVal, *offset, 80);
    *offset += 80;
    MOCK_Logf("LOA_ReadMatrix(mode %d)", readMode);
}

void LOA_ReadQuaternion(char** offset, Quat4* retVal, int readMode)
{
    if (retVal)
        memcpy(retVal, *offset, sizeof(Quat4));
    *offset += sizeof(Quat4);
    MOCK_Logf("LOA_ReadQuaternion(mode %d)", readMode);
}

void LOA_ReadCompressedQuaternion(char** offset, CompressedQuat* retVal, char readMode)
{
    if (retVal)
        memcpy(retVal, *offset, sizeof(CompressedQuat));
    *offset += sizeof(CompressedQuat);
    MOCK_Logf("LOA_ReadCompressedQuaternion(mode %d)", (int)readMode);
}

void LOA_ReadUltraCompressedQuaternion(char** offset, UltraCompressedQuat* retVal, char readMode)
{
    if (retVal)
        memcpy(retVal, *offset, sizeof(UltraCompressedQuat));
    *offset += sizeof(UltraCompressedQuat);
    MOCK_Logf("LOA_ReadUltraCompressedQuaternion(mode %d)", (int)readMode);
}

void LOA_ReadAINode(char** offset, tAINode* retVal, int readMode)
{
    if (retVal)
        memcpy(retVal, *offset, sizeof(tAINode));
    *offset += sizeof(tAINode);
    MOCK_Logf("LOA_ReadAINode(mode %d)", readMode);
}

void LOA_DeleteAddress(int a1)
{
    MOCK_Logf("LOA_DeleteAddress()");
}

void LOA_MakeFileRef(int key, void** retVal, void* (*func)(uint32_t), char flags)
{
    MOCK_Logf("LOA_MakeFileRef(%d, flags %d)", key, (int)flags);
}

BOOL LOA_IsSpecial(unsigned int a1)
{
    MOCK_Logf("LOA_IsSpecial(%u)", a1);
    return MOCK_isSpecial;
}

int LOA_ul_GetCurrentKey()
{
    MOCK_Logf("LOA_ul_GetCurrentKey()");
    return 0x1234;
}

int MOCK_isSpecial;

/* ------------------------------------------------------- game objects */

static Mtx4x4 MOCK_scratchMtx;
static tGaoMatrix MOCK_scratchGaoMtx;

Mtx4x4* OBJ_pst_GetAbsoluteMatrix(tGameObjectMain* gao)
{
    MOCK_Logf("OBJ_pst_GetAbsoluteMatrix(%s)", MOCK_Ptr(gao));
    if (gao && gao->matrices)
        return gao->matrices;
    return &MOCK_scratchMtx;
}

tGaoMatrix* OBJ_pst_GetLocalMatrix(tGameObjectMain* gao)
{
    MOCK_Logf("OBJ_pst_GetLocalMatrix(%s)", MOCK_Ptr(gao));
    if (gao && gao->basic && gao->basic->mtx)
        return gao->basic->mtx;
    return &MOCK_scratchGaoMtx;
}

void OBJ_ReinitMatrixStorageByFlags(tGameObjectMain* gao, Mtx4x4* mtx)
{
    MOCK_Logf("OBJ_ReinitMatrixStorageByFlags(%s, %s)", MOCK_Ptr(gao), MOCK_Ptr(mtx));
}

void OBJ_ComputeLocalWhenHie(struct sGameObjectMain* gao)
{
    MOCK_Logf("OBJ_ComputeLocalWhenHie(%s)", MOCK_Ptr(gao));
}

BOOL OBJ_b_IsChild(struct sGameObjectMain* gao)
{
    MOCK_Logf("OBJ_b_IsChild(%s)", MOCK_Ptr(gao));
    return gao && gao->basic && gao->basic->mtx && gao->basic->mtx->refGao;
}

void OBJ_ComputeGlobalWithLocal(struct sGameObjectMain* gao, Mtx4x4* dest, int a3)
{
    MOCK_Logf("OBJ_ComputeGlobalWithLocal(%s, %s, %d)", MOCK_Ptr(gao), MOCK_Ptr(dest), a3);
    if (dest)
    {
        MATH_SetIdentityMatrix(dest);
        dest->Pos.x = 1.0f;
        dest->Pos.y = 2.0f;
        dest->Pos.z = 3.0f;
    }
}

struct sGameObjectMain* OBJ_ul_GameObjectCallback(uint32_t Offset)
{
    MOCK_Logf("OBJ_ul_GameObjectCallback(%u)", Offset);
    return 0;
}

void ANI_UpdateFlash(struct sGameObjectMain* gao, char which)
{
    MOCK_Logf("ANI_UpdateFlash(%s, %d)", MOCK_Ptr(gao), (int)which);
}

void GAO_ModifierLegLink_Compute(Mtx4x4* hip, Mtx4x4* knee, Mtx4x4* foot,
                                 float upperLen, float lowerLen)
{
    MOCK_Logf("GAO_ModifierLegLink_Compute(%s, %s, %s, %.9g, %.9g)",
              MOCK_Ptr(hip), MOCK_Ptr(knee), MOCK_Ptr(foot),
              (double)upperLen, (double)lowerLen);
    if (foot)
        foot->Pos.z = foot->Pos.z + 0.25f;
}

/* ---------------------------------------------------------------- ai */

int MOCK_aiStackValue;

void AI_RunNode(tAINode* node)
{
    MOCK_Logf("AI_RunNode(%s)", MOCK_Ptr(node));
    AI_gul_SizeGlobalStack = 4;
    memcpy(AI_gac_GlobalStack, &MOCK_aiStackValue, sizeof(int));
}

int AI_GetIntFromStack()
{
    MOCK_Logf("AI_GetIntFromStack() -> %d", MOCK_aiStackValue);
    return MOCK_aiStackValue;
}

BOOL AI_SpecialWParam(char a1)
{
    MOCK_Logf("AI_SpecialWParam(%d)", (int)a1);
    return (a1 & 1) != 0;
}

BOOL AI_b_IsARef(int a1)
{
    MOCK_Logf("AI_b_IsARef(%d)", a1);
    return a1 != 0;
}

int AI_ul_ResolveOneRef(int a1, int a2)
{
    MOCK_Logf("AI_ul_ResolveOneRef(%d, %d)", a1, a2);
    return a1 + a2;
}
