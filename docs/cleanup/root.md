# The root directory

The 46 sources in `irisEng.vcxproj` with no directory of their own: the
engine proper, 72,692 lines and 635 `goto`s, and the largest thing left.
This file is the per-file record as the pass works through it.

Twelve of the 46 were already source and need nothing. Of the rest,
`grid.c` was done out of order in the previous batch, because
`ai/ai_gridfunctions.c` was blocked on it. This batch adds `math.c`,
`dynamics.c`, `ai_common.c`, the six small files, and five of the large
ones: `sprites.c`, `textures.c`, `animation.c`, `events.c` and
`strings.c`. The batch after it adds `collision.c` and `world.c`, and
the one after that the seven remaining large files: `sound.c`,
`gameobject.c`, `proceduraltextures.c`, `ok3.c`,
`particlegenerator.c`, `gfx.c` and `photo.c`. The fourth and last
batch takes the final nine: `soundmodifier.c`, `materialanddraw.c`,
`snp.c`, `actions.c`, `profiler.c`, `inputoutput.c`, `ways.c`,
`tables.c` and `bigfiles.c`.

**The directory is finished.** Thirty-three of the 46 are rewritten,
twelve were already source, and `main.c` is the harness and out of
scope.

Four of those five were done in parallel, and each came back with a
differential test of its own. Those tests are as much the batch's
output as the rewrites are: 470,352 cases for `events.c`, 27,407 for
`strings.c`, 2,418 for `textures.c`, and 1,196,489 values compared for
`animation.c`, none of them differing. Every one was mutation-checked
twice -- once by the pass that wrote it, and once afterwards with a
defect chosen independently of it. Two of those second mutations found
coverage holes rather than passing quietly, which is the only reason to
run them.

| file | lines | residue | gotos | state |
| --- | --- | --- | --- | --- |
| `grid.c` | 2,064 to 1,700 | 1,290 to 0 | 23 to 0 | done |
| `math.c` | 2,132 to 2,130 | 779 to 0 | 0 | done |
| `dynamics.c` | 1,282 to 1,268 | 782 to 0 | 5 to 0 | done |
| `cameraengine.c` | 285 | | | already source |
| `common.c` | 20 | | | already source |
| `dbg.c` | 162 | | | already source |
| `groups.c` | 173 | | | already source |
| `lights.c` | 879 | | | already source |
| `modifiers.c` | 572 | | | already source |
| `sectors.c` | 26 | | | already source |
| `sideload.c` | 308 | | | already source |
| `textget.c` | 324 | | | already source |
| `time.c` | 135 | | | already source |
| `view.c` | 97 | | | already source |
| `water.c` | 2,828 | | | already source |
| `collision.c` | 14,241 to 11,303 | 9,202 to 29 | 204 to 0 | done |
| `sound.c` | 7,109 to 6,585 | 3,134 to 2 | 72 to 0 | done |
| `gameobject.c` | 4,693 to 4,081 | 2,841 to 5 | 42 to 0 | done |
| `animation.c` | 3,068 to 2,097 | 1,846 to 1 | 16 to 0 | done |
| `textures.c` | 3,326 to 2,598 | 1,783 to 1 | 23 to 0 | done |
| `strings.c` | 3,360 to 2,823 | 1,701 to 0 | 44 to 0 | done |
| `world.c` | 2,881 to 2,318 | 1,499 to 11 | 9 to 0 | done |
| `events.c` | 2,723 to 2,347 | 1,463 to 0 | 9 to 0 | done |
| `proceduraltextures.c` | 1,751 to 1,400 | 1,108 to 5 | 11 to 0 | done |
| `ok3.c` | 1,316 to 885 | 974 to 0 | 23 to 0 | done |
| `particlegenerator.c` | 1,112 to 668 | 835 to 1 | 22 to 0 | done |
| `soundmodifier.c` | 1,505 to 1,354 | 750 to 1 | 12 to 0 | done |
| `gfx.c` | 1,574 to 1,444 | 721 to 0 | 60 to 0 | done |
| `materialanddraw.c` | 1,011 to 806 | 492 to 2 | 12 to 0 | done |
| `photo.c` | 747 to 649 | 425 to 0 | 13 to 0 | done |
| `snp.c` | 921 to 665 | 378 to 7 | 12 to 0 | done |
| `actions.c` | 724 to 612 | 310 to 0 | 6 to 0 | done |
| `profiler.c` | 504 to 390 | 246 to 0 | 8 to 0 | done |
| `inputoutput.c` | 1,141 to 1,073 | 231 to 0 | 14 to 0 | done |
| `ways.c` | 392 to 296 | 225 to 0 | 1 to 0 | done |
| `sprites.c` | 341 to 336 | 211 to 0 | 0 | done |
| `tables.c` | 499 to 470 | 186 to 0 | 2 to 0 | done |
| `ai_common.c` | 1,838 | 139 to 0 | 0 | done (belongs to `ai/`) |
| `bigfiles.c` | 915 to 872 | 124 to 0 | 9 to 0 | done |
| `engine.c` | 925 | 81 to 0 | 0 | done |
| `memory.c` | 664 | 78 to 0 | 2 | done |
| `loading.c` | 615 | 22 to 0 | 1 to 0 | done |
| `main.c` | 154 | 22 | 0 | out of scope |
| `basic_array.c` | 242 | 17 to 0 | 0 | done |
| `gdi.c` | 896 | 16 to 0 | 3 | done |
| `soft.c` | 568 | 3 to 0 | 0 | done |

The three residue lines left in `animation.c`, `textures.c` and
`sprites.h` are not residue. Two are inside comments of the user's own
that quote the Hex-Rays line being corrected, kept byte for byte; the
third is a struct field in `sprites.h` genuinely called `v1`, which the
metric's lookbehind cannot protect because it is a declaration and not
a member access.

**`ai_common.c` belongs to the `ai/` pass** rather than this one even
though it sits in the root: the AI stack, the type table and the AI2C
entry points live there, and the earlier `ai/` batches added
`AI_PopStackSlot` and `AI_GetObjectFromStack` to it without touching the
rest. Its own 139 residue lines were the stack primitives themselves —
`AI_PutIntToStack(int a1)` and its siblings — which every file in `ai/`
calls and none of them defines. Done now, and `ai.h` with it.

**`main.c` is out of scope**, and it is the directory's one exception:
everything below `WinMain` is the user's own hand-written allocator test
harness, and its `v2`, `v5a` and `v6` are deliberately arbitrary names
that the `printf` strings beside them name out loud. The same decision as
`ai/ai_compiledscripts.c`.

**`collision.c` was done for a reason beyond its own size**, the same
reason `grid.c` and `math.c` came first: `ai/ai_colfunctions.c` named
four arguments from `collision.h` and had to leave a fifth as
`edgeSearchArg`, because the edge search it reaches was still
decompiler output. It is `down`, and the word `edgeSearchArg` is gone
from the tree. `collision.h` still carries 5 residue lines, all in
prototypes for functions defined in files that have not had their
pass.

---

## math.c

2,132 lines to 2,130, 113 functions, residue 0 in both the source and
the header. The vector and matrix helpers were already named; the 779
residue lines sat in 47 functions, almost all of them rotation,
quaternion, parabola and cubic-solver code.

This file came before the rest of the directory on purpose: every other
file's inlined arithmetic is checked against its bodies, so a wrong
guess about what one of its parameters means propagates.

### The differential test

`tests/test_math.c` compiles the pre-rewrite copy and the rewrite into
one binary and compares them over **85,829 cases. Every value is
bit-identical.** That is the strongest check in the tree, and it is
cheap here for one reason: `math.c` needs three allocator mocks and
nothing else, so there are no mocks to measure instead of the code.

What it drives: the four lookup tables byte for byte after both copies
initialise them; `GetSin`, `GetCos`, `GetSqrt`, `GetInverse` and
`GetInverseSqrt` over a sweep through both signs and each end's wrap;
every vector, matrix and quaternion helper over randomised inputs,
with the in-place ones each given their own copy of the input; the
cubic solver over 4,000 random coefficient triples plus its triple-root
and double-root corners, which random inputs never hit; and both
parabola projections including their degenerate paths.

It earned its place immediately. The first run reported 441 differences,
all in `MATH_VecRotate` -- because the driver handed the second copy an
axis the first had already normalised in place. That is the same
mutation now flagged in the file, and nothing else in the pass would
have surfaced it.

The other guard worth keeping is a type check, `every local's declared
type before against after, per function`. Renaming cannot change a
type, but a rewrite can: the first draft of the cubic solver promoted
four `float` locals to `double`, which changes the arithmetic and which
neither the parse nor a reading would catch.

### What reading it settled

- **The cubic solver** is Cardano's method on a monic cubic, and its
  return value is how many real roots it found: 1 for one real root or
  a triple root, 2 for a double root, 3 for three. Its common tail --
  subtract a third of the quadratic coefficient from each root -- was
  written out four times, once per early return, and is now one exit.
- **The 3D parabola projection's second argument is an output**: the
  tangent at the projected point. `dynamics.c` passes a constraint's
  tangent vector there and normalises it after the call, and
  `ai/ai_mathfunctions.c` passes a local it discards. Its two reference
  points enter the frame build symmetrically, through their difference
  and their sum, so they are `refA` and `refB` rather than anything
  more specific.
- **The five quaternion delta decoders are one scheme.** The 16-bit
  field rotates through the three components while the other two take 8
  bits each, and the `_800` variant assigns where the other four
  accumulate -- so it is the keyframe and they are the deltas.
- **`MATH_f_MaxOf3` and `MATH_f_MinOf3`** are this file's two
  `_sub_<address>` names, now named, with their 78 call sites across
  `collision.c` and two `ai/` files following. Two off that sweep.
- **The blend cache** -- four globals that `MATH_MatrixBlend` fills and
  `MATH_Slerp` reads when the caller says not to recompute -- loses its
  address suffixes and takes the file's own prefix convention.

### Preserved rather than fixed, each with a line in the file

- **The 2D parabola projection takes its x from one root and its height
  from another.** When the middle root is the closest, it writes
  `dest->x` from `roots.y` and computes `dest->y` from `roots.x`, which
  the local holding the chosen root still contains. The other two
  branches are consistent. This is also why the three nearly identical
  tails are not folded: folding them would silently fix it.
- **The 3D form computes the second endpoint's local coordinates and
  never reads them.** The parabola is built from the first endpoint
  alone.
- **The 3x3 inverse scales a float it has already stored** for its
  first row, where the other two rows use the full-precision cofactor.
- **`MATH_QuaternionBlend` inverts the caller's quaternion in place**,
  and `MATH_VecRotate` normalises the caller's axis in place.

Three locals were deleted and one folded, each checked use by use
against the original first: two write-only pointers in `MATH_VecRotate`
and an exact copy of its cosine.

---

## dynamics.c

1,282 lines to 1,268, 45 functions, five gotos to none, residue 0 in
both the source and the header.

The three `_sub_<address>` functions here are called only from
`DYN_OneCall` in the same file, so they are `static` now and named:
`DYN_l_ApplyFrictionVector`, `DYN_l_ApplySpeed`, `DYN_l_AutoOrient`.

### The gotos, which were three different shapes

- The horizontal-speed setter's two jumped around a condition that is
  just "gravity is non-zero on some axis". Written as that test, with
  one flag, the duplication goes too.
- The auto-orient's jumped from one arm of an `if`/`else` into the
  other to share two statements. Both arms write them out.
- The main loop's two were a hierarchy climb. Having walked up to an
  unprocessed parent, the table slot is revisited so the child is
  handled after it, which is one flag test rather than two labels. The
  decrement of the loop variable reads as a mistake and carries a line
  saying it is not.

### Inlines restored, and one that must not be

