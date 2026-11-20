#version 410 core

in vec2 vUV;
out vec4 oColor;

uniform sampler2D uSrc;

uniform float uGain;
uniform float uBias;

uniform float uGrayscale;

uniform vec3 uWeights;

uniform float uBlend;

void main()
{
    vec4 src = texture(uSrc, vUV);
    vec3 perChannel = src.rgb * uGain + uBias;
    vec3 summed = vec3(dot(src.rgb, uWeights) * uGain + uBias);

    vec3 stepped = clamp(mix(perChannel, summed, uGrayscale), 0.0, 1.0);

    oColor = vec4(mix(src.rgb, stepped, uBlend), src.a);
}
