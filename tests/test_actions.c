/* Differential test for actions.c: the frozen pre-rewrite copy against
   the rewritten one, over identical state.

   Both copies run over one scene at one address, with the arena
   allocator rewound between them, so every pointer either copy stores
   can be compared directly rather than normalised. A case is equivalent
   when the scene bytes, the used prefix of the arena, the driver's own
   scratch block, the eight engine globals ACT_SetAction saves and
   restores, and the ordered sequence of mocked engine calls all match.

   What it is for. The rewrite replaced six gotos, five of which are in
   ACT_SetAction and form one shared tail reached from seven places; it
   folded the two parallel cursors of the action loader into one, turned
   a three-statement read-modify-write of the animation flags into one
   mask and an XOR-swap into the assignment it is, and rewrote two bit
   tests the decompiler had spelled as a complement and a shift. None of
   those is settled by reading, and the transition resolution in
   particular has four ways of reaching the plain-action path and one of
   reaching the blend.

   What it does not cover. The real loader queues a file reference and
   fills it in later; here LOA_MakeFileRef resolves at once to a pointer
   derived from the key, so what is compared is the key, the
   destination, which callback was named and the flags -- not what the
   loader would eventually write. Everything else actions.c touches is
   driven.

   One thing the harness cannot reproduce is the engine's word size.
   ACT_ul_ActionCallback sizes an action as 20 bytes per element, which
   is sizeof(tActionElem) in the 32-bit build the engine ships as and
   not in this one, so the elements it then writes run past the block.
   Both copies do exactly the same thing with it and the arena pads
   every block, so the comparison still sees any divergence; what it
   cannot see is a change that only shows at the real element size. The
   test is linked -no-pie for a related reason: a transition is stored
   through BAS as an int and read back as a pointer, which only round
   trips while the data sits below 2GB. */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "../common.h"
#include "../math.h"
#include "../basic_array.h"
#include "../events.h"
#include "../time.h"
#include "../engine.h"
#include "../gameobject.h"
#include "../animation.h"
#include "actions_mocks.h"

extern int LOA_gb_SpeedMode;
extern int BIG_gi_ReadMode;

/* ------------------------------------------- the two copies' exports */

#define DECL2(ret, name, args) ret BASE_##name args; ret NEW_##name args;

DECL2(void, ACT_FreeAction, (tAction**))
DECL2(void, ACT_ActionRestoreInit, (struct sActionKit*))
DECL2(void, ACT_FreeActionKit, (tActionKit**))
DECL2(struct sTransitionElem*, ACT_pst_FindTransition, (tActionKit*, uint16_t, uint8_t, uint16_t))
DECL2(void, ACT_SetAction, (struct sGameObjectMain*, int, int))
DECL2(void, ACT_SetUsageCounts, (struct sActionKit*))
DECL2(struct sAction*, ACT_ul_ActionCallback, (uint32_t))
DECL2(struct sActionKit*, ACT_ul_ActionKitCallback, (uint32_t))
DECL2(void, ACT_UseAction, (tAction*))
DECL2(void, ACT_ActionSaveInit, (tActionKit*))
DECL2(void, ACTKitMerge, (struct sGameObjectMain*, struct sGameObjectMain*, int, int, BOOL))
DECL2(void, ACTKitMergeNoOver, (struct sGameObjectMain*, struct sGameObjectMain*, int, int))
DECL2(void, ACTKitMergeOver, (struct sGameObjectMain*, struct sGameObjectMain*, int, int))

typedef struct sActSide
{
    const char* name;
    void (*FreeAction)(tAction**);
    void (*ActionRestoreInit)(struct sActionKit*);
    void (*FreeActionKit)(tActionKit**);
    struct sTransitionElem* (*FindTransition)(tActionKit*, uint16_t, uint8_t, uint16_t);
    void (*SetAction)(struct sGameObjectMain*, int, int);
    void (*SetUsageCounts)(struct sActionKit*);
    struct sAction* (*ActionCallback)(uint32_t);
    struct sActionKit* (*ActionKitCallback)(uint32_t);
    void (*UseAction)(tAction*);
    void (*ActionSaveInit)(tActionKit*);
    void (*KitMerge)(struct sGameObjectMain*, struct sGameObjectMain*, int, int, BOOL);
    void (*KitMergeNoOver)(struct sGameObjectMain*, struct sGameObjectMain*, int, int);
    void (*KitMergeOver)(struct sGameObjectMain*, struct sGameObjectMain*, int, int);
} tActSide;

