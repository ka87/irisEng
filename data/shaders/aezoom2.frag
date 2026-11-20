#version 410 core

in vec2 vUV;
out vec4 oColor;

uniform sampler2D uSrc;

uniform mat4 uTapsA;
uniform mat4 uTapsB;

uniform int uTapCount;

uniform float uWeight;

void main()
{
    vec2 d = vUV - vec2(0.5);
    vec3 sum = vec3(0.0);
    int  i;

    for (i = 0; i < uTapCount; i++)
    {
        vec4 t = (i < 4) ? uTapsA[i] : uTapsB[i - 4];

        sum += texture(uSrc, vec2(0.5)
                             + vec2(dot(t.xy, d), dot(t.zw, d))).rgb;
    }

    oColor = vec4(sum * uWeight, texture(uSrc, vUV).a);
}
