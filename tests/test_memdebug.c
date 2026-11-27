/* The debugging allocator, memdebug.inc, which had no coverage at all.
 *
 * It is the thing the engine is swapped onto when something is writing
 * over something else: every block becomes its own malloc, so a write
 * past one lands in the C library's guard rather than in the next
 * engine object, and a block handed back twice is named here instead
 * of corrupting a free chain. None of that is worth anything if the
 * allocator is itself wrong, and it was -- in three places, all of
 * them a figure or a slot that was right when a pointer was four
 * bytes.
 *
 * The cases below are the observable end of each one. They do not
 * reach into the file: they ask the two things it publishes, the
 * engine's own byte count and the number of reports, which is all a
 * caller can see and all that has to hold.
 */

#include <stdio.h>
#include <string.h>
#include "../memory.h"

extern unsigned MDBG_gul_Reports;

static int failures;

static void fail(const char* what)
{
    printf("  FAIL %s\n", what);
    ++failures;
}

/* Every block the engine takes is counted in, and every block it gives
   back is counted out, so a run that frees everything ends at zero.
   It did not: an aligned block is a block carved out of another one
   and the inner one is reached through an address kept in front of the
   user pointer. That address was stored as a uint32_t, so at eight
   bytes it came back as its low half, the table did not know it, and
   the inner block was neither freed nor counted out. */
static void check_aligned_round_trip(void)
{
    void* p[16];
    int i;

    printf("-- an aligned block gives back everything it took\n");

    MEM_gst_MemoryInfo.usedMemoryByEngine = 0;
    MDBG_gul_Reports = 0;

    for (i = 0; i < 16; ++i)
    {
        p[i] = MEM_p_AllocAligned(100 + 16 * i, 16);
        if (!p[i])
            fail("an aligned block was refused");
        else if (((size_t)p[i] & 15u) != 0)
            fail("an aligned block came back unaligned");
        else
            memset(p[i], 0x5A, (size_t)(100 + 16 * i));
    }
    for (i = 0; i < 16; ++i)
        MEM_FreeAlign(p[i]);

    if (MDBG_gul_Reports)
        fail("freeing an aligned block was reported as freeing a non-block");
    if (MEM_gst_MemoryInfo.usedMemoryByEngine != 0)
        fail("an aligned block was not counted back out");
}

/* The payload has to start after the header, not inside it. The offset
   was written out as 24, which is what the header measures where a
   pointer is four bytes; at eight it is 32, so the first eight bytes a
   caller wrote landed on the header's own length and magic. The
   temporary allocator is where that shows: it reads the length back
   out of the header to release the block. */
static void check_payload_clears_the_header(void)
{
    void* p[8];
    int i;

    printf("-- what a caller writes does not land on the block's header\n");

    MEM_gst_MemoryInfo.usedMemoryByEngine = 0;
    MDBG_gul_Reports = 0;

    for (i = 0; i < 8; ++i)
    {
        p[i] = MEM_p_AllocTmp(256);
        if (!p[i])
            fail("a temporary block was refused");
        else
            memset(p[i], 0xA5, 256);
    }
    MEM_ResetTmpMemory();

    if (MDBG_gul_Reports)
        fail("releasing the temporary blocks was reported");
    if (MEM_gst_MemoryInfo.usedMemoryByEngine != 0)
        fail("a temporary block was not counted back out");
}

/* And the thing the whole file is for. */
static void check_double_free_is_named(void)
{
    void* p;

    printf("-- a block handed back twice is named\n");

    MDBG_gul_Reports = 0;
    p = MEM_p_Alloc(96);
    if (!p)
        fail("a block was refused");
    MEM_Free(p);
    if (MDBG_gul_Reports)
        fail("an ordinary free was reported");
    MEM_Free(p);
    if (!MDBG_gul_Reports)
        fail("a block handed back twice was taken without a word");
}

int main(void)
{
    printf("=== the debugging allocator ===\n");

    check_aligned_round_trip();
    check_payload_clears_the_header();
    check_double_free_is_named();

    if (failures)
    {
        printf("  %d failure(s)\n", failures);
        return 1;
    }
    printf("  all checks passed\n");
    return 0;
}
