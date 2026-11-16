#version 450 core

in vec4  vColor;
in vec3  vTex;
in float vDepth;

out vec4 oColor;

uniform sampler2D uReflection;

// Which way up the reflection is sampled.
//
// It is a uniform rather than a constant on purpose. The two renderers'
// window spaces differ by a vertical flip, and the remap this samples
// through carries a flip of its own; working out what the two come to
// together is exactly the kind of derivation that has already been
// wrong once here -- the geometry culling was reasoned out from the
// same pair of conventions and came out backwards, invisibly, with two
// test harnesses agreeing with it. So the answer is measured in a real
// scene instead, and the console tweak gl_water_flip moves it while
// somebody looks.
uniform int uFlipV;

// fog.h's own numbering: 0 none, 1 linear, 2 exponential, 3 exponential
// squared. The factor is how much of the pixel survives.
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
    // The divide that makes the coordinate projective. A grid point
    // behind the camera puts a nought or a negative here; the clamp
    // below is what keeps that from fetching somewhere arbitrary, and
    // the sampler's own edge clamping does the rest.
    vec2 uv = vTex.z != 0.0 ? vTex.xy / vTex.z : vec2(0.0);

    if (uFlipV != 0)
        uv.y = 1.0 - uv.y;

    vec3 refl = texture(uReflection, clamp(uv, 0.0, 1.0)).rgb;

    // The surface is not blended with what is behind it. The water's
    // own colour and the reflection are mixed here by the vertex
    // alpha, and the result replaces the pixel -- which is right,
    // because what should show through the water is the reflection and
    // not the world above it. The renderer this came from says the
    // same thing the long way round, with a texture stage that blends
    // by the diffuse alpha and a blend equation that then keeps only
    // the source.
    vec3 c = mix(vColor.rgb, refl, vColor.a);

    // Colour only, after the mix: the fog of the interface this came
    // from leaves alpha alone.
    if (uFogCurve != 0)
        c = mix(uFogColor, c, FogFactor());

    oColor = vec4(c, vColor.a);
}
