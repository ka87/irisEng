/* The key/value array, against a model of what it should hold.
 *
 * basic_array.c is four hundred lines of insert, search and delete over
 * a sorted array, and it had no test of its own. Seven runners link it,
 * so it is exercised a great deal and checked not at all: a mutation
 * that made the delete shift one element too few survived every one of
 * them.
 *
 * That matters more than its size suggests. Every store the 64-bit work
 * converted goes through this file -- a transition, a sound buffer, a
 * voice, a pathfinding visited-set, a sound header -- and the keys it
 * compares are the handles that work put there.
 *
 * One thing it deliberately does not fail on. Both deletes shift the
 * tail with a count that is one element too many, so the memmove reads
 * one element past the last live one -- past the allocation itself when
 * the array is exactly full. The element lands in a slot that
 * --actElemNum drops on the next line, so no behavioural check can see
 * it; `./tests/run_bas.sh --asan` demonstrates it. The defect is older
 * than the 64-bit work (the same overflow, at the same place, with the
 * arithmetic written the old way) and what to do about it has not been
 * decided, so it is reproducible here rather than asserted.
 *
 * So this is a model check rather than a list of cases. The driver
 * keeps its own sorted array of what should be in there, applies the
 * same operation to both, and compares them element by element after
 * every single one. What is asserted is what the array owes its caller
 * and not how it goes about it: the keys are sorted and unique, every
 * key that went in and was not deleted is findable with its own value,
 * and nothing else is.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "../basic_array.h"

static int failures;
static long checks;

static void fail(const char* what)
{
    printf("  FAIL %s\n", what);
    ++failures;
}

/* ------------------------------------------------------------ the model */

#define MODEL_MAX 4096

static struct { int key; int val; } model[MODEL_MAX];
static int modelNum;

static int model_find(int key)
{
    int i;
    for (i = 0; i < modelNum; ++i)
        if (model[i].key == key)
            return i;
    return -1;
}

static int by_key(const void* a, const void* b)
{
    int ka = ((const struct { int key; int val; }*)a)->key;
    int kb = ((const struct { int key; int val; }*)b)->key;
    return (ka > kb) - (ka < kb);
}

static void model_insert(int key, int val)
{
    int at = model_find(key);

    /* A key already there keeps its place and takes the new value. */
    if (at != -1)
    {
        model[at].val = val;
        return;
    }
    if (modelNum >= MODEL_MAX)
    {
        fail("the model ran out of room");
        return;
    }
    model[modelNum].key = key;
    model[modelNum].val = val;
    ++modelNum;
    qsort(model, (size_t)modelNum, sizeof(model[0]), by_key);
}

static void model_delete(int key)
{
    int at = model_find(key);
    if (at == -1)
        return;
    memmove(&model[at], &model[at + 1],
            sizeof(model[0]) * (size_t)(modelNum - at - 1));
    --modelNum;
}

/* ------------------------------------------------- comparing the two */

static int same(const char* where, BASStruct* bas)
{
    int i;

    ++checks;
    if (bas->actElemNum != modelNum)
    {
        printf("  FAIL %s: the array holds %d and the model %d\n",
               where, bas->actElemNum, modelNum);
        ++failures;
        return 0;
    }
    for (i = 0; i < modelNum; ++i)
    {
        if (bas->keyValPtr[i].key != model[i].key)
        {
            printf("  FAIL %s: element %d has key %d and should have %d\n",
                   where, i, bas->keyValPtr[i].key, model[i].key);
            ++failures;
            return 0;
        }
        if (bas->keyValPtr[i].val != model[i].val)
        {
            printf("  FAIL %s: key %d has value %d and should have %d\n",
                   where, model[i].key, bas->keyValPtr[i].val,
                   model[i].val);
            ++failures;
            return 0;
        }
    }
    /* Sorted and unique, which is what the search depends on. */
    for (i = 1; i < bas->actElemNum; ++i)
        if (bas->keyValPtr[i - 1].key >= bas->keyValPtr[i].key)
        {
            printf("  FAIL %s: element %d's key is not above element "
                   "%d's\n", where, i, i - 1);
            ++failures;
            return 0;
        }
    /* And every key answers the search with its own value. */
    for (i = 0; i < modelNum; ++i)
        if (BAS_bsearch(model[i].key, bas) != model[i].val)
        {
            printf("  FAIL %s: the search gave %d for key %d and the "
                   "model has %d\n", where, BAS_bsearch(model[i].key, bas),
                   model[i].key, model[i].val);
            ++failures;
            return 0;
        }
    return 1;
}

