# gfx/

Twenty files are in the build. Three of them — `gfxborderfade.c`,
`gfxanotherflare.c` and `gfxexplode.c` — were already written out and
needed nothing but a header tidy; the other seventeen were rewritten,
**7,131 lines to 4,834**, and the directory as a whole went from
**8,395 to 6,098**.

`gfx/gfxexplode__.c` is deliberately untouched. It is not in
`irisEng.vcxproj`, it is a second copy of `gfxexplode.c` kept beside
it, and it references a `vecPtr` member its own header does not
declare, so it would not build if it were added. Nothing in this pass
reads it.

All twenty parse under the stub include set as standard C with
`-Werror=implicit-function-declaration`, none contains a `goto` (there
were **36**), and no source or header in the directory carries a
decompiler-named local or parameter. The *after* column below is not
zero for every file because `v1`, `v2`, `v3` and `v0` are real field
names in the engine's own structs — a matrix's basis vectors, a
profiler point's position and velocity — and those stay.

| file | lines | `v`/`a` names | gotos | test |
| --- | --- | --- | --- | --- |
| `gfxgao.c` | 47 to 45 | 5 to 0 | | |
| `gfxspeed.c` | 129 to 67 | 25 to 2 | | yes |
| `gfxline.c` | 157 to 85 | 34 to 3 | | yes |
| `gfxfade.c` | 141 to 98 | 21 to 0 | 1 | yes |
| `gfxwater2.c` | 197 to 155 | 33 to 3 | 1 | yes |
| `gfxtable.c` | 264 to 193 | 40 to 3 | | yes |
| `gfxdiaphragm.c` | 477 to 203 | 71 to 3 | 4 | yes |
| `gfxsmoke.c` | 272 to 219 | 33 to 3 | | yes |
| `gfxstr.c` | 277 to 233 | 34 to 3 | 4 | |
| `gfxripple.c` | 329 to 235 | 35 to 3 | | yes |
| `gfxdesintegrate.c` | 437 to 257 | 57 to 3 | | |
| `gfxflare.c` | 481 to 383 | 66 to 3 | 3 | yes |
| `gfxspark.c` | 658 to 398 | 69 to 4 | 5 | yes |
| `gfxlightning.c` | 709 to 473 | 61 to 3 | 6 | yes |
| `gfxprofiler.c` | 591 to 513 | 42 to 6 | | yes |
| `gfxcarte.c` | 795 to 596 | 92 to 2 | 9 | |
| `gfxsun.c` | 1170 to 681 | 125 to 4 | 3 | |

Twelve of the seventeen have differential tests, in five binaries, and
they run **887 cases**. Every path that can be compared is equivalent;
the three that cannot are each a decoder finding, and each is set out
below and checked another way.

## The tests

| binary | files | cases | result |
| --- | --- | --- | --- |
| `test_gfxgrid` | water2, ripple, smoke | 130 | 1196 floats differ below tolerance |
| `test_gfxlightning` | lightning | 141 | every value bit-identical |
| `test_gfxspark` | spark | 133 | 359 floats differ below tolerance |
| `test_gfxprofiler` | profiler | 190 | 353 floats differ below tolerance |
| `test_gfxdraw` | diaphragm, fade, line, table, speed, flare | 293 | 240 floats differ below tolerance |

Each binary prints its own case count, so these figures come from the
run rather than from counting the source.

They are built by `tests/run.sh`, which gained a `run_group` so one
driver can drive several effects that share a mock set.

Three things are worth knowing about how they are put together.

**The real `math.c` is linked, not mocked.** The gfx effects lean on
the engine's trig and reciprocal tables, its vector helpers and its
random sequence, and `math.c` compiles standalone under the stub set,
so nothing stands in for it. `MATH_InitModule` runs once before the
first case and builds the four tables.

**The 15-bit generator is supplied.** `MATH_RandFloat` is
`rand() * (1.0f / SHRT_MAX)`, which assumes MSVC's 15-bit `rand`.
glibc's is 31-bit, so under it every effect that draws a random number
runs four orders of magnitude out of range: the spark's emission
speeds came out in the tens of thousands, and the comparison then
measured rounding in a domain the effect never enters. The mocks
define `rand` and `srand` as MSVC's own linear congruential
generator, and the spark's differences dropped from "wildly apart" to
2.4e-07.

