#version 410 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec4 aColor;
layout(location = 2) in vec2 aUV;

uniform mat4 uViewProj;
uniform mat4 uView;

out vec3 vCam;

void main()
{
    gl_Position = uViewProj * vec4(aPos, 1.0);
    vCam = (uView * vec4(aPos, 1.0)).xyz;
}
