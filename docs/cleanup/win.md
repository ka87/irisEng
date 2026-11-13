# `win/` — the platform layer

The five files under `win/` are the only ones that talk to Windows and
to Direct3D directly: the renderer, the after-effects chain, the sound
device, the input devices and the ADPCM decoder. Everything else in
the engine goes through them.

They were left for last on purpose. Between them they hold the densest
decompiler residue in the tree — `win/winsound.c` alone had 46 `goto`s
— and the stub include set in `tests/stub/` had to be finished before
any of them would even parse, let alone compile into a differential
test.

| file | lines | residue | `goto` | functions |
|---|---|---|---|---|
| `win/wingdi.c` | 7,085 → **5,029** | 2,948 → **20** | 31 → **0** | 128 → 137 |
| `win/wingdi_aftereffects.c` | 5,780 → **3,180** | 2,259 → **3** | 14 → **0** | 50 → 53 |
| `win/winsound.c` | 3,395 → **2,943** | 1,568 → **13** | 46 → **0** | 117 |
| `win/wininputoutput.c` | 1,617 → **1,518** | 209 → **1** | 5 → **0** | 55 → 58 |
| `win/adpcm.c` | 211 → **136** | 130 → **0** | 0 | 3 → 4 |
| `win/wingdi.h` | 351 | 39 → **11** | — | — |
| `win/wininputoutput.h` | 109 | 6 → **0** | — | — |
| `win/adpcm.h` | 12 → 14 | 3 → **0** | — | — |

**Every residue line that is left is inside a comment of the user's.**
`win/wingdi.c`'s twenty are in four block comments — two disabled
debug dumps, a `TODO FIXME` quoting the Hex-Rays line it corrects, and
a note about a buffer — and `win/winsound.c`'s thirteen are the same
kind. The eleven in `win/wingdi.h` are struct **fields**, not locals;
see "Not renamed" below.

Function counts move only upward: nine helpers added in `wingdi.c`,
three in `wingdi_aftereffects.c`, three in `wininputoutput.c`, one in
`adpcm.c`, none removed anywhere.

## Verification

Every one of the five has a differential test: the frozen pre-pass copy
and the rewrite compiled into one binary with `BASE_`/`NEW_` prefixes
on every symbol either defines, driven over identical state, with the
ordered sequence of engine and device calls compared as well as the
results.

| test | cases | compared | differing |
|---|---|---|---|
| `tests/run_wingdi.sh` | 5,152 | 150,984 engine calls | 0 |
| `tests/run_wingdi_ae.sh` | 576 | 293,356 logged entries | 0 |
| `tests/run_winsound.sh` | 1,344 | 2,302,852 bytes of state and call log | 0 |
| `tests/run_winio.sh` | 77,891 | state and call log | 0 |
| `tests/run_adpcm.sh` | 2,326,786 | decoded samples | 0 |

All five are called from `tests/run.sh` and can also be run on their
own; `tests/run_winsound.sh --guards` prints its seed-bit distribution.

The Direct3D device, DirectSound, DirectInput and the Win32 window are
logging mocks. `math.c` and `profiler.c` are linked for real rather
than mocked in the `wingdi` test, with `MATH_InitModule()` called
before the first case — without it every table-driven result collapses
to one arm on both sides and compares equal.

### What the stubs had to become first

`tests/stub/` grew by roughly 480 lines across this batch: `d3d9.h` by
330, `windows.h` by 119, `d3dx9math.h` by 28. The additions are the
Direct3D interfaces and vtables the renderer actually calls through
(`IDirect3D9`, the vertex, index and swapchain buffers, the vertex
declaration and shader interfaces, ~38 device vtable members), the
`D3DX` font and surface entry points, and the Win32 window, thread,
file, COM, dialog and keyboard calls the platform layer makes.

The payoff beyond parsing: **implicit function declarations across the
tree went from 16 over six files to 6 over five**, and
`win/wingdi.c`, `win/wininputoutput.c` and `win/winsound.c` are all off
the list entirely. The four passes' sets of declarations resolve more
together than any one does alone — `tests/implicitcheck.sh`'s baseline
is measured on the merge for that reason, not carried over from a
branch.

### Two stub faults that were hiding real defects

Both were found by a mutation that should have been caught and was not,
and both are worth stating because the same trap applies to any stub.

**Constants that are distinct in the SDK must be distinct in the stub.**
Seven `D3D*` names in `tests/stub/d3d9.h` were defined as `0`, so no
differential test could tell them apart: swapping
`D3DTSS_TEXCOORDINDEX` for `D3DTSS_TEXTURETRANSFORMFLAGS` scored clean.
They now have distinct values, and the file carries a banner saying the
values are distinct-but-not-SDK. That fix alone did **not** close the
blind spot in `tests/test_matdraw.c`, because the fixture's own guards
for those two fields were derived from seed bits that are zero for
every seed it sweeps — a dead guard and a collapsed constant have to be
fixed together. With both fixed the swap moves 70 cases, and that
test's compared call count went from 9,345 to 9,479.

