// Differential test for animation.c: the frozen pre-rewrite copy against
// the rewritten one, over identical state.
//
// Everything both copies touch lives in one arena at a fixed address, so
// a case is compared three ways: the whole arena word for word, the
// engine globals the file writes, and the ordered log of every mocked
// engine call with its arguments.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "../gameobject.h"
#include "../animation.h"
#include "../events.h"
#include "../math.h"
#include "../time.h"
#include "../world.h"
#include "../groups.h"
#include "../profiler.h"
#include "../modifiers.h"
#include "../graphicsobjects/gro_common.h"
#include "animation_mocks.h"

// What the stand-ins hand back.
char* ANIT_fileData;
int ANIT_fileSize;
void* ANIT_keyPos[16];
struct sGameObjectEventsList* ANIT_loadedList;
void* ANIT_loadedGroup;

// -------------------------------------------------------------- both copies

#define P2(ret, name, a1, a2) \
    ret BASE_##name(a1, a2); ret NEW_##name(a1, a2);
#define P3(ret, name, a1, a2, a3) \
    ret BASE_##name(a1, a2, a3); ret NEW_##name(a1, a2, a3);
#define P4(ret, name, a1, a2, a3, a4) \
    ret BASE_##name(a1, a2, a3, a4); ret NEW_##name(a1, a2, a3, a4);
#define P5(ret, name, a1, a2, a3, a4, a5) \
    ret BASE_##name(a1, a2, a3, a4, a5); ret NEW_##name(a1, a2, a3, a4, a5);
#define P1(ret, name, a1) \
    ret BASE_##name(a1); ret NEW_##name(a1);

P3(void, ANI_LimitFrequencyBetween0And255, tAnim*, Vec3*, Vec3*)
P1(void, ANI_ClearBlendingFlag, tGameObjectMain*)
P1(void, ANI_DestroyAnimation, tAnim*)
P1(void, ANI_DestroySkeleton, tGroup*)
P1(void, ANI_Free, tGameObjectActorObject*)
P1(void, ANI_ApplyHierarchyOnFlash, tGameObjectMain*)
P1(int, ANI_GaoAnimationIsTransitioning, tGameObjectMain*)
P2(void, ANI_RecomputeFlash, tGameObjectMain*, tAnim*)
P2(void, ANI_UpdateFlash, tGameObjectMain*, char)
P2(uint8_t, ANI_uc_GetEngineCanalByAICanal, tGameObjectMain*, char)
P2(tGameObjectMain*, ANI_pst_GetObjectByAICanal, tGameObjectMain*, uint8_t)
P4(void, ANI_ChangeBoneShape, tGameObjectMain*, uint8_t, uint8_t, int)
P1(void, ANI_ApplyShape, tGameObjectMain*)
P2(void, ANI_ChangeShape, tGameObjectMain*, tShape*)
P2(void, ANI_SetTrackList, tAnim*, tGameObjectEventsList*)
P2(void, ANI_ChangeAction, tGameObjectMain*, tGameObjectActorObject*)
P3(void, ANI_UpdateFlashAfterMagicBoxTranslation, tGameObjectMain*, Vec3*, char)
P2(void, ANI_GetEndSpeedOfMB, tAnim*, Vec3*)
P4(void, ANI_InterpolateMagicBoxPositionAfterEndOfAnim, tGameObjectMain*, tGameObjectActorObject*, tAnim*, float)
P3(void, ANI_b_ComputeMagicBoxPositionForBlendAnim, tGameObjectMain*, tGameObjectActorObject*, tAnim*)
P3(void, ANI_b_BlendMagicBox, tGameObjectMain*, tGameObjectActorObject*, tAnim*)
P3(void, ANI_PlayLeadAnimation, tGameObjectMain*, tGameObjectActorObject*, tAnim*)
P4(void, ANI_PlayGameObjectAnimation, tGameObjectMain*, tGameObjectActorObject*, float, Vec3*)
P1(void, ANI_OneCall, tGameObjectMain*)
P2(void, ANI_InitSkeleton, tGroup*, tGameObjectMain*)
P3(void, ANI_pst_DuplicateGameObjectAnim, tWorld*, tGameObjectMain*, tGameObjectMain*)
P1(tShape*, ANI_ul_ShapeCallback, uint32_t)
P2(void, ANI_AttachSkeletonModelToWorld, tGroup*, tWorld*)
P5(void, ANI_pst_Load, int, int, int, tGameObjectMain*, tWorld*)
P1(void, ANI_ResolveRefs, tGameObjectMain*)
P1(void, ANI_ReinitAnimation, tAnim*)
P1(void, ANI_Reinit, tGameObjectMain*)
P1(void, ANI_RemoveHierarchyOnFlash, tGameObjectMain*)
P1(tGameObjectMain*, ANI_pst_GetReference, tGameObjectMain*)
P3(void, ANI_PlayBlendAnimationWithoutMagicBox, tGameObjectMain*, tGameObjectActorObject*, tAnim*)
P1(void, ANI_BeforeRender, tGameObjectMain*)
P1(void, ANI_BeforeRendering, tWorld*)
P1(void, ANI_MainCall, tWorld*)
P1(tGameObjectMain*, ANI_pst_GetReferenceInit, tGameObjectMain*)
P4(void, ANI_ComputeBoneMatrix, tGameObjectMain*, tGameObjectMain*, Mtx4x4*, int)
P1(void, ANI_Render, tGameObjectMain*)
P2(void, ANI_ChangeDefaultSkeleton, tGameObjectMain*, tGameObjectMain*)
P2(void, ANI_RestoreDefaultSkeleton, tGameObjectMain*, tGameObjectMain*)
P3(void, ANI_CloneSet, tGameObjectMain*, tGameObjectMain*, int)
P1(tGameObjectActorObject*, ANI_pst_CreateGameObjectAnim, void)
P1(tAnim*, ANI_AllocAnim, void)

extern uint8_t BASE_ANI_c_DefaultBitFieldLOD;
extern uint8_t NEW_ANI_c_DefaultBitFieldLOD;

#define CALL(fn, ...) (side ? NEW_##fn(__VA_ARGS__) : BASE_##fn(__VA_ARGS__))
#define CALL0(fn)     (side ? NEW_##fn() : BASE_##fn())

// ------------------------------------------------------------- the fixture

typedef struct Fixture
{
    tWorld* world;
    tGameObjectMain* gao;
    tGameObjectActorObject* actor;
    tGroup* skeleton;
    tGroup* model;
    tGameObjectMain* bone[6];
    int boneNum;
    tShape* shape;
    tShape* shape2;
    tAnim* anim[4];
    tActionKit* kit;
    tAction* action;
    tGameObjectEventsList* list;
    tGameObjectMain* other;
    tGameObjectActorObject* otherActor;
} Fixture;

static Fixture fx;
static unsigned rng;
static int g_invalidEntry;

static unsigned rnd(void)
{
    rng = rng * 1664525u + 1013904223u;
    return rng >> 8;
}

static float rndf(float lo, float hi)
{
    return lo + (hi - lo) * (float)(rnd() & 0xFFFF) / 65535.0f;
}

static void mkMtx(Mtx4x4* m)
{
    m->v1.x = rndf(0.5f, 1.5f);  m->v1.y = rndf(-0.3f, 0.3f); m->v1.z = rndf(-0.3f, 0.3f);
    m->_m14 = rndf(-0.1f, 0.1f);
    m->v2.x = rndf(-0.3f, 0.3f); m->v2.y = rndf(0.5f, 1.5f);  m->v2.z = rndf(-0.3f, 0.3f);
    m->_m24 = rndf(-0.1f, 0.1f);
    m->v3.x = rndf(-0.3f, 0.3f); m->v3.y = rndf(-0.3f, 0.3f); m->v3.z = rndf(0.5f, 1.5f);
    m->_m34 = rndf(-0.1f, 0.1f);
    m->Pos.x = rndf(-50.0f, 50.0f);
    m->Pos.y = rndf(-50.0f, 50.0f);
    m->Pos.z = rndf(-50.0f, 50.0f);
    m->w = 1.0f;
    m->mtxflag = 0x2 | 0x4 | 0x8;
    ANIT_MarkMtx(m, 1);
}

static tableDescStruct* mkTable(int n)
{
    tableDescStruct* tab = (tableDescStruct*)ANIT_Alloc(sizeof(tableDescStruct));
    tableElemStructPF* e = (tableElemStructPF*)ANIT_Alloc(sizeof(tableElemStructPF) * (n + 2));

    tab->firstElemPtr = e;
    tab->nextElemPtr = e + n;
    tab->lastElemPtr = e + n + 2;
    tab->actElemNum = n;
    tab->holeNum = 0;
    tab->tableType = 1;
    return tab;
}