Three restored after reading the real body: the traction length is
`MATH_VecLen` exactly, cast and summation order included; the guarded
`memcpy` after the angular-velocity multiply is `MATH_CopyMatrix`; and
the negation after a normalise is `MATH_VecInvert`.

A fourth looks identical and is left alone, which is the point of the
rule. The normalise in `DYN_b_SpeedReachInTime` keeps its reciprocal at
`long double`, while `MATH_NormalizeVector` narrows it to `float`
before multiplying. The call would not compute the same thing, and the
user's own `// MATH_NormalizeVector` note at that site is a comment
that it *looks* like the helper, not that it is.

### What reading it settled

- **The field called `mass` holds the reciprocal of the mass**, and
  `forceVector` holds momentum, so a speed is `forceVector` times that
  field. `DYN_Dyna_Init` is where it is set, and every user agrees.
- **`(ptr & 3) != 1` is the free-table tag**, the same test the `ai/`
  pass named, here cast through `uint8_t` rather than `uintptr_t`. It
  looks at the low byte, so it still reads the right two bits.
- Two of the inliner's dead null tests are dropped, both where the
  pointer is dereferenced unconditionally anyway and one where it is
  dereferenced *before* the test. Four reads of `dyn->forces` in one
  function stay as four, because the first sits in one arm of a branch
  and folding them would leave the pointer unset on the other path.

### Preserved rather than fixed: the friction decay

Each axis decays by 2 raised to `-friction * mass * dt`. The code
rounds the integer part of that exponent into a local and **then never
applies it**: the original x87 sequence scaled by it with `FSCALE`, and
the decompilation kept only the `F2XM1` of the fractional part. So the
decay factor is wrong as soon as `friction * dt` reaches a half.

It happens at three sites -- the friction vector path, the no-friction-
vector path, and the auto-orient's inertia blend -- and **the user's own
`TODO FIXME, jó ez így?` sits on two of them**, which is what makes this
a reading rather than a guess. The commented-out `__F2XM1__` lines the
user kept at each site lack the scaling too, so the decompiler's output
lacked it and the transcription is faithful.

Left as written, with a line at each site, because restoring the scale
changes how everything with friction moves.


---

## The six small files

`soft.c`, `gdi.c`, `basic_array.c`, `memory.c`, `engine.c` and
`loading.c`: 239 residue lines between them, all at zero, with 41
header prototypes synced from their definitions. Mostly mechanical, with
three things worth keeping.

### gdi.c's rotation matrix, which read as nonsense

`MAT_ComputeRotationMatrix` has five float locals of which **two load the
same field**, and both its trig locals are called `sin`:

```c
f1 = a2->f1;
f2 = a2->f1;   /* again */
f3 = a2->f3;
f4 = a2->f2;
f5 = a2->f4;
```

It is a 2x2 rotation applied to two pairs of the decompressed matrix,
`(f1, f3)` and `(f2, f4)`. The second table lookup advances the index by
four of sixteen, which is a quarter turn, so `sin2` is the **cosine**.
Written that way it is four locals and the duplicate read folds away.

### Two gotos go and three stay

`loading.c`'s `LABEL_31` existed only to skip one store, so the store
moves above the branch chain that reaches it. The flag it sets is read
in exactly one place, in another function, so nothing observes the
earlier write -- which is what makes the move safe rather than merely
equivalent-looking.

`gdi.c` keeps its three. They jump backwards to a label the author
called `RESTART`, from three different nesting depths, and they are a
real loop restart. `memory.c` keeps its two, a shared error tail. Both
are the case the pass's step 5 allows, and neither is a `LABEL_n`.

### loading.c cannot be parse-checked, so it is checked another way

`loading.c` is one of the five files that still fail, on the `fpos_t`
arithmetic README records as an open question rather than a stub gap. Its
compiler error output is **byte-identical before and after**, which is
the check that stands in for a parse -- the same technique the `ai/`
pass used for `ai_soundfunctions.c` before that gap was closed.

### What is left behind on purpose

`gdi.h` keeps eleven residue lines, and they are not reachable from
here: they are the display interface's function-pointer table, whose
parameter names live in the implementations in `win/wingdi.c`. Naming
them from the field names would be invention, so they wait for the
`win/` pass.


---

## grid.c

The pathfinding grid: a large per-world array of capability bytes with
a small live window sliding over it, the window's tiles run-length
encoded in the big file, and A* over the window. 2,064 lines to 1,700,
and the 23 `goto`s are gone.

### Six things repeated at nearly every site

Most of the shrink is these:

- **the row stride**, `s3Size7LimitMaxX * gridElemSize`;
- **the cell address**, `capasPtr[y * stride + x]`;
- **the cell-to-world conversion**, which is the window's origin
  (`gridElemSize * s5Limit1MinX`) plus the cell plus the half that
  centres it — written out three times in the pathfinder alone;
- **the all-or-nothing tag**: add the capability to every cell of a
  list, and if one refuses, take it back off the ones that took it.
  `GRI_Tag`, `GRI_TagBox`, `GRI_TagVec` and `GRI_TagDyn` each had their
  own copy, the last of them already written out by hand;
- **the blocked-neighbour test**, which `GRI_Surround` does ten times;
- **the step test**, which is the pathfinder's walkability rule.

### GRID_ShortestPath

A* in 500 lines with 85 locals. What it is:

- the target is first pulled back to the nearer of a cell cap and a
  world-distance cap along the start-to-target direction, and then
  nudged to a free neighbour if the cell it lands in is a wall;
- `GRID_gast_Best` is the open list and `GRID_gast_Explored` the closed
  one, with the open list's last entry swapped into the hole when an
  entry is taken out;
- the cheapest-first scan was **unrolled four at a time with a
  remainder loop**, the same shape `geostr.c`'s reveal effect had;
- **the neighbour walk starts from the quadrant the object faces.** The
  object's forward vector picks an index into `ai_Scan` and the walk
  steps `(i + 2) % 16` from there, so the cells ahead are tried first
  and a tie goes to the straightest path;
- **`ai_Scan`'s repeated tail is what makes the diagonal rule work.**
  The table is 43 entries where eight neighbours need 16: entries 16 on
  repeat the first fourteen, so `ai_Scan[i + 8 .. i + 11]` reads the two
  orthogonals a diagonal cuts between, which is the no-corner-cutting
  test;
- the step cost is the remaining Manhattan distance plus two turn
  penalties, one for turning away from the object's sight (times 20) and
  one for leaving the straight line to the target (times 30);
- the path is walked back from the target twice, once to count it and
  once to write it, so the leading points can be dropped where the path
  is longer than the output holds — which keeps the end nearest the
  target.

Its seventh parameter is unused. The caller in
`ai/ai_gridfunctions.c` builds it as the cell cap with its low byte
replaced by another argument, which is now said in both places.

### The run-length codec

`GRI_Compress` was a cell walk whose run emission was reached by a
`goto` from the middle of the scan and which re-entered the scan at its
increment. It is one flat walk that emits a run at each change and once
more at the end, with the four tag forms in one function:

| form | bytes | holds |
| --- | --- | --- |
| no tag | 1 | a single cell whose value fits in six bits |
| `0x80` | 3 | a count up to 255 |
| `0x40` | 4 | a count up to 65535 |
| `count \| 0xC0` | 2 | a count up to 63, value in six bits |

**A single cell with none of the 0xC0 bits emits its value byte and no
tag at all**, which is the decoder's no-tag form — the emitter is
written to keep that, because the obvious restructuring drops the byte.

### Pick — the compressed-grid loader's dead branch *(open)*

`GRID_ul_CallbackCompressedGrid` reads the records through without
decoding them, and dispatches on the tag with

```c
if ((tag & 0xC0) == 0xFFFFFFC0)
```

`tag` is a `char`, so `tag & 0xC0` is a positive `int`, and
`0xFFFFFFC0` is `unsigned` — the comparison can never be true. It is
the decompiler's rendering of a comparison the compiled code made at
byte width. The effect is that a short-run tag's value byte is not
read and the stream desynchronises from that record on.

**`GRI_UnCompress` had the identical artefact and already carries a
fix**, with the user's own note `// for routing fix, it was char` on
the widened local. The one-line equivalent here is
`((uint8_t)tag & 0xC0) == 0xC0`.

Left as written, because it changes what the loader reads and the
engine has been running with it; flagged for a decision.

### Preserved rather than fixed, each with a line in the file

- **The closed list is bounded at 2000 where its array holds 1000.**
  `GRID_gast_Explored` is `tGridGast[1000]` in `grid.h` and the search
  stops at `closedNum >= 2000`, so a search that explores more than a
  thousand cells writes past it.
- **Both lists are written before their bound is checked**, so entry
  1000 of the 1000-entry open list is written on the step that stops
  the search.
- **A neighbour past the window's far edge abandons the whole search**,
  where one below its near edge is merely skipped — the two bounds are
  not symmetric.
- **The sight conversion at the top of the pathfinder** has its result
  overwritten by the target's on the next line and never read.
- **The two-grid step test allows a cell that is both a wall and
  taken**, which reads oddly; the same pair of conditions guards the
  target nudge.
- The `> 65535` run form would drop its run rather than emit it, which
  a tile cannot reach: a tile is `gridElemSize` squared and
  `gridElemSize` is a `char`.
- `GRI_i_ComputeDest` builds its blocked mask with `+` rather than `|`,
  which differs only if the wall bit a script set overlaps `0xC0`.
- `GRI_TagBox` takes the second of its two conversions' outputs the
  other way round, so one local carries the x and the other the y.

### Verification

`grid.c` parses with `-Werror=implicit-function-declaration`, the
function set is identical before and after, and every caller of the
renamed functions across the tree still parses. The project is at 132
of 137 first-party sources parsing, up from 120: `grid.c` itself was
already in the set, and the nine that came in with it came from filling
out the stub's DirectInput and DirectSound headers, which is what had
been keeping most of the sound path and four root files out.

Not built under MSVC. There is no differential test: standing the grid
up means standing up the big-file reader, the loader and a world, and
the test would be measuring those. The pathfinder is the one function
here that would be worth one if the window could be built by hand, and
that is worth revisiting.

---

## sprites.c

341 lines to 336, residue 211 to 0 in the source and 4 to 1 in the
header, and no `goto`s to start with. Its three `_sub_<address>`
functions are called only from this file, so they are `static` now and
named `SPG_l_CalcDivider`, `SPG_l_DivideVertexAndColor` and
`SPG_l_SubtractVertexAndColor`. That leaves twenty-two of those names
in the tree.

`SPG_TransformTrisToSprites` is the file. It takes a triangle's three
corners and three packed colours and tessellates it into a grid of
sprites. The two edge steps are the deltas from the first corner,
halved once per division level, and the level comes from whichever edge
is shorter measured against a threshold. `SPG_l_DivideVertexAndColor`
builds its scale factor as `0x3F800000` minus the level shifted into the
exponent field, which is two to the minus level with no division
anywhere.

`GET_SqrLenght` is deleted. Its body is `MATH_VecLenSquared`'s,
component order included; both its callers were in this file and
nothing else in the tree referenced it. It is the one function this
batch removes.

### Pick -- the shift-and-scale block is left written out *(open)*

The block inside `SPG_TransformTrisToSprites` is what
`SPG_l_DivideVertexAndColor` does, twice over, and reads field for field
as equivalent -- except that it shifts the alpha and blue deltas from
four saved temporaries rather than from the struct fields. Those
temporaries do hold the same values on both paths that reach the block,
so the substitution would be correct.

That is a chain of reasoning about four locals, not something a test
could show, and this project's record with such chains is the reason
the step-6 rule says to read the real body first. Left written out.

---

## textures.c

