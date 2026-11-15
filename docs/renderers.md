# Choosing a renderer

The engine draws through the function-pointer interface in
`tGdi_DisplayData` — `Clear`, `Flip`, `SetViewMatrix`,
`DrawElementIndexedTriangles`, the texture calls, and the rest of the
22 entries. A renderer is a row in a table plus the function that
writes that interface, and **nothing else in the engine learns which
one it got**.

```ini
[render]
backend = d3d
```

## The table is the preference

`gdi.c` holds the table and `renderers.c` holds the picking:

```c
static const tGdi_Renderer gdi_renderers[] = {
    { "d3d", "Direct3D 9", GDI_l_FillD3D, GDI_l_b_D3DAvailable },
};
```

`GDI_pst_PickRenderer` takes that table and the name the ini asked
for, and returns the row to use: the one asked for if it can run,
otherwise the first row in the table that can, otherwise nothing. The
table's order *is* the preference order, so putting a renderer first
is how it becomes the default.

Adding a renderer is a row and a fill function. That is the whole
contract, and it is the reason the stages after this one have
somewhere to land.

## A renderer that cannot run is not an error

Each row answers for itself whether it can run on the machine in
front of it. The ini setting is a **preference**, so asking for a
renderer that cannot run leaves the game drawing through whatever
can, and says which in the debug output:

```
GDI: renderer "gl" cannot run here
GDI: renderer is Direct3D 9
```

An unknown name is reported differently, because it is a different
mistake — a typo, not a machine that is short of something:

```
GDI: no renderer called "vulkan"
```

An **empty** name is neither. `backend =` left blank, or no
`[render]` section at all, asks for nothing in particular and is
reported as nothing at all. The result is the same as an unknown name
— the first renderer that can run — which is why the test for it is a
check on the log rather than on the result.

## Names

A row is found by its name or by any alias of it, ignoring case:

| row | also answers to |
|---|---|
| `d3d` | `d3d9`, `direct3d`, `directx` |
| `gl` | `opengl`, `ogl` |

An alias reaches **only its own row**. That sounds too obvious to
need saying until you write the matching loop the other way, where
every alias matches every row and the table's first row always wins —
so there is a case for it, and it is arranged so that the row it
would wrongly return is not the one that is right.

## The tests

`tests/run_renderers.sh`, 36 checks. The picker is driven through
tables of its own rather than the engine's, so a case can say "this
renderer cannot run" without there being a machine that cannot run
it, and the real table in `gdi.c` stays free to be just a list.

The fake table deliberately puts `gl` **first**, so that a case
asking for `d3d` and getting it proves the name was read — with the
rows the other way round, half of these would pass on a picker that
ignored the name entirely.

Eight defects were injected and seven were caught: an alias matching
any row, the name compare made case-sensitive, availability not
checked for the row that was asked for, the fallback dropped, the
asked-for name not cleared before the unknown-name message, the
fallback picking a row that cannot run, and an empty name treated as
a name.

**The eighth is not observable.** Removing the `num <= 0` guard
changes nothing: with a negative count the loops below it do not
execute, so the function returns the same `NULL` by a different
route. The guard stays as a statement of intent, and this is recorded
rather than tested, since no case can tell the two apart.

The picker returns a row and does not draw through it — the caller
does that. That is what keeps it testable without a display, and
there is a check that it stays true.

## Drawing state the engine asks for by meaning

A renderer cannot be swapped while the engine reaches past the
interface into the one it happens to have. Five of those reaches were
render-state pokes, written out at the call site in
`graphicsobjects/shadow.c` and `materialanddraw.c` — each one a check
against Direct3D's own state cache, a write to it, and a device call
with a `D3DRS_` or `D3DTSS_` name.

They are interface entries now:

```c
void(* SetZEnable)(int on);
int (* i_ZEnable)(void);
void(* SetZWriteEnable)(int on);
void(* SetTexCoordIndex)(int stage, int index);
void(* SetTextureTransformFlags)(int stage, int flags);
```

The engine asks for what it wants and stops knowing that a cache
exists at all. Keeping one is the renderer's own business: the D3D
implementations are the cached setters the rest of `wingdi.c` already
used, so a value already in force still costs nothing and the device
sees exactly the calls it saw before.

`i_ZEnable` is there because `SDW_RenderShadow` saves the depth state,
draws, and puts it back. That read has to go somewhere, and a getter
is honest about it — the alternative, a save/restore pair inside the
renderer, would have been a second thing to get right.

### How this was shown not to change anything

`materialanddraw.c` has a frozen differential harness, and it is the
right instrument for exactly this: the frozen copy pokes the mocked
device directly, the rewrite goes through the interface, and both are
driven over identical state with the device's calls recorded in
order. 1280 cases, 0 differing — including the state caches, which
the harness randomises and compares.

The mocks for the two new entries are **written out in the harness
rather than calling the renderer's own**, which is not linked there.
That is what makes the comparison a check on the call sites. Four
defects injected into them were all caught: either call dropped, the
wrong index passed, and the two calls swapped round. Dropping the
cache check inside the mock is caught too, which is what makes those
four mean anything — without it the recorded sequence would not
depend on the cache.

**`graphicsobjects/shadow.c` has no harness**, so its two sites rest
on the equivalence being exact rather than measured:
`if (cache != v) { cache = v; device(v); }` written out is precisely
what `D3D_RenderStateSet` does, and the restore site's
`if (cache != backup)` is the same call with `backup` for `v`. Worth
knowing which of the two files is measured and which is read.

## The fullscreen video display

The engine's half of the video knows about files, decoding and the
clock. It did not stop there: it also held three Direct3D objects,
locked one of them, computed a letterbox rectangle and called
`StretchRect`. All of that is `win/wingdivideo.c` now, behind five
entries:

```c
int (* b_VideoDisplayCreate)(int width, int height);
void(* VideoDisplayClose)(void);
int (* b_VideoFrameLock)(void** bits, int* pitch, int* height);
void(* VideoFrameUnlock)(void);
void(* VideoFramePresent)(int width, int height, int fit);
```

`ai/ai_gfxfunctions.c` has **no Direct3D references left at all** —
it opens the file, locks the picture, decodes into it, and asks for
it to be shown.

`fit` is the engine's vocabulary, not a renderer's:
`GDI_VIDEOFIT_ACTUALSIZE`, `..._SHARP`, `..._SMOOTH`. The engine maps
the scale mode's bits onto those and the renderer maps them onto its
own filters, so `D3DTEXF_LINEAR` stays inside `win/`.

### The split found a bug

`VID_CopyToBuffer` clamps to the height it is handed. The picture is
the **movie's** size, and what used to be handed over was the
**screen's** height — so a window shorter than the movie left the
bottom of every frame unwritten, showing the black the picture was
created with. It only ever looked right because a window is usually
taller than the movie.

`b_VideoFrameLock` reports the picture's own height, so there is no
longer a size to pass in wrongly. The test makes the two deliberately
differ, and reporting the screen's height again is one of the injected
defects.

Two smaller things came out of the same move. The original checked
only one of its five device calls for failure and would have gone on
to dereference a surface it never got; each is checked now, and each
part-way failure gives back what it had already made. And the initial
clear used `4 * width * height` where the surface's pitch can exceed
`width * 4`, which could leave a strip uncleared; it uses the pitch
the lock reports.

### The tests

`tests/run_wingdivideo.sh`, 61 checks. The device is mocked and
records what it was asked for **in order**, so the checks are on the
sequence as much as the result — which is what the failure paths need,
since what they have to get right is giving back what they had
already made. Creations are counted against releases rather than the
code being eyeballed for tidiness.