static tGameObjectMain* mkGao(int flags)
{
    tGameObjectMain* g = (tGameObjectMain*)ANIT_Alloc(sizeof(tGameObjectMain));

    g->flags = flags;
    g->flags2 = 2;
    g->distance = 7;
    g->matrices = (Mtx4x4*)ANIT_Alloc(sizeof(Mtx4x4) * 3);
    mkMtx(&g->matrices[0]);
    mkMtx(&g->matrices[1]);
    mkMtx(&g->matrices[2]);
    if (flags & 0x1000)
    {
        g->basic = (tGameObjectBasic*)ANIT_Alloc(sizeof(tGameObjectBasic));
        if (flags & 0x400000)
        {
            g->basic->mtx = (tGaoMatrix*)ANIT_Alloc(sizeof(tGaoMatrix));
            mkMtx(&g->basic->mtx->mtx);
        }
    }
    return g;
}

static tAnim* mkAnim(int flags, int flags2, int transitionFlags, int freq, int tracks, int keys)
{
    tAnim* a = (tAnim*)ANIT_Alloc(sizeof(tAnim));
    tGameObjectEventsList* l = (tGameObjectEventsList*)ANIT_Alloc(sizeof(tGameObjectEventsList));
    tGameObjectEventsListElem* le =
        (tGameObjectEventsListElem*)ANIT_Alloc(sizeof(tGameObjectEventsListElem) * (tracks + 1));
    int i, k;

    a->flags = (int16_t)flags;
    a->flags2 = (uint8_t)flags2;
    a->transitionFlags = (uint8_t)transitionFlags;
    a->frequency = (uint8_t)freq;
    a->actionLength = 12;
    a->startTime = 2048;
    a->actionForTransition = 0;
    a->actionElemForTransition = 0;
    mkMtx(&a->flashMtx);
    a->ptr = (tEventData*)ANIT_Alloc(sizeof(tEventData));
    a->ptr->eventListStruct = l;
    a->ptr->tracks = (tAnimTracks*)ANIT_Alloc(sizeof(tAnimTracks) * (tracks + 4));
    for (i = 0; i < tracks + 4; i++)
    {
        a->ptr->tracks[i].currTime = rndf(0.0f, 1.0f);
        a->ptr->tracks[i].flag = (int16_t)(1 + 2 * (i & 1));
        *(float*)&a->ptr->tracks[i].privateData = rndf(0.1f, 0.9f);
    }
    l->num = (uint16_t)tracks;
    l->flags = 0;
    l->usageCount = 1;
    l->listElems = le;
    for (i = 0; i < tracks + 1; i++)
    {
        le[i].boneID = (uint16_t)(i < 2 ? 0xFFFF : i);
        le[i].elemNum = (uint16_t)keys;
        le[i].flags = 0;
        le[i].size = 0;
        le[i].gao = 0;
        le[i].trackElems =
            (tGameObjectEventElem*)ANIT_Alloc(sizeof(tGameObjectEventElem) * (keys + 1));
        for (k = 0; k < keys; k++)
        {
            le[i].trackElems[k].duration = (uint16_t)(1 + k);
            // 0x7C0 field: 0 keeps the backward scan going, 128 stops it,
            // anything else gives up on the track.
            le[i].trackElems[k].flags2 = (int16_t)(k == keys - 1 ? 128 : 0);
            le[i].trackElems[k].eptrdu.size = (uint16_t)(k & 3);
            le[i].trackElems[k].eptrdu.type = 0;
        }
    }
    return a;
}

static tShape* mkShape(int base)
{
    tShape* s = (tShape*)ANIT_Alloc(sizeof(tShape));
    int i;

    s->num = 64;
    s->usageCount = 2;
    for (i = 0; i < 64; i++)
    {
        s->shapesOriginal[i] = (uint8_t)(base + i);
        s->shapes[i] = (uint8_t)(i < 3 ? i : 0xFF);
    }
    s->shapesOriginal[3] = 0xFF;
    return s;
}

static void mkActionKit(void)
{
    tAction** actions;
    tActionElem* elems;
    int i;

    fx.kit = (tActionKit*)ANIT_Alloc(sizeof(tActionKit) + sizeof(tAction*) * 4);
    fx.kit->elemNum = 4;
    fx.kit->usageCount = 1;
    fx.kit->flags = 0;
    fx.kit->savedActions = 0;
    actions = (tAction**)((char*)fx.kit + sizeof(tActionKit));

    for (i = 0; i < 4; i++)
    {
        tAction* a = (tAction*)ANIT_Alloc(sizeof(tAction) + sizeof(tActionElem) * 3);
        a->actionElemNum = 3;
        a->actionElemIDForTransition = (uint8_t)(i == 3 ? 255 : 1);
        a->usageCount = 1;
        elems = (tActionElem*)((char*)a + sizeof(tAction));
        elems[0].eventLists = fx.anim[0] ? fx.anim[0]->ptr->eventListStruct : 0;
        elems[0].changeActionCallAmountToChange = 1;
        elems[0].actionFrequency = 32;
        elems[0].b5 = 0x25;
        elems[0].shape = fx.shape;
        elems[1].eventLists = fx.anim[1] ? fx.anim[1]->ptr->eventListStruct : 0;
        elems[1].changeActionCallAmountToChange = 2;
        elems[1].actionFrequency = 48;
        elems[1].b5 = 0x1A;
        elems[1].shape = fx.shape2;
        elems[2].eventLists = elems[0].eventLists;
        elems[2].changeActionCallAmountToChange = 3;
        elems[2].actionFrequency = 60;
        elems[2].b5 = 0x07;
        elems[2].shape = 0;
        actions[i] = a;
        if (i == 0)
            fx.action = a;
    }
}

// boneFlags lets a case make a bone that OBJ_pst_GetLocalMatrix refuses.
static void mkSkeleton(int n, int boneFlags)
{
    tableElemStructPF* e;
    int i;

    fx.skeleton = (tGroup*)ANIT_Alloc(sizeof(tGroup));
    fx.skeleton->tablePF = mkTable(n);
    fx.skeleton->groupElemNum = 2;
    e = (tableElemStructPF*)fx.skeleton->tablePF->firstElemPtr;
    fx.boneNum = n;
    for (i = 0; i < n; i++)
    {
        tGameObjectMain* b = mkGao(0x1000 | 0x2 | 0x4000 | 0x2000
            | ((i == 2) ? (boneFlags & ~0x400000) : boneFlags));
        b->basic->actorObjectOrVisualObject = ANIT_Alloc(sizeof(tGameObjectVisualObject));
        b->extended = (tGameObjectExtended*)ANIT_Alloc(sizeof(tGameObjectExtended));
        b->extended->grp = fx.skeleton;
        b->basic->multiPartData =
            (tGameObjectMultiPartData*)ANIT_Alloc(sizeof(tGameObjectMultiPartData));
        b->basic->multiPartData->elemNum = 2;
        b->basic->multiPartData->multipartGaoObjectPartsOrMatrices =
            ANIT_Alloc(sizeof(Mtx4x4) * 3);
        b->flags2 = (i == 1) ? -1 : 2;
        e[i].objectPtr = b;
        e[i].objectFlags = b->flags;
        fx.bone[i] = b;
    }
    // One invalid entry, as the free list leaves them. ANI_Render and
    // ANI_ComputeBoneMatrix do not test for one, in either copy, so the
    // cases that drive those ask for a clean table.
    if (n >= 4 && g_invalidEntry)
        e[3].objectPtr = (void*)((size_t)e[3].objectPtr | 1u);
}

static void mkWorld(void)
{
    tableElemStructPF* e;

    fx.world = (tWorld*)ANIT_Alloc(sizeof(tWorld));
    fx.world->objectTablePF = *mkTable(2);
    fx.world->visibleGameObjectsPF = *mkTable(2);
    fx.world->eot.animatedObjectsTablePF = *mkTable(2);
    e = (tableElemStructPF*)fx.world->visibleGameObjectsPF.firstElemPtr;
    e[0].objectPtr = fx.gao;
    e[1].objectPtr = fx.other;
    e = (tableElemStructPF*)fx.world->eot.animatedObjectsTablePF.firstElemPtr;
    e[0].objectPtr = fx.gao;
    e[1].objectPtr = fx.other;
    WOR_gpst_CurrentWorld = fx.world;
}

