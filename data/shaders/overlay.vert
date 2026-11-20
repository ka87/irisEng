#version 410 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec4 aColor;

uniform vec4 uViewport;

out vec4 vColor;

void main()
{
    float x = aPos.x / uViewport.x * 2.0 - 1.0;
    float y = 1.0 - aPos.y / uViewport.y * 2.0;

    gl_Position = vec4(x, y, 0.0, 1.0);

    vColor = aColor.bgra;
}