Twelve defects injected, all twelve caught: the screen's height
reported instead of the picture's, the picture leaked when the
texture fails, the texture and picture leaked when the surface level
fails, a second display leaking the first, the back buffer leaked on
create and on present, the clear dropped, smooth and sharp given the
same filter, actual size scaled anyway, the picture not centred, and
locking or unlocking with no display.

**Two mistakes in the test itself are worth recording.** The mock
first reported a pitch its own buffer could not back, so the code
overran it — the mock was lying, not the code, and the pitch now
follows the size the picture was asked for. And four expectations
were written with numbers copied out of a run; `tests/stub/d3d9.h`
gives the D3D constants deliberately arbitrary values, so those
checks would have passed whatever constant the code used. They are
built from the SDK's own names now.

## The sun's occlusion test

`gfx/gfxsun.c` was not engine code with a few leaks. About twenty of
its functions were already named `D3D_*`, and `gfxsun.h` put
`IDirect3DSurface9` inside the engine's own sun struct. The Direct3D
half is `win/wingdisun.c` now, behind seven entries, and the engine's
struct holds an opaque handle:

```c
void*(* p_SunSamplerCreate)(void);
void (* SunSamplerDestroy)(void* sampler);
int  (* b_SunSamplerReady)(void* sampler);
int  (* b_SunSamplingNow)(void* sampler);
void (* SunSampleQueue)(void* sampler, const int* x, const int* y, int num,
                        int pointMask, float cameraZ);
int  (* b_SunSampleRead)(void* sampler, int* outPassed);
void (* SunEffectOnOff)(void* sampler, int on);
```

### What is actually renderer-specific here

The question the engine wants answered is whether the sun can be seen
at nine positions. In Direct3D 9 the answer is an **asynchronous read
of the frame just drawn**: the alpha at each position is stretched
into a 16x1 render target, copied into a lockable surface, and picked
up a frame or two later when the hardware has finished with it. Three
of those surfaces sit in a ring so a sample can be queued every frame
without ever waiting on one.

That latency is the part worth hiding. The engine projects the sun,
clips the ring of positions to the viewport, queues a sample and takes
an answer **whenever one turns up** — which is a frame or more later,
and sometimes not at all. None of that needs a surface in the engine's
vocabulary, and a renderer with real occlusion queries would answer
the same question a completely different way.

### Why there are two readiness questions

`b_SunSamplerReady` and `b_SunSamplingNow` look redundant and are not.
The first is whether the sampler has its target yet; the second is
whether the renderer is in a mode where anything writes the alpha this
test reads. The engine does **different things** about them — the
first returns before the sun's effect is touched at all, the second
after it has been turned on — so collapsing them would change
behaviour.

### The engine's half, and why its bookkeeping is the risky part

`alphaThresholdFlags` carries this frame's nine answers in its low
bits and the previous frame's in its high half, so a frame with no
fresh answer can show the last one. Getting a shift or a mask wrong
there makes the sun flicker rather than crash, which is exactly the
kind of thing worth pinning down with cases.

### Two defects found on the way

A second `GFX_Sun_Destroy` used to release the same surfaces twice —
it dereferenced the embedded struct unconditionally. The destroy is
idempotent now, and a case checks it.

And `GFX_Sun_Destroy` originally took the address of the embedded
Direct3D struct, which is what made the frozen `tests/baseline_gfx.c`
stop compiling. See below.

### The tests, and what is measured against what

Two new harnesses, both direct checks:

- **`tests/run_wingdisun.sh`, 61 checks.** The device can be told,
  per surface, whether the hardware has finished with it, so the ring
  is driven through orders the real thing would produce. The cases
  check that a sample is never waited for, that the newest finished
  one is read, that the ring stops taking samples when nothing has
  been read and starts again when one has, and that a position
  outside the viewport counts as not occluded. Twelve injected
  defects, all caught.

  **Two of those twelve needed better cases first.** Every slot
  carried the same alpha, so reading the packed pixels by the wrong
  index was invisible — there is now a case giving each position its
  own alpha. And nothing read the ring down to empty, so the one
  branch that resets it was never taken; a case now fills the ring,
  reads its newest, and checks that samples are accepted again.

- **`tests/run_gfxsun.sh`, 41 checks.** The engine's half, with the
  projection mocked so the clipping can be driven, and the real
  `math.c` linked. Twelve injected defects, all caught, including
  every shift and mask in the flag bookkeeping.

**`tests/baseline_gfxsun.c` is a frozen copy of `gfxsun.c` that
nothing has ever driven** — it was added alongside the grid-effects
tests and never wired to a harness. It was frozen before this split,
so it cannot be compared across it; the new direct checks stand in
its place. Worth knowing it is there and that it is not a witness to
anything.

### The one edit to a frozen copy

`tests/baseline_gfx.c` called
`GFX_Sun_Destroy(&(...)->d3dStuff)`. With the renderer owning that
state there is no such member, and `-D` cannot map a struct member the
way it maps a renamed symbol. That one line was changed, with the
reason written beside it in the file.

The cost is stated rather than hidden: **that one destroy call is no
longer compared against the rewrite.** Everything else in the same
function still is, and `gfx.c`'s harness runs 2482 cases with 0
differing.

## What the renderer knows, and what is simply shared

With the operations behind the interface, what was left was **data**:
engine files reading and writing fields of `D3DRenderParams`. Sorting
it out needed the fields separated into two kinds, and the first
guess about them was wrong.

### Capabilities, which the renderer answers

`tweak_ae_d2x`, `tweak_ae_noflare` and `tweak_zonalpha` look like
debug tweaks that ended up in the wrong struct. They are not.
`win/wingdi.c` computes all three from the device's own capabilities
when it starts:

```c
D3DRenderParams.tweak_ae_d2x    = d3dcaps.TextureOpCaps & D3DTEXOPCAPS_MODULATE2X;
D3DRenderParams.tweak_ae_noflare = d3dcaps.MaxSimultaneousTextures < 4u;
D3DRenderParams.tweak_zonalpha  = 0;
```

They are **renderer capability flags**, exposed in the tweak console
so they can be forced. Moving them to `dbg.c` beside the other
fourteen tweaks would have separated them from the thing that works
them out, and a GL renderer would compute its own values anyway.

So they stay where they are computed, and the engine asks:

```c
int (* b_CanDiffuse2x)(void);
int (* b_WantsNoFlare)(void);
int (* b_NoZonAlpha)(void);
int (* b_DepthBlurOn)(void);
void(* GetDrawingSize)(int* width, int* height);
```

`b_DepthBlurOn` joins them because `isDepthBlurOn` is the same shape:
written only by the renderer, read by the engine. `GetDrawingSize`
too — `gdi.c` was working the window size out of the renderer's own
rect, and a device does not always get the size it asked for, so only
the renderer knows.

Each is read through the interface every time rather than cached,
because the tweak console can change the answer at any point.

### State both sides write, which belongs to neither

`diffuse2x` and `textureUVMode` are different: the engine writes them
as well as reading them, and so does the renderer. They are
`GDI_gi_Diffuse2x` and `GDI_gi_TextureUVMode` now, in `gdi.h`, named
directly by both sides.

`sunEffectIsOn` turned out to need nothing: after the sun split,
nothing outside `win/` touches it.

### The upshot

`materialanddraw.c`, `graphicsobjects/shadow.c`, `gfx/gfxsun.c`,
`ai/ai_gfxfunctions.c` and `gdi.c` now contain **no reference to
`D3DRenderParams` at all**. What is left is three lines of `dbg.c`,
and those are deliberate: the tweak console's table points at where
the renderer keeps its capability flags, which is exactly its job.

### How the move was kept honest