// The whole thing: an animated object with a four-bone skeleton, a lead
// and a blend animation, shapes, an action kit and a world around it.
static void build(int gaoFlags, int blendMask)
{
    tGameObjectMultiPartData* mp;
    int i;

    memset(&fx, 0, sizeof(fx));
    rng = 12345u;

    fx.gao = mkGao(gaoFlags);
    fx.actor = (tGameObjectActorObject*)ANIT_Alloc(sizeof(tGameObjectActorObject));
    fx.gao->basic->actorObjectOrVisualObject = fx.actor;
    fx.gao->extended = (tGameObjectExtended*)ANIT_Alloc(sizeof(tGameObjectExtended));
    fx.gao->extended->extraFlag = 1;
    fx.gao->basic->dyn = (tDyn*)ANIT_Alloc(2048);
    fx.gao->basic->dyn->mass = 2.5f;
    fx.gao->basic->dyn->forceVector.x = 1.25f;
    fx.gao->basic->dyn->forceVector.y = -0.5f;
    fx.gao->basic->dyn->forceVector.z = 3.0f;

    mkSkeleton(4, 0x400000);
    fx.model = (tGroup*)ANIT_Alloc(sizeof(tGroup));
    fx.model->tablePF = mkTable(2);
    fx.model->groupElemNum = 1;
    ((tableElemStructPF*)fx.model->tablePF->firstElemPtr)[0].objectPtr = fx.bone[0];
    ((tableElemStructPF*)fx.model->tablePF->firstElemPtr)[1].objectPtr = fx.bone[1];

    fx.shape = mkShape(0);
    fx.shape2 = mkShape(1);

    fx.anim[0] = mkAnim(0x0002, 0, 0, 40, 3, 4);
    fx.anim[1] = mkAnim(0x0002, 0, 4, 50, 3, 3);
    fx.anim[2] = mkAnim(0x0006, 1, 8, 60, 2, 2);
    fx.anim[3] = mkAnim(0x0000, 0, 0, 30, 2, 2);
    fx.list = fx.anim[3]->ptr->eventListStruct;
    mkActionKit();

    fx.actor->animPtr[0] = fx.anim[0];
    fx.actor->animPtr[1] = fx.anim[1];
    fx.actor->animPtr[2] = fx.anim[2];
    fx.actor->animPtr[3] = fx.anim[3];
    fx.actor->flagsEgBlendIw2 = (uint8_t)blendMask;
    fx.actor->skeletonGroup = fx.skeleton;
    fx.actor->group = fx.model;
    fx.actor->shape = fx.shape;
    fx.actor->shape2 = fx.shape;
    fx.actor->actionPtr = fx.action;
    fx.actor->actionKit = fx.kit;
    fx.actor->actionItem = 0;
    fx.actor->currentAction = 0;
    fx.actor->currentActionTransition = 3;
    fx.actor->callCountForChangeAction = 0;
    fx.actor->initedInANIMainCall = 0;
    fx.actor->eventRunningFrequency = 0;
    fx.actor->elapsedDT = 0.125f;
    fx.actor->currTime = 90;
    for (i = 0; i < 64; i++)
        fx.actor->shapes[i] = (uint8_t)(i < 3 ? i : 0xFF);

    mp = (tGameObjectMultiPartData*)ANIT_Alloc(sizeof(tGameObjectMultiPartData));
    mp->elemNum = 6;
    mp->multipartGaoObjectPartsOrMatrices = ANIT_Alloc(sizeof(Mtx4x4) * 7);
    for (i = 0; i < 7; i++)
        mkMtx(&((Mtx4x4*)mp->multipartGaoObjectPartsOrMatrices)[i]);
    fx.gao->basic->multiPartData = mp;
    fx.gao->flags |= 0x200000;

    // A second animated object, for the skeleton swap and the clone.
    fx.other = mkGao(gaoFlags);
    fx.otherActor = (tGameObjectActorObject*)ANIT_Alloc(sizeof(tGameObjectActorObject));
    fx.other->basic->actorObjectOrVisualObject = fx.otherActor;
    fx.other->basic->dyn = (tDyn*)ANIT_Alloc(2048);
    fx.other->basic->dyn->mass = 1.5f;
    fx.other->extended = (tGameObjectExtended*)ANIT_Alloc(sizeof(tGameObjectExtended));
    fx.otherActor->animPtr[0] = mkAnim(0x0002, 0, 0, 45, 3, 3);
    fx.otherActor->flagsEgBlendIw2 = 1;
    fx.otherActor->skeletonGroup = fx.skeleton;
    fx.otherActor->group = fx.model;
    fx.otherActor->shape = fx.shape2;
    fx.otherActor->shape2 = fx.shape2;
    fx.otherActor->actionKit = fx.kit;
    fx.other->basic->multiPartData = mp;
    fx.other->flags |= 0x200000;
    if (fx.gao->basic->mtx)
    {
        fx.gao->basic->mtx->refGao = fx.other;
        fx.gao->basic->mtx->refGao2 = fx.other;
    }

    mkWorld();
    TIM_gf_dt = 0.75f;
    ENG_gb_EVERunning = 0;
    EVE_gb_CanFlash = 1;
    EVE_gb_ForcePlayerOnAllTracks = 0;
    ANI_gst_PositionBeforeAnimPlayer.x = 1.0f;
    ANI_gst_PositionBeforeAnimPlayer.y = 2.0f;
    ANI_gst_PositionBeforeAnimPlayer.z = 3.0f;
    EVE_gpst_OwnerGAO = 0;
    EVE_gpst_CurrentGAO = 0;
    EVE_gpst_CurrentListTracks = 0;
    EVE_gpst_CurrentTrack = 0;
    EVE_gpst_CurrentEvent = 0;
    EVE_gpst_CurrentRefMatrix = 0;
    EVE_gpst_CurrentData = 0;
    EVE_gpst_CurrentParam = 0;
    PushedStack = 0;
    memset(pst_RenderingStack, 0, sizeof(pst_RenderingStack));
    memset(PerfCounterStruct, 0, sizeof(PerfCounterStruct));
    memset(PerfCountertimeSumArray, 0, sizeof(PerfCountertimeSumArray));
    AI_C_Callback = 0;
}

// ------------------------------------------------------------- the snapshot

typedef struct Snap
{
    char arena[ANIT_ARENA_SIZE];
    size_t used;
    ANIT_LogEntry log[ANIT_LOG_MAX];
    int logN;
    Vec3 posBefore;
    int pushed;
    float dt;
    int aiCallback;
    int everunning;
    int canFlash;
    void* owner;
    void* curGao;
    void* curList;
    void* curTrack;
    void* curEvent;
    void* curRef;
    void* curData;
    void* curParam;
    long long perf[8];
    long long perfSum[8];
    double ret;
} Snap;

static Snap snapA, snapB;

static void snapshot(Snap* s, double ret)
{
    int i;

    memcpy(s->arena, ANIT_arena, ANIT_ARENA_SIZE);
    s->used = ANIT_arenaUsed;
    memcpy(s->log, ANIT_log, sizeof(s->log));
    s->logN = ANIT_logN;
    s->posBefore = ANI_gst_PositionBeforeAnimPlayer;
    s->pushed = PushedStack;
    s->dt = TIM_gf_dt;
    s->aiCallback = AI_C_Callback;
    s->everunning = ENG_gb_EVERunning;
    s->canFlash = EVE_gb_CanFlash;
    s->owner = EVE_gpst_OwnerGAO;
    s->curGao = EVE_gpst_CurrentGAO;
    s->curList = EVE_gpst_CurrentListTracks;
    s->curTrack = EVE_gpst_CurrentTrack;
    s->curEvent = EVE_gpst_CurrentEvent;
    s->curRef = EVE_gpst_CurrentRefMatrix;
    s->curData = EVE_gpst_CurrentData;
    s->curParam = EVE_gpst_CurrentParam;
    for (i = 0; i < 8; i++)
    {
        s->perf[i] = PerfCounterStruct[i].QuadPart;
        s->perfSum[i] = PerfCountertimeSumArray[i];
    }
    s->ret = ret;
}

// ------------------------------------------------------------ the comparison

static int g_fails;
static long g_cases;
static long g_compared;
static long g_skipped;
static long g_diffCount;
static double g_worstAbs;
static double g_worstRel;
static char g_worstCase[96] = "-";
static char g_case[96] = "-";