**The allocator leaks on purpose and carries a guard band.** The first
side's state has to stay readable while the second side runs and the
two are compared, so nothing is handed back between them; the call log
records every allocation instead, which is what actually catches a
change in allocation behaviour. And every block is handed out 64 bytes
into a larger one, because a frozen copy may write a little way before
what it was given: the real engine writes into whatever the heap put
there and runs on, while glibc aborts the process. Without the guard,
the draw binary died at the first table case and the five groups of
cases after it never ran.

### The float-difference class, again

As in `modifiers/`, restoring an inlined helper rarely reproduces the
bits. The decompiler shows x87 registers as `double`, so
`v12 * v13 + v14` in a frozen copy is a double-precision expression,
while `MATH_AddScaleVector` computes in `float`. The differences are
in the last place or two:

| binary | floats differing | worst absolute | worst relative |
| --- | --- | --- | --- |
| grid | 1196 | 1.4e-06 | 2.6e-05 (abs 2.4e-07) |
| lightning | 0 | — | — |
| spark | 359 | 2.4e-07 | 1.4e-05 (abs 4.0e-08) |
| profiler | 353 | 2.4e-07 | 1.5e-05 (abs 1.2e-07) |
| draw | 240 | 1.5e-05 | 6.0e-07 (abs 1.5e-08) |

A difference fails a case only when it is large both absolutely and
relatively; a relative difference of 1e-05 on a value of 1e-07 is the
last bit of a near-zero number and says nothing.

### What the tests found that nothing else would have

Three. All three are in files the parse check and the review had both
passed.

**`gfxtable.c` was writing its vertex colours at the wrong indices,
and it was writing outside the array.** The frozen copy stores each
cell's four colours at `4n+1`, `4n`, `4n-1` and `4n-2` — backwards
from its cursor — so cell zero writes two ints in front of the colour
array and every later cell overwrites its neighbour's colour. The
rewrite writes `4n+1` to `4n+4`, which is where the four vertices of
cell *n* read theirs. The two arrays therefore cannot be compared, so
the test checks the rewritten side against what the geometry asks for,
cell by cell, and the site carries a comment — the out-of-bounds write
is the kind the user asked to have fixed and commented.

**`gfxflare.c`'s sub-flare chain reads nine structs, not nine
fields.** The frozen copy advances a cursor of the whole effect's
type, so `v14->i29`, `v14->color3` and `v14->materialNum1` step by
`sizeof(tGfxFlareData)` and eight of the nine sub-flares come out of
memory the effect does not own: the test saw materials of
`0xBF800000` and `0x56EE3E3B` where the rewrite reads 4, 5, 6. The
rewrite's reading — four parallel runs of nine fields — is set out
under `gfxflare.c` below. The nine-sub-flare path is therefore left
out of the comparison and checked against those fields directly.

**`gfxprofiler.c`'s slice-count setter divides by it.**
`GFX_SetNumberOfP` computes `255 / num`, in both copies, so a slice
count of zero takes the process down. That case is left out of the
test, named in the file, and left alone in the engine: it is a real
defect but not one this pass created, and the setter is only ever
reached from a script.

---

## gfxgao.c

Three functions that put a game object into the gfx render list. It
needed almost nothing: `GFX_Gao_Create` lost five decompiler locals
and the focale override in `draw_game_object_in_GFX_sub_45DBB0` became
an `overrideFocale` flag rather than a pair of shadowed temporaries.
The `sub_45DBB0` in the name is the engine's own and not this pass's
to change — nothing else in the tree knows the function by another
name.

### Pick — the focale test

`*(uint32_t*)&gfx->focale != 0` is kept as a bit test rather than
written `gfx->focale != 0.0f`, because the two differ on negative
zero and the frozen copy tests the bits.

## gfxspeed.c

A one-quad trail between where an object was last frame and where it
is now. 129 lines to 67.

