#version 450 core

// Glyphs. The atlas is single-channel coverage, so the colour comes
// entirely from the vertex and the texture decides only how much of
// it there is.
in vec4 vColor;
in vec2 vUV;
out vec4 oColor;

uniform sampler2D uAtlas;

void main()
{
    float coverage = texture(uAtlas, vUV).r;

    oColor = vec4(vColor.rgb, vColor.a * coverage);
}