#define SIDE(P) { #P, \
    P##_ACT_FreeAction, P##_ACT_ActionRestoreInit, P##_ACT_FreeActionKit, \
    P##_ACT_pst_FindTransition, P##_ACT_SetAction, P##_ACT_SetUsageCounts, \
    P##_ACT_ul_ActionCallback, P##_ACT_ul_ActionKitCallback, P##_ACT_UseAction, \
    P##_ACT_ActionSaveInit, P##_ACTKitMerge, P##_ACTKitMergeNoOver, P##_ACTKitMergeOver }

static const tActSide sides[2] = { SIDE(BASE), SIDE(NEW) };

typedef void (*tCaseFn)(const tActSide* s, int arg);

static void RunCase(const char* name, tCaseFn fn, int arg)
{
    int side;

    ACTMOCK_SetCase(name);
    for (side = 0; side < 2; side++)
    {
        ACTMOCK_BuildScene(0x4321u + (unsigned int)arg * 2654u, arg);
        ACTMOCK_BuildFiles(0x77u + (unsigned int)arg, arg);
        ACTMOCK_Reset();
        /* the loader callback whose address differs between the two
           copies, so the mock can log a name rather than a pointer */
        ACTMOCK_RegisterCallback((void*)sides[side].ActionCallback, 3);
        ACTMOCK_RegisterCallback((void*)EVE_ul_CallbackLoadListTracks, 1);
        ACTMOCK_RegisterCallback((void*)ANI_ul_ShapeCallback, 2);
        fn(&sides[side], arg);
        if (side == 0)
            ACTMOCK_Save();
        else
            ACTMOCK_CompareWithSaved(name);
    }
}

/* ---- ACT_SetAction ---------------------------------------------------- */

/* The whole of ACT_SetAction over one scene: the argument picks the
   starting action, whether the call is forced, whether a transition is
   already running and which of the engine's two "running" flags is on. */
static void Case_SetAction(const tActSide* s, int arg)
{
    int i;

    /* bits 13 and above are the case's own knobs; the scene pattern
       only reaches bit 9, so the two do not collide */
    ENG_gb_EVERunning = (arg >> 14) & 1;
    ENG_gb_AIRunning = (arg >> 15) & 1;
    ACTMOCK_scene.actor.currTime = (int16_t)((arg >> 16) & 3);

    for (i = 0; i < ACTMOCK_ACTIONS + 2; i++)
    {
        s->SetAction(&ACTMOCK_scene.gao, i, (arg >> 13) & 1);
        ACTMOCK_Push(ACTMOCK_Tag(ACTMOCK_scene.actor.transitionElem));
        ACTMOCK_Push(ACTMOCK_scene.actor.currentAction);
        ACTMOCK_Push(ACTMOCK_scene.actor.currentActionTransition);
        ACTMOCK_Push(ACTMOCK_scene.actor.flagsEgBlendIw2);
    }
}

/* The same action twice in a row, which is the early return at the top,
   and the negative form of the id, which is the 0x7FFFFFFF mask. */
