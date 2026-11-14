# libav (FFmpeg), 32-bit Windows, shared, LGPL

What `video.c` links against. Committed so the project builds from a
fresh clone with nothing to download.

    ffmpeg-n9.0.2-48-gd4070a14ce-win32-lgpl-shared-9.0
    from https://github.com/defisym/FFmpeg-Builds-Win32/releases

## What is here, and what is not

| | |
|---|---|
| `include/` | the public headers of the five libraries used |
| `lib/*.lib` | MSVC import libraries |
| `lib/*.dll.a` | the same for a MinGW build, unused by the project |
| `bin/*.dll` | the five runtime DLLs, copied next to the exe after a build |
| `LICENSE.txt` | the build's own licence text |

Five libraries, because that is what `video.c` calls and what they
need between them: **avcodec**, **avformat**, **avutil**, **swscale**
and **swresample**. `avdevice` and `avfilter` ship in the same zip and
are not here — nothing in these five imports them, and they are
another 36 MB.

Nothing else is needed at runtime. The DLLs import each other and
Windows' own libraries and nothing further: no `libgcc`, no
`libwinpthread`, despite being a MinGW build. They do want the
**Universal CRT** (`api-ms-win-crt-*`), which is part of Windows 10
and later and an update away on 7 and 8.

## It is a 32-bit build

Which is what the engine's primary configuration is. The two `Win32`
configurations link it; **`video.c` is excluded from the two `x64`
configurations**, because there would be nothing for an x64 link to
resolve against. To build x64, put a `win64` build of the same
version somewhere of its own, point the x64 configurations at it, and
remove the `ExcludedFromBuild` on `video.c` in `irisEng.vcxproj`.

## LGPL

This is the **LGPL** build, not the GPL one, and it is linked
dynamically. That is what keeps the engine's own licensing position
unchanged, and it is the reason to leave the DLLs as DLLs: anyone
given a binary has to be able to replace them with their own build of
the same libraries. `LICENSE.txt` is the build's own text and belongs
with any distribution of it.

Swapping in another build means replacing `bin/`, `lib/` and
`include/` together — a mismatched header and DLL is the one way this
goes wrong quietly.

## It is much larger than it needs to be

90 MB of DLLs, of which `avcodec-63.dll` alone is 65 MB, because a
default FFmpeg carries every codec it knows and the engine wants
exactly one: Bink.

A build configured for only that is in the region of a few hundred
kilobytes:

    ./configure --prefix=/mingw32 --arch=x86 --target-os=mingw32 \
        --enable-shared --disable-static --disable-programs \
        --disable-doc --disable-everything \
        --enable-demuxer=bink,binka --enable-protocol=file \
        --enable-decoder=binkvideo,binkaudio_dct,binkaudio_rdft \
        --enable-swscale --enable-swresample

under MSYS2's mingw32 shell. Dropping the result in here needs no
change to the project file as long as the layout is the same. It has
not been done because it needs a toolchain this repository does not
carry, and the prebuilt one works.

Note that a build this narrow decodes **Bink and nothing else**. That
is the right trade for the two files the game ships, and the wrong
one if anybody ever wants to drop an `.mp4` in instead.

## Bink 1, not Bink 2

libav's Bink support is reverse-engineered and carries no RAD code.
It covers **Bink 1** — the `BIKb`, `BIKf`, `BIKg`, `BIKh` and `BIKi`
files a 2003 game has. **Bink 2 (`KB2`) is not supported** by any
version of libav. Nothing here needs it; it is written down so the
limit is known rather than discovered.
