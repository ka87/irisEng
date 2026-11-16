#version 450 core

// The zoom smooth: the frame added to itself eight times, each at a
// slightly tighter crop about the centre of the band -- so a pixel is
// smeared along the line from the centre through it.
//
// Eight taps in one pass. The renderer this was read from takes four
// at a time and runs two passes, because four texture stages was all
// it had; the sum is the same and there is no clamping between its two
// passes to reproduce, since every tap only adds.
in vec2 vUV;
out vec4 oColor;

uniform sampler2D uSrc;

// The band's own corners in texture space, which is what the crop
// tightens towards: the taps are insets from these and not from the
// whole picture, so a letterboxed frame zooms about the middle of the
// strip rather than the middle of the screen.
uniform vec2 uBandLo;
uniform vec2 uBandHi;

// How much further in each tap crops than the one before.
uniform vec2 uInset;

// How many taps, and what each weighs. The weight is just under an
// eighth, so the eight together come to a little less than all of the
// light and the sum lands dark rather than saturating.
uniform int   uTaps;
uniform float uWeight;

void main()
{
    // Where this pixel sits inside the band, nought to one. Each tap
    // reads the same relative position inside its own tighter
    // rectangle, which is what makes it a zoom rather than an offset.
    vec2 at = (vUV - uBandLo) / (uBandHi - uBandLo);
    vec3 c = vec3(0.0);
    int  i;

    for (i = 0; i < uTaps; i++)
    {
        vec2 inset = uInset * float(i);
        vec2 lo = uBandLo + inset;
        vec2 hi = uBandHi - inset;

        c += texture(uSrc, mix(lo, hi, at)).rgb * uWeight;
    }

    oColor = vec4(clamp(c, 0.0, 1.0), texture(uSrc, vUV).a);
}
