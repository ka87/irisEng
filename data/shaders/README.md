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

A uniform renamed here and not in `win/wingl.c` is a failing test
rather than a black world: `tests/run_wingloverlay.sh` loads these
real files and checks the names the renderer asks for. What that test
cannot see is the arithmetic, because it has no GL to run it on.

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
