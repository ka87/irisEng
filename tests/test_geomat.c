/* A material loaded from a buffer, and its multi-texture chain.
 *
 * graphicsobjects/geomaterial.c had no test of any kind and no runner
 * built it, which is how the worst defect of the 64-bit work so far sat
 * there unseen: the multi-texture loader built its chain by taking the
 * address of the material's texture slot and writing a
 * tMultiTextureElemStruct* through it. The slot was an int, so that
 * stored four bytes of an eight-byte pointer into it and the other four
 * into the field above; every read of the chain then sign-extended the
 * low half and followed it. The validator walks the chain inside the
 * loader itself, so loading any type 5 material went straight into a
 * wild dereference.
 *
 * Which means most of this file is an ordinary behavioural check and it
 * has teeth: run it against the slot as an int and it does not get as
 * far as an assertion.
 *
 * One fault here is invisible to this file and wants
 * `./tests/run_geomat.sh --asan`: a type 4 material is the first five
 * fields of tGEOMAT and nothing else, and was allocated to a literal 20
 * bytes, which those five fields outgrow by eight once a pointer is
 * eight. The allocator rounds up, so every assertion below passes with
 * it present.
 *
 * And one is invisible to both, which is worth knowing rather than
 * assuming otherwise. The eight-byte store into the four-byte slot ran
 * into validityFlag, a field inside the material's own allocation, so
 * ASan has nothing to say about it; and the validator assigns that
 * field before anything reads it, so no assertion can see it. The
 * truncated head is the only way it ever showed, and that is what the
 * checks below catch. The range assertion on validityFlag is kept as a
 * cheap guard, not as the thing that sees this.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "../common.h"
#include "../math.h"
#include "../memory.h"
#include "../loading.h"
#include "../textures.h"
#include "../graphicsobjects/gro_common.h"
#include "../graphicsobjects/geomaterial.h"
#include "geomat_mocks.h"

/* The loaders and the destructor are reached only through the interface
   table, so no header declares them. Without these the driver would
   call them implicitly, which returns int and truncates every pointer
   they hand back -- and the -Werror=implicit-function-declaration the
   runners carry cannot say so, because -w disables it. */
struct sGroGeneric* MAT_pst_CreateSingleFromBuffer(struct sGroGeneric*, char**,
                                                   tWorld*);
struct sGroGeneric* MAT_pst_CreateMultiTextureFromBuffer(tGroGeneric*, char**,
                                                         tWorld*);
struct sGroGeneric* MAT_pst_CreateMultiFromBuffer(struct sGroGeneric*, char**,
                                                  tWorld*);
struct sGroGeneric* MAT_pst_CreateMulti(void);
void MAT_DestroyMaterial(struct sGroGeneric* gro);

static int failures;

static void fail(const char* what)
{
    printf("  FAIL %s\n", what);
    ++failures;
}

static void failf(const char* what, long got, long want)
{
    printf("  FAIL %s: got %ld, want %ld\n", what, got, want);
    ++failures;
}

/* ------------------------------------------------------- the world */

#define TEXELEMS    32
#define TEXIDX_BASE 16      /* the index a listed key resolves to */

static tEngTexListElemStruct texElems[TEXELEMS];
static tGroGeneric groSingle;
static tGroGeneric groMultiTex;
static tGroGeneric groMulti;

static void world_init(void)
{
    int i;

    memset(texElems, 0, sizeof(texElems));
    for (i = 0; i < TEXELEMS; i++)
    {
        texElems[i].texKey = i;
        /* a distinct index per element, and not equal to the key, so a
           check cannot pass by reading the key back. It has to stay a
           valid index into this array: MAT_Validate_Single goes
           straight back into the list with whatever came out. */
        texElems[i].textElemIdx = (int16_t)(TEXIDX_BASE + i);
    }

    memset(&TEX_gst_GlobalList, 0, sizeof(TEX_gst_GlobalList));
    TEX_gst_GlobalList.texElems = texElems;
    TEX_gst_GlobalList.maxTexNum = TEXELEMS;
    TEX_gst_GlobalList.actTexElemNum = GMMOCK_KNOWN;

    memset(GRO_gast_Interface, 0, sizeof(GRO_gast_Interface));
    for (i = 0; i < 12; i++)
        GRO_gast_Interface[i].id = i;

    groSingle.intf = &GRO_gast_Interface[3];
    groMultiTex.intf = &GRO_gast_Interface[5];
    groMulti.intf = &GRO_gast_Interface[4];
}