The sentinel is the thing to know: `v2.x` holding the bit pattern
`-1` means "no previous position yet", and the first frame only
records one and returns. That is `*(int32_t*)&speed->v2.x == -1`,
still a bit test, and `GFX_Speed_Create` carries a one-line comment
saying that the render looks for it.

### Pick — `!(speed->v2.z >= 0.0)` *(binary)*

The frozen copy tests the positive form and takes the other arm, so a
NaN previous depth falls through to the early return. Written
`speed->v2.z < 0.0` a NaN would draw. The test covers it.

## gfxline.c

A camera-facing quad along a line segment. 157 to 85.

The nearest-point clamp was three overlapping blocks of component
arithmetic; it is the obvious `along` parameter with three arms. The
degenerate case — the line pointing straight at the camera, so the
cross product is null — falls back to the line's own up vector, which
is what the frozen copy does through a flag.

### Pick — `!(along >= 0.0)` *(binary)*

Same shape as the speed trail: the frozen copy tests `>= 0.0` and the
NaN from a zero-length segment must take the first arm. The test has a
zero-length case.

## gfxfade.c

A full-screen colour wash with a five-stage envelope. 141 to 98, and
the one `goto` is gone.

The stage walk was a loop with the exit folded into the body. It is a
`for (stage = 0; stage < 5; stage++)` with a `break`, and the arm
selection a `switch`. Stage 0 and stage 4 both collapse the blend to
zero and share a `case` label, which is what the frozen copy's shared
tail was.

### Pick — the loop test, `blend <= factors[stage]` *(binary)*

The loop has to run all five times for a NaN elapsed time, so that the
function returns 0 and the effect ends, and it has to stop at the
first stage whose span contains the time. Testing `<=` and breaking
does both; testing `>` and continuing does not. The test includes a
NaN case.

## gfxwater2.c

Splash drops, each a single triangle along its own velocity. 197 to
155.

The compaction is the interesting part and it now says what it does in
two comments: a drop that is still alive advances both cursors, and a
drop that has expired is replaced by the last one in the list and the
same slot is looked at again. The frozen copy expressed that with a
`goto` and two counters that only sometimes moved together.

## gfxtable.c

A grid of coloured quads in screen space, for the inventory and the
debug overlays. 264 to 193.

Two walks: rows and columns accumulating the running `x` and `y`
edges, then a second pass over all `4 * cells` vertices projecting
them. The frozen copy interleaved both with a vertex cursor that it
read through negative offsets.

The colour fix is above. Two things are *not* fixed, and both carry a
line saying so:

- `GFX_Table_Setv`'s bound is the array length and not its last
  index, so an index of ten writes past `f[9]` into `f2[0]` — inside
  the struct, so it is a quirk and not a fault. Left as it stands.
- `GFX_Table_Create` advances only the `f` cursor, so `colors` and
  `f2` are written at index zero ten times over and their other nine
  entries keep the zero the `memset` left. Left as it stands: the
  initial values are the author's, whatever the loop looks like.

## gfxdiaphragm.c

The iris wipe: eight blades closing on a point. **477 lines to 203**,
the largest proportional saving in the directory, and four `goto`s
gone.

The blade loop was unrolled eight times in the frozen copy — eight
near-identical blocks of a dozen lines each, with the blade angle
written out as a literal. It is a `for` over a `static const double[8]`
of `pi/4` steps, which is what those literals were:

```c
static const double GFX_l_DiaphragmBladeAngles[8] =
{
    0.0, 0.78539819, 1.5707964, 2.3561945, 3.1415927, 3.926991, 4.712389, 5.4977875
};
```

The values are the frozen copy's own rounded decimals and not
recomputed multiples of pi, so the geometry is bit-identical. The test
confirms it.

The corner stitching became two small helpers,
`GFX_l_DiaphragmTriangle` and `GFX_l_DiaphragmCorner`, the second with
a comment: a blade tip's screen corner is bit 0 for the right half and
bit 1 for the lower, which is the index of that corner in the first
four vertices. The frozen copy computed that twice per blade with the
bit arithmetic spelled out.

### Pick — the stage walk as a `do`/`while` *(binary)*

