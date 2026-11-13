/* Differential test for snp.c: the frozen pre-rewrite copy against the
   rewritten one, over identical state.

   Both copies run over one scene at one address, with the arena
   allocator rewound between them, so every pointer either copy stores
   can be compared directly rather than normalised. A case is equivalent
   when the scene bytes, the used prefix of the arena, the driver's own
   scratch block, the return values and the ordered sequence of mocked
   engine calls all match.

   What it is for. The rewrite replaced twelve gotos, folded the
   three-times-unrolled axis body of INT_SnP_AddNode into a loop, folded
   the two mirror halves of INT_SnP_AddOverlap and INT_SnP_RemoveOverlap
   into one helper each, turned the pointer walks of the element
   compaction and of the insertion sort into index arithmetic, and
   dropped a guard that could not fire and a comparison that duplicated
   the one above it. Those are the changes a reading cannot settle --
   the insertion sort in particular carries a float epsilon, a narrowing
   to float inside its sink loop, and an element shuffle whose byte
   count is written out by hand.

   What it does not cover: nothing in the file is left out. Every
   function snp.c defines is driven, including the two the header does
   not declare.

   One thing the harness cannot reproduce is the engine's word size. The
   element shuffles in INT_SnP_UpdateAxisTable and INT_SnP_InsertionSort
   move a hard-coded 16 bytes per element, which is sizeof(tSnpAxisElem)
   in the 32-bit build the engine ships as and not in this one, so an
   element move here copies half an element. Both copies do exactly the
   same thing with it, so the comparison still sees any divergence; what
   it cannot see is a change that only shows at the real element size.
   The axis tables are pre-filled with valid objects so a half-moved
   element is still a readable one. */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "../common.h"
#include "../math.h"
#include "../gameobject.h"
#include "../tables.h"
#include "../world.h"
#include "../snp.h"
#include "snp_mocks.h"

/* ------------------------------------------- the two copies' exports */

#define DECL2(ret, name, args) ret BASE_##name args; ret NEW_##name args;

DECL2(tSnpAxis*, INT_SnP_pstCreateAxisTable, (void))
DECL2(void, INT_SnP_Alloc, (struct sWorld*))
DECL2(void, INT_SnP_DesAlloc, (struct sWorld*))
DECL2(void, INT_SnP_Reinit, (struct sWorld*))
DECL2(int, INT_SnP_GetFlag, (int*, char, int))
DECL2(void, INT_SnP_SetFlag, (int*, char, int, int))
DECL2(int, INT_SnP_FullGetFlag, (int*, int, int, int, int*, int*))
DECL2(int, INT_SnP_FullSetFlag, (int*, int, int, int, int*, int*, int))
DECL2(void, INT_SnP_ResetAllReferenceFlags, (tSnp*, uint16_t))
DECL2(int16_t, INT_SnP_GetFirstFreeIndex, (tSnp*))
DECL2(int, INT_SnP_ObjectMustBeRemoved, (tSnpObjectsToRemoveQueue*, tGameObjectMain*))
DECL2(void, INT_SnP_AddNode, (tSnp*, tGameObjectMain*, Vec3*, char))
DECL2(void, INT_SnP_AddBox, (tSnp*, tGameObjectMain*, Vec3*, Vec3*))
DECL2(void, INT_SnP_AddSphere, (tSnp*, tGameObjectMain*))
DECL2(void, INT_SnP_AttachObject, (struct sGameObjectMain*, struct sWorld*))
DECL2(void, INT_SnP_AddObject, (struct sGameObjectMain*, struct sSnp*))
DECL2(void, INT_SnP_Refresh, (struct sSnp*, tableDescStruct*))
DECL2(void, INT_SnP_AddOverlap, (tGameObjectMain*, tGameObjectMain*))
DECL2(void, INT_SnP_RemoveOverlap, (tGameObjectMain*, tGameObjectMain*))
DECL2(void, INT_SnP_UpdateAllDetectionLists, (tSnp*))
DECL2(void, INT_RegisterObjectForRemoving, (tSnpObjectsToRemoveQueue*, tGameObjectMain*))
DECL2(void, INT_SnP_UpdateAxisTable, (tSnp*, int))
DECL2(void, INT_SnP_DetachObject, (struct sGameObjectMain*, struct sWorld*))
DECL2(void, INT_SnP_UnknownInversionSortInner, (tSnpAxisElem*, int))
DECL2(void, INT_SnP_ComputeInversion, (tSnp*, int, int, int, int, int))
DECL2(void, INT_SnP_InsertionSort, (struct sSnp*, int))

