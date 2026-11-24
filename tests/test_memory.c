/* The engine's own allocator.
 *
 * memory.c carves one block from the C library and hands out pieces of
 * it, keeping a block's size and a free block's chain links in words
 * inside the arena itself. Nothing covered it until now, which made it
 * both the most dangerous file to change and the one most needing a
 * change: it keeps the arena's base, the bump pointer and a free
 * block's links in 32-bit words, so on a build with wider pointers it
 * loses the top half of its own base before it does anything else.
 *
 * So this is written to serve twice. Against the allocator as it
 * stands it is a characterisation test, run with the arena forced
 * below 4 GB where the 32-bit words are wide enough -- that is what
 * the runner's "low" mode is, and it has to pass now and keep passing
 * after any widening. Against the same allocator with the arena where
 * a 64-bit build really puts it, it is the demonstration of the
 * defect, and the runner says so rather than failing.
 *
 * What is asserted is what any allocator owes its caller, not what
 * this one happens to do: a block is writable to the size asked for,
 * two live blocks never overlap, and what was written to a block is
 * still there after other blocks have come and gone. Those hold for
 * any correct implementation, so a rewrite that keeps them is free to
 * change everything else. The few checks that do pin this allocator's
 * own arrangements -- the stored size, the hole count -- say so.
 */

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "../memory.h"

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

/* ------------------------------------------------------------------ */
/* Live blocks, so that overlap and corruption can be checked at all. */

#define TRACKED 512

typedef struct Live
{
    unsigned char* ptr;
    unsigned int size;
    unsigned char pattern;
} Live;

static Live live[TRACKED];
static int liveNum;

static unsigned char pattern_for(int i)
{
    /* Never 0, so that a block left zeroed is not mistaken for one
       holding its pattern. */
    return (unsigned char)(1u + (unsigned)i % 255u);
}

static void fill(Live* l)
{
    memset(l->ptr, l->pattern, l->size);
}

static int holds(const Live* l)
{
    unsigned int i;
    for (i = 0; i < l->size; ++i)
        if (l->ptr[i] != l->pattern)
            return 0;
    return 1;
}

/* Does this block overlap any other live one? */
static int overlaps_any(const unsigned char* p, unsigned int size, int skip)
{
    int i;
    for (i = 0; i < liveNum; ++i)
    {
        if (i == skip || !live[i].ptr)
            continue;
        if (p < live[i].ptr + live[i].size && live[i].ptr < p + size)
            return 1;
    }
    return 0;
}

static int take(unsigned int size)
{
    unsigned char* p = (unsigned char*)MEM_p_Alloc(size);
    int slot;

    if (!p)
        return -1;
    if (liveNum >= TRACKED)
    {
        fail("the driver ran out of room to track blocks");
        return -1;
    }
    if (overlaps_any(p, size, -1))
        fail("a fresh block overlaps one that is still live");

    slot = liveNum++;
    live[slot].ptr = p;
    live[slot].size = size;
    live[slot].pattern = pattern_for(slot);
    fill(&live[slot]);
    return slot;
}

static void drop(int slot)
{
    if (slot < 0 || !live[slot].ptr)
        return;
    MEM_Free(live[slot].ptr);
    live[slot].ptr = NULL;
}

static void check_all_live(const char* when)
{
    int i;
    for (i = 0; i < liveNum; ++i)
    {
        if (!live[i].ptr)
            continue;
        if (!holds(&live[i]))
        {
            printf("  FAIL %s: block %d (%u bytes at %p) no longer holds "
                   "what was written to it\n",
                   when, i, live[i].size, (void*)live[i].ptr);
            ++failures;
            return;                       /* one report is enough */
        }
    }
}

/* ------------------------------------------------------------------ */

