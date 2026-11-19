#version 410 core

// The water's surface smoothing: one quad over the reflection, with
// the four tap coordinates worked out per corner.
//
// The taps are computed here rather than in the fragment stage, and
// that is not a performance choice -- it is what makes the pass
// faithful. The renderer this came from writes four texture coordinate
// sets into each of the quad's four vertices and lets the rasteriser
// interpolate between them, and the vertical offsets it writes are
// **nought at the top two vertices and one, two and three steps at the
// bottom two**. Interpolating that gives a blur which opens out
// towards the bottom of the rectangle, and computing the offsets per
// pixel instead would give a uniform one.
layout(location = 0) in vec2 aPos;
// Which corner this is: x nought at the left edge and one at the
// right, y nought at the top and one at the bottom. The tap
// arithmetic is written in terms of the corner rather than the
// position so that the quad can be anywhere.
layout(location = 1) in vec2 aCorner;

// The rectangle being read, in texture coordinates, in the quad's own
// terms rather than the texture's. The vertical pair is given as the
// quad's top edge and then its bottom edge, and which row of the
// source each of those is comes from the C side -- because in this
// interface a target's first row is the **bottom** of the picture, so
// the quad's bottom corresponds to the rectangle's first row and not
// its last.
//
// That is settled inside this interface and needs no comparison with
// the other one: the reflection is drawn into one of these targets and
// sampled from the same target, so both ends agree. It is not the
// question gl_water_flip exists for, which is about a matrix carried
// over from elsewhere.
uniform vec2 uSrcU;        // left, right
uniform vec2 uSrcV;        // the quad's top edge, then its bottom edge
// One tap step, already in texture coordinates.
uniform float uStep;
uniform float vStep;

out vec2 vTap0;
out vec2 vTap1;
out vec2 vTap2;
out vec2 vTap3;

void main()
{
    gl_Position = vec4(aPos, 0.0, 1.0);

    float u = mix(uSrcU.x, uSrcU.y, aCorner.x);
    float v = mix(uSrcV.x, uSrcV.y, aCorner.y);

    // The vertical offsets apply at the bottom edge and not at the
    // top, so they are scaled by how far down the corner is. The
    // horizontal ones apply everywhere.
    float ramp = aCorner.y;

    // Subtracted, because they step further towards the bottom of the
    // picture and a lower row is a smaller v here. The renderer this
    // came from adds them, for the same reason the other way round.
    vTap0 = vec2(u,         v - ramp * 0.0 * vStep);
    vTap1 = vec2(u - uStep, v - ramp * 1.0 * vStep);
    vTap2 = vec2(u + uStep, v - ramp * 2.0 * vStep);
    vTap3 = vec2(u,         v - ramp * 3.0 * vStep);
}