extern unsigned long BASE_LOA_ul_FileTypeSize_38;
extern unsigned long NEW_LOA_ul_FileTypeSize_38;

typedef struct sSnpSide
{
    const char* name;
    tSnpAxis* (*CreateAxisTable)(void);
    void (*Alloc)(struct sWorld*);
    void (*DesAlloc)(struct sWorld*);
    void (*Reinit)(struct sWorld*);
    int (*GetFlag)(int*, char, int);
    void (*SetFlag)(int*, char, int, int);
    int (*FullGetFlag)(int*, int, int, int, int*, int*);
    int (*FullSetFlag)(int*, int, int, int, int*, int*, int);
    void (*ResetAllReferenceFlags)(tSnp*, uint16_t);
    int16_t (*GetFirstFreeIndex)(tSnp*);
    int (*ObjectMustBeRemoved)(tSnpObjectsToRemoveQueue*, tGameObjectMain*);
    void (*AddNode)(tSnp*, tGameObjectMain*, Vec3*, char);
    void (*AddBox)(tSnp*, tGameObjectMain*, Vec3*, Vec3*);
    void (*AddSphere)(tSnp*, tGameObjectMain*);
    void (*AttachObject)(struct sGameObjectMain*, struct sWorld*);
    void (*AddObject)(struct sGameObjectMain*, struct sSnp*);
    void (*Refresh)(struct sSnp*, tableDescStruct*);
    void (*AddOverlap)(tGameObjectMain*, tGameObjectMain*);
    void (*RemoveOverlap)(tGameObjectMain*, tGameObjectMain*);
    void (*UpdateAllDetectionLists)(tSnp*);
    void (*RegisterObjectForRemoving)(tSnpObjectsToRemoveQueue*, tGameObjectMain*);
    void (*UpdateAxisTable)(tSnp*, int);
    void (*DetachObject)(struct sGameObjectMain*, struct sWorld*);
    void (*InversionSortInner)(tSnpAxisElem*, int);
    void (*ComputeInversion)(tSnp*, int, int, int, int, int);
    void (*InsertionSort)(struct sSnp*, int);
    unsigned long* fileTypeSize;
} tSnpSide;

#define SIDE(P) { #P, \
    P##_INT_SnP_pstCreateAxisTable, P##_INT_SnP_Alloc, P##_INT_SnP_DesAlloc, \
    P##_INT_SnP_Reinit, P##_INT_SnP_GetFlag, P##_INT_SnP_SetFlag, \
    P##_INT_SnP_FullGetFlag, P##_INT_SnP_FullSetFlag, \
    P##_INT_SnP_ResetAllReferenceFlags, P##_INT_SnP_GetFirstFreeIndex, \
    P##_INT_SnP_ObjectMustBeRemoved, P##_INT_SnP_AddNode, P##_INT_SnP_AddBox, \
    P##_INT_SnP_AddSphere, P##_INT_SnP_AttachObject, P##_INT_SnP_AddObject, \
    P##_INT_SnP_Refresh, P##_INT_SnP_AddOverlap, P##_INT_SnP_RemoveOverlap, \
    P##_INT_SnP_UpdateAllDetectionLists, P##_INT_RegisterObjectForRemoving, \
    P##_INT_SnP_UpdateAxisTable, P##_INT_SnP_DetachObject, \
    P##_INT_SnP_UnknownInversionSortInner, P##_INT_SnP_ComputeInversion, \
    P##_INT_SnP_InsertionSort, &P##_LOA_ul_FileTypeSize_38 }

