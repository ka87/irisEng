#version 410 core

in vec3 vCam;
out vec4 oColor;

uniform sampler2D uRamp;

uniform bool  uProjected;
uniform float uTranslateU;
uniform float uScaleW;
uniform float uScaleUfromZ;

void main()
{
    float u;

    if (uProjected)
    {
        float w = vCam.z * uScaleW;

        u = w != 0.0 ? uTranslateU / w : 0.0;
    }
    else
    {
        u = vCam.z * uScaleUfromZ + uTranslateU;
    }

    oColor = vec4(0.0, 0.0, 0.0, texture(uRamp, vec2(u, 0.5)).r);
}
