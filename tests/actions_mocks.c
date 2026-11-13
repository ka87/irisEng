#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <math.h>

#include "actions_mocks.h"
#include "../memory.h"
#include "../loading.h"
#include "../bigfiles.h"
#include "../animation.h"
#include "../time.h"
#include "../engine.h"

tActMockCall ACTMOCK_log[ACTMOCK_LOG_MAX];
int ACTMOCK_logLen;
int ACTMOCK_logOverflow;

unsigned char ACTMOCK_arena[ACTMOCK_ARENA];
size_t ACTMOCK_arenaUsed;
static size_t ACTMOCK_arenaHigh;   /* the most any case has used */

tActScene ACTMOCK_scene;
tActScratch ACTMOCK_scratch;

int ACTMOCK_cases;
int ACTMOCK_failures;
long ACTMOCK_compares;

static const char* ACTMOCK_case = "?";
static int ACTMOCK_reported;

static unsigned char ACTMOCK_savedArena[ACTMOCK_ARENA];
static size_t ACTMOCK_savedArenaUsed;
static tActMockCall ACTMOCK_savedLog[ACTMOCK_LOG_MAX];
static int ACTMOCK_savedLogLen;
static tActScene ACTMOCK_savedScene;
static tActScratch ACTMOCK_savedScratch;

/* the engine globals actions.c reads and writes */
static tGameObjectMain* ACTMOCK_savedOwner;
static tEventData* ACTMOCK_savedData;
static tGameObjectEventsList* ACTMOCK_savedList;
static tGameObjectEventsListElem* ACTMOCK_savedTrack;
static tGameObjectEventElem* ACTMOCK_savedEvent;
static Mtx4x4* ACTMOCK_savedRefMtx;
static tAnimTracks* ACTMOCK_savedParam;
static float ACTMOCK_savedDt;

void ACTMOCK_SetCase(const char* name)
{
    ACTMOCK_case = name;
    ACTMOCK_reported = 0;
    ACTMOCK_cases++;
}

void ACTMOCK_Push(int value)
{
    /* the hash takes every value: a case that pushes more than the
       array holds would otherwise stop comparing its tail, which is
       where the interesting arguments tend to be */
    ACTMOCK_scratch.hash = (ACTMOCK_scratch.hash ^ (unsigned long long)(unsigned)value)
                           * 1099511628211ULL;
    if (ACTMOCK_scratch.retLen < 256)
        ACTMOCK_scratch.ret[ACTMOCK_scratch.retLen++] = value;
}

/* A pointer reduced to something printable. Both sides run over the
   same statics at the same addresses, so the low half of the pointer is
   already comparable; the region letter is for reading the failures. */
int ACTMOCK_Tag(const void* p)
{
    const unsigned char* c = (const unsigned char*)p;

    if (!p)
        return 0;
    if (c >= ACTMOCK_arena && c < ACTMOCK_arena + ACTMOCK_ARENA)
        return 0x10000000 + (int)(c - ACTMOCK_arena);
    if (c >= (const unsigned char*)&ACTMOCK_scene
        && c < (const unsigned char*)&ACTMOCK_scene + sizeof(tActScene))
        return 0x20000000 + (int)(c - (const unsigned char*)&ACTMOCK_scene);
    return 0x30000000 | (int)((size_t)p & 0xFFFFFF);
}

/* ---- the call log ---------------------------------------------------- */

static void ACTMOCK_Record(const char* fn, int ni, int nf, ...)
{
    va_list ap;
    int i;
    tActMockCall* c;

    if (ACTMOCK_logLen >= ACTMOCK_LOG_MAX)
    {
        ACTMOCK_logOverflow = 1;
        return;
    }
    c = &ACTMOCK_log[ACTMOCK_logLen++];
    memset(c, 0, sizeof(*c));
    c->fn = fn;
    c->ni = ni;
    c->nf = nf;
    va_start(ap, nf);
    for (i = 0; i < ni; i++)
        c->ia[i] = va_arg(ap, int);
    for (i = 0; i < nf; i++)
        c->fa[i] = (float)va_arg(ap, double);
    va_end(ap);
}

/* ---- the allocator --------------------------------------------------- */

#define ACTMOCK_GUARD 512

void* MEM_p_Alloc(unsigned int size)
{
    size_t off;
    void* p;

    off = (ACTMOCK_arenaUsed + 15) & ~(size_t)15;
    off += ACTMOCK_GUARD;
    if (off + size + ACTMOCK_GUARD > ACTMOCK_ARENA)
    {
        printf("  [%s] arena exhausted at %u bytes\n", ACTMOCK_case, size);
        exit(2);
    }
    p = &ACTMOCK_arena[off];
    ACTMOCK_arenaUsed = off + size + ACTMOCK_GUARD;
    if (ACTMOCK_arenaUsed > ACTMOCK_arenaHigh)
        ACTMOCK_arenaHigh = ACTMOCK_arenaUsed;
    ACTMOCK_Record("MEM_p_Alloc", 2, 0, (int)size, (int)off);
    return p;
}

