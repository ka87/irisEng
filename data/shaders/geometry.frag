#version 450 core

// Untextured for now: the colour is the vertex's. The texture
// coordinate is carried through so that the layout matches what the
// engine's geometry vertex holds, and so this stays the shader to
// extend when textures arrive.
in vec4 vColor;
in vec2 vUV;
out vec4 oColor;

void main()
{
    oColor = vColor;
}
