#version 450 core

// A movie frame, stretched onto the screen. The vertex stage is the
// glyphs' one -- it already maps pixel coordinates to clip space and
// carries a texture coordinate -- so this is the whole difference
// between the two: a glyph's texture is coverage and decides how much
// of the vertex colour there is, and a frame's texture is the picture.
in vec4 vColor;
in vec2 vUV;
out vec4 oColor;

uniform sampler2D uAtlas;

void main()
{
    // The frame as it is, and opaque whatever its alpha says: the
    // engine's decoder writes into a surface whose fourth channel is
    // padding, and the other renderer's blit ignores it too.
    oColor = vec4(texture(uAtlas, vUV).rgb, 1.0);
}
