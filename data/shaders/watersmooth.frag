#version 450 core

in vec2 vTap0;
in vec2 vTap1;
in vec2 vTap2;
in vec2 vTap3;

out vec4 oColor;

uniform sampler2D uSrc;
// The four weights, which are watersmooth.c's -- and that file asks
// the depth blur for them, because both effects are the same run of
// hardware stages mixing each sample into what came before by one
// fixed factor. They are not a quarter each: the chain leaves the
// first two taps on the same power of one-less-the-factor and the last
// two heavier.
uniform vec4 uWeights;

void main()
{
    // Colour only. The renderer this came from takes the alpha from
    // the first tap and carries it through the chain unmixed -- the
    // later stages select the running alpha rather than blending it --
    // so the alpha here is tap nought's and not a weighted sum.
    vec3 c = texture(uSrc, vTap0).rgb * uWeights.x
           + texture(uSrc, vTap1).rgb * uWeights.y
           + texture(uSrc, vTap2).rgb * uWeights.z
           + texture(uSrc, vTap3).rgb * uWeights.w;

    oColor = vec4(c, texture(uSrc, vTap0).a);
}