static const tSnpSide sides[2] = { SIDE(BASE), SIDE(NEW) };

/* ---------------------------------------------------------------- cases */

/* Each case is a function run once per side over a freshly built scene;
   the first side's result is saved and the second is compared with it. */
typedef void (*tCaseFn)(const tSnpSide* s, int arg);

static void RunCase(const char* name, tCaseFn fn, int arg)
{
    int side;

    SNPMOCK_SetCase(name);
    for (side = 0; side < 2; side++)
    {
        SNPMOCK_BuildScene(0x1234u + (unsigned int)arg * 7919u, arg);
        SNPMOCK_Reset();
        *sides[side].fileTypeSize = 0;
        fn(&sides[side], arg);
        if (side == 0)
            SNPMOCK_Save();
        else
        {
            SNPMOCK_CompareWithSaved(name);
            SNPMOCK_SameLong((long long)*sides[0].fileTypeSize,
                             (long long)*sides[1].fileTypeSize,
                             "LOA_ul_FileTypeSize_38", name);
        }
    }
}

/* ---- the flag primitives --------------------------------------------- */

static void Case_Flags(const tSnpSide* s, int arg)
{
    int* flags = SNPMOCK_scratch.flags;
    int bit;
    int word;
    int a;
    int b;
    int i;
    int acc = 0;

    (void)arg;
    for (i = 0; i < 64; i++)
        s->SetFlag(flags, (char)(i & 31), i % 11, (i % 3) != 0);
    for (i = 0; i < 64; i++)
        acc = acc * 3 + (s->GetFlag(flags, (char)(i & 31), i % 11) != 0);

    for (a = 0; a < 40; a++)
        for (b = 0; b < 40; b++)
        {
            if (a == b)
                continue;
            bit = -1;
            word = -1;
            acc += s->FullSetFlag(flags, a, b, 1024, &bit, &word, (a + b) & 1) != 0;
            acc = acc * 2 + bit + word;
            bit = -1;
            word = -1;
            acc += s->FullGetFlag(flags, a, b, 1024, &bit, &word) != 0;
            acc = acc * 2 + bit + word;
        }

    /* the far end of the triangular index, where the word index is
       largest */
    for (a = 1015; a < 1024; a++)
        for (b = 1015; b < 1024; b++)
        {
            if (a == b)
                continue;
            acc += s->FullSetFlag(SNPMOCK_scratch.snp.axisPtr[0]
                                      ? flags : flags, a, b, 1024, &bit, &word, 1) != 0;
            acc = acc * 2 + bit + word;
        }
    SNPMOCK_scratch.ret = acc;
}

/* ---- GetFirstFreeIndex ------------------------------------------------ */

static void Case_FirstFree(const tSnpSide* s, int arg)
{
    tSnp* snp = &SNPMOCK_scratch.snp;
    int i;
    int acc = 0;

    for (i = 0; i < 1024; i++)
        snp->gaosPtr[i] = &SNPMOCK_scene.gaos[i % SNPMOCK_OBJECTS];

    acc = acc * 31 + s->GetFirstFreeIndex(snp);     /* full */
    snp->gaosPtr[0] = NULL;
    acc = acc * 31 + s->GetFirstFreeIndex(snp);     /* hole at 0 */
    snp->gaosPtr[0] = &SNPMOCK_scene.gaos[0];
    snp->gaosPtr[1] = NULL;
    acc = acc * 31 + s->GetFirstFreeIndex(snp);     /* hole at 1 */
    snp->gaosPtr[1] = &SNPMOCK_scene.gaos[1];
    snp->gaosPtr[1023] = NULL;
    acc = acc * 31 + s->GetFirstFreeIndex(snp);     /* hole at the end */
    snp->gaosPtr[1023] = &SNPMOCK_scene.gaos[3];
    snp->gaosPtr[1022] = NULL;
    acc = acc * 31 + s->GetFirstFreeIndex(snp);
    snp->gaosPtr[1022] = &SNPMOCK_scene.gaos[2];
    snp->gaosPtr[arg % 1024] = NULL;
    acc = acc * 31 + s->GetFirstFreeIndex(snp);
    memset(snp->gaosPtr, 0, sizeof(snp->gaosPtr));
    acc = acc * 31 + s->GetFirstFreeIndex(snp);     /* empty */
    SNPMOCK_scratch.ret = acc;
}