static void Case_SetActionRepeat(const tActSide* s, int arg)
{
    int id = arg % ACTMOCK_ACTIONS;

    ENG_gb_EVERunning = 0;
    ENG_gb_AIRunning = (arg >> 1) & 1;
    ACTMOCK_scene.actor.currTime = (int16_t)(arg % 3);

    s->SetAction(&ACTMOCK_scene.gao, id, 0);
    s->SetAction(&ACTMOCK_scene.gao, id, 0);
    s->SetAction(&ACTMOCK_scene.gao, id, 1);
    s->SetAction(&ACTMOCK_scene.gao, id | 0x80000000, 0);
    s->SetAction(&ACTMOCK_scene.gao, -id - 1, 0);
    s->SetAction(&ACTMOCK_scene.gao, -1, 1);
    s->SetAction(&ACTMOCK_scene.gao, 1000, 0);
    s->SetAction(&ACTMOCK_scene.gao, (int)(0x80010000u | (unsigned)id), 0);
    s->SetAction(&ACTMOCK_scene.gao, (int)(0x80010000u | (unsigned)id), 1);
    ACTMOCK_Push(ACTMOCK_scene.actor.currentAction);
    ACTMOCK_Push(ACTMOCK_scene.actor.currentActionTransition);
}

/* A transition already in progress when the next action is asked for,
   which is the arm that reads prevTransition's 0x40. */
static void Case_SetActionDuringTransition(const tActSide* s, int arg)
{
    int i;
    int j;

    ENG_gb_EVERunning = 0;
    ENG_gb_AIRunning = 0;

    for (i = 0; i < ACTMOCK_ACTIONS; i++)
    {
        ACTMOCK_scene.actor.currentAction = (uint16_t)((i + arg) % ACTMOCK_ACTIONS);
        ACTMOCK_scene.actor.currentActionTransition =
            (uint16_t)((arg >> 1) & 1 ? 0xFFFF : (i + 1) % ACTMOCK_ACTIONS);
        ACTMOCK_scene.actor.actionItem = (uint8_t)(i % ACTMOCK_ELEMS);
        ACTMOCK_scene.actor.transitionElem =
            &ACTMOCK_scene.transElems[i % ACTMOCK_ACTIONS][0][(i + arg) % ACTMOCK_TRANS];
        ACTMOCK_scene.actor.animPtr[0] = &ACTMOCK_scene.anims[i & 1];
        ACTMOCK_scene.actor.animPtr[1] = ((arg >> 2) & 1) ? NULL : &ACTMOCK_scene.anims[2];
        ACTMOCK_scene.actor.flagsEgBlendIw2 = (uint8_t)((arg >> 3) & 3);

        for (j = 0; j < ACTMOCK_ACTIONS; j++)
        {
            s->SetAction(&ACTMOCK_scene.gao, j, (arg >> 4) & 1);
            ACTMOCK_Push(ACTMOCK_Tag(ACTMOCK_scene.actor.transitionElem));
        }
    }
}

/* The arm that reads the running transition's own flags. It is reached
   only when a transition is in progress, the action it was heading for
   is 0xFFFF, and the lookup from the current action finds nothing --
   so the current action and item are chosen to be a pair that carries
   no transition at all. */
static void Case_SetActionNoFromTransition(const tActSide* s, int arg)
{
    int i;
    int j;

    ENG_gb_EVERunning = 0;
    ENG_gb_AIRunning = 0;

    for (i = 0; i < ACTMOCK_ACTIONS; i++)
    {
        int item = (2 - i % 3 + 3) % 3;          /* (i + item) % 3 == 2 */

        if (item >= ACTMOCK_ELEMS)
            continue;
        ACTMOCK_scene.actor.currentAction = (uint16_t)i;
        ACTMOCK_scene.actor.actionItem = (uint8_t)item;
        ACTMOCK_scene.actor.currentActionTransition = 0xFFFF;
        ACTMOCK_scene.actor.animPtr[0] = &ACTMOCK_scene.anims[(i + arg) % 3];
        ACTMOCK_scene.actor.animPtr[1] = &ACTMOCK_scene.anims[2];
        ACTMOCK_scene.actor.flagsEgBlendIw2 = 0;

        for (j = 0; j < ACTMOCK_ACTIONS; j++)
        {
            ACTMOCK_scene.actor.transitionElem =
                &ACTMOCK_scene.transElems[j][0][(j + arg) % ACTMOCK_TRANS];
            ACTMOCK_scene.actor.currentAction = (uint16_t)i;
            ACTMOCK_scene.actor.actionItem = (uint8_t)item;
            ACTMOCK_scene.actor.currentActionTransition = 0xFFFF;
            s->SetAction(&ACTMOCK_scene.gao, j, (arg >> 1) & 1);
            ACTMOCK_Push(ACTMOCK_Tag(ACTMOCK_scene.actor.transitionElem));
            ACTMOCK_Push(ACTMOCK_scene.actor.currentAction);
        }
    }
}

