#version 410 core

in vec4  vColor;
in vec3  vTex;
in float vDepth;

out vec4 oColor;

uniform sampler2D uReflection;

uniform int uFlipV;

uniform int   uFogCurve;
uniform float uFogStart;
uniform float uFogEnd;
uniform float uFogDensity;
uniform vec3  uFogColor;

float FogFactor(void)
{
    float f;

    if (uFogCurve == 1)
    {
        if (uFogEnd == uFogStart)
            f = vDepth < uFogEnd ? 1.0 : 0.0;
        else
            f = (uFogEnd - vDepth) / (uFogEnd - uFogStart);
    }
    else if (uFogCurve == 2)
    {
        f = exp(-(vDepth * uFogDensity));
    }
    else
    {
        float d = vDepth * uFogDensity;

        f = exp(-(d * d));
    }
    return clamp(f, 0.0, 1.0);
}

void main()
{
    vec2 uv = vTex.z != 0.0 ? vTex.xy / vTex.z : vec2(0.0);

    if (uFlipV != 0)
        uv.y = 1.0 - uv.y;

    vec3 refl = texture(uReflection, clamp(uv, 0.0, 1.0)).rgb;

    vec3 c = mix(vColor.rgb, refl, vColor.a);

    if (uFogCurve != 0)
        c = mix(uFogColor, c, FogFactor());

    oColor = vec4(c, vColor.a);
}