void* MEM_p_AllocAligned(int size, int alignment)
{
    (void)alignment;
    return MEM_p_Alloc((unsigned int)size);
}

void* MEM_p_Realloc(void* src, size_t size)
{
    void* p;

    ACTMOCK_Record("MEM_p_Realloc", 2, 0, ACTMOCK_Tag(src), (int)size);
    p = MEM_p_Alloc((unsigned int)size);
    if (src)
        memcpy(p, src, size);
    return p;
}

void MEM_Free(void* ptr)
{
    ACTMOCK_Record("MEM_Free", 1, 0, ACTMOCK_Tag(ptr));
}

void* MEM_p_ReallocAligned(void* ptr, unsigned int size, int alignment)
{
    (void)alignment;
    return MEM_p_Realloc(ptr, size);
}

void MEM_FreeAlign(void* ptr)
{
    MEM_Free(ptr);
}

/* ---- the loader ------------------------------------------------------ */

int LOA_gb_SpeedMode;
int BIG_gi_ReadMode;

static const unsigned char* ACTMOCK_fileBase;

char* BIG_pc_ReadFileTmp(int Offset, int* size)
{
    unsigned char* buf;

    if (Offset == 0)
    {
        buf = ACTMOCK_scene.kitFile;
        *size = ACTMOCK_scene.kitFileSize;
    }
    else if (Offset > 0)
    {
        buf = ACTMOCK_scene.file;
        *size = ACTMOCK_scene.fileSize;
    }
    else
    {
        buf = ACTMOCK_scene.file;
        *size = 0;
    }
    ACTMOCK_fileBase = buf;
    ACTMOCK_Record("BIG_pc_ReadFileTmp", 2, 0, Offset, *size);
    return (char*)buf;
}

/* The three readers, with the engine's own skip rule. */
char LOA_ReadChar(char** offset, char* retVal, char readMode)
{
    char result = 0;
    int size;

    if ((readMode & 2) != 0 && LOA_gb_SpeedMode && BIG_gi_ReadMode == 2)
    {
        size = 0;
    }
    else
    {
        result = **offset;
        size = 1;
    }
    *offset += size;
    if (retVal)
        *retVal = result;
    return result;
}

uint16_t LOA_ReadShort(char** offset, uint16_t* retVal, char readMode)
{
    int size = 2;
    int16_t result = 0;

    if ((readMode & 2) != 0 && LOA_gb_SpeedMode && BIG_gi_ReadMode == 2)
        size = 0;
    else
        result = *(uint16_t*)*offset;
    *offset += size;
    if (retVal)
        *retVal = result;
    return result;
}

int LOA_ReadLong(char** offset, uint32_t* retVal, char readMode)
{
    int result = 0;
    int size;

    if ((readMode & 2) != 0 && LOA_gb_SpeedMode && BIG_gi_ReadMode == 2)
    {
        size = 0;
    }
    else
    {
        result = *(uint32_t*)*offset;
        size = 4;
    }
    *offset += size;
    if (retVal)
        *retVal = result;
    return result;
}

void LOA_DeleteAddress(int pos)
{
    ACTMOCK_Record("LOA_DeleteAddress", 1, 0, pos ? 1 : 0);
}

#define ACTMOCK_CALLBACKS 8
static void* ACTMOCK_cbFn[ACTMOCK_CALLBACKS];
static int ACTMOCK_cbId[ACTMOCK_CALLBACKS];
static int ACTMOCK_cbNum;

void ACTMOCK_RegisterCallback(void* fn, int id)
{
    int i;

    for (i = 0; i < ACTMOCK_cbNum; i++)
        if (ACTMOCK_cbId[i] == id)
        {
            ACTMOCK_cbFn[i] = fn;
            return;
        }
    if (ACTMOCK_cbNum < ACTMOCK_CALLBACKS)
    {
        ACTMOCK_cbFn[ACTMOCK_cbNum] = fn;
        ACTMOCK_cbId[ACTMOCK_cbNum] = id;
        ACTMOCK_cbNum++;
    }
}

static int ACTMOCK_CallbackId(void* fn)
{
    int i;

    for (i = 0; i < ACTMOCK_cbNum; i++)
        if (ACTMOCK_cbFn[i] == fn)
            return ACTMOCK_cbId[i];
    return -1;
}

