#include "watermesh.h"

#include <math.h>
#include <string.h>

/* The reflection target's nominal size. It is written into the
   projection remap as a literal in the renderer this came from, where
   the target happens to be exactly this big, so the ratio it forms is
   one and the term is invisible. It is kept as a ratio here so that a
   renderer with a differently sized target is scaled rather than
   silently wrong. */
#define WMS_NOMINAL     512.0

/* How hard the ripple pushes at the near edge, and how much of that it
   has lost by the far one. */
#define WMS_RIPPLE_NEAR 48.0
#define WMS_RIPPLE_DROP 45.0

/* The fall-off is spent before the grid is, by this much: the row's
   fraction is stretched and then clamped, so the last sixth of the
   grid sits at the far value throughout. */
#define WMS_RIPPLE_RUSH 1.2

int WMS_i_PrimitiveCount(unsigned int columns, int rows)
{
    /* The formula and nothing else, including where it is nonsense: at
       a single row it comes to minus three, and the renderer this came
       from goes on to size a buffer from that. Clamping it here would
       be a different renderer rather than a tidier one, so the figure
       is passed through and the note is left for whoever meets it. */
    return (int)((columns + 3) * (unsigned int)(2 * rows - 2)) - 3;
}

int WMS_i_IndexCount(unsigned int columns, int rows)
{
    /* A strip's first two indices draw nothing on their own. */
    return WMS_i_PrimitiveCount(columns, rows) + 2;
}

float WMS_f_RippleScale(unsigned int row, int rows)
{
    double fade;

    if (rows <= 0)
        return 0.0f;
    fade = (double)row * WMS_RIPPLE_RUSH / (double)rows;
    if (fade > 1.0)
        fade = 1.0;
    return (float)(WMS_RIPPLE_NEAR - fade * WMS_RIPPLE_DROP);
}

unsigned int WMS_u_SwapRedBlue(unsigned int color)
{
    /* Alpha and green keep their places; the two that move are read
       out before either is overwritten. */
    unsigned int red = (color >> 16) & 0xFFu;
    unsigned int blue = color & 0xFFu;

    return (color & 0xFF00FF00u) | red | (blue << 16);
}

int WMS_i_StripIndices(unsigned int columns, int rows,
                       unsigned short* out, int max)
{
    int          want = WMS_i_IndexCount(columns, rows);
    int          written = 0;
    int          vertex = 0;
    int          started = 0;
    int          row;
    unsigned int half;
    unsigned int rest;
    unsigned int i;

    /* A grid too small to strip writes nothing and the figure above
       does not describe it: at one row the loop never runs while the
       count comes to minus one, and at one column the loop writes
       three where the count says seven. The two agree from two columns
       and two rows on, which is every grid the simulation builds. */
    if (!out || want <= 0 || max < want)
        return 0;

    /* Transcribed, loop for loop. The index arithmetic is signed and
       the counters are not, and both are kept that way: `rest` goes
       round the houses at a column count of one, and the test below
       that reads as a bounds check is what stops it. */
    for (row = 0; row < rows - 1; row++)
    {
        if (started)
            out[written++] = (unsigned short)vertex;
        else
            started = 1;

        half = columns >> 1;
        for (i = 0; i < half; i++)
        {
            out[written++] = (unsigned short)vertex;
            out[written++] = (unsigned short)(columns + (unsigned int)vertex);
            ++vertex;
        }

        rest = half - 1;
        --vertex;
        out[written++] = (unsigned short)(columns + (unsigned int)vertex);

        if (rest < columns)
        {
            for (i = columns - rest; i; i--)
            {
                out[written++] = (unsigned short)(columns + (unsigned int)vertex);
                out[written++] = (unsigned short)vertex;
                ++vertex;
            }
        }

        out[written++] = (unsigned short)(vertex - 1);
        out[written++] = (unsigned short)(vertex - 1);
    }
    return written;
}

int WMS_i_Vertices(const tWms_Source* src, tWms_Vertex* out, int max)
{
    const Vec3*         pos;
    const Vec3*         wave;
    const unsigned int* colors;
    unsigned int        row;
    unsigned int        col;
    int                 written = 0;
    int                 want;

    if (!src || !out || !src->positions || !src->waves || !src->colors)
        return 0;
    if (!src->columns || src->rows <= 0)
        return 0;

    want = (int)src->columns * src->rows;
    if (max < want)
        return 0;

    pos = src->positions;
    wave = src->waves;
    colors = src->colors;

    for (row = 0; row < (unsigned int)src->rows; row++)
    {
        float scale = WMS_f_RippleScale(row, src->rows);

        for (col = 0; col < src->columns; col++)
        {
            /* The wave's two horizontal components added together is
               what pushes the coordinate -- one number out of two, and
               the sum rather than a length, so it cancels where they
               are opposite.
             *
             * The sum is narrow and then widens: both components are
             * floats, so the addition rounds to a float before the
             * multiply below sees it. Adding them wide instead moves
             * the depth component by one unit in the last place, which
             * the differential comparison caught and nothing else
             * would have. */
            double ripple = wave->y + wave->x;

            out[written].pos = *pos;
            /* Spelled as the original spells them: the horizontal
               component a float subtraction, the vertical one done
               wide.
             *
             * Only the depth one matters. A lone subtraction of two
             * floats comes to the same float whether it is done narrow
             * or wide -- checked over forty million random pairs, not
             * assumed -- so the first two could be written either way
             * and the difference would be invisible. The depth
             * component is a second operation on that difference, and
             * there the narrowing is real: widening it moves the
             * result by a unit in the last place, which the
             * differential comparison against the other renderer
             * catches. */
            out[written].dir.x = pos->x - src->camera.x;
            out[written].dir.y = (float)((double)pos->y - src->camera.y);
            out[written].dir.z = (float)((pos->z - src->camera.z)
                                         - fabs(ripple * scale));
            out[written].diffuse = WMS_u_SwapRedBlue(*colors);
            out[written].specular = 0;

            ++written;
            ++pos;
            ++wave;
            ++colors;
        }
    }
    return written;
}

void WMS_ProjectionToTexture(Mtx4x4WithoutAdditionalData* out,
                             const Mtx4x4WithoutAdditionalData* proj,
                             int width, int height)
{
    Mtx4x4WithoutAdditionalData flipped;
    Mtx4x4WithoutAdditionalData remap;
    const float*                a;
    const float*                b;
    float*                      r;
    double                      invHeight;
    double                      halfHeight;
    int                         i, j, k;

    if (!out || !proj || width <= 0 || height <= 0)
        return;

    memcpy(&flipped, proj, sizeof(flipped));
    flipped.f22 = -flipped.f22;

    memset(&remap, 0, sizeof(remap));
    remap.f33 = 1.0f;
    remap.f44 = 1.0f;
    remap.f43 = 0.0f;
    remap.f11 = (float)(WMS_NOMINAL / (double)width * 0.5);
    invHeight = 1.0 / (double)height;
    halfHeight = WMS_NOMINAL * invHeight * 0.5;
    /* Half a texel each way on the vertical, and the two signs are not
       the same: the scale loses it and the offset gains it. */
    remap.f22 = (float)(halfHeight - invHeight);
    remap.f41 = remap.f11;
    remap.f42 = (float)(halfHeight + invHeight);

    a = (const float*)&flipped;
    b = (const float*)&remap;
    r = (float*)out;
    for (i = 0; i < 4; i++)
        for (j = 0; j < 4; j++)
        {
            float s = 0.0f;

            for (k = 0; k < 4; k++)
                s += a[i * 4 + k] * b[k * 4 + j];
            r[i * 4 + j] = s;
        }
}
