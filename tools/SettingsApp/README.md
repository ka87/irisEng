# irisEng settings

Writes the display settings the engine reads when it starts into the
`[display]` section of `settings.ini`, in the player's own folder
beside the saves. Without them it stops with
"Application settings not correctly set or invalid" before opening a
window, and a wrong depth format or display mode shows up later as
"IDirect3D9_CreateDevice failed".

It used to write these as registry values under
`HKEY_CURRENT_USER\Software\IRISEng`, and then into `engine.ini`
beside the game. Nothing in the engine reads the registry any more,
and `engine.ini` is the install's file rather than the player's -- a
`[display]` section left in it is ignored.

## Build

    dotnet build -c Release

Produces `irisEngSettings`, or `irisEngSettings.exe` on Windows. It is
an Avalonia program on .NET 10 and runs on Windows and on Linux.

On Windows it builds x86, because an adapter can offer a different
mode list to a 32-bit caller and the engine is 32-bit -- the stored
mode is an index into that list. Off Windows there is no such
constraint and .NET has no 32-bit Linux runtime anyway; nothing there
stores an index.

It is a framework-dependent build, so the machine needs the **.NET 10
runtime**: the desktop runtime and specifically the **x86** one on
Windows, which is a separate download from the x64. That is a change
from the .NET Framework 4.8 version, which ran on what Windows already
had. To hand someone the program without asking them to install
anything:

    dotnet publish -c Release -r win-x86   --self-contained true -p:PublishSingleFile=true
    dotnet publish -c Release -r linux-x64 --self-contained true -p:PublishSingleFile=true

## Where the modes come from

Two backends, because they answer for two renderers rather than being
two ways of asking the same question.

**Windows** asks Direct3D 9, because the engine's Direct3D renderer is
what reads the answer, and the `mode` it stores is an index into
Direct3D's own list of X8R8G8B8 modes. It also offers the depth-buffer
formats that adapter supports, and the mip-mapping flag.

**Linux** asks SDL first and the kernel second:

- **SDL** is the same library the engine opens its window with, so its
  list is the one that will actually be honoured, and it is the only
  one of the two that speaks both X11 and Wayland. Wayland has no
  protocol for a client to ask a monitor what modes it has -- the
  compositor tells the toolkit, and SDL is the toolkit.
- **`/sys/class/drm`** is the fallback, for a machine with no
  `libSDL3.so` installed -- which is most of them, since the engine
  links SDL statically and so does not leave one about. It is the
  driver's own mode list, needs nothing installed, and is readable
  with no display server running at all. It carries sizes but no
  refresh rates, which is enough: the engine does not store one.

The depth-buffer and mip-mapping controls are hidden on Linux. They
are not shy about being unsupported -- the engine's OpenGL path never
reads them. Off the Direct3D path it reads `width`, `height` and
`windowed` and nothing else. Whatever those two keys already held is
written back untouched rather than cleared, so a dual boot does not
lose the Windows side's settings.

## Where it writes

`Documents\irisEng\settings.ini` -- the player's own folder, the same
one the saves and the screenshots are in, and where the engine looks.
The folder is created if it is not there yet, which it will not be if
this is run before the game ever has been; the engine creates it too,
on its way to the same file, so whichever runs first does it.

Not the folder this application is in. That folder is the install, and
these settings are not what the install is: they are what this player
has chosen. Keeping them apart means an install can be replaced or
copied without taking somebody's resolution with it, and that their
choices survive one. It also means the file is somewhere that can
certainly be written -- the game has already had to write there to
save at all, where an install folder under Program Files is not
somewhere either program can count on.

The engine asks SDL for the documents folder and puts `irisEng` under
it; this asks .NET for the same folder and uses the same name. That
name is the one thing the two have to agree on, and it is a constant
in `Settings.cs` on this side and `DPT_GAMENAME` in `datapath.c` on
the other.

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

`settings.ini` may be hand-edited, and anything in it that is not one
of the seven keys is none of this application's business -- its
comments above all. Saving therefore edits the file rather than
writing it out afresh. Each of the seven keys already in `[display]` has its
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