/* The real loader queues the reference and fills it in later. Here it
   is resolved at once, to a pointer derived from the key, so the
   reference that was written can be compared. */
void LOA_MakeFileRef(int key, void** retVal, void* (*func)(uint32_t), char flags)
{
    ACTMOCK_Record("LOA_MakeFileRef", 4, 0, key, ACTMOCK_Tag(retVal),
                   ACTMOCK_CallbackId((void*)func), flags);
    if (retVal)
        *retVal = (void*)&ACTMOCK_arena[(unsigned)(key * 37) % 4096];
}

/* ---- the engine around actions.c ------------------------------------- */

void EVE_DeleteListTracks(tGameObjectEventsList* list)
{
    ACTMOCK_Record("EVE_DeleteListTracks", 1, 0, ACTMOCK_Tag(list));
}

static int ACTMOCK_timeCall;

void EVE_GetTracksTime(struct sEventData* eve, int trackIdx, float* currTime, float* fullTime)
{
    ACTMOCK_Record("EVE_GetTracksTime", 3, 0, ACTMOCK_Tag(eve), trackIdx,
                   (currTime ? 1 : 0) | (fullTime ? 2 : 0));
    ACTMOCK_timeCall++;
    if (currTime)
        *currTime = 0.25f * (float)(ACTMOCK_timeCall % 7);
    if (fullTime)
    {
        /* one call in five hands back a zero length, which is the only
           way the blend division's degenerate path is reached */
        *fullTime = (ACTMOCK_timeCall % 5 == 4) ? 0.0f
                                                : 1.0f + 0.5f * (float)(ACTMOCK_timeCall % 9);
    }
}

void EVE_SetTracksTime(struct sEventData* eve, float time)
{
    ACTMOCK_Record("EVE_SetTracksTime", 1, 1, ACTMOCK_Tag(eve), (double)time);
}

struct sAnim* ANI_AllocAnim(void)
{
    tAnim* anim;

    anim = (tAnim*)MEM_p_AllocAligned(sizeof(tAnim), 16);
    memset(anim, 0, sizeof(tAnim));
    anim->ptr = (tEventData*)MEM_p_AllocAligned(sizeof(tEventData), 16);
    anim->ptr->eventListStruct = NULL;
    anim->ptr->tracks = NULL;
    ACTMOCK_Record("ANI_AllocAnim", 1, 0, ACTMOCK_Tag(anim));
    return anim;
}

void ANI_SetTrackList(struct sAnim* anim, struct sGameObjectEventsList* list)
{
    ACTMOCK_Record("ANI_SetTrackList", 2, 0, ACTMOCK_Tag(anim), ACTMOCK_Tag(list));
    if (anim && anim->ptr && list)
    {
        anim->ptr->eventListStruct = list;
        anim->ptr->tracks = &ACTMOCK_scene.animTracks[0];
    }
}

void ANI_ChangeShape(struct sGameObjectMain* gao, struct sShape* shape)
{
    ACTMOCK_Record("ANI_ChangeShape", 2, 0, ACTMOCK_Tag(gao), ACTMOCK_Tag(shape));
}

void ANI_UpdateFlash(struct sGameObjectMain* gao, char which)
{
    ACTMOCK_Record("ANI_UpdateFlash", 2, 0, ACTMOCK_Tag(gao), which);
}

/* ANI_OneCall scribbles over every global ACT_SetAction saves, so a
   restore that is dropped or reordered shows up. */
void ANI_OneCall(struct sGameObjectMain* gao)
{
    ACTMOCK_Record("ANI_OneCall", 2, 0, ACTMOCK_Tag(gao), (int)(TIM_gf_dt * 1000.0f));
    EVE_gpst_OwnerGAO = &ACTMOCK_scene.srcGao;
    EVE_gpst_CurrentData = &ACTMOCK_scene.animData[3];
    EVE_gpst_CurrentListTracks = &ACTMOCK_scene.eventLists[3];
    EVE_gpst_CurrentTrack = &ACTMOCK_scene.listElems[3];
    EVE_gpst_CurrentEvent = &ACTMOCK_scene.trackElems[3][0];
    EVE_gpst_CurrentRefMatrix = &ACTMOCK_scene.flashMtx;
    EVE_gpst_CurrentParam = &ACTMOCK_scene.animTracks[3];
    TIM_gf_dt = 99.0f;
}

struct sGameObjectEventsList* EVE_ul_CallbackLoadListTracks(uint32_t Offset)
{
    ACTMOCK_Record("EVE_ul_CallbackLoadListTracks", 1, 0, (int)Offset);
    return &ACTMOCK_scene.eventLists[Offset % ACTMOCK_ACTIONS];
}

