#pragma once

#include "math.h"
#include "gameobject.h"

/* The water's reflection, as arithmetic, with no renderer in it.
 *
 * The water is two passes and this is the first of them. Before the
 * world is drawn, a target is cleared to the colour the water is
 * looking down into and the camera is kept as it stands; then, as each
 * object is drawn, it is drawn a **second time** into that target with
 * its transform mirrored in the water plane. The surface drawn later
 * samples the result -- that part is watermesh.c.
 *
 * What is here is the three decisions that pass makes and that have
 * nothing to do with which graphics interface is drawing: whether an
 * object is worth reflecting at all, what its mirrored transform is,
 * and where the plane sits that keeps the reflection under the water.
 *
 * None of it is invented. Every number is the one the renderer that
 * already had this computes, and tests/run_d3drender.sh drives that
 * renderer and this side by side over the same objects and compares
 * the matrices and the device calls. That comparison is the warrant
 * for the file.
 */

/* How big the renderer considers an object to be.
 *
 * `b_sphere` is the engine's own answer about the bounding volume's
 * kind, asked by the caller rather than here, because answering it
 * means reading a sentinel out of a field that is a coordinate the
 * rest of the time -- which is the engine's business and not this
 * file's. */
float WRF_f_Extent(const tGameObjectBV* bv, int b_sphere);

/* Whether the object earns a second draw. Small things cost as much to
 * reflect as large ones and are not visible in the water, so there is
 * a threshold, and it is a debug tweak that starts at nought -- where
 * everything is worth it. */
int WRF_b_WorthReflecting(const tGameObjectBV* bv, int b_sphere,
                          float minimum);

/* The object's transform, mirrored in the water plane.
 *
 * `objectToWorld` is the engine's matrix for the object, taken as it
 * is found; `out` may be the same matrix. The result maps object space
 * to the *reflected* world, so the view that follows it is the
 * camera's own rather than the combined object-and-camera matrix the
 * ordinary draw uses -- which is the one thing a caller has to get
 * right, and the reason the reflection cannot reuse the matrix already
 * set up for the object. */
void WRF_Mirror(Mtx4x4* out, Mtx4x4* objectToWorld, float waterHeight);

/* The plane that keeps the reflection below the surface, as the four
 * coefficients of `ax + by + cz + d`, with the kept half-space the one
 * where that is positive. `offset` is a debug tweak for nudging it off
 * the water line. */
void WRF_ClipPlane(float out[4], float waterHeight, float offset);
