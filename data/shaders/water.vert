#version 410 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aDir;
layout(location = 2) in vec4 aColor;

uniform mat4 uViewProj;
uniform mat4 uView;
uniform mat4 uTexMtx;

out vec4  vColor;
out vec3  vTex;
out float vDepth;

void main()
{
    gl_Position = uViewProj * vec4(aPos, 1.0);

    vec3 camDir = (uView * vec4(aDir, 0.0)).xyz;
    vTex = (uTexMtx * vec4(camDir, 1.0)).xyz;

    vDepth = (uView * vec4(aPos, 1.0)).z;
    vColor = aColor.bgra;
}
