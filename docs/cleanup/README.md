# Turning the decompiler output into source

Most of the engine came out of a decompiler, and it still reads like it:
locals called `v1`..`v44`, parameters called `a1`..`a9`, MSVC's `__int16`,
`goto LABEL_7`, and helper functions spelled out inline at every call
site. This directory records the pass that turns that into source, file
by file, and what was decided at each fork.

## Scope

The 137 first-party files in `irisEng.vcxproj`. Not in scope:

- `jpeg-6b/` and `minilzo/`, which are third-party;
- `ai/ai_bge/`, which is not in the build and which nothing includes. It
  is a reference copy kept for comparison, and its value is being an
  unaltered record.

## What each file gets

1. **Dead locals deleted.** Declared and never read.
2. **Alias locals folded.** Assigned once, read once, carrying nothing.
3. **Everything left renamed** for what it holds, parameters included.
   Names are short, lowercase and say the role: `dest`, `src`, `mdf`,
   `gao`, `mtx`, `offset`. The already-clean files are the model;
   `cameraengine.c` is the clearest one. No Hungarian notation.
4. **Parameter names propagated** to the header and to any other
   declaration of the same function.
5. **`goto LABEL_n` replaced by real control flow** where the shape
   allows it — a backward jump is a loop, a forward jump to a tail is a
   `break` or an early `return`. Where several paths genuinely share a
   tail and no structured form is equivalent, the `goto` stays.
6. **Inlined helpers restored to calls.** The decompiler spells out
   small functions at each call site. Where the engine has the function,
   the call goes back — but only after reading the real body and
   confirming it matches the inlined arithmetic exactly.
7. **MSVC types replaced by stdint spellings.**

No comments are added. What is worth saying goes in this directory.

Two exceptions so far, both at a site where the code does something a
reader would otherwise take for a mistake: the chunk-indexing fix in
`modifiers/explodemodifier.c`, which the user asked to have commented,
and the half-stride window in `modifiers/xmenmodifier.c`, which is
preserved rather than fixed and would read as an oversight without a
line saying so.

## Verifying without the real toolchain

The engine builds under MSVC against the DirectX SDK and Bink, none of
which are available where this work is done, so "it still compiles" is
not free. Two things stand in for it.

**A stub include set** (`tests/stub/`) is enough for a native `gcc
-fsyntax-only -Werror=implicit-function-declaration` to parse the real
sources: `windows.h`, `d3d9.h` and
`dsound.h` with the types the engine actually names, and a few empty
headers. It catches what a compiler catches — a mistyped name, a lost
brace, a changed signature. It does not prove behaviour, and it is not
the real toolchain: every batch should still be built under MSVC before
it is merged.

`-Werror=implicit-function-declaration` is part of the check and earns
its place: a first draft of `gfx/gfxdesintegrate.c` called a
`MATH_SetNullVector` that did not exist, and without that flag it
parsed clean. The corpus has no other implicit declaration, so the flag
costs nothing and catches every invented call.

**Differential tests** (`tests/`) compile the old and the new copy of a
file into one binary, run both over identical state, and compare the
results and the sequence of engine calls each made. That is the only
check here that sees behaviour.

## Passes

### 1. MSVC integer types to stdint

`unsigned __int16` and friends are MSVC spellings of fixed-width types
that C has had a standard name for since C99, and much of the engine had
already moved: `Mtx4x4` was using `uint32_t` and `int32_t` while its
neighbours used `__int16`.

1,126 substitutions over 104 files:

| from | to | count |
|---|---|---|
| `__int16` | `int16_t` | 434 |
| `byte` | `uint8_t` | 584 |
| `unsigned __int16` | `uint16_t` | 97 |
| `__int8` | `int8_t` | 10 |
| `unsigned __int64` | `uint64_t` | 1 |

`byte` is MSVC's, out of `rpcndr.h`, and is `unsigned char`. Nothing in
the engine uses the word for a variable or a member, so every occurrence
was the type.

Comments and string literals were masked out before substituting, so ten
occurrences inside commented-out blocks — a disabled `sSndWavHdrStruct`
in `sound.h`, a chunk header in `geo.h` — were left as they were.

Checked three ways: no occurrence survives in live code; every one of
the 1,058 changed line pairs in the diff is reproduced exactly by
applying the substitution to the old line, so nothing else moved; and
the same 74 files parse before and after, with the other 63 failing on
the same stub gaps as before.