The envelope walk keeps its `do`/`while` with the `return 0` inside,
because the first test is the loop-entry guard `!(blend < f01[0])` and
hoisting it would duplicate it.

## gfxsmoke.c

Puffs rising and spreading. 272 to 219.

The render's 60 Hz sub-stepping is the only subtlety: the elapsed time
is converted to a whole number of 1/60 s steps, the remainder is
carried, and `GFX_Smoke_Update` takes the step count. The frozen copy
had the conversion inline with the `f2iMagic` double trick around it;
it is a plain cast now, and the test's `smoke update steps=0..4` cases
cover the inner loop.

## gfxstr.c

The on-screen string effect: a quad per letter, with UVs out of the
font atlas. 277 to 233, four `goto`s gone.

`GFX_Str_Seti` was the goto maze — the alignment adjustment, the font
material switch and the glyph lookup were all interleaved. They are
now three straight blocks: the alignment, if the string has any
alignment flags; then a per-letter loop that switches the font
material only when the letter's material id changes, and reads the
glyph from the atlas.

No differential test. The function reads a string through four `STR_*`
accessors and a font descriptor behind a visual object, and standing
all of that up would be most of a test of the mocks. Everything it
does is local arithmetic on values those accessors return, and the
shape is visible in the file.

### Pick — `glyphIdx = (ch < 32) ? 0 : ch - 32` *(binary)*

The frozen copy branched to a shared tail with the index already
computed. Written as a conditional expression the generated code is
the same and the intent — control characters all use glyph zero — is
on one line.

## gfxripple.c

Expanding rings on water. 329 to 235.

Three functions came out of the one: `GFX_Ripple_Generate` emits a
ring every `f3` seconds, `GFX_Ripple_Update` ages them and compacts
the list, and the render builds a quad per ring. The frozen copy had
all three in the render with the ring cursor shared between them.

`MATH_GetInverse` replaces an inline reciprocal in the render, and the
`f1 == 0.0` guard in front of it is the frozen copy's own.

### Pick — `!(ring->f1 >= 0.0)` in the update *(binary)*

As elsewhere: the positive form is what the frozen copy tests, so a
NaN life drops the ring.

## gfxdesintegrate.c

An object's triangles turned into free particles. 437 to 257.

The work is a geometry copy: a new `tGEOGRO` with its own vertices and
normals, the source's UVs shared, and one particle per triangle seeded
at that triangle's centre with an outward normal. The frozen copy had
the chunk walk, the triangle walk and the particle walk as one body
with three cursors.

Two things are preserved rather than fixed, and both have a comment:

**The chunk element source.** Every chunk copies from
`source->triangleChunks->triangleChunkElems[chunk]` — the *first*
chunk's element array, indexed by the chunk number — rather than from
its own chunk's array. So chunk 1 copies starting one element into
chunk 0's triangles, chunk 2 two elements in, and so on. For a
single-chunk object, which is the common case, it is correct. For a
multi-chunk object it is almost certainly a bug, and it is almost
certainly the author's: the index is a plain `chunk` where the
surrounding code uses `source->triangleChunks[chunk]` everywhere else,
and nothing in the frozen copy suggests a mis-decoded cursor. Left as
it stands, because changing it changes what the effect draws and that
is the user's call.

**The particle reset.** The positions are reset to the null vector
only when flag 2 is clear, which reads backwards until you notice that
flag 2 means "the positions were seeded". The comment says so.

No differential test: standing it up needs `GEO_pst_Create`,
`GEO_AllocElementContent`, `PAG_pst_Create`, `OBJ_p_GetGro`,
`OBJ_p_GetGrm` and `MDF_pst_GetByType`, and most of a test would be
the mocks rather than the effect.

## gfxflare.c

Lens flares: the depth test, the single quad and the nine sub-flares
along the line through the screen centre. 481 to 383.

### The four parallel runs

This is the find of the batch. The frozen copy walks the nine
sub-flares with a cursor of the whole effect's type and increments it
once per iteration, which reads `sizeof(tGfxFlareData)` further each
time — so eight of the nine sub-flares come out of whatever follows
the effect in memory. The differential test shows it plainly: materials
of `0xBF800000`, `0x56EE3E3B` and `32766`.