static int closeEnough(double a, double b, const char* what, long where)
{
    double d, r, m;

    if (a == b)
        return 1;
    d = fabs(a - b);
    m = fabs(a) > fabs(b) ? fabs(a) : fabs(b);
    r = m > 0.0 ? d / m : d;
    g_diffCount++;
    if (d > g_worstAbs)
        g_worstAbs = d;
    if (r > g_worstRel)
    {
        g_worstRel = r;
        strncpy(g_worstCase, g_case, sizeof(g_worstCase) - 1);
    }
    if (r > 1e-5)
    {
        printf("  %s: %s at %ld: base %.9g new %.9g (abs %.3g rel %.3g)\n",
            g_case, what, where, a, b, d, r);
        return 0;
    }
    return 1;
}

static int plausibleFloat(float f)
{
    double a = fabs((double)f);

    if (f != f)
        return 0;
    return a == 0.0 || (a > 1e-20 && a < 1e20);
}

static void compare(const char* name)
{
    size_t off;
    int i;
    int bad = 0;
    size_t used = snapA.used > snapB.used ? snapA.used : snapB.used;

    strncpy(g_case, name, sizeof(g_case) - 1);
    g_cases++;

    if (snapA.used != snapB.used)
    {
        printf("  %s: arena use %zu vs %zu\n", name, snapA.used, snapB.used);
        bad = 1;
    }
    for (off = 0; off + 4 <= used; off += 4)
    {
        unsigned av, bv;
        float af, bf;

        memcpy(&av, snapA.arena + off, 4);
        memcpy(&bv, snapB.arena + off, 4);
        g_compared++;
        if (av == bv)
            continue;
        if (ANIT_SkipWord(off))
        {
            g_skipped++;
            continue;
        }
        memcpy(&af, snapA.arena + off, 4);
        memcpy(&bf, snapB.arena + off, 4);
        if (plausibleFloat(af) && plausibleFloat(bf))
        {
            if (!closeEnough((double)af, (double)bf, "arena float", (long)off))
                bad = 1;
        }
        else
        {
            printf("  %s: arena word at %ld: base %08x new %08x\n",
                name, (long)off, av, bv);
            bad = 1;
        }
    }

    if (snapA.logN != snapB.logN)
    {
        printf("  %s: call count %d vs %d\n", name, snapA.logN, snapB.logN);
        bad = 1;
    }
    for (i = 0; i < snapA.logN && i < snapB.logN; i++)
    {
        const char* an = snapA.log[i].name ? snapA.log[i].name : "?";
        const char* bn = snapB.log[i].name ? snapB.log[i].name : "?";

        g_compared++;
        if (strcmp(an, bn) != 0)
        {
            printf("  %s: call %d: base %s new %s\n", name, i, an, bn);
            bad = 1;
            continue;
        }
        if (!closeEnough(snapA.log[i].a, snapB.log[i].a, an, i)
            || !closeEnough(snapA.log[i].b, snapB.log[i].b, an, i)
            || !closeEnough(snapA.log[i].c, snapB.log[i].c, an, i)
            || !closeEnough(snapA.log[i].d, snapB.log[i].d, an, i))
            bad = 1;
    }

    g_compared += 20;
    if (!closeEnough(snapA.ret, snapB.ret, "return", 0)) bad = 1;
    if (!closeEnough(snapA.posBefore.x, snapB.posBefore.x, "posBefore.x", 0)) bad = 1;
    if (!closeEnough(snapA.posBefore.y, snapB.posBefore.y, "posBefore.y", 0)) bad = 1;
    if (!closeEnough(snapA.posBefore.z, snapB.posBefore.z, "posBefore.z", 0)) bad = 1;
    if (!closeEnough(snapA.dt, snapB.dt, "TIM_gf_dt", 0)) bad = 1;
    if (snapA.pushed != snapB.pushed
        || snapA.aiCallback != snapB.aiCallback
        || snapA.everunning != snapB.everunning
        || snapA.canFlash != snapB.canFlash
        || snapA.owner != snapB.owner
        || snapA.curGao != snapB.curGao
        || snapA.curList != snapB.curList
        || snapA.curTrack != snapB.curTrack
        || snapA.curEvent != snapB.curEvent
        || snapA.curRef != snapB.curRef
        || snapA.curData != snapB.curData
        || snapA.curParam != snapB.curParam)
    {
        printf("  %s: an engine global differs\n", name);
        bad = 1;
    }
    for (i = 0; i < 8; i++)
    {
        if (snapA.perf[i] != snapB.perf[i] || snapA.perfSum[i] != snapB.perfSum[i])
        {
            printf("  %s: profiler slot %d differs\n", name, i);
            bad = 1;
        }
    }

    if (bad)
        g_fails++;
}

// Each case builds the state from scratch, runs one side and snapshots.
typedef double (*CaseFn)(int side);

static void runCase(const char* name, CaseFn fn)
{
    double ret;

    if (getenv("ANIT_TRACE"))
        printf("[%s]\n", name);

    g_invalidEntry = 1;
    ANIT_ResetArena();
    ANIT_ResetLog();
    ret = fn(0);
    snapshot(&snapA, ret);

    g_invalidEntry = 1;
    ANIT_ResetArena();
    ANIT_ResetLog();
    ret = fn(1);
    snapshot(&snapB, ret);

    compare(name);
}

// ------------------------------------------------------------------- cases

static double c_limitFrequency(int side)
{
    Vec3 step, speed;
    tAnim* a;
    int i;
    double acc = 0.0;

    build(0x1000 | 0x2 | 0x400000 | 0x4000000, 1);
    a = fx.anim[0];
    for (i = 0; i < 6; i++)
    {
        step.x = (float)i * 0.25f;
        step.y = (float)i * -0.5f;
        step.z = (i == 3) ? 0.0f : 1.0f;
        if (i == 0)
        {
            step.x = 0.0f; step.y = 0.0f; step.z = 0.0f;
        }
        speed.x = 10.0f; speed.y = (float)i; speed.z = -3.0f;
        CALL(ANI_LimitFrequencyBetween0And255, a, &step, &speed);
        acc += a->frequency;
    }
    return acc;
}

static double c_shapeCallback(int side)
{
    static char data[64];
    tShape* s;
    int i;

    build(0x1000 | 0x2 | 0x400000, 1);
    for (i = 0; i < 64; i++)
        data[i] = (char)(i * 7);
    data[0] = 5;
    ANIT_fileData = data;
    ANIT_fileSize = 64;
    s = CALL(ANI_ul_ShapeCallback, 0x1234u);
    return (double)s->num + s->shapes[14] + s->shapesOriginal[14];
}

static double c_canal(int side)
{
    double acc = 0.0;
    int i;

    build(0x1000 | 0x2 | 0x400000, 1);
    for (i = 0; i < 8; i++)
    {
        acc += CALL(ANI_uc_GetEngineCanalByAICanal, fx.gao, (char)i);
        acc += ANIT_Ref(CALL(ANI_pst_GetObjectByAICanal, fx.gao, (uint8_t)i));
    }
    fx.actor->shape = 0;
    for (i = 0; i < 4; i++)
    {
        acc += CALL(ANI_uc_GetEngineCanalByAICanal, fx.gao, (char)i);
        acc += ANIT_Ref(CALL(ANI_pst_GetObjectByAICanal, fx.gao, (uint8_t)i));
    }
    fx.actor->skeletonGroup = 0;
    acc += CALL(ANI_uc_GetEngineCanalByAICanal, fx.gao, 1);
    acc += ANIT_Ref(CALL(ANI_pst_GetObjectByAICanal, fx.gao, 1));
    fx.gao->flags &= ~2;
    acc += CALL(ANI_uc_GetEngineCanalByAICanal, fx.gao, 1);
    return acc;
}

static double c_applyShape(int side)
{
    build(0x1000 | 0x2 | 0x400000, 1);
    CALL(ANI_ApplyShape, fx.gao);
    fx.actor->shape = fx.shape2;
    CALL(ANI_ApplyShape, fx.gao);
    fx.actor->shape = 0;
    CALL(ANI_ApplyShape, fx.gao);
    return 0.0;
}

static double c_changeShape(int side)
{
    build(0x1000 | 0x2 | 0x400000, 1);
    CALL(ANI_ChangeShape, fx.gao, fx.shape2);
    CALL(ANI_ChangeShape, fx.gao, 0);
    CALL(ANI_ChangeShape, fx.gao, fx.shape2);
    fx.actor->shape = 0;
    CALL(ANI_ChangeShape, fx.gao, fx.shape);
    return 0.0;
}

