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
-fsyntax-only -Werror=implicit-function-declaration` to parse **135 of
the 137** first-party sources (136 of 138 since `win/overlay.c` was
added, which is not part of this pass -- see `docs/overlay.md`): `windows.h`, `d3d9.h`, `dsound.h` and
`dinput.h` with the types, interfaces and vtable methods the engine
actually names, and a few empty headers. The two left are `bigfiles.c`
and `loading.c`, which fail on the `fpos_t` arithmetic described below
-- an open question rather than a stub gap. The Direct3D enum and
device-vtable set the two `win/wingdi` files want, and the Bink
constants `proceduraltextures.c` wants, were the other three, and the
`win/` pass closed all of them. It catches what a compiler catches — a mistyped name, a lost
brace, a changed signature. It does not prove behaviour, and it is not
the real toolchain: every batch should still be built under MSVC before
it is merged.

**`-Werror=implicit-function-declaration` was in that command and was
not doing anything,** which is worth saying plainly because this file
claimed otherwise for several batches. On gcc 13 the `-w` beside it
disables it, whichever order the two are given in; confirmed with a
one-line file that calls an undeclared function and compiles with exit
0 under either ordering, and errors only once `-w` is dropped.

What the parse sweep measures is unaffected -- a file that does not
parse still fails it, and the 132 of 137 stands -- but the
implicit-declaration half of it had never fired, and the claim that
"the corpus has no other implicit declaration" was never tested. It has
66, across ten files.

`tests/implicitcheck.sh` runs that half on its own, without `-w`,
reading the one diagnostic out of the noise. All but one of the 66 are
in files that have not had their pass yet; the exception is
`strings.c`'s `isdigit`, which wants `<ctype.h>` and is left alone
because adding the include turns the call into a macro, which the
frozen copy in the differential test would not get. The script fails on
a count that rises and on a file that appears and is not listed, and it
was checked against both by injecting a call.

The flag still earns its place where it does fire: a first draft of
`gfx/gfxdesintegrate.c` called a `MATH_SetNullVector` that did not
exist, which is exactly the class of mistake it catches.

**Differential tests** (`tests/`) compile the old and the new copy of a
file into one binary, run both over identical state, and compare the
results and the sequence of engine calls each made. That is the only
check here that sees behaviour. `tests/test_math.c` is the one to read
first: `math.c` is pure arithmetic over its arguments and four lookup
tables, so it needs three allocator mocks and nothing else, and all
85,829 of its comparisons come out bit-identical.

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

**A tolerance that accepts them is also a tolerance that hides things,
and one of these was measured rather than assumed.** The particle
harness accepts its float differences the same way the four below do.
Injecting one real narrowing into `PAG_l_ScaleToRandomSpeed` -- so it
divides by the `double` length rather than the `float` it is narrowed
into -- left all 1,853 cases passing; the only thing that saw it was
the count, 19,637 becoming 20,216. That count is pinned now, and the
test fails on any movement. Two full suite runs give byte-identical
counts, so it is a property of the two copies rather than of the
machine.

The other tolerance-accepting tests are **not** pinned, and the reason
is worth stating because it corrects a plausible mistake. The attempt
to demonstrate the same gap in `gfx/gfxspark.c` dropped a `(float)`
cast from `elem->i5 = (float)(up.x * acrossUp + elem->i5)` and changed
nothing at all -- not the count, not the worst absolute, not the worst
relative. **That is because a cast like it is not a narrowing: the
assignment to a `float` lvalue converts anyway.** Measured over
2,000,000 triples with zero differing. So that probe says nothing
about the harness, and pinning the remaining counts would have been
done on no evidence. A narrowing only moves a value where the wider
operand is *used* before the store -- inside a division, a comparison
or another operand -- which is exactly the shape
`ANI_PlayGameObjectAnimation` and the particle helper have, and is the
shape to look for.


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

**Done.** Full record in [ai.md](ai.md).

The AI interpreter and the builtins the game's scripts call: 32 files,
of which **31 are finished**. The one left, `ai_compiledscripts.c`, is
the user's own hand-written hook file rather than decompiler output and
is deliberately out of scope; one function elsewhere,
`AI_EvalFunc_SAVEDataGet_C`, is left byte-for-byte for a reason given
below.

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
statement is exact rather than approximate. **Across every rewritten file in the
directory -- 1,660 functions over the three batches -- no pop sequence
differs.** The remaining differences are all one push replacing two in
an if/else, or the reverse.

Two files got more than that. `ai_keywords.c`'s two coercion rules were
checked against the originals exhaustively, 7 types by 13 bit patterns
and the four shift float paths separately: **9,594 cases, none
differing**. And `ai_gameobjecttextfunctions.c` was checked a third way,
mapping every engine call's arguments back to the pop ordinal they came
from.

#### The three largest files, and the order they had to be done in

`ai_colfunctions.c` (2,775 to 2,498, 188 builtins) and
`ai_gameobjectfunctions.c` (3,799 to 3,545, 225 functions) are the two
big builtin tables, and both are now residue-free in their sources and
their headers. `ai_gridfunctions.c` was held back a whole batch because
it is almost pure pass-through into `grid.c` and nothing said what
`GRI_Tag(grid, pos, a3, a4)`'s last two arguments meant; naming them
from the calling side would have been invention, so `grid.c` was done
first and the file followed in an afternoon.

Two things found in those files are worth the space:

- **`COL_ListCreate`'s last argument does double duty.** Negative values
  are sentinels selecting which pair of shapes to test; anything else is
  the second object's zone number for a zone-against-zone test. That is
  why `COLZDEZDEListGet` is the only one of the eight pairing builtins
  whose mode comes from the script.
- **A one-bit `flags2` change was written as three statements** at six
  sites — build the high half, truncate the field, OR the half back —
  which is a plain `|=` or `&= ~`. It is only equivalent because the flag
  has no bits in the low half the truncation keeps, and that was checked
  exhaustively rather than argued from the shape. The counterexample is
  in the same file: the two builtins that do use a low-half mask were
  already written plainly.

#### Two things not done, and why

- `ai_compiledscripts.c` is the user's own hand-written hook file, not
  decompiler output. Left alone.
- `AI_EvalFunc_SAVEDataGet_C` in `ai_inofunctions.c` is left
  byte-for-byte, and that one is worth reading about: it returns
  `v3[a2]` with `a2` in 100..115 and `v3` declared `int[87]`, and its
  third loop writes `v10[15]` where `v10` is `int[11]`. Both reach
  outside their arrays, so what the function does depends on the frame
  layout and renaming its locals would change it. Everything around it
  in that file is rewritten.

### 7. The root directory

**Done.** Per-file record in [root.md](root.md).

46 files, 72,692 lines, 635 `goto`s — the engine proper, and the largest
thing left. Twelve of the 46 were already source.

**All 46 are accounted for**: thirty-three rewritten, twelve already
source, and `main.c` the harness, out of scope. `grid.c`, **2,064 lines to 1,700**
with all **23** `goto`s gone, came first because `ai/ai_gridfunctions.c`
was waiting on it to learn what the grid functions' arguments mean.
`math.c` and `dynamics.c` followed, then `ai_common.c` and six small
files, then the five large ones below.

**`math.c`** (2,132 to 2,130, 113 functions) was done by hand rather
than forked, because every other file's inlined arithmetic is checked
against its bodies. It also has the strongest verification in the tree:
a differential test over **85,829 cases, every value bit-identical**,
which caught a real in-place mutation on its first run. Its two
`_sub_<address>` helpers are named, with all 78 call sites following.

**`dynamics.c`** (1,282 to 1,268, five `goto`s to none) names three more
`_sub_<address>` functions, all file-local. Its find is the friction
decay: the integer part of the exponent is rounded off and then never
applied, because the original scaled by it with `FSCALE` and the
decompilation kept only the `F2XM1` of the fraction. It happens at three
sites and the user's own `TODO FIXME` sits on two of them.

**Five large files, four of them in parallel.** `sprites.c` (341 to
336), `textures.c` (**3,326 to 2,598**, 23 `goto`s gone),
`animation.c` (**3,067 to 2,097**, sixteen gone), `events.c` (**2,723
to 2,347**, nine gone) and `strings.c` (**3,359 to 2,823**, **44**
gone). Residue across the five goes from 7,004 lines to two, and both
of those are inside comments of the user's own quoting the Hex-Rays
line they correct.

Four of them came back with a differential test, and those tests are as
much the output as the rewrites are: 470,352 cases for `events.c`,
27,407 for `strings.c`, 2,418 for `textures.c`, and 1,196,489 values
compared for `animation.c`. None differing. Each was mutation-checked
twice --
once by the pass that wrote it, and once afterwards with a defect
chosen independently of it, which is the only check that says a
no-difference result means anything.

Two of those second mutations paid for the practice. One in `events.c`
was initially missed and located a real coverage hole, now closed by a
new case. One in `strings.c` was not caught at all: every key-map
fixture set indices by parity and the six call sites pair indices two
apart, so a sentinel test on either of two keys read the same at every
site. A fourth mapping keyed on `i % 3` separates them, and the
mutation now produces eight differing cases.

Four findings from the five are worth naming here. `textures.c`'s BMP
loader steps its header pointer by `sizeof(header)` rather than by one,
so the pixels are read 2,862 bytes past the end of it -- measured, by
compiling the struct standalone. `animation.c` compares a blend clock
as `double` and divides by the `float` it was narrowed into, and the
pass found the triple that straddles the boundary, so moving the
comparison onto the float now fails a test instead of passing silently.
`events.c` has the same shape as `grid.c`'s dead comparison: a
`uint16_t` type tested `== -1` by five callers that hold it in an
`int`, where the four that hold it in an `int16_t` do fire. And
`strings.c` guards a `[24]` array with `(uint8_t)idx < 24u` while
indexing it with the full `int`.

Two names in `ai/` were corrected from the definitions rather than the
other way round: `STR_SetStringExt`'s third argument is a bitfield and
not a first-character index, and `STR_Init3DString*`'s last is a cap on
the letter count and not a flag word. `ai_gameobjecttextfunctions.c`'s
four locals follow, and `tests/popcheck.py` still matches on all 1,660
`ai/` functions.

**`collision.c`, the largest file in the tree, and `world.c`.**
`collision.c` goes from 14,241 lines to 11,303 with **all 204 `goto`s
gone** and residue from 9,202 to 29, every one of those inside a
comment of the user's own. `world.c` goes from 2,881 to 2,318 with its
nine gone and 1,499 to 11.

`collision.c` was done in **three ranges at once**, split at function
boundaries into thirds of roughly equal weight, each in its own
worktree. That the three merge cleanly was checked on a throwaway
clone first. What actually conflicted was one range renaming two
`_sub_<address>` functions and their call sites, six of which sit in
the other ranges -- resolved by taking the rewritten side of every
hunk and reapplying the two renames over the merged file. The evidence
that the merge is sound is that all three ranges' tests run against
the *merged* file against one frozen baseline: **1.9 million
comparisons, none differing.**

**The file was done first for a reason beyond its size.**
`ai/ai_colfunctions.c` had carried `edgeSearchArg` as a placeholder
since the `ai/` pass. It is `down`: passed straight through as
`COL_FindEdgeInFollowDirection`'s parameter of that name, used to pick
the lower end of a steep edge, and -- decisively -- named by the
game's own script, which passes `i_rayjump_down_point` into it. The
word `edgeSearchArg` is gone from the tree.

Three findings in `collision.c` were reported as changing what the
engine does; one has since been withdrawn. **`INT_SphereAABBox` returns TRUE for
every well-formed box**, so the light far-plane cull at `lights.c:548`
never fires and the OK3 acceleration walk rejects nothing;
~~**`COL_b_TestFaceTag` returns `BOOL`, which is `int8_t`**~~ -- that
one was wrong and is withdrawn: `common.h`'s `int8_t` typedef is
inside a comment block, so `BOOL` is `windows.h`'s `int` and nothing
is truncated (see [defects.md](defects.md)); and **`COL_b_SingleBVIn` has
two dead branches**, each testing the outer volume where it meant the
inner.

#### Batch 3: the last seven large files

`sound.c` (**7,109 to 6,585**, 72 `goto`s gone), `gameobject.c`
(**4,693 to 4,081**, 42), `proceduraltextures.c` (1,751 to 1,400, 11),
`ok3.c` (1,316 to 885, 23), `particlegenerator.c` (1,112 to 668, 22),
`gfx.c` (1,574 to 1,444, **60** -- the densest of the batch) and
`photo.c` (747 to 649, 13). Residue across the seven goes from 10,038
lines to 13, every one of the thirteen inside a comment of the user's
own. `tests/run.sh` takes in all seven standalone runners, so it is the
single entry point again.

Two headers keep two `Vec3 v1;` fields each, `gameobject.h` and
`particlegenerator.h`, for the same reason `collision.h`'s
`tColCylinder` rename is parked: a **field** rename in a shared header
stops every frozen `tests/baseline_*.c` that includes it from
compiling. All three wait for the next baseline rebuild.

The batch's own lesson is about the tests rather than the rewrites, and
both halves of it are now in the shared brief:

- **Link `math.c` and you must call `MATH_InitModule`.** The `sound.c`
  fixture did not, so `MATH_gd_magicDoubleForRound` was zero and
  `SND_FadeUpdate`'s step index -- the low word of a double after that
  constant is added -- was zero for the entire 1,728-case sweep. The
  whole fade dispatch had been running one arm, and two injected
  boundary flips scored clean because of it. The `world.c` fixture the
  batch before had the same omission, found independently.

- **Coverage that comes from a particular seed is coverage the next
  inserted case can remove.** Removing `OBJ_uc_CalcLODVis`'s 255 clamp
  was caught before two fixtures were added to `t_culling` and not
  afterwards -- not because anything about the clamp changed, but
  because four more `CASE`s shifted every later `seed++` and the one
  random scene that drove `lod` past 255 stopped doing so. Added loops
  now take a seed base above everything else in their function, and the
  clamp is driven on purpose.

Review's independently chosen mutations found two real holes in
`gameobject.c`, neither of them no-ops: a `Duplicate` branch reached
exactly once in the whole sweep, in a case where the source's matrices
happened to be byte-identical to each other; and a corner-scan bound
whose stop index does not reach the output at all, so only a box whose
sole on-screen corner is the last one could ever see it change. Both
are closed, with a hand-built box and camera for the second.

#### Batch 4: the last nine files

`soundmodifier.c` (**1,505 to 1,354**, 12 `goto`s gone),
`inputoutput.c` (1,141 to 1,073, **14**), `materialanddraw.c` (1,011
to 806, 12), `bigfiles.c` (915 to 872, 9), `snp.c` (921 to 665, 12),
`actions.c` (724 to 612, 6), `profiler.c` (504 to 390, 8), `tables.c`
(499 to 470, 2) and `ways.c` (392 to 296, 1). Residue across the nine
goes from 2,942 lines to 10, all ten inside comments of the user's
own. `tests/run.sh` takes in all nine runners, and now covers the
whole directory.

**Four of the five passes started in the shared checkout rather than
their own worktree, and collided.** One stayed there, and its
`git add -A` swept three other passes' in-progress files into its own
commits before they relocated; one branch was also reset back to its
base under the pass that owned it. Two passes reported this
independently. Nothing was lost and the integration branch was never
touched: each pass's own branch was merged normally, and from the
contaminated one **only its own sixteen files** were taken, since
merging it whole would have brought stale duplicates. The lesson is
one line -- **give every parallel pass its own worktree up front**.

Two corrections were made to the passes' output, both of a kind a
green test cannot catch: eight findings across three files were
described in a report as "preserved" with no line in the file saying
so, and one preserved line overstated its own defect. All were checked
against the pre-pass code before being written.

`bigfiles.c` was stood up for testing **without touching the file** --
a compat header forced in with `-include` redefines `fpos_t` as the
64-bit integer it is on the target, so the seek arithmetic under test
is the file's own. That also settles the standing `fpos_t` question:
`BIG_fseek` is the only site in the file, three options are written up
in root.md, and all three change the same case, an absolute seek with
a negative offset. A **second, previously unrecorded** reason the file
will not parse here also turned up: `_S_IWRITE` is an MSVC macro the
stub set has no header for.

Also resolved: all nine of `profiler.c`'s implicit declarations, which
were undeclared Win32 calls. Adding them to the stub took
`win/wininputoutput.c` from 10 to 9 as well.

Two findings worth a decision, both preserved with a line:
`ENG_ForceStartRasters` applies a byte count to a typed pointer, so it
steps 48 elements and clears **one of eleven** user rasters; and
`BIG_fread` counts a decoded block twice in its loop-exit test, where
its twin tests the sum before the increment, leaving the read seek
above the decompressed size on a short read.


What it holds: a per-world array of capability bytes with a small live
window sliding over it, the window run-length encoded in the big file,
and A* over the window. `GRID_ShortestPath` was 500 lines and 85 locals;
it searches the neighbours of a cell starting from the quadrant the
object faces, and `ai_Scan`'s repeated tail is what makes its
no-corner-cutting rule work — the table is 43 entries where eight
neighbours need 16, so a diagonal's two orthogonals are readable at a
fixed offset from it.

Six things repeated at nearly every site became functions: the row
stride, the cell address, the cell-to-world conversion, the
all-or-nothing capability tag (four copies, one of them already written
out by hand), the blocked-neighbour test and the pathfinder's step rule.

#### One finding the user should see

`GRID_ul_CallbackCompressedGrid` dispatches on a run tag with
`(tag & 0xC0) == 0xFFFFFFC0`, which can never be true: `tag` is a
`char`, so the left side is a small positive `int`. A short-run tag's
value byte is therefore not read and the stream desynchronises from
that record on. **`GRI_UnCompress` two functions away had the identical
artefact and already carries the user's own fix**, noted
`// for routing fix, it was char`. Left as written and flagged, because
it changes what the loader reads.

