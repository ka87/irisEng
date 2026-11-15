#version 450 core

// The blur amount at this pixel's depth, written to alpha and nowhere
// else. The colour write is off for this pass, so the first three
// components go nowhere and are not worth computing.
//
// Alpha runs the other way from what the name suggests: 255 is no blur
// and 0 is full. The ramp is solid 255 when no blur is in force, which
// is what says so.
in vec3 vCam;
out vec4 oColor;

// 256 by 1, one blur amount per depth, in the red channel.
uniform sampler2D uRamp;

// How the camera-space position becomes the ramp's coordinate. Two
// forms, and which one is in force depends on the blur mode:
//
//   projected     u = uTranslateU / (z * uScaleW)
//                 the reciprocal form. u reaches the end of the ramp at
//                 a depth of uTranslateU and falls towards the start as
//                 the depth grows, so blur rises with distance.
//
//   not projected u = z * uScaleUfromZ + uTranslateU
//                 the linear form, which the sun's mode uses with
//                 uScaleUfromZ at one over the far plane -- so u is the
//                 depth as a fraction of the frustum.
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

        // A pixel exactly on the camera plane has nothing to divide
        // by. The other renderer's divide is undefined there too, so
        // this is a defined answer where there was none rather than a
        // change of one.
        u = w != 0.0 ? uTranslateU / w : 0.0;
    }
    else
    {
        u = vCam.z * uScaleUfromZ + uTranslateU;
    }

    // Clamped by the sampler, so a coordinate outside the ramp takes
    // the nearest end.
    oColor = vec4(0.0, 0.0, 0.0, texture(uRamp, vec2(u, 0.5)).r);
}
