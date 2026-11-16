# The renderer's shaders

GLSL, read at run time from beside the executable rather than compiled
in. A shader is the part of a renderer most likely to need changing on
somebody else's machine, and a file that can be edited with the game
restarted is worth more than one that needs a rebuild.

They are **4.50 core**, which is what the OpenGL renderer asks the
driver for. They are not used by the Direct3D renderer at all.

| | |
|---|---|
| `overlay.vert` / `.frag` | the debug overlay's flat quads |
| `overlaytext.vert` / `.frag` | glyphs, sampling the font atlas |
| `geometry.vert` / `.frag` | the world, textured and modulated |
| `video.frag` | a movie frame, on the glyphs' vertex stage |

## What the world's fragment shader is doing that the pipeline cannot

Core OpenGL dropped the fixed-function texture stage and the alpha
test, and the engine uses both. So `geometry.frag` does them:

* the texture is **modulated** by the vertex colour -- all four
  channels, which is what the other renderer's stage computes -- and
  `uDiffuse2x` doubles the colour but not the alpha, as that renderer's
  `MODULATE2X` does;
* `uTextured` says whether there is a texture at all. A material can
  ask for none, and then the vertex colour is the colour. The engine
  says so by handing the renderer an index of `-1`, not by binding
  nothing;
* `uAlphaMode` and `uAlphaRef` are the alpha test, done with `discard`
  so that the depth write is suppressed too. The two modes are the two
  senses the material's flags select between.

Two tests stand behind these files, and they see different things.
`tests/run_wingloverlay.sh` mocks OpenGL outright: it loads these real
files and checks the uniform names the renderer asks for, so a uniform
renamed here and not in `win/wingl.c` is a failing test rather than a
black world. What it cannot see is the arithmetic.
`tests/run_glrender.sh` can: it brings up a real 4.5 core context with
no window, draws through the renderer's own entry points and reads the
pixels back. Every line of every shader here is covered by it. That
test is allowed to be unavailable, and says so and passes when it is,
so the first one is still what runs everywhere.

## One vertex stage, two fragment stages

`video.frag` has no vertex shader of its own: it is paired with
`overlaytext.vert`, which already maps pixel coordinates to clip space
and carries a texture coordinate. That is the whole difference between
a glyph and a movie frame -- a glyph's texture is coverage and decides
how much of the vertex colour there is, and a frame's texture is the
picture. Sharing the stage means the pixel mapping is written and
tested once.

## The depth blur

`depthblur.vert` and `depthblur.frag` are the pass that writes the
frame's alpha. The world is drawn a second time through a 256 by 1 ramp
of blur-amount-per-depth, with the ramp's coordinate taken from the
camera-space position -- so the coordinate is the depth -- and the
colour write narrowed to alpha. What it leaves behind is a per-pixel
blur amount the after-effects read, and the sun's occlusion test with
them.

Alpha runs the other way from what the name suggests: 255 means no blur
and 0 means full. The ramp is solid 255 when no blur is in force, which
is what says so.

The coordinate has two forms and the blur mode decides which. The
projected one is `uTranslateU / (z * uScaleW)`, a reciprocal: it reaches
the end of the ramp at a depth of `uTranslateU` and falls towards the
start as the depth grows, so blur rises with distance. The other is
`z * uScaleUfromZ + uTranslateU`, linear, which the sun's mode uses with
the scale at one over the far plane -- so the coordinate is the depth as
a fraction of the frustum.

The camera-space position is passed across rather than the finished
coordinate, because one form divides and a divided value interpolates
to something else. The transform is affine in that position and the
interpolation is perspective-correct, so transforming in the vertex
stage and interpolating is the same as interpolating and transforming
in the fragment stage -- which is the order used, so that the divide
happens per pixel, as it does in the other renderer.

`depthblur.c` decides which blur is in force and builds the table. The
renderer turns it into the texture and sets these uniforms; nothing
about the numbers is decided here.

## The material's texture-coordinate projection

A material's texture coordinate is not always the one stored in the
geometry. The engine computes a transform and says which source it
applies to, and `geometry.frag` reads both: mode 0 is the stored
coordinate untouched, mode 1 the camera-space position transformed,
mode 3 the stored coordinate transformed. Modes 4 and 5 belong to the
depth-blur pass, which has its own program.

The two transformed modes disagree about the shape of the input, and
that is the engine's doing rather than a guess. A generated coordinate
is a point, so its fourth component is one and the transform's last row
is its translation. A stored coordinate is two numbers, so the engine
moves the translation up into the third row before handing the matrix
over -- which is only necessary if the fourth component is zero there.
One is `(x, y, z, 1)` and the other `(u, v, 1, 0)`.

Mode 2 takes the camera-space normal, which is how chrome mapping
works. It is not here: the engine keeps normals in a vertex stream of
their own, this renderer builds one interleaved vertex without them,
and inventing a normal would be drawing something arbitrary. The
renderer says so once and draws the stored coordinates.

## Fog

`geometry.vert` passes the camera-space depth across and
`geometry.frag` fogs by it. Three curves -- linear, exponential, and
exponential squared -- with the range, the density and the colour
coming from the engine through `fog.c`, and the curve number chosen per
material.

The depth is **not** negated. The engine's camera space has z growing
away from the camera, and the authority on that is the depth-blur pass:
its coordinate is the depth over the far plane and it multiplies z by
one over the far plane, with no sign change. The two read the same
depth, so they agree or one of them is wrong. A first draft negated it
and fogged nothing at all, because every factor clamped to one.

Fog is applied after the alpha test and to the colour only. The
hardware's own fog leaves alpha alone, and here that matters twice
over: the depth-blur pass reads the frame's alpha, and the sun's
occlusion test reads what the blur wrote.

The factor runs the same way as the hardware's: 1 is no fog, so the
colour is `mix(fogColor, pixel, factor)`.

## Two programs rather than one with a branch

Their vertex layouts differ -- the textured one carries a UV -- and a
vertex array object describes one layout. A branch in the fragment
shader would still need two of those, so it would save nothing and
read worse.

## No projection matrix

Positions arrive in pixels with the origin at the top left, which is
what the engine's 2D code has always worked in. Mapping that to clip
space is two lines and the viewport size, so the viewport is the only
thing passed in.

## The colour swizzle

`vColor = aColor.bgra` is not a mistake. The engine's colours are
packed A8R8G8B8 words, and on a little-endian machine the bytes of one
are blue, green, red, alpha in that order. Read as four normalised
bytes that is what the attribute holds, and the swizzle puts them
back.

## If one will not compile

The renderer prints the driver's own log, which is the only thing that
names a line, and then draws nothing rather than drawing wrongly. The
game carries on.
