#pragma once

#include "math.h"

/* The water surface as a mesh, with no renderer in it.
 *
 * The water is two things, and this is the second of them. The first is
 * a reflection: the world is drawn a second time into an off-screen
 * target with the camera mirrored in the water plane. The second is the
 * surface itself -- a grid the simulation has already built, drawn with
 * that reflection projected onto it and rippled. What is here belongs
 * to the surface: the index strip, the vertex stream, and the matrix
 * that projects the reflection onto it.
 *
 * None of it is a decision. Every number below is the one the renderer
 * that has this already computes, transcribed so a second renderer can
 * compute the same thing, and tests/test_watermesh.c drives the
 * original and this side by side and compares what each one writes.
 * That comparison is the whole warrant for this file: nothing here was
 * worked out from what the water ought to look like.
 */

/* One vertex, 32 bytes, in the order the stream wants them.
 *
 * `pos` is the grid point in world space. `dir` is **not** a normal,
 * whatever a stream declaration calls it -- it is the vector from the
 * camera to the vertex, with the depth component pushed about by the
 * wave, and it is there to be turned into a texture coordinate rather
 * than lit. `diffuse` is the water's own colour, and its alpha is how
 * much of the reflection shows through. `specular` is never given a
 * value; it pads the vertex to its stride. */
typedef struct sWms_Vertex
{
    Vec3         pos;
    Vec3         dir;
    unsigned int diffuse;
    unsigned int specular;
} tWms_Vertex;

/* What a vertex build reads. The three arrays are the simulation's own,
   one entry per grid point, row-major with `columns` to a row. */
typedef struct sWms_Source
{
    const Vec3*         positions;
    const Vec3*         waves;
    const unsigned int* colors;
    unsigned int        columns;
    int                 rows;
    Vec3                camera;
} tWms_Source;

/* How many indices the strip takes for a grid this size, and how many
 * primitives that draws. The strip's own arithmetic says
 *
 *     primitives = (columns + 3) * (2 * rows - 2) - 3
 *
 * and a triangle strip draws two fewer primitives than it has indices,
 * so the two agree exactly -- which is worth knowing, because it means
 * the generator below cannot be off by an index without one of the two
 * figures moving.
 *
 * Neither is clamped. Below two rows the formula comes to minus three
 * and the renderer this came from goes on to size a buffer from it;
 * below two columns the strip writes fewer indices than the figure
 * claims. Both are facts about the original rather than oversights
 * here, and the simulation builds no grid that small. */
int WMS_i_IndexCount(unsigned int columns, int rows);
int WMS_i_PrimitiveCount(unsigned int columns, int rows);

/* Fills the index strip. Returns how many indices were written, or 0 if
 * `max` is too small for the whole strip -- a partial strip is worse
 * than none, since it would draw a torn surface rather than fail.
 *
 * The route is not the obvious one. A row is walked in two groups: the
 * first half pairing the near row's vertex with the far row's, then one
 * step back and the rest of the row pairing them the other way round,
 * with degenerate indices joining the groups and the rows. The two
 * groups pair their vertices in opposite orders, which looks as though
 * half the surface must come out wound backwards -- it does not. The
 * joins between them are an odd number of indices, so the strip's own
 * alternation flips back and every triangle in the row ends up wound
 * the same way. Checked by computing the orientation of all six
 * triangles of a four-by-two grid, not by looking at a picture. */
int WMS_i_StripIndices(unsigned int columns, int rows,
                       unsigned short* out, int max);

/* Fills the vertex stream, one entry per grid point, and returns how
 * many were written -- again 0 rather than a partial fill.
 *
 * Per vertex: the position straight through; the camera-to-vertex
 * vector with its depth pushed by the wave (see WMS_f_RippleScale); and
 * the colour with its red and blue exchanged, because the simulation
 * stores a colour the other way round from the way the stream wants
 * it. */
int WMS_i_Vertices(const tWms_Source* src, tWms_Vertex* out, int max);

/* How hard the wave pushes the texture coordinate, by row.
 *
 * It falls off with distance: 48 at the row nearest the camera down to
 * 3 at the far end, linearly in the row's fraction of the grid -- and
 * the fraction is multiplied by 1.2 and clamped, so the fall-off is
 * spent by five-sixths of the way back and the last sixth of the grid
 * all ripples by the same little. That last part is a fact about the
 * original rather than a rounding of it. */
float WMS_f_RippleScale(unsigned int row, int rows);

/* The colour swap. Red and blue change places; green and alpha stay. */
unsigned int WMS_u_SwapRedBlue(unsigned int color);

/* The reflection's projection, as a matrix to run the camera-space
 * direction through.
 *
 * The reflection is sampled by where the rippled direction lands on the
 * screen, so the matrix is the projection with the clip cube remapped
 * to the texture's nought-to-one -- a half scale and a half offset --
 * and the vertical negated, because the target is sampled the opposite
 * way up from the way it is drawn. It also shifts the vertical by half
 * a texel in each direction, which is the sampling rule of the
 * interface this came from and not anything about the water -- a
 * renderer that puts its texel centres elsewhere has to account for
 * that itself, because the shift is folded into the matrix here rather
 * than offered separately.
 *
 * `width` and `height` are the reflection target's. The original writes
 * a literal size into this arithmetic and its own target is exactly
 * that big, so the ratio the two form is one and the term never shows;
 * it is kept as a ratio so that a differently sized target is scaled
 * rather than silently wrong.
 *
 * `proj` and `out` are both row-vector matrices: a direction goes
 * through as a row on the left. The third component of the result is
 * the divide, which is what makes it a projection rather than a
 * mapping. */
void WMS_ProjectionToTexture(Mtx4x4WithoutAdditionalData* out,
                             const Mtx4x4WithoutAdditionalData* proj,
                             int width, int height);
