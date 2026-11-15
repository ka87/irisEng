#version 450 core

// The same as overlay.vert with a texture coordinate carried through.
// It is a separate program rather than a branch, because the two
// vertex layouts differ and a vertex array has to describe one of
// them.
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec4 aColor;
layout(location = 2) in vec2 aUV;

uniform vec4 uViewport;

out vec4 vColor;
out vec2 vUV;

void main()
{
    float x = aPos.x / uViewport.x * 2.0 - 1.0;
    float y = 1.0 - aPos.y / uViewport.y * 2.0;

    gl_Position = vec4(x, y, 0.0, 1.0);
    vColor = aColor.bgra;
    vUV = aUV;
}
