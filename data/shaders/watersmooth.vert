#version 410 core

layout(location = 0) in vec2 aPos;
layout(location = 1) in vec2 aCorner;

uniform vec2 uSrcU;
uniform vec2 uSrcV;
uniform float uStep;
uniform float vStep;

out vec2 vTap0;
out vec2 vTap1;
out vec2 vTap2;
out vec2 vTap3;

void main()
{
    gl_Position = vec4(aPos, 0.0, 1.0);

    float u = mix(uSrcU.x, uSrcU.y, aCorner.x);
    float v = mix(uSrcV.x, uSrcV.y, aCorner.y);

    float ramp = aCorner.y;

    vTap0 = vec2(u,         v - ramp * 0.0 * vStep);
    vTap1 = vec2(u - uStep, v - ramp * 1.0 * vStep);
    vTap2 = vec2(u + uStep, v - ramp * 2.0 * vStep);
    vTap3 = vec2(u,         v - ramp * 3.0 * vStep);
}
