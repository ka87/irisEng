# irisEng settings

Writes the display settings the engine reads when it starts into the
`[display]` section of `engine.ini`. Without them it stops with
"Application settings not correctly set or invalid" before opening a
window, and a wrong depth format or display mode shows up later as
"IDirect3D9_CreateDevice failed".

It used to write these as registry values under
`HKEY_CURRENT_USER\Software\IRISEng`. Nothing in the engine reads the
registry any more.

## Build

    dotnet build -c Release

Produces `irisEngSettings.exe`. It targets .NET Framework 4.8 and
builds as x86, so the display modes it enumerates are the ones the
engine will see.

## Where it writes

`engine.ini`, in the folder this application is in. That is the game's
folder as well, because the two ship together -- and beside the
executable is where the engine looks for the file rather than in the
working directory, since a game started from a shortcut has whatever
working directory the shortcut says.

## What it writes

Seven keys under `[display]`:

| Key | Meaning |
|---|---|
| `adapter` | adapter ordinal |
| `mode` | **index** into the adapter's X8R8G8B8 display-mode list |
| `zbuffer` | a `D3DFORMAT`, used directly as the auto depth-stencil format |
| `mipmapping` | 0 or 1 |
| `windowed` | 0 or 1; run in a window rather than fullscreen |
| `width` | the chosen mode's width, used to size the window |
| `height` | the chosen mode's height, used to size the window |

`mode` is a list position, not a resolution, so it only means anything
next to the adapter it was enumerated from. Changing the adapter
re-reads the list and re-picks the mode.

`adapter`, `mode`, `zbuffer` and `mipmapping` are all required: the
renderer checks each one is there and a number, and exits if any is
missing.

`windowed`, `width` and `height` are optional. Without them the engine
runs fullscreen and creates its window at 640x480, which is what it did
before they existed. `windowed` only ever turns windowed mode on: a
debugger being attached, and the `/WINDOWED` command line switch, each
still force it on by themselves, and a zero here does not override
them.

Only the modes and depth formats the chosen adapter actually reports
are offered, so a saved configuration should not fail device creation.
`mode` is only consulted for fullscreen. Windowed mode takes its size
from the window, which is created at `width` x `height` -- the same
resolution the mode names, written out separately so the engine can
size the window before Direct3D exists.

## The rest of the file is left alone

`engine.ini` is hand-edited and most of it is none of this
application's business: its comments, and the `[data]`, `[render]` and
`[video]` sections. Saving therefore edits the file rather than writing
it out afresh. Each of the seven keys already in `[display]` has its
value replaced where it stands, keeping any comment on the end of that
line; only the ones that are not there yet are added, at the end of the
section. Nothing else is touched, down to the file's line endings. A
file with no `[display]` section gets one appended, and a file that is
not there at all is created.

Values are read the way the engine reads them: section and key names
without regard to case, whitespace trimmed, a line beginning with `;`
or `#` a comment, decimal or `0x` hex accepted, and the last value
given for a repeated key. A key that is absent or is not a number
counts as not set, and the form then offers its own default for it.

## The audio device is not chosen here

The sound backend picks its own output device, so the two values this
used to write under `Software\IRISEng\Sound` -- `Sound FX` and
`Audio board GUID` -- have no reader left. The sound device and sound
effects controls are gone from the form with them.
