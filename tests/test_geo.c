/* A geometry loaded from a buffer, and the markers the loader parks in
 * its pointer slots.
 *
 * graphicsobjects/geo.c had no test of any kind and no runner built it.
 * It is the second file in a row to turn out that way, and the second
 * to have a fault behind it.
 *
 * The loader reads three values out of the file into slots that are
 * declared as pointers and are not addresses yet: the vertex colour
 * count, an MRM magic number, and a per-chunk MRM UV flag.
 * GEO_AllocContent and GEO_AllocElementContent read each one back and
 * replace it with the storage they size from it. The MRM one is not
 * just a flag -- the allocator compares it against 0xc0de0008 and
 * allocates a second per-vertex array when it matches:
 *
 *     if (mrmMagic == (void*)0xc0de0008)
 *         geo->mrm->ptr = MEM_GEO_p_Alloc(..);
 *
 * which does not match where a pointer is eight bytes. The value
 * arrives as a signed int, so the slot holds it sign extended, and the
 * literal on the right is unsigned and zero extended. On a 32-bit
 * target both are 0xc0de0008 and the comparison holds.
 *
 * The consequence is not a missing array. `geo->mrm->ptr` staying NULL
 * makes the loader skip a block of vertexNum shorts, so the read head
 * ends up short by that much and every field after it in the file --
 * the colours, the UVs, every triangle chunk, the sprites -- is read
 * from the wrong place. A geometry with an MRM would load as rubbish
 * and the file would not say so.
 *
 * So the byte count is the strongest assertion here, and it is why
 * check_the_mrm_magic below is written as a pair: the same geometry
 * with the magic and with another non-zero value, which differ by
 * exactly vertexNum shorts and in nothing else.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "../common.h"
#include "../math.h"
#include "../memory.h"
#include "../loading.h"
#include "../graphicsobjects/gro_common.h"
#include "../graphicsobjects/geo.h"
#include "geo_mocks.h"

/* Reached through the interface table, so no header declares it. */
struct sGroGeneric* GEO_p_CreateFromBuffer(struct sGroGeneric* gro,
                                           char** readPtr,
                                           struct sWorld* world);
void GEO_Free(struct sGroGeneric* geo);

extern int GEO_b_IsInFix;

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

/* ------------------------------------------------- building a geometry */

#define MRM_MAGIC_WITH_PTR 0xc0de0008

static char buf[65536];
static char* put;

static void putlong(int32_t v)  { memcpy(put, &v, 4); put += 4; }
static void putfloat(float v)   { memcpy(put, &v, 4); put += 4; }
static void putshort(uint16_t v){ memcpy(put, &v, 2); put += 2; }

struct shape
{
    int vertexNum;
    int colourCount;    /* the marker, not the array                   */
    int uvNum;
    int chunkCount;
    uint32_t mrmMagic;  /* 0 for none                                  */
    int uvIdxFlag;      /* per chunk, 0 for none                       */
    int triangleNum;    /* per chunk                                   */
};

/* Lays the shape out the way GEO_p_CreateFromBuffer reads it: no
   skinning, no collision, no triangle strips and no sprites, so flags
   and unkFlags are both nought. */
