#version 450 core

// The world's geometry. One matrix, because the engine hands over a
// view and a projection and the renderer multiplies them before they
// get here.
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec4 aColor;
layout(location = 2) in vec2 aUV;

uniform mat4 uViewProj;
// To camera space, for the fog: fog is a function of the distance from
// the camera, and that is the one thing the combined matrix has
// already thrown away.
uniform mat4 uView;

out vec4  vColor;
out vec2  vUV;
out float vDepth;

void main()
{
    // The engine's matrices are row-vector: a point is transformed as
    // v * M, and they are stored row-major. Uploaded without
    // transposing, OpenGL reads that storage as column-major, which
    // hands GLSL the transpose -- and the transpose times a column
    // vector is the row vector times the original. So this is the
    // engine's own arithmetic, not a rearrangement of it.
    gl_Position = uViewProj * vec4(aPos, 1.0);

    // As in the overlay: a packed A8R8G8B8 word read as four
    // normalised bytes arrives blue, green, red, alpha.
    vColor = aColor.bgra;
    vUV = aUV;

    // The camera-space depth, interpolated perspective-correctly, as
    // the hardware's own fog does.
    //
    // Not negated. The engine's camera space has z growing away from
    // the camera, and the authority on that is the depth-blur pass:
    // its coordinate is the depth over the far plane and it multiplies
    // z by one over the far plane, with no sign change. The two read
    // the same depth, so they agree or one of them is wrong. A first
    // draft negated this and fogged nothing at all, because every
    // factor clamped to one.
    vDepth = (uView * vec4(aPos, 1.0)).z;
}