This pass went in on its own and ahead of the rest, because the types
sit mostly in shared headers — `ai.h`, `geo.h`, `gfx.h`, `snp.h`,
`textures.h` — which the per-directory passes would otherwise have had
to touch again and again.

### 2. MSVC-specific constructs to standard C

The engine leaned on MSVC extensions in a handful of places. Compiling
the whole tree with `gcc -std=gnu11` and no `-fms-extensions` turns the
list of them into a list of errors, and 24 of the 137 files parsed.
After this pass, **116 do, and `-fms-extensions` makes no difference to
that number** — nothing in the tree depends on an MSVC extension any
more. The other 21 fail only on gaps in `tests/stub`, which is not the
real SDK.

**Tagged anonymous aggregates**, 4 sites. `union dataByType { ... };`
declares a union *type* in standard C and nothing else, so
`texElem.normalData` does not name anything; MSVC treats it as an
anonymous member. Dropping the tag makes it a real anonymous member,
which C11 and MSVC both accept. The tags were unreferenced, so nothing
else had to change. This one was the root of most of the noise: it
alone accounted for the 777 "invalid use of incomplete typedef" and 296
"undeclared" errors cascading out of `textures.h` and `sprites.h`.

**Casts as lvalues**, 5 sites in 3 files. `++((tableElemStructPF*)p)`
and `((tGEOGRO*)x->field) = y` are an old MSVC extension. The pointer
walks are written as the arithmetic they are,
`p = (tableElemStructPF*)p + 1`, and the assignment casts the value
being stored rather than the place it goes.

**`0i64`**, 5 sites, now `0LL`.

**Pointer subtraction across types**, 1 site. `grid.c` subtracted a
`char*` from a `uint8_t*`; standard C wants compatible types, so the
left side is cast.

**Prototype-scope struct declarations**, 11 sites in 7 headers. A
prototype whose parameter names a struct the file has not yet seen
declares a *new* type scoped to that prototype, so the definition's
type does not match and the two conflict. MSVC is content; standard C
is not. Each header now forward-declares what its prototypes mention:
`sWorld`, `sGameObjectCollMain`, `tableDescStruct`,
`sWaterParamsGlobalStruct`, `sGdi_DisplayData`, `sShadowElemStruct`,
`sModifier`, `sLoadingSoundMdfStruct`.

**A missing include.** `win/adpcm.h` used `int16_t` without
`<stdint.h>` and happened to work because of include order.

#### Left alone on purpose: arithmetic on `fpos_t`

`bigfiles.c:61` and `loading.c:399` do arithmetic on an `fpos_t` and
cast one to an integer. `fpos_t` is an integer under MSVC and an opaque
type in standard C, so neither is portable — but `BIG_fseek` is using
`fgetpos`/`fsetpos` **because** they are 64-bit where `fseek` takes a
32-bit `long` on Win32, and that looks deliberate.

Collapsing the function to `fseek(fp, offset, origin)` would be
standard and, for the `int` offsets it is given, equivalent — except
for an absolute seek with a negative offset, which the current code
turns into a position above 2GB via `(unsigned int)`.

That is a change to how a file-archive reader seeks, in code that
cannot be tested here, so it is being left for the user to decide
rather than guessed at.

### 3. The rewrite itself, directory by directory

The seven things above, applied by hand, one file at a time, with the
findings for each directory in its own file here.

#### `modifiers/` — done

25 files: the 24 modifier implementations and the dispatcher. See
[modifiers.md](modifiers.md) for the per-file record.

- **4,996 lines to 4,043.** No `goto` anywhere in the directory, no
  decompiler-named local or parameter in any of the sources or their
  headers.
- **Four differential tests**, `snakemodifier`, `protexmodifier`,
  `lazymodifier` and `xmenmodifier`, covering the files whose arithmetic
  is intricate enough for a silent slip to be plausible: **192 cases,
  all equivalent.**
- **One bug fixed**, in `explodemodifier.c`: the triangle-chunk array
  was indexed by a byte offset in half of one loop, so every model with
  more than one chunk read and wrote 1KB past the array.
- **Two suspected bugs preserved and recorded** rather than fixed, in
  `protexmodifier.c` and `xmenmodifier.c`, because the file as written
  is what the engine has been compiling and which reading the author
  meant is not answerable from this tree. Both are one-token changes
  with a test ready to measure them; see modifiers.md.
- **Several oddities preserved and flagged**: `soundfxmodifier`'s guard
  that tests one field and assigns another, `otcmodifier`'s twelfth wave
  variant computing a different function from its weighted twin,
  `rotrmodifier`'s `Create` resolving a pointer and then clearing it,
  `loadingsoundmodifier`'s `Load` guarding reads and then dereferencing
  unconditionally.

