// Stand-ins for everything animation.c calls, for tests/test_animation.c.
//
// Two kinds, as tests/README.md describes:
//
//   - faithful copies of the real thing, for the helpers the frozen copy
//     spells out inline and the rewrite calls: OBJ_pst_GetAbsoluteMatrix,
//     OBJ_pst_GetLocalMatrix, OBJ_pst_GetFlashMatrix,
//     TAB_TableElemIsInvalid, TAB_ul_PFtable_GetElemIndexWithPointer,
//     PRO_QPC_RecordTimeDecRef and LOA_ReadChar, copied from
//     gameobject.c, tables.c, profiler.c and loading.c. math.c itself is
//     linked, so the vector and matrix helpers are the engine's own.
//   - deterministic stand-ins for the rest. Both copies call them the
//     same way, so they only have to be reproducible, and they record
//     the call.
//
// Everything is allocated out of one arena at a fixed address, so the two
// runs see identical pointers and the whole arena can be compared.

#include "../gameobject.h"
#include "../memory.h"
#include "../loading.h"
#include "../events.h"
#include "../math.h"
#include "../time.h"
#include "../bigfiles.h"
#include "../dbg.h"
#include "../animation.h"
#include "../graphicsobjects/gro_common.h"
#include "animation_mocks.h"

#include <stdio.h>
#include <string.h>

char ANIT_arena[ANIT_ARENA_SIZE];
size_t ANIT_arenaUsed;

static int ANIT_runCalls;
ANIT_LogEntry ANIT_log[ANIT_LOG_MAX];
int ANIT_logN;
int ANIT_seq;

size_t ANIT_mtxOff[ANIT_MTX_MAX];
int ANIT_mtxN;

void ANIT_ResetArena(void)
{
    memset(ANIT_arena, 0, ANIT_ARENA_SIZE);
    ANIT_arenaUsed = 0;
    ANIT_mtxN = 0;
}

// A Mtx4x4's three trailing ints are not written by any MATH function, so
// a matrix built by one and then copied carries whatever was in the
// caller's frame. Both copies do that in the same places, but their
// frames differ, so those words are registered here and left out of the
// comparison.
void ANIT_MarkMtx(void* p, int n)
{
    int i;

    for (i = 0; i < n; i++)
    {
        long off = ANIT_Ref((char*)p + (size_t)i * sizeof(Mtx4x4));
        if (off < 0 || ANIT_mtxN >= ANIT_MTX_MAX)
            continue;
        ANIT_mtxOff[ANIT_mtxN++] = (size_t)off;
    }
}

int ANIT_SkipWord(size_t off)
{
    int i;

    for (i = 0; i < ANIT_mtxN; i++)
    {
        size_t b = ANIT_mtxOff[i];
        if (off == b + 68 || off == b + 72 || off == b + 76)
            return 1;
    }
    return 0;
}

void* ANIT_Alloc(size_t n)
{
    void* p;

    n = (n + 15u) & ~(size_t)15u;
    if (ANIT_arenaUsed + n > ANIT_ARENA_SIZE)
    {
        printf("arena exhausted: %zu used, %zu wanted\n", ANIT_arenaUsed, n);
        return 0;
    }
    p = ANIT_arena + ANIT_arenaUsed;
    ANIT_arenaUsed += n;
    return p;
}

long ANIT_Ref(const void* p)
{
    if ((const char*)p >= ANIT_arena && (const char*)p < ANIT_arena + ANIT_ARENA_SIZE)
        return (long)((const char*)p - ANIT_arena);
    if (!p)
        return -1;
    return -2;
}

void ANIT_ResetLog(void)
{
    memset(ANIT_log, 0, sizeof(ANIT_log));
    ANIT_logN = 0;
    ANIT_seq = 0;
    ANIT_runCalls = 0;
}

