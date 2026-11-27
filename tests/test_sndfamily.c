/* The AND-family list, grown past its first block.
 *
 * SND_pst_GetOrArray keeps a list of sound families and grows it
 * sixteen at a time. Two things about that growth were wrong, and
 * neither can be seen by comparing the rewrite against the frozen copy,
 * because the frozen copy has both:
 *
 *   - the room it asks for is written folded, as
 *     16 * (9 * size + sizeof(tSoundFamilyStruct)). On a 32-bit target
 *     9 * 16 is the struct's own size and the two agree; where the
 *     struct is wider the folded figure is a whole element short per
 *     element, and the list then indexes past what it allocated.
 *   - the clear afterwards starts at the first new element and runs for
 *     that same whole figure, rather than for the sixteen elements the
 *     list just gained -- so it runs off the end by every element the
 *     list already had. That one is wrong on a 32-bit target too.
 *
 * So this file does not compare anything. It grows the list well past
 * the first block and asks, at every step, whether the room the list
 * was given is enough for the families it holds -- which the mock
 * allocator can answer, because it records what each block was asked
 * for.
 *
 * That is the check rather than AddressSanitizer, and the reason is
 * worth stating: the mocks hand every block **twice** what it asks
 * for, deliberately, so that a differential reports a defect instead
 * of dying on it. Under that allocator neither of these faults
 * touches anything it should not, so --asan sees nothing. It is kept
 * as a mode, and it is not what catches these.
 *
 * The second fault, the over-long clear, is not observable here at all
 * and this file does not pretend otherwise: the extra zeros land in
 * the slack that same doubling provides. What says it is wrong is the
 * arithmetic -- the clear starts at the first new element and runs for
 * the whole list's figure -- and that is recorded in the notes rather
 * than asserted here.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "../common.h"
#include "../math.h"
#include "../sound.h"
#include "sound_mocks.h"

#if SND_FAMILY_FROZEN
#error "this is the check on the real growth; build it without SND_FAMILY_FROZEN"
#endif

tSoundFamilyStruct* SND_pst_GetOrArray(int mask, int bits);

static int failures;

static void fail(const char* what)
{
    printf("  FAIL %s\n", what);
    ++failures;
}

/* Enough families to take the list through four growths: the first
   block is sixteen and each growth adds sixteen. */
#define FAMILIES 70

static void check_the_list_grows(void)
{
    tSoundFamilyStruct* got[FAMILIES];
    int i;

    printf("-- %d families, which is four growths past the first block\n",
           FAMILIES);

    SND_gast_AndFamillyList = NULL;
    SND_gi_AndFamillyNb = 0;
    SND_gi_AndFamillyListSize = 0;

    /* A distinct mask each time, so each call makes a new family rather
       than finding one. One bit of `bits` each, so the split-and-share
       path below the creation is not entered. */
    for (i = 0; i < FAMILIES; i++)
    {
        got[i] = SND_pst_GetOrArray(0x10000 + i, 1 << (i & 7));
        if (!got[i])
        {
            fail("a family was not made");
            return;
        }
        if (SND_gi_AndFamillyNb != i + 1)
        {
            printf("  FAIL after %d families the list holds %d\n",
                   i + 1, SND_gi_AndFamillyNb);
            ++failures;
            return;
        }
        if (SND_gi_AndFamillyListSize < SND_gi_AndFamillyNb)
        {
            printf("  FAIL the list holds %d families with room for %d\n",
                   SND_gi_AndFamillyNb, SND_gi_AndFamillyListSize);
            ++failures;
            return;
        }

        /* The one that matters: what the block was asked for, against
           what the list now indexes. */
        {
            int asked = snd_heap_block_request(SND_gast_AndFamillyList);
            size_t needed = sizeof(tSoundFamilyStruct)
                          * (size_t)SND_gi_AndFamillyListSize;

            if (asked < 0)
            {
                fail("the list is not a block this allocator handed out");
                return;
            }
            if ((size_t)asked < needed)
            {
                printf("  FAIL with room for %d families the list asked "
                       "for %d bytes and %d of them need %u\n",
                       SND_gi_AndFamillyListSize, asked,
                       SND_gi_AndFamillyListSize, (unsigned int)needed);
                ++failures;
                return;
            }
        }
    }

    /* Every mask still answers with its own family, and with the one
       the list actually holds at that index -- which is what a growth
       that moved the block has to preserve. */
    for (i = 0; i < FAMILIES; i++)
    {
        tSoundFamilyStruct* again = SND_pst_GetOrArray(0x10000 + i,
                                                       1 << (i & 7));

        if (again != &SND_gast_AndFamillyList[i])
        {
            printf("  FAIL family %d is not the one at index %d\n", i, i);
            ++failures;
            return;
        }
        if (again->requiredMask != 0x10000 + i)
        {
            printf("  FAIL family %d has mask 0x%x\n", i,
                   again->requiredMask);
            ++failures;
            return;
        }
    }
    if (SND_gi_AndFamillyNb != FAMILIES)
        fail("looking the families up again changed how many there are");

    /* The elements the last growth added have to read as empty: that is
       what the clear is for, and clearing too much is what it did
       instead. */
    for (i = SND_gi_AndFamillyNb; i < SND_gi_AndFamillyListSize; i++)
    {
        tSoundFamilyStruct* spare = &SND_gast_AndFamillyList[i];

        if (spare->requiredMask || spare->innerNum || spare->anyMaskUnion
            || spare->unkIntArrPtr)
        {
            printf("  FAIL the spare family at index %d is not empty\n", i);
            ++failures;
            break;
        }
    }
}

/* A free slot is found by stepping the element, not a number.
 *
 * SND_i_AllocSound used to walk the array as words, fourteen at a
 * time, which is the element's size only where a pointer is four.
 * Everywhere else -- SND_l_GetSoundIndex among them -- steps by the
 * element, so the scan read `flags` out of the middle of its
 * neighbours, stopped at the first thing that looked free, and handed
 * the same slot out again. What that looks like from outside is a
 * sound that cannot be found by the key it was just stored under. */
static void check_a_slot_is_found_by_the_element(void)
{
    enum { SOUNDS = 8, KEY0 = 0x7A000 };
    int idx[SOUNDS];
    int i, k;

    printf("-- each sound gets a slot of its own\n");
    for (i = 0; i < SOUNDS; ++i)
    {
        idx[i] = SND_i_AllocSound();
        if (idx[i] < 0 || idx[i] >= SND_gst_Params.allocatedSoundsMaxNum)
        {
            fail("a slot outside the array");
            return;
        }
        /* 0x400 is what marks a slot taken, and what both the scan and
           the lookup test. */
        SND_gst_Params.allocatedSounds[idx[i]].flags = 0x400;
        SND_gst_Params.allocatedSounds[idx[i]].soundKey = KEY0 + i;
    }

    for (i = 0; i < SOUNDS; ++i)
        for (k = 0; k < i; ++k)
            if (idx[k] == idx[i])
            {
                fail("two sounds were handed the same slot");
                return;
            }

    for (i = 0; i < SOUNDS; ++i)
        if (SND_l_GetSoundIndex(KEY0 + i) != idx[i])
        {
            fail("a sound is not where its key says it is");
            return;
        }
}

int main(void)
{
    printf("=== the AND-family list, grown past its first block ===\n");

    check_the_list_grows();
    check_a_slot_is_found_by_the_element();

    if (failures)
    {
        printf("  %d failure(s)\n", failures);
        return 1;
    }
    printf("  all checks passed\n");
    return 0;
}