Eight other oddities are preserved with a line each, the sharpest being
that the closed list is bounded at 2000 where `GRID_gast_Explored` holds
1000, and that both lists are written before their bound is checked.

### 8. `win/` — done

**Done.** Per-file record in [win.md](win.md).

Five files, 18,088 lines, 96 `goto`s — the platform layer, and the
only code in the tree that talks to Windows and to Direct3D directly.
They were left for last because they hold the densest residue in the
tree and because the stub include set had to be finished before any of
them would parse.

`win/wingdi.c` (**7,085 to 5,029**, 31 `goto`s gone),
`win/wingdi_aftereffects.c` (**5,780 to 3,180**, 14),
`win/winsound.c` (**3,395 to 2,943**, **46** — the densest single file
in the tree), `win/wininputoutput.c` (1,617 to 1,518, 5) and
`win/adpcm.c` (211 to 136). Residue across the five goes from **7,114
lines to 37**, every one of the thirty-seven inside a comment of the
user's own. Nine helpers were added in `wingdi.c` and seven more
across the rest; no function was removed anywhere.

All five came back with a differential test, and all five are called
from `tests/run.sh`: 5,152 cases and 150,984 engine calls for
`wingdi.c`, 2,326,786 for `adpcm.c`, 77,891 for `wininputoutput.c`,
1,344 cases and 2.3 MB of state for `winsound.c`, and 576 cases and
293,356 logged entries for `wingdi_aftereffects.c`. None differing,
and each mutation-checked twice.