static void check_basics(void)
{
    void* p;
    unsigned char* q;

    if (!MEM_gst_MemoryInfo.memoryStartPointer)
        fail("the arena has no base after init");
    if (MEM_gst_MemoryInfo.memorySize <= 0)
        fail("the arena has no size after init");

    p = MEM_p_Alloc(100);
    if (!p)
    {
        fail("a 100-byte request failed on an empty arena");
        return;
    }
    /* A block is at least as big as it was asked to be. The exact
       figure is this allocator's own: requests are rounded up to a
       size class, and a class is rounded up again to a whole number of
       the words the allocator keeps its bookkeeping in -- which is
       what lets a freed block put a footer at a word offset from its
       chain link. Both are checked, and neither pins a width. */
    if (MEM_GetBlockSizeBySize(p) < 100)
        failn("the block handed out for a 100-byte request",
              MEM_GetBlockSizeBySize(p), 100);
    if ((unsigned int)MEM_GetBlockSizeBySize(p) % MEM_WORD_BYTES != 0)
        failn("a block's size is not a whole number of words",
              (unsigned int)MEM_GetBlockSizeBySize(p) % MEM_WORD_BYTES, 0);

    q = (unsigned char*)p;
    memset(q, 0x5A, 100);
    if (q[0] != 0x5A || q[99] != 0x5A)
        fail("a block is not writable to the size asked for");

    if (MEM_b_CheckBlockHasBeenFreed(p))
        fail("a live block is reported as freed");

    MEM_Free(p);
}

static void check_churn(void)
{
    static const unsigned int sizes[] = {
        8, 9, 15, 16, 17, 31, 40, 64, 100, 127, 128, 200, 333, 512,
        1000, 1024, 2048, 4095, 8192
    };
    const int n = (int)(sizeof(sizes) / sizeof(sizes[0]));
    int i, slot;
    int taken = 0;

    /* Enough blocks, of enough different sizes, to put something in
       most of the allocator's size classes. */
    for (i = 0; i < 300; ++i)
    {
        slot = take(sizes[i % n] + (unsigned)(i % 7));
        if (slot < 0)
        {
            fail("an allocation failed with the arena nearly empty");
            return;
        }
        ++taken;
    }
    check_all_live("after filling");

    /* Give every third one back, then ask for as many again. The
       returned blocks go on the free lists, so most of what follows
       is reuse rather than fresh arena -- which is the path that
       writes links into the blocks themselves. */
    for (i = 0; i < taken; i += 3)
        drop(i);
    check_all_live("after freeing every third block");

    for (i = 0; i < 100; ++i)
    {
        slot = take(sizes[i % n] + (unsigned)(i % 5));
        if (slot < 0)
        {
            fail("an allocation failed after blocks had been returned");
            return;
        }
    }
    check_all_live("after reusing freed blocks");

    /* And in the other order: free the rest, then refill. */
    for (i = 1; i < taken; i += 3)
        drop(i);
    for (i = 0; i < 60; ++i)
        if (take(sizes[i % n]) < 0)
        {
            fail("an allocation failed on a heavily fragmented arena");
            return;
        }
    check_all_live("after a second round of reuse");
}

static void check_hole_reuse(void)
{
    int before, after, bytesBefore, bytesAfter;
    void* a;
    void* b;

    MEM_GetHolesStat(&before, &bytesBefore);

    a = MEM_p_Alloc(300);
    if (!a)
    {
        fail("a 300-byte request failed");
        return;
    }
    /* Allocated after it, so that freeing a does not simply wind the
       bump pointer back and leave no hole at all. */
    b = MEM_p_Alloc(300);
    if (!b)
    {
        fail("the second 300-byte request failed");
        MEM_Free(a);
        return;
    }

    MEM_Free(a);
    MEM_GetHolesStat(&after, &bytesAfter);
    if (after != before + 1)
        failn("holes after freeing a block that is not the last one",
              after, before + 1);
    if (bytesAfter <= bytesBefore)
        fail("the bytes held in holes did not grow when a block was freed");

    /* The same size again should come out of that hole. */
    a = MEM_p_Alloc(300);
    if (!a)
        fail("a request the same size as the hole just made failed");
    MEM_GetHolesStat(&after, &bytesAfter);
    if (after != before)
        failn("holes after the hole was taken again", after, before);

    MEM_Free(a);
    MEM_Free(b);
}

/* Several holes in one size class, which is the only thing that walks
   the chain a freed block keeps in its own first word. One hole proves
   nothing: with the link dropped the newest free block still becomes
   the head of a list of one, and a count of holes, a reuse and a
   pattern check all come out right while every earlier hole has been
   lost. */