**Sequential numbering can invert an ordering the code tests.** The
stub numbered the `WM_*` messages in declaration order
(`WM_ACTIVATEAPP` 16385 … `WM_SYSCOMMAND` 16393), which is the reverse
of Win32's. Both of `win/wingdi.c`'s message handlers open with a range
test — `msg > WM_CLOSE`, `msg > WM_ACTIVATEAPP` — so under the stub's
numbering **every other arm of both handlers was unreachable** and no
input could enter them. `tests/wingdi_msg_sdk.h` forces the real
numbers into both sides with `-include`.
`tests/wingdi_ae_sdk.h` does the same thing for the `D3D*` family in
the after-effects test, for the different reason that the frozen copy
writes bare numbers where the rewrite writes names.

## Defects found

### A dropped guard with a 2³⁰-iteration loop behind it

`D3D_l_DrawElementIndexedTriangles` fills a colour buffer either from
the vertex-colour array or, when there is none, with a solid colour.
The original guards the solid-fill arm with
`else if (v102a < &v102a[v4->vertexNum])` — a pointer comparison that
is simply `vertexNum > 0`. The rewrite had dropped it, leaving a bare
`else`, and the loop bound underneath is `(((4 * vertexNum - 1) >> 2) +
1)`, which at `vertexNum == 0` is about 2³⁰. Restored as
`else if (geogro->vertexNum)`.

Found by reading, not by the sweep: the fixture never generates a
zero-vertex mesh. That is the shape of defect a differential test is
worst at — one that only fires on an input the fixture does not
produce.

### Preserved rather than fixed

Eighteen sites across the batch carry a one-line `// preserved:`
comment, which is the only comment any of these passes added. They are
listed per file below; each says what the code does that a reader would
otherwise take for a transcription mistake.

**`win/wingdi.c`** (six) — a `return` that skips its matching profiler
end block, so raster 38 stays entered; a source array walked with no
null test unlike the one above it; an alignment test that can never
fail because `align` is masked to 0..15; an `elem` still pointing at
the `0xC0DE` marker, so a read lands at `second[49374]`; the colour
arg 2 backup taken from arg 1; and one of nine second-colour lookups
scaling its index by four.

**`win/winsound.c`** (eight) — a registry key left open when the value
cannot be read, and another left open on every failing path; a handle
array grown from `sndPlayList` rather than `sndEventHandles`; the
distances and position in `pcSND_SetDefaultSoundPositionAndDistance`
passed as integers when they are float bit patterns; a guard that
tests the address of a field rather than the interface in it; an ADPCM
block stride that equals the struct size only in a 32-bit build; two
allocations that leak when either call fails; and an index one past
the array when no instance matches.

**`win/wingdi_aftereffects.c`** (three) — a count used where the other
arms use an index, so a loop stops short of 254; a first pass taking
its vertical offset from the unnarrowed product where the rest take
the narrowed step; and one getter returning the motion blur for the
unknown slot where every other getter returns zero.

**`win/wininputoutput.c`** (four, written as block comments because
they need a sentence) — a float read through an `int*` because the
caller passes a float; a path reached with `pcINO_mouseDevice` NULL; a
dereference of `pcINO_keybDevice` in `pcINO_getKeyNames`; and a buffer
size of 266 where the first call and the buffer disagree.

**`win/adpcm.c`** (one) — a floor constant that is not `-32768`, though
every use of it narrows to `int16_t`.

## Mutation testing

A zero-difference result means nothing until the harness is shown to
have teeth, so every one of the five was mutation-checked twice: once
by the pass that wrote it, once on review with an independently chosen
defect.

The `wingdi` pass injected **24 and caught 23** on the second attempt.
The first run caught 17, and all seven misses were fixture blind spots
rather than no-ops; six were closed, and the reasons are instructive:

- a projection fit test needed `fitW == rectW` **and** a non-unit fit
  ratio, because at a square window the refit is the identity and even
  hitting the boundary exactly is invisible;
- an index-scale site needed four seed bits to agree, and the first fix
  looped to 6 where the arm is `== 7`;
- a colour-ring bound needed a cursor value *inside* the window, not
  above both bounds;
- an immediate-mode guard needed an index buffer and a strip block
  both in place;
- a snapshot stride was invisible because the mock's locked surface was
  memset to zero and the destination was never hashed;
- a window-message range test was unreachable under the stub's
  numbering, as above.

**One was not caught, and it is a provable no-op of the code.**
`D3D_SetTexture`'s `if (textureID != -1 && textureID != -2)`: the block
above it already stores `-2` into the stage's `d3dTextureID` whenever
`textureID == -2`, so the enclosing
`if (textureID != …d3dTextureID)` is false at that point and the
`!= -2` term can never decide anything. Redundant in the original,
preserved as-is.

Review mutations on top of each pass's own, all caught:

| file | mutation | cases moved |
|---|---|---|
| `wingdi.c` | stride `+= 4` → `+= 6` in the texture-size loop | caught |
| `wingdi.c` | blend case 3's else-arm `INVDESTALPHA` → `DESTALPHA` | caught |
| `wingdi.c` | fog enable mask `0x4000` → `0x2000` | caught |
| `wingdi.c` | viewport height clamp subtracts `clampedX`, not `clampedY` | caught |
| `winsound.c` | pan arm reads `arr[balance]` on both sides | 21 |
| `winsound.c` | centre reference `arr[64]` → `arr[65]` | 24 |

### A winsound mutation that scored clean, and why that was right

Changing a `<=` to a `<` on the volume table's index bound moved
nothing. That is not a harness hole: the table was parsed out of the
file and has 128 entries with `arr[63] == arr[64] == -300`, so the two
bounds select equal values and no input can separate them. Parsing it
needed care — the values are hex-encoded negatives (`0xFFFFFFF6`), and
a naive `-?\d+` scan returns 238 nonsense entries instead of 128.

### Two fixture faults worth carrying forward

**Uninitialised locals read the other copy's leftovers.** The two sides
run one after the other from the same stack frame, so a local the
second copy fails to initialise lands on bytes the first copy left
behind and reads back as correct. Dropping `Joystick_Update`'s clear of
`mouseData` scored clean until `tests/test_winio.c` gained a
`dirtyStack()` that scribbles 3 KiB of the frame between runs. **None
of the 35 harnesses written before this batch have it**, which is an
open item.

**Inserting cases shifts the seed stream.** Adding four cases to
`tests/test_gameobject.c` moved every later `seed++` and silently
un-caught a mutation that had been caught before. Added loops go on a
fresh seed base — 5900 there — and the condition they are meant to
drive is driven deliberately rather than left to the sweep.

## Not renamed

Four `_sub_<address>` names survive in `win/`, all of them because the
call sites are outside the pass that found them:

| name | proposed | other sites |
|---|---|---|
| `MAT_ComputeUV_sub_4521A0` | `MAT_UV_ComputeAndSetTransform` | `materialanddraw.c`, `tests/matdraw_mocks.c`, and the frozen `tests/baseline_matdraw.c` |
| `INO_SetControlMode_inner_sub_48ACB0` | `pcINO_SetControlMode` | `inputoutput.c`, `strings.c`, `tests/io_mocks.c`, `tests/strings_mocks.c` |
| `INO_SetControlSubmode_inner_sub_48A650` | `pcINO_SetControlSubmode` | the same four |
| `PC_AE_MASTER_GET_ONOFF_sub_4A37D0` | `PC_AE_MASTER_GET_ONOFF` | `ai/ai_miscfunctions.c` and its header |

All four are blocked on the same question: a frozen baseline calls the
old name, so either the mock files carry **both** spellings or the
baselines are regenerated. That is the user's call, and it is the same
question as the five deferred shared-header field renames.

The eleven residue lines in `win/wingdi.h` are the `v1` field of
`tVertex_type_204/404/244/104/144/444/142_generic` and `v1`..`v4` of
`tD3dSpriteData`. `v1` should be `pos`, but the field is read by
`win/wingdi.c`, `win/wingdi_aftereffects.c` and two frozen baselines,
and the usual `#define` shim does not work because `v1` is also an
`Mtx4x4` field.

The last parameter of `D3D_l_DrawElementIndexedTriangles` is left
deliberately unnamed: naming it makes `tests/hdrcheck.py` report
`types moved 1`, because the checker parses a bare `int` as the
parameter name.

## What the tests cannot reach

`win/wingdi.c`'s device lifecycle and Win32 entry points —
`D3D_l_Init`, `ENG_WinGameWindowInit`, `PC_EngineCheatFinal`,
`D3D_SaveScreenshot`, `PNG_WriteBgra`, `D3D_ReinitD3DAppBackCache`,
`D3D_pst_CreateDevice`, `s_HandleWinMessages`, `PC_WNDProcHandler`,
`RegQueryVal` and the small release helpers — are not driven by the
sweep. Each was verified textually unchanged apart from two parameter
renames, so nothing untested was restructured.

Two of the user's comments in `win/wingdi.c` are lost: `// cc` and
`// esi`, Hex-Rays register annotations that sat on the declaration
lines of a local that is now deleted and a local that is now `i`.
Everything else survives byte for byte, checked by diffing the comment
multiset against the pre-pass copy.

## Open questions for the user

- `pcSND_SetDefaultSoundPositionAndDistance` passes float bit patterns
  as integers. Preserved; it may be a decompilation artefact or it may
  be what the original did.
- `pcINO_InputHandlerCase2` casts a float through an `int*`. Same
  question.
- The six memory-card failure flags in `win/wininputoutput.c` are set
  and never read.
- Whether the frozen baselines may be regenerated, which unblocks all
  four `_sub_` renames and the `v1` → `pos` field rename.