Two pieces of state changing where they live cannot be expressed with
`-D` the way a renamed function can, so the frozen copies still read
the struct while the rewrite reads the globals. The harnesses were
taught to **drive both spellings**, and three separate places needed
it — each found by the comparison failing, not by reading the code:

1. **Installing the state.** `Setup` sets the struct member and the
   global to the same value.
2. **Clearing it.** `ResetShared` zeroes every shared global before
   each side runs, and the two new ones had to join the list.
   Without that the rewrite began each case holding what the
   previous one left, while the frozen copy began from the zeroed
   struct — 481 differences that looked like a defect in the
   rewrite and were a gap in the harness.
3. **Third parties that perturb it.** `wingdi_mocks.c` stands in for
   the UV compute functions and deliberately pokes the UV mode. It
   writes both spellings now; writing one left the two sides looking
   at different worlds.

And where the state is **compared**, each side is read where it
actually keeps it: `test_matdraw.c`'s snapshot takes a side
parameter, and `test_wingdi.c`'s overwrites the two members of its
copied struct for the rewrite's side. Reading one spelling for both
would have compared the move instead of the code.

All of which is the argument for doing this against frozen copies
rather than by inspection: every one of those three gaps would have
shipped silently.

## Nothing neutral is named after Direct3D any more

`gdi.h` is the engine's drawing header, and it used to `#include
<d3d9.h>` and define the whole Direct3D renderer's private state —
`IDirect3D9*`, `D3DCAPS9`, `LPDIRECT3DDEVICE9` and ninety-odd other
fields — in the middle of the interface every renderer has to fill.

That was a question of **where things live** more than what they are
called. `tGdi_DisplayData` itself turned out to contain no Direct3D
type at all beyond three references to that struct, so once those
three were dealt with the include could go.

- The state struct moved to `win/wingdi.h`, where it belongs. It
  keeps its Direct3D name, because it *is* Direct3D's.
- `CreateDevice` and `DestroyDevice` deal in `void*`: the engine
  holds a renderer's private state opaquely and never looks inside.
  The one place that does look is inside the renderer, which casts
  to its own type.
- `pd3drp` became `rendererState`.
- `D3D_ReleaseTextures` and
  `D3D_SetRenderTargetAndBackupPreviousSDW` lost their prefixes —
  they are interface entries, and an interface entry named after one
  renderer is a contradiction. The *functions* that implement them
  keep their `D3D_` names, which are correct.
- `tD3dRequestSnapshot` became `tGdi_SnapshotRequest`. It holds a
  flags word and some sizes and never held anything of Direct3D's;
  the name was the only thing renderer-specific about it.
- `D3D_OutputDebugString` became `GDI_OutputDebugString`. It is
  `dbg.c`'s own logger, byte for byte the one above it, with a
  leftover name.

`gdi.h` now contains no Direct3D include, no Direct3D type and no
Direct3D-named member. Only two comments mention it, to explain the
arrangement.

### The tweak console

Three of the seventeen tweaks were pointers into the renderer's
state. They go through the interface now, as the tweak table's **type
2** — the setter kind the console already knew how to call and
nothing had ever used:

```c
{"ae_d2x", 2, (void*)DBG_l_TweakCanDiffuse2x},
```

So `dbg.h` includes `gdi.h` rather than `win/wingdi.h`, and nothing
in `dbg` depends on Direct3D.

### How the frozen copies were kept

Type and function renames are what `-D` is for, and three went that
way: `D3D_OutputDebugString`, `tD3dRequestSnapshot` and
`sD3dRequestSnapshot`.

**A member rename can go that way too**, which was not obvious:
`-Dpd3drp=rendererState` rewrites the token after the `->` just as
well as a symbol, and it carried two of the three sites.

The third dereferenced it, and the member's type is opaque now, so a
rename alone was not enough there. That one line of
`tests/baseline_wingdi.c` is edited, with the reason beside it. The
comparison is **not** weakened by it: both sides read the same object
through the same renderer's type, which is a different situation from
the edit the sun split needed, where the two sides genuinely stopped
doing the same thing.

## A geometry's buffers belong to the renderer

`graphicsobjects/geo.h` is included by most of the engine, and it
opened with `#include <d3d9.h>` for the sake of five members at the
bottom of `tGEOGRO`: two vertex buffers, an array of UV buffers and
an index buffer. Those are whatever the renderer that made them says
they are, and nothing in the engine does anything with them except
hand them back to be freed.

They are `void*` now — one opaque handle per buffer — so the header
names no renderer's types and pulls in no renderer's API. They lost
their Direct3D names with it:

| was | is |
| --- | --- |
| `d3dvb` | `vertexBuffer` |
| `d3dvb2` | `vertexBuffer2` |
| `d3dUVVBNum` | `uvBufferNum` |
| `d3dUVVB` | `uvBuffers` |
| `d3dIndexBuf` | `indexBuffer` |

The renderer's view of its own buffers lives in `win/wingdi.h`, where
it belongs:

```c
#define D3D_GEO_VB(g)     ((IDirect3DVertexBuffer9*)(g)->vertexBuffer)
#define D3D_GEO_VB2(g)    ((IDirect3DVertexBuffer9*)(g)->vertexBuffer2)
#define D3D_GEO_UVVB(g)   ((IDirect3DVertexBuffer9**)(g)->uvBuffers)
#define D3D_GEO_IB(g)     ((IDirect3DIndexBuffer9*)(g)->indexBuffer)
```

Thirteen sites in `win/wingdi.c` dereference a buffer and go through
those. Three more hand `&geogro->vertexBuffer` and its siblings to
`CreateVertexBuffer` as out-parameters, which the renderer casts at
the call, because the address of a cast is not an lvalue. Everywhere
else a handle is only tested for null, freed, or passed to a Direct3D
entry point that declares its own parameter type — and C converts
`void*` to an object pointer on its own, so those needed nothing.

After the change, no translation unit that includes `geo.h` sees
`d3d9.h` any more — `gcc -H gameobject.c | grep -c d3d9.h` is `0` —
and `geo.h` itself mentions Direct3D nowhere at all.

### Two entries, because two callers reached past the interface

`strings.c` and `graphicsobjects/geo.c` both called
`PC_FreeABackCache` directly, which is Direct3D code, and one of them
first read `gro->d3dIndexBuf` to decide whether to bother. Both now
ask the interface:

```c
if (gpst_DX_stDD->b_GeometryHasCache(gro))
    gpst_DX_stDD->DiscardGeometryCache(gro);
```

`b_GeometryHasCache` is a question because the caller in `strings.c`
asked it, not because the engine has any use for the answer beyond
this.

**The guard is narrower than what it guards.** It tests the index
buffer alone, exactly as the old code did, while
`PC_FreeABackCache` checks each of the five buffers for itself and
frees whichever are there. A geometry holding vertex or UV buffers
but no index buffer would therefore keep them. That is the existing
behaviour and it is preserved rather than quietly fixed; the note
sits beside the implementation in `win/wingdi.c` as well.

**The call in `GEO_FreeContent` gained a null check.** It used to go
straight to the renderer's function, so it ran whether or not a
display existed; through the interface it needs `gpst_DX_stDD`. Only
a renderer can have created a cache, so there is nothing to drop in
that window, and `geo.c` guards that pointer the same way in three
other places. `strings.c` needed no such guard: the function it sits
in already dereferences `gpst_DX_stDD` a few lines above.

### The tests, and the harness that was already there

`run_wingdi.sh` carries the five member renames as `-D` and reports
no differences across its 5,154 cases and 150,984 compared engine
calls.

