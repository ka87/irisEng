# irisEng settings

Writes the registry values the engine reads when it starts. Without
them it stops with "Application settings not correctly set or invalid"
before opening a window, and a wrong depth format or display mode
shows up later as "IDirect3D9_CreateDevice failed".

## Build

    dotnet build -c Release

Produces `irisEngSettings.exe`. It targets .NET Framework 4.8 and
builds as x86, so the display modes it enumerates are the ones the
engine will see.

## What it writes

Under `HKEY_CURRENT_USER\Software\Ubisoft\Beyond Good & Evil\SettingsApplication.INI`:

| Key | Value | Type | Meaning |
|---|---|---|---|
| `Basic video` | `Video Adapter` | DWORD | adapter ordinal |
| `Basic video` | `Mode` | DWORD | **index** into the adapter's X8R8G8B8 display-mode list |
| `Basic video` | `Z Buffer depth` | DWORD | a `D3DFORMAT`, used directly as the auto depth-stencil format |
| `Basic video` | `MipMapping` | DWORD | 0 or 1 |
| `Sound` | `Sound FX` | DWORD | effects are **off** when this reads exactly 1 |
| `Sound` | `Audio board GUID` | BINARY, 16 bytes | DirectSound device; all zero is the default device |

`Mode` is a list position, not a resolution, so it only means anything
next to the adapter it was enumerated from. Changing the adapter
re-reads the list and re-picks the mode.

The four `Basic video` values are all required: the engine checks each
one is present and a DWORD, and exits if any is missing. The two
`Sound` values are not fatal, but sound initialisation gives up
without them.

Only the modes and depth formats the chosen adapter actually reports
are offered, so a saved configuration should not fail device creation.
`Mode` is only consulted for fullscreen; windowed mode takes its size
from the window.