/* ------------------------------------------------- building a buffer */

static char buf[4096];
static char* put;

static void begin(void)
{
    memset(buf, 0, sizeof(buf));
    put = buf;
    GMMOCK_Reset();
}

static void putlong(int32_t v)
{
    memcpy(put, &v, 4);
    put += 4;
}

static void putfloat(float v)
{
    memcpy(put, &v, 4);
    put += 4;
}

static void putshort(uint16_t v)
{
    memcpy(put, &v, 2);
    put += 2;
}

/* The eight fields every single and multi-texture material starts with.
   The seventh is the texture slot: a key for a single material, and
   for a multi-texture one merely non-zero to say a chain follows. */
static void put_header(int32_t slot)
{
    putlong(0x11223344);        /* multiMaterialNum / ambiant colour  */
    putlong(0x55667788);        /* multiMaterialPtr / diffuse colour  */
    putlong(0x0A0B0C0D);        /* solidVertexColor                   */
    putfloat(0.25f);            /* unkFloat1                          */
    putfloat(0.75f);            /* unkFloat2                          */
    putlong(1);                 /* negativeZeroOrOneUnknown           */
    putlong(slot);              /* the texture slot                   */
    putlong(0x7E);              /* validityFlag, which the validator  */
                                /* overwrites                         */
}

/* One multi-texture element. `more` says another follows, and goes in
   the short; `texKey` is this element's texture and goes in the long --
   which is the other way round from what the two fields are named, and
   is the thing the recorded lookups below pin down. */
static void put_elem(int more, int32_t texKey, uint32_t flags,
                     int32_t t1, int32_t t2, uint16_t phong)
{
    putshort((uint16_t)more);
    putshort(phong);
    putlong((int32_t)flags);
    putlong(t1);
    putlong(t2);
    putlong(texKey);
}

/* ------------------------------------------------------- the checks */

/* The check on every other check here. The chain's elements come out of
   malloc, and on this platform that is an address that does not fit in
   thirty-two bits -- so a slot that truncated one could not hold it and
   nothing below would pass by accident. If that ever stops being true
   the behavioural checks go quiet and only --asan still speaks. */
static void check_an_address_does_not_fit_in_the_old_slot(void)
{
    void* p = MEM_p_Alloc(64);

    printf("-- what the slot has to hold\n");
    if (!((uintptr_t)p >> 32))
        fail("a heap address here fits in thirty-two bits, so the checks "
             "below would also pass with the slot truncating it; only "
             "--asan is decisive in that case");
    MEM_Free(p);
}

/* A single material: the slot holds a texture key on the way in and the
   resolved index on the way out. */
static void check_single(void)
{
    tGEOMAT* mat;
    char* readptr;

    printf("-- a single material\n");

    /* a key the list already has */
    begin();
    put_header(5);
    readptr = buf;
    mat = (tGEOMAT*)MAT_pst_CreateSingleFromBuffer(&groSingle, &readptr, NULL);

    if (readptr != buf + 32)
        failf("the loader consumed the wrong number of bytes",
              (long)(readptr - buf), 32);
    if (GMMOCK_askedNum != 1)
        failf("the number of texture lookups", GMMOCK_askedNum, 1);
    else if (GMMOCK_asked[0] != 5)
        failf("the key the loader looked up", GMMOCK_asked[0], 5);
    if (mat->idxOrPtr != TEXIDX_BASE + 5)
        failf("the slot holds the resolved index", (long)mat->idxOrPtr,
              TEXIDX_BASE + 5);
    if (mat->negativeZeroOrOneUnknown != 1)
        fail("the field before the slot did not survive the load");
    if (mat->solidVertexColor != 0x0A0B0C0D)
        fail("solidVertexColor did not survive the load");
    MEM_Free(mat);

    /* a key the list does not have, which is added */
    begin();
    put_header(GMMOCK_KNOWN + 3);
    readptr = buf;
    mat = (tGEOMAT*)MAT_pst_CreateSingleFromBuffer(&groSingle, &readptr, NULL);
    if (GMMOCK_addedNum != 1)
        failf("an unknown key was added to the list", GMMOCK_addedNum, 1);
    if (mat->idxOrPtr != GMMOCK_KNOWN)
        failf("the slot holds the index the list gave back",
              (long)mat->idxOrPtr, GMMOCK_KNOWN);
    MEM_Free(mat);

    /* no texture at all: -1 is left alone and nothing is looked up */
    begin();
    put_header(-1);
    readptr = buf;
    mat = (tGEOMAT*)MAT_pst_CreateSingleFromBuffer(&groSingle, &readptr, NULL);
    if (GMMOCK_askedNum != 0)
        failf("a material with no texture looked one up", GMMOCK_askedNum, 0);
    if (mat->idxOrPtr != -1)
        failf("the no-texture slot", (long)mat->idxOrPtr, -1);
    MEM_Free(mat);
}

