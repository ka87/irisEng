#version 410 core

in vec4 vColor;
in vec2 vUV;
out vec4 oColor;

uniform sampler2D uAtlas;

void main()
{
    oColor = vec4(texture(uAtlas, vUV).rgb, 1.0);
}