#### What the tests found that nothing else would have

Worth recording, because it is the argument for writing them:

- `protexmodifier`'s water stencil is not the eight neighbours it looks
  like. An unsigned division made the row-above offset zero, so two
  samples are counted twice and the row above is never read. A first
  draft "fixed" it silently; the test caught the change at the first
  interior cell.
- `lazymodifier`'s box clamp reuses a matrix the semi-mechanical model
  computed at the top of the function. A first draft had the extracted
  helper compute it itself, which added a second `MATH_MulMatrixMatrix`
  call. No value differed — the call-sequence comparison caught it.
- `xmenmodifier`'s bitangent cross product has its operands the other
  way round from how it reads. Getting it backwards would have flipped
  every ribbon in the game.

#### The float differences, and why they are accepted

Restoring an inlined helper to a call rarely reproduces the arithmetic
bit for bit. The decompiler renders x87's 80-bit registers as `double`
and `long double` locals, and the real helpers compute in `float`, so
the last bits move. Across the four tests:

| test | floats differing | worst |
|---|---|---|
| `snakemodifier` | 208 | 2.7e-6 relative, 6.1e-5 absolute |
| `protexmodifier` | 12 | 2.4e-7 relative |
| `lazymodifier` | 254 | 6.8e-8 relative at the largest absolute |
| `xmenmodifier` | 2,802 | 2.7e-7 relative, 2.4e-7 absolute |

They are accepted because the difference is between two decompilations
of the same original, not between the original and a change: the author
called the helper, and the extra precision on one operand is an artifact
of instruction selection. `lazymodifier` has one site reading 1.3%
relative, whose absolute difference is 1.2e-7 — a matrix component near
zero after a cancellation.

Each test reports its own figures on every run, so a regression that
moves them shows up without anyone having to remember these numbers.

### 5. graphicsobjects/

**Done.** Full record in [graphicsobjects.md](graphicsobjects.md).

Ten files, **7,011 lines to 5,725**. All ten are finished: the **64**
`goto`s are gone, and no source or header in the directory carries a
decompiler-named local or parameter.

`geo.c` and `camera.c` needed nothing: they were already source.

#### One differential test, and why only one

`tests/test_streveal.c` covers the string reveal effect: **434 cases,
every value bit-identical**. It was the one function here big enough
to be worth one, and isolating it in its own function before rewriting
it made it cheap, because the frozen copy is then a drop-in. Anything
`geostr.c` refers to that the reveal does not need is stubbed to abort
if it is reached, so a new dependency fails the test rather than
passing on a stand-in.

The rest of the directory has none, and that is a decision: every
other file implements the graphics-object interface — a loader walking
a byte stream, a free function, a render entry point handing work to
the display device — so standing one up means standing up the loader,
the texture manager, the display device and the geometry buffer, and
the test would be measuring the mocks.

The risky changes got mechanical verification of the one thing that
could break instead:

- **The particle loader's read order.** A switch over twelve file
  versions. The reader and the target of every read, per version, were
  extracted from both copies and compared with the old copy's jumps
  resolved. All twelve agree.
- **The nine-patch's 72 stores.** `STR_AddFrame` was 274 lines of
  scrambled stores; every one was resolved through its temporaries
  into a vertex-index-to-expression map, and the rewritten loop was
  checked against that map. All 36 vertices and all 36 UVs agree. The
  one-quad sprite the same way.
- **The nineteen other text effects**, three ways each: the count of
  every engine call the arm makes, the writes to each of the effect's
  own fields, and the multiset of geometry indices it stores to.

#### Findings

- **`geostr.c`'s effect dispatcher was 2,227 lines with 43 gotos**,
  three of which were the shared tails — and those turn out to be the
  return value: 1 means the effect is finished and the dispatcher
  should drop it, 0 means keep it, and the one that also sets `0x400`
  hides the string for good.
- **The reveal effect was five copies of one loop.** Four blocks were
  plain 4-times unrolling with a remainder; the fifth was the goto
  maze, which is the same loop once two decompiler habits are undone —
  a `goto` printed as `break` because its target sat past the closing
  brace, and a condition inverted with its arms swapped.
- **Four narrowing bugs were avoided**, all the same shape: the
  decompiler holds a clock or a factor in a `double` and the
  comparison that ends the effect is on that double, not on the float
  it was just narrowed into. Two of them are in arms whose factor is
  squared, where narrowing first changes the value outright.
