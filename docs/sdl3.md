# SDL3

SDL3 is vendored as source under `thirdparty/SDL3`, pinned at **3.2.24**
(the `release-3.2.24` tag). It is being taken on so that the sound and
the input can stop being DirectSound and DirectInput: those two are the
parts of the engine that cannot leave Windows. The input is done; the
sound is next.

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

The six Win32 window handlers in `platform/d3drender.c` are gone. What they
did for the engine -- creating the display, tearing it down, pausing
on focus loss -- is done where it belongs now: `main.c` opens and
closes the display around the window, and `platform.c` says the rest.

`PLT_p_NativeWindow` hands out the `HWND` for as long as Direct3D and
the Win32 parts of the sound still want one.

One thing the handlers did has moved rather than survived: the
engine's statistics used to go into the window's caption on a click of
the title bar, which is a non-client click and not something a window
SDL owns passes on. They are on **Home** now, the function keys all
being taken -- F1 to F10 by the debug keys and F11 by the overlay's
command field, which moved there from F12.

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

### There was no gamepad

The game came off a console, so a **pad is the only input model it
has**: everything above the backend reads `sInoPad` and the two
sticks, and the PC build fills them from the keyboard and the mouse
according to the selected control mode. The names say so once you
look -- `pcINO_joystickPosition` is a stick driven from the arrow
keys, and `pcINO_JoystickPositionAlignSetFromKeyboard` is what drives
it.

So no joystick device was ever opened. What was left of DirectInput
after the keyboard and the mouse moved was an `IDirectInput8A` that
got created, released, and used for nothing -- its only purpose had
been making those two devices -- plus a property wrapper nothing
called. All of it is deleted and `dinput8.lib` is out of the linker
inputs. **DirectInput is gone.**

A real pad would be a feature rather than a port: the interface is
already there and already the one the whole game reads, so
`SDL_Gamepad` would feed `padData`, the two sticks and the two empty
rumble functions. That is behaviour the retail PC build never had.

## What is left

- `<dsound.h>` out of `sound.h`, which wants a wave format of the
  engine's own first;
- the Win32 thread, event and file calls, on SDL's;
- `platform/eax/eax.h`, once it is decided whether the room the engine
  computes for a reverb nothing applies is worth keeping.

### The audio device, and a voice per sound

`sndplatform.c` is the sound's equivalent of `platform.c`, and the
first part of taking the audio off DirectSound. Nothing in the engine
calls it yet; `platform/sndbackend.c` is the next piece.

It is not a thin wrapper, because the two models do not meet. A
DirectSound buffer was **memory the engine wrote into** and then told
to play, loop, seek, pan and attenuate, and it reported back how far
it had got. An SDL audio stream is the other way round: a queue the
device pulls from, with no buffer to seek in and no play position to
read. So a voice here is the buffer *plus* the cursor over it, and the
feed is what turns one into the other -- a chunk at a time pushed into
a stream of its own, with SDL mixing the streams together.

The units stay the engine's, which are DirectSound's: attenuation and
pan are both hundredths of a decibel, attenuation 0 for untouched and
-10000 for silence, pan the difference between the two channels.

Three places where the two ways of doing it cannot be reconciled, and
what was chosen:

- **Attenuation** is applied by SDL as it pulls, so a change is heard
  almost at once -- which is what the engine's fades expect. The
  device's attenuation and a voice's add, as two attenuations in
  decibels do.
- **Pan** is applied as the chunk is fed, because a stream carries one
  gain and not two. A change is therefore heard one queue later, which
  is why the chunk is about 10 ms and no more than four are queued.
- **Nothing here is spatial.** DirectSound could place a buffer in a
  room, and the engine never asked it to beyond deriving a position on
  a unit circle from the pan it had already computed itself. Both of
  `pcSND_i_ChangeRenderMode`'s modes end in a plain `SetPan` and
  `SetVolume`. So the pan is what carries the intent, and the pan is
  what is kept; what goes is DirectSound's own spatialisation.

`tests/run_sndplatform.sh` is 93 checks against the real library on
SDL's dummy audio driver -- a real device, real streams bound to it,
real conversion, consumed by nothing. Twenty mutations are each
caught, including the pan not reaching a channel, the two attenuations
not adding, a one-shot looping anyway, a pause losing its place, and
a duplicate not sharing its sound.

### The sound

`platform/sndbackend.c` has no DirectSound in it. The fifteen buffer verbs,
in all three of their implementations, go over a voice from
`sndplatform.c`: `Lock`/`memcpy`/`Unlock` became a write at an offset
that wraps, `GetCurrentPosition` the cursor, `SetVolume` and `SetPan`
the attenuation and the pan, `DuplicateSoundBuffer` the duplicate
that shares its sound. `platform/sndvideo.c` went the same way, and
had to -- it was being handed the engine's `IDirectSound8`.

`SND_gpst_Interface` is six tables and three implementations: `win`
is the base, `stream` overrides the fifteen verbs, `music`,
`ambience` and `dialog` are copies of `stream` with a different tag,
and `track` overrides the same fifteen again.

**The thread stays a thread**, which is what was asked for. It waits
on the same four control events -- add to the play list, remove,
exit, stop-and-remove -- and the refill positions have become a
timeout: nothing in SDL says when a cursor reaches a position, so the
thread looks instead. A block of the buffer it has not seen the
cursor in before is the refill the device used to announce, and a
voice that is no longer playing is the stop it used to announce.

**No spatialisation and no reverb.** Both render modes already ended
in a plain pan and volume on the 2D buffer; the 3D buffer was parked
at a fixed position so the EAX reverb had something to hang off, and
the one place it was moved derived the position from the pan the
engine had already worked out. So the pan is kept and the
spatialisation goes. The reverb entries answer that it cannot be set,
which is what a card without EAX answered -- and the engine coped
with that then, because plenty of cards were.

What still carries DirectSound's name:

- `sound.h` includes `<dsound.h>`, for four fields the frozen copy in
  `tests/` still uses and for `WAVEFORMATEX`, which the wav headers
  are declared with. That one is Win32's rather than DirectSound's,
  so getting the include out means giving the engine a wave format of
  its own, which reaches `sound.c` and the loaders.
- `platform/eax/eax.h` stays for `EAXLISTENERPROPERTIES`, the shape of a
  room the engine still computes and nothing now consumes.
  `eaxguid.lib` and its library path are out of the project.
- The Win32 thread and event calls -- `CreateThread`, `CreateEventA`,
  `WaitForMultipleObjects` -- and the `CreateFileA`/`ReadFile` path
  the decoder reads through.
