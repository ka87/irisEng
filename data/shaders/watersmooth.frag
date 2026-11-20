#version 410 core

in vec2 vTap0;
in vec2 vTap1;
in vec2 vTap2;
in vec2 vTap3;

out vec4 oColor;

uniform sampler2D uSrc;
uniform vec4 uWeights;

void main()
{
    vec3 c = texture(uSrc, vTap0).rgb * uWeights.x
           + texture(uSrc, vTap1).rgb * uWeights.y
           + texture(uSrc, vTap2).rgb * uWeights.z
           + texture(uSrc, vTap3).rgb * uWeights.w;

    oColor = vec4(c, texture(uSrc, vTap0).a);
}
