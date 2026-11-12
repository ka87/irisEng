# Differential tests

Each test compiles the pre-refactor copy of a file and the rewritten one
into a single binary, runs both over identical state, and compares the
results and the sequence of engine calls each made.

    ./tests/run.sh              everything
    ./tests/run.sh snake        only tests whose name matches

Needs nothing but `gcc`.

## How it works

The two copies define the same function names, so `run.sh` compiles each
with `-D` renaming every public name — `BASE_` for the old copy, `NEW_`
for the live one. Both see the real engine headers, so a changed
signature is a compile error rather than a silent mismatch.

`engine_mocks.c` supplies everything the file under test calls. Two
kinds:

- **Faithful copies of `math.c`** for anything the old code spells out
  inline and the new code calls — `MATH_VecLen`, `MATH_VecInvert`,
  `MATH_NormalizeVector`, `MATH_NormalizeEqualVector`,
  `MATH_DistanceOfTwoPoints`. If these are not identical to the real
  ones the comparison means nothing, so they are copied verbatim.
- **Deterministic stand-ins** for everything else. Both copies call
  these the same way, so they only have to be reproducible, and they
  record the call.

The call log is the important half. Comparing final state alone would
miss a reordered or dropped engine call that happens not to change the
numbers, so every observable call is recorded with its arguments and the
index of the game object it acted on, and the two logs must match
element for element.

`tests/stub` holds just enough of `windows.h`, `d3d9.h` and `dsound.h`
for the real headers to parse, plus a few empty ones. **It is not the
real toolchain** — it does not prove the struct layouts MSVC produces,
and it cannot see anything the DirectX or Bink headers would. Build
under MSVC before merging.

`tests/baseline_*.c` are frozen pre-refactor copies, taken straight out
of git. They live directly in `tests/` rather than a subdirectory
because the originals sit one level below the repo root and their
`#include "../x.h"` lines have to keep resolving the same way; the
original's own directory is added with `-I` so its local header is
found.

## Floats

Equality is bitwise first. Where it fails the test reports the absolute
and relative difference and fails only past 1e-5 relative, because
restoring an inlined call can legitimately change the last bits: the
decompiler shows x87's 80-bit intermediates as `double`, and the real
function computes in `float`.

The run prints how many of the compared floats differed at all and the
worst one, so a change in that number is visible even when everything
still passes.

## Adding a test

1. `git show HEAD:path/to/file.c > tests/baseline_file.c` before
   rewriting anything.
2. Write `tests/test_file.c` with a driver that builds state, runs
   `BASE_*` and `NEW_*` over two identical copies, and compares.
3. Add the file's public names and a `run_one` line to `run.sh`.
4. Add mocks for whatever it calls that is not there yet, copying
   verbatim from the engine any function the old code inlines.

## Coverage

| file | cases | state |
|---|---|---|
| `modifiers/snakemodifier.c` | 46 | equivalent |
