# CIN_CutsceneMode

Where the implementation in `ai/ai_miscfunctions.c` and
`win/wingdi_aftereffects.c` came from, kept here so the source files
do not carry it.

## The id

`0x3CF8`, handler `0x00ABA370` in the 2003 retail `BGE.exe`.

irisEng had no row for it at all — its table stepped from `0x3CF7`
(`ConsoleSpecificOption`) straight to `0x3D22`
(`GFX_FS_VideoStart`), and nothing in the tree mentioned the name.
The remaster's PDB carries `CIN_CutsceneMode` and
`AI_EvalFunc_CIN_CutsceneMode` as public symbols, which is where the
spelling comes from; it has no public symbols for the globals, so
the names below are ours.

## The signature

The handler calls the stack-pop helper `0x00494140` exactly once,
moves the result into `ecx`, calls the `_C` at `0x00ABA300`, and
advances the node pointer by eight without pushing anything back. So
it is

```
CIN_CutsceneMode(INT) -> void
```

Retail's own AI-function table in `ka87/bgeaitools` records it as
`params: []` with `ret: TYPE_UNKNOWN`, flagged `inferred` and
`arity_verified: false` — a guess, and the wrong one.

## What the _C does

```
void CIN_CutsceneMode_C(int on)        // 0x00ABA300
{
    <double at 0x00B8A1C8> = 0.0;      // two dword stores, both arms
    if (<0x00B8A1C0>)
        <0x00B5D228> = on ? 1 : 0;
}
```

MSVC emitted the zeroing once per arm of the `if (on)`; there is no
shared tail.

Three globals, and what each turned out to be:

| retail | what it is | here |
|---|---|---|
| `0x00B5D228` | cinematic bands currently drawn | `CIN_Bands` |
| `0x00B8A1C0` | the `NoBands` application setting | `CIN_NoBands` |
| `0x00B8A1C8` | a `double` the host's frame-time smoothing accumulates into | not reproduced |

**`0x00B5D228` is the band flag.** The viewport code at `0x00403C50`
reads it and fills four floats at `0x00B66E00`: `0, 1.0, 0, 1.0`
when it is clear, and `0x3DD55555, 0x3F655555, 0x3DAAAAAB,
0x3F6AAAAB` when it is set. Those four constants are
`0.10416666, 0.89583331, 0.083333336, 0.91666669` — exactly
irisEng's `bandsVal1` … `bandsVal4`, so `0x00B66E00` is that array
and the test above it is the one irisEng spells
`if (tweak_ae_nobands)`.

**`0x00B8A1C0` is a setting, not state.** Startup reads it at
`0x00402414` from the application settings under the key `NoBands`
(the string is at `0x00B4FD04`, beside `Blurryness` at
`0x00B4FCF8`), and in the same breath sets
`0x00B5D228 = (NoBands == 0)`. So the setting means *let the script
drive the bands*: with it off, the bands are latched on at startup
and `CIN_CutsceneMode` cannot move them.

**`0x00B8A1C8` is a `double`**, not two ints — MSVC splits the store.
The host's frame-time smoothing accumulates into it between
`0x00402A4F` and `0x00402AAA` and divides the result into
`0x00B66E10`. Clearing it on a mode change stops the smoothing
carrying across the cut. irisEng has no counterpart to that
accumulator, so nothing here resets one.

## Who calls it

Two behaviours in retail's whole AI2C table, and they are a pair:

| key | behaviour | argument |
|---|---|---|
| `0x3500B728` | `Cine_Events_Cine_exec_InitialiseCine` | `1` (`mov ecx,0x1` at `0x00779071`) |
| `0x350091C9` | `Cine_Events_cine_play_eventtrack` | `0` (`xor ecx,ecx` at `0x00778BAD`) |

Both also clear the same model variable at `+0x134` immediately
before the call. So the bands go up when a cine is initialised and
down when its event track finishes.

## What this means for the default

`CIN_NoBands` defaults to **1** here. irisEng has no reader for the
application settings, so there is nothing to fill it from, and 1 is
the configuration that gives the game its normal behaviour: bands
down during play, up for the length of a cine, which is what the two
callers above are written for.

That is a change: before this, irisEng drew the bands permanently,
because `tweak_ae_nobands` defaults to 0 and sat in the slot where
retail reads the band flag. The tweak is kept as a forced override —
the two live sites now read `tweak_ae_nobands || !CIN_Bands` — so
setting it still pins the full frame. Setting `CIN_NoBands` to 0
restores the old always-banded behaviour exactly, and is also what
retail does when the setting is absent.

The third `tweak_ae_nobands` site in `wingdi_aftereffects.c` is
inside a commented-out copy of `D3D_AE_CopyToScreen` and was left
alone.

## One thing not changed

irisEng binds **both** `0x3CF4` and `0x3CF5` to
`AI_EvalFunc_CIN_OnOff`. Retail has `0x3CF4` as `CIN_OnOff` but
`0x3CF5` as `DBR_SetN` (handler `0x00ABA2D0`, one `INT`), and
irisEng already has `AI_EvalFunc_DBR_GetN` on `0x3CF6` matching
retail's `DBR_GetN` — so the DBR pair looks split with the setter
missing. Not touched here; it is a separate question from this one.
