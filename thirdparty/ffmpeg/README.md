# libav (FFmpeg), split by pointer width

    x32/    the 32-bit Windows build, committed
    x64/    the 64-bit one, not committed

Each half is a complete and self-consistent set: `include/`, `lib/`
and `bin/` from one build, plus that build's `LICENSE.txt`. They are
kept apart because a header does not travel without its library -- a
mismatched pair compiles, links, and then goes wrong at run time,
which is the one way this breaks quietly.

The two `Win32` configurations of `irisEng.vcxproj` use `x32`; the two
`x64` configurations use `x64`. CMake picks by `CMAKE_SIZEOF_VOID_P`
and stops with a message if the half it wants is empty, as the project
file does.

The Linux sweeps under `tests/` read `x32/include` whichever width
they compile at. The public headers do not differ between the two
Windows builds -- `avconfig.h` carries only endianness and
fast-unaligned -- so they read the half that is committed.

`x32/README.md` has the version, the LGPL position, what is and is not
included and how to build a far smaller one. `x64/README.md` says what
to put in the other half.