What the fields say is that the nine sub-flares are four runs of nine
consecutive fields, read in step:

```c
subSize     = &flare->i29;                        /* i29 .. i37        */
subAlong    = (const float*)&flare->basic.color2; /* color2, color1,
                                                     i22 .. i28        */
subColor    = &flare->color3;                     /* color3, i48 .. i55 */
subMaterial = &flare->materialNum1;               /* materialNum1,
                                                     i39 .. i46        */
```

Nine of each, exactly, with no overlap and nothing left over — which
is the evidence. The two colour fields at the head of the `subAlong`
run hold the floats `-0.15` and `0.0`, which is why the frozen copy's
first read of them carries a `TODO FIXME ez miert?????`: they are
distances along the line, stored in fields the header names as
colours. The comment in the file records that.

The nine-sub-flare path is therefore excluded from the differential
comparison, and the test checks each sub-flare's material and colour
against those runs directly.

### The nine-sample depth window, preserved

`GFX_i_Flare_TestDepth`'s nine-sample mode walks a cursor over
`f_9Factor_296` and reads `dir[-1]` and `dir[0]`, starting with the
cursor at the head of the table — so the first sample takes its x from
the float *in front of* the array and the last entry, `0.2`, is never
read as a sample. It is read, separately, as the flare's base size.
Left as it stands, with a comment: a shift of one would change which
nine points the flare samples, and that is a visual decision.

The two modes that use it (`flags & 0x300` of `0x200` or `0x300`) are
out of the differential test for a reason of the test's own: each copy
in the binary reads whatever its own linker put in front of its own
table, so there is no shared memory to compare. Every other path is
covered.

### The mask window

The save/clear/restore of the draw mask wraps *every* render the call
makes, which is nine of them for a sub-flare chain, so it cannot be
`GFX_RenderGeom`. It is written out with a comment saying why.

## gfxspark.c

Sparks: emission in a cone, sub-stepped integration, and a stretched
strip per spark. 658 to 398, five `goto`s gone.

Three helpers came out of the body: `GFX_l_SparkStep` is the
gravity-and-damping step applied at four different time steps,
`GFX_l_SparkDrop` is the compaction, and the emission's orthonormal
pair is a named block with a comment about the parallel-axis case.

The spare vertex slot is worth a comment and has one: `head[1]` sits
between the spark's leading and trailing points, and the update stores
the strip width in its `x` and the strip colour, as raw bits, in its
`y`, which the render reads back. The test compares that slot's `y` as
an int for exactly that reason.

### The unreachable four-vertex paths

`GFX_i_Spark_Render` sets flag `0x20` on entry and then computes
`stride = 4 - ((flags & 0x20) != 0)`, so the stride is always three
and the four-vertex branches below cannot run as the file stands.
Preserved, with a comment. The test drives `GFX_Spark_Update` at both
strides directly, so the code is covered even though the render cannot
reach it.

### Pick — `for (subStep = TIM_gf_dt; subStep >= 0.033333335; subStep -= 0.016666668)` *(binary)*

The frozen copy had this as a `do`/`while` with the test at the
bottom and the first iteration peeled. The `for` runs the same number
of times for every dt the test covers, including zero, and the
peeled copy is gone.

## gfxlightning.c

A fractal lightning bolt. 709 to 473, six `goto`s gone, and
**bit-identical over 141 cases** — the only file in the directory
where nothing moved at all.

Four things were decoded rather than just renamed:

**The count snap.** `GFX_l_LightningSnapCount` is a named helper now,
with a comment: the subdivision needs one fewer than a power of two,
so the requested count is snapped to the nearer of the two candidates
around it. The frozen copy computed the bit length, both candidates
and the comparison inline, twice.

**The two spreads.** Growing and shrinking the pattern are separate
paths that look alike, and the growing one reads the samples back out
of the buffer it has just cleared — so it copies zeros. That is the
frozen copy's behaviour, it does not matter because the fractal pass
refills the pattern either way, and it has a comment rather than a
fix.

