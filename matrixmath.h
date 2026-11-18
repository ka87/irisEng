#pragma once
#include "math.h"

/* Four matrix operations both renderers use, and the engine with
   them.
 *
 * Row-major storage and row vectors, so a point is transformed as
 * v * M and a translation lives in the fourth row; the orthographic
 * projection maps z to [0, 1]. Those are conventions and not an API:
 * they are what the engine's matrices are written in, which is why
 * the OpenGL renderer asks for a 4.5 core context and tells it to use
 * that depth range rather than converting every matrix.
 *
 * Separate from math.c because that file works in the engine's full
 * Mtx4x4, which carries a flag and three words past the sixteen
 * floats, and because its inverse is the rigid one. These four work
 * on the sixteen floats alone.
 *
 * They were platform/d3dmath.c, typed on Mtx4x4WithoutAdditionalData and including
 * <d3d9.h> -- which was the whole of what stopped an otherwise shared
 * file from compiling off Windows. Every body already worked through
 * a float pointer, so the type was never more than a name. */

/* out = a * b, in that order. Aliasing is safe: the result is formed
   in a temporary first, which is what let the call sites that pass
   the destination as an operand keep doing it. */
Mtx4x4WithoutAdditionalData* MTX_p_Multiply(Mtx4x4WithoutAdditionalData* out, const Mtx4x4WithoutAdditionalData* a, const Mtx4x4WithoutAdditionalData* b);

/* Identity with the translation set. */
Mtx4x4WithoutAdditionalData* MTX_p_Translation(Mtx4x4WithoutAdditionalData* out, float x, float y, float z);

/* A right-handed off-centre orthographic projection, mapping the box
   to clip space with z in [0, 1] -- Direct3D's range, not OpenGL's. */
Mtx4x4WithoutAdditionalData* MTX_p_OrthoOffCenterRH(Mtx4x4WithoutAdditionalData* out, float l, float r,
                                      float b, float t, float zn, float zf);

/* The general inverse, by the adjugate over the determinant: the
   matrices that reach it may carry scale or shear, which is why the
   engine's own rigid inverse in math.c is not enough for this one
   caller. Returns NULL and leaves out untouched if the matrix is
   singular, and writes the determinant through det when that is not
   NULL -- both as the library this replaced did. Aliasing is safe. */
Mtx4x4WithoutAdditionalData* MTX_p_Inverse(Mtx4x4WithoutAdditionalData* out, float* det, const Mtx4x4WithoutAdditionalData* in);
