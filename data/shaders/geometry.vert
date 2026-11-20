#version 410 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec4 aColor;
layout(location = 2) in vec2 aUV;
layout(location = 3) in vec3 aNormal;

uniform mat4 uViewProj;
uniform int  uClipping;
uniform vec4 uClipPlane;
uniform mat4 uWorld;
uniform mat4 uView;

out vec4  vColor;
out vec2  vUV;
out float vDepth;
out vec3  vCam;
out vec3  vNrm;

void main()
{
    gl_Position = uViewProj * vec4(aPos, 1.0);

    gl_ClipDistance[0] = uClipping != 0
        ? dot(uClipPlane, vec4((uWorld * vec4(aPos, 1.0)).xyz, 1.0))
        : 1.0;

    vColor = aColor.bgra;
    vUV = aUV;

    vCam = (uView * vec4(aPos, 1.0)).xyz;
    vNrm = (uView * vec4(aNormal, 0.0)).xyz;

    vDepth = vCam.z;
}
