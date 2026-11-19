#version 410 core

// The vertex stage every after-effect pass shares.
//
// An after-effect is one quad over the finished frame, so there is no
// transform to apply: the quad arrives in clip coordinates and is
// passed straight through. What the quad covers is the band the effect
// is allowed to touch, which is why the coordinates are uploaded per
// frame rather than being a fixed full-screen quad generated here.
layout(location = 0) in vec2 aPos;

// Where to read the frame, worked out from the position rather than
// uploaded beside it: the quad is screen-aligned and the frame target
// is the size of the screen, so the texture coordinate is the clip
// coordinate moved into the nought-to-one range. That holds for the
// vertical too, because the frame was drawn into a target whose first
// row is the bottom of the picture -- the same way round as the
// texture is sampled here.
out vec2 vUV;

void main()
{
    gl_Position = vec4(aPos, 0.0, 1.0);
    vUV = aPos * 0.5 + 0.5;
}