/* ---- ObjectMustBeRemoved and the removal queue ------------------------ */

static void Case_Queue(const tSnpSide* s, int arg)
{
    tSnpObjectsToRemoveQueue* q = &SNPMOCK_scratch.queue;
    int i;
    int acc = 0;

    q->num = 0;
    for (i = 0; i < SNPMOCK_OBJECTS; i++)
        acc = acc * 3 + s->ObjectMustBeRemoved(q, &SNPMOCK_scene.gaos[i]);

    for (i = 0; i < SNPMOCK_OBJECTS; i += 2)
        s->RegisterObjectForRemoving(q, &SNPMOCK_scene.gaos[i]);
    for (i = 0; i < SNPMOCK_OBJECTS; i++)
        acc = acc * 3 + s->ObjectMustBeRemoved(q, &SNPMOCK_scene.gaos[i]);
    acc = acc * 3 + s->ObjectMustBeRemoved(q, NULL);

    q->num = 1;
    for (i = 0; i < SNPMOCK_OBJECTS; i++)
        acc = acc * 3 + s->ObjectMustBeRemoved(q, &SNPMOCK_scene.gaos[i]);
    (void)arg;
    SNPMOCK_scratch.ret = acc;
}

/* ---- the detection lists --------------------------------------------- */

static void Case_Overlaps(const tSnpSide* s, int arg)
{
    int i;
    int j;

    for (i = 0; i < SNPMOCK_OBJECTS; i++)
        for (j = 0; j < SNPMOCK_OBJECTS; j++)
            if (i != j && ((i + j) % 3) != 2)
                s->AddOverlap(&SNPMOCK_scene.gaos[i], &SNPMOCK_scene.gaos[j]);

    /* the same pairs again: every one is already in its list */
    for (i = 0; i < SNPMOCK_OBJECTS; i++)
        for (j = 0; j < SNPMOCK_OBJECTS; j++)
            if (i != j && ((i + j) % 3) != 2)
                s->AddOverlap(&SNPMOCK_scene.gaos[i], &SNPMOCK_scene.gaos[j]);

    for (i = 0; i < SNPMOCK_OBJECTS; i++)
        for (j = SNPMOCK_OBJECTS - 1; j >= 0; j--)
            if (i != j && ((i * j) % 4) != 3)
                s->RemoveOverlap(&SNPMOCK_scene.gaos[i], &SNPMOCK_scene.gaos[j]);

    /* removing what is no longer there */
    for (i = 0; i < SNPMOCK_OBJECTS; i++)
        s->RemoveOverlap(&SNPMOCK_scene.gaos[i], &SNPMOCK_scene.gaos[(i + 1) % SNPMOCK_OBJECTS]);
    (void)arg;
}

/* The realloc arm: a list that is already at the 150-entry bound. */
static void Case_OverlapGrow(const tSnpSide* s, int arg)
{
    tGameObjectCollDetectionList* list = &SNPMOCK_scene.dlist[0];
    int i;

    SNPMOCK_scene.gaos[0].flags2 |= 0x20;
    SNPMOCK_scene.gaos[1].flags2 &= ~0x20;
    list->overlapNum = 148 + (arg % 4);
    for (i = 0; i < list->overlapNum; i++)
        list->detectionListGaos[i] = (tGameObjectMain*)(void*)&SNPMOCK_scene.table[i % SNPMOCK_OBJECTS];

    s->AddOverlap(&SNPMOCK_scene.gaos[0], &SNPMOCK_scene.gaos[1]);
    s->AddOverlap(&SNPMOCK_scene.gaos[0], &SNPMOCK_scene.gaos[2]);
    s->AddOverlap(&SNPMOCK_scene.gaos[0], &SNPMOCK_scene.gaos[1]);
    s->RemoveOverlap(&SNPMOCK_scene.gaos[0], &SNPMOCK_scene.gaos[1]);
}