3,326 lines to 2,598, 23 `goto`s to none, 82 functions and eight new
`static` helpers with nothing lost. Residue 1,783 to 1 -- a note of the
user's own, kept verbatim -- and 38 to 0 in the header, whose 35
prototypes are regenerated from the definitions.

### What reading it settled

`TEX_l_ConvertSize` is a box filter in 8.8 fixed point, and the two
`malloc(16 * n)` blocks are per-destination-pixel tap tables of
first/firstWeight/last/lastWeight, walked as `unsigned int*` with `+= 4`
and now a named struct.

`ConvertSizeFast`'s sixteen blocks are one shape -- the source is the
temp buffer if there is one and the image otherwise -- which was settled
by normalising all sixteen to the same text rather than by reading them
one against another.

`TEX_GetColorConvertParams` returns a shift plus the destination mask
repositioned into the source's bit layout, which is why `ConvertPF`
writes `(mask & px) >> shift`.

### The differential test

`tests/test_textures.c` drives 59 frozen pre-pass bodies against the
live file in one binary: 2,418 cases, 0 differences, comparing the whole
32KB arena byte for byte on every case so a stray write anywhere is
caught, along with the allocator call sequence and the texture-memory
total.

It was then checked for teeth. Nine one-token defects were injected into
the rewrite -- a loop bound, a channel swap, a `>=` for a `>`, a
keep/drop mask, TGA byte order, a palette shuffle, a platform remap, an
inverted `continue`, a tap tail -- and all nine were caught. A tenth,
swapping the two arms of the box filter's tap-weight pick, was injected
independently afterwards and produced 54 differences.

Eight of the 25 `ConvertPF` format pairs are skipped, and the reason is
itself a finding: see the uninitialised outputs below.

### Preserved rather than fixed, each with a line in the file

Nine, of which three were measured rather than inferred.

The BMP loader steps its header pointer by
`sizeof(tTexWeirdBMPHeader)` rather than by one, so with a 54-byte
header the pixels are read 2,916 bytes in, 2,862 past the end of it.
Checked by compiling the struct standalone.

`TEX_GetColorConvertParams` leaves both outputs unwritten when the
source channel is narrower than the destination one, so `ConvertPF`
reads uninitialised locals. That is observable, and it is exactly why
eight format pairs cannot be compared.

`TEX_Hardware_Load` assigns the width from the resized height and the
height from the resized width. Two of the four places in the tree that
set that pair swap them and two do not, so there are two conventions in
play and this is not simply a typo -- which is why it is preserved
rather than corrected.

### Five prototypes named from another file

Five prototypes in `textures.h` are for functions defined in
`proceduraltextures.c`, which has not had its pass, so their
definitions still carry `a1`. They are named here from the bodies and
the call sites rather than left as residue -- `TEX_Procedural_Add`'s
fourth argument is stored to `proceduralTexPtr`, its second indexes the
interface table -- and **they should be confirmed when that file is
done.**

---

## animation.c

3,067 lines to 2,097, sixteen `goto`s to none, 47 functions plus ten
`static` helpers and nothing lost. Residue 1,846 to 1 and the header 24
to 0; the one line left is inside a comment of the user's own recording
the broken Hex-Rays output and their fix, kept byte for byte.

### The narrowing, which is the case worth having

`ANI_PlayGameObjectAnimation` compares a blend clock against a blend
length as `double`s, and divides by the length using the value narrowed
to `float`. The two differ, and moving the comparison onto the float is
the exact mistake this project has made before -- four times in one
file.

The pass found a triple that straddles the boundary: action length 2,
start time 291, a `dt` of `0x3CFE6668`. So `PlayGaoNarrowing` is a case
now, and that mistake fails the test instead of passing silently.
Confirmed by making it: two differing values and a differing engine
global, and the file passes again restored.

### What reading it settled

`ANI_BeforeRender`'s `Mtx4x4[64]` is a per-bone blend accumulator, and
which way each slot copies was hidden behind a label that two paths
enter after swapping the pointers: slot 0 copies the bone's local matrix
in, later slots blend into it and copy back out.

`(~((flags & 0xff) >> 7) & 1) != 0` is "the animation has not
finished", at three sites in two spellings.

`ANI_OneCall` used its own parameter slot as a float scratch, which
`ANI_MainCall` writes out properly and so settles.

`ANI_GetEndSpeedOfMB`'s inner loop is a backwards scan for the last
interpolation key, and its `goto` was "abandon this track".

### Two substitutions deliberately not made

`ANI_l_PosMatrix` is the flag-conditional matrix pick, and it is **not**
`OBJ_pst_GetLocalMatrix`, which returns NULL where this returns the
matrix array.

Folding a pointer into a local around `OBJ_ChangeIdentityFlags` would
have compiled, parsed and passed every test that does not exercise a
bone without a visual object: that call allocates the object as a side
effect, so the writes after it have to go through a re-read.

### The differential test

1,196,489 values compared, every one bit-identical. Both copies
allocate from one arena at a fixed address so each case compares the
whole arena, fourteen engine globals, eight profiler slots and the
ordered log of every mocked engine call with its arguments. `math.c` is
linked rather than mocked, and seven engine getters are verbatim copies
of the real bodies because the rewrite calls them where the old copy
wrote them out. 94% of the file's lines are covered.

### Preserved rather than fixed, and reported without a comment

With a line each: a path that leaves the global frame time set to its
argument, and a dead `MATH_GetScale` whose call is kept because removing
it would change the engine-call sequence.

Reported here and not commented: `ANI_Render` and `ANI_ComputeBoneMatrix`
do not test skeleton entries for the free-list tag where every other walk
in the file does, which the test found by crashing both copies
identically; `ANI_uc_GetEngineCanalByAICanal` compares a `uint8_t` table
entry against a `char` parameter, so a canal byte of `0x80` or more can
never match; and two functions copy a matrix whose three trailing words
were never written, which is what the test's skipped-pad-word counter
is.

---

## events.c

2,723 lines to 2,347, 52 functions plus eleven `static` helpers, nothing
lost. Residue 1,463 to 0 in the source and 22 to 0 in the header. No
comment was removed: every Hungarian note and TODO survives byte for
byte, and six lines were added to flag what is preserved.

### The nine gotos, and the loop that was not three loops

`EVE_RunOneTrack` appeared to have three nested `while(1)` loops. The
middle and outer both jump back to the top of the innermost one, so
there are two; and once written that way the inner loop's only job is
resolving label jumps until it lands on a key that is not one. Four of
the nine gotos are its `break`, one is an early `return`, and one is
what makes the loop-flag test an `else if`.

The `0x7C0` dispatch stays an `if`/`else` chain and is deliberately not
a `switch`, since a `break` inside a switch would leave the switch
rather than the loop.

Elsewhere: two gotos in the key loader are a fall-through count, which
is a `for` over `type & 3`; and four ten-line offset walks are one
multiplication by `sizeof(Vec3) * (type & 3)`.

### Two inlines restored and one deliberately not

The component multiplies are `MATH_VecMultScalar` and are bit-exact.

The scale-then-add pairs are **not** folded into `MATH_AddScaleVector`
even though the author's own note says that is what they are: the
inlined form rounds the product to `float` before adding and the helper
does not, which differs under x87.

### The differential test

470,352 cases, equivalent, not one float differing bitwise. Both copies
run over one scene buffer at one address with a bump allocator rewound
between them, so pointers are comparable and the whole scene is a
`memcmp`; a case passes only when the scene bytes, the mock heap's used
prefix, 24 engine globals, the return values and the full mocked-call
sequence all match. 98% of the file's lines are covered and
`EVE_RunOneTrack` is 100%.

Six mutations were injected and six caught -- and one of them was
initially missed, which located a real coverage hole that a new case now
closes. A seventh, perturbing one of the collapsed offset walks from
`& 3` to `& 7`, was injected independently afterwards and produced 245
differences.

### Preserved rather than fixed, each with a line in the file

Six, and the sharpest is the same class as `grid.c`'s dead comparison.
`EVE_w_Event_InterpolationKey_GetType` returns `uint16_t`, and five
callers hold it in an `int` and test `== -1`, which can never hold. The
sentinel is real -- the four callers that hold it in an `int16_t` test
the same thing and do fire -- so those five walk the offsets for a
`0xFFFF` type and return a pointer where they meant NULL. Every caller
and its declared type was enumerated to confirm the split.

Also: an AI-globals save/zero whose two early returns never restore; a
mirrored position that reads 12 and 24 bytes past a one-vector local; a
track reference that dereferences the value it also uses as an index; a
key length taken from a union member that is the data pointer's own low
half; and a reparse restart that does not terminate when the label
resolves to a key at or before the one that set it -- found empirically,
by the test hanging for three minutes.

### A name that is not waiting on another file

`EVE_RunOneTrack`'s third argument gates both end-of-track behaviours,
so it is `handleEnd`. Both call sites are in this file and nothing
outside declares it, so unlike the other open names this one needed no
other pass to settle.

### events_bugos_orig.c

Read and not touched. It is the same file to within renaming, with one
behavioural difference worth recording: it computes a key's frame time
in `double` where the live file uses `float`, which is the only site in
the live file that is not the double idiom.

---

## strings.c

3,359 lines to 2,823, 44 `goto`s to none, 86 functions plus seven
`static` helpers and nothing lost. Residue 1,701 to 0 in the source and
62 to 0 in the header, whose 69 prototypes are regenerated from the
definitions; 61 picked up names and the duplicate
`STR_3DStringList_Add` prototype is gone.

### The seven helpers

| helper | absorbs | sites |
| --- | --- | --- |
| `STR_l_NewFont` | grow the font table by 20 and hand back the next slot | 2 |
| `STR_l_FontDescForPage` | the font descriptor for a page of the shared string object's material | 4 |
| `STR_l_UseGaoMaterial` | copy the game object's material onto the shared string object and drop the cached font | 10 |
| `STR_l_GlyphIndex` | character code to glyph slot, clamped below 32 | 3 |
| `STR_l_LetterRange` | resolve `firstChar`/`charNum` into a begin/end letter pair | 12 |
| `STR_l_TwoKeyNames` | `STR_GetKeyName`'s shared tail, the three `goto LABEL_31` paths | 6 |
| `STR_l_CopyUpTo` | copy `src`..`end` inclusive into `dst` | 3 |

`STR_AddFont`'s inlined font search was restored to a call to
`STR_pst_GetFont`, which sits twelve lines above it in the same file and
matches the inlined walk exactly -- same base, same `texKey` test, same
`0` on miss. That is the file's only step-6 restoration, and no function
was invented.

### The two goto mazes were switches

`STR_EffectSetf`'s `a3 - 2050`, `a3 - 1151` and `v6 - 99` chains with
their shared `LABEL_42`/`LABEL_38` tails resolve to a 28-value
dispatch: `f1` for 150/250/450/550/650/1150/1350/1550/1650/1750/1850/2050,
`f3` for 151/451, `timeElapsed` for 551/651/751/1551/1751, `color1` for
552/652/1152/1351/1651/2051, `color2` for 553/1151, with 750 and 752
special and everything else nothing. `STR_EffectSeti`'s maze is the same
shape with seven values. Both are covered field by field from -4 to
2100.

### What reading it settled

`STR_l_ReadFormat_Hexa`'s decimal digit was written
`v2 = *v1 + 16 * (v2 + 0xFFFFFFD)`. `16 * 0xFFFFFFD` is `0xFFFFFFD0`,
which is -48 in 32 bits, so this is `16 * value + ch - '0'` with the
`- 48` folded into the constant -- the same shape as the `- 87` and
`- 55` branches beside it.

