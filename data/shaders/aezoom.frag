#version 410 core

in vec2 vUV;
out vec4 oColor;

uniform sampler2D uSrc;

uniform vec2 uBandLo;
uniform vec2 uBandHi;

uniform vec2 uInset;

uniform int   uTaps;
uniform float uWeight;

void main()
{
    vec2 at = (vUV - uBandLo) / (uBandHi - uBandLo);
    vec3 c = vec3(0.0);
    int  i;

    for (i = 0; i < uTaps; i++)
    {
        vec2 inset = uInset * float(i);
        vec2 lo = uBandLo + inset;
        vec2 hi = uBandHi - inset;

        c += texture(uSrc, mix(lo, hi, at)).rgb * uWeight;
    }

    oColor = vec4(clamp(c, 0.0, 1.0), texture(uSrc, vUV).a);
}