**Finishing the stubs was half the work and paid for itself.**
`tests/stub/` grew by about 480 lines — the Direct3D interfaces and
vtables the renderer calls through, the `D3DX` font and surface entry
points, and the Win32 window, thread, file, COM, dialog and keyboard
calls. Implicit function declarations across the whole tree went from
**16 over six files to 6 over five**, and all three `win/` files that
were on that list are off it.

**Two stub faults were hiding real defects, and both generalise.**
Seven `D3D*` constants were defined as `0`, so no differential test
could tell them apart — swapping `D3DTSS_TEXCOORDINDEX` for
`D3DTSS_TEXTURETRANSFORMFLAGS` scored clean. And the stub numbered the
`WM_*` messages in declaration order, which is the reverse of Win32's,
so the range test that opens both of `wingdi.c`'s message handlers
made every other arm of both unreachable. Fixing the first did **not**
close the matdraw blind spot by itself: that fixture's own guards for
the two fields were derived from seed bits that are zero for every
seed it sweeps, and a dead guard and a collapsed constant have to be
fixed together.

**One real defect, found by reading rather than by the sweep.**
`D3D_l_DrawElementIndexedTriangles`' solid-fill arm had lost its
`vertexNum > 0` guard, and the loop bound underneath is about 2³⁰ at
`vertexNum == 0`. The fixture never generates a zero-vertex mesh —
which is the shape of defect a differential test is worst at.
Eighteen further oddities are preserved with a line each.