`STR_ParseString`'s `v9 = v23` and `v12 = geostr` are register reloads,
not assignments: `v23` is only ever assigned from `v9` and `v12` only
from `geostr`, so both are self-assignments and both are gone. Its
`goto LABEL_16`/`LABEL_37` is one emit loop, which runs once for a plain
character or a raw code and once per character of a global string; the
two single-shot paths enter at `LABEL_37` and the loop's own exit test
fires immediately because the global pointer is null.

`STR_GlobalSet` is a five-times unrolled copy whose length cap is tested
only every fifth character -- which is exactly
`for (i = 0; i < 15 && src[i]; i++)` followed by the terminator, because
15 is a multiple of 5 and the cap is never overshot. It writes at most
`str[15]` into a `char[16]`; it is **not** an overflow. Checked against
the struct.

`STR_RecomputePos` has `v7`, `v8` and `v21` all holding the same end
pointer; `STR_Align` re-assigns `v15 = v26` once per line where the two
are already equal; `STR_GetRectShort`'s `v6` is an alias for the running
minimum X, used as both the value and the "nothing found" sentinel.

### Two arguments named against their callers

Both are cases where the `ai/` pass named its local from the general
convention and the body disagrees. The four locals in
`ai/ai_gameobjecttextfunctions.c` were renamed to follow.

`STR_SetStringExt`'s third argument is `flags`, not `firstChar`: the
body tests it against `0x10000`, `0x100000`, `0x40000`, `0x20000` and
`0x80000`, and shifts it `(x >> 16) >> 8 & 0xF` for the alignment mode.
Its fifth argument is `pivot`, which is what it is assigned to.

`STR_Init3DString{,Ext,Center,ExtExt}`'s last argument is `maxLetters`,
not `flags`: the body uses it only as
`if (maxLetters >= 0 && count > maxLetters) count = maxLetters`.

### One name taken on trust

`STR_SetFrame`'s `frame` argument goes into bits 12..17 -- the position
every other function in this file uses for the page or material index.
The name comes from `ai/`, and settling it properly means reading how
`graphicsobjects/geostr.c` renders the frame field. That file has had
its pass, so it is answerable; it was not chased.

### The differential test

`tests/test_strings.c`, 27,407 cases, no behavioural differences. The
world -- the string object, its letters and string records, the global
slots, the font table, the texture list, the 3D-string geometry and the
key mapping -- is the driver's and is rebuilt from scratch before each
side runs; after each run the whole world is captured as bytes along
with return values and the ordered list of engine calls, and the two
captures are compared.

The pass's own sweep caught 25 of 46 mutations at first, because the
fixture had no effects on the string object and so the entire
`EffectSetf`/`Seti`/`Geti` dispatch -- the largest restructuring in the
file -- was returning early and was untested. After that and eight other
fixture gaps were fixed the sweep catches 39 of 46; of the seven left,
six are provably no-ops rather than blind spots, and the seventh is a
real gap: `STR_InsertString`'s `sd->number > len` boundary needs the
capacity clamp to bite and could not be reached without an unrealistic
fixture. That branch is a direct transcription and was left alone.

Three further mutations were injected independently afterwards:
`STR_l_LetterRange` given `firstChar` where it passes `first` (2,496
cases differ), an `EffectSetf` case label moved by one (6), and
`STR_l_TwoKeyNames`'s sentinel test moved from `key1` to `key2` -- which
**was not caught**. Every key-map fixture set indices by parity, and the
six call sites pair indices two apart, so the two keys were always unset
together and the two spellings agreed at every site. A fourth mapping,
keyed on `i % 3`, puts exactly one of each pair out of range; the
mutation then produces 8 differing cases, and the sweep is 27,407.

### Preserved rather than fixed, each with a line in the file

- `STR_GlobalSet` -- the bound test truncates to a byte
  (`(uint8_t)idx < 24u`) but the index does not, so an index of 256 or
  -256 passes and writes outside the `[24]` array. Same shape in
  `STR_GlobalSetChar`, `STR_sz_GlobalGet` and `STR_i_GlobalLength`;
  commented at the first site only.
- `STR_ParseString` -- a global's characters index the 256-entry table
  as signed `char`, where the sibling line one branch up casts to
  `uint8_t`, so any byte of `0x80` or more reads in front of the table.
- `STR_ParseString` -- the digit-packing `do`/`while` can only run once,
  because `(n - n % 10) % 10` is always 0, so only the lowest digit is
  packed. The arithmetic is left exactly as written.
- `STR_ChangeStringLength` -- the clamped length goes into the edited
  string's `number2` rather than the clamped one's. Mutating this moves
  60 cases, so it is live.
- `STR_Init3DStringExtExt` -- the scratch string takes the camera width
  as its height and the height as its width. **Inert**: the scratch
  object's width and height only scale the letters' `x`/`y`, and this
  path reads nothing back but `flagsAndChar`. Flagged because it reads
  as a swap, not because it changes anything.
- `STR_GetLetterFlag` -- `STR_pst_Get`'s result goes straight into
  `STR_pst_GetString`, which dereferences it. Every sibling checks.

### Reported without a comment

`STR_pst_GetFontDesc` guards `matNum`'s upper bound but not its lower,
so a negative sub-material index reads `multiMaterialPtr[-1]`. Not
reachable from the two real callers -- the page field is masked to
`0x3F` and `Init3DStringExtExt` passes 0 -- so it is uncommented; the
test steps around it and says so at that line.

`STR_ParseString`'s inner loop decrements its length without re-testing
it, so a global string would expand past the caller's buffer if the
length precomputation and the parse ever disagreed. They are computed
from the same source through `STR_l_GetStringLength`, so this is
probably unreachable and is not called a bug.

`STR_Init3DStringExtExt` gives `STR_ParseString` a `tGEOSTRLetter[256]`
and the full string length, and `STR_Init3DStringFromSTR` copies into a
`char[256]`; neither is capped. Whether any asset is that long cannot be
told from this tree.

`STR_EffectSeti` had `v6 = v5 << 16; v6 = a4;` -- the first overwritten
before any read. Dropped; the user's `TODO FIXME` comment above the loop
it refers to is kept byte for byte and reads the same without it.

### Pick -- isdigit is left without its include *(open)*

`strings.c` calls `isdigit` with no `<ctype.h>`, in the original and in
the rewrite. Adding the include would be behaviour-neutral in principle
but turns the call into a macro, which the frozen copy would not get, so
it is a change the differential test cannot speak to. Left as it was,
and recorded in `tests/implicitcheck.sh`'s baseline.

---

## world.c

2,881 lines to 2,318, nine `goto`s to none, and 53 functions to 54:
nothing lost, the two oddly-named helpers renamed, and one new `static`
helper for the plane inversion the mask pre-compute does twice. Residue
1,499 to 11 in the source and 21 to 0 in the header, 21 of whose 27
prototypes picked up names from the definitions.

**The eleven residue lines left are all inside comments of the user's
own** and are kept byte for byte: the commented-out `AddElemP inline`
block in `WOR_World_AttachGameObject` and the crash-fix note under it
that names `v25`, the `// fixed:` note on the static-LOD element read,
and the two `COERCE_FLOAT` notes recording the Hex-Rays form of the
bit-twiddling sign test.

**Seven comments were removed and they were all the same note.** The
bounding-volume mask's corner test was unrolled eight times, and each
copy carried its own
`//if (v60 == COERCE_FLOAT(LODWORD(v3N) & 0xBF800000 | 0x3F800000))`
naming that copy's local. Folding the eight copies into one loop leaves
one site, so one of the eight is kept verbatim on it and the other seven
went with the code they annotated. Nothing else was dropped: the comment
multiset is otherwise identical to `origin/claude/sideload`.

### The nine gotos, which were four shapes

- **Five `goto LABEL_64` in `WOR_pst_World_Load`** are one test written
  five times. Each is `if (world == target)`, with the merge work
  between them and `v10` reloaded from `world` before every one, so the
  whole run is `if (loaded == target) ... else { merge; }` and the label
  is the `if` arm of the final GRO-cursor pick.
- **`goto restart` in `EOT_AISortWithPriority`** is "a hole turned up
  while counting, compact the table and start the histogram again". A
  `do { ... } while (holeFound)` around the count.
- **`goto LABEL_9` in `WOR_BVMaskPreCompute`** skips to the shared
  `pool[3].z` store from the arm that picks a different value for it.
  The three arms write it out, which is what `dynamics.c`'s auto-orient
  goto became.
- **Three in `WOR_Render_All_GO`**: `LABEL_25` is the flag store two
  paths share before `continue`, `LABEL_51` is the loop's increment, and
  `LABEL_50` is the unapply tail. The tail's only live variable, `v21`,
  is assigned `dxdd` on both paths that reach it, so the two arms and
  the tail collapse into one `if`/`else if` with the tail written once.

### What reading it settled

**The bounding-volume mask is a four-point quad, not a box.**
`WOR_BVMaskPreCompute` picks the object's flattest axis, builds the four
corners of the box's cross-section through the other two, pushes them
out to world space, and makes a plane per edge from the camera position.
`is14` records whether the quad is flat in Z -- which is why the inside
test zeroes the X difference when it is clear and the Z difference when
it is set -- and `is15` is the facing of the quad, whose sign flips all
four planes.

**`WOR_BVMaskComputeInside`'s eight-times unrolled block is one loop
over the box's eight corners**, with the index carried in the running
`v13` (`+= 1` through `+= 7` at the breaks, `+= 8` at the fall-through)
and `v16 += 2` at the bottom dead because the loop can only run once.
It stops at the first corner on the same side of the quad as the camera;
only a box with all eight corners on the far side goes on to the
four-plane test.

**Four of those eight corners can never be the first match**, which
the fold made visible and which is worth recording because it says the
original's eight copies were half redundant. The loop projects each
corner onto two axes -- (x,y) when `is14` is set and (y,z) when it is
clear -- and the box's eight corners give only four distinct
projections either way: `{0,5} {1,3} {2,6} {4,7}` in the first case
and `{0,4} {1,6} {2,3} {5,7}` in the second. A corner whose projection
already appeared cannot stop the walk, so it can only ever stop at 0,
1, 2 and then 4 or 5.

This was found by mutation rather than by reading: an off-by-one in
the folded loop's exit test scored completely clean, because the
random scene never placed the first match past corner 2. `K_BVCORNER`
builds the mask and the box by hand so the 4 and the 5 are both hit --
a plane whose dot product is positive at every earlier corner and
negative at the target, with `is15` negative so the walk is looking
for the negative one -- and the off-by-one now moves 2,280 cases. The
same mutation at corner 7 is still not caught and cannot be.

**`_floatIsNegative` is `copysignf(1.0f, x)`** -- the mask keeps the sign
bit and the `| 0x3F800000` forces the exponent of 1.0 -- and every one of
its nine uses is a same-sign comparison. It is `WOR_l_SignOf` now, and
`static`, which leaves twenty-one `_sub_`-style names in the tree.

**The other `_sub_` name is a per-frame statistics reset.**
`zero_out_param1_values_statistics_sub_43EBF0` clears the `unk2` field of
rasters 3 through 14 of the display's raster array, once per
`WOR_Render`; it is `WOR_l_ResetRasterStats` now, and `static`, since
`WOR_Render` is its only caller and no header declared it.

**`EOT_AISortWithPriority` is a counting sort with cycle placement.** A
histogram of the 256 AI priorities, a five-times unrolled prefix sum over
it, and then a walk that carries one displaced element at a time to its
slot and marks each placed entry with `0x40000000`. The sentinel for
"nothing in hand" is the pointer `-1`, which is also what an emptied slot
holds. Its `v16`/`v29` pair and its `v30` are constants -- the table's
last and first element -- so the three `v16 = v29` restores are dead, and
the two `objectFlags` stores at each placement site are a dead store
followed by the real one.

