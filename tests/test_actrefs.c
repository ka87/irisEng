/* A transition reached through the translation layer.
 *
 * actions.c keeps a transition in the four-byte value beside its key in
 * a BAS array, so what goes in there is a handle and not an address.
 * Three sites do it: the loader stores one, ACT_pst_FindTransition
 * reads one back, and ACT_FreeAction gives one up.
 *
 * The differential beside this one cannot see any of that. It compares
 * the rewritten actions.c against a frozen copy that stores a truncated
 * address, so it has to build both sides with the translation layer in
 * its identity form or disagree at that word in 92 of its 864 cases by
 * construction. That keeps its byte-for-byte comparison, which is what
 * it is for, and leaves the translating path uncovered.
 *
 * This is that path. Everything here builds with translation on, so a
 * handle really is a small number that is not an address, and the first
 * thing checked is exactly that -- otherwise every check below would
 * pass on a copy that never translated anything at all.
 */

#include <stdio.h>
#include <string.h>
#include <stdint.h>

#include "../common.h"
#include "../math.h"
#include "../basic_array.h"
#include "../events.h"
#include "../gameobject.h"
#include "../animation.h"
#include "../handle.h"
#include "actions_mocks.h"

#if !HDL_TRANSLATES
#error "this test is the translating path; build it without HDL_TRANSLATES=0"
#endif

extern int LOA_gb_SpeedMode;
extern int BIG_gi_ReadMode;

struct sTransitionElem* ACT_pst_FindTransition(tActionKit*, uint16_t,
                                               uint8_t, uint16_t);
struct sAction* ACT_ul_ActionCallback(uint32_t);
void ACT_FreeAction(tAction**);

static int failures;

static void fail(const char* what)
{
    printf("  FAIL %s\n", what);
    ++failures;
}

static void setup(unsigned int seed, int pattern)
{
    ACTMOCK_BuildScene(seed, pattern);
    ACTMOCK_BuildFiles(0x77u + (unsigned int)pattern, pattern);
    ACTMOCK_Reset();
    ACTMOCK_RegisterCallback((void*)ACT_ul_ActionCallback, 3);
    ACTMOCK_RegisterCallback((void*)EVE_ul_CallbackLoadListTracks, 1);
    ACTMOCK_RegisterCallback((void*)ANI_ul_ShapeCallback, 2);
}

/* The check on every other check here.
 *
 * With translation on, a handle is an index and a generation in the top
 * thirty bits and is nothing like the address it stands for. If the two
 * ever coincided -- a build where the layer is the identity, a slot
 * value that was never put through it -- then reading a value back as a
 * pointer would work whether or not anything resolved it, and none of
 * the checks below would mean anything. */
static void check_the_value_is_not_the_address(void)
{
    int a;
    int e;
    int k;
    int compared = 0;

    setup(0x4321u, 0);

    for (a = 0; a < ACTMOCK_ACTIONS; a++)
        for (e = 0; e < ACTMOCK_ELEMS; e++)
        {
            tActionElem* elems = ACTMOCK_Elems(&ACTMOCK_scene.actions[a].action);
            if (!elems[e].transition)
                continue;
            for (k = 0; k < ACTMOCK_TRANS; k++)
            {
                const void* real = &ACTMOCK_scene.transElems[a][e][k];
                uint32_t stored = (uint32_t)ACTMOCK_scene.keyVals[a][e][k].val;

                if (stored == (uint32_t)(uintptr_t)real)
                {
                    fail("a stored value is the address itself, so this "
                         "test cannot tell a resolved reference from an "
                         "unresolved one");
                    return;
                }
                if (HDL_p_Pointer(stored) != real)
                    fail("a stored value does not resolve to the "
                         "transition it was made from");
                ++compared;
            }
        }

    if (compared < 100)
        fail("the scene has almost no transitions, so this proves little");
}

/* The search. Every key the scene put in an element's array has to come
   back as the transition it was made from, and a key that is not in it
   has to come back as nothing. */