- **The nine-patch's middle column and row sample one line of texels
  each**, at the midpoint between the glyph's two edges. That is what
  lets them stretch.
- **`shadow.c` carried an unsigned-to-double conversion written out
  longhand** — the signed conversion plus 2^32 behind a sign test.
- **`MAT_Validate_Multi`'s closing XOR**, which carried two
  `TODO FIXME ??????? ez mi`, sets bit 0x40 of the result to bit 0x40
  of the running AND of the children's masks, where every other bit is
  the union.

#### Preserved rather than fixed

`geostr.c`'s per-material counter block is cleared for 24 of its 64
entries, so a material with more than 24 sub-materials can have counts
written above index 23 that are never read; `STR_Init` clears 23 of
the 24 global string slots; the two pivot spellings in `STR_Render`
differ in a cast the parameter truncates away and are left as the
original has them; the slice effect's six edges keep their non-uniform
narrowing; and `geomaterial.c`'s off-by-one `Setv` bound stands.

#### One stub gap, since closed

`shadow.c` did not parse against the committed stub: it calls two
`IDirect3DDevice9` vtable methods the stub did not declare. The `ai/`
pass added those along with the ones `ai/ai_gfxfunctions.c`'s
full-screen video path needs, so `shadow.c` parses now.
### 4. gfx/

**Done.** Full record in [gfx.md](gfx.md).

Twenty files in the build, seventeen of them rewritten: **7,131 lines
to 4,834**, and the directory as a whole **8,395 to 6,098**. All
twenty parse, the **36** `goto`s are gone, and no source or header in
the directory carries a decompiler-named local or parameter.

`gfx/gfxexplode__.c` is left alone on purpose: it is not in the
project file, it is a second copy of `gfxexplode.c`, and it names a
`vecPtr` member its own header does not declare, so it would not build
if it were added.

Five test binaries cover twelve of the seventeen, **887 cases**, with
every comparable path equivalent. Three new things in the test
infrastructure, each of which earned its place:

- **The real `math.c` is linked rather than mocked.** It compiles
  standalone under the stub set, so the trig and reciprocal tables,
  the vector helpers and the random sequence are the engine's own.
- **The 15-bit generator is supplied.** `MATH_RandFloat` scales
  `rand()` by `1/SHRT_MAX`, which assumes MSVC's 15-bit `rand`; under
  glibc's 31-bit one the spark's emission speeds came out in the tens
  of thousands and the comparison measured rounding in a domain the
  effect never enters.
- **The allocator leaks on purpose and carries a guard band.** The
  first side's state has to stay readable while the second side runs,
  so nothing is handed back between them and the call log carries the
  allocation sequence instead; and every block sits 64 bytes into a
  larger one, because a frozen copy that writes a little way before
  its block makes glibc abort the process where the real engine runs
  on. Without the guard, one binary died at its first table case and
  five groups of later cases never ran.

#### What the tests found that nothing else would have

- **`gfxtable.c` wrote its vertex colours outside the array.** The
  colour cursor ran backwards, so each cell wrote the indices `4n+1`,
  `4n`, `4n-1` and `4n-2`: the first cell stored two ints in front of
  the colour array and every later cell overwrote its neighbour's.
  Fixed, commented at the site as the user asked for the
  `explodemodifier` fix, and checked by the test against what the
  geometry wants rather than against the frozen copy.
- **`gfxflare.c`'s sub-flare chain read nine structs, not nine
  fields.** The frozen copy steps a cursor of the whole effect's type,
  so eight of the nine sub-flares come out of memory the effect does
  not own — the test saw materials of `0xBF800000` and `0x56EE3E3B`
  where the rewrite reads 4, 5, 6. The rewrite's reading is four
  parallel runs of nine consecutive fields, which is the batch's find.
- **`gfxprofiler.c`'s slice-count setter divides 255 by it**, in both
  copies, so a count of zero faults. Not this pass's defect, left
  alone, and named where the test leaves the case out.

#### Two more mis-typed cursors, decoded

Neither is visible to a test, because both read memory the effect does
not own in the frozen copy:

- `gfxsun.c`'s `D3D_SetSunEffectOnOff` stepped a struct pointer
  through an array of pointers. `gD3dSunSurfaces` and
  `gSunEffectOnOffArray` are parallel sixteen-entry tables and are now
  walked by index.
