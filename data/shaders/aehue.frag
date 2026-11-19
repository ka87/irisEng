#version 410 core

// The hue sweep: a colour taken away from the picture, saturating at
// black.
//
// A different amount from each channel, and the three amounts are one
// wave sampled a third of a cycle apart -- so one channel is left
// alone while the others are darkened, and which one that is sweeps
// round as the phase advances. All of that arithmetic is aehue.c's;
// what arrives here is the three amounts, already as fractions.
in vec2 vUV;
out vec4 oColor;

uniform sampler2D uSrc;

// What comes off each channel, red green blue. Three numbers and not
// one, which is the whole effect: the same amount off all three would
// only be a brightness.
uniform vec3 uSubtract;

void main()
{
    vec4 src = texture(uSrc, vUV);

    // Saturating at black and not wrapping. The other renderer's
    // subtract clamps the same way, and the target is eight-bit
    // normalised so it would clamp anyway -- written out so that a
    // target of some other format later does not quietly change it.
    //
    // The alpha is the picture's own, carried rather than computed,
    // and the renderer masks the alpha write off before this draw --
    // so what is here only matters if that mask is ever dropped. The
    // other renderer does both of those too.
    oColor = vec4(max(src.rgb - uSubtract, vec3(0.0)), src.a);
}
