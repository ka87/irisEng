#version 410 core

// The overlay's flat quads. Positions arrive in pixels, with the
// origin at the top left, because that is what the engine's 2D code
// has always worked in -- so there is no projection matrix here, only
// the viewport size and two lines of arithmetic.
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec4 aColor;

// x and y hold the drawing surface's width and height in pixels.
uniform vec4 uViewport;

out vec4 vColor;

void main()
{
    float x = aPos.x / uViewport.x * 2.0 - 1.0;
    float y = 1.0 - aPos.y / uViewport.y * 2.0;

    gl_Position = vec4(x, y, 0.0, 1.0);

    // The colour arrives as four normalised bytes in the order a
    // packed A8R8G8B8 word has them in memory on a little-endian
    // machine: blue, green, red, alpha. This puts them back.
    vColor = aColor.bgra;
}