- `gfxdiaphragm.c`'s eight blades were unrolled eight times with the
  blade angle as a literal in each copy. They are a `for` over a
  `static const double[8]` of the frozen copy's own rounded `pi/4`
  steps, so the geometry is bit-identical — which the test confirms.

#### Preserved rather than fixed

Each with a line in the file saying so: `gfxdesintegrate.c`'s chunk
element source (every chunk copies from the first chunk's array
indexed by the chunk number), `gfxflare.c`'s nine-sample depth window
(the cursor reads one float in front of its table and never reads the
last entry), `gfxspark.c`'s unreachable four-vertex paths, the
lightning pattern spread that reads back the buffer it has just
cleared, the lightning render's double time step, `gfxtable.c`'s
off-by-one `Setv` bound and its `Create` writing two of three arrays
at index zero ten times over.

#### The float differences

| test | floats differing | worst absolute | worst relative |
|---|---|---|---|
| `gfxgrid` | 1,196 | 1.4e-06 | 2.6e-05 (abs 2.4e-07) |
| `gfxlightning` | 0 | — | — |
| `gfxspark` | 359 | 2.4e-07 | 1.4e-05 (abs 4.0e-08) |
| `gfxprofiler` | 353 | 2.4e-07 | 1.5e-05 (abs 1.2e-07) |
| `gfxdraw` | 240 | 1.5e-05 | 6.0e-07 (abs 1.5e-08) |

Same class as `modifiers/` and accepted for the same reason. The
lightning bolt is the one file in the directory where nothing moved at
all.

### 6. ai/

**In progress.** Full record in [ai.md](ai.md).

The AI interpreter and the builtins the game's scripts call: 32 files,
of which **28 are finished** and four remain. 120 of the project's 137
first-party sources parse now, against 117 before this directory --
`ai/ai_gfxfunctions.c` and `graphicsobjects/shadow.c` came in with the
d3d9 stub additions the full-screen video path needed.

Four shapes run through the whole directory, and naming them once is
most of the work: the wrapper that saves the current ultra object and
pops its arguments in reverse; the object pop, written out at 218 sites
and now `AI_GetObjectFromStack()` in `ai_common.c`, with
`AI_PopStackSlot()` beside it for the handful of builtins that want the
stack slot itself because a vector is stored inline there; the object
returned to a script, which went through a frame buffer of 3, 5, 6, 7,
9 or 13 ints for what the type copies as one pointer; and the free-table
tag test on a pointer's low two bits.

#### No differential tests here, and what stands in for them

Every file is a table of thin wrappers over the engine: a builtin pops
its arguments, calls one engine function and pushes the result. Standing
one up means standing up the interpreter, the game object tree, the
world and whichever subsystem is behind the builtin, and the test would
measure the mocks.

What the directory gets instead is **a mechanical comparison of the
stack traffic**, which is the one thing a wrapper can get wrong. For
every function in every rewritten file, the ordered sequence of pops and
pushes is extracted from the pre-rewrite copy and from the rewrite and
compared, with the pops that moved into a helper credited back to the
caller. A pop nested inside a push's argument still evaluates first, and
no statement anywhere in `ai/` holds more than one pop, so ordering by
statement is exact rather than approximate. **Across the eight files
of the second batch -- 614 functions -- no pop sequence differs.** The remaining
differences are all one push replacing two in an if/else, or the
reverse.

Two files got more than that. `ai_keywords.c`'s two coercion rules were
checked against the originals exhaustively, 7 types by 13 bit patterns
and the four shift float paths separately: **9,594 cases, none
differing**. And `ai_gameobjecttextfunctions.c` was checked a third way,
mapping every engine call's arguments back to the pop ordinal they came
from.

#### Three files not done, and why

- `ai_compiledscripts.c` is the user's own hand-written hook file, not
  decompiler output. Left alone.
- `ai_gridfunctions.c` is almost pure pass-through into `grid.c`, which
  is still decompiler output, so nothing says what
  `GRI_Tag(grid, pos, a3, a4)`'s last two arguments mean. Naming them
  from the calling side would be invention, so the file waits for
  `grid.c`.
- `AI_EvalFunc_SAVEDataGet_C` in `ai_inofunctions.c` is left
  byte-for-byte, and that one is worth reading about: it returns
  `v3[a2]` with `a2` in 100..115 and `v3` declared `int[87]`, and its
  third loop writes `v10[15]` where `v10` is `int[11]`. Both reach
  outside their arrays, so what the function does depends on the frame
  layout and renaming its locals would change it. Everything around it
  in that file is rewritten.
