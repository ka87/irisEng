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

## Verifying without the real toolchain

The engine builds under MSVC against the DirectX SDK and Bink, none of
which are available where this work is done, so "it still compiles" is
not free. Two things stand in for it.

**A stub include set** (`tests/stub/`) is enough for a native `gcc
-fsyntax-only` to parse the real sources: `windows.h`, `d3d9.h` and
`dsound.h` with the types the engine actually names, and a few empty
headers. It catches what a compiler catches — a mistyped name, a lost
brace, a changed signature. It does not prove behaviour, and it is not
the real toolchain: every batch should still be built under MSVC before
it is merged.

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
