#version 410 core

in vec2 vUV;
out vec4 oColor;

uniform sampler2D uSrc;

uniform vec4 uTapU;
uniform vec4 uTapV;

uniform float uWeight;

void main()
{
    vec3 sum = vec3(0.0);
    int  i;

    for (i = 0; i < 4; i++)
        sum += texture(uSrc, vUV + vec2(uTapU[i], uTapV[i])).rgb;

    oColor = vec4(sum * uWeight, texture(uSrc, vUV).a);
}
