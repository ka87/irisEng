/* The address store, and the reference resolver that reads it.
 *
 * When a world file is loaded, a thing's position in the file is
 * recorded against the address it was loaded to, and a reference to it
 * elsewhere is resolved by looking that up. The slots the answer goes
 * into are four bytes wide and fixed by the file formats, so on a
 * build with wider pointers the answer cannot be an address and is a
 * handle instead.
 *
 * Nothing covered any of this. There is no differential for loading.c
 * or ai_common.c and no frozen copy of either, so these are direct
 * assertions, and they are of two kinds:
 *
 *   the store, and the resolver's own rules about what it does and
 *   does not look up -- true before the handles went in and after, so
 *   they are what says the change preserved the behaviour
 *
 *   and a pointer surviving the round trip, which is the point of the
 *   change and which could not have passed before it
 *
 * The resolver's rules are the interesting half, because they are what
 * decides which values a handle may never take: nought means no
 * object, an answer of -1 means the key was not found, and one and two
 * are the two main actors rather than references at all.
 */

#include <stdio.h>
#include <stdint.h>
#include "../loading.h"
#include "../ai.h"
#include "../handle.h"

/* The driver's half of the big-file key lookup, in the mocks. */
void LOAMOCK_SetKeyPos(uint32_t key, int pos);
void LOAMOCK_ForgetKeys(void);

/* loading.h does not declare it. */
void LOA_BeforeLoading(void);

static int failures;

static void fail(const char* what)
{
    printf("  FAIL %s\n", what);
    ++failures;
}

static void failn(const char* what, long long got, long long want)
{
    printf("  FAIL %s: got %lld, wanted %lld\n", what, got, want);
    ++failures;
}

/* What a position resolves to. -1 is the store's own "not here". */
#define LOA_NOT_FOUND (-1)

/* ------------------------------------------------------------------ */
/* The store                                                          */

static void check_store(void)
{
    enum { N = 300 };
    int i;

    /* Put a distinct value against each of many positions, then ask
       for every one of them. The positions are spread so that the
       store's own eight-bit bucket hash has to put several in the same
       bucket -- which is the part a single pair would not reach. */
    for (i = 0; i < N; ++i)
        LOA_AddPosAddress((unsigned int)(i * 4096 + 8), (unsigned int)((i + 1) * 4));

    for (i = 0; i < N; ++i)
    {
        int got = LOA_ul_SearchAddress(i * 4096 + 8);
        if (got != (i + 1) * 4)
            failn("a position did not give back what was stored against it",
                  got, (i + 1) * 4);
    }

    /* Positions that land in the same bucket as one another. The hash
       is (pos >> 8) + pos taken as a byte, so these three collide. */
    LOA_AddPosAddress(0x10000u, 0x40u);
    LOA_AddPosAddress(0x20000u, 0x80u);
    LOA_AddPosAddress(0x30000u, 0xC0u);
    if (LOA_ul_SearchAddress(0x10000) != 0x40)
        fail("the first of three colliding positions");
    if (LOA_ul_SearchAddress(0x20000) != 0x80)
        fail("the second of three colliding positions");
    if (LOA_ul_SearchAddress(0x30000) != 0xC0)
        fail("the third of three colliding positions");

    /* A position nothing was stored against. */
    if (LOA_ul_SearchAddress(0x7777) != LOA_NOT_FOUND)
        failn("a position nothing was stored against",
              LOA_ul_SearchAddress(0x7777), LOA_NOT_FOUND);

    /* Overwriting a position with a second value. */
    LOA_AddPosAddress(0x10000u, 0x44u);
    if (LOA_ul_SearchAddress(0x10000) != 0x44)
        fail("a position was not overwritten by a second value");
}

static void check_delete(void)
{
    LOA_AddPosAddress(0x50000u, 0x1234u);
    if (LOA_ul_SearchAddress(0x50000) != 0x1234)
    {
        fail("the entry to be deleted was not there to start with");
        return;
    }

    /* Deletion goes by the stored value, not by the position: that is
       what the store's second array is for, and the parameter's name
       says otherwise. */
    LOA_DeleteAddress(0x1234);
    if (LOA_ul_SearchAddress(0x50000) != LOA_NOT_FOUND)
        failn("the position after its entry was deleted",
              LOA_ul_SearchAddress(0x50000), LOA_NOT_FOUND);

    /* Deleting something that was never stored is harmless, and does
       not disturb what is. */
    LOA_AddPosAddress(0x51000u, 0x2345u);
    LOA_DeleteAddress(0x9999);
    if (LOA_ul_SearchAddress(0x51000) != 0x2345)
        fail("a stray delete disturbed another entry");

    /* And the position can be used again afterwards. */
    LOA_AddPosAddress(0x50000u, 0x5678u);
    if (LOA_ul_SearchAddress(0x50000) != 0x5678)
        fail("a position could not be used again after a delete");
}

