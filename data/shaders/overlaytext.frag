#version 410 core

in vec4 vColor;
in vec2 vUV;
out vec4 oColor;

uniform sampler2D uAtlas;

void main()
{
    float coverage = texture(uAtlas, vUV).r;

    oColor = vec4(vColor.rgb, vColor.a * coverage);
}
