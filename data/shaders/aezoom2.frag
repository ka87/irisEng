#version 450 core

// The second zoom smooth: eight samples of the frame, each turned
// about the middle of the picture by a different angle, summed.
//
// All the arithmetic is aezoom2.c's, including the thing worth knowing
// about it: the transform is written as a rotation and is not one --
// its second row reads the first row's answer, so its determinant is
// cos squared rather than one and every sample is turned *and* shrunk.
// That is what makes a rotational smear look like a zoom, and why the
// effect is named as it is.
//
// What arrives here is eight such transforms, already conjugated by
// the frame's aspect and already with the vertical the way round this
// renderer measures it. Nothing is rebuilt from an angle.
in vec2 vUV;
out vec4 oColor;

uniform sampler2D uSrc;

// The eight transforms, four to a matrix, **one per column**: a
// column is (a, b, c, d) of the two-by-two in row order. Packed this
// way because two of these hold exactly eight taps and the renderer
// has a four-by-four uniform to hand, where it has no vector-array
// one.
uniform mat4 uTapsA;
uniform mat4 uTapsB;

// How many of the eight are really sampled. Eight unless the card
// reports exactly three texture stages, where the other renderer takes
// six -- which this renderer never does, but the count comes from the
// same place either way.
uniform int uTapCount;

// What each sample is multiplied by. Not an eighth: `256/8 - 1` of
// 255, so the eight together come to 248 of 255 and the smear is a
// shade dark rather than saturating.
uniform float uWeight;

void main()
{
    // Measured from the middle of the picture, which is where the turn
    // is about.
    vec2 d = vUV - vec2(0.5);
    vec3 sum = vec3(0.0);
    int  i;

    for (i = 0; i < uTapCount; i++)
    {
        vec4 t = (i < 4) ? uTapsA[i] : uTapsB[i - 4];

        sum += texture(uSrc, vec2(0.5)
                             + vec2(dot(t.xy, d), dot(t.zw, d))).rgb;
    }

    // The turn pushes the corners outside the quad, and the targets
    // clamp at their edges -- so a corner sample repeats the edge
    // rather than wrapping. The other renderer leaves its addressing
    // mode alone here and inherits whatever the last pass set, so this
    // is the sane reading rather than a matched one.
    //
    // The alpha is the picture's own and the renderer masks the alpha
    // write off before this draw, as the other renderer narrows its
    // colour write.
    oColor = vec4(sum * uWeight, texture(uSrc, vUV).a);
}