**Nine inlined copies of `TAB_AddElemToTablePF` and one of
`TAB_PTable_AddElemToTable` are restored**, after reading both bodies:
eight in `EOT_SetOfEOT_Build`, one in `EOT_SetOfEOT_OneBuild`, one in
`WOR_ul_GameObjectGroupCallback`, and the resize-then-add in
`WOR_pst_World_Load`'s static-LOD walk, which keeps the cursor-index
save and restore around it because the resize moves the array. Field for
field, including the order the two words are written in.

### The differential test

`tests/test_world.c` with `tests/world_mocks.c`, run by
`tests/runworld.sh`: **16,760 cases, 324,326 engine calls and 3.3 GB of
state compared, no differences.** Both copies run over one 188KB scene
at one address with the mock allocator rewound between them, so pointers
are directly comparable; a case passes only when the scene bytes, the
used prefix of the mock heap, the per-side world globals, the profiler
rasters, the return values and the full ordered call log all match. The
real `tables.c` is linked rather than mocked, because the restored adds
above are only worth checking against the engine's own primitives.

Nineteen entry points are driven, including both mask functions, the
three `EOT_SetOfEOT` builders, the priority sort, the activation and
visibility walks, the father and group checks, the AI-reference resolve,
and `WOR_World_AttachGameObject` and `WOR_World_JustAfterLoadObject`
against fake graphics-object interfaces whose `AddRef` is logged.

**What it does not cover, and why.** `WOR_pst_World_Load`, the two
loader callbacks, `WOR_World_Init`/`Close`/`Destroy`, `WOR_Render`,
`WOR_Render_All_GO`, `WOR_SetCam` and `WOR_pst_Universe_AddWorldList`
need the big-file reader, the loader's address table and a Direct3D
display device; standing them up means measuring the mocks. Every mock
on those paths aborts rather than returning, so a rewrite that reached
one of them fails the test instead of passing on a stand-in. That leaves
the five `goto LABEL_64` in `WOR_pst_World_Load` and the three in
`WOR_Render_All_GO` verified by reading only.

#### Teeth

**26 one-token defects injected, 25 caught.** Four of the first 25 were
missed and three of those were real coverage holes, now closed:

- the flattest-axis pick's two `>=` only differ from `>` when two of the
  box's extents tie exactly, which random floats never produce. Every
  fourth box now ties a pair, and both `>=` mutations produce 412 and
  418 differing cases;
- the sub-material filter's `id == 5` arm was never entered, because no
  fixture material had that id. A third interface does, and the mutation
  now produces 1,728;
- `WOR_World_JustAfterLoadObject`'s `flags & 0x700` was driven with one
  object per seed and no flag word ever had bit `0x400` without its
  neighbours. The collision field is now driven exhaustively from the
  case index, and the mutation produces 640.

The one still not caught is `WOR_GetLastChildObj`'s `++step >= depth`,
and it is a provable no-op -- see below.

Two of the caught mutations, both in the counting sort, are caught as a
**segmentation fault** rather than as a reported difference: a corrupted
prefix sum or a lost placement flag walks the table outside its bounds.
That is a pass for the harness and a fact about the function.

### Preserved rather than fixed, each with a line in the file

- **`WOR_GetLastChildObj` can only ever return NULL.** `child` is the
  object itself or an ancestor, and neither can be NULL by the time the
  tail runs, so the `onlyChild == child` arm sets the result to NULL and
  the other arm returns NULL outright; the loop below it then calls
  `WOR_i_GetAllChildrenOf` with a NULL parent, which returns 0 at once.
  Read from the code, and the test agrees: none of its 320 calls returned
  anything else. That is also why the one uncaught mutation is a no-op --
  `step` and `depth` are dead.
- **`WOR_GetLastChildObj` passes a single pointer as a children array.**
  `WOR_i_GetAllChildrenOf` writes one entry per match with no bound, so
  an object whose parent has more than one child writes past the local.
  Not reachable from this tree, because nothing calls the function.
- **`BVMask_ga_Masks` is `[200]` and nothing bounds the index.**
  `WOR_BVMaskPreComputeAll` increments `BVMask_gi_Num` once per visible
  object with the BV-mask flag and `WOR_BVMaskPreCompute` writes
  `BVMask_ga_Masks[BVMask_gi_Num]` with no cap, so a world with more
  than 200 of them writes past the array. Read from the code; not
  measured in a running game.
- **`WOR_pst_World_Load`'s GRO-table walk dereferences a hole.** It reads
  `((tGroGeneric*)cursor->objectPtr)->intf->id` with none of the
  null-or-tagged test every other walk in the file makes, so a
  `worldGROTable` with a hole in it faults.

### Reported without a comment

- **The two cross products are narrowed differently.**
  `WOR_BVMaskComputeInside` narrows all three components of its quad
  normal to `float`; `WOR_BVMaskPreCompute` computes the same cross
  product for `is15` and leaves the Z component a `double`. Both are
  preserved exactly as written -- this is the narrowing trap, and the
  test would see a change.
- **`EOT_AISortWithPriority`'s placement loop can spin.** If the cycle
  drain breaks because its target slot is past the table's end, the
  cursor does not advance and the next iteration recomputes the same
  slot and breaks at the same place. The priorities are `uint8_t` and
  the prefix sum is over the table's own contents, so the break looks
  defensive rather than reachable; the test's 1,280 sort cases never
  reached it, and I could not construct an input that does.
- **`WOR_DelObj`'s decrement is dead, and that one has a line in the
  file.** The loop has no `break`, so the loop variable is
  `deletedObjNum` when it ends and the `i == deletedObjNum - 1` test can
  never hold. The slot is cleared and reused by the next `WOR_AddDelObj`,
  so the list works -- it just never shrinks. Removing the `- 1` is one
  of the injected mutations and moves 320 cases, so the test sees it.
- `WOR_gi_NumKilledGO` is reset by `WOR_ResetKillGO` here and filled in
  `ai/ai_gameobjectfunctions.c`, which does bound it at 200.

---

## collision.c

14,241 lines to 11,303, **204 `goto`s to none**, 159 functions plus
twelve new `static` helpers. Residue 9,202 to 29 -- all 29 inside
comments of the user's own, checked line by line -- and the header 64
to 5.

### Done in three ranges at once, and how they were merged

The file is four times the size of the largest one any single pass had
handled, so it was split at two function boundaries into thirds of
roughly equal weight: lines 18-4988, 4989-9506 and 9507-end, at about
3,050 residue lines each. Each range was rewritten in its own worktree
on its own branch.

Three branches each rewriting a disjoint region of one file merge
cleanly in principle, and that was checked on a throwaway clone before
the passes started rather than discovered afterwards. What actually
conflicted was something else: **range A renamed the file's two
`_sub_<address>` functions and, to keep its own branch linking, their
eight call sites too -- six of which sit in the other two ranges.**
Where another range had rewritten the same line, git could not align
A's rename of the old text with the other's rewrite.

The resolution is mechanical and the same every time: the rewritten
side wins the hunk, and the two renames are reapplied over the merged
file afterwards. `COL_test_flag_sub_436970` is
`COL_w_TestInstanceFlag` and
`COL_get_activation_flag_for_id__max_16_sub_4132C0` is
`COL_i_ZoneActivationGet`; neither old name survives anywhere. The
header conflicts are the ranges naming disjoint prototypes in one
block, resolved per prototype.

**What says the merge is sound rather than merely conflict-free** is
that all three ranges' tests run against the *merged* file and compare
it with one frozen baseline: 971,406 comparisons for A, 376,599 for B
and 555,902 for C, none differing.

A tool worth keeping from this: a range-compliance check that reports,
in base-file line numbers, the first and last line each branch
changed. It is what caught A's eight strays before the merge rather
than after.

### COL_ComputeJumpPoint's fourth argument, which is why this file came first

`ai/ai_colfunctions.c` had carried it as `edgeSearchArg` since the
`ai/` pass, with a comment saying `collision.c` does not name it. It
is **`down`**, settled three ways independently:

1. It is passed straight through as `COL_FindEdgeInFollowDirection`'s
   seventh parameter, which that function already calls `down`.
2. Its second use picks which end of a steep edge to walk to: the
   lower endpoint when set, the higher when clear.
3. **The game's own script names it.** Both call sites pass a variable
   called `i_rayjump_down_point`.

The third argument is the **margin kept from each end of the edge**,
not a distance to travel: the jump point is placed at least that far
along the edge from either endpoint, and an edge shorter than twice it
is halved instead.

### Three findings that change what the engine does

Preserved with one line each, because changing any of them changes
what the engine computes.

**`INT_SphereAABBox` returns TRUE for every well-formed box.** The x
and y arms cannot reject: the `c.x > max.x` arm then tests
`c.x < min.x`, which needs `min > max`, and the `else` arm tests
`c.x - max.x > radius`, which is `<= 0 > radius`. The z arm returns 1
outright whenever `c.z > max.z`. Measured over 398,580,750 cases, zero
rejections -- and confirmed separately from the code, which is the
stronger of the two. The author's own `// TODO FIXME lehet nem jo` sits
on the y arm. At `lights.c:548` the light's far-plane cull therefore
never fires, and in the OK3 acceleration walk no box is ever rejected,
so the tree costs its traversal and buys nothing.

**`COL_b_TestFaceTag` truncates its own result.** ***Withdrawn --
this was wrong, and the correction is below.***

The claim was that `BOOL` is `int8_t`, so the function's
`tags[face >> 5] & (1 << (face % 32))` loses every bit above 7 and
reports false for three-quarters of face indices. The premise does not
hold: **`common.h`'s `typedef int8_t BOOL;` sits inside a `/* */`
block**, so the live typedef is `windows.h`'s `typedef int BOOL;`,
exactly as in the Win32 SDK. `sizeof(BOOL)` is 4 in a real translation
unit of this tree, checked with a static assert in `collision.c`
itself. Nothing was ever truncated.

The "measured: 32 of 128 read back true" figure was never measured.
The fixture declared the same `BOOL` return for both copies, so it
could not have seen a difference either way; rebuilt to count, it
reports **0 of 48,000 high-index bits truncated** by the frozen copy.

The function now returns `int` rather than `BOOL`, which says what it
does without depending on which `BOOL` is in scope. That is a
clarification, not a behaviour change, and the gold binary agrees: see
[defects.md](defects.md).

**`COL_b_SingleBVIn` has two branches that cannot be taken.**
`OBJ_BV_IsAABBox` and `OBJ_BV_IsSphere` are exact complements on the
same volume, and inside `if (OBJ_BV_IsAABBox(outer))` the code tests
`OBJ_BV_IsAABBox(outer)` again -- always true -- so the
sphere-inside-box arm is dead and a sphere passed as the inner volume
is measured as a box. The mirror arm is dead the same way, and it is
the only call site of `COL_f_SqrLen`. Both tests look as though they
were meant to be on the *inner* volume.

### Pick -- tColCylinder's field names are left as they are *(open)*

`collision.h`'s `tColCylinder` calls the radius `height` and the
half-height `radius`. The evidence is not in doubt: `COL_ZDM_Cylinder`
scales `->radius` by `scale.z` and feeds it as the half-height while
scaling `->height` by `max(scaleX, scaleY)` and feeding it as the
radius, `INT_b_FullRayCylinder` uses those two arguments as the radius
squared and as the axial coordinate, and the loader reads them in that
order off disk.

The rename was made and then reverted. `collision.h` is included by
`tests/baseline_collision.c`, the frozen pre-pass copy that all three
differential tests measure against, so renaming a field stops that
file compiling -- and editing the baseline to follow a change made to
the live tree would break the one rule the baseline exists under. The
rename is two lines in the header and twelve identifier substitutions
in `collision.c`, with the field **order** untouched so the asset
layout is unaffected. Worth doing the next time the baseline is
rebuilt.

### What could not be stood up

