#pragma once
#include <d3d9.h>

/* The four matrix operations the renderer used to get from D3DX.
   Direct3D's conventions throughout: row-major storage, row vectors,
   so a point is transformed as v * M and a translation lives in the
   fourth row.

   They are here rather than in math.c because the conventions are
   Direct3D's, not the engine's -- math.c works in the engine's own
   Mtx4x4, and a projection's depth range is a renderer's business. A
   second renderer wanting an orthographic projection will want its
   own, with its own clip space. */

/* out = a * b, in that order. Aliasing is safe: the result is formed
   in a temporary first, which is what let the call sites that pass
   the destination as an operand keep doing it. */
D3DMATRIX* D3D_MatrixMultiply(D3DMATRIX* out, const D3DMATRIX* a, const D3DMATRIX* b);

/* Identity with the translation set. */
D3DMATRIX* D3D_MatrixTranslation(D3DMATRIX* out, float x, float y, float z);

/* A right-handed off-centre orthographic projection, mapping the box
   to clip space with z in [0, 1] -- Direct3D's range, not OpenGL's. */
D3DMATRIX* D3D_MatrixOrthoOffCenterRH(D3DMATRIX* out, float l, float r,
                                      float b, float t, float zn, float zf);

/* The general inverse, by the adjugate over the determinant: the
   matrices that reach it may carry scale or shear, which is why the
   engine's own rigid inverse in math.c is not enough for this one
   caller. Returns NULL and leaves out untouched if the matrix is
   singular, and writes the determinant through det when that is not
   NULL -- both as D3DX did. Aliasing is safe. */
D3DMATRIX* D3D_p_MatrixInverse(D3DMATRIX* out, float* det, const D3DMATRIX* in);