struct sShape* ANI_ul_ShapeCallback(uint32_t Offset)
{
    ACTMOCK_Record("ANI_ul_ShapeCallback", 1, 0, (int)Offset);
    return &ACTMOCK_scene.shapes[Offset % ACTMOCK_ACTIONS];
}

Mtx4x4* OBJ_pst_GetFlashMatrix(struct sGameObjectMain* gao)
{
    ACTMOCK_Record("OBJ_pst_GetFlashMatrix", 1, 0, ACTMOCK_Tag(gao));
    return &ACTMOCK_scene.flashMtx;
}

/* ---- the scene ------------------------------------------------------- */

static unsigned int ACTMOCK_rngState;

static unsigned int ACTMOCK_Rand(void)
{
    ACTMOCK_rngState = ACTMOCK_rngState * 1103515245u + 12345u;
    return (ACTMOCK_rngState >> 16) & 0x7FFF;
}

void ACTMOCK_Init(void)
{
    MATH_InitModule();
}

void ACTMOCK_Reset(void)
{
    /* the whole of what any case has ever used, not just the last
       one: clearing only this side's high-water mark leaves an earlier,
       larger case's bytes above it for one side and not the other */
    memset(ACTMOCK_arena, 0, ACTMOCK_arenaHigh + 256 < ACTMOCK_ARENA
                                 ? ACTMOCK_arenaHigh + 256 : ACTMOCK_ARENA);
    ACTMOCK_arenaUsed = 0;
    ACTMOCK_logLen = 0;
    ACTMOCK_logOverflow = 0;
    ACTMOCK_timeCall = 0;
    memset(&ACTMOCK_scratch, 0, sizeof(ACTMOCK_scratch));

    EVE_gpst_OwnerGAO = &ACTMOCK_scene.gao;
    EVE_gpst_CurrentData = &ACTMOCK_scene.animData[0];
    EVE_gpst_CurrentListTracks = &ACTMOCK_scene.eventLists[0];
    EVE_gpst_CurrentTrack = &ACTMOCK_scene.listElems[0];
    EVE_gpst_CurrentEvent = &ACTMOCK_scene.trackElems[0][0];
    EVE_gpst_CurrentRefMatrix = NULL;
    EVE_gpst_CurrentParam = &ACTMOCK_scene.animTracks[0];
    TIM_gf_dt = 0.0333f;
    EVE_gf_ElapsedDT = 1.5f;
}

static void ACTMOCK_BuildKit(tActKitBuf* kit, tActBuf* acts, int pattern)
{
    int i;
    int j;
    int k;

    kit->kit.elemNum = ACTMOCK_ACTIONS;
    /* what ACT_pst_FindTransition would read if its 0xFFFF guard went */
    kit->actions[0xFFFF] = &acts[1].action;
    kit->kit.usageCount = 1;
    kit->kit.flags = 0;
    kit->kit.savedActions = NULL;

    for (i = 0; i < ACTMOCK_ACTIONS; i++)
    {
        tActionElem* elems = ACTMOCK_Elems(&acts[i].action);

        /* action 3 is absent, which is what the elemNum guard is for */
        kit->actions[i] = (i == 3 && ((pattern >> 4) & 1)) ? NULL : &acts[i].action;

        acts[i].action.actionElemNum = (i == 7 && ((pattern >> 5) & 1))
                                           ? 0 : (uint8_t)(1 + (i % ACTMOCK_ELEMS));
        acts[i].action.actionElemIDForTransition = (uint8_t)(i % 3);
        acts[i].action.usageCount = 1;

        for (j = 0; j < ACTMOCK_ELEMS; j++)
        {
            /* element 0 always has a list: the blend test walks
               animPtr[0]->ptr->eventListStruct->listElems, and that
               field is cleared by the blend and only put back by
               ANI_SetTrackList, which a null list makes a no-op */
            elems[j].eventLists = (j > 0 && (i + j) % 5 == 4)
                                              ? NULL : &ACTMOCK_scene.eventLists[i];
            elems[j].shape = ((i + j) % 4 == 3)
                                         ? NULL : &ACTMOCK_scene.shapes[i];
            elems[j].changeActionCallAmountToChange = (uint8_t)(1 + i);
            elems[j].b2 = (uint8_t)j;
            elems[j].actionFrequency = (uint8_t)(16 + 7 * i);
            elems[j].customBits = (uint8_t)(i * 3);
            elems[j].designFlags = (int16_t)(i * 17 + j);
            elems[j].b5 = (uint8_t)(((i * 13 + j * 5) & 0x7F)
                                    | ((i & 2) ? 0x80 : 0));
            elems[j]._pad = 0;

            if ((i + j) % 3 == 2)
            {
                elems[j].transition = NULL;
                continue;
            }
            elems[j].transition = &ACTMOCK_scene.transitions[i][j];
            ACTMOCK_scene.transitions[i][j].keyValPtr = ACTMOCK_scene.keyVals[i][j];
            ACTMOCK_scene.transitions[i][j].actElemNum = ACTMOCK_TRANS;
            ACTMOCK_scene.transitions[i][j].maxElemNum = ACTMOCK_TRANS;
            ACTMOCK_scene.transitions[i][j].elemNumToAddWhenFull = 2;
            for (k = 0; k < ACTMOCK_TRANS; k++)
            {
                /* keys ascending, so the engine's own binary search
                   finds them; every action id is a reachable key for
                   some element */
                ACTMOCK_scene.keyVals[i][j][k].key = k * 2;
                ACTMOCK_scene.keyVals[i][j][k].val =
                    (int)(size_t)&ACTMOCK_scene.transElems[i][j][k];
                ACTMOCK_scene.transElems[i][j][k].actionID =
                    (uint16_t)((k * 3 + i) % ACTMOCK_ACTIONS);
                ACTMOCK_scene.transElems[i][j][k].flags =
                    (uint8_t)(((i + k) & 3) | (((i + k) & 1) ? 0x10 : 0)
                              | (((i + 2 * k) & 2) ? 0x20 : 0)
                              | (((i * k) & 1) ? 0x40 : 0));
                ACTMOCK_scene.transElems[i][j][k].transitionLength =
                    (uint8_t)((pattern >> 6) & 1 ? 0 : (k % 3) * 4);
            }
        }
    }
}