static void build(const struct shape* s)
{
    int i;
    int c;

    memset(buf, 0, sizeof(buf));
    put = buf;
    GEOMOCK_Reset();

    putlong(s->vertexNum);
    putlong(s->colourCount);
    putlong(s->uvNum);
    putlong(s->chunkCount);
    putlong(0x1BADF00D);                /* skipped by the loader       */
    putlong((int32_t)s->mrmMagic);
    putlong(0);                         /* flags                       */

    for (i = 0; i < s->vertexNum; i++)  /* vertices                    */
    {
        putfloat((float)i);
        putfloat((float)i + 0.5f);
        putfloat((float)i + 0.25f);
    }
    for (i = 0; i < s->vertexNum; i++)  /* skipped by the loader       */
    {
        putfloat(-1.0f); putfloat(-1.0f); putfloat(-1.0f);
    }

    if (s->mrmMagic)
    {
        for (i = 0; i < s->vertexNum; i++)
            putshort((uint16_t)(0x100 + i));
        if (s->mrmMagic == MRM_MAGIC_WITH_PTR)
            for (i = 0; i < s->vertexNum; i++)
                putshort((uint16_t)(0x200 + i));
        putlong(0x5A5A);                /* mrm->b                      */
        for (i = 0; i < 8; i++)
            putfloat((float)i * 2.0f);  /* mrm->f                      */
    }

    /* the colours, as many as the smaller of the two counts */
    for (i = 0; i < (s->colourCount < s->vertexNum
                     ? s->colourCount : s->vertexNum); i++)
        putlong(0xC0100 + i);

    for (i = 0; i < s->uvNum; i++)      /* UVs                         */
    {
        putfloat((float)i * 0.125f);
        putfloat((float)i * 0.25f);
    }

    for (c = 0; c < s->chunkCount; c++) /* the chunk headers           */
    {
        putlong(s->triangleNum);
        putlong(7 + c);                 /* materialNum                 */
        putlong(s->uvIdxFlag);
        putlong(0);                     /* MRMUVIndicesNum             */
        for (i = 0; i < 6; i++)
            putlong(0);                 /* skipped                     */
    }

    for (c = 0; c < s->chunkCount; c++) /* the chunk contents          */
    {
        for (i = 0; i < s->triangleNum; i++)
        {
            putshort(0); putshort(1); putshort(2);          /* v1..v3  */
            putshort(0); putshort(1); putshort(2);          /* uv1..3  */
            putlong(0x30 + i);                              /* smooth  */
            putlong(0x40 + i);                              /* unk2    */
        }
        if (s->uvIdxFlag)
        {
            putlong(0x11 + c);          /* UVStartOffset               */
            putlong(0x22 + c);          /* i2                          */
            putlong(0);                 /* skipped                     */
        }
    }

    putlong(0);                         /* unkFlags: no strips         */
    putlong(0);                         /* spritesNum: none            */
}

/* What the shape above occupies, worked out from the shape and not from
   the loader, so the two can disagree. */
static long expected_bytes(const struct shape* s)
{
    long n = 7 * 4;                                 /* the header      */
    int colours = s->colourCount < s->vertexNum ? s->colourCount
                                                : s->vertexNum;

    n += (long)s->vertexNum * 12 * 2;               /* two vertex runs */
    if (s->mrmMagic)
    {
        n += (long)s->vertexNum * 2;                /* mrm->shorts     */
        if (s->mrmMagic == MRM_MAGIC_WITH_PTR)
            n += (long)s->vertexNum * 2;            /* mrm->ptr        */
        n += 4 + 8 * 4;                             /* b and f[8]      */
    }
    n += (long)colours * 4;
    n += (long)s->uvNum * 8;
    n += (long)s->chunkCount * 10 * 4;              /* chunk headers   */
    n += (long)s->chunkCount * (long)s->triangleNum * 20;
    if (s->uvIdxFlag)
        n += (long)s->chunkCount * 12;
    n += 4 + 4;                                     /* unkFlags, both  */
    return n;
}

/* ------------------------------------------------------------ the load */

static tGroGeneric gro;

static tGEOGRO* load(const struct shape* s)
{
    char* readptr = buf;
    tGEOGRO* geo;

    build(s);
    geo = (tGEOGRO*)GEO_p_CreateFromBuffer(&gro, &readptr, NULL);
    if (!geo)
    {
        fail("the loader gave back nothing");
        return NULL;
    }
    if (readptr - buf != expected_bytes(s))
        failf("the bytes the loader consumed",
              (long)(readptr - buf), expected_bytes(s));
    return geo;
}