**One line of `tests/baseline_wingdi.c` is edited**, and it is the
only one the opaque handles cost. `-D` renames a member but cannot
introduce a cast, and the implicit `void*` conversion covers every
site in that copy but one: `geogro->d3dUVVB[0]->lpVtbl->Lock(...)`
subscripts the array and dereferences the result. That line now uses
`D3D_GEO_UVVB`, with the reason written beside it. The comparison is
not weakened — both copies read the same object through the same
renderer's type — which is the same argument as the `pd3drp` edit
above, and a different situation from the sun split, where the two
sides genuinely stopped doing the same thing.

The interesting one is `strings.c`, and the first thing to say about
it is that it was **not** an orphan: `tests/run.sh` builds and drives
that harness inline, at `run_strings()`, rather than through a
`tests/run_strings.sh` of its own — as it does for `animation.c` too.
A standalone script written for it was a duplicate of a live harness,
which is the drift this repository has been bitten by before, so it
was deleted and `run.sh`'s own copy got the `-D` mapping it needed.
The full suite failing on `'tGEOGRO' has no member named
'd3dIndexBuf'` is what said so.

Driving the new path needed the harness changed, because the old
guard read a field that every case left null:

- `tests/strings_mocks.c` records both entries, under the old name,
  so a case's expectations read the same as before.
- `tests/test_strings.c` sets the cache state per case and runs every
  case in both, which took it from 27,407 cases to **54,814**.

Without that, dropping the `DiscardGeometryCache` call entirely
scored clean — the call was never reached. With it, three mutations
are caught:

| mutation | caught by |
| --- | --- |
| the guard dropped | cases with no cache now free one |
| the call dropped | cases with a cache now keep it |
| the guard inverted | both halves disagree |

## Text is FreeType's, and the renderer only draws quads

The engine drew debug text with **D3DX**: two `ID3DXFont` objects, one
Arial at ten pixels for the raster readouts and one Courier New sized
to the viewport for the command overlay. `DrawTextA` did the layout,
the measuring and the drawing, which meant a renderer with no D3DX had
no text at all.

Text now belongs to `font.c`, which is renderer-neutral, and FreeType
rasterizes it. The vendored library is in `thirdparty/freetype` and
that folder's README says exactly what is there and how the file set
was derived.

### What moved where

| | |
|---|---|
| `font.c` | the faces, the glyph atlas, measuring, and where each glyph goes |
| the renderer | an alpha texture, and textured quads inside the overlay pass |

Three interface entries, and nothing more:

```c
void*(* p_CreateAlphaTexture)(int width, int height, const uint8_t* coverage);
void(* DestroyAlphaTexture)(void* texture);
void(* OverlayDrawTexturedQuads)(void* texture, const void* vertices,
                                 int triangleCount);
```

The coverage is 8-bit, one byte per pixel with no padding, and the
renderer picks its own format: the Direct3D one makes an A8R8G8B8
texture, white with the coverage in alpha, because `D3DFMT_A8` is half
the memory and a format a card may decline — not something to discover
on somebody else's machine. A `MODULATE` stage then takes the colour
from the vertex and the shape from the atlas, and the two alphas
multiply so a translucent request stays translucent.

The quads are `tGdi_TexturedVertex` triangles in the overlay pass's own
pixel coordinates. They are drawn *inside* that pass, which is where
the text was drawn before: the ortho projection, the alpha blending and
the disabled Z are already in effect, so this adds a texture and a UV
and nothing else. The atlas is not one of the engine's textures, so it
is bound straight to stage 0 and the cached id is left reading "nothing
bound" — which is true when the call returns, and is what stops the
next `D3D_SetTexture` from trusting a stale cache.

### The two faces, and why Liberation

`data/fonts` holds `LiberationSans-Regular.ttf` and
`LiberationMono-Regular.ttf`, which stand in for the Arial and Courier
New the engine asked for. Liberation exists to be metric-compatible
with those two, so text occupies the space it used to and a caret
measured by summing advances lands where it landed. `data/fonts/README.md`
has the numbers and the licence.

They are looked for beside the executable, under `data\fonts\`, not in
the working directory — the same place the sideload folder is, because
they are part of an install and not of wherever a shortcut points. The
separator is taken from whatever `GetModuleFileNameA` came back with
rather than assumed, which is also what lets the test drive this on a
host that has only the other one.

A missing face is logged **once** and then nothing draws: `FNT_i_Height`
and `FNT_i_TextWidth` answer zero and `FNT_DrawText` returns. Nothing
else in the engine changes behaviour, and the log does not repeat every
frame.

### What the atlas does

One texture per face, holding printable ASCII in a grid of equal cells,
on the smallest power-of-two square that fits. Equal cells waste a
little area and make the placement arithmetic checkable by eye. Each
glyph is loaded twice -- once to find the largest bitmap, once to copy
it in -- because FreeType keeps one glyph slot and the atlas cannot be
sized until the first pass has finished. That happens when the size
changes, not per frame.

The overlay's size still follows the viewport, a thirtieth of its
height clamped to 14..44, and asking for the height is what rebuilds
the atlas after a resize -- so the command field measures itself
against the size it is about to be drawn at. The raster face stays at
ten pixels, because the layout around it assumes that.

A character the atlas does not hold is drawn **and measured** as `?`.
Measuring and drawing agreeing matters more than the substitution being
pretty: the caret is placed from the measurement.

### The tests

`tests/run_font.sh` compiles the vendored FreeType and the real
`font.c`, points the module's idea of the executable at the repository,
and lets it read the committed faces. So glyph metrics come from the
same files the game reads rather than from a mock that would only check
the module's arithmetic against itself. 219 checks.

The renderer is mocked, which is what lets a case inspect the coverage
map and every vertex. **21 mutations, 21 caught**, and two of them only
after the harness was sharpened:

| mutation | what it took to catch |
| --- | --- |
| the left bearing dropped | a glyph chosen *because* its bearing is non-zero -- `A`'s is zero at this size, so the obvious check was vacuous |
| the minimum height off by one | a viewport at exactly `(14 - 1) * 30`, the only ratio where `<` against `<=` is visible |

The rest were caught as written: the baseline losing the ascender, the
glyph hanging the wrong way, UVs that stop short, a pen that does not
advance, the atlas never rebuilt, the old atlas not released, a missing
face retried every frame, a blank glyph emitting a quad, the colour on
only the first vertex, the grid filled down instead of across, the
triangle count halved, and the three refusal paths.

### Three things the change turned up

**`D3D_DrawARaster` still drew text.** Two calls in `win/wingdi.c` that
the first sweep missed, and they linked because the harness compiles
with `-w`, which also disables the error on an implicit declaration.
`tests/implicitcheck.sh` is what said so -- it exists for exactly this
-- and the calls now go through the font module. `sideload.c` was
carrying an implicit `GetModuleFileNameA` of its own; declaring that
call in the stub for `font.c` took it to zero, so it is off the
baseline list.

**The frozen copy and the live one draw raster text by different
machinery**, so the mocks were made to log it in the same words --
string hash, x, y, colour -- and the D3DX font's *creation* is no
longer recorded at all, because making a face is not a renderer call
any more. Everything else `D3D_DrawARaster` does stays compared, and
three mutations confirm it: dropping a string, moving it a pixel, and
changing its colour are all still caught. The colour is narrowed to 32
bits in both mocks on purpose: `D3DCOLOR` is `DWORD`, which the test
stub makes 64-bit wide on the host, so `-1` logged as `-1` on one side
and `4294967295` on the other while both carried the same colour.

**A positional initialiser in a mock's vtable.** Adding `LockRect` and
`UnlockRect` to the stub's `IDirect3DTexture9Vtbl` -- in the middle,
where they belong -- shifted every pointer after them in
`tests/wingdi_ae_mocks.c`, which filled that table by position. The
after-effects harness segfaulted. The members are appended now, the
stub says why order matters there, and the two Direct3D tables in that
file are filled **by name**: inserting a member mid-struct afterwards
leaves the harness passing, which is the check that the trap is really
gone.

### What this did not do, and what did

At the time, `win/overlay.c` still called `D3D_OverlayBegin`,
`D3D_OverlayDrawQuads` and `D3D_OverlayEnd` directly and read the
viewport out of the renderer's own state. Only the text calls had to
move then, because the glyphs had to come from somewhere other than
D3DX. The rest of that pass is routed now -- see **The overlay draws
through the interface** below.

## _CRT_SECURE_NO_WARNINGS, and why common.h was not enough

The engine builds with SDL checks on, which turns MSVC's deprecation
warning for `strcpy`, `sprintf`, `strcat` and their relatives into an
error. `common.h` has defined `_CRT_SECURE_NO_WARNINGS` at its third
line for as long as it has existed, and every engine source reaches it,
so the decompiled code full of those calls has always compiled.

Two things broke that, and the definition moved to
`irisEng.vcxproj` -- all four configurations -- where include order
cannot defeat it.

**FreeType never sees `common.h`.** It is upstream source and includes
only its own headers, and it reaches `strcat`, `strcpy`, `strncpy`,
`sprintf`, `fopen` and `getenv` through the `ft_*` aliases in
`ftstdlib.h`. Its per-file settings inherit the project definition
through `%(PreprocessorDefinitions)`, so nothing had to be added to
the thirteen entries themselves.

**`font.c` could not pick it up either, and the reason is worth
knowing.** Its first include is `font.h`, whose first line is
`#include <stdint.h>`. On MSVC that pulls in the CRT's own
configuration headers, which read `_CRT_SECURE_NO_WARNINGS` **once**
to decide whether the deprecation attributes get attached. By the time
`gdi.h` reaches `common.h` and defines it, that decision has been
taken. So a new file whose first header includes anything from the CRT
before `common.h` is unprotected, and the symptom is a compile error
in the new file while every file around it builds -- which reads like
a problem with the new code rather than with its include order.