The detection pipeline: `COL_MainCall`, `COL_Compute`, `COL_OneCall`,
`COL_PreComputeReports`, `COL_ComputeReports`, `COL_Actor_GameObject`,
`COL_ZDM_ZDR`'s mesh walk, the `REC_` set and the five loader
callbacks. The reason is stated rather than papered over: a test would
have had to build a `tWorld` with consistent object tables, a
game-object graph with matrices, hierarchy parents and bounding
volumes, `tDyn` with forces and gravity, animation channels, a
sweep-and-prune list the engine's own insertion sort agrees with,
per-object detection lists, a loaded collision mesh with its OK3 tree
and face-tag bitmap, and a camera-assigned object. Every number those
functions produce would then come out of that fixture.

What stands in for it is a **per-function ordered call-multiset
comparison**, old against new, with calls that moved into a helper
credited back to the caller, plus the same check on the loaders'
ordered stream-read and allocation sequences. No unexplained
difference.

### One blind spot left open

Changing `>` to `>=` on any of the slab rejection's six bounds scores
clean. It needs a triangle's extreme coordinate exactly equal to the
ray origin's **and** a hit at the ray's own start plane; two attempts
at constructing it -- snapping both mesh and ray to a half-unit grid,
then adding exactly axis-perpendicular rays -- did not get there.
Those cases are kept, because grid-aligned geometry and a `dir == 0`
slab arm had no coverage at all, but the comment on them says they do
not close this. It is a real blind spot, not a no-op, and it is
narrow.

---

# Batch 3: the last seven large files

`sound.c`, `gameobject.c`, `proceduraltextures.c`, `ok3.c`,
`particlegenerator.c`, `gfx.c` and `photo.c` -- 18,302 lines to 15,712,
**243 `goto`s to none**, and 10,038 residue lines to 13, every one of
the thirteen inside a comment of the user's own.

Six passes ran, never more than five at once. Each came back with a
differential test of its own, and `tests/run.sh` takes all seven
standalone runners in, so it is the single entry point again.

A note on what the residue figures do not say: `gameobject.h` and
`particlegenerator.h` each keep two `Vec3 v1;` struct fields. Renaming
a *field* in a shared header stops every frozen `tests/baseline_*.c`
that includes it from compiling, so both are deferred to the next time
the baselines are rebuilt, alongside `collision.h`'s `tColCylinder`
rename that the previous batch parked for the same reason.

---

## sound.c

7,109 lines to 6,585, **72 `goto`s to none**, residue 3,134 to 2 and
the header 154 to 0, with 110 prototypes renamed and no type moved.
203 functions before and after. The ten `_sub_<address>` helpers are
named for what they do and made static; nothing outside `sound.c`
called any of them, and all 27 occurrences of the old names are gone.

The differential test compiles `tests/sound_side.c` twice, once against
each copy, with every `sound.c` symbol renamed by `-D` so the two sides
drive their own globals over one mock device. A case passes only when
the ordered log of every device, allocator, big-file and loader call
*and* the structured state dump are byte-identical: 1,728 cases, about
27 MB of state and call log, none differing.

### The fade path was untested, and the test said it was fine

Three defects injected into `SND_FadeUpdate` -- one caught, two
boundary flips not. The first diagnosis was that the terminal fade step
was simply unreachable: `SND_Fade` sets `i6 = 256 / duration`, so
reaching step 255 needs `elapsed / duration` to land on exactly
255/256, which no sum of the sweep's dt values produces. A terminal
case was added by hand. It still did not catch them.

The real cause was one line below the test: **`MATH_InitModule()` was
never called**, so `MATH_gd_magicDoubleForRound` was zero, and the step
index -- which is taken as the low word of a double after adding that
magic constant -- was zero in every case of the whole sweep. The entire
fade dispatch had been running one arm. With the call in, all three
mutations move 96 cases each.

The same omission was found independently in the `world.c` fixture the
batch before, which is why it is now in the shared brief: **link
`math.c` and you must call `MATH_InitModule`**, or every table-driven
result collapses to the same arm on both sides and compares equal.

### Pick -- SND_i_AllocSound keeps its hardcoded stride *(compiler artefact)*

The scan strides 56 bytes rather than `sizeof(tSndSoundElem)`. The two
agree only in a 32-bit build and the harness is 64-bit, so the literal
is kept: substituting the `sizeof` would be correct on the target and
wrong under the test, and the point of the frozen copy is that both
sides see the same thing.

---

## gameobject.c

4,693 lines to 4,081, **42 `goto`s to none**, residue 2,841 to 5 and
the header 61 to 2. 89 functions before and after, an identical set.
59 of the header's 70 prototypes renamed, no type moved.

### The 24 gotos in OBJ_ChangeIdentityFlags were one dead assignment

Every one of them skipped a `v3 = a3;`. The function opens

    v3 = a3;
    if (a3 == -1) { a3 = a1->flags; v3 = a1->flags; }

and `a3` is never assigned again, so `v3 == a3` holds unconditionally
from there and all 25 later `v3 = a3;` statements are dead. What is
left is a linear run of independent `if (new & BIT) / else if (old &
BIT)` blocks. This was re-derived from the frozen copy during review
rather than taken on the pass's word, because it is the file's largest
structural claim: 26 assignments to `v3`, two distinct right-hand
sides, one assignment to `a3`, its address never taken.

The other eighteen are ordinary joins: a scan with three exits in
`OBJ_Frame_CullingAABBox`, a blend-ramp entry and its join in
`OBJ_EyeFollow`, and four shared tails.

### Inlined helpers restored

`MATH_CopyMatrix` 21 times, `MATH_SetIdentityMatrix` twice,
`MATH_GetSqrt` once, one MSVC dword-count-plus-remainder `memcpy`, and
`MATH_NormalizeEqualVector` four times in `OBJ_UpdateCullingVars`, 32
lines to 4.

That last one is **not bit-exact**: the inlined copies sum `z2+y2+x2`
and the real body sums `x2+y2+z2`. Measured cost is 14 floats differing,
worst 1e-7 relative and 5.96e-8 absolute, and **the count is pinned in
the test**, not merely reported, so a later narrowing cannot hide inside
the tolerance.

`OBJ_BankingGet` and `OBJ_HorizonGet` are deliberately **not** restored.
The user's own comment calls them `MATH_NormalizeVector inline`, but the
real `MATH_NormalizeVector` computes `1.0f / (float)sqrt(...)` and
multiplies in float while these compute `1.0 / len * x` in double.
Restoring the call would change the arithmetic; restoring
`MATH_NormalizeEqualVector` instead would need a copy first, which is
inventing source. Left inline, comment intact.

### Six things preserved rather than fixed, one line each

`OBJ_CullingObject` stores an uninitialised byte -- `OBJ_CullingAABBox`
writes `*outFlags` only on the path that returns 0, and the byte is
stored whatever happened, which is why the test cannot compare it.
`OBJ_ComputeRecursivScale` can dereference a pointer its own loop has
just set to null. `OBJ_EyeFollow` dereferences a null-safe getter's
result unchecked at three sites. Two cursors in
`OBJ_ChangeIdentityFlags` index a freshly allocated array of structs as
an array of *pointers* and dereference what they read -- every other
site in the tree indexes the array itself. Two statements in the same
function are bare member accesses with the `= 0` missing. And
`OBJ_GameObject_Duplicate` copies the local matrix twice, 96 bytes then
the first 80 again from the same source, which is inert.

### Two things the test found that reading did not

`OBJ_SetInitialAbsolutePosition` and `OBJ_SetInitialAbsoluteMatrix`
copy three words of stack garbage: `MATH_MulMatrixMatrix` fills a
matrix's 4x4 and `mtxflag` and leaves `unk1..unk3` alone, and four
sites copy all 80 bytes of the stack local it has just filled. Both
copies do it identically -- only *which* garbage differs -- so the
harness zeroes those three words with the reason written at the site.

`OBJ_ChangeIdentityFlags` faults on one flag transition: dropping
`0x200000` while `0x1000000` is still set runs the multi-part teardown,
which frees and nulls `multiPartData`, and then the `0x1000000` block
dereferences it. Present in both copies; the test steers around it
rather than papering over it.

### The collision.c bounding-volume reading, confirmed from this side

`OBJ_BV_IsAABBox(bv)` is `*(uint32_t*)&bv->BVMax.z != 0xFFFFFFFF` and
`OBJ_BV_IsSphere(bv)` is `== 0xFFFFFFFF`: same field, same object, no
side effects, both yielding 0 or 1 so `BOOL` being `int8_t` truncates
nothing. They are exact complements, which is what the previous batch's
`collision.c` pass claimed from the other end. So both of its dead-arm
findings hold.

One caveat this side adds: `OBJ_BV_SetSphereFlag` writes the bit
pattern `0xFFFFFFFF`, which as a float is a negative quiet NaN, so a box
whose max z were ever exactly that would be misread as a sphere. Every
writer of `BVMax.z` in this file stores a finite coordinate, so it
cannot occur.

### Review found two coverage holes the pass's own twelve mutations did not

The pass injected twelve defects and caught eleven, closing two fixture
holes in the process. Three further defects chosen independently during
review found two more, and both were **real holes, not no-ops**:

**`OBJ_GameObject_Duplicate`'s third-matrix copy.** Taking it from
`srcGao->matrices + 1` instead of `srcGao->matrices` scored clean. The
branch sits under two destination flags and is reached exactly once in
the whole sweep -- and in that one case `OBJ_GameObject_Create` had left
the source's matrices byte-identical to each other, so copying either
was the same. The source's slots now carry a mark each, written into
`v1` rather than `Pos`, which `Duplicate` overwrites when it is handed a
position. The defect moves one case.

**`OBJ_Frame_CullingAABBox`'s corner-scan bound.** Changing `< 8u` to
`< 7u` scored clean. The reason is worth recording because it is not
the one the `world.c` corner loop had: **the scan's stop index does not
reach the output at all.** Once a corner has been found, the measuring
loop below reads all eight regardless; what the scan decides is only
whether a match exists. So the bound is visible only for a box whose
*sole* on-screen corner is the last one, and the random boxes never
produced one -- every match they found was at corner 0, 2, 4 or 6,
because the odd corners were either behind the camera or dominated by
their even partner, which shares their x and y and has the larger
depth.

Two changes close it. The mock projection gained a depth shear, so
corners that share x and y no longer project to the same point; and a
box and camera are built by hand so that corner 7 projects to the exact
centre while every other corner fails both the inside test and the two
straddle arms. That needs each of the seven either outside the viewport
on both axes, or outside on one and on the latch's non-triggering side
of the other -- the first attempt got it wrong and stopped at corner 5.
The defect now moves 3 cases.

### A third mutation was caught, but only by luck of a seed

Removing the `lod >= 255.0` clamp in `OBJ_uc_CalcLODVis` was caught
before the two fixtures above were added and **not** afterwards. Nothing
about the clamp had changed: inserting four cases shifted every later
`seed++`, and the one random scene that happened to drive `lod` past
255 stopped doing so.

That is a fragility in its own right, so both added loops now run from a
seed base above everything else in their function, leaving the existing
stream untouched, and the clamp is driven on purpose by a large volume
seen from close up. It moves 5 cases and no longer depends on where it
sits.

The general lesson is in the brief now: **coverage that comes from a
particular seed is coverage that the next inserted case can remove**,
silently, with every test still passing.

### What could not be stood up

`OBJ_ul_GameObjectCallback` and `OBJ_ul_World_LoadGolCallback`, the two
functions gcov reports as never entered (87 of 89 are, 79.5% of lines).
A test would have had to build a `.gao` byte stream matching the
loader's field order, a working `BIG_pc_ReadFileTmp` over it, and a
`LOA_MakeFileRef` resolver with its address table -- and would then be
measuring that fixture. Their mocks **abort** rather than return, so a
rewrite that reached one fails the test instead of passing on a
stand-in.

