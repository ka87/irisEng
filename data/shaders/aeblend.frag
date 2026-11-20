#version 410 core

in vec2 vUV;
out vec4 oColor;

uniform sampler2D uSrc;

uniform float uAlpha;

void main()
{
    oColor = vec4(texture(uSrc, vUV).rgb, uAlpha);
}