Defining it in the project file removes the trap for any file added
later.

**And it has to be the only definition.** `common.h` kept its own at
first, on the grounds that it was harmless. It is not: the command
line makes the macro `1` and a bare `#define` makes it empty, so the
two replacement lists differ and MSVC reports a redefinition in every
translation unit that includes the header -- which is all of them.
`jpeg-6b/jconfig.h` had the same definition and the same effect, once
for each of the 46 jpeg sources. Both are gone, each leaving a comment
saying where the definition went.

The three calls in `font.c` that reported the error were replaced by
one bounded `snprintf` rather than being silenced, because that was
the better code anyway -- it reports truncation instead of quietly
producing a short path. The definition is for FreeType and for
whatever is added next.
## No D3DX at all

The two fonts were the visible part of the D3DX dependency. Six more
calls were left after them, all in `win/wingdi.c`, and the engine now
has its own:

| was | is | where |
|---|---|---|
| `D3DXMatrixMultiply` | `D3D_MatrixMultiply` | `win/wingdimath.c` |
| `D3DXMatrixTranslation` | `D3D_MatrixTranslation` | " |
| `D3DXMatrixOrthoOffCenterRH` | `D3D_MatrixOrthoOffCenterRH` | " |
| `D3DXMatrixInverse` | `D3D_p_MatrixInverse` | " |
| `D3DXLoadSurfaceFromMemory` | `D3D_b_SurfaceLoadFromMemory` | `win/wingditex.c` |
| `D3DXFilterTexture` | `D3D_b_TextureBuildMipLevels` | " |

`win/wingdi.h` includes no D3DX header, `d3dx9.lib` is gone from both
Win32 configurations, and the only places the engine says "D3DX" are
comments saying what a function replaced. The two frozen-copy font
pointers and the overlay's old height moved into
`tests/stub/d3dx9math.h`, where the frozen copy that still refers to
them lives.

### The matrices

Direct3D's conventions, not the engine's: row-major, row vectors,
translation in the fourth row, and a projection whose clip-space z
runs 0 to 1. They are in `win/` for that reason — `math.c` works in
the engine's own `Mtx4x4`, and a second renderer will want its own
projection with its own clip space.

Three of the four are the documented formulas. The inverse is the
general one, by the adjugate over the determinant, because the one
caller reaches it exactly when the matrix may carry scale or shear —
the branch beside it uses the engine's own rigid inverse from
`math.c`, and that is the case this one is not.

`D3D_p_MatrixInverse` keeps D3DX's contract: it returns `NULL` and
writes nothing when the matrix is singular, and fills in the
determinant when asked. A caller that ignores the result must not be
handed noise.

### The textures

`D3DXLoadSurfaceFromMemory` was called once, with the same dimensions
at both ends, a 32-bit source and a point filter — which together
mean a copy and nothing else, so that is what replaced it. The
destination is `X8R8G8B8` and the source `A8R8G8B8`: D3DX dropped the
alpha, and copying it into the byte `X8R8G8B8` ignores comes to the
same thing when the surface is sampled.

`D3DXFilterTexture` built the mip chain. The replacement averages
each (up to) 2x2 block per channel, **rounding rather than
truncating**, and an odd dimension halves to the floor with the last
row or column averaged against itself rather than dropped. Bit-exact
agreement with D3DX is not something this repository can verify — the
library is not here to compare against — so the claim is that the
filtering is equivalent in kind, and the rounding is a deliberate
choice recorded rather than an accident.

One of the two call sites filters a texture created with a single
level, where both the old call and the new one do nothing. The call
is kept because the texture there may be one that already existed and
carries levels.

### What the tests check, and what they stopped checking

Two new harnesses, both direct rather than differential, because
there is no earlier copy of this arithmetic to compare against.

`tests/run_wingdimath.sh` — **385 checks**. The reference inverses and
the reference product were computed outside the test in exact
rational arithmetic and pasted in; an expected value worked out with
the formulas under test would only prove they agree with themselves.
Each inverse is then checked the other way too, by multiplying it
back and asking for the identity, which depends on neither. The
projection is checked by where it sends the corners of its box rather
than by comparing sixteen numbers against the same formula.

**Every one of the sixteen adjugate signs was flipped in turn, and all
sixteen were caught** — but only after a fifth test matrix was added.
Four matrices with zeros in them missed a sign error on one term,
because that term was multiplied by zero in all of them; the fifth
has no zero entries anywhere.

The projection case also got the handedness wrong at first, and
failed against correct code: in a right-handed projection the near
plane sits at view z = **-zn**, while `zn` and `zf` are given as
positive distances. That is written into the test, because it is the
kind of thing that reads as a bug in the implementation.

`tests/run_wingditex.sh` — **44 checks**, under AddressSanitizer, with
textures and surfaces that have real buffers behind them and pitches
deliberately wider than their rows. 17 mutations, 17 caught. Two
needed the harness sharpened:

| mutation | what it took |
| --- | --- |
| the clamp on x removed | a **1-pixel-wide** texture. With floor-halved dimensions a column never lacks a partner *unless* the width is already 1 while the height is still halving, so a 3x1 case scored clean |
| truncating instead of rounding | a block averaging to exactly .5 |

**The renderer's own differential harness checks less than it did, in
two places, and more in a third.**

