#include "waterreflect.h"

#include <math.h>
#include <string.h>

float WRF_f_Extent(const tGameObjectBV* bv, int b_sphere)
{
    if (!bv)
        return 0.0f;
    /* A sphere says its size with one number and a box with two, and
       the two numbers are *added* rather than averaged or taken as a
       diagonal -- so a flat wide object scores about the same as a
       square one twice its area. That is the renderer's own arithmetic
       and not a reading of what it meant. */
    if (b_sphere)
        return bv->BVMax.x;
    return (float)(fabs((double)bv->BVMax.y - bv->BVMin.y)
                   + fabs((double)bv->BVMax.x - bv->BVMin.x));
}

int WRF_b_WorthReflecting(const tGameObjectBV* bv, int b_sphere,
                          float minimum)
{
    /* Strictly less, so an object exactly at the threshold is
       reflected. The threshold starts at nought, where everything is. */
    if (!bv)
        return 0;
    return !(WRF_f_Extent(bv, b_sphere) < minimum);
}

void WRF_Mirror(Mtx4x4* out, Mtx4x4* objectToWorld, float waterHeight)
{
    if (!out || !objectToWorld)
        return;

    /* Flattened first, as every other path through this matrix does:
       a matrix carrying separate scale factors is turned into a plain
       affine one before anything reads its rows. */
    MATH_MakeOGLMatrix(out, objectToWorld);

    /* The third column of the rotation, negated. These are row-vector
       matrices, so a row is one of the transform's axes and the `.z` of
       each row is the third column -- negating all three negates the
       transform's output z, which is the reflection's rotation part.
       Negating a *row* instead would mirror in a different plane
       altogether and still look like a mirror from most angles. */
    out->v1.z = -out->v1.z;
    out->v2.z = -out->v2.z;
    out->v3.z = -out->v3.z;

    /* And the position reflected in the water plane. The renderer this
       came from writes it as the position less twice its distance
       above the water, spelled out the long way; it comes to twice the
       water height less the position, and that is written plainly here
       because the long form reads like a mistake. */
    out->Pos.z = 2.0f * waterHeight - out->Pos.z;
}

void WRF_ClipPlane(float out[4], float waterHeight, float offset)
{
    if (!out)
        return;
    /* Pointing down, so the half-space kept is the one at or below the
       water line. After the mirror above, that is where everything
       which was above the water has landed -- so the plane is what
       stops the reflection showing the parts of the world that are
       above the surface and could not be seen in it. */
    out[0] = 0.0f;
    out[1] = 0.0f;
    out[2] = -1.0f;
    out[3] = offset + waterHeight;
}