static double c_changeBoneShape(int side)
{
    int i;

    build(0x1000 | 0x2 | 0x400000, 1);
    for (i = 0; i < 4; i++)
        CALL(ANI_ChangeBoneShape, fx.gao, (uint8_t)i, (uint8_t)i, 0);
    CALL(ANI_ChangeBoneShape, fx.gao, 1, 0xFE, 0);
    CALL(ANI_ChangeBoneShape, fx.gao, 1, 0xFD, 0);
    CALL(ANI_ChangeBoneShape, fx.gao, 1, 0xFF, 0);
    CALL(ANI_ChangeBoneShape, fx.gao, 0, 1, 1);
    CALL(ANI_ChangeBoneShape, fx.gao, 9, 1, 0);
    return 0.0;
}

// The bone rebuild: the source's geometry, material, gizmo and modifiers
// replace the bone's own.
static double c_replaceBoneVisual(int side)
{
    tGameObjectVisualObject* boneVis;
    tGameObjectVisualObject* srcVis;
    tModifier* mdf0;
    tModifier* mdf1;
    int* colors;

    build(0x1000 | 0x2 | 0x400000, 1);
    fx.bone[0]->flags |= 0x800000;
    fx.bone[1]->flags |= 0x800000;
    fx.bone[2]->flags |= 0x800000;
    boneVis = (tGameObjectVisualObject*)fx.bone[0]->basic->actorObjectOrVisualObject;
    srcVis = (tGameObjectVisualObject*)fx.bone[1]->basic->actorObjectOrVisualObject;
    boneVis->GROMAT = ANIT_MakeGro(1);
    boneVis->GROGEOOrSomethingElse = ANIT_MakeGro(2);
    boneVis->RLIVertexColors = (int*)ANIT_Alloc(16);
    boneVis->RLIVertexColors[0] = 3;
    srcVis->GROMAT = ANIT_MakeGro(3);
    srcVis->GROGEOOrSomethingElse = ANIT_MakeGro(4);
    srcVis->objDrawMask = 0x1234;
    colors = (int*)ANIT_Alloc(16);
    colors[0] = 3;
    colors[1] = 0x11223344;
    srcVis->RLIVertexColors = colors;
    mdf0 = (tModifier*)ANIT_Alloc(sizeof(tModifier));
    mdf1 = (tModifier*)ANIT_Alloc(sizeof(tModifier));
    mdf0->nextElem = mdf1;
    fx.bone[0]->extended->mdf = mdf0;
    fx.bone[1]->extended->mdf = mdf1;
    CALL(ANI_ChangeBoneShape, fx.gao, 0, 1, 0);
    // The same with no visual object on the bone at all, which is the
    // one path that reaches the rebuild without comparing anything.
    fx.bone[2]->basic->actorObjectOrVisualObject = 0;
    CALL(ANI_ChangeBoneShape, fx.gao, 2, 1, 0);
    // And once more with the source carrying no gizmo.
    fx.bone[1]->basic->multiPartData->elemNum = 0;
    fx.bone[0]->flags2 |= 0x200000;
    CALL(ANI_ChangeBoneShape, fx.gao, 0, 1, 0);
    return 0.0;
}

static double c_setTrackList(int side)
{
    build(0x1000 | 0x2 | 0x400000, 1);
    CALL(ANI_SetTrackList, fx.anim[0], fx.list);
    CALL(ANI_SetTrackList, fx.anim[1], 0);
    CALL(ANI_SetTrackList, 0, fx.list);
    CALL(ANI_SetTrackList, fx.anim[2], fx.anim[3]->ptr->eventListStruct);
    return 0.0;
}

static double c_changeAction(int side)
{
    double acc = 0.0;
    int i;

    build(0x1000 | 0x2 | 0x4 | 0x400000, 1);
    for (i = 0; i < 10; i++)
    {
        CALL(ANI_ChangeAction, fx.gao, fx.actor);
        acc += fx.actor->actionItem * 31 + fx.actor->callCountForChangeAction;
    }
    fx.actor->actionItem = 2;
    fx.actor->transitionElem = (tTransitionElem*)ANIT_Alloc(sizeof(tTransitionElem));
    CALL(ANI_ChangeAction, fx.gao, fx.actor);
    CALL(ANI_ChangeAction, fx.gao, fx.actor);
    fx.actor->actionPtr = 0;
    CALL(ANI_ChangeAction, fx.gao, fx.actor);
    return acc;
}

static double c_endSpeed(int side)
{
    Vec3 speed;
    double acc = 0.0;
    int i;
    static Vec3 pos[16];

    build(0x1000 | 0x2 | 0x400000, 1);
    for (i = 0; i < 16; i++)
    {
        pos[i].x = (float)i * 1.5f;
        pos[i].y = (float)i * -2.25f;
        pos[i].z = 7.0f - (float)i;
        ANIT_keyPos[i] = (i == 5) ? 0 : &pos[i];
    }
    for (i = 0; i < 4; i++)
    {
        tAnim* a = fx.anim[i];
        CALL(ANI_GetEndSpeedOfMB, a, &speed);
        acc += speed.x + speed.y + speed.z;
    }
    // A track whose last key is not an interpolation key at all.
    fx.anim[0]->ptr->eventListStruct->listElems[0].trackElems[3].flags2 = 64;
    CALL(ANI_GetEndSpeedOfMB, fx.anim[0], &speed);
    acc += speed.x;
    // A track with no keys.
    fx.anim[1]->ptr->eventListStruct->listElems[0].elemNum = 0;
    CALL(ANI_GetEndSpeedOfMB, fx.anim[1], &speed);
    acc += speed.y;
    // A first track that is not a magic box track.
    fx.anim[2]->ptr->eventListStruct->listElems[0].boneID = 3;
    CALL(ANI_GetEndSpeedOfMB, fx.anim[2], &speed);
    acc += speed.z;
    return acc;
}

static double c_interpolateMB(int side)
{
    static Vec3 pos[16];
    int i;

    build(0x1000 | 0x2 | 0x400000, 1);
    for (i = 0; i < 16; i++)
    {
        pos[i].x = (float)i * 1.5f;
        pos[i].y = (float)i * -2.25f;
        pos[i].z = 7.0f - (float)i;
        ANIT_keyPos[i] = &pos[i];
    }
    fx.anim[0]->flags2 = 2;
    CALL(ANI_InterpolateMagicBoxPositionAfterEndOfAnim, fx.gao, fx.actor, fx.anim[0], 0.25f);
    CALL(ANI_InterpolateMagicBoxPositionAfterEndOfAnim, fx.gao, fx.actor, fx.anim[0], 0.0f);
    fx.anim[1]->flags2 = 4;
    CALL(ANI_InterpolateMagicBoxPositionAfterEndOfAnim, fx.gao, fx.actor, fx.anim[1], 0.5f);
    fx.anim[2]->flags2 = 0;
    CALL(ANI_InterpolateMagicBoxPositionAfterEndOfAnim, fx.gao, fx.actor, fx.anim[2], 0.5f);
    return 0.0;
}

static double c_interpolateMBFlat(int side)
{
    static Vec3 pos[16];
    int i;

    build(0x1000 | 0x2, 1);
    for (i = 0; i < 16; i++)
    {
        pos[i].x = (float)i; pos[i].y = 1.0f; pos[i].z = -(float)i;
        ANIT_keyPos[i] = &pos[i];
    }
    fx.anim[0]->flags2 = 2;
    CALL(ANI_InterpolateMagicBoxPositionAfterEndOfAnim, fx.gao, fx.actor, fx.anim[0], 0.25f);
    CALL(ANI_InterpolateMagicBoxPositionAfterEndOfAnim, fx.gao, fx.actor, fx.anim[0], 0.75f);
    return 0.0;
}

static double c_transitioning(int side)
{
    double acc = 0.0;

    build(0x1000 | 0x2 | 0x400000 | 0x4000000, 3);
    acc += CALL(ANI_GaoAnimationIsTransitioning, fx.gao);
    fx.actor->flagsEgBlendIw2 = 2;
    acc += CALL(ANI_GaoAnimationIsTransitioning, fx.gao);
    fx.actor->flagsEgBlendIw2 = 1;
    acc += CALL(ANI_GaoAnimationIsTransitioning, fx.gao);
    fx.gao->flags &= ~2;
    acc += CALL(ANI_GaoAnimationIsTransitioning, fx.gao);
    acc += CALL(ANI_GaoAnimationIsTransitioning, 0);
    return acc;
}

static double c_clearBlending(int side)
{
    build(0x1000 | 0x2 | 0x400000, 3);
    CALL(ANI_ClearBlendingFlag, fx.gao);
    CALL(ANI_ClearBlendingFlag, fx.other);
    CALL(ANI_ClearBlendingFlag, 0);
    fx.gao->flags &= ~2;
    CALL(ANI_ClearBlendingFlag, fx.gao);
    return fx.actor->flagsEgBlendIw2;
}