Less: `D3DXMatrixInverse`'s mock was knowingly an approximation —
"transpose the 3x3 and negate the translation", which is not a
general inverse and not even a correct rigid one. No correct
implementation can match it, and two different correct algorithms
would not match each other in the last bits either, which an exact
comparison cannot tolerate. So that mock now delegates to the
implementation, and the differential harness says nothing about the
inverse; `run_wingdimath.sh` is what checks it.

Less: the two texture functions are **stood in for** rather than
linked. `win/wingditex.c` really walks the chain, locking each level
and averaging it into the next, and the textures in
`tests/wingdi_mocks.c` are not that — every level hands out one
shared buffer and reports the same dimensions, so a real box filter
would read far past it. Both sides now log the same line, so what
stays compared is the call: which object, what pitch, what size. The
old D3DX mocks moved no pixels either, so nothing that used to be
compared has been lost — only the level and filter arguments, which
the replacements do not take.

More: `SetTransform` recorded **the diagonal** of every matrix, and
the diagonal cannot see the fourth row — which is exactly where a
translation and a projection's offsets live. It now records a hash of
all sixteen floats as well. That turns the comparison into a real
bit-for-bit check of three of the four matrix functions against the
reference formulas in the mock, for every matrix that reaches the
device. Four mutations confirm it: dropping the projection's x offset
or its depth offset, dropping the translation entirely, and shifting
every product by one part in ten million are all caught, and none of
them would have been before.

## The OpenGL renderer, as far as it goes

A second row in the table:

```c
{ "gl", "OpenGL 4.5 core", GDI_l_FillGL, GL_b_Available },
```

**Direct3D is still first**, which is what makes it the fallback and
what the game gets when the ini asks for nothing. OpenGL is reached by
asking for it, and declines itself on a machine that cannot give a 4.5
core context.

What it does so far: comes up, clears, presents. Nothing is drawn
through it. 31 of the 51 interface entries are present and **inert**,
because an entry left NULL is a crash rather than a missing feature,
and each one carries a comment saying what it waits on. They answer
with the values that make the engine carry on rather than wait:
nothing ready, nothing created, nothing to read. The log says this
once at startup, next to the driver's vendor and renderer strings —
because the first question about a renderer that draws nothing is
which driver answered.

A few of those inert answers land on paths that already existed for
other reasons, which is why they are safe:
`b_VideoDisplayCreate` returning 0 is what the engine already does
with a missing video file; `p_CreateAlphaTexture` returning NULL makes
`font.c` log once and draw no text; `b_SunSampleRead` returning 0 is
indistinguishable from a sample still in flight, which the sun's ring
was built to tolerate.

### 4.5 core, and no fallback below it

There is no compatibility profile and no lower version. The point of
the rewrite is the modern API, and a renderer that quietly came up on
3.3 would fail later and further from the cause. A machine that cannot
give 4.5 core makes the row unavailable and the picker falls back to
Direct3D, which is the behaviour the picker was built for.

### The loader

Windows ships headers for OpenGL 1.1 and nothing else, so everything
past that is a function pointer the driver hands over at run time.
`win/glcore.h` is the part of the GL headers the renderer uses,
written out as one X-macro list — which is what the declarations, the
loader and the test are all generated from, so an entry point cannot
be added to one and missed in the others. The list holds what is used
and nothing more: an entry point declared but never called is a name
that has to resolve for no reason, and on a driver that happens not to
export it that is a renderer refusing to start over a function it
would never have called.

`win/glload.c` fills the pointers, and **nothing in it calls
Windows**. The two ways of resolving a name are passed in, which is
the whole reason it can be tested: a driver's `wglGetProcAddress` is
not something a test can arrange.

Two things it gets right that are easy to get wrong:

**Both lookups are needed and neither is enough.** A driver's
`wglGetProcAddress` answers for everything its context added past
OpenGL 1.1 and is entitled to refuse the 1.1 entry points, which
`opengl32.dll` exports itself. Asking the module alone gets those and
nothing newer.

**A failed lookup is not always NULL.** `wglGetProcAddress` is
documented to be allowed to return 1, 2, 3 or -1 as well, and those
have been seen in the wild. Taken at face value each is a function
pointer that crashes on its first call, a long way from the loader.
All four are screened.

### Getting the first context

The awkwardness at the heart of this: the function that creates a
modern context can only be found through a context that already
exists. So a throwaway window gets a throwaway 1.1 context, that
context answers for `wglCreateContextAttribsARB`, and the real context
is made with it.

It has to be a **separate** window, not the game's. A window's pixel
format can be set once and only once, and the format that suits a 1.1
context is not necessarily the one the real context wants.
`GL_b_Available` spends a third throwaway window for the same reason:
asking the driver whether it can give 4.5 core is the only answer that
is not a guess, and asking it on the game's window would spend that
window's one chance.

### What is tested, and what cannot be

`tests/run_glload.sh` — **55 checks**, under AddressSanitizer. Which
lookup answers, each of the four sentinels, every pointer landing in
its own variable (a loader that shifted them all by one would
otherwise score), which name is reported when one is missing, buffer
truncation, and seventeen version strings including real ones from
NVIDIA, Mesa and AMD. 16 mutations, 16 caught.

Two of those mutations started as misses, and both were **the code's
fault rather than the test's**: a special case rejecting OpenGL ES
strings could not fire, because an ES string does not begin with a
digit and the guard below already rejected it. It is gone, and the
comment on the guard that does the work says so. The other was the
same shape, and that guard stayed with a test pinning the contract it
rests on.

**The context creation itself is not tested here** and cannot be:
there is no driver behind the stub. What the harness does give it is a
compile — `win/wingl.c` is Windows-only, so building it in the test is
the only check it gets — and the file logs generously at every step, so
the first run on a real machine says where it got to rather than
needing to be reasoned about from here.

`tests/ifacecheck.sh` is new and applies to both renderers: it reads
the entry names out of `gdi.h` and checks every fill function in
`gdi.c` assigns all of them. A renderer is a row plus a fill function,
and an entry that function leaves alone is a NULL the engine will
call; nothing about that is enforced by the compiler. It is a text
check on purpose, since the fill functions are static and driving them
would mean linking the engine. Three mutations confirm it: an entry
dropped from either row, and an entry added to the interface that
neither row fills.

### A blind spot in implicitcheck.sh

Adding `font.c` to the build should have put it in that sweep, and did
not. The sweep counts lines matching one phrase, so a file that stops
with a *different* error produces none of it and scores a clean zero —
`font.c` was missing FreeType's include path and had never been
checked at all.

It now reports any file it cannot compile, separately from the count,
and carries a short list of the ones that genuinely cannot be built on
this host. There is one: `loading.c` casts `call_fgetpos()`'s result to
`unsigned int`, and `fpos_t` is an integer on MSVC and a struct in
glibc, so the cast is right where the engine is built and impossible
here. It is listed rather than fixed — changing engine code to suit
the test host is the wrong way round — and the check also fails if a
listed file starts compiling, so the list cannot go stale.

The sweep no longer walks `thirdparty/`, for the same reason it never
walked `jpeg-6b`: upstream source is not ours to tidy, and FreeType's
needs defines this sweep does not pass, so it was failing for other
reasons and counting zero.

## The overlay draws through the interface

`win/overlay.c` was the last thing in the engine talking to Direct3D
by name. Seven bypasses: three reads of the viewport out of
`D3DRenderParams`, a null check on the device, and the three calls
that make up the 2D pass. A renderer that is not Direct3D could not
satisfy any of them, so the debug overlay was Direct3D-only by
construction.

Four entries, and the viewport now comes from `GetDrawingSize`, which
was already there:

```c
int (* b_Ready)(void);
void(* OverlayBegin)(void);
void(* OverlayDrawQuads)(const void* vertices, int triangleCount);
void(* OverlayEnd)(void);
```

