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
they compile at. The two halves are the 9.0 branch at different
commits, so their headers are not byte-identical -- five micro
versions, the version string and two documentation comments differ --
but no declaration, type or constant does, and that is what those
sweeps are checking. `tests/run_ffmpegtrees.sh` is what keeps it
true.

`x32/README.md` has the version, the LGPL position, what is and is not
included and how to build a far smaller one. `x64/README.md` says what
to put in the other half.

## It is 211 MB between them

93 MB for `x32` and 118 MB for `x64`, of which the two `avcodec` DLLs
are 155 MB, because a default FFmpeg carries every codec it knows and
the engine wants exactly one: Bink.

`x32/README.md` has the `configure` line for a build with only Bink
enabled, which comes out in the region of a few hundred kilobytes.
Doing that for both halves would take this directory from 211 MB to
under 2, and needs no change to the project file as long as the layout
is kept. It has not been done because it needs an MSYS2 toolchain this
repository does not carry.
