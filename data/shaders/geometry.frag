#version 450 core

// The world's geometry: the texture modulated by the vertex colour,
// which is what the other renderer's texture stage computes.
in vec4  vColor;
in vec2  vUV;
in float vDepth;
in vec3  vCam;
out vec4 oColor;

uniform sampler2D uTex;

// The material's texture-coordinate projection. The engine computes a
// transform and says which source it applies to:
//
//   0  the stored coordinate, untransformed
//   1  the camera-space position, transformed
//   3  the stored coordinate, transformed
//
// Mode 2 takes the camera-space normal and is not here: this
// renderer's vertex carries no normal, so the renderer says so and
// falls back to mode 0 rather than making one up.
//
// The two transformed modes do not agree on the input's shape, and
// that is the engine's doing rather than a guess. A generated
// coordinate is a point, so its fourth component is one and the
// transform's last row is its translation. A stored coordinate is two
// numbers, so the engine moves the translation up into the third row
// before handing the matrix over -- which is only necessary if the
// fourth component is zero there. So one is (x, y, z, 1) and the other
// is (u, v, 1, 0).
uniform int  uUVMode;
uniform mat4 uUVMtx;

vec2 TexCoord(void)
{
    if (uUVMode == 1)
        return (uUVMtx * vec4(vCam, 1.0)).xy;
    if (uUVMode == 3)
        return (uUVMtx * vec4(vUV, 1.0, 0.0)).xy;
    return vUV;
}

// Whether there is a texture at all. A material can ask for none, and
// then the vertex colour is the colour; the engine says so by handing
// the renderer an index of -1 rather than by binding nothing.
uniform int uTextured;

// Core OpenGL has no alpha test, so it is done here. 0 is off, 1 keeps
// a fragment whose alpha is above the reference, 2 keeps one below --
// the two senses the material's flags select between.
uniform int   uAlphaMode;
uniform float uAlphaRef;

// A display setting that doubles the modulated colour, which the other
// renderer asks its texture stage for. Colour only: the alpha is
// modulated either way.
uniform int uDiffuse2x;

// Fog. uFogCurve is 0 for none, 1 linear, 2 exponential, 3 exponential
// squared -- fog.h's own numbering. The factor is how much of the
// pixel survives, so 1 is no fog, which is the sense the hardware's
// fog factor has.
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
        // A range of nothing has no ramp to be on; the far end is a
        // defined answer where the division has none.
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

    // Discarding rather than writing: the depth and stencil writes
    // have to be suppressed too, which is what the alpha test was for.
    if (uAlphaMode == 1 && !(c.a > uAlphaRef))
        discard;
    if (uAlphaMode == 2 && !(c.a < uAlphaRef))
        discard;

    // After the alpha test, and the colour only: the hardware's fog
    // leaves alpha alone, and the depth-blur pass reads that alpha.
    if (uFogCurve != 0)
        c.rgb = mix(uFogColor, c.rgb, FogFactor());

    oColor = c;
}