`b_Ready` is the device check made renderer-neutral. The overlay runs
from the message loop and can be asked to draw before a device exists
or after one has gone, so the question is real and not Direct3D's
alone — the OpenGL row answers it from whether it has a context.

### The vertex type

The shape batch used `tVertexAndColor` from `win/wingdi.h`. It is
`tGdi_Vertex` in `gdi.h` now, and the renderer passes
`sizeof(tGdi_Vertex)` to `DrawPrimitiveUP` as the stride.

The two are the same shape, which is why adopting it moved nothing —
**and that is asserted rather than assumed**, in the one harness where
both types are visible: same size, colour at the same offset. The
frozen copy still uses the old type, so a mismatch would show up as a
comparison difference rather than as a crash, and a mutation
confirms it does.

### What it changed for OpenGL, which is nothing yet

The three pass entries are present and inert there, like the rest. The
overlay calls all three every frame it is visible, so they have to
exist; what they do is nothing, and the screen stays as the clear left
it. They are the next thing to become real, because the overlay is the
smallest thing worth drawing: 2D, no matrices from the engine, and
visible the moment it works.

### The tests

The overlay's harness stood in for `win/wingdi.c` — it included that
header, defined `D3DRenderParams` because the header declares it
without `extern`, and drove the viewport through it. It stands in for
the interface now, which is both smaller and the right shape: a
`tGdi_DisplayData` with five entries filled. 162 checks, unchanged.

Five mutations confirm the routing is watched rather than merely
compiled: the pass never begun, never ended, drawing with no renderer,
the drawing size never asked for, and a vertex layout that no longer
matches.

## OpenGL draws the overlay

The first thing this renderer draws. Six of its entries are real now —
the three that make up the 2D pass, and the three that put a glyph
atlas on screen — so asking for `backend = gl` and pressing the
overlay's key draws the panel, the raster readouts and their text.
Everything else is still inert.

The overlay was chosen deliberately: it is 2D, it needs no matrices
from the engine, and it exercises shaders, vertex buffers, vertex
arrays and textures end to end. If it draws, the pieces the geometry
path will need are working.

### The shaders are files

`data/shaders` holds four of them, read at run time:

| | |
|---|---|
| `overlay.vert` / `.frag` | flat quads |
| `overlaytext.vert` / `.frag` | glyphs, sampling the atlas |

Read rather than compiled in, because a shader is the part of a
renderer most likely to need changing on somebody else's machine, and
a file that can be edited with the game restarted is worth more than
one that needs a rebuild. A missing or broken one is reported **with
the driver's own log**, which is the only thing that names a line, and
then the pass draws nothing rather than drawing wrongly.

Two programs rather than one with a branch, because their vertex
layouts differ and a vertex array object describes one layout. A
branch would still need two of those.

**No projection matrix.** Positions arrive in pixels with the origin
at the top left, which is what the engine's 2D code has always worked
in; mapping that to clip space is two lines and the viewport size, so
the viewport is the only thing passed in.

**`vColor = aColor.bgra` is not a mistake.** The engine's colours are
packed A8R8G8B8 words, and on a little-endian machine the bytes of one
are blue, green, red, alpha in that order. Read as four normalised
bytes that is what the attribute holds, and the swizzle puts them
back.

### The path rule is shared now

`font.c` resolved `data\fonts` itself. The shader loader wants
`data\shaders` by the same rule, and two copies of a path rule drift
in opposite directions — this repository has the scars. It is
`datapath.c` now, one function, and `font.c` calls it.

### The handle that is not a pointer

A GL texture is a name, an integer, and the interface hands atlases
back as `void*`. They travel **offset by one**, because zero is "no
texture" to GL and NULL to everyone else, and an atlas that failed to
create has to be distinguishable from atlas number zero.

### Why all of this is testable

**Every GL call in the renderer goes through a function pointer**, so
the pointers can be aimed at mocks and the whole pass driven with no
driver anywhere. That is worth more than it sounds: a case can say
"this stage will not compile", "the link fails", "the driver will not
give a buffer name" — and then check what was *cleaned up*, which on a
real machine stays invisible until something runs out of objects.

`tests/run_glshader.sh` — **63 checks** under AddressSanitizer, 15 of
15 mutations caught. Both stages compiled as the right kinds, the
bytes on disk reaching the driver unchanged, the driver's log printed,
and every refusal path freeing what it had made.

`tests/run_wingloverlay.sh` — **103 checks** under AddressSanitizer,
24 of 24 mutations caught. It reads **the real shipped shaders**, so a
uniform renamed in a `.frag` and not in `wingl.c` is a failing test
rather than a black overlay on somebody's machine. And the vertex
layouts are checked against `offsetof` on the actual structs rather
than against the numbers written in `wingl.c` — an offset written by
hand and a struct that moved would otherwise agree with each other and
with nothing else.

Four mutations needed the harnesses sharpened first, and all four were
the tests being vacuous rather than the code being right:

| mutation | why it passed |
|---|---|
| the text batch bound the flat vertex array | nothing checked *which* array a draw bound |
| width and height swapped in the atlas upload | the test atlas was square |
| an empty program resolving uniforms | the mock's answer happened to be -1 either way |
| the shader source left unterminated | the test searched the source for a substring instead of comparing it |

### What is still inert

The geometry, the textures the world is drawn with, the video display
and the sun's occlusion test. 25 of the 55 entries. The geometry path
is the next one, and it is the one that needs the fixed-function
pipeline emulating rather than replacing — texture stages, fog, alpha
test — which is a larger piece of work than anything here.

## Fitting a camera to a surface is not Direct3D's job

`D3D_SetProjectionMatrix` was 169 lines, and almost none of it was
Direct3D. It worked out the viewport a camera occupies — the aspect
ratio the game was authored for, the letterboxing rules, the pixel
aspect — and then built one of three projections: a 2D one for drawing
into a target, a perspective one, and an isometric one. Only the last
three lines handed anything to a device.

A second renderer needs exactly those answers, and a second copy of a
hundred and twenty lines of it would drift. It is `projection.c` now.
`PRJ_Compute` reads a camera and a description of the surface and
fills in a matrix and a rectangle; the renderer hands the matrix over
and sets the viewport.

**The extraction is bit-for-bit.** 5,154 cases and 147,601 engine
calls compared against the frozen copy, no differences — which is
what the full-matrix hash added to `SetTransform` in the D3DX work was
for. 11 of 11 mutations in the extracted code are caught by that
comparison alone.

### Three things it would have been easy to get wrong

**`MATH_SetIdentityMatrix` masks the matrix's flag word rather than
clearing it** — `mtxflag &= 0x2`. So *which* matrix it is handed is
not a detail. The first version of this built into a local and copied
sixteen floats to the camera afterwards; the camera's flag then kept
its old value instead of being masked, and the harness reported a
state difference at a fixed offset in every projection case. The
computation writes the camera's matrix in place now, because that is
what the renderer did.

**All three branches touch the camera's matrix, and only two leave the
projection in it.** The isometric branch sets the camera's matrix to
the identity and then builds its projection somewhere else entirely —
so the camera is left holding an identity that is *not* the
projection. Something reads that. It is preserved, and the header says
so.

**The 2D branch returns without setting the viewport.** It is used for
drawing into a target whose viewport was already chosen. The output
carries that as a flag rather than the caller guessing.

### What was not tidied

`PRJ_OffCentre` writes its offsets into the **fourth column**, which is
the transpose of the convention the perspective projection next to it
is written in, and its depth scale is `-4/(far - near)` where a
textbook orthographic projection has `-2`. Both are as the game
shipped. The comment says they are odd; the code leaves them alone.

