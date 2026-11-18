# SDL3

SDL3 is vendored as source under `thirdparty/SDL3`, pinned at **3.2.24**
(the `release-3.2.24` tag). It is being taken on so that the sound and
the input can stop being DirectSound and DirectInput: those two are the
parts of the engine that cannot leave Windows.

## What is here, and what is not

The library and what builds it:

    include/            the public headers
    src/                the library itself
    cmake/              what its CMake build needs
    build-scripts/      likewise
    wayland-protocols/  for a Linux build that wants Wayland
    CMakeLists.txt
    VisualC/SDL/        its own Visual Studio project, unmodified
    LICENSE.txt, README.md

Left out, because none of it is the library: `test/` and `examples/`
(29 MB between them), `docs/`, `Xcode/`, `android-project/` and
`VisualC-GDK/`.

Nothing in `thirdparty/SDL3` is modified. Updating SDL is replacing the
directory with the same set of folders from a newer tag -- which is why
its own Visual Studio project is used rather than SDL's sources being
listed in ours.

## The Windows build

`irisEng.sln` carries SDL's project beside ours, and `irisEng.vcxproj`
references it, so Visual Studio builds SDL first and puts `SDL3.dll`
next to the exe. `thirdparty\SDL3\include` is on the include path of
all four configurations.

SDL's project builds a DLL, which is how SDL is normally shipped and
is left as it ships. No `.lib` or `.dll` is committed.

## The tests

`tests/sdl3.sh` builds the vendored source once into `tests/build/sdl3`
and is sourced by the harnesses that need it. The build is deliberately
**headless**: no X11, no Wayland, no ALSA, no PulseAudio, no Vulkan.

What is left is SDL's own dummy audio driver and dummy video driver,
and that is the point rather than a limitation -- the tests get a real
SDL, with real audio streams and a real event queue, writing to
nothing. So the sound and input backends can be tested against the
library they are written for instead of against a mock of it, which
would only encode what the author assumed the library does.

`tests/run_sdl3.sh` checks the dependency on its own: that it builds,
that it comes up headless, that the library is the vendored one rather
than another on the machine, and that the calls the two backends will
be built out of behave as expected.

## What SDL owns so far

### The window and the events

`platform.c` is the whole of it, and `platform.h` is what the rest of
the engine sees. SDL creates the window, and `PLT_Pump` is what used
to be the message loop: a quit or a close request sets the engine's
exit flag, focus gained and lost pause the engine and take or give
back the input, typed text and the three editing keys go to the
overlay's command field, and a wheel movement is accumulated for the
next time the game asks. The pump is timed the way the message loop
was, so the profiler still sees it.

The six Win32 window handlers in `win/wingdi.c` are gone. What they
did for the engine -- creating the display, tearing it down, pausing
on focus loss -- is done where it belongs now: `main.c` opens and
closes the display around the window, and `platform.c` says the rest.

`PLT_p_NativeWindow` hands out the `HWND` for as long as Direct3D and
the Win32 parts of the sound still want one.

One thing the handlers did has moved rather than survived: the
engine's statistics used to go into the window's caption on a click of
the title bar, which is a non-client click and not something a window
SDL owns passes on. They are on **F11** now.

### The keyboard and the mouse

`PLT_ReadDevices` answers both in one call: the keyboard as a block
of what is down now, the mouse as how far it has moved since the last
frame plus its buttons. `PLT_InputCapture` takes the input and gives
it back -- relative mouse mode on the window, which is what the
exclusive DirectInput devices used to do -- and `PLT_sz_KeyName` says
what a key is called, for the controls screen.

The codes are the important part. The engine indexes a 266-byte block
with DirectInput's key codes, and so do its per-mode key tables, its
action map, its controls screen and **every player's saved
bindings**. So the platform layer translates SDL's scancodes into
those codes rather than changing anything above it: a 104-entry table
built once into a forward and a reverse array. Three slots past the
256 keys are the engine's own mouse bits, filled in the packing the
device used.

`win/wininputoutput.c` keeps its DirectInput interface for the
**joystick only**; the mouse and keyboard devices, the input-lost
path and the wheel-granularity property have gone with them.

## What is left

- the gamepad, on `SDL_Gamepad`, and the last of DirectInput with it;
- the sound, on `SDL_AudioStream`, replacing `win/winsound.c` and
  `win/winvideosound.c`;
- `dsound.lib` and `dinput8.lib` out of the linker inputs.
