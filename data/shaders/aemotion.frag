#version 450 core

// The motion smooth: four samples of the frame along the way the
// camera just moved, summed.
//
// Where the four sit and what they are weighted by is aemotion.c's.
// What is worth knowing here is that the four are **not** a symmetric
// spread about the pixel: they are at minus three, plus two, minus one
// and nothing times the step. So this is not a blur centred on
// anything -- it leans one way, and that is the effect.
in vec2 vUV;
out vec4 oColor;

uniform sampler2D uSrc;

// The four offsets, across and down, one component each. Packed as two
// vectors rather than four pairs so that the order they are summed in
// is the order they are written in, and visibly so.
uniform vec4 uTapU;
uniform vec4 uTapV;

// What each sample arrives with. Not a quarter: the renderer this came
// from carries 63 of 255 in a vertex colour, so the four together come
// to 252 of 255 and the smeared picture is a shade darker than the one
// it was made from.
uniform float uWeight;

void main()
{
    vec3 sum = vec3(0.0);
    int  i;

    // Summed in the order the offsets are given, which is the order
    // the other renderer binds its four stages in. Eight-bit
    // arithmetic is not associative either, so the order is part of
    // the answer.
    for (i = 0; i < 4; i++)
        sum += texture(uSrc, vUV + vec2(uTapU[i], uTapV[i])).rgb;

    // The alpha is the picture's own and the renderer masks the alpha
    // write off before this draw, so what is here only matters if that
    // mask is ever dropped. The other renderer writes its vertex
    // colour's alpha into the same masked-off channel.
    oColor = vec4(sum * uWeight, texture(uSrc, vUV).a);
}