void ANIT_Log4(const char* name, double a, double b, double c, double d)
{
    if (ANIT_logN >= ANIT_LOG_MAX)
        return;
    ANIT_log[ANIT_logN].name = name;
    ANIT_log[ANIT_logN].a = a;
    ANIT_log[ANIT_logN].b = b;
    ANIT_log[ANIT_logN].c = c;
    ANIT_log[ANIT_logN].d = d;
    ANIT_logN++;
}

#define LOG0(n)             ANIT_Log4(n, 0, 0, 0, 0)
#define LOG1(n, a)          ANIT_Log4(n, (double)(a), 0, 0, 0)
#define LOG2(n, a, b)       ANIT_Log4(n, (double)(a), (double)(b), 0, 0)
#define LOG3(n, a, b, c)    ANIT_Log4(n, (double)(a), (double)(b), (double)(c), 0)
#define LOG4(n, a, b, c, d) ANIT_Log4(n, (double)(a), (double)(b), (double)(c), (double)(d))

// ---------------------------------------------------------------- faithful

Mtx4x4* OBJ_pst_GetAbsoluteMatrix(tGameObjectMain* a1)
{
    return a1->matrices;
}

tGaoMatrix* OBJ_pst_GetLocalMatrix(tGameObjectMain* a1)
{
    tGaoMatrix* result;

    if ((a1->flags & 0x400000) != 0)
        result = a1->basic->mtx;
    else
        result = 0;
    return result;
}

Mtx4x4* OBJ_pst_GetFlashMatrix(struct sGameObjectMain* a1)
{
    Mtx4x4* v1;
    Mtx4x4* result;

    if ((a1->flags & 0x4000000) == 0)
        return 0;
    v1 = a1->matrices;
    if ((a1->flags & 0x10000) != 0)
        result = v1 + 2;
    else
        result = v1 + 1;
    return result;
}

BOOL TAB_TableElemIsInvalid(void* elem)
{
    return !elem || ((uint8_t)(size_t)elem & 3) == 1;
}

int TAB_ul_PFtable_GetElemIndexWithPointer(tableDescStruct* a1, void* a2)
{
    tableElemStructPF* v2;
    tableElemStructPF* v3;
    int result;

    v2 = a1->firstElemPtr;
    v3 = a1->nextElemPtr;
    result = 0;
    if (v2 >= v3)
        return -1;
    while (a2 != v2->objectPtr)
    {
        ++v2;
        ++result;
        if (v2 >= v3)
            return -1;
    }
    return result;
}

void PRO_QPC_RecordTimeDecRef(int id)
{
    LARGE_INTEGER pc;

    QueryPerformanceCounter(&pc);
    if (PerfCounterStruct[id].QuadPart != 0)
        PerfCountertimeSumArray[id] += pc.QuadPart - PerfCounterStruct[id].QuadPart;
    PerfCounterStruct[id].QuadPart = 0;
}

char LOA_ReadChar(char** offset, char* retVal, char readMode)
{
    char result;

    result = **offset;
    *offset += 1;
    if (retVal)
        *retVal = result;
    return result;
}

// ------------------------------------------------------------ deterministic

int EVE_gb_CanFlash;

void* MEM_p_ReallocAligned(void* p, unsigned int size, int align)
{
    void* q = ANIT_Alloc(size);
    LOG3("MEM_p_ReallocAligned", ANIT_Ref(p), size, ANIT_Ref(q));
    if (p && q)
        memcpy(q, p, size);
    return q;
}

void MEM_FreeAlign(void* p)
{
    LOG1("MEM_FreeAlign", ANIT_Ref(p));
}

tGroIntf ANIT_groIntf;

static void ANIT_GroAddRef(struct sGroGeneric* gro, int delta)
{
    LOG2("GRO_AddRef", ANIT_Ref(gro), delta);
    gro->usageCount += delta;
}

struct sGroGeneric* ANIT_MakeGro(int id)
{
    tGroGeneric* gro = (tGroGeneric*)ANIT_Alloc(sizeof(tGroGeneric));

    ANIT_groIntf.id = id;
    ANIT_groIntf.AddRef = ANIT_GroAddRef;
    gro->intf = &ANIT_groIntf;
    gro->usageCount = 1;
    return gro;
}

