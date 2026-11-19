#version 410 core

// The fade: a frozen copy of the picture dissolving into the live one.
//
// The frozen copy was taken on the frame the fade started and has not
// moved since, so this is not a blur or a trail -- it is the world
// carrying on underneath a still photograph of itself. The photograph's
// share falls from all of it to none of it over the fade's length, and
// on the first frame the two are the same picture, so a fade begins
// invisibly.
in vec2 vUV;
out vec4 oColor;

// The live frame, and the copy frozen when the fade began.
uniform sampler2D uLive;
uniform sampler2D uFrozen;

// The frozen copy's share, one at the start of the fade and nought at
// the end. The renderer hands this over as the opacity byte over the
// whole range, which is the same number the other renderer puts in the
// alpha of a constant colour and lets the hardware divide.
uniform float uFrozenShare;

void main()
{
    vec4 live = texture(uLive, vUV);

    // Colour only, and the live frame's alpha carried rather than
    // mixed.
    //
    // The alpha here is not observable: the other renderer leaves its
    // alpha stage disabled through this effect, so what it writes is
    // whatever the vertices carry, and its quad carries no colour at
    // all. Nothing downstream reads it either -- the one effect whose
    // input is the frame's alpha runs before this one, and the next
    // effect along overwrites the alpha to drive a blend. So this
    // passes the live frame's through, which is the choice that leaves
    // the frame meaning what it meant.
    oColor = vec4(mix(live.rgb, texture(uFrozen, vUV).rgb, uFrozenShare),
                  live.a);
}
