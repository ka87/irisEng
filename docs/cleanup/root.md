# The root directory

The 46 sources in `irisEng.vcxproj` with no directory of their own: the
engine proper, 72,692 lines and 635 `goto`s, and the largest thing left.
This file is the per-file record as the pass works through it.

**One file done so far**, and out of order: `grid.c`, because
`ai/ai_gridfunctions.c` was blocked on it. Twelve of the other 45 were
already source and need nothing.

| file | lines | residue | gotos | state |
| --- | --- | --- | --- | --- |
| `grid.c` | 2,064 to 1,700 | 1,290 to 0 | 23 to 0 | done |
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
| `dynamics.c` | 1,282 | 782 | 5 | |
| `math.c` | 2,132 | 779 | 0 | |
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

Three of the rest are worth doing early for reasons beyond their own
size: **`math.c`**, because every other file's inlined arithmetic is
checked against its bodies and its parameters are still `a1`/`a2`;
**`strings.c`**, because `ai/ai_gameobjecttextfunctions.c` already had
to read it to name its own arguments; and **`collision.c`**, for the
same reason `grid.c` came first — `ai/ai_colfunctions.c` named four
arguments from `collision.h` and had to leave a fifth as
`edgeSearchArg`, because the edge search it reaches is still decompiler
output.

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