void ACTMOCK_BuildScene(unsigned int seed, int pattern)
{
    int i;
    int k;

    memset(&ACTMOCK_scene, 0, sizeof(ACTMOCK_scene));
    ACTMOCK_rngState = seed;

    for (i = 0; i < ACTMOCK_ACTIONS; i++)
    {
        ACTMOCK_scene.eventLists[i].listElems = &ACTMOCK_scene.listElems[i];
        ACTMOCK_scene.eventLists[i].num = 1;
        ACTMOCK_scene.eventLists[i].usageCount = 3;
        ACTMOCK_scene.listElems[i].trackElems = ACTMOCK_scene.trackElems[i];
        ACTMOCK_scene.listElems[i].elemNum = (uint16_t)(1 + (i % 4));
        for (k = 0; k < 4; k++)
            ACTMOCK_scene.trackElems[i][k].duration = (uint16_t)(k + i);
        ACTMOCK_scene.shapes[i].usageCount = (int16_t)(1 + (i % 3));
        ACTMOCK_scene.shapes[i].num = (uint16_t)i;
    }

    for (i = 0; i < 4; i++)
    {
        ACTMOCK_scene.anims[i].ptr = &ACTMOCK_scene.animData[i];
        /* bit 7 short-circuits the blend's key-frame test, so only the
           anims without it reach the rest of the condition */
        ACTMOCK_scene.anims[i].flags = (int16_t)(((i & 1) ? 0x80 : 0) | (i * 7));
        ACTMOCK_scene.anims[i].frequency = (uint8_t)(30 + i);
        ACTMOCK_scene.animData[i].eventListStruct = &ACTMOCK_scene.eventLists[i];
        ACTMOCK_scene.animData[i].tracks = &ACTMOCK_scene.animTracks[i];

        /* anim 0: on its last key and exactly at that key's duration,
           so both halves of the test are true and either one of them
           moving changes the outcome.
           anim 2: exactly at a key's duration but not the last key, so
           the key-frame half decides on its own.
           the odd ones carry bit 7 and never get this far. */
        if (i == 2)
            ACTMOCK_scene.animTracks[i].currElemKeyFrame = 0;
        else
            ACTMOCK_scene.animTracks[i].currElemKeyFrame =
                (uint16_t)(ACTMOCK_scene.listElems[i].elemNum - 1);
        if (i & 1)
            ACTMOCK_scene.animTracks[i].currTime = 0.5f;
        else
            ACTMOCK_scene.animTracks[i].currTime =
                (float)((double)ACTMOCK_scene.trackElems[i][
                            ACTMOCK_scene.animTracks[i].currElemKeyFrame].duration
                        * 0.016666668);
    }

    ACTMOCK_BuildKit(&ACTMOCK_scene.kit, ACTMOCK_scene.actions, pattern);
    ACTMOCK_BuildKit(&ACTMOCK_scene.srcKit, ACTMOCK_scene.srcActions, pattern >> 8);

    ACTMOCK_scene.basic.actorObjectOrVisualObject = &ACTMOCK_scene.actor;
    ACTMOCK_scene.basic.dyn = &ACTMOCK_scene.dyn;
    ACTMOCK_scene.srcBasic.actorObjectOrVisualObject = &ACTMOCK_scene.srcActor;
    ACTMOCK_scene.srcBasic.dyn = &ACTMOCK_scene.srcDyn;
    ACTMOCK_scene.dyn.flags = 0x155555;
    ACTMOCK_scene.srcDyn.flags = 0x2AAAAA;

    ACTMOCK_scene.gao.basic = &ACTMOCK_scene.basic;
    ACTMOCK_scene.gao.extended = ((pattern >> 1) & 1) ? NULL : &ACTMOCK_scene.ext;
    ACTMOCK_scene.gao.flags = ((pattern >> 2) & 1) ? 4 : 0;
    ACTMOCK_scene.ext.extraFlag = (int16_t)(((pattern >> 3) & 1) ? 1 : 0);
    ACTMOCK_scene.srcGao.basic = &ACTMOCK_scene.srcBasic;

    ACTMOCK_scene.actor.actionKit = &ACTMOCK_scene.kit.kit;
    ACTMOCK_scene.srcActor.actionKit = &ACTMOCK_scene.srcKit.kit;
    ACTMOCK_scene.actor.animPtr[0] = &ACTMOCK_scene.anims[0];
    ACTMOCK_scene.actor.animPtr[1] = ((pattern >> 7) & 1) ? NULL : &ACTMOCK_scene.anims[1];
    ACTMOCK_scene.actor.currentAction = 0xFFFF;
    ACTMOCK_scene.actor.currentActionTransition = 0xFFFF;
    ACTMOCK_scene.actor.actionItem = 0;
    ACTMOCK_scene.actor.transitionElem = NULL;
    ACTMOCK_scene.actor.flagsEgBlendIw2 = 0;
    ACTMOCK_scene.actor.currTime = 0;

    MATH_CopyMatrix(&ACTMOCK_scene.flashMtx, &MATH_gst_IdentityMatrix);
    (void)ACTMOCK_Rand();
}

