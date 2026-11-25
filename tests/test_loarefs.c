/* The four loading.h helpers, and the one rule that separates them.
 *
 * A word out of the file sometimes lands in a slot declared as a
 * pointer and sits there until something resolves it. Two kinds do:
 *
 *   a key     is tested against -1, and so sign extends
 *   a marker  is tested against the literal it was written as, and so
 *             zero extends
 *
 * That is not a preference. A sign-extended marker with its top bit set
 * does not compare equal to the literal, because a literal like
 * 0xffffffff or 0xc0de0008 is unsigned in C and zero extends; and a
 * zero-extended key does not compare equal to a pointer of every bit
 * set, which is what -1 becomes.
 *
 * Both halves of that have now cost something. A geometry's MRM magic
 * was compared as a pointer against its literal and never matched, so
 * the loader skipped a block of the file and read everything after it
 * from the wrong offset. And before the key helper existed, the actions
 * differential failed 72 of its 864 cases on the other half, because a
 * zero-extended -1 is not the pointer the guards test against.
 *
 * So this file does not test any one engine source. It tests the rule
 * the conversions all rest on, which is cheap and has teeth where the
 * files themselves often have no coverage at all -- the sprite
 * generator's not-set sentinel is a marker chosen on exactly this
 * ground, and nothing builds the frozen copy of that file.
 */

#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "../common.h"
#include "../math.h"
#include "../loading.h"

static int failures;

static void fail(const char* what)
{
    printf("  FAIL %s\n", what);
    ++failures;
}

/* The check on every other check here: a pointer has to be wider than
   the word, or both helpers are the identity and nothing below can
   tell them apart. */
static void check_a_pointer_is_wider(void)
{
    printf("-- what makes the two differ at all\n");
    if (sizeof(void*) <= sizeof(uint32_t))
        printf("  NOTE a pointer is %d bytes here, so the key and the "
               "marker agree and the checks below pass either way\n",
               (int)sizeof(void*));
}

/* A key: -1 means no reference, and the guards test the slot against a
   pointer of every bit set. */
static void check_a_key(void)
{
    void* slot;
    int i;
    static const int keys[] = { 0, 1, 2, 0x1234, 0x7FFFFFFF, -1, -2,
                                (int)0x80000000, (int)0xC0DE0008 };

    printf("-- a key, which sign extends\n");

    /* Every key comes back as itself. */
    for (i = 0; i < (int)(sizeof(keys) / sizeof(keys[0])); i++)
    {
        slot = (void*)LOA_ref_FromKey(keys[i]);
        if (LOA_i_KeyFromRef(slot) != keys[i])
        {
            printf("  FAIL key %d came back as %d\n", keys[i],
                   LOA_i_KeyFromRef(slot));
            ++failures;
        }
    }

    /* And the one that matters: -1 in a slot is the pointer the guards
       compare against. */
    slot = (void*)LOA_ref_FromKey(-1);
    if (slot != (void*)-1)
        fail("a key of -1 is not the pointer of every bit set, so a "
             "guard written as == (T*)-1 would not fire");
    if (slot == (void*)(uintptr_t)(uint32_t)-1 && sizeof(void*) > 4)
        fail("a key of -1 zero extended, which is the marker rule and "
             "the wrong one here");
}

/* A marker: it is read back as the 32-bit word it was, and compared
   against the literal it was written as. */
static void check_a_marker(void)
{
    void* slot;
    int i;
    static const uint32_t markers[] = { 0, 1, 8, 0x1234, 0x7FFFFFFF,
                                        0x80000000u, 0xC0DE0008u,
                                        0xC0DE2002u, 0xFFFFFF30u,
                                        0xFFFFFFFFu };

    printf("-- a marker, which zero extends\n");

    for (i = 0; i < (int)(sizeof(markers) / sizeof(markers[0])); i++)
    {
        slot = (void*)LOA_ref_FromMarker((int)markers[i]);
        if (LOA_ui_MarkerFromRef(slot) != markers[i])
        {
            printf("  FAIL marker 0x%08x came back as 0x%08x\n",
                   markers[i], LOA_ui_MarkerFromRef(slot));
            ++failures;
        }
    }

    /* The two the engine actually compares against a literal, written
       the way the engine writes them. */
    slot = (void*)LOA_ref_FromMarker((int)0xC0DE0008u);
    if (slot != (void*)0xc0de0008)
        fail("a geometry's MRM magic does not compare equal to the "
             "literal it is tested against");

    slot = (void*)LOA_ref_FromMarker((int)0xFFFFFFFFu);
    if (slot != (void*)0xffffffff)
        fail("the sprite generator's not-set sentinel does not compare "
             "equal to the literal it is tested against");

    /* And the same two written as keys, which is the mistake this rule
       exists to stop. On a build with four-byte pointers they are the
       same value and this says nothing, which is what the note above
       is for. */
    if (sizeof(void*) > 4)
    {
        if ((void*)LOA_ref_FromKey((int)0xC0DE0008u) == (void*)0xc0de0008)
            fail("the MRM magic compares equal to its literal even as a "
                 "key, so this check cannot tell the two apart");
        if ((void*)LOA_ref_FromKey(-1) == (void*)0xffffffff)
            fail("the not-set sentinel compares equal to its literal "
                 "even as a key, so this check cannot tell the two "
                 "apart");
    }
}

/* A marker is also how a small number rides in a pointer slot -- a
   count, a rank, a zone index -- and those have to survive the trip
   whichever way they are read back. */
static void check_a_small_number(void)
{
    int i;

    printf("-- a small number parked in a pointer slot\n");

    for (i = 0; i <= 256; i++)
    {
        void* slot = (void*)LOA_ref_FromMarker(i);

        if ((int)LOA_ui_MarkerFromRef(slot) != i)
        {
            printf("  FAIL %d came back as %d\n", i,
                   (int)LOA_ui_MarkerFromRef(slot));
            ++failures;
            break;
        }
        if ((uint8_t)LOA_ui_MarkerFromRef(slot) != (uint8_t)i)
        {
            printf("  FAIL %d came back wrong as a byte\n", i);
            ++failures;
            break;
        }
        if (!i && slot)
        {
            fail("nought in a slot is not empty, so a guard written as "
                 "if (slot) would fire on it");
            break;
        }
        if (i && !slot)
        {
            printf("  FAIL %d in a slot reads as empty\n", i);
            ++failures;
            break;
        }
    }
}

int main(void)
{
    printf("=== the file-reference helpers, and the rule behind them ===\n");

    check_a_pointer_is_wider();
    check_a_key();
    check_a_marker();
    check_a_small_number();

    if (failures)
    {
        printf("  %d failure(s)\n", failures);
        return 1;
    }
    printf("  all checks passed\n");
    return 0;
}
