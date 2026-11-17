/* The screenshot's file, now written with libpng.
 *
 * A round trip: a picture of known colours goes in, the file is read
 * back with libpng's own reader, and the pixels are compared. That is
 * the whole of what this has to prove, and it proves the two things
 * the writer does beyond handing rows over -- both of which are
 * libpng transformations rather than code here, so getting them wrong
 * is a one-line mistake that produces a plausible file:
 *
 *   the byte order. The rows are BGRA because that is what a locked
 *   Direct3D surface holds and what OpenGL is asked for, and libpng is
 *   told so instead of being handed repacked copies. Forget it and the
 *   picture comes out with its reds and blues swapped, which is not a
 *   broken file and not obvious in a screenshot of a blue-green scene;
 *
 *   which way up it is. OpenGL hands a frame back bottom row first.
 *   Forget it and the screenshot is upside down, which an evenly lit
 *   scene hides.
 *
 * The old writer stored its pixels uncompressed, so the test that
 * replaced this one could read them out of the file with memcpy. A
 * real encoder's output cannot be read that way, which is the reason
 * the reader is libpng here -- and the better test for it: a file
 * libpng can read is a file every other reader can too, which the
 * hand-rolled writer was never actually checked for.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "png.h"

#include "../pngsave.c"

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

/* The file, decoded by libpng. */
static unsigned char  rd_pix[4096 * 3];
static int            rd_width;
static int            rd_height;

static int readBack(const char* path)
{
    FILE*       f;
    png_structp png;
    png_infop   info;
    png_bytep   rows[64];
    int         y;

    rd_width = 0;
    rd_height = 0;
    f = fopen(path, "rb");
    if (!f)
        return 0;
    png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png)
    {
        fclose(f);
        return 0;
    }
    info = png_create_info_struct(png);
    if (!info || setjmp(png_jmpbuf(png)))
    {
        png_destroy_read_struct(&png, info ? &info : NULL, NULL);
        fclose(f);
        return 0;
    }
    png_init_io(png, f);
    png_read_info(png, info);
    rd_width = (int)png_get_image_width(png, info);
    rd_height = (int)png_get_image_height(png, info);
    /* What the writer says it makes: eight bits a channel, three
       channels, no interlacing. Asserted here rather than converted,
       because a file that needed converting is not the file the
       writer claims to write. */
    ok(png_get_bit_depth(png, info) == 8, "eight bits a channel");
    ok(png_get_color_type(png, info) == PNG_COLOR_TYPE_RGB,
       "three channels and no alpha");
    ok(png_get_interlace_type(png, info) == PNG_INTERLACE_NONE,
       "and not interlaced");

    if (rd_height > 64 || rd_width * 3 > 4096)
    {
        png_destroy_read_struct(&png, &info, NULL);
        fclose(f);
        return 0;
    }
    for (y = 0; y < rd_height; y++)
        rows[y] = rd_pix + (size_t)y * (size_t)rd_width * 3u;
    png_read_image(png, rows);
    png_read_end(png, NULL);
    png_destroy_read_struct(&png, &info, NULL);
    fclose(f);
    return 1;
}

/* A pixel of the decoded file, as red, green, blue. */
static void pixelAt(int row, int col, int* r, int* g, int* b)
{
    const unsigned char* p = rd_pix
                             + ((size_t)row * (size_t)rd_width
                                + (size_t)col) * 3u;

    *r = p[0];
    *g = p[1];
    *b = p[2];
}

static void rowPixel(int row, int* r, int* g, int* b)
{
    pixelAt(row, 0, r, g, b);
}

