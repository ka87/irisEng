#version 410 core

// One widening pass of the depth blur's blur.
//
// Four taps -- two across, two down -- mixed by weights the renderer
// hands over. They are not equal, and that is not an oversight: the
// hardware this effect was written for accumulated the four taps by
// mixing each one half-and-half with the running result, so the last
// tap keeps about half of it and the first two about an eighth each.
// The weights are worked out from that mix rather than chosen, so that
// the two renderers blur the same way.
//
// The offsets arrive already pointed the way this renderer's textures
// run, so there is no flipping to do here.
in vec2 vUV;
out vec4 oColor;

uniform sampler2D uSrc;

// Four offsets and four weights, in step. Packed two to a vec4 rather
// than passed as arrays so that the whole pass is four uniforms.
uniform vec4 uOffsets01;   // tap 0 in xy, tap 1 in zw
uniform vec4 uOffsets23;
uniform vec4 uWeights;

void main()
{
    vec3 c;

    c  = texture(uSrc, vUV + uOffsets01.xy).rgb * uWeights.x;
    c += texture(uSrc, vUV + uOffsets01.zw).rgb * uWeights.y;
    c += texture(uSrc, vUV + uOffsets23.xy).rgb * uWeights.z;
    c += texture(uSrc, vUV + uOffsets23.zw).rgb * uWeights.w;

    // Alpha is not blurred and is not read from here either: the mix
    // at the end of the effect takes its alpha from the frame itself,
    // never from a blurred copy. One is written anyway so that the
    // target has a defined alpha rather than whatever the pass before
    // left in it.
    oColor = vec4(c, 1.0);
}