/* The blend's own flag test, driven on purpose. It is reached only when
   a transition is found whose length is not zero, and it reads the
   track the outgoing animation was on: the pair below puts that track
   on its last key at duration zero, which is the one arrangement where
   the test's second half can be true at all. */
static void Case_SetActionBlend(const tActSide* s, int arg)
{
    int anim;
    int from;

    ENG_gb_EVERunning = 0;
    ENG_gb_AIRunning = 0;

    for (anim = 0; anim < 4; anim++)
        for (from = 0; from < ACTMOCK_ACTIONS; from++)
        {
            int other = (anim + 1) & 3;

            /* the blend swaps the two animations' event data, so both
               have to be put back before the next one */
            ACTMOCK_scene.anims[anim].ptr = &ACTMOCK_scene.animData[anim];
            ACTMOCK_scene.anims[other].ptr = &ACTMOCK_scene.animData[other];
            ACTMOCK_scene.animData[anim].eventListStruct = &ACTMOCK_scene.eventLists[anim];
            ACTMOCK_scene.animData[anim].tracks = &ACTMOCK_scene.animTracks[anim];
            ACTMOCK_scene.animData[other].eventListStruct = &ACTMOCK_scene.eventLists[other];
            ACTMOCK_scene.animData[other].tracks = &ACTMOCK_scene.animTracks[other];

            ACTMOCK_scene.actor.animPtr[0] = &ACTMOCK_scene.anims[anim];
            ACTMOCK_scene.actor.animPtr[1] = &ACTMOCK_scene.anims[other];
            ACTMOCK_scene.actor.currentAction = (uint16_t)from;
            ACTMOCK_scene.actor.actionItem = 0;
            ACTMOCK_scene.actor.currentActionTransition = 0xFFFF;
            ACTMOCK_scene.actor.transitionElem = NULL;
            ACTMOCK_scene.actor.flagsEgBlendIw2 = 0;

            /* key 2 is the one whose transition length is not zero */
            s->SetAction(&ACTMOCK_scene.gao, 2 + 4 * ((arg >> 1) & 1), arg & 1);
            ACTMOCK_Push(ACTMOCK_scene.anims[other].transitionFlags);
            ACTMOCK_Push(ACTMOCK_scene.anims[other].flags2);
            ACTMOCK_Push(ACTMOCK_scene.anims[other].actionLength);
        }
}

/* ---- ACT_pst_FindTransition ------------------------------------------- */

static void Case_FindTransition(const tActSide* s, int arg)
{
    int a;
    int e;
    int id;

    for (a = 0; a < ACTMOCK_ACTIONS; a++)
        for (e = 0; e < ACTMOCK_ELEMS; e++)
            for (id = 0; id < 2 * ACTMOCK_TRANS + 1; id++)
                ACTMOCK_Push(ACTMOCK_Tag(s->FindTransition(&ACTMOCK_scene.kit.kit,
                                                           (uint16_t)a, (uint8_t)e,
                                                           (uint16_t)id)));
    for (id = 0; id < 2 * ACTMOCK_TRANS + 1; id++)
    {
        ACTMOCK_Push(ACTMOCK_Tag(s->FindTransition(&ACTMOCK_scene.kit.kit, 0xFFFF,
                                                   0, (uint16_t)id)));
        ACTMOCK_Push(ACTMOCK_Tag(s->FindTransition(&ACTMOCK_scene.kit.kit, 0xFFFE,
                                                   0, (uint16_t)id)));
    }
    (void)arg;
}

/* ---- the reference counts and the free paths -------------------------- */

