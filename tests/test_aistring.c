/* A script's string constants, across a four-byte node field.
 *
 * A compiled AI function keeps its string constants in one block and
 * refers to them from its nodes. A node of the type category whose
 * type is 38 -- `string`, which ai/ai_types.c gives a width of 4 --
 * carries the offset into that block in its own `val`, and
 * AI_ResolveAllFctRef turns that offset into something the interpreter
 * can use. `val` is an int32_t, because that is what one node occupies
 * in a world file.
 *
 * So the resolve has four bytes to put a reference to the block in,
 * and it did this:
 *
 *     currNode->val += (int)aiModel->funcs[i]->localStringConstsData;
 *
 * which is the block's address, truncated. The read side had already
 * been converted and expects a handle --
 *
 *     AI_GetStringFromStack:
 *         if (var.type == AI_gaw_EnumLink[38])
 *             return (char*)HDL_p_Pointer((uint32_t)((tTextGet*)converted)->key);
 *
 * -- so the two halves disagreed, and the pair worked only where the
 * truncation loses nothing and the layer's pass-through happens to
 * return what went in. Every script that formats a string goes through
 * this.
 *
 * This checks the writer against that reader: what the resolve leaves
 * in the node has to resolve back to the string the block holds, and
 * must not be the address with its top half cut off. The handles are
 * given back when the model is freed, which is the other half of the
 * change and the thing a leak would show up in.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "../common.h"
#include "../math.h"
#include "../memory.h"
#include "../handle.h"
#include "../ai.h"
#include "aistring_mocks.h"

void AI_ResolveAllFctRef(tAIModel* aiModel);
void AI_FreeModel(tAIModel* aiModel);

static int failures;

static void fail(const char* what)
{
    printf("  FAIL %s\n", what);
    ++failures;
}

/* ------------------------------------------------------- a tiny model */

#define STRINGS 6

static const char* const texts[STRINGS] = {
    "%d", "hello", "%s/%d", "", "a rather longer format %f %d %s", "x"
};

/* All of it on the mock heap, because AI_FreeModel frees the model, the
   funcs array, each function, its nodes and its string block -- and the
   last check here is that it does. */
static tAIModel* model;
static tAINode* nodes;
static int offsets[STRINGS];
static char* block;

#define NODES (STRINGS + 3)

/* The block the strings live in, and a function whose nodes point into
   it at those offsets. One node that is not a string sits in the
   middle, so the walk has to pick the right ones. */
static void build(void)
{
    size_t total = 0;
    int i;
    int n = 0;

    for (i = 0; i < STRINGS; i++)
    {
        offsets[i] = (int)total;
        total += strlen(texts[i]) + 1;
    }
    block = (char*)MEM_p_Alloc((unsigned int)total);
    for (i = 0; i < STRINGS; i++)
        memcpy(block + offsets[i], texts[i], strlen(texts[i]) + 1);

    nodes = (tAINode*)MEM_p_AllocAligned((int)(NODES * sizeof(tAINode)), 32);
    memset(nodes, 0, NODES * sizeof(tAINode));
    for (i = 0; i < STRINGS; i++)
    {
        nodes[n].category = 1;              /* the type category */
        nodes[n].val2 = 38;                 /* string */
        nodes[n].val = offsets[i];
        ++n;
        if (i == 2)
        {
            /* an int constant, which the resolve must leave alone */
            nodes[n].category = 1;
            nodes[n].val2 = 0x21;           /* int */
            nodes[n].val = 1234;
            ++n;
        }
    }
    nodes[n].category = 5;                  /* a local var, not a type */
    nodes[n].val2 = 0x21;
    nodes[n].val = 7;
    ++n;
    nodes[n].category = 31;                 /* the terminator */

    {
        tAIFunction* func = (tAIFunction*)MEM_p_Alloc(sizeof(tAIFunction));
        tAIFunction** funcs =
            (tAIFunction**)MEM_p_Alloc(sizeof(tAIFunction*));

        memset(func, 0, sizeof(*func));
        func->nodes = nodes;
        func->localStringConstsData = block;
        func->usageCount = 1;
        funcs[0] = func;

        model = (tAIModel*)MEM_p_Alloc(sizeof(tAIModel));
        memset(model, 0, sizeof(*model));
        model->funcs = funcs;
        model->funcNum = 1;
    }
}

/* The check on every other check here.
 *
 * The resolve rewrites each node's category and val2 through
 * AI_gaw_EnumLink, so everything below depends on what those entries
 * are -- and they come from the dispatch tables, which are the mocks'
 * own. If the mocks' funcids drifted from the engine's, this file
 * would still pass while testing something else. So: the string type
 * has to land where ai/ai_types.c puts it, and the type category
 * where ai/ai_categories.c puts it. */
static void check_the_tables_say_what_the_engine_says(void)
{
    printf("-- the ids the resolve and the reader share\n");

    if (AI_gaw_EnumLink[38] != 8)
        printf("  FAIL the string type resolves to %d and ai/ai_types.c "
               "puts 0x26 at index 8\n", AI_gaw_EnumLink[38]);
    if (AI_gaw_EnumLink[38] != 8)
        ++failures;
    if (AI_gaw_EnumLink[1] != 0)
        printf("  FAIL the type category resolves to %d and "
               "ai/ai_categories.c puts funcid 1 at index 0\n",
               AI_gaw_EnumLink[1]);
    if (AI_gaw_EnumLink[1] != 0)
        ++failures;
}