The renderer still exposes `D3D_CalcAndSetProjection` under its own
name because the engine calls it directly, but it is two lines over
`PRJ_OffCentre` now, so the arithmetic has one home.

### What this is for

The OpenGL renderer can have the same projection without a second copy
of any of it. That is the prerequisite for drawing geometry through it,
which is the next piece and a larger one: the geometry path is where
the fixed-function pipeline has to be emulated rather than replaced.

## Building the geometry's vertices is not Direct3D's job either

The engine keeps a mesh as separate arrays — positions here, texture
coordinates there, two colour arrays somewhere else — and a renderer
wants interleaved vertices. The immediate-mode half of
`D3D_l_DrawElementIndexedTriangles` walked a chunk of triangles,
picked each corner out of those arrays and packed the result.

None of that is Direct3D's. It is `geovertex.c` now: `GVX_BuildTriangles`
takes the arrays and a description of how to read them and fills in a
buffer. What stays with a renderer is where the buffer comes from and
how it is drawn.

**Bit-for-bit again**, over the same 5,154 cases, and **11 of 11
mutations caught** before the run was cut short by its own length — the
red/blue swizzle dropped, the second colour set ignored or masked
wrongly, the mask applied with `|` instead of `^`, the index-by-four
quirk dropped or moved to the wrong corner, two corners swapped, UVs
read per-vertex when they are per-corner and the other way round, the
UV corners swapped, and `u` and `v` swapped.

### The colour packing is a swizzle

`GVX_ui_Color` ends with

```c
packed & 0xFF00FF00 | ((uint8_t)packed << 16) | ((packed >> 16) & 0xff)
```

which exchanges the first and third bytes. There is a second copy of
that shape for the single-colour case, `GVX_ui_SolidColor`, written
differently: one of its three terms masks the colour's low byte before
the exclusive-or and the other two do not.

**That mask turns out to be redundant.** The term is truncated to a
byte afterwards, so `(uint8_t)(mask ^ (alpha | (c & 0xff)))` and
`(uint8_t)(mask ^ (alpha | c))` are the same value for every input —
which is why removing it is a mutation the harness cannot catch. It is
kept anyway, because it is what shipped and the point of these
extractions is to move code rather than improve it. Both expressions
are now in one file where they can be compared, instead of one of them
being inline in a hundred-line function.

### The quirk that is kept

One of the second-colour lookups multiplies its index by four, and
only on the middle corner of a triangle, and only when the geometry has
per-vertex texture coordinates:

```c
if (src->uvs && src->useVertexUv && k == 1)
    secondIdx = idx * 4;
```

It reads like a bug and it may well be one. It is preserved, with the
comment it had, because the game shipped with it and the difference
would be visible.

### The vertex types line up

`tVertex_type_142_generic` — the renderer's geometry vertex — and
`tGdi_TexturedVertex` — the interface's textured one — are the same
shape: position, packed colour, one texture coordinate. So the builder
writes the interface's type and the renderer casts its buffer to it,
and the OpenGL renderer can feed geometry through the vertex layout it
already describes for text.

That cast is **asserted** in the wingdi harness, both sizes and all
three offsets. A field added to either type would otherwise make it
write the wrong bytes with nothing to say so.

### What is still in the renderer

The cached path. When the geometry has an index buffer and a triangle
strip block and nothing has changed under it, the renderer draws from
what it cached rather than rebuilding anything — and that half is
genuinely about buffers belonging to a device. The immediate path is
the one a second renderer needs first, because it is the fallback
whenever the cache does not apply.

## OpenGL draws the world

Untextured, vertex-coloured, but the world. Three more entries are
real — the view matrix, the projection, and the indexed triangle draw
— and almost none of what they do is new code: `projection.c` fits the
camera and `geovertex.c` builds the vertices, both shared with the
Direct3D renderer. What is here is the matrix the shader gets, the
buffer the vertices go in, and the draw.

### glClipControl is most of why this renderer asks for 4.5

The engine's projections put clip-space z in **0 to 1**, which is
Direct3D's range and not OpenGL's −1 to 1. Core 4.5 can be told to use
it:

```c
GLFN_glClipControl(GL_LOWER_LEFT, GL_ZERO_TO_ONE);
```

So the engine's matrices need no adjusting and `projection.c` can be
shared between both renderers rather than having a variant for each.
The origin stays at the **lower left**, OpenGL's own: both conventions
put clip y = +1 at the top of the screen, one measuring the window
from the bottom and the other from the top, so nothing is upside down
and the overlay's pixel mapping is untouched.

### The matrix reaches the shader without being transposed

The engine's matrices are row-vector — a point is transformed as
`v * M` — and stored row-major. Uploaded with `transpose` false,
OpenGL reads that storage as column-major, which hands GLSL the
transpose; and the transpose times a column vector is the row vector
times the original. So `uViewProj * vec4(aPos, 1.0)` in the shader is
the engine's own arithmetic, not a rearrangement of it, and no bytes
are moved on the way.

### The viewport's y is measured from the other end

`PRJ_Compute` gives a rectangle in the engine's window coordinates,
whose origin is the top left. OpenGL's viewport origin is the bottom
left, so

```c
GLFN_glViewport(out.viewX, gl_state.height - (out.viewY + out.viewHeight),
                out.viewWidth, out.viewHeight);
```

**A centred viewport cannot see that flip**, which is worth knowing.
Letterboxing alone produces a rectangle `(height - fit) / 2` from the
top and the same from the bottom, so the flipped y and the unflipped
one are the same number. The first version of the test used a
letterboxed camera and passed with the flip removed. It uses a camera
with an off-centre, half-height viewport now — an inset or a split
screen — which is the only shape that tells them apart, and is exactly
the case that would have shipped broken.

### Two settings stopped being Direct3D's

`D3D_gi_Widescreen` and `D3D_gf_RefYoverX` were defined in the
Direct3D renderer and named after it, and `cameraengine.c` read them
anyway. They are `GDI_gi_Widescreen` and `GDI_gf_RefYoverX` in
`gdi.c` now, because every renderer's projection depends on them, and
the alternative was the OpenGL renderer declaring a Direct3D symbol.
The frozen copy reaches them through a `-D` mapping of the old names;
the differential harness drives them as one shared pair rather than
one per side, which is what the engine does. No differences.

### What the harness checks

`tests/run_wingloverlay.sh` grew from 103 checks to **136**, still
with no driver anywhere. The geometry cases drive the real
`GVX_BuildTriangles` and the real `PRJ_Compute`, so what the mocks see
is what a driver would be handed.

8 of 8 mutations caught, and **two of them only after the test was
fixed**:

| mutation | why it passed at first |
|---|---|
| view and projection multiplied the other way round | nothing checked the combined matrix's *value*, only that a matrix was uploaded |
| the "leave the viewport alone" flag ignored | every case used a camera whose flag was the other way |

The first is now compared against a product worked out in the test
itself rather than by the helper the renderer uses — the order is the
whole question, and two matrices multiplied the wrong way round is a
picture that looks plausible and is wrong. Writing that case also
turned up that `MATH_MakeOGLMatrix` sets `w` to 1, which changes the
four products of that row; the expected value takes the conversion as
given and checks the order.

### What is still inert

Textures, the video display, the sun's occlusion test, sprites, and
the cached geometry path. Culling is off: which winding the engine's
geometry has is a question to answer by looking at the result rather
than by reasoning here, and drawing both faces is the reading that
cannot hide anything.

## What the ini does not decide

Only *which* renderer. Everything about how a renderer behaves is the
renderer's own business, and `datafiles.c` deliberately knows nothing
about what the names mean: it carries the string across and `gdi.c`
decides.
