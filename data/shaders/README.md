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
| `depthblur.vert` / `.frag` | the pass that writes the frame's blur amount |
| `aepass.vert` | one quad over the finished frame, shared by every after-effect |
| `aeblur.frag` | one widening pass of the depth blur's blur |
| `aedepthmix.frag` | sharp and blurred, mixed by the frame's own alpha |
| `aecolor.frag` | one step of the brightness, contrast or grayscale |
| `aeremanence.frag` | the frame with a wide blur of it added on top |
| `aeblend.frag` | the frame over the frame before, for the motion blur |
| `aezoom.frag` | the frame summed at eight tightening crops |
| `water.vert` / `.frag` | the water surface, with the reflection projected onto it |

## The water, and the one thing in it that is not derived

`water.vert` and `water.frag` draw the water's surface: a grid the
simulation builds, with the reflection target projected onto it through
the rippled direction from the camera to each grid point. The grid, the
direction and the projection matrix are not in the shader -- they are in
`watermesh.c`, shared with the other renderer and compared against it
byte for byte, so the shader does the drawing and none of the deciding.

Three things in it are worth knowing:

* **`aDir` is not a normal.** It is the vector from the camera to the
  grid point with its depth pushed about by the wave, and it is a
  vertex attribute only because the stream it came from declares that
  slot as a normal. It goes through the view's rotation and then the
  projection remap, and the pair to sample with is its first two
  components over its third. Its length is left alone on purpose.
* **The surface is not blended with the frame.** The water's own colour
  and the reflection are mixed in the shader by the vertex alpha and
  the result replaces the pixel. That is right: what shows through
  water is the reflection, not the world above it. The other renderer
  says the same thing the long way round, with a texture stage that
  blends by the diffuse alpha and a blend equation that then keeps only
  the source.
* **`uFlipV` is a measurement, not a derivation.** The two renderers'
  window spaces differ by a vertical flip and the remap carries a flip
  of its own, and working out what the two come to together is exactly
  the reasoning that was wrong about the geometry culling -- it came out
  backwards and two test harnesses agreed with it, because both had been
  written from the same reading. So the console tweak `gl_water_flip`
  moves it while somebody looks at a real scene, and the default is
  whichever way round that measurement settled.

## The reflection, which is in `geometry.vert`

The reflection pass has no shader of its own. It is the world's own
geometry drawn a second time with different matrices, so it goes through
`geometry.vert` and `geometry.frag` like everything else -- the only
addition is a clip plane, and three uniforms the reflection sets and
nothing else touches:

* **`uClipping`** is nought for every other draw, and then the clip
  distance the shader writes is a positive constant and clips nothing.
* **`uClipPlane`** is the plane, in world space, that keeps the
  reflection at or below the water line. Mirrored geometry that lands
  above the surface is the part of the world that could not be seen in
  water, and this is what removes it.
* **`uWorld`** is the mirrored object transform, and it is there only
  because the plane is in world space and the vertex has to get there.
  For the ordinary draw the renderer does not have the object's world
  transform on its own at all -- the engine hands over a view matrix
  with the object already folded in -- so this is the one draw that
  can supply it.

The distance is written on every draw, clipping or not: a clip distance
that is enabled and never written is undefined.

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
`tests/run_gloverlay.sh` mocks OpenGL outright: it loads these real
files and checks the uniform names the renderer asks for, so a uniform
renamed here and not in `platform/glrender.c` is a failing test rather than a
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

## The depth blur's after-effect

What the pass above writes, something has to read, and that is the
after-effect: `aeblur.frag` three times over the frame and then
`aedepthmix.frag` once.

Each blur pass takes **four taps** -- two across, two down -- and they
are **not weighted alike**. The other renderer accumulates its four
taps by mixing each one half-and-half with the running result, so the
last keeps about half of it, the one before a quarter, and the first
two about an eighth each. "About", because the factor it mixes with is
128 of 255 rather than an exact half, which is also the only thing that
separates the two taps across from each other. The weights are worked
out from that mix in `depthblur.c` and handed over as a uniform, so
both renderers blur the same way.

The asymmetry has a direction, and the direction is the one thing here
that a blurred picture cannot show. The heaviest tap is the one
**below** the pixel on screen. The other renderer's texture
coordinates run down the screen and this one's run up it, so the
vertical offsets are negated on the way to the uniform -- get that
wrong and the smear falls above every edge instead of below it, with
the picture just as blurred either way.

The three passes reach one, two and three pixels of a 640 by 480
picture, as a fraction of the texture -- so the reach grows with the
resolution rather than staying a fixed number of pixels. Each pass
reads what the one before wrote, so red crosses up to five pixels of a
hard edge.

`aedepthmix.frag` then mixes the sharp frame with the blurred copy by
the sharp frame's **own** alpha, which is the blur amount the pass
above wrote. Alpha one takes the sharp pixel whole. The frame's alpha
is carried through unchanged: nothing downstream reads it today, but
the effects still to come are mixed the same way and would be no-ops
without it.