/* The chain. This is the one the int slot could not survive. */
static void check_the_chain(int n)
{
    tGEOMAT* mat;
    tMultiTextureElemStruct* elem;
    char* readptr;
    int i;
    int walked;

    printf("-- a multi-texture chain of %d\n", n);

    begin();
    put_header(1);
    for (i = 0; i < n; i++)
    {
        /* a different mapping mode and blend per element, so the
           validator has something to do at each one, and transforms
           that it leaves alone */
        put_elem(i + 1 < n ? i + 1 : 0,         /* another follows      */
                 i < GMMOCK_KNOWN ? i : -1,     /* this one's texture   */
                 0x00010000u | (uint32_t)((i % 7) << 12) | 0x400u,
                 0x40000000 + i, 0x40800000 + i,
                 (uint16_t)(0x1000 + i));
    }
    readptr = buf;
    mat = (tGEOMAT*)MAT_pst_CreateMultiTextureFromBuffer(&groMultiTex,
                                                         &readptr, NULL);

    /* two shorts and four longs an element */
    if (readptr != buf + 32 + n * 20)
        failf("the loader consumed the wrong number of bytes",
              (long)(readptr - buf), 32 + n * 20);

    /* The slot holds an address, and it is the first element's. */
    elem = (tMultiTextureElemStruct*)mat->idxOrPtr;
    if (!elem)
    {
        fail("the chain's head is not in the slot at all");
        return;
    }
    if ((uintptr_t)elem != (uintptr_t)mat->idxOrPtr)
        fail("the slot does not hold the whole address");

    /* The field above the slot holds what the validator put there and
       not the top half of that address. */
    if ((unsigned int)mat->validityFlag > 0xFF)
        failf("validityFlag is not a mask", (long)mat->validityFlag, 0xFF);

    /* Every element, in order, with the fields the buffer gave it. */
    walked = 0;
    for (; elem; elem = elem->nextTextureKey)
    {
        i = walked;
        if (i >= n)
        {
            fail("the chain is longer than the buffer described");
            return;
        }
        if (elem->phongOrPlanarParam != (uint16_t)(0x1000 + i))
            failf("an element's second short", elem->phongOrPlanarParam,
                  0x1000 + i);
        if (elem->subTextureFlags
            != (0x00010000u | (unsigned int)((i % 7) << 12) | 0x400u))
            fail("an element's flags");
        if (elem->transform2 != 0x40800000 + i)
            fail("an element's second transform");
        /* The resolved index, which the loader writes over the marker
           the short carried in. */
        if (i < GMMOCK_KNOWN)
        {
            if (elem->texElemIdx != (int16_t)(TEXIDX_BASE + i))
                failf("an element's resolved texture index",
                      elem->texElemIdx, TEXIDX_BASE + i);
        }
        else if (elem->texElemIdx < GMMOCK_KNOWN)
        {
            fail("an unlisted texture did not get an added index");
        }
        ++walked;
    }
    if (walked != n)
        failf("the number of elements in the chain", walked, n);

    /* The key came out of the long and the marker out of the short. Read
       the other way round the loader would have asked for 1, 2, 3, ..
       and 0 -- which are also valid keys, so only this says which field
       it used. */
    if (GMMOCK_askedNum != n)
        failf("the number of texture lookups", GMMOCK_askedNum, n);
    else
        for (i = 0; i < n; i++)
            if (GMMOCK_asked[i] != (i < GMMOCK_KNOWN ? i : -1))
                failf("the key looked up for an element", GMMOCK_asked[i],
                      i < GMMOCK_KNOWN ? i : -1);

    /* And it all goes back. */
    mat->usageCount = 0;
    MAT_DestroyMaterial((tGroGeneric*)mat);
    if (GMMOCK_liveBlocks != 0)
        failf("blocks still held after the material was destroyed",
              GMMOCK_liveBlocks, 0);
}

