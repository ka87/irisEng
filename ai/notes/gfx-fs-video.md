# GFX_FS_VideoStart / VideoStop / VideoFinish

Where the implementation in `ai/ai_gfxfunctions.c` came from, kept
here so the source file does not carry it.

## The ids

| id | what it is |
|---|---|
| `0x3D22` | `GFX_FS_VideoStart` |
| `0x3D23` | `GFX_FS_VideoStop` |
| `0x3D24` | `GFX_FS_VideoFinish` |

`0x3D22` and `0x3D23` used to be bound to `AI_EvalFunc_CIN_OnOff` and
`AI_EvalFunc_DbgClearLog`. Both of those are bound again elsewhere —
`CIN_OnOff` at `0x3CF4` and `0x3CF5`, `DbgClearLog` at `0x100B` — so
the two rows looked like filler, and in the retail game the ids are
the fullscreen video pair that belongs beside the `VideoFinish`
already at `0x3D24`.

## What each one does

**Start** takes one `INT` off the AI stack and throws it away. The
file is not a parameter: it is hard-coded to `BGEMakingOf.bik`, the
making-of reel the main-menu trailer behaviour plays. The sequence is

```
BinkSetSoundSystem(BinkOpenDirectSound, <the DirectSound object>);
handle = BinkOpen("BGEMakingOf.bik", 0);
if (!handle) return;
<create an offscreen surface, lock it, clear Width * Height * 4
 bytes to black, unlock it>
playing = 1;
```

**Stop** takes no arguments: it releases the surface, closes the Bink
handle and clears both globals.

**Finish** returns 1 when there is no handle, and otherwise whether
the current frame has reached the frame count. With no Bink library
linked there is never a handle, so it stays 1 and a script waiting on
it proceeds immediately.

## The Bink entry points

The retail game links `binkw32.dll` and uses `BinkOpen`, `BinkClose`,
`BinkSetSoundSystem` and `BinkOpenDirectSound` for the three
functions above. It also imports `BinkDoFrame`, `BinkWait`,
`BinkNextFrame`, `BinkPause`, `BinkCopyToBuffer`, `BinkSetIO`,
`BinkSetIOSize`, `BinkSetSoundTrack` and `RADSetMemory` for the
per-frame path that runs while the playing flag is set — that path is
not implemented here.

Everything above is behind `IRISENG_HAVE_BINK` in the C, and the
`#else` leaves the state cleared.
