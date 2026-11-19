#version 410 core

// The remanence: the sharp frame with a wide blur of it added on top.
//
// A bloom, whatever its name suggests -- nothing here reads the frame
// before. The blur is four passes reaching one, three, five and seven
// pixels, which is the same pass the depth blur uses at twice the
// reach.
in vec2 vUV;
out vec4 oColor;

uniform sampler2D uSharp;
uniform sampler2D uBlurred;

// How much of the blurred copy goes in. Twice the effect's own
// parameter, because the renderer this was read from adds the blurred
// copy in two successive stages carrying the same weight and the same
// coordinates -- one addition said twice, not two samples.
//
// It can exceed one, and that is the point: the sum saturates, and a
// bloom that could not saturate would only be a haze.
uniform float uWeight;

void main()
{
    vec4 sharp = texture(uSharp, vUV);
    vec3 blurred = texture(uBlurred, vUV).rgb;

    // Saturating is the effect, so the clamp is on purpose rather than
    // a guard -- though an eight-bit target would do it anyway.
    //
    // The alpha travels on: the effects after this one are mixed by
    // it, and this one has no business changing it.
    oColor = vec4(clamp(sharp.rgb + blurred * uWeight, 0.0, 1.0), sharp.a);
}