static long long ANIT_qpc;

int QueryPerformanceCounter(LARGE_INTEGER* out)
{
    ANIT_qpc += 1000;
    out->QuadPart = ANIT_qpc;
    return 1;
}

void* MEM_p_Alloc(unsigned int size)
{
    void* p = ANIT_Alloc(size);
    LOG2("MEM_p_Alloc", size, ANIT_Ref(p));
    return p;
}

void* MEM_p_AllocAligned(int size, int align)
{
    void* p = ANIT_Alloc(size);
    LOG3("MEM_p_AllocAligned", size, align, ANIT_Ref(p));
    return p;
}

void MEM_Free(void* p)
{
    LOG1("MEM_Free", ANIT_Ref(p));
}

void LOA_DeleteAddress(int a1)
{
    LOG0("LOA_DeleteAddress");
}

void LOA_Resolve()
{
    LOG0("LOA_Resolve");
}

void LOA_MakeFileRef(int key, void** retVal, void* (*func)(uint32_t), char flags)
{
    LOG2("LOA_MakeFileRef", key, flags);
    *retVal = func((uint32_t)key);
}

char* BIG_pc_ReadFileTmp(int Offset, int* size)
{
    LOG1("BIG_pc_ReadFileTmp", Offset);
    *size = ANIT_fileSize;
    return ANIT_fileData;
}

// The event player. Both copies drive it through a float of remaining
// time, so the stand-in has to consume that time or the loops never end.
static int ANIT_RunTracks(const char* name, tEventData* data, float* remain)
{
    int finished;

    ANIT_seq++;
    ANIT_runCalls++;
    // A case must terminate: past a fixed number of calls the player
    // reports the frame's time all used up.
    if (ANIT_runCalls > 40)
    {
        LOG4(name, ANIT_Ref(data), -2.0, 0, ANIT_seq);
        if (remain)
            *remain = 0.0f;
        return 0;
    }
    finished = (ANIT_seq % 3) == 0;
    LOG4(name, ANIT_Ref(data), remain ? *remain : -1.0, finished, ANIT_seq);
    if (remain)
    {
        if (*remain > 0.25f)
            *remain = *remain - 0.25f;
        else
            *remain = 0.0f;
    }
    return finished;
}

int EVE_b_RunAllTracks(tEventData* a1, float* a2)
{
    return ANIT_RunTracks("EVE_b_RunAllTracks", a1, a2);
}

int EVE_b_RunAnimation(tEventData* a1, float* a4)
{
    return ANIT_RunTracks("EVE_b_RunAnimation", a1, a4);
}

void EVE_GetTracksTime(struct sEventData* a1, int a2, float* currTime, float* fullTime)
{
    ANIT_seq++;
    if (currTime)
        *currTime = 0.125f * (float)(ANIT_seq % 7) + 0.0625f * (float)a2;
    if (fullTime)
        *fullTime = 1.5f + 0.25f * (float)(ANIT_seq % 5);
    LOG4("EVE_GetTracksTime", ANIT_Ref(a1), a2,
        currTime ? *currTime : -1.0, fullTime ? *fullTime : -1.0);
}

void EVE_SetTracksTime(struct sEventData* a1, float time)
{
    LOG2("EVE_SetTracksTime", ANIT_Ref(a1), time);
}

void EVE_InitData(tEventData* a1, tGameObjectEventsList* a2)
{
    LOG2("EVE_InitData", ANIT_Ref(a1), ANIT_Ref(a2));
    a1->eventListStruct = a2;
    a1->tracks = (tAnimTracks*)ANIT_Alloc(sizeof(tAnimTracks) * (a2->num > 4 ? a2->num : 4));
}

void EVE_DeleteListTracks(tGameObjectEventsList* a1)
{
    LOG1("EVE_DeleteListTracks", ANIT_Ref(a1));
}

void EVE_ReinitData(struct sEventData* a1)
{
    LOG1("EVE_ReinitData", ANIT_Ref(a1));
}