### One blind spot left open

`OBJ_EyeFollow`'s `headZMax + 0.17453294 >= headZTarget` needs exact
equality between a double sum carrying that constant's low mantissa bits
and a float angle widened to double. It could not be constructed. The
structure of the arithmetic suggests it may be unreachable, since
`headZTarget` is a bounded `MATH_f_ZAngle` result, but that is not
proved, so it is recorded as a narrow blind spot rather than a no-op --
the same class as `collision.c`'s slab bound.

### Needs a change in another file -- not made

`get_reference_point_gao_2_from_gao_sub_40BEA0` is the file's one
`_sub_<address>` name, with four occurrences here and one in the header.
It is **not** renamed, because its call sites are in `collision.c` and
in three collision mock files. The rename to make is
**`OBJ_pst_GetHierarchyParent`**: it returns `gao->basic->mtx->refGao2`
null-safely, which is what both `collision.c` and `OBJ_EyeFollow` use it
for. Its parameter is renamed to `gao` in both definition and prototype;
the function name is untouched.

`tMessage`'s `v1` and `v2` are the header's remaining residue. Live cost
outside this file is 4 lines in 2 files, plus 40-odd files under
`ai/ai_bge/` which is not in the build -- but it is a field rename in the
tree's most-included header, so it waits for the baselines.

---

## proceduraltextures.c

1,751 lines to 1,400, **11 `goto`s to none**, residue 1,108 to 5, all
five inside comments of the user's own and kept byte for byte.

The two Bink memory callbacks are named from what they do and made
static. The five `textures.h` prototypes that were named from this file
are confirmed against their definitions and **one is corrected**: the
loader's third argument is a byte count, not a pointer.

`tests/stub/bink.h` gains the custom-I/O half of the SDK this file
names -- the `BINKIO` callback table, the open and memory callback
types, and the four `BinkOpen` flags. **That is what took the parse
sweep from 132 of 137 to 133**, and it is why a pass branched before
this one will still measure 132.

### One thing reverted after review

`TEXPRO_Photo_SetMapGao` projects four corners through what the pass had
read as a split `Vec3 projected[4]` and given an array of its own. It is
not a split array. The walk starts in `edge0` and advances one `Vec3`
per corner, so the third and fourth projections land past `edge1`, on
the view matrix that `container->cam` is pointing at while they are
taken -- and the two scratch vectors are used individually, with
meanings of their own, by the edge search above. Giving them an array
changes what the loop writes under every layout, so it is an invention,
not a reassembly.

The walk is back as written, with one `// preserved:` line saying what
it does, and the test's exclusion of the fourth point is gone with it.

---

## ok3.c

1,316 lines to 885, **23 `goto`s to none**, residue 974 to none.

`COL_OK3_Optimize`'s two interleaved compaction loops become a chunk
loop inside a box loop. `COL_OK3_SubdivideBox`'s maze -- one switch
whose seven arms jump through each other's bodies to a shared store --
becomes seven arms that each write the sub-box corner outright; the arms
turn out to split the same way, one axis per set bit of the mask, with
the box's bits assigned to the sub-box index in an order that differs
between arms, and writing them out is what shows that.
`COL_OK3_TriangleTouchBox`'s four plane probes are the four body
diagonals of the box and are now a loop over a two-row corner table.

Two sites keep a line: the box bound admits an index one past the end of
the array, and the compaction tail copies one box too many, which the
clearing of the last slot is what makes harmless.

588 cases, 20,037 values, compared on results, every byte the allocator
handed out and the ordered call sequence, none differing. Thirty-two
defects injected, thirty-one caught; the one that is not needs 65,536
kept triangles in one chunk, which overruns the scratch array first.

### What the test cannot reach, stated in its own header

`ok3.c` writes the sizes of two of its structs as the literals **8 and
32**, which are their 32-bit sizes. On a 64-bit host every copy length
computed from them is short. Both copies compute the same short length,
so the comparison holds -- but what those copies move here is not what
they move on the target. Worth taking when the tree is next built under
MSVC.

---

## gfx.c

1,574 lines to 1,444, **60 `goto`s to none** -- the densest goto count
of the batch -- and residue 721 to none, header 21 to none.

All sixty are in three functions. The twenty-one-arm create switches in
`GFX_i_Add` and `GFX_i_AddSorted` keep their shared tail as
straight-line code after the switch, and `GFX_Render`'s dispatch loses
its shared "store the result, set the dead flag" tail to one variable
set before the switch.

### GFX_SetRenderState restored to a call

`GFX_Render` opened by writing out the seven statements that
`GFX_SetRenderState` is, interleaved with the three saves it makes
before them -- and the function had no caller anywhere in the tree,
which is what an out-of-line copy looks like when every call site has
been inlined. The saves move above the call, which nothing between them
affects, and the test compares the display data byte for byte on every
case: nothing moved.

Two sites keep a line, because both read as oversights:
`GFX_NeedGeomElemNbSprites` tests the triangle table's capacity and
updates the sprite table's, and `GFX_Attached`'s walk never stops early
so it always returns NULL.

The gfx test was strengthened by the `ok3.c` work rather than by its
own: freed blocks are now scrubbed, and each live element's own fields
are recorded rather than read back out of dead memory. 2,482 cases, none
differing.

---

## particlegenerator.c

1,112 lines to 668, **22 `goto`s to none**, residue 835 to one inside a
comment of the user's.

The three functions are the three halves of one particle system: the
integrator, the emitter and the geometry builder. The emitter's four
generator cursors are one index, its `while(2)`/`switch` maze is two
switches and a spawn loop, and the integrator's recycle jump is a flag.
`MATH_AddScaleVector`, `MATH_VecMultScalar`, `MATH_VecAddVec`,
`MATH_RandFloat` and `GFX_SetQuadUVs` are restored to calls, and the
radial-speed tail that subModes 3 and 5 share is a static helper. The
zero-length speed scale in the radial subModes is written as the zero it
is -- they reach the multiply with the length still in hand when it is
zero, which read as if the length mattered.

1,853 cases over 1,217,208 comparisons, nothing differing beyond the
float rounding the restored helpers move, worst 1.91e-06 absolute and
2.05e-05 relative. **That count is pinned**, 19,637, for the same reason
`gameobject.c`'s is.

### Three coverage holes its own mutations found

All three matrices the generator reads were the identity, so the
positions a generator builds in the emitter shape's frame and the
velocities it builds in its own were indistinguishable; the one
generator with an object of its own never reached the spawn loop, so
which object its matrix comes from was never measured; and the hole in
the floor was placed at the world origin rather than at the object's
own. Three mutations that scored clean now move 62, 248 and 4 cases.

---

## photo.c

747 lines to 649, **13 `goto`s to none**, residue 425 to none and the
header 3 to none.

The second generation of each analyser accumulates where the first
returns early; the shared bounding-box tail and the three-byte result
set are written as the loop and the if/else they are.

34,806 cases over 969,762 comparisons, none differing. Across this file
and `particlegenerator.c` the pass injected 42 one-token defects and
caught 39; the three not caught are measured no-ops.

`photo.h` defines three globals in a header rather than declaring them,
which is left alone and noted here because it is the kind of thing that
only bites when a second translation unit includes it.

---

# Batch 4: the last nine files, and the directory is done

`soundmodifier.c`, `materialanddraw.c`, `snp.c`, `actions.c`,
`profiler.c`, `inputoutput.c`, `ways.c`, `tables.c` and `bigfiles.c`
-- 7,612 lines to 6,538, **76 `goto`s to none**, and 2,942 residue
lines to 10, every one of the ten inside a comment of the user's own.

Five passes ran. `tests/run.sh` takes in all nine standalone runners,
so it is the single entry point again, and it now covers the whole
root directory.

## The shared-checkout collision, because it cost real time

**Four of the five passes started in the shared checkout rather than a
worktree of their own, and they collided.** One of them --
`cleanup/matdraw-prof` -- stayed there, and before the others
relocated its `git add -A` swept their in-progress files into its own
commits: `ways.c` and `ways.h` landed inside a commit titled
"profiler.c: rewrite decompiler output as source", and
`soundmodifier.c`, `snp.c` and `actions.c` arrived as early drafts.
`cleanup/io-ways` was also reset back to the base commit under the
pass that owned it, which had to redo its commits.

Two passes reported this independently, which is what made it
credible rather than one agent misreading its own history.

**Nothing was lost**, and the integration branch was never touched.
The resolution: merge each pass's own branch normally, and from
`cleanup/matdraw-prof` take **only its own sixteen files** rather than
merging it, since merging it whole would have brought stale duplicates
of three other passes' work. Checked afterwards that no foreign file
moved.

The lesson for next time is one line: **give every parallel pass its
own worktree up front**, and never let one run `git add -A` in a tree
another is using.

## Two corrections made to the passes' output

Both the same shape, and worth recording because they are the kind of
thing a green test cannot catch:

- **Eight findings across `ways.c`, `snp.c` and `actions.c` were
  described in a report as "preserved" with no line in the file saying
  so.** The brief asks for exactly one, and it is the only comment the
  no-comments rule allows. Each was checked against the pre-pass code
  first -- all eight are unchanged from it -- and then given its line.
- **`soundmodifier.c`'s one preserved line overstated its defect**,
  saying an index "collapses onto 0 for every instance" where the
  divide actually maps the first 152 onto 0 and the rest onto 1. The
  report had it right and the line did not.

---

## soundmodifier.c

1,505 lines to 1,354, **12 `goto`s to none**, residue 750 to one
inside the user's own commented-out Hex-Rays line. 37 functions, an
identical set. No header changed: eleven prototypes live in `sound.h`
and one in `modifiers/gensoundmodifier.h`, both named in the `sound.c`
batch, and this pass adopted those names exactly.

672 cases, 2,674,296 bytes of state and call log, none differing.
`sound.c` is not linked -- the 28 entry points it reaches are stood up
in `tests/soundmod_side.c`, compiled twice so each side drives its own
globals, with the four whose behaviour matters following `sound.c`'s
own bodies.

### SND_SModifierFadeInitInstance sets the wrong instance's volume

It computes
`(unsigned)(inst - (tSoundInstanceElem*)SND_gst_Params.soundInstances) / sizeof(tSoundInstanceElem)`.
Pointer subtraction **already** yields an element index; dividing by
the element size again throws it away. `sizeof(tSoundInstanceElem)` is
152 in a 32-bit build and `soundInstanceNum` reaches 200, so instances
0-151 all map to 0 and 152-199 to 1 -- never the instance the function
was handed. Every sibling in the file spells the same conversion
correctly with `(char*)` arithmetic. Reached whenever a type-1 fade
modifier is instanced.

### The fade-in arm's lower clamp cannot fire

`255 - *(uint32_t*)&rounded` is an **unsigned** subtraction, so the
guard written as `>= 0` is always true and the `else step = 0` is
dead -- leaving a negative index into `SND_SModifier_FadeTable`. The
pass found this by driving it deliberately and watching the test
process die. In practice the value read is discarded one line later,
so it is a dead out-of-bounds *read* rather than a wrong volume, as
long as `i4 == 255/i3` as `FadeInit` sets it. The fade-out arm reads
the same double as `int32_t` and clamps both ends correctly, so the
two were written differently.

### Pick -- a mutation that scored clean is a no-op of the code *(binary)*

Making slot 3's interface inherit slot 0's instead of slot 1's scores
completely clean. It is not a fixture gap: slot 1 is itself a copy of
slot 0 with exactly `{Type, Init, Update, InitChainedElemVersion,
PlayApplyOnInstance}` overridden, and slot 3 re-sets all five. Every
field the two candidate sources differ in is overwritten, so the two
spellings cannot be told apart by anything.

