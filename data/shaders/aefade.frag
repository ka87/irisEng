#version 410 core

in vec2 vUV;
out vec4 oColor;

uniform sampler2D uLive;
uniform sampler2D uFrozen;

uniform float uFrozenShare;

void main()
{
    vec4 live = texture(uLive, vUV);

    oColor = vec4(mix(live.rgb, texture(uFrozen, vUV).rgb, uFrozenShare),
                  live.a);
}