/* ---- allocation, refresh, attach, detach ------------------------------ */

static void AttachAll(const tSnpSide* s, int useAddObject)
{
    int i;

    for (i = 0; i < SNPMOCK_OBJECTS; i++)
    {
        if (useAddObject)
            s->AddObject(&SNPMOCK_scene.gaos[i], SNPMOCK_scene.world.snp);
        else
            s->AttachObject(&SNPMOCK_scene.gaos[i], &SNPMOCK_scene.world);
    }
}

static void Case_AllocAttach(const tSnpSide* s, int arg)
{
    s->Alloc(&SNPMOCK_scene.world);
    SNPMOCK_FillAxisTails(SNPMOCK_scene.world.snp);
    AttachAll(s, arg & 1);
    /* attaching twice: every object already has an index */
    AttachAll(s, arg & 1);
    s->Reinit(&SNPMOCK_scene.world);
    s->DesAlloc(&SNPMOCK_scene.world);
}

static void Case_Refresh(const tSnpSide* s, int arg)
{
    s->Alloc(&SNPMOCK_scene.world);
    SNPMOCK_FillAxisTails(SNPMOCK_scene.world.snp);
    s->Refresh(SNPMOCK_scene.world.snp, &SNPMOCK_scene.tableDesc);
    s->Refresh(SNPMOCK_scene.world.snp, &SNPMOCK_scene.tableDesc);
    (void)arg;
}

static void Case_AddNodeDirect(const tSnpSide* s, int arg)
{
    int i;

    s->Alloc(&SNPMOCK_scene.world);
    SNPMOCK_FillAxisTails(SNPMOCK_scene.world.snp);
    for (i = 0; i < SNPMOCK_OBJECTS; i++)
    {
        /* every flag combination the engine uses, plus the two the
           callers never build */
        s->AddNode(SNPMOCK_scene.world.snp, &SNPMOCK_scene.gaos[i],
                   &SNPMOCK_scene.bvs[i].BVMin, (char)(0x11 + (i & 1)));
        s->AddNode(SNPMOCK_scene.world.snp, &SNPMOCK_scene.gaos[i],
                   &SNPMOCK_scene.bvs[i].BVMin, (char)(0x09 + (i & 1)));
    }
    for (i = 0; i < SNPMOCK_OBJECTS; i++)
    {
        s->AddBox(SNPMOCK_scene.world.snp, &SNPMOCK_scene.gaos[i],
                  &SNPMOCK_scene.bvs[i].BVMin, &SNPMOCK_scene.bvs[i].BVMax);
        s->AddSphere(SNPMOCK_scene.world.snp, &SNPMOCK_scene.gaos[i]);
    }
    (void)arg;
}

/* ---- the axis-table compaction ---------------------------------------- */

static void Case_UpdateAxisTable(const tSnpSide* s, int arg)
{
    tSnp* snp;
    int i;

    s->Alloc(&SNPMOCK_scene.world);
    snp = SNPMOCK_scene.world.snp;
    SNPMOCK_FillAxisTails(snp);
    AttachAll(s, 0);

    /* a removal queue picked by the argument, so adjacent, first-slot
       and last-slot removals all come up */
    snp->snpObjectsRemoveQueue->num = 0;
    for (i = 0; i < SNPMOCK_OBJECTS; i++)
        if (((arg >> (i & 15)) & 1) || (i % 5) == (arg % 5))
            s->RegisterObjectForRemoving(snp->snpObjectsRemoveQueue, &SNPMOCK_scene.gaos[i]);

    s->UpdateAllDetectionLists(snp);
    s->UpdateAxisTable(snp, 0);
    s->UpdateAxisTable(snp, 1);
    s->UpdateAxisTable(snp, 2);
}