static void check_the_body(const struct shape* s, tGEOGRO* geo)
{
    int i;
    int c;
    int colours = s->colourCount < s->vertexNum ? s->colourCount
                                                : s->vertexNum;

    if ((int)geo->vertexNum != s->vertexNum)
        failf("vertexNum", (long)geo->vertexNum, s->vertexNum);
    if ((int)geo->uvNum != s->uvNum)
        failf("uvNum", (long)geo->uvNum, s->uvNum);
    if ((int)geo->triangleChunkCount != s->chunkCount)
        failf("triangleChunkCount", (long)geo->triangleChunkCount,
              s->chunkCount);

    for (i = 0; i < s->vertexNum; i++)
        if (geo->vertices[i].x != (float)i)
        {
            fail("a vertex did not come through, so the read head was in "
                 "the wrong place");
            break;
        }

    for (i = 0; i < s->uvNum; i++)
        if (geo->UVs[i].u != (float)i * 0.125f)
        {
            fail("a UV did not come through, so the read head was in the "
                 "wrong place");
            break;
        }

    /* The colour slot is an array now, and its first word is the count
       the allocator put there -- which is the vertex count, not the
       file's colour count. */
    if (colours)
    {
        if (!geo->vertexColors)
            fail("no colour array was made");
        else
        {
            if ((int)geo->vertexColors[0] != s->vertexNum)
                failf("the count the allocator wrote into the colour array",
                      (long)geo->vertexColors[0], s->vertexNum);
            for (i = 0; i < colours; i++)
                if (geo->vertexColors[i + 1] != (unsigned int)(0xC0100 + i))
                {
                    fail("a vertex colour did not come through");
                    break;
                }
        }
    }

    for (c = 0; c < s->chunkCount; c++)
    {
        if (geo->triangleChunks[c].materialNum != 7 + c)
            failf("a chunk's material", geo->triangleChunks[c].materialNum,
                  7 + c);
        if (s->uvIdxFlag)
        {
            if (!geo->triangleChunks[c].MRMUVIndexElems)
                fail("a chunk's UV index block was not made");
            else if (geo->triangleChunks[c].MRMUVIndexElems->UVStartOffset
                     != 0x11 + c)
                failf("a chunk's UV start offset",
                      geo->triangleChunks[c].MRMUVIndexElems->UVStartOffset,
                      0x11 + c);
        }
        else if (geo->triangleChunks[c].MRMUVIndexElems)
            fail("a chunk with no UV index flag got a block anyway");
    }
}

/* ---------------------------------------------------------- the checks */

/* The MRM magic, which is the one that was wrong. Two loads of the same
   shape: one whose magic asks for the second per-vertex array and one
   whose magic does not. */
