# The root directory

The 46 sources in `irisEng.vcxproj` with no directory of their own: the
engine proper, 72,692 lines and 635 `goto`s, and the largest thing left.
This file is the per-file record as the pass works through it.

Twelve of the 46 were already source and need nothing. Of the rest,
`grid.c` was done out of order in the previous batch, because
`ai/ai_gridfunctions.c` was blocked on it; `math.c` and `dynamics.c`
followed, and four more are in progress.

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
| `animation.c` | 3,068 | 1,846 | 16 | |
| `textures.c` | 3,326 | 1,783 | 23 | |
| `strings.c` | 3,360 | 1,701 | 44 | |
| `world.c` | 2,881 | 1,499 | 9 | |
| `events.c` | 2,723 | 1,463 | 9 | |
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
| `sprites.c` | 341 | 211 | 0 | |
| `tables.c` | 500 | 186 | 2 | |
| `ai_common.c` | 1,838 | 139 | 0 | partly |
| `bigfiles.c` | 916 | 124 | 9 | |
| `engine.c` | 925 | 81 | 0 | |
| `memory.c` | 664 | 78 | 2 | |
| `loading.c` | 616 | 22 | 1 | |
| `main.c` | 154 | 22 | 0 | |
| `basic_array.c` | 242 | 17 | 0 | |
| `gdi.c` | 896 | 16 | 3 | |
| `soft.c` | 568 | 3 | 0 | |

**`ai_common.c` is the one marked partly**, and it belongs to the `ai/`
pass rather than this one even though it sits in the root: the AI stack
and the type table live there, and the `ai/` batches added
`AI_PopStackSlot` and `AI_GetObjectFromStack` to it. Its own 139 residue
lines are the stack primitives themselves — `AI_PutIntToStack(int a1)`
and its siblings — which every file in `ai/` calls and none of them
defines.

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