**Five cross-file renames went in afterwards**, four `_sub_<address>`
names and `PRO_ProfilerWindowHander`'s missing letter. Each was
blocked on a frozen `tests/baseline_*.c` using the old spelling, and
each is resolved by carrying both: where the frozen copy *defines* the
function, its runner maps the old name onto the same per-side symbol
with `-D<old>=BASE_<new>`; where it *calls* one, the mock keeps the old
name as a forwarder to the new body. All eight affected tests pass with
the same counts as before.

**Eleven `v1` struct fields in `win/wingdi.h` are still left**, because
neither half of that works for a field: a mock cannot forward one and a
`-D` cannot map one, since `v1` is also an `Mtx4x4` field and both
frozen baselines use both. Carrying both spellings there means eleven
anonymous unions in a shipped header, so it waits on the same decision
as the deferred field renames in `gameobject.h`,
`particlegenerator.h`, `collision.h` and `sound.h`.

## Every defect found, and what was done with each

[defects.md](defects.md) is the register: **4 corrected against 143
preserved**, each preserved site carrying a line at the site saying
what the code does. The rule is that a defect in what the *game* does
stays, because changing it changes the game, while a defect in what the
*decompiler* made of a type is corrected, because in each of those four
cases the frozen copy reads or writes memory the object does not own.