static double c_updateFlashTranslation(int side)
{
    Vec3 off;

    build(0x1000 | 0x2 | 0x400000 | 0x4000000, 3);
    off.x = 1.5f; off.y = -2.5f; off.z = 0.25f;
    CALL(ANI_UpdateFlashAfterMagicBoxTranslation, fx.gao, &off, 3);
    CALL(ANI_UpdateFlashAfterMagicBoxTranslation, fx.gao, &off, 1);
    CALL(ANI_UpdateFlashAfterMagicBoxTranslation, fx.gao, &off, 2);
    CALL(ANI_UpdateFlashAfterMagicBoxTranslation, fx.gao, &off, 0);
    fx.actor->flagsEgBlendIw2 = 1;
    CALL(ANI_UpdateFlashAfterMagicBoxTranslation, fx.gao, &off, 3);
    CALL(ANI_UpdateFlashAfterMagicBoxTranslation, 0, &off, 3);
    return 0.0;
}

static double c_hierarchyOnFlash(int side)
{
    build(0x1000 | 0x2 | 0x400000 | 0x4000000 | 0x10000, 3);
    CALL(ANI_ApplyHierarchyOnFlash, fx.gao);
    CALL(ANI_RemoveHierarchyOnFlash, fx.gao);
    fx.actor->flagsEgBlendIw2 = 1;
    CALL(ANI_ApplyHierarchyOnFlash, fx.gao);
    CALL(ANI_RemoveHierarchyOnFlash, fx.gao);
    fx.gao->flags &= ~0x4000000;
    CALL(ANI_ApplyHierarchyOnFlash, fx.gao);
    CALL(ANI_RemoveHierarchyOnFlash, fx.gao);
    fx.actor->animPtr[0] = 0;
    CALL(ANI_ApplyHierarchyOnFlash, fx.gao);
    CALL(ANI_RemoveHierarchyOnFlash, fx.gao);
    return 0.0;
}

static double c_recomputeFlash(int side)
{
    build(0x1000 | 0x2 | 0x400000 | 0x4000000, 3);
    CALL(ANI_RecomputeFlash, fx.gao, fx.anim[0]);
    CALL(ANI_RecomputeFlash, fx.gao, fx.anim[1]);
    CALL(ANI_RecomputeFlash, 0, fx.anim[1]);
    return 0.0;
}

static double c_recomputeFlashFlat(int side)
{
    build(0x1000 | 0x2 | 0x4000000 | 0x10000, 3);
    CALL(ANI_RecomputeFlash, fx.gao, fx.anim[0]);
    CALL(ANI_RecomputeFlash, fx.gao, fx.anim[1]);
    fx.anim[0]->ptr->eventListStruct = 0;
    CALL(ANI_RecomputeFlash, fx.gao, fx.anim[0]);
    return 0.0;
}

static double c_updateFlash(int side)
{
    build(0x1000 | 0x2 | 0x400000 | 0x4000000, 3);
    CALL(ANI_UpdateFlash, fx.gao, 3);
    CALL(ANI_UpdateFlash, fx.gao, 1);
    CALL(ANI_UpdateFlash, fx.gao, 2);
    CALL(ANI_UpdateFlash, fx.gao, 0);
    fx.actor->flagsEgBlendIw2 = 1;
    CALL(ANI_UpdateFlash, fx.gao, 3);
    fx.gao->flags &= ~0x4000000;
    CALL(ANI_UpdateFlash, fx.gao, 3);
    CALL(ANI_UpdateFlash, 0, 3);
    return 0.0;
}

static double c_getReference(int side)
{
    double acc = 0.0;

    build(0x1000 | 0x2 | 0x400000 | 1, 1);
    acc += ANIT_Ref(CALL(ANI_pst_GetReference, fx.gao));
    acc += ANIT_Ref(CALL(ANI_pst_GetReferenceInit, fx.gao));
    fx.other->flags |= 1;
    acc += ANIT_Ref(CALL(ANI_pst_GetReference, fx.gao));
    acc += ANIT_Ref(CALL(ANI_pst_GetReferenceInit, fx.gao));
    // A two-step chain. Never a cycle: neither copy detects one.
    fx.other->basic->mtx->refGao2 = fx.bone[0];
    fx.other->basic->mtx->refGao = fx.bone[0];
    fx.bone[0]->basic->mtx->refGao2 = 0;
    fx.bone[0]->basic->mtx->refGao = 0;
    acc += ANIT_Ref(CALL(ANI_pst_GetReference, fx.gao));
    acc += ANIT_Ref(CALL(ANI_pst_GetReferenceInit, fx.gao));
    fx.bone[0]->flags |= 1;
    acc += ANIT_Ref(CALL(ANI_pst_GetReference, fx.gao));
    acc += ANIT_Ref(CALL(ANI_pst_GetReferenceInit, fx.gao));
    fx.gao->flags &= ~1;
    acc += ANIT_Ref(CALL(ANI_pst_GetReference, fx.gao));
    acc += ANIT_Ref(CALL(ANI_pst_GetReferenceInit, fx.gao));
    return acc;
}

static double c_initSkeleton(int side)
{
    build(0x1000 | 0x2 | 0x400000, 1);
    CALL(ANI_InitSkeleton, fx.skeleton, fx.gao);
    CALL(ANI_InitSkeleton, fx.skeleton, fx.gao);
    CALL(ANI_InitSkeleton, fx.model, fx.other);
    return 0.0;
}

static double c_initSkeletonFresh(int side)
{
    int i;

    build(0x1000 | 0x2 | 0x400000, 1);
    for (i = 0; i < fx.boneNum; i++)
    {
        if (fx.bone[i] && fx.bone[i]->basic && fx.bone[i]->basic->mtx)
            fx.bone[i]->basic->mtx->refGao2 = 0;
        if (fx.bone[i])
            fx.bone[i]->flags |= 0x200000;
    }
    fx.bone[0]->flags &= ~0x400000;
    CALL(ANI_InitSkeleton, fx.skeleton, fx.gao);
    return 0.0;
}

static double c_attachSkeleton(int side)
{
    build(0x1000 | 0x2 | 0x400000, 1);
    CALL(ANI_AttachSkeletonModelToWorld, fx.skeleton, fx.world);
    CALL(ANI_AttachSkeletonModelToWorld, fx.model, fx.world);
    fx.bone[0]->flags &= ~0x800000;
    fx.bone[1]->flags |= 0x800000;
    CALL(ANI_AttachSkeletonModelToWorld, fx.skeleton, fx.world);
    return 0.0;
}

static double c_computeBoneMatrix(int side)
{
    Mtx4x4 dest;
    int i;

    g_invalidEntry = 0;
    build(0x1000 | 0x2 | 0x400000, 1);
    mkMtx(&dest);
    for (i = 0; i < fx.boneNum; i++)
    {
        if (!fx.bone[i])
            continue;
        if (fx.bone[i]->basic && fx.bone[i]->basic->mtx)
            fx.bone[i]->basic->mtx->refGao2 = fx.gao;
        CALL(ANI_ComputeBoneMatrix, fx.bone[i], fx.gao, &dest, 1);
        CALL(ANI_ComputeBoneMatrix, fx.bone[i], fx.gao, &dest, 0);
    }
    // Second branch: the bone's reference is another bone.
    fx.bone[0]->basic->mtx->refGao2 = fx.bone[1];
    ((tableElemStructPF*)fx.skeleton->tablePF->firstElemPtr)[1].objectPtr = fx.bone[1];
    CALL(ANI_ComputeBoneMatrix, fx.bone[0], fx.gao, &dest, 1);
    // Third branch: no reference at all.
    fx.bone[0]->basic->mtx->refGao2 = 0;
    fx.bone[0]->flags &= ~0x400000;
    CALL(ANI_ComputeBoneMatrix, fx.bone[0], fx.gao, &dest, 1);
    return dest.Pos.x + dest.v1.y + dest.mtxflag;
}

static double c_playBlendWithoutMB(int side)
{
    build(0x1000 | 0x2 | 0x400000, 3);
    CALL(ANI_PlayBlendAnimationWithoutMagicBox, fx.gao, fx.actor, fx.anim[1]);
    CALL(ANI_PlayBlendAnimationWithoutMagicBox, fx.gao, fx.actor, fx.anim[2]);
    fx.anim[1]->transitionFlags = 4;
    TIM_gf_dt = 0.5f;
    CALL(ANI_PlayBlendAnimationWithoutMagicBox, fx.gao, fx.actor, fx.anim[1]);
    fx.actor->skeletonGroup = 0;
    TIM_gf_dt = 0.75f;
    CALL(ANI_PlayBlendAnimationWithoutMagicBox, fx.gao, fx.actor, fx.anim[1]);
    return 0.0;
}