static void Case_Detach(const tSnpSide* s, int arg)
{
    int i;

    s->Alloc(&SNPMOCK_scene.world);
    SNPMOCK_FillAxisTails(SNPMOCK_scene.world.snp);
    AttachAll(s, 0);
    for (i = 0; i < SNPMOCK_OBJECTS; i++)
        if (((arg >> (i & 15)) & 1) == 0)
            s->DetachObject(&SNPMOCK_scene.gaos[i], &SNPMOCK_scene.world);
    /* detaching one that is already gone */
    s->DetachObject(&SNPMOCK_scene.gaos[0], &SNPMOCK_scene.world);
}

/* ---- the sort --------------------------------------------------------- */

static void Case_InversionInner(const tSnpSide* s, int arg)
{
    tSnp* snp;
    int i;
    int axis;

    s->Alloc(&SNPMOCK_scene.world);
    snp = SNPMOCK_scene.world.snp;
    SNPMOCK_FillAxisTails(snp);
    AttachAll(s, 0);
    for (axis = 0; axis < 3; axis++)
        for (i = 0; i < snp->currElemIdx; i++)
            s->InversionSortInner(&snp->axisPtr[axis]->elems[i], axis);
    /* an axis index the callers never pass */
    s->InversionSortInner(&snp->axisPtr[0]->elems[0], 3 + (arg & 1));
}

static void Case_ComputeInversion(const tSnpSide* s, int arg)
{
    tSnp* snp;
    int a;
    int b;
    int axis;

    s->Alloc(&SNPMOCK_scene.world);
    snp = SNPMOCK_scene.world.snp;
    SNPMOCK_FillAxisTails(snp);
    AttachAll(s, 0);

    for (axis = 0; axis < 3; axis++)
        for (a = 0; a < SNPMOCK_OBJECTS; a++)
            for (b = 0; b < SNPMOCK_OBJECTS; b++)
            {
                if (a == b)
                    continue;
                /* all four combinations of the two endpoint flags */
                s->ComputeInversion(snp, axis, a, b, (a + arg) & 1, (b + arg) & 1);
                s->ComputeInversion(snp, axis, a, b, (a + arg + 1) & 1, (b + arg) & 1);
            }
}

static void Case_Sort(const tSnpSide* s, int arg)
{
    tSnp* snp;
    int frame;
    int mode = arg % 3;

    s->Alloc(&SNPMOCK_scene.world);
    snp = SNPMOCK_scene.world.snp;
    SNPMOCK_FillAxisTails(snp);
    AttachAll(s, 0);

    for (frame = 0; frame < 6; frame++)
    {
        SNPMOCK_MoveObjects(0x9E37u + (unsigned int)(frame * 31 + arg), 6.0f * frame);
        s->InsertionSort(&SNPMOCK_scene.world.snp[0], mode);
    }
    /* the modes no caller passes */
    s->InsertionSort(SNPMOCK_scene.world.snp, 3);
    s->InsertionSort(SNPMOCK_scene.world.snp, -1);
}

/* An empty sweep-and-prune: the early return. */
static void Case_SortEmpty(const tSnpSide* s, int arg)
{
    s->Alloc(&SNPMOCK_scene.world);
    SNPMOCK_FillAxisTails(SNPMOCK_scene.world.snp);
    s->InsertionSort(SNPMOCK_scene.world.snp, arg % 4);
    s->UpdateAllDetectionLists(SNPMOCK_scene.world.snp);
    s->DetachObject(&SNPMOCK_scene.gaos[0], &SNPMOCK_scene.world);
    SNPMOCK_scene.world.snp = NULL;
    s->DetachObject(&SNPMOCK_scene.gaos[0], &SNPMOCK_scene.world);
    s->Reinit(&SNPMOCK_scene.world);
    s->DesAlloc(&SNPMOCK_scene.world);
}

