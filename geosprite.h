#pragma once

#include "gdi.h"

struct sGeoSprites;

/* Turning the engine's sprites into quads.
 *
 * This was inside the Direct3D renderer and none of it is Direct3D's.
 * A sprite is a position, a size and an index into the colour arrays;
 * what the renderer is given is a list of those, and what it has to
 * draw is four corners each, facing the camera. The facing comes out
 * of the top of the engine's matrix stack, the size out of the
 * sprite's own and the batch's, and the colour out of the same arrays
 * and the same packing the world's vertices use.
 *
 * A second renderer needs every one of those answers, and two of the
 * things in here are not the kind of thing to write twice: the
 * explode generator's alternative layout, which it marks with a
 * sentinel index, and the wobble a display setting puts on two of the
 * four corners.
 *
 * What stays with a renderer is where the quads go and how they are
 * drawn.
 */

/* The arrays and the state the walk reads. A renderer gathers this
   from the big buffer, the matrix stack and its own colour mask. */
typedef struct sGsp_Source
{
    /* Positions. Vec3 per sprite normally, and tSpriteC0DEData -- a
       position and a size together -- when the list is the explode
       generator's. The sentinel in the element list says which. */
    const void*   positions;

    const int*    colors;      /* NULL for one colour throughout */
    const int*    second;      /* NULL when there is no second set */
    int           mask;
    int           alpha;
    unsigned int  solidColor;  /* used when colors is NULL */

    /* The top of the engine's matrix stack, which is what makes a
       sprite face the camera: its first three columns are the axes the
       corners are built along. */
    const Mtx4x4* matrix;

    int           waveSprite;  /* the display setting that wobbles them */
} tGsp_Source;

/* How many quads the walk will produce, which is the batch's own
   count either way -- the sentinel layout does not change it. Here so
   that a renderer sizes its buffer from one place. */
int GSP_i_QuadCount(const struct sGeoSprites* sprites);

/* What the walk does with each quad as it finishes it. */
typedef void (*GSP_pfn_Emit)(void* user, const tGdi_SpriteQuad* quad);

/* Walks the batch and hands over one quad at a time.

   One at a time rather than filling an array, because one of the two
   renderers wants each quad the moment it exists -- it writes them
   into a buffer the device has locked -- and giving it an array would
   have meant allocating one per batch where it allocated nothing.
   GSP_BuildQuads below is the array form for the renderer that does
   want one. */
void GSP_Walk(const struct sGeoSprites* sprites, const tGsp_Source* src,
              GSP_pfn_Emit emit, void* user);

/* The same walk, into an array of at least GSP_i_QuadCount quads. */
void GSP_BuildQuads(tGdi_SpriteQuad* out, const struct sGeoSprites* sprites,
                    const tGsp_Source* src);
