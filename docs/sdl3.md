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