uint16_t EVE_w_Event_InterpolationKey_GetType(struct sGameObjectEventElem* a1)
{
    LOG2("EVE_w_Event_InterpolationKey_GetType", ANIT_Ref(a1), a1->eptrdu.size);
    return a1->eptrdu.size;
}

Vec3* EVE_pst_Event_InterpolationKey_GetPos(tGameObjectEventElem* a1)
{
    Vec3* p = (Vec3*)ANIT_keyPos[a1->duration & 15];
    LOG2("EVE_pst_Event_InterpolationKey_GetPos", ANIT_Ref(a1), ANIT_Ref(p));
    return p;
}

struct sGameObjectEventsList* EVE_ul_CallbackLoadListTracks(uint32_t Offset)
{
    LOG1("EVE_ul_CallbackLoadListTracks", Offset);
    return ANIT_loadedList;
}

struct sGroup* GRP_ul_LoadGrpCallback(uint32_t Offset)
{
    LOG1("GRP_ul_LoadGrpCallback", Offset);
    return (struct sGroup*)ANIT_loadedGroup;
}

struct sGroup* GRP_pst_CreateNewGroup()
{
    tGroup* grp = (tGroup*)ANIT_Alloc(sizeof(tGroup));
    grp->tablePF = (tableDescStruct*)ANIT_Alloc(sizeof(tableDescStruct));
    grp->tablePF->firstElemPtr = ANIT_Alloc(sizeof(tableElemStructPF) * 4);
    grp->tablePF->nextElemPtr = grp->tablePF->firstElemPtr;
    grp->tablePF->lastElemPtr = (char*)grp->tablePF->firstElemPtr
        + sizeof(tableElemStructPF) * 4;
    LOG1("GRP_pst_CreateNewGroup", ANIT_Ref(grp));
    return grp;
}

void GRP_DuplicateGroupObjects(struct sWorld* a1, struct sGroup* src, struct sGroup* dst)
{
    LOG3("GRP_DuplicateGroupObjects", ANIT_Ref(a1), ANIT_Ref(src), ANIT_Ref(dst));
}

void UpdateModifiersGroup(struct sGameObjectMain* gao, struct sGroup* grp)
{
    LOG2("UpdateModifiersGroup", ANIT_Ref(gao), ANIT_Ref(grp));
}

void MDF_Modifier_Destroy(struct sModifier* a1)
{
    LOG1("MDF_Modifier_Destroy", ANIT_Ref(a1));
}

void MDF_Modifier_DuplicateList(struct sGameObjectMain* a1, struct sModifier* a2)
{
    LOG2("MDF_Modifier_DuplicateList", ANIT_Ref(a1), ANIT_Ref(a2));
}

void MDF_ApplyAllGao(struct sGameObjectMain* a1)
{
    LOG1("MDF_ApplyAllGao", ANIT_Ref(a1));
}

void MDF_UnApplyAllGao(struct sGameObjectMain* a1)
{
    LOG1("MDF_UnApplyAllGao", ANIT_Ref(a1));
}

void GRO_Render(struct sGameObjectMain* a2)
{
    LOG1("GRO_Render", ANIT_Ref(a2));
}

void AI_ExecCallback(struct sGameObjectMain* a1, int a2)
{
    LOG2("AI_ExecCallback", ANIT_Ref(a1), a2);
}

void ACT_FreeActionKit(tActionKit** a1)
{
    LOG1("ACT_FreeActionKit", ANIT_Ref(*a1));
}

void ACT_SetAction(struct sGameObjectMain* a1, int actionId, int forced)
{
    LOG3("ACT_SetAction", ANIT_Ref(a1), actionId, forced);
}

void ACT_ActionRestoreInit(struct sActionKit* a1)
{
    LOG1("ACT_ActionRestoreInit", ANIT_Ref(a1));
}

void DYN_SetHorizontalSpeedComponent(struct sDyn* a1, Vec3* a2)
{
    LOG4("DYN_SetHorizontalSpeedComponent", ANIT_Ref(a1), a2->x, a2->y, a2->z);
}