/* The resolve, read back the way AI_GetStringFromStack reads it. */
static void check_the_strings_resolve(void)
{
    int i;
    int n = 0;
    int checked = 0;

    printf("-- what the resolve leaves in each node\n");

    AISMOCK_Reset();
    build();

    if (!((uintptr_t)block >> 32))
        printf("    NOTE the block is below thirty-two bits here, so the "
               "truncated address would also resolve and this proves "
               "less\n");

    AI_ResolveAllFctRef(model);

    for (i = 0; i < STRINGS; i++)
    {
        tAINode* node = &nodes[n++];
        char* got = (char*)HDL_p_Pointer((uint32_t)node->val);
        char* want = block + offsets[i];

        if (node->val2 != AI_gaw_EnumLink[38])
        {
            fail("a string node's type was not remapped");
            return;
        }
        if (got != want)
        {
            printf("  FAIL string %d resolved to %p and lives at %p\n",
                   i, (void*)got, (void*)want);
            ++failures;
        }
        else if (strcmp(got, texts[i]) != 0)
        {
            printf("  FAIL string %d reads as \"%s\"\n", i, got);
            ++failures;
        }
        else
        {
            ++checked;
        }
        /* And not the address with its top half gone, which is what
           the old resolve left. */
        if ((uintptr_t)want >> 32
            && (uint32_t)node->val == (uint32_t)(uintptr_t)want)
        {
            printf("  FAIL string %d holds the truncated address\n", i);
            ++failures;
        }
        if (i == 2)
            ++n;                        /* step over the int constant */
    }

    if (checked != STRINGS)
        fail("not every string was checked");

    /* The int constant beside them is untouched. */
    if (nodes[3].val != 1234)
        fail("the int constant next to a string was resolved as one");
    /* And so is the local-var node, which is not a type node at all. */
    if (nodes[STRINGS + 1].val != 7)
        fail("a node outside the type category was resolved");
}

/* Running it twice changes nothing: the first node's flag says the
   function is done, and without that the offsets would be folded in a
   second time. */
static void check_the_resolve_runs_once(void)
{
    int before[STRINGS];
    int i;
    int n = 0;

    printf("-- resolving the same function again\n");

    for (i = 0; i < STRINGS; i++)
    {
        before[i] = nodes[n++].val;
        if (i == 2)
            ++n;
    }
    AI_ResolveAllFctRef(model);
    n = 0;
    for (i = 0; i < STRINGS; i++)
    {
        if (nodes[n++].val != before[i])
        {
            printf("  FAIL string %d moved on the second resolve\n", i);
            ++failures;
            break;
        }
        if (i == 2)
            ++n;
    }
}

/* Freeing the model gives every handle back, and they stop resolving.
   Without that a world reloaded often enough would run the index space
   down, since the block lands somewhere new each time. */
static void check_the_handles_come_back(void)
{
    int live_before_build;
    int live_after_resolve;
    int held[STRINGS];
    int i;
    int n = 0;

    printf("-- freeing the model\n");

    live_before_build = HDL_i_LiveCount();

    AISMOCK_Reset();
    build();
    AI_ResolveAllFctRef(model);
    live_after_resolve = HDL_i_LiveCount();

    if (live_after_resolve <= live_before_build)
        fail("the resolve took no handles at all");

    for (i = 0; i < STRINGS; i++)
    {
        held[i] = nodes[n++].val;
        if (i == 2)
            ++n;
    }

    AI_FreeModel(model);

    if (HDL_i_LiveCount() != live_before_build)
        printf("  FAIL %d handle(s) were kept after the model was freed\n",
               HDL_i_LiveCount() - live_before_build);
    if (HDL_i_LiveCount() != live_before_build)
        ++failures;

    for (i = 0; i < STRINGS; i++)
        if (HDL_p_Pointer((uint32_t)held[i]))
        {
            fail("a string still resolves after the model was freed");
            break;
        }
}

/* A function that was loaded and never resolved holds offsets, not
   handles, and must be freed without anything being given back.
 *
 * This passes with the release's flag check taken out, and that is not a
 * hole in it: an unresolved node still carries the raw ids a world file
 * holds -- category 1, type 38 -- and the release walk matches the
 * remapped ones the resolve leaves, 0 and 8. So the ids already settle
 * this arm and the flag check is belt-and-braces. It is kept because it
 * says what the release assumes, not because anything here can see it
 * go. */
static void check_an_unresolved_model_frees_clean(void)
{
    int live_before;

    printf("-- freeing a model that was never resolved\n");

    live_before = HDL_i_LiveCount();

    AISMOCK_Reset();
    build();
    AI_FreeModel(model);

    if (HDL_i_LiveCount() != live_before)
        printf("  FAIL the live count moved by %d\n",
               HDL_i_LiveCount() - live_before);
    if (HDL_i_LiveCount() != live_before)
        ++failures;
    if (AISMOCK_liveBlocks)
    {
        printf("  FAIL %d block(s) were left behind\n", AISMOCK_liveBlocks);
        ++failures;
    }
}

int main(void)
{
    printf("=== a script's string constants, across a node's four bytes ===\n");

    AI_InitModule();

    check_the_tables_say_what_the_engine_says();
    check_the_strings_resolve();
    check_the_resolve_runs_once();
    check_the_handles_come_back();
    check_an_unresolved_model_frees_clean();

    if (failures)
    {
        printf("  %d failure(s)\n", failures);
        return 1;
    }
    printf("  all checks passed\n");
    return 0;
}
