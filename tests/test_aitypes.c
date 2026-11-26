/* Every type's copier against the width the table declares for it.
 *
 * AI_gast_Types pairs a script type with two things: the number of
 * bytes a value of it occupies, dataTypeSize, and the function that
 * copies one. Both are used, and separately:
 *
 *   - the copier is what moves a value, in AI_PutObjectToStack,
 *     AI_GetVarFromStack, the two AI_PutToUserStack directions and
 *     the assignment keywords;
 *   - dataTypeSize is what the user stack advances by
 *     (AI_gul_SizeUserStack += ... on the way in, -= on the way out)
 *     and what an array of that type is strided by
 *     (data[dataTypeSize * num + 4] and the two-dimensional form).
 *
 * So a copier that writes more bytes than the declared size runs past
 * the value it was given: on the user stack it writes into whatever
 * the next push will use, and in an array it writes into the next
 * element. Nothing in the engine compares the two, and nothing could
 * -- one is a number in a table and the other is a function.
 *
 * This measures it. Each copier is handed a source of distinct bytes
 * and a destination filled with a known pattern, twice with different
 * patterns so a byte that happens to match is not read as untouched,
 * and the highest byte it changed is the width it actually writes.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "../common.h"
#include "../math.h"
#include "../handle.h"
#include "../ai.h"
#include "../gameobject.h"

void AITYPES_MockActors(void);

static int failures;

/* Room either side of the window the copier is allowed, so a write
   past the end lands somewhere this can see. */
#define PAD 64

/* The ids the table carries, in order. A tripwire: if an entry is
   added, removed or renumbered, the widths below are being checked
   against a different table than the one this file was written for. */
static const int expect_ids[23] = {
    0x20, 0x21, 0x22, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2A, 0x2B, 0x2C,
    0x2D, 0x2E, 0x7C, 0x7D, 0x7E, 0x7F, 0x80, 0x81, 0x82, 0x83, 0x84,
    0x85
};

static const int NTYPES = (int)(sizeof(expect_ids) / sizeof(expect_ids[0]));

/* The mismatches the engine carries on purpose, because the table it
   reproduces has them.
 *
 * 0x84 is one of the vec family, 0x7C..0x85, and every other member
 * declares twelve bytes and copies a vector. This one declares four
 * and copies a vector too -- the same function as its nine siblings --
 * so the declared size is four and the copier writes twelve. The
 * engine's table is a faithful copy and the line carries the original
 * author's own "? size" beside it.
 *
 * Preserved rather than corrected, as with the raster loop in
 * profiler.c: changing it would be a divergence and the call is not
 * this file's to make. Listed here so the run can be green while the
 * mismatch is on the record, and the run fails if one of these stops
 * mismatching -- a list that keeps entries nothing needs stops being
 * evidence of anything. */
struct known
{
    int id;
    int declared;
    int writes;
    int seen;
};

static struct known known_mismatches[] = {
    { 0x84, 4, 12, 0 },
};

static const int NKNOWN =
    (int)(sizeof(known_mismatches) / sizeof(known_mismatches[0]));

static struct known* known_for(int id)
{
    int i;

    for (i = 0; i < NKNOWN; i++)
        if (known_mismatches[i].id == id)
            return &known_mismatches[i];
    return NULL;
}

/* How many bytes the copier at `i` writes, measured. */
static int bytes_written(int i, unsigned char fill)
{
    unsigned char src[PAD];
    unsigned char dst[PAD];
    int k, last = -1;

    for (k = 0; k < PAD; k++)
        src[k] = (unsigned char)(0xA0 + k);
    memset(dst, fill, sizeof(dst));

    AI_gast_Types[i].funcptr(src, dst);

    for (k = 0; k < PAD; k++)
        if (dst[k] != fill)
            last = k;
    return last + 1;
}

