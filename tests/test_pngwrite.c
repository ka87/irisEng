/* The screenshot's file format.
 *
 * The writer itself is proven by the Direct3D differential: it was
 * moved out of that renderer whole, and the frozen copy still has it
 * inline, so the two write the same file byte for byte. What the
 * differential cannot see is the parameter the move added --
 * `bottomUp`, which exists because OpenGL hands a frame back with its
 * bottom row first -- so that is what this harness is for.
 *
 * The file is read back and its pixels decoded. There is no decoder
 * here and none is needed: the writer stores the pixels in an
 * uncompressed deflate stream, so the rows sit in the file in plain
 * sight behind a fixed-size header.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../pngwrite.c"

static int checks;
static int failures;

static void okEq(long got, long want, const char* what)
{
    checks++;
    if (got != want)
    {
        failures++;
        printf("  FAIL %s: got %ld want %ld\n", what, got, want);
    }
}

static void ok(int cond, const char* what)
{
    checks++;
    if (!cond) { failures++; printf("  FAIL %s\n", what); }
}

/* The pixels out of a file the writer made: past the signature and
 * the IHDR, into the IDAT, past the zlib header and the stored
 * block's own, and then the rows -- each one a filter byte and then
 * three bytes a pixel, red first.
 */
static unsigned char pix[4096];
static int           pixLen;

static int readPixels(const char* path, int width, int height)
{
    unsigned char buf[8192];
    size_t        n;
    FILE*         f = fopen(path, "rb");
    int           i;

    pixLen = 0;
    if (!f)
        return 0;
    n = fread(buf, 1, sizeof(buf), f);
    fclose(f);

    /* The IDAT chunk: find its type, then step over the four bytes of
       type, the two of the zlib header and the five of the stored
       block's header. */
    for (i = 0; i + 4 < (int)n; i++)
        if (memcmp(buf + i, "IDAT", 4) == 0)
        {
            int at = i + 4 + 2 + 5;
            int want = height * (1 + width * 3);

            if (at + want > (int)n || want > (int)sizeof(pix))
                return 0;
            memcpy(pix, buf + at, (size_t)want);
            pixLen = want;
            return 1;
        }
    return 0;
}

/* The nth row's first pixel, as red, green, blue. */
static void rowPixel(int row, int width, int* r, int* g, int* b)
{
    const unsigned char* p = pix + (size_t)row * (size_t)(1 + width * 3);

    *r = p[1];
    *g = p[2];
    *b = p[3];
}

int main(void)
{
    /* Two rows of one pixel: the first in memory red, the second
       blue. Four bytes a pixel in the order a Windows surface and a
       read-back OpenGL frame both have -- blue, green, red, then a
       byte nobody reads, which is filled with something that would be
       noticed if it were. */
    unsigned char rows[2][4] = {
        { 0x00, 0x00, 0xFF, 0x7F },     /* red */
        { 0xFF, 0x00, 0x00, 0x7F }      /* blue */
    };
    const char* path = "tests/build/pngwrite.png";
    int r, g, b;

    printf("-- the byte order, and which way up the rows are\n");

    /* Top-down: the first row in memory is the top of the picture. */
    ok(PNG_i_WriteBgra(path, (const unsigned char*)rows, 4, 1, 2, 0),
       "a top-down file is written");
    ok(readPixels(path, 1, 2), "and its pixels read back");
    if (pixLen)
    {
        rowPixel(0, 1, &r, &g, &b);
        okEq(r, 0xFF, "the file's first row is the red one");
        okEq(b, 0x00, "with no blue in it");
        rowPixel(1, 1, &r, &g, &b);
        okEq(b, 0xFF, "and its second row is the blue one");
        okEq(r, 0x00, "with no red");
    }

    /* Bottom-up: the first row in memory is the *bottom*, so the file
       has to come out the other way round. This is the parameter the
       move added and the one the OpenGL screenshot passes. */
    ok(PNG_i_WriteBgra(path, (const unsigned char*)rows, 4, 1, 2, 1),
       "a bottom-up file is written");
    ok(readPixels(path, 1, 2), "and read back");
    if (pixLen)
    {
        rowPixel(0, 1, &r, &g, &b);
        okEq(b, 0xFF, "its first row is the blue one, which was last in"
                      " memory");
        rowPixel(1, 1, &r, &g, &b);
        okEq(r, 0xFF, "and its last row is the red one");
    }

    /* A pitch wider than the picture, which both a locked surface and
       a read-back frame can have: the padding must not reach the
       file. */
    {
        unsigned char padded[2][12];

        memset(padded, 0xA5, sizeof(padded));   /* the padding */
        padded[0][0] = 0x00; padded[0][1] = 0x00; padded[0][2] = 0xFF;
        padded[1][0] = 0xFF; padded[1][1] = 0x00; padded[1][2] = 0x00;
        ok(PNG_i_WriteBgra(path, (const unsigned char*)padded, 12, 1, 2, 0),
           "a padded file is written");
        ok(readPixels(path, 1, 2), "and read back");
        if (pixLen)
        {
            rowPixel(0, 1, &r, &g, &b);
            okEq(r, 0xFF, "the first row is still the red one");
            okEq(g, 0x00, "with none of the padding in it");
        }
    }

    /* And a path that cannot be opened is a nought rather than a
       crash: the screenshot key must not take the frame down. */
    okEq(PNG_i_WriteBgra("tests/build/no/such/dir/x.png",
                         (const unsigned char*)rows, 4, 1, 2, 0), 0,
         "an unwritable path answers nought");

    printf("  %d checks, %d failure(s)\n", checks, failures);
    return failures ? 1 : 0;
}