/* ------------------------------------------------------------ the cases */

/* A deterministic generator, so a failure is reproducible. */
static uint32_t seed;

static uint32_t next(void)
{
    seed = seed * 1664525u + 1013904223u;
    return seed >> 8;
}

static void forget(BASStruct* bas)
{
    memset(bas, 0, sizeof(*bas));
    modelNum = 0;
}

/* The straightforward shapes first, each of which is a branch in the
   insert and the delete: at the front, at the back, in the middle, the
   only element, and a key that is not there at all. */
static void check_the_shapes(void)
{
    BASStruct bas;
    int val;
    int i;

    printf("-- one element, and the shapes around it\n");

    forget(&bas);
    BAS_binit(&bas, 4);
    same("empty", &bas);

    if (BAS_bsearch(1000, &bas) != -1)
        fail("the search found something in an empty array");

    /* Inserted ascending, so every one goes at the back. */
    for (i = 0; i < 8; ++i)
    {
        BAS_binsert((unsigned int)(100 + 10 * i), 7000 + i, &bas);
        model_insert(100 + 10 * i, 7000 + i);
        same("after an insert at the back", &bas);
    }

    /* Descending, so every one goes at the front. */
    for (i = 0; i < 8; ++i)
    {
        BAS_binsert((unsigned int)(90 - 10 * i), 8000 + i, &bas);
        model_insert(90 - 10 * i, 8000 + i);
        same("after an insert at the front", &bas);
    }

    /* And into the gaps, so every one goes in the middle. */
    for (i = 0; i < 8; ++i)
    {
        BAS_binsert((unsigned int)(105 + 10 * i), 9000 + i, &bas);
        model_insert(105 + 10 * i, 9000 + i);
        same("after an insert in the middle", &bas);
    }

    /* A key already there keeps its place and takes the new value. */
    BAS_binsert(100u, 12345, &bas);
    model_insert(100, 12345);
    same("after inserting a key that is already there", &bas);
    if (BAS_bsearch(100, &bas) != 12345)
        fail("inserting an existing key did not replace its value");

    /* Deleted from the middle, which is the shift this file gets
       wrong when its arithmetic is wrong. */
    BAS_bdelete(105u, &bas);
    model_delete(105);
    same("after a delete from the middle", &bas);

    BAS_bdeletegetval(115, &bas, &val);
    if (val != 9001)
        printf("  FAIL the delete gave back %d and should have 9001\n", val);
    if (val != 9001)
        ++failures;
    model_delete(115);
    same("after a delete that hands the value back", &bas);

    /* From the front and from the back. */
    BAS_bdelete((unsigned int)bas.keyValPtr[0].key, &bas);
    model_delete(model[0].key);
    same("after a delete from the front", &bas);

    BAS_bdelete((unsigned int)bas.keyValPtr[bas.actElemNum - 1].key, &bas);
    model_delete(model[modelNum - 1].key);
    same("after a delete from the back", &bas);

    /* A key that is not there changes nothing, and the value comes
       back as nought.
    
       That nought is this engine as it stands and gold has not been
       read for it, which matters in one place: pcSND_SB_Stop does
       BAS_bdeletegetval(.., &wasListed) and then `if (wasListed)`,
       so nought skips that branch where -1 would take it and
       decrement a count for a buffer that was never listed. The two
       other callers guard with `!= -1` and come out the same either
       way. Pinned here so it cannot drift while the question is
       open. */
    val = 0x5EED;
    BAS_bdeletegetval(999999, &bas, &val);
    if (val != 0)
        printf("  FAIL deleting a key that is not there answered %d and "
               "this engine answers 0\n", val);
    if (val != 0)
        ++failures;
    same("after deleting a key that is not there", &bas);

    /* Down to nothing, one at a time. */
    while (bas.actElemNum)
    {
        BAS_bdelete((unsigned int)bas.keyValPtr[0].key, &bas);
        model_delete(model[0].key);
        same("while emptying the array", &bas);
    }

    BAS_bfree(&bas);
}