/* A chain of nothing: the slot's long is zero, so the loader returns
   before the walk and the slot stays empty. */
static void check_an_empty_chain(void)
{
    tGEOMAT* mat;
    char* readptr;

    printf("-- a multi-texture material with no chain\n");

    begin();
    put_header(0);
    readptr = buf;
    mat = (tGEOMAT*)MAT_pst_CreateMultiTextureFromBuffer(&groMultiTex,
                                                         &readptr, NULL);
    if (readptr != buf + 32)
        failf("the loader read past the header", (long)(readptr - buf), 32);
    if (mat->idxOrPtr != 0)
        fail("the empty slot is not empty");
    if (mat->validityFlag != 0)
        failf("the validator's answer for an empty chain",
              (long)mat->validityFlag, 0);
    mat->usageCount = 0;
    MAT_DestroyMaterial((tGroGeneric*)mat);
    if (GMMOCK_liveBlocks != 0)
        fail("destroying an empty multi-texture material leaked");
}

/* A type 4 material, which is the first five fields of tGEOMAT and
   nothing else. Its children arrive as keys for the loader to resolve
   later, and -1 means no child. */
static void check_multi_material(void)
{
    tGEOMAT* mat;
    char* readptr;
    int i;

    printf("-- a multi material and its children\n");

    begin();
    putlong(3);
    putlong(41);
    putlong(-1);
    putlong(43);
    readptr = buf;
    mat = (tGEOMAT*)MAT_pst_CreateMultiFromBuffer(&groMulti, &readptr, NULL);

    if (readptr != buf + 16)
        failf("the loader consumed the wrong number of bytes",
              (long)(readptr - buf), 16);
    if (mat->multiMaterialNumIfType4ThenAmbiantColor != 3)
        failf("the child count", mat->multiMaterialNumIfType4ThenAmbiantColor, 3);
    if (!mat->multiMaterialPtrIfType4ThenDiffuseColor)
    {
        fail("no room was made for the children");
        return;
    }
    /* -1 is written as nothing and never handed to the resolver. */
    if (GMMOCK_fileRefNum != 2)
        failf("the number of children handed to the resolver",
              GMMOCK_fileRefNum, 2);
    else
    {
        if (GMMOCK_fileRefKeys[0] != 41)
            failf("the first child's key", GMMOCK_fileRefKeys[0], 41);
        if (GMMOCK_fileRefKeys[1] != 43)
            failf("the second child's key", GMMOCK_fileRefKeys[1], 43);
    }
    if (mat->multiMaterialPtrIfType4ThenDiffuseColor[1] != NULL)
        fail("the -1 child is not empty");
    for (i = 0; i < 3; i++)
        (void)mat->multiMaterialPtrIfType4ThenDiffuseColor[i];

    MEM_Free(mat->multiMaterialPtrIfType4ThenDiffuseColor);
    MEM_Free(mat);

    /* And the other allocator for the same five-field object. */
    begin();
    mat = (tGEOMAT*)MAT_pst_CreateMulti();
    if (mat->intf != &GRO_gast_Interface[4])
        fail("a created multi material is not a type 4");
    if (mat->multiMaterialNumIfType4ThenAmbiantColor != 1)
        fail("a created multi material does not have room for one child");
    MEM_Free(mat->multiMaterialPtrIfType4ThenDiffuseColor);
    MEM_Free(mat);
}

int main(void)
{
    printf("=== a material loaded from a buffer ===\n");

    world_init();

    check_an_address_does_not_fit_in_the_old_slot();
    check_single();
    check_an_empty_chain();
    check_the_chain(1);
    check_the_chain(2);
    check_the_chain(5);
    check_the_chain(16);
    check_multi_material();

    if (failures)
    {
        printf("  %d failure(s)\n", failures);
        return 1;
    }
    printf("  all checks passed\n");
    return 0;
}