**The resume index.** `GFX_l_LightningSplit` returns where along the
pattern a `0..1` fraction falls and how far into that sample's span,
which the frozen copy computed twice in two slightly different
spellings.

**The double time step.** The render advances `timeElapsed` by
`TIM_gf_dt` twice, once at the top and once before the fade windows.
That is what the frozen copy does and the line says so.

## gfxprofiler.c

The profiler ribbon: a trail of slices, each a ring of points, with a
mesh rebuilt every frame. 591 to 513.

`prof->a1` is the only struct field this pass renamed; it is
`currentSlice`, which is what every use of it says. The frozen copy in
`tests/` carries the rename too, and says so at the top of the file —
that is the one change in any baseline that is not a verbatim copy.

Two helpers: `GFX_l_ProfilerNextSlice` is the ring advance written
three times in the frozen copy, and `GFX_l_ProfilerRandomInBox` plus
`GFX_l_ProfilerAddInBasis` are the two halves of seeding a point,
which appeared twice each.

The nine-slice fade walk has a comment: when a slice has moved far
enough from the one before it to be worth keeping, the nine slices
ahead of it each step one notch further down their fade.

### The divide by zero

`GFX_SetNumberOfP` computes `255 / num`. With `num` zero it faults,
in both copies. Not fixed — it is reached only from a script setting a
slice count of zero, and what it ought to do instead is a decision
about the effect, not about the rewrite. The test says so where it
leaves the case out.

## gfxcarte.c

The in-game map: sixteen piece textures, a palette per piece, a
cursor and a fade-in. 795 to 596, nine `goto`s gone — the most in the
directory.

Two things have comments because they are not visible from the code:

- the palette is one 32-byte bank per piece, picked by that piece's
  status bit;
- the sixteen piece textures live on the texture key chain behind the
  material, and the render takes them off it while `Destroy` puts them
  back.

The `goto render;` that a first draft of this file still had is gone;
it is an `if (carte->objectCount && ...)` guard, so the directory is
goto-free.

No differential test: almost every path goes through a texture
manager or a D3D device, and a test would exercise the mocks.

## gfxsun.c

The sun, its rays and the depth-blur chain. **1170 lines to 681**, the
largest file in the directory and the largest saving.

### The parallel sixteen-entry tables

`D3D_SetSunEffectOnOff` stepped a `tGfxSunD3DSpecificData*` through an
array of *pointers*, so it read past the first entry. The two things
it walks — `gD3dSunSurfaces` and `gSunEffectOnOffArray` — are parallel
sixteen-entry tables, and the rewrite walks both by index, with a
comment saying so. That is the same class of finding as the flare's
sub-flares: a cursor the decompiler gave the wrong type.

### Named blocks

Three stretches of arithmetic became named functions or carry a
comment:

- `GFX_l_SunMakeRay` builds one ray — a direction, a length, a
  half-width and the two colour octants its angle falls between;
- the ray luminance pass's octant weighting has a comment: each
  octant is lit by its own sample at double weight and by its two
  neighbours at single, and the centre sample halves the lot;
- the surface pick walks back from the one just queued to find the
  newest the hardware has finished with.

The one Hungarian comment in the file, on `D3D_MultipleStretchWithArray`,
is the user's own and predates this pass. It stays.

No differential test. The ray geometry and the luminance pass would
test cleanly, but everything around them is `IDirect3DSurface9`
vtable calls, and the file's value is in the two decodings above
rather than in arithmetic a test would pin down.

---

## What is still owed here

- `gfxstr.c`, `gfxdesintegrate.c`, `gfxcarte.c` and `gfxsun.c` have no
  differential test. For the last two that is a judgement — the paths
  are D3D and texture-manager plumbing — but `gfxstr` and
  `gfxdesintegrate` could be tested with a day's worth of mocks, and
  should be if either is ever touched again.
- Three preserved suspected bugs want a decision from the user:
  `gfxdesintegrate.c`'s chunk element source, `gfxflare.c`'s
  nine-sample window, and `gfxprofiler.c`'s divide by zero.
- None of this has been built under MSVC. The stub set is not the real
  toolchain.
