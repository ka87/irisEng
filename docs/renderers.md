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

## What the ini does not decide

Only *which* renderer. Everything about how a renderer behaves is the
renderer's own business, and `datafiles.c` deliberately knows nothing
about what the names mean: it carries the string across and `gdi.c`
decides.
