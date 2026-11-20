#version 410 core

in vec2 vUV;
out vec4 oColor;

uniform sampler2D uSharp;
uniform sampler2D uBlurred;

void main()
{
    vec4 sharp;
    vec3 blurred;

    sharp = texture(uSharp, vUV);
    blurred = texture(uBlurred, vUV).rgb;

    oColor = vec4(mix(blurred, sharp.rgb, sharp.a), sharp.a);
}