/* One object attached, then the whole frame loop: attach, sort, move,
   sort, detach, sort. This is the only case that runs the pieces in the
   order the engine does. */
static void Case_Frames(const tSnpSide* s, int arg)
{
    tSnp* snp;
    int frame;
    int i;

    s->Alloc(&SNPMOCK_scene.world);
    snp = SNPMOCK_scene.world.snp;
    SNPMOCK_FillAxisTails(snp);

    for (frame = 0; frame < 8; frame++)
    {
        for (i = 0; i < SNPMOCK_OBJECTS; i++)
            if ((i % 8) <= frame)
                s->AttachObject(&SNPMOCK_scene.gaos[i], &SNPMOCK_scene.world);
        SNPMOCK_MoveObjects(0x5EEDu + (unsigned int)(frame * 101 + arg * 7),
                            2.0f * (float)frame);
        s->InsertionSort(SNPMOCK_scene.world.snp, 0);
        if (frame == 4)
            s->DetachObject(&SNPMOCK_scene.gaos[(arg + 1) % SNPMOCK_OBJECTS],
                            &SNPMOCK_scene.world);
    }
    s->Refresh(snp, &SNPMOCK_scene.tableDesc);
    s->InsertionSort(SNPMOCK_scene.world.snp, 0);
    s->Reinit(&SNPMOCK_scene.world);
    s->DesAlloc(&SNPMOCK_scene.world);
}

/* The sort's two float boundaries, which randomly placed objects never
   reach: a key chain whose step is exactly the 0.001 the sink loop adds
   (so elems[pos] lands exactly on the threshold), and one whose step
   sits between 0.0001 and 0.001 (so the size of the epsilon decides
   whether anything sinks at all). */
static void Case_SortTies(const tSnpSide* s, int arg)
{
    tSnp* snp;

    SNPMOCK_ChainKeys(0.001, 20.0f);
    s->Alloc(&SNPMOCK_scene.world);
    snp = SNPMOCK_scene.world.snp;
    SNPMOCK_FillAxisTails(snp);
    AttachAll(s, 0);
    s->InsertionSort(snp, arg % 3);
    s->InsertionSort(snp, 0);
}

static void Case_SortEpsilon(const tSnpSide* s, int arg)
{
    tSnp* snp;

    SNPMOCK_ChainKeys(0.0005, 0.0002f);
    s->Alloc(&SNPMOCK_scene.world);
    snp = SNPMOCK_scene.world.snp;
    SNPMOCK_FillAxisTails(snp);
    AttachAll(s, 0);
    s->InsertionSort(snp, arg % 3);
    s->InsertionSort(snp, 0);
}

/* Overlaps built by the sort and then cleared, which is the only way
   INT_SnP_UpdateAllDetectionLists' first arm has a list to walk. The
   scene is laid out tight so the boxes really do overlap. */
static void Case_ClearLists(const tSnpSide* s, int arg)
{
    tSnp* snp;
    int frame;
    int i;

    s->Alloc(&SNPMOCK_scene.world);
    snp = SNPMOCK_scene.world.snp;
    SNPMOCK_FillAxisTails(snp);
    AttachAll(s, 0);

    for (frame = 0; frame < 5; frame++)
    {
        SNPMOCK_MoveObjects(0x77F1u + (unsigned int)(frame * 13 + arg), 3.0f * frame);
        s->InsertionSort(snp, 0);
    }

    SNPMOCK_scratch.ret = 0;
    for (i = 0; i < SNPMOCK_OBJECTS; i++)
        SNPMOCK_scratch.ret += SNPMOCK_scene.dlist[i].overlapNum;

    snp->snpObjectsRemoveQueue->num = 0;
    for (i = 0; i < SNPMOCK_OBJECTS; i++)
        if ((i + arg) % 3 != 2)
            s->RegisterObjectForRemoving(snp->snpObjectsRemoveQueue, &SNPMOCK_scene.gaos[i]);
    s->UpdateAllDetectionLists(snp);
    s->UpdateAllDetectionLists(snp);
}