/* ------------------------------------------------------------------ */
/* The resolver                                                       */

static void check_which_types_are_refs(void)
{
    static const int refs[] = { 39, 40, 42, 43 };
    /* Written in decimal on purpose. These are the resolver's own type
       ids, not the variable-type ids the type table uses, and the two
       spaces overlap confusingly -- 0x28 reads like a type-table entry
       and is 40, which is a reference. */
    static const int notRefs[] = { 0, 1, 38, 41, 44, 124, 100 };
    unsigned i;

    for (i = 0; i < sizeof(refs) / sizeof(refs[0]); ++i)
        if (!AI_b_IsARef(refs[i]))
            failn("this type should be a reference", refs[i], 1);
    for (i = 0; i < sizeof(notRefs) / sizeof(notRefs[0]); ++i)
        if (AI_b_IsARef(notRefs[i]))
            failn("this type should not be a reference", notRefs[i], 0);
}

static void check_resolver_rules(void)
{
    const int refType = 39;          /* a reference, but not the actor one */
    const int actorType = 40;        /* the one that reserves 1 and 2 */

    LOAMOCK_ForgetKeys();

    /* Nought, one and two have to be handed straight back, and the
       only way to show that is to make the lookup able to answer them
       and check that it is not asked. With nothing registered against
       them the lookup misses and returns the key anyway, so the guards
       and their absence give the same answer and the check is empty --
       which is what a mutation sweep found here.

       So: each of the three is given a position holding a value that
       is not the key, and the answer still has to be the key. */
    LOAMOCK_SetKeyPos(0u, 0x61000);
    LOA_AddPosAddress(0x61000u, 0x111u);
    LOAMOCK_SetKeyPos(1u, 0x62000);
    LOA_AddPosAddress(0x62000u, 0x222u);
    LOAMOCK_SetKeyPos(2u, 0x63000);
    LOA_AddPosAddress(0x63000u, 0x333u);

    /* Nought is no object, whatever the type, and is never looked up. */
    if (AI_ul_ResolveOneRef(refType, 0) != 0)
        failn("nought did not stay nought",
              AI_ul_ResolveOneRef(refType, 0), 0);
    if (AI_ul_ResolveOneRef(actorType, 0) != 0)
        failn("nought did not stay nought for the actor type",
              AI_ul_ResolveOneRef(actorType, 0), 0);

    /* One and two are the two main actors for the actor type, and are
       handed back rather than looked up. This is the rule that makes
       those two values unusable as handles. */
    if (AI_ul_ResolveOneRef(actorType, 1) != 1)
        failn("the first main actor was not passed through",
              AI_ul_ResolveOneRef(actorType, 1), 1);
    if (AI_ul_ResolveOneRef(actorType, 2) != 2)
        failn("the second main actor was not passed through",
              AI_ul_ResolveOneRef(actorType, 2), 2);

    /* For any other reference type they are ordinary keys, so they do
       go to the lookup -- which is the other half of the same rule,
       and what says the reservation belongs to the actor type alone. */
    if (AI_ul_ResolveOneRef(refType, 1) != 0x222)
        failn("a key of one was not resolved for a non-actor type",
              AI_ul_ResolveOneRef(refType, 1), 0x222);
    if (AI_ul_ResolveOneRef(refType, 2) != 0x333)
        failn("a key of two was not resolved for a non-actor type",
              AI_ul_ResolveOneRef(refType, 2), 0x333);

    /* A key the lookup does not know is left as it is. */
    if (AI_ul_ResolveOneRef(refType, 0xABCDEF) != 0xABCDEF)
        fail("an unknown key was not left as it was");

    /* A type that is not a reference is never resolved at all, even
       when the key is one the lookup knows. */
    LOAMOCK_SetKeyPos(0x600000u, 0x60000);
    LOA_AddPosAddress(0x60000u, 0x7Cu);
    if (AI_ul_ResolveOneRef(38, 0x600000) != 0x600000)
        fail("a key of a non-reference type was resolved anyway");

    /* And a key the lookup does know, of a type that is a reference,
       comes back as what the store holds. */
    if (AI_ul_ResolveOneRef(refType, 0x600000) != 0x7C)
        failn("a known key of a reference type",
              AI_ul_ResolveOneRef(refType, 0x600000), 0x7C);

    /* A key whose position is known but holds nothing: the store
       answers -1 and that is what comes back. */
    LOAMOCK_SetKeyPos(0x700000u, 0x70000);
    if (AI_ul_ResolveOneRef(refType, 0x700000) != LOA_NOT_FOUND)
        failn("a key whose position holds nothing",
              AI_ul_ResolveOneRef(refType, 0x700000), LOA_NOT_FOUND);
}