Also recorded: `tLinkedSoundFade` is the named version of
`tSoundModifierDataFade`, same 36 bytes and same layout, measured --
renaming those six fields in `sound.h` is the right fix and waits for
the next baseline rebuild; a `.smd` record with a zero payload length
is fatal, because the loader's `else` arm passes a null pointer to an
`Init` that dereferences it on its first statement; and
`SND_InitMdFModule` hardcodes a 4-byte pointer, kept for the same
reason `SND_i_AllocSound`'s 56-byte stride is.

---

## inputoutput.c and ways.c

`inputoutput.c` 1,141 lines to 1,073, **14 `goto`s to none**, residue
231 to 0, header 27 to 0. `ways.c` 392 to 296, its one `goto` gone,
residue 225 to 0, header 6 to 0. Two deliberate `static` helpers added
to `inputoutput.c`: `INO_l_ApplyJoystickDir` and `INO_l_RunCardOp`,
the latter absorbing a `[state + 1]` dispatch that was one label
shared by sixteen `goto`s.

2,404 cases and 222 cases, none differing, both at **100% line
coverage** of the file under test.

**The card-manager table and the `+1` dispatch are exactly matched.**
The 21 dispatching states map to precisely the set of slots
`INO_SavInitModule` fills, minus the async slot and the two
module-init ones. Nothing is off by one and no dispatching state hits
a hole -- which also explains the fall-through that is preserved:
state 8 and state 11 share one body because the buffer setup is the
same, and they dispatch to a read and a write respectively.

**`INO_Keys` is never filled from the keyboard.** The DirectInput
state goes into `pcINO_keyboardDataBackup` in
`win/wininputoutput.c`, and the only writes to `INO_Keys`' contents
anywhere in the tree are a `memset` and two stores mapping codes
59..68 to 96..105 and 164 while 157 is held. So `INO_b_IsKeyPressed`
and its three siblings can only ever report those eleven codes.
Grep-verified tree-wide; it may simply be the `win/` side being
unfinished.

**`SAV_PhotoSet` reads past its array at photo 99.** `elems[photo +
1]` lands on `photoBufSize`, because `elems[100]` starts exactly at
that member's offset -- measured by compiling the struct standalone,
same at 32- and 64-bit. With a size change there the `memcpy` length
goes non-positive and converts to a huge `size_t`.

**`INO_PafSet` clamps above but not below**, so a negative intensity
reaches `s_JoystickPafIntensity` unchanged -- live, not theoretical:
adding the lower clamp moves 28 cases. And **`SAV_PhotoDecal`'s two
bounds are checked apart, never together**, so `first + count` can
reach 197 and walk well past the hundred entries.

`ways.c`: **"pound" is *poids*** -- these functions compute each
link's weight as the distance between its two waypoints.
`gpst_CurrentWorld` is `ways.h`'s own global, not `world.h`'s
`WOR_gpst_CurrentWorld`, and nothing but `WAY_ComputePoundNetworks`
touches it, so its set-and-clear is dead; renaming it would make the
pound walk clear the engine's real current world, so it was left
alone. `WAY_RemoveAllNetworks` indexes `networks[i]` and only
afterwards tests `if (allNetworks->networks)`, and the stop object in
`WAY_ComputePoundNetworksRec` returns from the whole call rather than
the loop, so the waypoint's remaining links go unweighted. Both now
carry their line.

---

## materialanddraw.c and profiler.c

`materialanddraw.c` 1,011 lines to 806, **12 `goto`s to none**,
residue 492 to 2 (both inside the user's Hungarian comments).
`profiler.c` 504 to 390, 8 `goto`s to none, residue 246 to 0.

1,280 cases over 999,504 values and 30 cases over 38,220 raster
fields, none differing. No float tolerance anywhere in either, so
there is no count to pin.

### All nine of profiler.c's implicit declarations resolved

They were undeclared Win32 calls -- `GetSysColor`, `SetBkColor`,
`SetTextColor`, `CreateSolidBrush`, `ExtTextOutA`, `DrawTextA`,
`GetDC`, `ReleaseDC` and `GetModuleHandleA`. None needed an invented
prototype; they went into `tests/stub/windows.h` exactly as the Bink
declarations did, which also took `win/wininputoutput.c` from **10 to
9**. `profiler.c` is off `implicitcheck.sh`'s baseline entirely.

### ENG_ForceStartRasters clears one of eleven user rasters

`raster += sizeof(profilerRasterStructElem)` applies a byte count to a
**typed** pointer, so it steps 48 elements -- 2,304 bytes -- past a
528-byte group, and the bound test fails on the first pass. Only
`PRO_UserProfilerRasters[0]` is zeroed; the other five user rasters,
`PRO_COBFromVisual`, the two unknown rasters and
`PRO_UserProfilerRastersEND` keep their counters. The sibling loop in
`profiler_dbg_init` steps by one element and the bound only makes
sense for a one-element stride, so the intended step is `++raster`.
**Preserved**, and a one-token change if it should be fixed.

Also: `PRO_RecordTimeDecRef` and `PRO_PerfCounterEndBlock` are the
same function written twice, identical once the alias locals are
folded, with six engine files calling the first and four sites the
second. `MAT_SetRenderState` and `MAT_DrawIndexedSprites` do not guard
the multi-material count where `MAT_DrawIndexedTriangle` does, so a
type-4 material with a count of zero indexes `[-1]` at two of the
three sites. Eight inlined profiler start/end blocks were restored to
calls after checking each against the real bodies including the
`int16_t` wrap -- the symbol table confirms it, since the frozen copy
references `TIM_GetTicksAsInt32` directly and the rewrite does not.

One harness detail worth keeping: `tGEOMAT.idxOrPtr` is an `int`
holding a `tMultiTextureElemStruct*`, which does not round-trip on
x86-64, so the texture-element pool is `mmap`ed with `MAP_32BIT`.

---

## snp.c and actions.c

`snp.c` 921 lines to 665, **12 `goto`s to none**, residue 378 to 7
(all inside the user's own commented-out fragments), three deliberate
`static` helpers added. `actions.c` 724 to 612, 6 `goto`s to none,
residue 310 to 0. **There is no `actions.h`** -- its prototypes live
in `gameobject.h`, already with real parameter names, so nothing
needed propagating.

416 cases over 167,318 comparisons and 864 over 138,204, none
differing. Real `math.c` and `basic_array.c` are linked in the actions
test, so the transition binary search is the engine's own.

**`INT_SnP_l_PairBitIndex(0, 0, 1024)` is -1**, which indexes
`flagsArray[-1]` -- the pointer sitting in front of the array. Every
caller guards against equal indices except `INT_SnP_ComputeInversion`,
which is reached with them whenever an object's own start and end
endpoints invert. Not driven, because doing so corrupts the table, but
reachable.

Both overlap functions return outright when the **first** object's
collision structures are missing, so the second object's list is never
touched -- a real asymmetry, not cosmetic: swapping the arguments
moves 17 cases.

The insertion sort carried **two copies of the same comparison**; the
pass re-derived that `prevKey` always holds the previous element's key
at that point and dropped the duplicate. Its sink loop narrows a
threshold to `float` while the loop-entry test uses the `double`, and
the fixture that distinguishes the two now exists.

In `actions.c`, three mutations scored clean and all three are
**provable no-ops**, each measured rather than argued -- the sharpest
being that the blend's "track has finished" test can essentially never
fire: over all 65,536 `uint16_t` durations, `(double)d * 0.016666668`
is exactly representable as a `float` only for `d == 0`.

### A harness bug worth knowing about

The pass found that its own mock `Reset` cleared the arena only up to
the **previous** case's high-water mark, so an earlier larger case's
bytes survived for one side and not the other. It produced two
spurious failures. Both harnesses now clear a global high-water mark
-- and it is worth checking whether any other `tests/*_mocks.c` has
the same shape.

---

## tables.c and bigfiles.c

`tables.c` 499 lines to 470, both `goto`s gone, residue 186 to 0,
header 15 to 0. `bigfiles.c` 915 to 872, **9 `goto`s to none**,
residue 124 to 0, header 1 to 0. No helper added to either.

3,207 cases and 473 cases, none differing.

### bigfiles.c was stood up for testing without touching the file

`BIG_fseek` is left **byte-identical**. `tests/bigfiles_compat.h` is
forced in ahead of both copies with `-include`: it redefines `fpos_t`
as the 64-bit integer it is on the target, after the real `<stdio.h>`
has made its own typedef, redirects `fgetpos`/`fsetpos` with it and
the file calls to an in-memory bin, and supplies `_access`, `_chmod`
and `_S_IWRITE`. So the seek arithmetic under test is the file's own,
run over the target's type -- confirmed by perturbing exactly that
arithmetic, which moves 7 cases.

Not covered, and the test header says so: LZO (every synthetic block
is stored uncompressed and `lzo1x_decompress` aborts, so a change
reaching it fails the run), `PreLoadMoveBlocks`' move arm, and the
`LOA_`/`BAS_` stand-ins.

### The fpos_t question, answered

`BIG_fseek` is the **only** site: `fpos_t posToSet`, `posToSet +=
(unsigned int)offset`, and `posToSet = (unsigned int)offset`. No other
function in the file touches an `fpos_t`; `loading.c:379` has the
sibling shape. Three options, none taken, for whoever decides: an
`fgetpos`/`fsetpos` pair doing the arithmetic in an explicit
`int64_t`; a 64-bit integer type with `_fseeki64`/`_ftelli64` on the
target; or a typedef switched per toolchain. All three change the
same case -- an absolute seek with a negative offset, where
`(unsigned int)offset` currently turns `-1` into position
4294967295.

**`bigfiles.c` has a second, independent reason not to parse here**,
which was not previously recorded: `_S_IWRITE` is an MSVC macro and
the stub set has no `sys/stat.h`. So settling `fpos_t` alone would not
put the file into the parse sweep. Its two implicit declarations are
`_access` and `_chmod`.

### The compressed-buffer rotate in BIG_fread is unreachable, and its dead branch has a wild write

`memcpy(&BIG_gp_CompressedBuffer - keptSize, ...)` takes the address
of the **global pointer variable** and steps back that many pointer
widths, where the twin in `BIG_SpeedMode_fread` writes
`&BIG_gp_CompressedBuffer[-keptSize]`, before the buffer. The
enclosing `if` compares the pointer against the value assigned to it
two statements earlier. That was checked with the harness rather than
argued: replacing the branch's condition with a null dereference runs
clean over all 473 cases, while changing the `else` arm breaks the run
at once.

**`BIG_fread` also counts the decoded block twice in its loop exit** --
the test runs after the increment where the twin computes it before.
It exits a block early, and the consequence is measurable: a request
the loop satisfies short leaves `BIG_gi_ReadSeek` above
`BIG_si_DecompressedSize`, so the next call takes the difference as an
unsigned length. Both copies do it.

The two `fread` twins are near-duplicates that disagree in four
places, which is why the shared refill and init blocks were **not**
extracted after the attempt was started.

In `tables.c`: `TAB_PFtable_RemoveElem` tests the **element**, not the
object it holds, so its `--holeNum` arm is dead and removing the top
element always decrements `actElemNum` -- confirmed independently at
review, where a mutation on that arm scored clean and turned out to be
a provable no-op rather than a coverage gap. `TAB_Ptable_RemoveHoles`
and `TAB_PFtable_RemoveHoles` are the same function with different
endings, and the PF version leaves `holeNum` untouched on all three
exits, so a PF table compacted to the end of its array keeps its hole
count and is compacted again next frame. `TAB_DLlist_Remove` frees
every element and leaves `firstElem`, `lastElem` and `elemNum`
pointing at freed memory.
