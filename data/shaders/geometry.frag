#version 410 core

in vec4  vColor;
in vec2  vUV;
in float vDepth;
in vec3  vCam;
in vec3  vNrm;
out vec4 oColor;

uniform sampler2D uTex;

uniform int  uUVMode;
uniform mat4 uUVMtx;

vec2 TexCoord(void)
{
    if (uUVMode == 1)
        return (uUVMtx * vec4(vCam, 1.0)).xy;
    if (uUVMode == 2)
        return (uUVMtx * vec4(normalize(vNrm), 1.0)).xy;
    if (uUVMode == 3)
        return (uUVMtx * vec4(vUV, 1.0, 0.0)).xy;
    return vUV;
}

uniform int uTextured;

uniform int   uAlphaMode;
uniform float uAlphaRef;

uniform int uDiffuse2x;

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
    vec4 c = vColor;

    if (uTextured != 0)
    {
        c *= texture(uTex, TexCoord());
        if (uDiffuse2x != 0)
            c.rgb *= 2.0;
    }

    if (uAlphaMode == 1 && !(c.a > uAlphaRef))
        discard;
    if (uAlphaMode == 2 && !(c.a < uAlphaRef))
        discard;

    if (uFogCurve != 0)
        c.rgb = mix(uFogColor, c.rgb, FogFactor());

    oColor = c;
}
