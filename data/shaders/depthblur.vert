#version 450 core

// The depth-blur pass. The world's geometry is drawn a second time
// through a 256 by 1 ramp of blur-amount-per-depth, and only the
// frame's alpha is written -- so what the pass leaves behind is a
// per-pixel blur amount for the after-effects, and for the sun's
// occlusion test, to read.
//
// Only the position is used. The colour and the texture coordinate are
// in the vertex layout and are left alone, because the pass shares the
// world's vertex buffer and its attribute layout.
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec4 aColor;
layout(location = 2) in vec2 aUV;

uniform mat4 uViewProj;
uniform mat4 uView;

// The camera-space position, which is where the ramp's coordinate
// comes from. It is passed across rather than the finished coordinate
// because one of the two forms divides, and a divided value
// interpolates to something else. The transform is affine in this
// position and the interpolation is perspective-correct, so
// transforming here and interpolating is the same as interpolating and
// transforming there -- which is the order the fragment stage uses, so
// that the divide happens per pixel.
out vec3 vCam;

void main()
{
    // The engine's matrices are row-vector and stored row-major, and
    // they are uploaded without transposing -- so OpenGL hands GLSL
    // the transpose, and the transpose times a column vector is the
    // row vector times the original. The same arithmetic as the
    // geometry program's, for the same reason.
    gl_Position = uViewProj * vec4(aPos, 1.0);
    vCam = (uView * vec4(aPos, 1.0)).xyz;
}
