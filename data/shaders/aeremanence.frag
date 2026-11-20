#version 410 core

in vec2 vUV;
out vec4 oColor;

uniform sampler2D uSharp;
uniform sampler2D uBlurred;

uniform float uWeight;

void main()
{
    vec4 sharp = texture(uSharp, vUV);
    vec3 blurred = texture(uBlurred, vUV).rgb;

    oColor = vec4(clamp(sharp.rgb + blurred * uWeight, 0.0, 1.0), sharp.a);
}
