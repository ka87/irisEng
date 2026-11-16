#version 450 core

// One step of the colour after-effects: brightness, contrast or
// grayscale. All three are a gain and a bias, and the grayscale
// applies them to a weighted sum of the channels instead of to each
// channel on its own -- so one shader covers them, with the weights
// deciding which it is.
//
// One draw per step rather than all three in one pass. The renderer
// that has these runs each as its own draw into an eight-bit target,
// so a channel pushed past white by one step is white before the next
// sees it. Doing them in one pass would need a clamp between them to
// match, and would still differ by the rounding the eight-bit target
// does; a draw each matches outright and costs a pass nobody will
// measure.
in vec2 vUV;
out vec4 oColor;

uniform sampler2D uSrc;

// The channel's multiply and add.
uniform float uGain;
uniform float uBias;

// How much of this step is a grayscale: 1 takes the weighted sum, 0
// takes the channel. Written as a number rather than a branch so that
// the two arms cost the same and the shader has no step kind of its
// own to keep in step with the engine's.
uniform float uGrayscale;

// The weights the grayscale sums with. They are green-heavy,
// blue-light, and they do not come to one -- so white does not come
// back white, which is the renderer's own arithmetic and not an
// oversight here.
uniform vec3 uWeights;

void main()
{
    vec4 src = texture(uSrc, vUV);
    vec3 perChannel = src.rgb * uGain + uBias;
    vec3 summed = vec3(dot(src.rgb, uWeights) * uGain + uBias);

    // The clamp is belt and braces rather than the thing that makes a
    // step come out right: the target is eight-bit normalised, so a
    // channel outside nought to one is clamped on the way into it
    // whatever this says. That is also how the other renderer gets
    // its clamp between steps, and why a draw per step reproduces it
    // without modelling it. Written out all the same, so a target of
    // some other format later on does not quietly change the answer.
    //
    // The alpha travels on. Nothing in this group reads it or writes
    // it -- the renderer that has them narrows the write to colour --
    // and the effects that mix by it still need it.
    oColor = vec4(clamp(mix(perChannel, summed, uGrayscale), 0.0, 1.0),
                  src.a);
}
