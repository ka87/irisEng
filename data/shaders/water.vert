#version 450 core

// The water surface: a grid the simulation builds, drawn once a frame
// with the reflection projected onto it.
//
// Three attributes, and the middle one is the odd one. aDir is not a
// normal, whatever a stream declaration elsewhere calls it: it is the
// vector from the camera to this grid point, with its depth component
// already pushed about by the wave, and it exists to be turned into a
// texture coordinate rather than lit. The pushing is done on the
// engine's side, in watermesh.c, because that is where the wave array
// is.
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aDir;
layout(location = 2) in vec4 aColor;

uniform mat4 uViewProj;
// To camera space, twice over: the direction above goes through its
// rotation, and the fog needs the depth that the combined matrix has
// already thrown away.
uniform mat4 uView;
// The projection, with the clip cube remapped to the reflection's
// nought-to-one. Built in watermesh.c from the same projection the
// world was drawn with, so the reflection lands where the world it
// mirrors did.
uniform mat4 uTexMtx;

out vec4  vColor;
// Projective: the pair to sample with is xy over z, and the divide is
// left to the fragment stage so it happens per pixel rather than being
// interpolated after the fact.
out vec3  vTex;
out float vDepth;

void main()
{
    // The engine's matrices are row-vector and stored row-major, and
    // they are uploaded untransposed: OpenGL reads that storage as
    // column-major and hands GLSL the transpose, and the transpose
    // times a column is the row times the original. So this is the
    // engine's own arithmetic. The same note is on geometry.vert.
    gl_Position = uViewProj * vec4(aPos, 1.0);

    // A direction, so the view's translation is dropped -- hence the
    // nought. The length is deliberately left alone: the reflection
    // path of the renderer this came from turns normal normalising off
    // for this draw by hand, and the length does not cancel in the
    // divide below, because the remap adds its own row.
    vec3 camDir = (uView * vec4(aDir, 0.0)).xyz;
    vTex = (uTexMtx * vec4(camDir, 1.0)).xyz;

    vDepth = (uView * vec4(aPos, 1.0)).z;
    vColor = aColor.bgra;
}