static void Case_UsageCounts(const tActSide* s, int arg)
{
    int i;

    s->SetUsageCounts(&ACTMOCK_scene.kit.kit);
    s->SetUsageCounts(&ACTMOCK_scene.kit.kit);
    for (i = 0; i < ACTMOCK_ACTIONS; i++)
        if (ACTMOCK_scene.kit.actions[i])
            s->UseAction(ACTMOCK_scene.kit.actions[i]);
    (void)arg;
}

static void Case_FreeAction(const tActSide* s, int arg)
{
    int i;

    /* the usage count decides whether the transitions are freed too */
    for (i = 0; i < ACTMOCK_ACTIONS; i++)
        if (ACTMOCK_scene.kit.actions[i])
            ACTMOCK_scene.kit.actions[i]->usageCount = (int16_t)(1 + ((i + arg) % 3));

    for (i = 0; i < ACTMOCK_ACTIONS; i++)
        if (ACTMOCK_scene.kit.actions[i])
        {
            s->FreeAction(&ACTMOCK_scene.kit.actions[i]);
            ACTMOCK_Push(ACTMOCK_Tag(ACTMOCK_scene.kit.actions[i]));
        }
}

static void Case_FreeActionKit(const tActSide* s, int arg)
{
    tActionKit* kit = &ACTMOCK_scene.kit.kit;

    kit->usageCount = (int16_t)(1 + (arg % 3));
    if ((arg >> 2) & 1)
        s->ActionSaveInit(kit);
    if ((arg >> 3) & 1)
        kit->elemNum = 0;
    s->FreeActionKit(&kit);
    ACTMOCK_Push(ACTMOCK_Tag(kit));
}

static void Case_SaveRestore(const tActSide* s, int arg)
{
    tActionKit* kit = &ACTMOCK_scene.kit.kit;
    int i;

    s->ActionSaveInit(kit);
    s->ActionSaveInit(kit);          /* the second call must do nothing */

    /* move some actions about, so restore has both arms to take */
    for (i = 0; i < ACTMOCK_ACTIONS; i++)
        if ((i + arg) % 3 == 0)
            ACTMOCK_scene.kit.actions[i] =
                ACTMOCK_scene.srcKit.actions[(i + 1) % ACTMOCK_ACTIONS];

    s->ActionRestoreInit(kit);
    s->ActionRestoreInit(kit);
    for (i = 0; i < ACTMOCK_ACTIONS; i++)
        ACTMOCK_Push(ACTMOCK_Tag(ACTMOCK_scene.kit.actions[i]));
}

static void Case_Merge(const tActSide* s, int arg)
{
    int first = arg % ACTMOCK_ACTIONS;
    int last = first + (arg % 4);

    if (last >= ACTMOCK_ACTIONS)
        last = ACTMOCK_ACTIONS - 1;

    if ((arg >> 2) & 1)
        s->KitMergeOver(&ACTMOCK_scene.gao, &ACTMOCK_scene.srcGao, first, last);
    else
        s->KitMergeNoOver(&ACTMOCK_scene.gao, &ACTMOCK_scene.srcGao, first, last);
    s->KitMerge(&ACTMOCK_scene.gao, &ACTMOCK_scene.srcGao, 0, ACTMOCK_ACTIONS - 1,
                (arg >> 3) & 1);
}

/* ---- the loader ------------------------------------------------------- */

static void Case_LoadAction(const tActSide* s, int arg)
{
    struct sAction* act;

    LOA_gb_SpeedMode = (arg >> 4) & 1;
    BIG_gi_ReadMode = ((arg >> 5) & 1) ? 2 : 1;

    act = s->ActionCallback(1);
    ACTMOCK_Push(ACTMOCK_Tag(act));
    ACTMOCK_Push(act ? act->actionElemNum : -1);
    ACTMOCK_Push(act ? act->actionElemIDForTransition : -1);
    ACTMOCK_Push(act ? act->usageCount : -1);

    /* an empty file, which is the 324-byte fallback */
    act = s->ActionCallback((uint32_t)-1);
    ACTMOCK_Push(ACTMOCK_Tag(act));
    ACTMOCK_Push(act ? act->actionElemNum : -1);
}

