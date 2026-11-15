#version 450 core

// The world's geometry: the texture modulated by the vertex colour,
// which is what the other renderer's texture stage computes.
in vec4 vColor;
in vec2 vUV;
out vec4 oColor;

uniform sampler2D uTex;

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

void main()
{
    vec4 c = vColor;

    if (uTextured != 0)
    {
        c *= texture(uTex, vUV);
        if (uDiffuse2x != 0)
            c.rgb *= 2.0;
    }

    // Discarding rather than writing: the depth and stencil writes
    // have to be suppressed too, which is what the alpha test was for.
    if (uAlphaMode == 1 && !(c.a > uAlphaRef))
        discard;
    if (uAlphaMode == 2 && !(c.a < uAlphaRef))
        discard;

    oColor = c;
}
