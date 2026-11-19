#version 410 core

// The world's geometry. One matrix, because the engine hands over a
// view and a projection and the renderer multiplies them before they
// get here.
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec4 aColor;
layout(location = 2) in vec2 aUV;
// The engine's normal, for the one projection that takes its
// coordinate from it. The engine keeps normals in an array of their
// own, so this is a buffer of its own too, and the attribute is
// enabled only for a draw that has them -- with the array disabled it
// reads the generic value, which is what the other modes want and
// never look at.
layout(location = 3) in vec3 aNormal;

uniform mat4 uViewProj;
/* The water's reflection, and nothing else, uses these three.
 *
 * The reflection draws the world a second time with each object
 * mirrored in the water plane, and then has to throw away whatever
 * ends up above the surface -- the parts of the world that could not
 * be seen in water. The plane is in world space, so the vertex has to
 * get there, and for this draw only the renderer knows the object's
 * world transform separately: the ordinary draw is handed a view
 * matrix with the object already folded into it. Hence uWorld, which
 * is the mirrored transform and is read nowhere else.
 *
 * uClipping is nought for every other draw, and then the distance
 * written below is a constant positive and clips nothing. */
uniform int  uClipping;
uniform vec4 uClipPlane;
uniform mat4 uWorld;
// To camera space, for the fog: fog is a function of the distance from
// the camera, and that is the one thing the combined matrix has
// already thrown away.
uniform mat4 uView;

out vec4  vColor;
out vec2  vUV;
out float vDepth;
// The camera-space position, for the generated texture coordinates:
// one of the material's projections takes its coordinate from it.
out vec3  vCam;
out vec3  vNrm;

void main()
{
    // The engine's matrices are row-vector: a point is transformed as
    // v * M, and they are stored row-major. Uploaded without
    // transposing, OpenGL reads that storage as column-major, which
    // hands GLSL the transpose -- and the transpose times a column
    // vector is the row vector times the original. So this is the
    // engine's own arithmetic, not a rearrangement of it.
    gl_Position = uViewProj * vec4(aPos, 1.0);

    // Written whether or not it is enabled: a clip distance that is
    // switched on and never written is undefined, and writing a
    // positive constant is what "no plane" means.
    gl_ClipDistance[0] = uClipping != 0
        ? dot(uClipPlane, vec4((uWorld * vec4(aPos, 1.0)).xyz, 1.0))
        : 1.0;

    // As in the overlay: a packed A8R8G8B8 word read as four
    // normalised bytes arrives blue, green, red, alpha.
    vColor = aColor.bgra;
    vUV = aUV;

    vCam = (uView * vec4(aPos, 1.0)).xyz;
    // A direction, so the fourth component is zero and the view
    // matrix's translation does not reach it.
    vNrm = (uView * vec4(aNormal, 0.0)).xyz;

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
    vDepth = vCam.z;
}