void OBJ_ComputeBV(tGameObjectMain* a1, int a2)
{
    LOG2("OBJ_ComputeBV", ANIT_Ref(a1), a2);
}

void OBJ_ComputeGlobalWhenHie(struct sGameObjectMain* a1)
{
    LOG1("OBJ_ComputeGlobalWhenHie", ANIT_Ref(a1));
}

void OBJ_FreeGroup(tGroup* a1)
{
    LOG1("OBJ_FreeGroup", ANIT_Ref(a1));
}

void OBJ_GameObject_Remove(tGameObjectMain* gao, BOOL refCountDown)
{
    LOG2("OBJ_GameObject_Remove", ANIT_Ref(gao), refCountDown);
}

void OBJ_ReinitMatrixStorageByFlags(tGameObjectMain* a1, Mtx4x4* a2)
{
    LOG2("OBJ_ReinitMatrixStorageByFlags", ANIT_Ref(a1), ANIT_Ref(a2));
}

void WOR_World_DetachObject(struct sWorld* a1, struct sGameObjectMain* a2)
{
    LOG2("WOR_World_DetachObject", ANIT_Ref(a1), ANIT_Ref(a2));
}

void WOR_World_JustAfterLoadObject(struct sWorld* a1, struct sGameObjectMain* a2, int a3, int a4)
{
    LOG4("WOR_World_JustAfterLoadObject", ANIT_Ref(a1), ANIT_Ref(a2), a3, a4);
}

// The two that have to build something, because the code that follows
// them writes through what they allocate.

void OBJ_GameObject_AllocateGizmo(struct sGameObjectMain* a1, int a2, int a3)
{
    tGameObjectMultiPartData* mp;
    size_t elemSize;

    LOG3("OBJ_GameObject_AllocateGizmo", ANIT_Ref(a1), a2, a3);
    if (!a1->basic)
        return;
    elemSize = sizeof(tMultiPartGameObjectLinks) > sizeof(Mtx4x4)
        ? sizeof(tMultiPartGameObjectLinks) : sizeof(Mtx4x4);
    mp = a1->basic->multiPartData;
    if (!mp)
    {
        mp = (tGameObjectMultiPartData*)ANIT_Alloc(sizeof(tGameObjectMultiPartData));
        a1->basic->multiPartData = mp;
    }
    if (!mp->multipartGaoObjectPartsOrMatrices || mp->elemNum < a2)
    {
        mp->multipartGaoObjectPartsOrMatrices = ANIT_Alloc(elemSize * (a2 + 1));
        ANIT_MarkMtx(mp->multipartGaoObjectPartsOrMatrices, a2 + 1);
    }
    mp->elemNum = a2;
    a1->flags |= a3 ? 0x1200000 : 0x200000;
}

void OBJ_ChangeIdentityFlags(tGameObjectMain* a1, unsigned int a2, int a3)
{
    LOG3("OBJ_ChangeIdentityFlags", ANIT_Ref(a1), a2, a3);
    if (!a1)
        return;
    if ((a2 & 0x1000) != 0 && (a3 & 0x1000) == 0 && !a1->basic)
        a1->basic = (tGameObjectBasic*)ANIT_Alloc(sizeof(tGameObjectBasic));
    if ((a2 & 0x4000) != 0 && (a3 & 0x4000) == 0 && a1->basic
        && !a1->basic->actorObjectOrVisualObject)
        a1->basic->actorObjectOrVisualObject = ANIT_Alloc(sizeof(tGameObjectVisualObject));
    if ((a2 & 0x400000) != 0 && (a3 & 0x400000) == 0 && a1->basic && !a1->basic->mtx)
    {
        a1->basic->mtx = (tGaoMatrix*)ANIT_Alloc(sizeof(tGaoMatrix));
        ANIT_MarkMtx(a1->basic->mtx, 1);
    }
    if ((a2 & 0x10000) != 0 && (a3 & 0x10000) == 0)
        a1->flags |= 0x10000;
    a1->flags |= a2 & 0x400000;
}
