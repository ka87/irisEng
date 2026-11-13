# The root directory

The 46 sources in `irisEng.vcxproj` with no directory of their own: the
engine proper, 72,692 lines and 635 `goto`s, and the largest thing left.
This file is the per-file record as the pass works through it.

Twelve of the 46 were already source and need nothing. Of the rest,
`grid.c` was done out of order in the previous batch, because
`ai/ai_gridfunctions.c` was blocked on it. This batch adds `math.c`,
`dynamics.c`, `ai_common.c`, the six small files, and five of the large
ones: `sprites.c`, `textures.c`, `animation.c`, `events.c` and
`strings.c`. Fifteen of the 46 are rewritten now and twelve were
already source, which leaves eighteen.

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
| `collision.c` | 14,241 | 9,202 | 204 | |
| `sound.c` | 7,110 | 3,134 | 72 | |
| `gameobject.c` | 4,693 | 2,841 | 42 | |
| `animation.c` | 3,068 to 2,097 | 1,846 to 1 | 16 to 0 | done |
| `textures.c` | 3,326 to 2,598 | 1,783 to 1 | 23 to 0 | done |
| `strings.c` | 3,360 to 2,823 | 1,701 to 0 | 44 to 0 | done |
| `world.c` | 2,881 | 1,499 | 9 | |
| `events.c` | 2,723 to 2,347 | 1,463 to 0 | 9 to 0 | done |
| `proceduraltextures.c` | 1,752 | 1,108 | 11 | |
| `ok3.c` | 1,317 | 974 | 23 | |
| `particlegenerator.c` | 1,112 | 835 | 22 | |
| `soundmodifier.c` | 1,506 | 750 | 12 | |
| `gfx.c` | 1,575 | 721 | 60 | |
| `materialanddraw.c` | 1,012 | 492 | 12 | |
| `photo.c` | 748 | 425 | 13 | |
| `snp.c` | 922 | 378 | 12 | |
| `actions.c` | 725 | 310 | 6 | |
| `profiler.c` | 505 | 246 | 8 | |
| `inputoutput.c` | 1,141 | 231 | 14 | |
| `ways.c` | 392 | 225 | 1 | |
| `sprites.c` | 341 to 336 | 211 to 0 | 0 | done |
| `tables.c` | 500 | 186 | 2 | |
| `ai_common.c` | 1,838 | 139 to 0 | 0 | done (belongs to `ai/`) |
| `bigfiles.c` | 916 | 124 | 9 | |
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

**`collision.c`** is the one left that is worth doing early for a
reason beyond its own size, and it is the same reason `grid.c` and
`math.c` came first: `ai/ai_colfunctions.c` named four arguments from
`collision.h` and had to leave a fifth as `edgeSearchArg`, because the
edge search it reaches is still decompiler output.

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