static void Case_LoadActionKit(const tActSide* s, int arg)
{
    struct sActionKit* kit;

    LOA_gb_SpeedMode = (arg >> 4) & 1;
    BIG_gi_ReadMode = ((arg >> 5) & 1) ? 2 : 1;

    kit = s->ActionKitCallback(0);
    ACTMOCK_Push(ACTMOCK_Tag(kit));
    ACTMOCK_Push(kit ? kit->elemNum : -1);
    ACTMOCK_Push(kit ? kit->flags : -1);

    kit = s->ActionKitCallback((uint32_t)-1);
    ACTMOCK_Push(ACTMOCK_Tag(kit));
    ACTMOCK_Push(kit ? kit->elemNum : -1);
}

/* Loading an action and then freeing it, which is the only path that
   runs the loader's BAS structures through BAS_bfree. */
static void Case_LoadAndFree(const tActSide* s, int arg)
{
    struct sAction* act;

    LOA_gb_SpeedMode = (arg >> 4) & 1;
    BIG_gi_ReadMode = ((arg >> 5) & 1) ? 2 : 1;

    act = s->ActionCallback(1);
    /* not ACT_UseAction: the loader leaves an event-list key of -1 in
       place as a pointer, and the engine would only walk it once the
       deferred reference had been resolved */
    act->usageCount = (int16_t)(1 + (arg % 2));
    s->FreeAction(&act);
    ACTMOCK_Push(ACTMOCK_Tag(act));
    if (act)
        s->FreeAction(&act);
    ACTMOCK_Push(ACTMOCK_Tag(act));
}

/* ------------------------------------------------------------------ main */

int main(void)
{
    int arg;
    char name[64];

    ACTMOCK_Init();

    for (arg = 0; arg < 48; arg++)
    {
        sprintf(name, "findtransition/%d", arg);
        RunCase(name, Case_FindTransition, arg);
        sprintf(name, "setaction/%d", arg);
        RunCase(name, Case_SetAction, arg);
        sprintf(name, "setactionNo0/%d", arg);
        RunCase(name, Case_SetAction, arg | (1 << 10));
        sprintf(name, "setaction2/%d", arg);
        RunCase(name, Case_SetAction, arg | (1 << 13));
        sprintf(name, "setactionEVE/%d", arg);
        RunCase(name, Case_SetAction, arg | (1 << 14));
        sprintf(name, "setactionAI/%d", arg);
        RunCase(name, Case_SetAction, arg | (1 << 15) | ((arg & 3) << 16));
        sprintf(name, "nofrom/%d", arg);
        RunCase(name, Case_SetActionNoFromTransition, arg);
        sprintf(name, "blend/%d", arg);
        RunCase(name, Case_SetActionBlend, arg);
        sprintf(name, "repeat/%d", arg);
        RunCase(name, Case_SetActionRepeat, arg);
        sprintf(name, "during/%d", arg);
        RunCase(name, Case_SetActionDuringTransition, arg);
        sprintf(name, "usage/%d", arg);
        RunCase(name, Case_UsageCounts, arg);
        sprintf(name, "freeaction/%d", arg);
        RunCase(name, Case_FreeAction, arg);
        sprintf(name, "freekit/%d", arg);
        RunCase(name, Case_FreeActionKit, arg);
        sprintf(name, "saverestore/%d", arg);
        RunCase(name, Case_SaveRestore, arg);
        sprintf(name, "merge/%d", arg);
        RunCase(name, Case_Merge, arg);
        sprintf(name, "loadaction/%d", arg);
        RunCase(name, Case_LoadAction, arg);
        sprintf(name, "loadkit/%d", arg);
        RunCase(name, Case_LoadActionKit, arg);
        sprintf(name, "loadfree/%d", arg);
        RunCase(name, Case_LoadAndFree, arg);
    }

    ACTMOCK_Summary("actions");
    return ACTMOCK_failures ? 1 : 0;
}