static double c_computeMBForBlend(int side)
{
    static Vec3 pos[16];
    int i;

    build(0x1000 | 0x2 | 0x400000, 3);
    for (i = 0; i < 16; i++)
    {
        pos[i].x = (float)i; pos[i].y = 2.0f; pos[i].z = -(float)i;
        ANIT_keyPos[i] = &pos[i];
    }
    CALL(ANI_b_ComputeMagicBoxPositionForBlendAnim, fx.gao, fx.actor, fx.anim[1]);
    CALL(ANI_b_ComputeMagicBoxPositionForBlendAnim, fx.gao, fx.actor, fx.anim[2]);
    fx.anim[3]->actionElemForTransition = 0xFF;
    CALL(ANI_b_ComputeMagicBoxPositionForBlendAnim, fx.gao, fx.actor, fx.anim[3]);
    TIM_gf_dt = 0.0f;
    CALL(ANI_b_ComputeMagicBoxPositionForBlendAnim, fx.gao, fx.actor, fx.anim[1]);
    return 0.0;
}

static double c_blendMB(int side)
{
    static Vec3 pos[16];
    int i;

    build(0x1000 | 0x2 | 0x400000, 3);
    for (i = 0; i < 16; i++)
    {
        pos[i].x = 1.0f + (float)i; pos[i].y = 2.0f; pos[i].z = -(float)i;
        ANIT_keyPos[i] = &pos[i];
    }
    CALL(ANI_b_BlendMagicBox, fx.gao, fx.actor, fx.anim[1]);
    fx.anim[1]->transitionFlags |= 8;
    CALL(ANI_b_BlendMagicBox, fx.gao, fx.actor, fx.anim[1]);
    fx.anim[0]->flags |= 2;
    CALL(ANI_b_BlendMagicBox, fx.gao, fx.actor, fx.anim[0]);
    return 0.0;
}

static double c_playLead(int side)
{
    build(0x1000 | 0x2 | 0x400000 | 0x4000000, 3);
    CALL(ANI_PlayLeadAnimation, fx.gao, fx.actor, fx.anim[0]);
    fx.actor->eventRunningFrequency = 3;
    CALL(ANI_PlayLeadAnimation, fx.gao, fx.actor, fx.anim[0]);
    fx.actor->eventRunningFrequency = 0xFF;
    fx.actor->currTime = 0;
    CALL(ANI_PlayLeadAnimation, fx.gao, fx.actor, fx.anim[0]);
    fx.anim[0]->flags |= 0x80;
    CALL(ANI_PlayLeadAnimation, fx.gao, fx.actor, fx.anim[0]);
    return 0.0;
}

static double c_playGao(int side)
{
    Vec3 speed;

    build(0x1000 | 0x2 | 0x4 | 0x400000 | 0x4000000, 3);
    speed.x = 2.0f; speed.y = 0.5f; speed.z = -1.5f;
    CALL(ANI_PlayGameObjectAnimation, fx.gao, fx.actor, 0.25f, &speed);
    CALL(ANI_PlayGameObjectAnimation, fx.gao, fx.actor, 0.0f, &speed);
    CALL(ANI_PlayGameObjectAnimation, fx.gao, fx.actor, 0.5f, 0);
    return speed.x + speed.y + speed.z;
}

static double c_playGaoModes(int side)
{
    Vec3 speed;

    build(0x1000 | 0x2 | 0x4 | 0x400000, 1);
    speed.x = 1.0f; speed.y = -2.0f; speed.z = 3.0f;
    fx.anim[0]->flags = 6;
    CALL(ANI_PlayGameObjectAnimation, fx.gao, fx.actor, 0.25f, &speed);
    fx.anim[0]->flags = 0;
    CALL(ANI_PlayGameObjectAnimation, fx.gao, fx.actor, 0.25f, &speed);
    fx.anim[0]->flags = 0x80;
    CALL(ANI_PlayGameObjectAnimation, fx.gao, fx.actor, 0.25f, &speed);
    fx.actor->animPtr[0] = 0;
    CALL(ANI_PlayGameObjectAnimation, fx.gao, fx.actor, 0.25f, &speed);
    return speed.x + speed.y + speed.z;
}

static double c_playGaoBlendEnd(int side)
{
    Vec3 speed;

    build(0x1000 | 0x2 | 0x4, 3);
    speed.x = 0.5f; speed.y = 0.25f; speed.z = 0.125f;
    // A blend whose elapsed time has run past its length.
    fx.anim[1]->startTime = 60000;
    fx.anim[1]->actionLength = 1;
    CALL(ANI_PlayGameObjectAnimation, fx.gao, fx.actor, 0.25f, &speed);
    fx.actor->flagsEgBlendIw2 = 3;
    fx.anim[1]->startTime = 16;
    fx.anim[1]->actionLength = 200;
    fx.anim[1]->flags2 = 1;
    CALL(ANI_PlayGameObjectAnimation, fx.gao, fx.actor, 0.25f, &speed);
    return speed.x + speed.y + speed.z;
}

// The blend time is held in a double and the comparison that ends the
// blend is on that double, not on the float it is narrowed into. These
// three values straddle exactly that: the double sits below the blend's
// length and the float does not.
static double c_playGaoNarrowing(int side)
{
    unsigned int pattern = 0x3CFE6668u;
    float dtf;

    build(0x1000 | 0x2, 3);
    memcpy(&dtf, &pattern, sizeof(dtf));
    fx.anim[0]->flags = 0x80;
    fx.actor->eventRunningFrequency = 0;
    fx.anim[1]->flags = 0;
    fx.anim[1]->flags2 = 1;
    fx.anim[1]->actionLength = 2;
    fx.anim[1]->startTime = 291;
    *(float*)&fx.anim[1]->ptr->tracks[1].privateData = dtf;
    CALL(ANI_PlayGameObjectAnimation, fx.gao, fx.actor, 0.25f, 0);
    return (double)fx.actor->flagsEgBlendIw2 * 1000.0 + fx.anim[1]->startTime;
}

static double c_oneCall(int side)
{
    build(0x1000 | 0x2 | 0x4 | 0x400000, 3);
    CALL(ANI_OneCall, fx.gao);
    CALL(ANI_OneCall, fx.gao);
    fx.gao->flags &= ~4;
    fx.actor->initedInANIMainCall = 0;
    CALL(ANI_OneCall, fx.gao);
    return 0.0;
}

static double c_mainCall(int side)
{
    build(0x1000 | 0x2 | 0x4 | 0x400000, 3);
    CALL(ANI_MainCall, fx.world);
    CALL(ANI_MainCall, fx.world);
    fx.gao->extended->extraFlag = 1;
    fx.gao->flags2 = 2;
    fx.actor->initedInANIMainCall = 0;
    CALL(ANI_MainCall, fx.world);
    return 0.0;
}

static double c_beforeRendering(int side)
{
    build(0x1000 | 0x2 | 0x400000, 3);
    CALL(ANI_BeforeRendering, fx.world);
    return 0.0;
}

static double c_beforeRender(int side)
{
    build(0x1000 | 0x2 | 0x400000, 3);
    CALL(ANI_BeforeRender, fx.gao);
    return 0.0;
}

static double c_beforeRenderMask(int side)
{
    build(0x1000 | 0x2 | 0x400000, 0xF);
    CALL(ANI_BeforeRender, fx.gao);
    fx.actor->flagsEgBlendIw2 = 0xA;
    CALL(ANI_BeforeRender, fx.gao);
    fx.actor->flagsEgBlendIw2 = 4;
    CALL(ANI_BeforeRender, fx.gao);
    fx.actor->flagsEgBlendIw2 = 0;
    CALL(ANI_BeforeRender, fx.gao);
    return 0.0;
}

static double c_beforeRenderShort(int side)
{
    build(0x1000 | 0x2 | 0x400000, 3);
    // Fewer gizmo matrices than bones, so the inner loop stops early.
    fx.gao->basic->multiPartData->elemNum = 2;
    CALL(ANI_BeforeRender, fx.gao);
    fx.gao->basic->multiPartData->elemNum = 0;
    CALL(ANI_BeforeRender, fx.gao);
    fx.gao->basic->multiPartData->elemNum = 6;
    fx.anim[1]->transitionFlags |= 0x80;
    CALL(ANI_BeforeRender, fx.gao);
    fx.actor->skeletonGroup = 0;
    CALL(ANI_BeforeRender, fx.gao);
    return 0.0;
}