static void check_search(int pattern)
{
    int a;
    int e;
    int id;
    int found = 0;

    setup(0x4321u + (unsigned int)pattern * 2654u, pattern);

    for (a = 0; a < ACTMOCK_ACTIONS; a++)
        for (e = 0; e < ACTMOCK_ELEMS; e++)
        {
            tActionElem* elems = ACTMOCK_Elems(&ACTMOCK_scene.actions[a].action);

            for (id = 0; id < 2 * ACTMOCK_TRANS + 1; id++)
            {
                struct sTransitionElem* got =
                    ACT_pst_FindTransition(&ACTMOCK_scene.kit.kit,
                                           (uint16_t)a, (uint8_t)e,
                                           (uint16_t)id);
                const void* want = NULL;

                if (elems[e].transition && (id % 2) == 0
                    && id / 2 < ACTMOCK_TRANS)
                    want = &ACTMOCK_scene.transElems[a][e][id / 2];

                if ((const void*)got != want)
                {
                    printf("  FAIL action %d element %d id %d: the search "
                           "gave %p and the scene put %p there\n",
                           a, e, id, (void*)got, want);
                    ++failures;
                }
                if (want)
                    ++found;
            }
        }

    if (!found)
        fail("no key was found at all, so the search was never exercised");
}

/* The sentinel, which is a pointer-shaped question of its own: an
   action index of 0xFFFF means no action rather than the action at that
   index, and the harness owns memory there so the guard can be driven
   rather than reading whatever follows the table. */
static void check_no_action(void)
{
    setup(0x4321u, 0);
    if (ACT_pst_FindTransition(&ACTMOCK_scene.kit.kit, 0xFFFF, 0, 0))
        fail("the no-action sentinel found a transition");
}

/* The loader's own store, and then giving it up again.
 *
 * The loader allocates each transition out of the arena and puts a
 * handle beside its key; freeing the action walks the array and gives
 * every one of them back. So the live count has to return to where it
 * started, and the handles have to stop resolving. */
static void check_load_and_free(int pattern)
{
    struct sAction* act;
    tActionElem* elems;
    int before;
    int afterLoad;
    int i;
    int k;
    int stored[ACTMOCK_ELEMS * ACTMOCK_TRANS];
    int storedNum = 0;

    setup(0x4321u + (unsigned int)pattern * 2654u, pattern);
    LOA_gb_SpeedMode = (pattern >> 4) & 1;
    BIG_gi_ReadMode = ((pattern >> 5) & 1) ? 2 : 1;

    before = HDL_i_LiveCount();
    act = ACT_ul_ActionCallback(1);
    if (!act)
    {
        fail("the loader gave back no action");
        return;
    }
    afterLoad = HDL_i_LiveCount();

    elems = ACTMOCK_Elems(act);
    for (i = 0; i < act->actionElemNum; i++)
    {
        BASStruct* t = elems[i].transition;
        if (!t)
            continue;
        for (k = 0; k < t->actElemNum; k++)
        {
            uint32_t h = (uint32_t)t->keyValPtr[k].val;
            void* p = HDL_p_Pointer(h);

            /* A handle the loader issued, so it resolves, and to a
               block the arena handed out rather than to the number
               itself. */
            if (!p)
                fail("a handle the loader stored resolves to nothing");
            else if ((unsigned char*)p < ACTMOCK_arena
                     || (unsigned char*)p >= ACTMOCK_arena + ACTMOCK_ARENA)
                fail("a handle the loader stored resolves outside the arena");
            else if (p == (void*)(uintptr_t)h)
                fail("a handle the loader stored is its own address, so "
                     "nothing was translated");

            if (storedNum < (int)(sizeof(stored) / sizeof(stored[0])))
                stored[storedNum++] = (int)h;
        }
    }

    if (!storedNum)
    {
        fail("the loaded action has no transitions, so the store and the "
             "release were never exercised");
        return;
    }
    if (afterLoad <= before)
        fail("the loader stored transitions and took no handles for them");

    /* Freeing it gives them all back. */
    act->usageCount = 1;
    ACT_FreeAction(&act);
    if (act)
        fail("freeing the action left the caller's pointer set");

    if (HDL_i_LiveCount() != before)
        fail("freeing the action did not give back every handle it took");

    for (k = 0; k < storedNum; k++)
        if (HDL_p_Pointer((uint32_t)stored[k]))
        {
            fail("a handle still resolves after the action that held it "
                 "was freed");
            break;
        }
}

int main(void)
{
    int pattern;

    printf("=== a transition reached through the translation layer ===\n");

    ACTMOCK_Init();

    check_the_value_is_not_the_address();
    check_no_action();
    for (pattern = 0; pattern < 8; pattern++)
    {
        check_search(pattern);
        check_load_and_free(pattern);
    }

    if (failures)
    {
        printf("  %d failure(s)\n", failures);
        return 1;
    }
    printf("  all checks passed\n");
    return 0;
}