/* ---- the synthetic files --------------------------------------------- */

static unsigned char* ACTMOCK_w;

static void W8(int v)  { *ACTMOCK_w++ = (unsigned char)v; }
static void W16(int v) { W8(v & 0xFF); W8((v >> 8) & 0xFF); }
static void W32(int v) { W16(v & 0xFFFF); W16((v >> 16) & 0xFFFF); }

void ACTMOCK_BuildFiles(unsigned int seed, int pattern)
{
    int elemNum;
    int i;
    int k;
    int transNum;
    int hasTransition[8];

    ACTMOCK_rngState = seed;
    memset(ACTMOCK_scene.file, 0, sizeof(ACTMOCK_scene.file));
    memset(ACTMOCK_scene.kitFile, 0, sizeof(ACTMOCK_scene.kitFile));

    /* the action file */
    elemNum = (pattern & 7) + 1;
    transNum = 1 + (pattern % 4);
    ACTMOCK_w = ACTMOCK_scene.file;
    W8(elemNum);
    W8(pattern % 5);
    W16(0);
    for (i = 0; i < elemNum; i++)
    {
        hasTransition[i] = ((i + pattern) % 3) != 2;
        W32(((i + pattern) % 4 == 3) ? -1 : (100 + i));      /* eventLists key */
        W32(hasTransition[i] ? (200 + i) : 0);               /* transition key */
        W32(((i + pattern) % 5 == 4) ? -1 : (300 + i));      /* shape key */
        W8((i == 0) ? 0 : (i + 1));                          /* callAmount, 0 -> 1 */
        W8(i * 2);
        W8(20 + i);
        W8(i * 5);
        W16(i * 11);
        W8((i * 7) & 0xFF);
        W8(0);
    }
    for (i = 0; i < elemNum; i++)
    {
        if (!hasTransition[i])
            continue;
        W32(0);
        W32(transNum);
        W32(0);
        W32(2 + i);
        for (k = 0; k < transNum; k++)
        {
            W32(k * 2);
            W32(0);
        }
        for (k = 0; k < transNum; k++)
        {
            W16((k + i) % ACTMOCK_ACTIONS);
            W8(((i + k) & 3) | 0x10);
            W8((k % 3) * 4);
        }
    }
    ACTMOCK_scene.fileSize = (int)(ACTMOCK_w - ACTMOCK_scene.file);

    /* the action-kit file: eight bytes per action, the key and a
       skipped long */
    ACTMOCK_w = ACTMOCK_scene.kitFile;
    for (i = 0; i < ACTMOCK_ACTIONS; i++)
    {
        if (i == ACTMOCK_ACTIONS - 1 && ((pattern >> 1) & 1))
            W32(1);                                 /* the terminator key */
        else if (i == 2)
            W32(2);                                 /* next to the terminator */
        else if ((i + pattern) % 4 == 3)
            W32(-1);
        else if ((i + pattern) % 7 == 6)
            W32(0);
        else
            W32(500 + i);
        W32(0);
    }
    ACTMOCK_scene.kitFileSize = (int)(ACTMOCK_w - ACTMOCK_scene.kitFile);
}

