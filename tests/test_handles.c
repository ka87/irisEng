/* The 32-bit stand-in for an address.
 *
 * This runs natively here: the harness builds engine sources at the
 * host word size, and on a 64-bit host that is exactly the build the
 * layer exists for. So these are not a model of the problem, they are
 * the problem.
 *
 * What is checked, and why each one is not optional:
 *
 *   round trip      the whole point
 *   stability       the same address must give the same handle, or a
 *                   slot written twice ends up holding two handles to
 *                   one object and releasing one strands the other
 *   distinctness    two objects must not share a handle
 *   shape           a handle has to be a multiple of four in the
 *                   signed range, or the low-bit tag test reads it as
 *                   an unresolved key and the signed key search
 *                   mis-sorts it
 *   transparency    the four reserved values and a tagged slot must
 *                   come back unchanged, so that a caller which tests
 *                   a slot before using it needs to know nothing
 *   release         a released handle resolves to nothing
 *   generation      and keeps resolving to nothing after the slot is
 *                   reused, rather than to whatever took its place
 *   exhaustion      the table's own limit, reached for real
 *
 * The addresses are made up rather than allocated. Nothing in the
 * layer dereferences what it is given, which is itself worth pinning:
 * a handle stands for a bit pattern, so a pointer into the middle of a
 * block works as well as the start of one, and the string-block case
 * needs that.
 */

#include <stdio.h>
#include "../handle.h"

static int failures;

static void fail(const char* what)
{
    printf("  FAIL %s\n", what);
    ++failures;
}

static void failn(const char* what, unsigned long long got, unsigned long long want)
{
    printf("  FAIL %s: got %llu, wanted %llu\n", what, got, want);
    ++failures;
}

/* Somewhere no real allocation would be, and above the 32-bit range so
   that a truncating build could not accidentally pass. */
static void* fake(unsigned long long i)
{
    return (void*)(uintptr_t)(0x7F0000000ULL + i);
}