/* Insert and delete in a mixture, so the array grows past its first
   block, reuses the room a delete left, and ends up in shapes no list
   of cases would think of. */
static void check_a_long_mixture(int rounds, int keySpan, int initial)
{
    BASStruct bas;
    int i;

    printf("-- %d operations over %d keys, starting with room for %d\n",
           rounds, keySpan, initial);

    forget(&bas);
    BAS_binit(&bas, initial);

    for (i = 0; i < rounds; ++i)
    {
        int key = (int)(next() % (uint32_t)keySpan);
        uint32_t what = next() % 10u;

        if (what < 6u)
        {
            int val = 1 + (int)(next() % 100000u);
            BAS_binsert((unsigned int)key, val, &bas);
            model_insert(key, val);
            if (!same("after an insert", &bas))
                break;
        }
        else if (what < 9u)
        {
            int val = 0x5EED;
            int wanted = model_find(key);

            BAS_bdeletegetval(key, &bas, &val);
            if (wanted == -1)
            {
                if (val != 0)
                {
                    fail("a delete of a key that is not there did not "
                         "answer nought");
                    break;
                }
            }
            else if (val != model[wanted].val)
            {
                printf("  FAIL a delete gave back %d for key %d and the "
                       "model has %d\n", val, key, model[wanted].val);
                ++failures;
                break;
            }
            model_delete(key);
            if (!same("after a delete", &bas))
                break;
        }
        else
        {
            /* A search changes nothing. */
            int got = BAS_bsearch(key, &bas);
            int wanted = model_find(key);

            if (got != (wanted == -1 ? -1 : model[wanted].val))
            {
                printf("  FAIL the search gave %d for key %d\n", got, key);
                ++failures;
                break;
            }
            if (!same("after a search", &bas))
                break;
        }
    }

    BAS_bfree(&bas);
    if (bas.keyValPtr)
        fail("freeing the array left its storage behind");
}

/* The keys this file now compares are handles, which are large and
   sparse, and one of them is every bit set. Sorted order is what the
   search depends on, and the comparison is signed. */
static void check_handle_shaped_keys(void)
{
    BASStruct bas;
    int i;

    printf("-- keys shaped like handles, which is what goes in them now\n");

    forget(&bas);
    BAS_binit(&bas, 4);

    /* (index << 10) | (generation << 2), as the layer issues them. */
    for (i = 1; i <= 64; ++i)
    {
        int key = (i << 10) | ((i & 0xFF) << 2);
        BAS_binsert((unsigned int)key, 4000 + i, &bas);
        model_insert(key, 4000 + i);
        same("after a handle went in", &bas);
    }

    /* The largest key a handle can be, which is the one the sign bit
       would spoil if it were set. */
    {
        int key = (int)((1u << 31) - 4u);
        BAS_binsert((unsigned int)key, 5555, &bas);
        model_insert(key, 5555);
        same("after the largest handle went in", &bas);
    }

    for (i = 1; i <= 64; i += 3)
    {
        int key = (i << 10) | ((i & 0xFF) << 2);
        BAS_bdelete((unsigned int)key, &bas);
        model_delete(key);
        same("after a handle came out", &bas);
    }

    BAS_bfree(&bas);
}

int main(void)
{
    printf("=== the key/value array, against a model ===\n");

    BAS_InitModule();

    check_the_shapes();
    check_handle_shaped_keys();

    seed = 0x1234567u;
    check_a_long_mixture(4000, 40, 4);      /* dense: every key, often */
    check_a_long_mixture(4000, 400, 8);     /* sparse, and it has to grow */
    check_a_long_mixture(2000, 3, 1);       /* almost no keys at all */

    BAS_CloseModule();

    printf("  %ld comparisons against the model\n", checks);
    if (failures)
    {
        printf("  %d failure(s)\n", failures);
        return 1;
    }
    printf("  all checks passed\n");
    return 0;
}