static void check_every_copier_stays_inside_its_width(void)
{
    int i;

    printf("-- %d types, each copier against its declared width\n", NTYPES);

    for (i = 0; i < NTYPES; i++)
    {
        int declared = AI_gast_Types[i].dataTypeSize;
        int a = bytes_written(i, 0x5A);
        int b = bytes_written(i, 0xC3);
        int wrote = a > b ? a : b;

        if (AI_gast_Types[i].funcid != expect_ids[i])
        {
            printf("  FAIL entry %d is id 0x%02X and this file was written "
                   "for 0x%02X\n", i, AI_gast_Types[i].funcid,
                   expect_ids[i]);
            ++failures;
            continue;
        }
        if (declared <= 0 || declared > PAD)
        {
            printf("  FAIL id 0x%02X declares %d bytes\n",
                   AI_gast_Types[i].funcid, declared);
            ++failures;
            continue;
        }
        {
            struct known* k = known_for(AI_gast_Types[i].funcid);

            if (k)
            {
                k->seen = 1;
                if (wrote == declared)
                {
                    printf("  FAIL id 0x%02X is on the known list and no "
                           "longer mismatches; take it off\n",
                           AI_gast_Types[i].funcid);
                    ++failures;
                }
                else if (declared != k->declared || wrote != k->writes)
                {
                    printf("  FAIL id 0x%02X is known to declare %d and "
                           "write %d, and now declares %d and writes %d\n",
                           AI_gast_Types[i].funcid, k->declared, k->writes,
                           declared, wrote);
                    ++failures;
                }
                else
                {
                    printf("  known  id 0x%02X declares %d and copies %d, "
                           "as the original's table does\n",
                           AI_gast_Types[i].funcid, declared, wrote);
                }
                continue;
            }
        }
        if (wrote > declared)
        {
            printf("  FAIL id 0x%02X declares %d bytes and its copier "
                   "writes %d\n", AI_gast_Types[i].funcid, declared, wrote);
            ++failures;
        }
    }

    for (i = 0; i < NKNOWN; i++)
        if (!known_mismatches[i].seen)
        {
            printf("  FAIL id 0x%02X is on the known list and is not in the "
                   "table; take it off\n", known_mismatches[i].id);
            ++failures;
        }
}

/* The main-actor substitution in AI_EvalType_GetGAO, which is the one
   copier that does more than move bytes -- it must still move only the
   four the table gives it. */
static void check_the_gao_copier_substitutes_in_place(void)
{
    uint32_t src, dst;
    int gao_index = -1;
    int i;

    printf("-- the object copier's main-actor substitution\n");

    for (i = 0; i < NTYPES; i++)
        if (AI_gast_Types[i].funcid == 0x28)
            gao_index = i;
    if (gao_index < 0)
    {
        printf("  FAIL there is no 0x28 entry to check\n");
        ++failures;
        return;
    }

    /* An ordinary handle passes through untouched. */
    src = 0x1234 << 10;
    dst = 0;
    AI_gast_Types[gao_index].funcptr(&src, &dst);
    if (dst != src)
    {
        printf("  FAIL a handle came back as 0x%X\n", dst);
        ++failures;
    }

    /* The two the scripts name rather than refer to become handles for
       the actors they name, so that everything downstream holds one
       representation and not two. */
    for (i = 0; i < 2; i++)
    {
        src = (uint32_t)(i + 1);
        dst = 0;
        AI_gast_Types[gao_index].funcptr(&src, &dst);
        if (dst == src)
        {
            printf("  FAIL main actor %d passed through as %u\n", i + 1,
                   dst);
            ++failures;
        }
        else if (HDL_p_Pointer(dst) != (void*)AI_MainActor[i])
        {
            printf("  FAIL main actor %d resolved elsewhere\n", i + 1);
            ++failures;
        }
    }
}

int main(void)
{
    printf("=== each script type's copier against its declared width ===\n");

    /* Without this both actors are NULL, HDL_ul_Handle gives nought for
       each, and the substitution check below compares NULL with NULL and
       passes having tested nothing. */
    AITYPES_MockActors();

    check_every_copier_stays_inside_its_width();
    check_the_gao_copier_substitutes_in_place();

    if (failures)
    {
        printf("  %d failure(s)\n", failures);
        return 1;
    }
    printf("  all checks passed\n");
    return 0;
}