static void check_hole_chain(void)
{
    enum { CHAIN = 6 };
    void* p[CHAIN + 1];
    void* again[CHAIN];
    int before, after, bytes;
    int i;

    MEM_GetHolesStat(&before, &bytes);

    /* One more than are freed, so the last is never the block the bump
       pointer could simply be wound back over. */
    for (i = 0; i < CHAIN + 1; ++i)
    {
        p[i] = MEM_p_Alloc(700);
        if (!p[i])
        {
            fail("a 700-byte request failed while building a hole chain");
            return;
        }
    }
    for (i = 0; i < CHAIN; ++i)
        MEM_Free(p[i]);

    MEM_GetHolesStat(&after, &bytes);
    if (after != before + CHAIN)
        failn("holes after freeing a run of same-sized blocks",
              after, before + CHAIN);

    /* Every one of them has to come back, and none of them twice. */
    for (i = 0; i < CHAIN; ++i)
    {
        again[i] = MEM_p_Alloc(700);
        if (!again[i])
        {
            failn("the hole chain gave back only this many blocks", i, CHAIN);
            return;
        }
        memset(again[i], 0xD7, 700);
    }
    for (i = 0; i < CHAIN; ++i)
    {
        int j;
        for (j = i + 1; j < CHAIN; ++j)
            if (again[i] == again[j])
                fail("the hole chain handed the same block out twice");
    }
    MEM_GetHolesStat(&after, &bytes);
    if (after != before)
        failn("holes after the whole chain was taken again", after, before);

    for (i = 0; i < CHAIN; ++i)
        MEM_Free(again[i]);
    MEM_Free(p[CHAIN]);
}

static void check_realloc(void)
{
    unsigned char* p;
    unsigned char* grown;
    int i;

    p = (unsigned char*)MEM_p_Alloc(200);
    if (!p)
    {
        fail("a 200-byte request failed");
        return;
    }
    for (i = 0; i < 200; ++i)
        p[i] = (unsigned char)(i & 0xFF);

    grown = (unsigned char*)MEM_p_Realloc(p, 800);
    if (!grown)
    {
        fail("growing a block to 800 bytes failed");
        return;
    }
    for (i = 0; i < 200; ++i)
        if (grown[i] != (unsigned char)(i & 0xFF))
        {
            failn("a grown block lost the byte at", i, -1);
            break;
        }
    /* Writable to the new size. */
    memset(grown, 0x3C, 800);
    if (grown[799] != 0x3C)
        fail("a grown block is not writable to its new size");

    MEM_Free(grown);
}

static void check_aligned(void)
{
    static const int alignments[] = { 16, 32, 64, 128, 256 };
    const int n = (int)(sizeof(alignments) / sizeof(alignments[0]));
    void* p[5];
    int i;

    for (i = 0; i < n; ++i)
    {
        p[i] = MEM_p_AllocAligned(500, alignments[i]);
        if (!p[i])
        {
            failn("an aligned request failed for alignment", alignments[i], 0);
            continue;
        }
        if ((uintptr_t)p[i] % (uintptr_t)alignments[i] != 0)
            failn("an aligned block is not aligned to", alignments[i], 0);
        memset(p[i], 0x7E, 500);
    }

    /* The size query for aligned blocks follows the back-pointer this
       allocator writes in the word before the payload, so it is both a
       check on the query and on that pointer surviving. */
    for (i = 0; i < n; ++i)
    {
        if (!p[i])
            continue;
        if (MEM_MemBlockSizeGetAligned(p[i]) < 500)
            failn("the reported size of an aligned block",
                  MEM_MemBlockSizeGetAligned(p[i]), 500);
    }

    for (i = 0; i < n; ++i)
        if (p[i])
        {
            if (((unsigned char*)p[i])[499] != 0x7E)
                fail("an aligned block lost what was written to it");
            MEM_FreeAlign(p[i]);
        }
}

static void check_too_big(void)
{
    void* p;

    /* Larger than the arena. The answer has to be nothing, and the
       blocks already handed out have to be untouched by the attempt. */
    p = MEM_p_Alloc((unsigned int)MEM_gst_MemoryInfo.memorySize);
    if (p)
        fail("a request larger than the whole arena was granted");
    check_all_live("after a request larger than the arena");
}

int main(void)
{
    unsigned long long base;

    printf("=== the engine's allocator ===\n");

    MEM_InitModule();
    base = (unsigned long long)MEM_gst_MemoryInfo.memoryStartPointer;
    printf("  arena base %#llx, %d bytes\n",
           base, MEM_gst_MemoryInfo.memorySize);

    check_basics();
    check_churn();
    check_hole_reuse();
    check_hole_chain();
    check_realloc();
    check_aligned();
    check_too_big();

    if (failures)
    {
        printf("  %d failure(s)\n", failures);
        return 1;
    }
    printf("  all checks passed\n");
    return 0;
}
