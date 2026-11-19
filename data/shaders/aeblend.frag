#version 410 core

// The motion blur: this frame laid over whatever the target already
// holds, which is what this effect wrote into it last frame.
//
// There is no history buffer. The target is simply never cleared, so
// the frame before is still in it -- and the mixing is the
// framebuffer's own blend rather than anything here, because what is
// being mixed with is the destination and a shader cannot read that.
//
// So all this does is hand the blend an alpha. The renderer sets
// GL_SRC_ALPHA / GL_ONE_MINUS_SRC_ALPHA, so the alpha below is the new
// frame's share and one less it is the old frame's.
in vec2 vUV;
out vec4 oColor;

uniform sampler2D uSrc;

// The new frame's share, which is one less the share kept of the frame
// before.
uniform float uAlpha;

void main()
{
    // The frame's own alpha is replaced rather than carried: it is
    // what drives the blend, and it is the only way to reach the
    // blend from here. The other renderer does the same, and nothing
    // downstream of this effect reads the frame's alpha.
    oColor = vec4(texture(uSrc, vUV).rgb, uAlpha);
}
