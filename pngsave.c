#include "pngsave.h"

#include <stdio.h>
#include <stdlib.h>

#include "png.h"

/* The screenshot's file, written with libpng.
 *
 * What this has to do beyond handing libpng the rows is the two things
 * the callers' pixels differ by:
 *
 *   **the byte order.** The pixels are BGRA, because that is what a
 *   locked Direct3D surface holds and what OpenGL is asked for, and
 *   libpng is told so rather than being handed swapped copies:
 *   png_set_bgr turns the channel order round inside the write, and
 *   png_set_filler drops the fourth byte. So nothing is repacked here
 *   and the rows go to libpng exactly as the renderer read them.
 *
 *   **which way up they are.** A row pointer per row, taken from
 *   either end -- there is no copy for this either.
 *
 * libpng reports errors by longjmp, so the error path is a setjmp
 * rather than a return value to check. A screenshot that cannot be
 * written says so and the frame carries on; the key must not take the
 * game down.
 */

/* The rows, as libpng wants them: one pointer each. A frame's worth
   at the sizes this is called with is a few thousand pointers, which
   is not worth an allocation failure path of its own, but it is worth
   not putting on the stack. */
static png_bytep* PNG_l_Rows(const unsigned char* pixels, int pitch,
                             int height, int bottomUp)
{
    png_bytep* rows = (png_bytep*)malloc((size_t)height
                                         * sizeof(png_bytep));
    int        y;

    if (!rows)
        return NULL;
    for (y = 0; y < height; y++)
    {
        /* Bottom-up means the first row in memory is the bottom of the
           picture, so the file's first row is the last of them. */
        int source = bottomUp ? height - 1 - y : y;

        rows[y] = (png_bytep)(pixels + (size_t)source * (size_t)pitch);
    }
    return rows;
}

int PNG_i_WriteBgra(const char* path, const unsigned char* pixels,
                    int pitch, int width, int height, int bottomUp)
{
    FILE*       file;
    png_structp png;
    png_infop   info;
    png_bytep*  rows;

    if (!path || !pixels || width <= 0 || height <= 0 || pitch <= 0)
        return 0;

    rows = PNG_l_Rows(pixels, pitch, height, bottomUp);
    if (!rows)
        return 0;

    file = fopen(path, "wb");
    if (!file)
    {
        free(rows);
        return 0;
    }

    png = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png)
    {
        fclose(file);
        free(rows);
        return 0;
    }
    info = png_create_info_struct(png);
    if (!info)
    {
        png_destroy_write_struct(&png, NULL);
        fclose(file);
        free(rows);
        return 0;
    }

    /* Where libpng lands if anything goes wrong from here on. */
    if (setjmp(png_jmpbuf(png)))
    {
        png_destroy_write_struct(&png, &info);
        fclose(file);
        free(rows);
        return 0;
    }

    png_init_io(png, file);
    png_set_IHDR(png, info, (png_uint_32)width, (png_uint_32)height, 8,
                 PNG_COLOR_TYPE_RGB, PNG_INTERLACE_NONE,
                 PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);
    png_write_info(png, info);

    /* The two things that make the renderers' own pixels usable as
       they are: the fourth byte of each four is dropped, and the three
       that are left are read back to front.
     *
       After the header and not before it. These are write transforms,
       and libpng fixes the row format when the header goes out; asked
       for ahead of it they are overwritten, and the rows then go out
       four bytes wide against a header that says three: the file is
       not one libpng will read back, which is how the harness fails
       when the two lines are moved up. */
    png_set_filler(png, 0, PNG_FILLER_AFTER);
    png_set_bgr(png);

    png_write_image(png, rows);
    png_write_end(png, NULL);

    png_destroy_write_struct(&png, &info);
    fclose(file);
    free(rows);
    return 1;
}