/* ---- INT_SnP_pstCreateAxisTable and ResetAllReferenceFlags ------------ */

static void Case_CreateAxis(const tSnpSide* s, int arg)
{
    tSnpAxis* a;
    tSnpAxis* b;
    tSnp* snp = &SNPMOCK_scratch.snp;

    a = s->CreateAxisTable();
    b = s->CreateAxisTable();
    snp->axisPtr[0] = a;
    snp->axisPtr[1] = b;
    snp->axisPtr[2] = s->CreateAxisTable();
    memset(snp->axisPtr[0]->flagsArray, 0xA5, sizeof(snp->axisPtr[0]->flagsArray));
    memset(snp->axisPtr[1]->flagsArray, 0x5A, sizeof(snp->axisPtr[1]->flagsArray));
    memset(snp->axisPtr[2]->flagsArray, 0xFF, sizeof(snp->axisPtr[2]->flagsArray));
    s->ResetAllReferenceFlags(snp, (uint16_t)(arg % 1024));
    s->ResetAllReferenceFlags(snp, 0);
    s->ResetAllReferenceFlags(snp, 1023);
    s->ResetAllReferenceFlags(snp, 0xFFFF);
}

/* ------------------------------------------------------------------ main */

int main(void)
{
    int arg;
    char name[64];

    SNPMOCK_Init();

    for (arg = 0; arg < 8; arg++)
    {
        sprintf(name, "flags/%d", arg);
        RunCase(name, Case_Flags, arg);
        sprintf(name, "firstfree/%d", arg);
        RunCase(name, Case_FirstFree, arg);
        sprintf(name, "queue/%d", arg);
        RunCase(name, Case_Queue, arg);
        sprintf(name, "createaxis/%d", arg);
        RunCase(name, Case_CreateAxis, arg * 137);
    }

    for (arg = 0; arg < 24; arg++)
    {
        sprintf(name, "overlaps/%d", arg);
        RunCase(name, Case_Overlaps, arg | (1 << 17));
        sprintf(name, "overlapgrow/%d", arg);
        RunCase(name, Case_OverlapGrow, arg);
        sprintf(name, "allocattach/%d", arg);
        RunCase(name, Case_AllocAttach, arg);
        sprintf(name, "refresh/%d", arg);
        RunCase(name, Case_Refresh, arg | (1 << 18));
        sprintf(name, "addnode/%d", arg);
        RunCase(name, Case_AddNodeDirect, arg);
        sprintf(name, "axistable/%d", arg);
        RunCase(name, Case_UpdateAxisTable, arg);
        sprintf(name, "detach/%d", arg);
        RunCase(name, Case_Detach, arg);
        sprintf(name, "inner/%d", arg);
        RunCase(name, Case_InversionInner, arg);
        sprintf(name, "computeinv/%d", arg);
        RunCase(name, Case_ComputeInversion, arg);
        sprintf(name, "sort/%d", arg);
        RunCase(name, Case_Sort, arg);
        sprintf(name, "sortempty/%d", arg);
        RunCase(name, Case_SortEmpty, arg);
        sprintf(name, "frames/%d", arg);
        RunCase(name, Case_Frames, arg);
        sprintf(name, "sortties/%d", arg);
        RunCase(name, Case_SortTies, arg);
        sprintf(name, "sorteps/%d", arg);
        RunCase(name, Case_SortEpsilon, arg);
        sprintf(name, "clearlists/%d", arg);
        RunCase(name, Case_ClearLists, arg | (1 << 19));
    }

    SNPMOCK_Summary("snp");
    return SNPMOCK_failures ? 1 : 0;
}