int main(void)
{
    /* Two rows of **three** pixels, BGRA. Three and not one on
       purpose: a one-pixel row never reaches the fourth byte of its
       four, so the transformation that drops that byte does nothing
       and a writer missing it passes. At three pixels, a writer that
       walks three bytes a pixel instead of four reads the second
       pixel out of the first one's padding and every colour is
       wrong.

       The first row is red, green, blue across; the second is the
       same three turned round. The fourth byte of every pixel is a
       value none of the channels have, so it would be noticed if it
       reached the file. */
    unsigned char rows[2][12] = {
        { 0x00, 0x00, 0xFF, 0x7F,       /* red */
          0x00, 0xFF, 0x00, 0x7F,       /* green */
          0xFF, 0x00, 0x00, 0x7F },     /* blue */
        { 0xFF, 0x00, 0x00, 0x7F,       /* blue */
          0x00, 0xFF, 0x00, 0x7F,       /* green */
          0x00, 0x00, 0xFF, 0x7F }      /* red */
    };
    const char* path = "tests/build/pngsave.png";
    int r, g, b;

    printf("-- a file libpng can read, with the colours it was given\n");

    ok(PNG_i_WriteBgra(path, (const unsigned char*)rows, 12, 3, 2, 0),
       "a top-down file is written");
    ok(readBack(path), "and libpng reads it back");
    okEq(rd_width, 3, "three pixels across");
    okEq(rd_height, 2, "two rows down");
    if (rd_height == 2 && rd_width == 3)
    {
        /* Every pixel of both rows, which is what catches a writer
           that walks the wrong number of bytes a pixel: the second
           pixel is the first one to come out wrong. */
        pixelAt(0, 0, &r, &g, &b);
        okEq(r, 0xFF, "the first pixel is red");
        okEq(g, 0x00, "with no green");
        okEq(b, 0x00, "and no blue -- so the channels were not swapped");
        pixelAt(0, 1, &r, &g, &b);
        okEq(g, 0xFF, "the second is green");
        okEq(r, 0x00, "and nothing else");
        okEq(b, 0x00, "in either channel");
        pixelAt(0, 2, &r, &g, &b);
        okEq(b, 0xFF, "the third is blue");
        okEq(r, 0x00, "and nothing else");

        pixelAt(1, 0, &r, &g, &b);
        okEq(b, 0xFF, "the second row starts with the blue one");
        pixelAt(1, 2, &r, &g, &b);
        okEq(r, 0xFF, "and ends with the red");
    }

    printf("-- and the rows the way round the caller has them\n");

    ok(PNG_i_WriteBgra(path, (const unsigned char*)rows, 12, 3, 2, 1),
       "a bottom-up file is written");
    ok(readBack(path), "and read back");
    if (rd_height == 2 && rd_width == 3)
    {
        rowPixel(0, &r, &g, &b);
        okEq(b, 0xFF, "its first row is the one that was last in"
                      " memory");
        rowPixel(1, &r, &g, &b);
        okEq(r, 0xFF, "and its last row is the one that was first");
    }

    printf("-- a pitch wider than the picture\n");

    /* Which both a locked surface and a read-back frame can have: the
       padding must not reach the file. */
    {
        unsigned char padded[2][24];

        /* Three pixels and then twelve bytes of padding a row, filled
           with a value none of the channels have. */
        memset(padded, 0xA5, sizeof(padded));
        memcpy(padded[0], rows[0], 12);
        memcpy(padded[1], rows[1], 12);
        ok(PNG_i_WriteBgra(path, (const unsigned char*)padded, 24, 3, 2, 0),
           "a padded file is written");
        ok(readBack(path), "and read back");
        if (rd_height == 2 && rd_width == 3)
        {
            int bad = 0;
            int col;

            pixelAt(0, 0, &r, &g, &b);
            okEq(r, 0xFF, "the first pixel is still red");
            /* And none of the padding is anywhere in the picture. */
            for (col = 0; col < 3; col++)
            {
                pixelAt(0, col, &r, &g, &b);
                if (r == 0xA5 || g == 0xA5 || b == 0xA5)
                    bad++;
                pixelAt(1, col, &r, &g, &b);
                if (r == 0xA5 || g == 0xA5 || b == 0xA5)
                    bad++;
            }
            okEq(bad, 0, "and none of the padding reached the file");
        }
    }

    printf("-- a picture with every channel different\n");

    /* One pixel whose three channels are all different, which pins
       which byte of the four is which channel. One pixel is enough
       for that and not enough for anything else, which is why it is
       not the picture the cases above use. */
    {
        unsigned char one[4] = { 0x11, 0x22, 0x33, 0x44 };

        ok(PNG_i_WriteBgra(path, one, 4, 1, 1, 0), "written");
        ok(readBack(path), "and read back");
        if (rd_height == 1)
        {
            rowPixel(0, &r, &g, &b);
            okEq(r, 0x33, "the red is the third byte of the four");
            okEq(g, 0x22, "the green the second");
            okEq(b, 0x11, "and the blue the first");
        }
    }

    printf("-- and the ways it can be asked for nothing\n");

    okEq(PNG_i_WriteBgra("tests/build/no/such/dir/x.png",
                         (const unsigned char*)rows, 12, 3, 2, 0), 0,
         "an unwritable path answers nought");
    okEq(PNG_i_WriteBgra(path, NULL, 12, 3, 2, 0), 0,
         "so do no pixels");
    okEq(PNG_i_WriteBgra(path, (const unsigned char*)rows, 12, 0, 2, 0), 0,
         "and a picture with no width");
    okEq(PNG_i_WriteBgra(NULL, (const unsigned char*)rows, 12, 3, 2, 0), 0,
         "and no path at all");

    printf("  %d checks, %d failure(s)\n", checks, failures);
    return failures ? 1 : 0;
}