Three strips are involved and two of them differ. The blur passes
cover the letterbox band **widened** by a margin either side; the mix
covers the band itself, which is also what the screen is copied from. A
tap at the band's own edge has to land on blurred pixels, which is what
the margin is for.

## The colour after-effects

`aecolor.frag` is brightness, contrast and grayscale, which are three
effects sharing one shader because all three are a gain and a bias:

* **brightness** adds a grey to every channel, or takes one off. Gain
  of one throughout -- scaling as well would change the contrast,
  which is the next effect's job;
* **contrast** is a gain about mid grey, which is `128/255` and not a
  half. The shade matters: pivoting on a half would move every pixel
  of a frame the effect is meant to leave alone;
* **grayscale** applies the same gain and bias to a weighted sum of
  the three channels instead of to each one. `uGrayscale` picks
  between the two, written as a number and a `mix` rather than a
  branch -- so the shader carries no step numbering of its own to keep
  in step with the engine's.

The grayscale's weights are green-heavy, blue-light, and they **do not
come to one**, so white does not come back white. That is the other
renderer's arithmetic, not an oversight: it arrives there through a dot
product that reads both sides as signed, and `aftereffects/aecolor.c` has the
working.

**One draw per step, not one pass doing all three.** The other
renderer runs each as its own draw into an eight-bit target, so a
channel one step pushes past white is white before the next step sees
it. One pass would need a clamp between the steps to come out the same
and would still differ by the rounding; a draw each matches outright
and costs a pass nobody will measure. It also means the step order is
the chain's rather than the shader's, and the order is part of the
answer -- brightening then stretching is not stretching then
brightening.

`aftereffects/aecolor.c` decides which steps apply and what each one's numbers are,
reproducing the byte truncation the other renderer's texture factors
do. Nothing about the numbers is decided here.

## The after-effects that sum whole frames

Three more, and they are a group because each one's output is a sum of
frames rather than a function of a pixel. `aftereffects/aeframe.c` has the schedules
and the weights.

**`aeremanence.frag`** is the remanence, which is a **bloom** whatever
its name suggests -- nothing in it reads the frame before. Four blurs
of the frame, reaching one, three, five and seven pixels, and then the
blurred copy added to the sharp frame at twice the effect's own
parameter. Twice because the renderer this was read from adds it in two
successive stages with the same weight and the same coordinates: one
addition said twice, not two samples. The weight can exceed one and the
sum saturates, which is what makes a bloom rather than a haze.

**`aeblend.frag`** is the motion blur, and it is the only after-effect
that reads across frames. There is no history buffer: the target is
never cleared, so the frame before is still in it. The mixing is the
framebuffer's own blend, because what is being mixed with is the
destination and a shader cannot read that -- so all the shader does is
hand the blend an alpha, which is the new frame's share.

That makes the *choice of target* the substance of the effect, and the
choice is not this effect's to make. `platform/glrender.c` carries a *current*
and a *spare* through the chain and exchanges them after every effect
that writes the spare, which is how the other renderer carries its
frame; the motion blur writes the spare, wherever the exchanges have
got it to.

So what it reads as "the frame before" depends on what ran before it.
Run on its own, the spare has not moved and holds its own output from
last frame: the trail works. Run behind the remanence, one exchange has
happened and the spare is the frame itself -- whose contents are the
world as it was drawn this frame -- so the trail silently stops being a
trail. That is what the other renderer does, and matching it is
deliberate rather than an oversight.

There are three spare targets rather than two for a different reason:
the depth blur's mix cannot read and write one texture here, so it
writes a third spare and is copied back over the frame, which is where
the other renderer leaves it in place. The two renderers then agree
about what every later target holds -- which is exactly what the motion
blur depends on.

**`aezoom.frag`** is the zoom smooth: the frame added to itself at
eight tightening crops about the centre of the band, so a pixel smears
along the line from the centre through it. Eight taps in one pass here;
the renderer this was read from takes four at a time over two passes,
and the sum is the same because every tap only adds.

Two things about it that a smeared picture cannot tell you. The crops
tighten **inwards**, which magnifies -- so a pixel reads positions
nearer the centre than itself and light appears further out than it
was. The smear trails **away** from the centre. And the crop is square
in texture space rather than in pixels, so on a 640 by 480 frame a
pixel smears thirteen and a third pixels sideways for every ten
vertically -- with a letterbox tilting it further still, because only
the horizontal step divides by the band's height.

Every tap weighs the same and the weight is just under an eighth, so
the eight together come to 248 of 255 and a flat frame comes back a
shade dark. The same flavour of deliberate undershoot as the
grayscale's weights.

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
works. The engine keeps normals in an array on the geometry rather than
handing them to the draw, so they come in a buffer of their own and
`aNormal` is enabled only for a draw that has them -- with the array
disabled the attribute reads the generic value, which the other modes
never look at. A material that asks for this projection where the
geometry has no normals is drawn as mode 0: visibly unprojected rather
than built on a normal invented from nothing.

The normal is transformed as a **direction**, with a fourth component
of zero, so the view matrix's translation does not reach it. The
texture transform then treats the result as a point, with a fourth
component of one, because the chrome transform maps a normal's -1..1
onto 0..1 with a half in its last row.

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