static double c_render(int side)
{
    g_invalidEntry = 0;
    build(0x1000 | 0x2 | 0x400000, 3);
    CALL(ANI_Render, fx.gao);
    CALL(ANI_Render, fx.gao);
    return (double)PushedStack;
}

static double c_renderNoLead(int side)
{
    g_invalidEntry = 0;
    build(0x1000 | 0x2 | 0x400000, 3);
    fx.actor->animPtr[0] = 0;
    fx.actor->initedInANIMainCall = 1;
    CALL(ANI_Render, fx.gao);
    fx.actor->skeletonGroup = 0;
    CALL(ANI_Render, fx.gao);
    return (double)PushedStack;
}

static double c_renderMulti(int side)
{
    g_invalidEntry = 0;
    build(0x1000 | 0x2 | 0x400000, 3);
    fx.bone[0]->basic->multiPartData = fx.gao->basic->multiPartData;
    fx.bone[1]->basic->multiPartData = 0;
    CALL(ANI_Render, fx.gao);
    return (double)PushedStack;
}

static double c_reinit(int side)
{
    build(0x1000 | 0x2 | 0x400000, 0xF);
    CALL(ANI_Reinit, fx.gao);
    CALL(ANI_ReinitAnimation, fx.anim[0]);
    CALL(ANI_ReinitAnimation, 0);
    fx.actor->shape2 = 0;
    fx.actor->skeletonGroupToRestoreOnReinit = fx.model;
    CALL(ANI_Reinit, fx.gao);
    return 0.0;
}

static double c_skeletonSwap(int side)
{
    build(0x1000 | 0x2 | 0x400000, 3);
    fx.otherActor->skeletonGroup = fx.model;
    CALL(ANI_ChangeDefaultSkeleton, fx.gao, fx.other);
    CALL(ANI_RestoreDefaultSkeleton, fx.gao, fx.other);
    CALL(ANI_ChangeDefaultSkeleton, fx.gao, 0);
    CALL(ANI_RestoreDefaultSkeleton, fx.gao, fx.other);
    fx.otherActor->skeletonGroup = 0;
    CALL(ANI_ChangeDefaultSkeleton, fx.gao, fx.other);
    return 0.0;
}

static double c_duplicate(int side)
{
    build(0x1000 | 0x2 | 0x400000, 3);
    CALL(ANI_pst_DuplicateGameObjectAnim, fx.world, fx.other, fx.gao);
    fx.actor->flagsEgBlendIw2 = 2;
    fx.actor->group = 0;
    fx.actor->shape = 0;
    CALL(ANI_pst_DuplicateGameObjectAnim, fx.world, fx.other, fx.gao);
    return 0.0;
}

static double c_cloneSet(int side)
{
    build(0x1000 | 0x2 | 0x400000, 3);
    CALL(ANI_CloneSet, fx.other, fx.gao, 0);
    CALL(ANI_CloneSet, fx.other, fx.gao, -1);
    fx.other->flags &= ~2;
    CALL(ANI_CloneSet, fx.other, fx.gao, 1);
    fx.actor->actionKit = 0;
    CALL(ANI_CloneSet, fx.other, fx.gao, 1);
    return 0.0;
}

static double c_load(int side)
{
    static char data[64];
    int i;

    build(0x1000 | 0x2 | 0x400000, 3);
    for (i = 0; i < 64; i++)
        data[i] = (char)(i * 3);
    data[0] = 4;
    ANIT_fileData = data;
    ANIT_fileSize = 64;
    ANIT_loadedList = fx.list;
    ANIT_loadedGroup = fx.model;
    fx.actor->actionKit = 0;
    CALL(ANI_pst_Load, 10, 20, 30, fx.gao, fx.world);
    CALL(ANI_pst_Load, -1, -1, -1, fx.gao, fx.world);
    CALL(ANI_ResolveRefs, fx.gao);
    return 0.0;
}

static double c_destroy(int side)
{
    tGameObjectActorObject* a;
    tAnim* anim;

    build(0x1000 | 0x2 | 0x400000, 3);
    a = CALL0(ANI_pst_CreateGameObjectAnim);
    anim = CALL0(ANI_AllocAnim);
    a->animPtr[0] = anim;
    a->animPtr[1] = (tAnim*)-1;
    a->skeletonGroup = fx.skeleton;
    a->group = fx.model;
    a->shape2 = fx.shape2;
    a->shape2->usageCount = 1;
    a->actionKit = fx.kit;
    CALL(ANI_Free, a);
    fx.shape->usageCount = 1;
    CALL(ANI_DestroyShape, &fx.shape);
    fx.shape2->usageCount = 3;
    CALL(ANI_DestroyShape, &fx.shape2);
    CALL(ANI_DestroyAnimation, fx.anim[3]);
    CALL(ANI_DestroyAnimation, 0);
    CALL(ANI_DestroySkeleton, fx.model);
    CALL(ANI_DestroySkeleton, fx.model);
    return (double)a->aniBitFieldLOD;
}

static struct { const char* name; CaseFn fn; } cases[] = {
    { "LimitFrequency", c_limitFrequency },
    { "ShapeCallback", c_shapeCallback },
    { "Canal", c_canal },
    { "ApplyShape", c_applyShape },
    { "ChangeShape", c_changeShape },
    { "ChangeBoneShape", c_changeBoneShape },
    { "ReplaceBoneVisual", c_replaceBoneVisual },
    { "SetTrackList", c_setTrackList },
    { "ChangeAction", c_changeAction },
    { "GetEndSpeedOfMB", c_endSpeed },
    { "InterpolateMB", c_interpolateMB },
    { "InterpolateMBFlat", c_interpolateMBFlat },
    { "IsTransitioning", c_transitioning },
    { "ClearBlending", c_clearBlending },
    { "FlashTranslation", c_updateFlashTranslation },
    { "HierarchyOnFlash", c_hierarchyOnFlash },
    { "RecomputeFlash", c_recomputeFlash },
    { "RecomputeFlashFlat", c_recomputeFlashFlat },
    { "UpdateFlash", c_updateFlash },
    { "GetReference", c_getReference },
    { "InitSkeleton", c_initSkeleton },
    { "InitSkeletonFresh", c_initSkeletonFresh },
    { "AttachSkeleton", c_attachSkeleton },
    { "ComputeBoneMatrix", c_computeBoneMatrix },
    { "PlayBlendNoMB", c_playBlendWithoutMB },
    { "ComputeMBForBlend", c_computeMBForBlend },
    { "BlendMB", c_blendMB },
    { "PlayLead", c_playLead },
    { "PlayGao", c_playGao },
    { "PlayGaoModes", c_playGaoModes },
    { "PlayGaoBlendEnd", c_playGaoBlendEnd },
    { "PlayGaoNarrowing", c_playGaoNarrowing },
    { "OneCall", c_oneCall },
    { "MainCall", c_mainCall },
    { "BeforeRendering", c_beforeRendering },
    { "BeforeRender", c_beforeRender },
    { "BeforeRenderMask", c_beforeRenderMask },
    { "BeforeRenderShort", c_beforeRenderShort },
    { "Render", c_render },
    { "RenderNoLead", c_renderNoLead },
    { "RenderMulti", c_renderMulti },
    { "Reinit", c_reinit },
    { "SkeletonSwap", c_skeletonSwap },
    { "Duplicate", c_duplicate },
    { "CloneSet", c_cloneSet },
    { "Load", c_load },
    { "Destroy", c_destroy },
};

int main(void)
{
    size_t i;

    setvbuf(stdout, NULL, _IONBF, 0);
    MATH_InitModule();
    if (BASE_ANI_c_DefaultBitFieldLOD != NEW_ANI_c_DefaultBitFieldLOD)
    {
        printf("ANI_c_DefaultBitFieldLOD differs\n");
        g_fails++;
    }
    for (i = 0; i < sizeof(cases) / sizeof(cases[0]); i++)
        runCase(cases[i].name, cases[i].fn);

    printf("animation: %ld cases, %ld values compared, %ld matrix pad words skipped,\n  %ld differing, "
        "worst abs %.3g worst rel %.3g (%s)\n",
        g_cases, g_compared, g_skipped, g_diffCount, g_worstAbs, g_worstRel, g_worstCase);
    if (g_fails)
    {
        printf("animation: %d case(s) differ\n", g_fails);
        return 1;
    }
    printf("animation: equivalent\n");
    return 0;
}
