/* The same header on a build whose pointers already fit.
 *
 * There is nothing to translate there, so the three entry points are
 * the casts the engine writes today and the table is not compiled at
 * all. That is the whole promise of the 32-bit path, and it is the one
 * that keeps a 32-bit binary exactly as it was -- which matters beyond
 * tidiness, because the differential tests compare engine sources
 * against frozen baselines and a change that moved this path would
 * move every one of them at once.
 *
 * Checked here rather than under -m32, because a host without a
 * 32-bit libc cannot link one, and the path is a property of the
 * header and not of the word size: forcing HDL_TRANSLATES to 0 picks
 * it on any host. The companion check in the runner is the one that
 * needs a real 32-bit compiler, and it asserts the other half -- that
 * handle.c contributes no code and no data there.
 *
 * Addresses are kept under 2^32 so that the identity really is one on
 * a 64-bit host; that is the arithmetic a 32-bit build gets for free.
 */

#include <stdio.h>

#define HDL_TRANSLATES 0
#include "../handle.h"

static int failures;

static void fail(const char* what)
{
    printf("  FAIL %s\n", what);
    ++failures;
}

static int sideEffects;

static void* withSideEffect(void* p)
{
    ++sideEffects;
    return p;
}

int main(void)
{
    /* 0 and 1 and 2 and the not-found value, a tagged slot, and a few
       ordinary ones. */
    static const uint32_t values[] = {
        0u, 1u, 2u, 3u, 4u, 0x12345u, 0x1000u, 0x7FFFFFFCu,
        0x80000000u, 0xDEADBEEFu, 0xFFFFFFFFu
    };
    unsigned i;

    printf("=== the path a 32-bit build takes ===\n");

    if (HDL_TRANSLATES)
    {
        fail("the header did not take the no-translation path");
        return 1;
    }

    for (i = 0; i < sizeof(values) / sizeof(values[0]); ++i)
    {
        uint32_t v = values[i];
        void* p = (void*)(uintptr_t)v;

        if (HDL_ul_Handle(p) != v)
            fail("an address did not come back as the same 32-bit value");
        if (HDL_p_Pointer(v) != p)
            fail("a value did not come back as the same address");
        /* Both directions, composed, for good measure. */
        if (HDL_p_Pointer(HDL_ul_Handle(p)) != p)
            fail("the round trip is not the identity");
    }

    /* Releasing does nothing, but it must still evaluate what it is
       given: written as a cast to void of nothing, an argument with a
       side effect would silently vanish on this build and not on the
       other. */
    HDL_Release(withSideEffect((void*)(uintptr_t)0x1234u));
    if (sideEffects != 1)
        fail("HDL_Release discarded its argument instead of evaluating it");

    if (HDL_i_LiveCount() != 0)
        fail("the live count is not zero where nothing is tracked");

    /* Closing is a no-op and must compile as a statement. */
    HDL_CloseModule();

    if (failures)
    {
        printf("  %d failure(s)\n", failures);
        return 1;
    }
    printf("  all checks passed\n");
    return 0;
}
