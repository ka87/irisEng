#pragma once

#include "gdi.h"

struct sGeoTriangleChunks;
struct sGeoUVElem;

/* Building the interleaved vertices the engine's geometry is drawn
   from.

   This was inside the Direct3D renderer, and none of it is Direct3D:
   it walks a chunk of triangles, picks each corner out of the
   engine's position, colour and texture-coordinate arrays, and packs
   the result. The arrays are separate and the vertices are
   interleaved, so something has to do this, and a second renderer
   needs the same answers -- including the colour packing, which is
   not something to write twice.

   What stays with a renderer is where the vertices go and how they
   are drawn. */

/* The engine's arrays, and the state that decides how they are read.
   A renderer gathers this from the big buffer and its own flags. */
typedef struct sGvx_Source
{
    const Vec3*              vertices;
    const struct sGeoUVElem* uvs;      /* NULL when the geometry has none */
    const int*               colors;   /* NULL for one colour throughout */
    const int*               second;   /* NULL when there is no second set */
    int                      mask;
    int                      alpha;
    unsigned int             solidColor;    /* used when colors is NULL */
    int                      useVertexUv;   /* one coordinate per vertex */
} tGvx_Source;

/* The colour one vertex gets. Public because the solid case needs the
   same packing and a renderer may want it directly. */
unsigned int GVX_ui_Color(const int* colors, const int* second, int mask,
                          int alpha, int idx, int secondIdx);

/* The one colour used when there is no per-vertex array -- the same
   packing, applied to the big buffer's single colour. */
unsigned int GVX_ui_SolidColor(int mask, int alpha, unsigned int solidVertexColor);

/* Writes three vertices per triangle into out, which must hold at
   least 3 * chunks->triangleNum of them. */
void GVX_BuildTriangles(tGdi_TexturedVertex* out,
                        const struct sGeoTriangleChunks* chunks,
                        const tGvx_Source* src);