/* ---- comparison ------------------------------------------------------ */

void ACTMOCK_Save(void)
{
    ACTMOCK_savedArenaUsed = ACTMOCK_arenaUsed;
    memcpy(ACTMOCK_savedArena, ACTMOCK_arena, ACTMOCK_arenaUsed);
    ACTMOCK_savedLogLen = ACTMOCK_logLen;
    memcpy(ACTMOCK_savedLog, ACTMOCK_log, sizeof(tActMockCall) * ACTMOCK_logLen);
    ACTMOCK_savedScene = ACTMOCK_scene;
    ACTMOCK_savedScratch = ACTMOCK_scratch;

    ACTMOCK_savedOwner = EVE_gpst_OwnerGAO;
    ACTMOCK_savedData = EVE_gpst_CurrentData;
    ACTMOCK_savedList = EVE_gpst_CurrentListTracks;
    ACTMOCK_savedTrack = EVE_gpst_CurrentTrack;
    ACTMOCK_savedEvent = EVE_gpst_CurrentEvent;
    ACTMOCK_savedRefMtx = EVE_gpst_CurrentRefMatrix;
    ACTMOCK_savedParam = EVE_gpst_CurrentParam;
    ACTMOCK_savedDt = TIM_gf_dt;
}

int ACTMOCK_SameInt(int base, int New, const char* what, const char* where)
{
    ACTMOCK_compares++;
    if (base == New)
        return 0;
    if (!ACTMOCK_reported)
    {
        ACTMOCK_reported = 1;
        ACTMOCK_failures++;
        printf("  [%s] %s %s: base %d new %d\n", ACTMOCK_case, where, what, base, New);
    }
    return 1;
}

int ACTMOCK_SameLong(long long base, long long New, const char* what, const char* where)
{
    ACTMOCK_compares++;
    if (base == New)
        return 0;
    if (!ACTMOCK_reported)
    {
        ACTMOCK_reported = 1;
        ACTMOCK_failures++;
        printf("  [%s] %s %s: base %lld new %lld\n", ACTMOCK_case, where, what, base, New);
    }
    return 1;
}

int ACTMOCK_SameFloat(float base, float New, const char* what, const char* where)
{
    ACTMOCK_compares++;
    if (memcmp(&base, &New, sizeof(float)) == 0)
        return 0;
    if (isnan(base) && isnan(New))
        return 0;
    if (!ACTMOCK_reported)
    {
        ACTMOCK_reported = 1;
        ACTMOCK_failures++;
        printf("  [%s] %s %s: base %.9g new %.9g\n", ACTMOCK_case, where, what,
               (double)base, (double)New);
    }
    return 1;
}

int ACTMOCK_SameBytes(const void* base, const void* New, size_t n,
                      const char* what, const char* where)
{
    size_t i;
    const unsigned char* a = (const unsigned char*)base;
    const unsigned char* b = (const unsigned char*)New;

    ACTMOCK_compares++;
    if (memcmp(a, b, n) == 0)
        return 0;
    for (i = 0; i < n; i++)
        if (a[i] != b[i])
            break;
    if (!ACTMOCK_reported)
    {
        ACTMOCK_reported = 1;
        ACTMOCK_failures++;
        size_t lo = i > 16 ? i - 16 : 0;
        size_t hi = lo + 48 < n ? lo + 48 : n;
        size_t j;

        printf("  [%s] %s %s differs at byte %lu of %lu: base %02x new %02x\n",
               ACTMOCK_case, where, what, (unsigned long)i, (unsigned long)n,
               a[i], b[i]);
        printf("      base %lu:", (unsigned long)lo);
        for (j = lo; j < hi; j++)
            printf(" %02x", a[j]);
        printf("\n      new  %lu:", (unsigned long)lo);
        for (j = lo; j < hi; j++)
            printf(" %02x", b[j]);
        printf("\n");
    }
    return 1;
}

