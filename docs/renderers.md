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

## What the ini does not decide

Only *which* renderer. Everything about how a renderer behaves is the
renderer's own business, and `datafiles.c` deliberately knows nothing
about what the names mean: it carries the string across and `gdi.c`
decides.
