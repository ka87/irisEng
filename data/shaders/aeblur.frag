#version 410 core

in vec2 vUV;
out vec4 oColor;

uniform sampler2D uSrc;

uniform vec4 uOffsets01;
uniform vec4 uOffsets23;
uniform vec4 uWeights;

void main()
{
    vec3 c;

    c  = texture(uSrc, vUV + uOffsets01.xy).rgb * uWeights.x;
    c += texture(uSrc, vUV + uOffsets01.zw).rgb * uWeights.y;
    c += texture(uSrc, vUV + uOffsets23.xy).rgb * uWeights.z;
    c += texture(uSrc, vUV + uOffsets23.zw).rgb * uWeights.w;

    oColor = vec4(c, 1.0);
}