int main(void)
{
    printf("=== 32-bit stand-ins for addresses ===\n");

#if !HDL_TRANSLATES
    printf("  SKIP: pointers already fit in 32 bits on this build\n");
    return 0;
#else
    {
        enum { N = 4096 };
        uint32_t h[N];
        int i, j;

        /* --- round trip, stability, distinctness, shape ----------- */
        for (i = 0; i < N; ++i)
        {
            /* Not a multiple of anything: an address the layer is
               handed need not be aligned. */
            void* p = fake((unsigned long long)i * 3u + 1u);
            h[i] = HDL_ul_Handle(p);

            if (h[i] == 0)
            {
                fail("a handle was refused with the table nearly empty");
                break;
            }
            if (HDL_p_Pointer(h[i]) != p)
                fail("an address did not survive the round trip");
            if (HDL_ul_Handle(p) != h[i])
                fail("the same address gave two different handles");

            if ((h[i] & 3u) != 0u)
                failn("handle is not a multiple of four", h[i], 0);
            if (h[i] & 0x80000000u)
                failn("handle has the top bit set", h[i], 0);
            if (h[i] <= 2u)
                failn("handle collides with a reserved value", h[i], 3);
            /* The tag test the world and collision code writes. */
            if (((unsigned char)h[i] & 3u) == 1u)
                fail("a handle reads as an unresolved key to the tag test");
        }

        for (i = 0; i < N; ++i)
            for (j = i + 1; j < N; j += 997)   /* a sample, not N^2 */
                if (h[i] == h[j])
                    fail("two addresses share a handle");

        if (HDL_i_LiveCount() != N)
            failn("live count after taking N handles", (unsigned)HDL_i_LiveCount(), N);

        /* --- the reserved values pass straight through ------------ */
        if (HDL_ul_Handle(NULL) != 0u)
            fail("NULL did not give handle 0");
        if (HDL_p_Pointer(0u) != NULL)
            fail("handle 0 did not resolve to NULL");
        if (HDL_ul_Handle((void*)(uintptr_t)1u) != 1u)
            fail("the first main actor's stand-in was not passed through");
        if (HDL_ul_Handle((void*)(uintptr_t)2u) != 2u)
            fail("the second main actor's stand-in was not passed through");
        if (HDL_p_Pointer(1u) != (void*)(uintptr_t)1u)
            fail("1 did not resolve back to 1");
        if (HDL_p_Pointer(2u) != (void*)(uintptr_t)2u)
            fail("2 did not resolve back to 2");
        if (HDL_p_Pointer(0xFFFFFFFFu) != (void*)(uintptr_t)0xFFFFFFFFu)
            fail("the not-found value was not passed through");

        /* --- a tagged slot keeps its bits ------------------------- */
        {
            uint32_t tagged = 0x12345u;        /* low bits 01 */
            if ((tagged & 3u) != 1u)
                fail("the test's own tagged value is not tagged");
            if (HDL_p_Pointer(tagged) != (void*)(uintptr_t)tagged)
                fail("a tagged slot did not keep its bits");
            if (((unsigned char)(uintptr_t)HDL_p_Pointer(tagged) & 3u) != 1u)
                fail("a tagged slot stopped reading as an unresolved key");
        }

        /* --- release, and the generation -------------------------- */
        {
            void* a = fake(1u);                /* the first one taken */
            uint32_t ha = h[0];
            void* b;
            uint32_t hb;
            int before = HDL_i_LiveCount();

            if (HDL_p_Pointer(ha) != a)
                fail("the first handle stopped resolving");

            HDL_Release(a);
            if (HDL_i_LiveCount() != before - 1)
                fail("releasing did not drop the live count");
            if (HDL_p_Pointer(ha) != NULL)
                fail("a released handle still resolves");

            /* The next handle taken should reuse that slot, and the
               old handle must not come back to life pointing at the
               new address. */
            b = fake(0xBEEFu);
            hb = HDL_ul_Handle(b);
            if (hb == 0)
                fail("no handle after a release");
            if ((hb >> HDL_IDX_SHIFT) != (ha >> HDL_IDX_SHIFT))
                fail("the released slot was not reused");
            if (hb == ha)
                fail("the reused slot gave back the very same handle");
            if (HDL_p_Pointer(hb) != b)
                fail("the reused slot does not resolve to its new address");
            if (HDL_p_Pointer(ha) == b)
                fail("a stale handle resolves to whatever took the slot");
            if (HDL_p_Pointer(ha) != NULL)
                fail("a stale handle does not resolve to NULL");

            /* Releasing something that never had a handle, and the
               reserved values, must all be harmless. */
            HDL_Release(fake(0xABCDEFu));
            HDL_Release(NULL);
            HDL_Release((void*)(uintptr_t)1u);
            if (HDL_p_Pointer(hb) != b)
                fail("a stray release disturbed a live handle");
        }

        HDL_CloseModule();
        if (HDL_i_LiveCount() != 0)
            fail("closing did not empty the table");
    }

    /* --- the table's own limit, reached for real ------------------ */
    {
        unsigned long long i;
        uint32_t last = 1;
        unsigned long long issued = 0;

        for (i = 0; i < HDL_MAX_SLOTS + 16ull; ++i)
        {
            last = HDL_ul_Handle(fake(0x1000000ull + i));
            if (last == 0)
                break;
            ++issued;
        }

        /* Index 0 is never issued, so the limit is one short of the
           index space. Past it the answer is 0, which is "no object"
           -- a leak shows up as objects that stop resolving, not as a
           handle that collides with a live one. */
        if (issued != HDL_MAX_SLOTS - 1ull)
            failn("handles issued before the table filled", issued, HDL_MAX_SLOTS - 1ull);
        if (last != 0)
            fail("the table never reported itself full");

        HDL_CloseModule();
    }

    if (failures)
    {
        printf("  %d failure(s)\n", failures);
        return 1;
    }
    printf("  all checks passed\n");
    return 0;
#endif
}