static int ACTMOCK_CompareLog(const char* where)
{
    int i;
    int j;

    if (ACTMOCK_logOverflow)
    {
        printf("  [%s] call log overflowed\n", ACTMOCK_case);
        ACTMOCK_failures++;
        return 1;
    }
    if (ACTMOCK_SameInt(ACTMOCK_savedLogLen, ACTMOCK_logLen, "call count", where))
    {
        for (i = 0; i < ACTMOCK_savedLogLen && i < ACTMOCK_logLen; i++)
            if (ACTMOCK_savedLog[i].fn != ACTMOCK_log[i].fn)
            {
                printf("      first differing call %d: base %s new %s\n", i,
                       ACTMOCK_savedLog[i].fn, ACTMOCK_log[i].fn);
                break;
            }
        return 1;
    }
    for (i = 0; i < ACTMOCK_logLen; i++)
    {
        const tActMockCall* a = &ACTMOCK_savedLog[i];
        const tActMockCall* b = &ACTMOCK_log[i];

        if (a->fn != b->fn)
        {
            if (!ACTMOCK_reported)
            {
                ACTMOCK_reported = 1;
                ACTMOCK_failures++;
                printf("  [%s] %s call %d: base %s new %s\n", ACTMOCK_case, where, i,
                       a->fn, b->fn);
            }
            return 1;
        }
        for (j = 0; j < a->ni; j++)
            if (ACTMOCK_SameInt(a->ia[j], b->ia[j], a->fn, where))
                return 1;
        for (j = 0; j < a->nf; j++)
            if (ACTMOCK_SameFloat(a->fa[j], b->fa[j], a->fn, where))
                return 1;
    }
    return 0;
}

int ACTMOCK_CompareWithSaved(const char* where)
{
    int bad = 0;

    bad |= ACTMOCK_CompareLog(where);
    bad |= ACTMOCK_SameLong((long long)ACTMOCK_savedArenaUsed,
                            (long long)ACTMOCK_arenaUsed, "arena used", where);
    if (ACTMOCK_savedArenaUsed == ACTMOCK_arenaUsed)
        bad |= ACTMOCK_SameBytes(ACTMOCK_savedArena, ACTMOCK_arena,
                                 ACTMOCK_arenaUsed, "arena", where);
    bad |= ACTMOCK_SameBytes(&ACTMOCK_savedScene, &ACTMOCK_scene,
                             sizeof(tActScene), "scene", where);
    bad |= ACTMOCK_SameBytes(&ACTMOCK_savedScratch, &ACTMOCK_scratch,
                             sizeof(tActScratch), "scratch", where);

    bad |= ACTMOCK_SameInt(ACTMOCK_Tag(ACTMOCK_savedOwner), ACTMOCK_Tag(EVE_gpst_OwnerGAO),
                           "EVE_gpst_OwnerGAO", where);
    bad |= ACTMOCK_SameInt(ACTMOCK_Tag(ACTMOCK_savedData), ACTMOCK_Tag(EVE_gpst_CurrentData),
                           "EVE_gpst_CurrentData", where);
    bad |= ACTMOCK_SameInt(ACTMOCK_Tag(ACTMOCK_savedList),
                           ACTMOCK_Tag(EVE_gpst_CurrentListTracks),
                           "EVE_gpst_CurrentListTracks", where);
    bad |= ACTMOCK_SameInt(ACTMOCK_Tag(ACTMOCK_savedTrack), ACTMOCK_Tag(EVE_gpst_CurrentTrack),
                           "EVE_gpst_CurrentTrack", where);
    bad |= ACTMOCK_SameInt(ACTMOCK_Tag(ACTMOCK_savedEvent), ACTMOCK_Tag(EVE_gpst_CurrentEvent),
                           "EVE_gpst_CurrentEvent", where);
    bad |= ACTMOCK_SameInt(ACTMOCK_Tag(ACTMOCK_savedRefMtx),
                           ACTMOCK_Tag(EVE_gpst_CurrentRefMatrix),
                           "EVE_gpst_CurrentRefMatrix", where);
    bad |= ACTMOCK_SameInt(ACTMOCK_Tag(ACTMOCK_savedParam), ACTMOCK_Tag(EVE_gpst_CurrentParam),
                           "EVE_gpst_CurrentParam", where);
    bad |= ACTMOCK_SameFloat(ACTMOCK_savedDt, TIM_gf_dt, "TIM_gf_dt", where);
    return bad;
}

void ACTMOCK_Summary(const char* title)
{
    printf("%s: %d cases, %ld comparisons, %d failing\n", title, ACTMOCK_cases,
           ACTMOCK_compares, ACTMOCK_failures);
}