static void check_the_mrm_magic(void)
{
    struct shape s;
    tGEOGRO* geo;
    int i;

    printf("-- the MRM magic, and the array it asks for\n");

    memset(&s, 0, sizeof(s));
    s.vertexNum = 9;
    s.colourCount = 4;
    s.uvNum = 5;
    s.chunkCount = 2;
    s.triangleNum = 3;
    s.uvIdxFlag = 1;

    /* the magic that asks for both arrays */
    s.mrmMagic = MRM_MAGIC_WITH_PTR;
    geo = load(&s);
    if (!geo)
        return;
    if (!geo->mrm)
        fail("the MRM was not made");
    else
    {
        if (LOA_ui_MarkerFromRef(geo->mrm) == s.mrmMagic)
            fail("the MRM slot still holds the magic, so the allocator "
                 "never replaced it");
        if (!geo->mrm->shorts)
            fail("the MRM's first array was not made");
        if (!geo->mrm->ptr)
            fail("the magic asks for the MRM's second array and it was not "
                 "made, so the loader skipped a block of the file");
        else
            for (i = 0; i < s.vertexNum; i++)
                if (geo->mrm->ptr[i] != (uint16_t)(0x200 + i))
                {
                    fail("the MRM's second array did not come through");
                    break;
                }
        if (geo->mrm->shorts)
            for (i = 0; i < s.vertexNum; i++)
                if (geo->mrm->shorts[i] != (uint16_t)(0x100 + i))
                {
                    fail("the MRM's first array did not come through");
                    break;
                }
        if ((int)geo->mrm->endNum != s.vertexNum)
            failf("the MRM's endNum", (long)geo->mrm->endNum, s.vertexNum);
    }
    check_the_body(&s, geo);
    GEO_Free((tGroGeneric*)geo);
    if (GEOMOCK_liveBlocks)
        failf("blocks still held after the geometry was freed",
              GEOMOCK_liveBlocks, 0);

    /* another non-zero magic, which asks for only the first array. The
       shape differs from the one above by exactly vertexNum shorts. */
    s.mrmMagic = 0xc0de0004;
    geo = load(&s);
    if (!geo)
        return;
    if (!geo->mrm)
        fail("the MRM was not made for the other magic");
    else
    {
        if (!geo->mrm->shorts)
            fail("the MRM's first array was not made for the other magic");
        if (geo->mrm->ptr)
            fail("a magic that does not ask for the second array got one");
    }
    check_the_body(&s, geo);
    GEO_Free((tGroGeneric*)geo);
    if (GEOMOCK_liveBlocks)
        fail("freeing the other geometry left blocks behind");
}

/* No MRM at all, which is the common shape and the one that would have
   gone on working. */
static void check_without_an_mrm(void)
{
    struct shape s;
    tGEOGRO* geo;

    printf("-- a geometry with no MRM\n");

    memset(&s, 0, sizeof(s));
    s.vertexNum = 6;
    s.colourCount = 6;
    s.uvNum = 3;
    s.chunkCount = 1;
    s.triangleNum = 2;

    geo = load(&s);
    if (!geo)
        return;
    if (geo->mrm)
        fail("a geometry with no MRM got one");
    check_the_body(&s, geo);
    GEO_Free((tGroGeneric*)geo);
    if (GEOMOCK_liveBlocks)
        fail("freeing it left blocks behind");
}

/* The colour count marker on its own: more colours in the file than
   there are vertices, which the loader clamps, and none at all. */
static void check_the_colour_count(void)
{
    struct shape s;
    tGEOGRO* geo;

    printf("-- the vertex colour count\n");

    memset(&s, 0, sizeof(s));
    s.vertexNum = 4;
    s.uvNum = 2;
    s.chunkCount = 1;
    s.triangleNum = 1;

    /* more than there are vertices */
    s.colourCount = 10;
    geo = load(&s);
    if (geo)
    {
        check_the_body(&s, geo);
        GEO_Free((tGroGeneric*)geo);
        if (GEOMOCK_liveBlocks)
            fail("freeing the clamped geometry left blocks behind");
    }

    /* none, so the slot stays empty and no array is made */
    s.colourCount = 0;
    geo = load(&s);
    if (geo)
    {
        if (geo->vertexColors)
            fail("a geometry with no colours got a colour array");
        check_the_body(&s, geo);
        GEO_Free((tGroGeneric*)geo);
        if (GEOMOCK_liveBlocks)
            fail("freeing the colourless geometry left blocks behind");
    }
}

int main(void)
{
    int i;

    printf("=== a geometry loaded from a buffer ===\n");

    memset(GRO_gast_Interface, 0, sizeof(GRO_gast_Interface));
    for (i = 0; i < 12; i++)
        GRO_gast_Interface[i].id = i;
    gro.intf = &GRO_gast_Interface[1];
    GEO_b_IsInFix = 0;

    check_without_an_mrm();
    check_the_mrm_magic();
    check_the_colour_count();

    if (failures)
    {
        printf("  %d failure(s)\n", failures);
        return 1;
    }
    printf("  all checks passed\n");
    return 0;
}
