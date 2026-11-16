#version 450 core

// The end of the depth blur: the sharp frame and the blurred copy of
// it, mixed per pixel by the sharp frame's own alpha.
//
// That alpha is the blur amount the depth-blur pass wrote, and it runs
// the other way from what the name suggests -- 255 is no blur and 0 is
// full blur, which is why the ramp behind it is solid 255 when no blur
// is in force. So alpha one takes the sharp pixel whole.
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

    // The frame's alpha travels on unchanged. Nothing downstream reads
    // it today -- the copy to the screen takes colour only -- but the
    // effects that come after this one are mixed the same way, and a
    // mix that threw the alpha away would silently make every one of
    // them a no-op.
    oColor = vec4(mix(blurred, sharp.rgb, sharp.a), sharp.a);
}
