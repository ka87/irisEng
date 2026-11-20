#version 410 core

in vec2 vUV;
out vec4 oColor;

uniform sampler2D uSrc;

uniform vec3 uSubtract;

void main()
{
    vec4 src = texture(uSrc, vUV);

    oColor = vec4(max(src.rgb - uSubtract, vec3(0.0)), src.a);
}