## Where the tree stands

With `win/` done, every directory in scope has been through the pass.
What is left of the decompiler's naming, measured with the grep in the
section above over all 137 first-party sources and their headers, is
**343 lines over 43 files**, and it breaks down into four groups, none
of which is unfinished work:

- **126 lines in `ai/ai_inofunctions.c` and its header** — all of them
  `AI_EvalFunc_SAVEDataGet_C`, left byte-for-byte on purpose because
  two of its arrays are indexed outside their bounds and renaming its
  locals would change what it does. Section 6 has the detail.
- **22 lines in `main.c`**, the harness, and **11 in
  `ai_compiledscripts.c`**, the user's own hand-written hook file.
  Neither is decompiler output.
- **81 lines in 23 headers** — struct **fields**, not locals: the `v1`
  of a vertex or a `Vec3`, the `a1` of a profiler record. Renaming a
  field in a shared header stops every frozen `tests/baseline_*.c` that
  includes it from compiling, so all of them wait on the same decision
  about regenerating the baselines.
- **103 lines in 16 sources** — every one of them inside a comment of
  the user's own: a disabled debug dump, a `TODO FIXME` quoting the
  Hex-Rays line it corrects, a note about a buffer. The code around
  them is rewritten.

`goto LABEL_n` is gone from every file in scope. 635 in the root
directory, 204 in `collision.c` alone, 96 across `win/` — none left.