/* ------------------------------------------------------------------ */
/* An address through a four-byte slot                                */

static void check_pointer_round_trip(void)
{
    /* Four things at addresses this build really uses. They are only
       ever compared and resolved, never read through, but they are
       real objects so that nothing here depends on an invented
       address being acceptable. */
    static int things[4];
    unsigned i;

    for (i = 0; i < 4; ++i)
    {
        void* thing = &things[i];
        unsigned int pos = 0x80000u + i * 0x1000u;
        uint32_t key = 0x900000u + i;
        uint32_t stored;
        void* back;

        /* What a loader does: record where the thing went. */
        LOA_AddPosAddress(pos, HDL_ul_Handle(thing));
        LOAMOCK_SetKeyPos(key, (int)pos);

        /* What a reference to it resolves to, through the four-byte
           slot the file formats fix. */
        stored = (uint32_t)AI_ul_ResolveOneRef(39, (int)key);

        if (stored == 0)
        {
            fail("resolving a recorded thing gave no object");
            continue;
        }
        if (stored == (uint32_t)LOA_NOT_FOUND)
        {
            fail("resolving a recorded thing gave not-found");
            continue;
        }
        /* It has to be usable as the key of the store it came out of,
           which is a signed search, and it has to leave the slot tag
           reading as resolved. */
        if ((int32_t)stored <= 0)
            failn("what a slot holds is not a positive signed value", stored, 0);
        if (stored % 4u != 0u)
            failn("what a slot holds is not a multiple of four", stored % 4u, 0);

        back = HDL_p_Pointer(stored);
        if (back != thing)
        {
            printf("  FAIL a thing at %p came back as %p through a "
                   "four-byte slot\n", thing, back);
            ++failures;
        }
    }

    /* Two different things never share a slot value. */
    if (HDL_ul_Handle(&things[0]) == HDL_ul_Handle(&things[1]))
        fail("two things resolve to the same slot value");
}

/* ------------------------------------------------------------------ */
/* The engine's own path: a second reference to a loaded file          */

static int loa_thing;
static int loa_loadCount;

static void* loa_loader(uint32_t position)
{
    (void)position;
    ++loa_loadCount;
    return &loa_thing;
}

/* What the loader does for real, and the one place the store's value
   is read back as an address rather than merely compared.
 *
 * The first reference to a key loads the thing and records where it
 * went. A second reference to the same key does not load it again: it
 * takes the recorded value and hands that back as the address. So the
 * second reference is where a value too narrow to hold an address
 * shows up, and it is the check that could not pass before the store
 * held handles. */
static void check_second_reference(void)
{
    const uint32_t key = 0xA10000u;
    const unsigned int pos = 0xA1000u;
    void* first = NULL;
    void* second = NULL;

    LOAMOCK_ForgetKeys();
    LOAMOCK_SetKeyPos(key, (int)pos);
    loa_loadCount = 0;

    LOA_MakeFileRef((int)key, &first, loa_loader, 0);
    LOA_Resolve();
    if (first != &loa_thing)
    {
        printf("  FAIL the first reference gave %p, wanted %p\n",
               first, (void*)&loa_thing);
        ++failures;
        return;
    }
    if (loa_loadCount != 1)
        failn("times the loader ran for the first reference", loa_loadCount, 1);

    LOA_MakeFileRef((int)key, &second, loa_loader, 0);
    LOA_Resolve();

    if (loa_loadCount != 1)
        failn("times the loader ran in total -- the second reference "
              "should have come from the store", loa_loadCount, 1);
    if (second != &loa_thing)
    {
        printf("  FAIL the second reference gave %p, wanted %p -- what the "
               "store held did not come back as an address\n",
               second, (void*)&loa_thing);
        ++failures;
    }
}

int main(void)
{
    printf("=== the address store and the reference resolver ===\n");

    /* The store's buckets are sized here; nothing works before it. */
    LOA_BeforeLoading();

    check_store();
    check_delete();
    check_which_types_are_refs();
    check_resolver_rules();
    check_pointer_round_trip();
    check_second_reference();

    if (failures)
    {
        printf("  %d failure(s)\n", failures);
        return 1;
    }
    printf("  all checks passed\n");
    return 0;
}
