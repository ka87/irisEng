# libav (FFmpeg), 64-bit Windows, shared, LGPL — not committed

This directory is the x64 half of the split and is **empty of
binaries**. The two `x64` configurations of `irisEng.vcxproj` point
here, so an x64 build needs the files dropped in before it will link.

## What goes here

The same five libraries the 32-bit tree carries — **avcodec**,
**avformat**, **avutil**, **swscale**, **swresample** — from a
**win64, LGPL, shared** build, laid out exactly as `../x32`:

    x64/include/libavcodec/...      the public headers
    x64/include/libavformat/...
    x64/include/libavutil/...
    x64/include/libswscale/...
    x64/include/libswresample/...
    x64/lib/avcodec.lib             MSVC import libraries
    x64/lib/avformat.lib
    x64/lib/avutil.lib
    x64/lib/swscale.lib
    x64/lib/swresample.lib
    x64/bin/avcodec-*.dll           the runtime DLLs
    x64/bin/avformat-*.dll
    x64/bin/avutil-*.dll
    x64/bin/swscale-*.dll
    x64/bin/swresample-*.dll
    x64/LICENSE.txt                 the build's own licence text

`include/`, `lib/` and `bin/` have to come from **one** build. A
header that does not match its DLL is the one way this goes wrong
quietly, and it goes wrong at run time rather than at the link.

## Which build

The 32-bit tree is

    ffmpeg-n9.0.2-48-gd4070a14ce-win32-lgpl-shared-9.0

so the matching one is the `win64-lgpl-shared` archive of the same
version. The two trees do **not** have to be the same version to work
— each is self-consistent — but keeping them matched is what stops the
two platforms decoding differently.

`avdevice` and `avfilter` are not wanted: nothing in these five
imports them, and they are another 36 MB.

## Or build a narrow one

The 32-bit README's `configure` line applies here with `--arch=x86_64
--target-os=mingw64` instead, under MSYS2's mingw64 shell. A build
with only Bink enabled is a few hundred kilobytes against 90 MB,
decodes Bink and nothing else, and needs no change to the project
file as long as the layout above is kept.

## Until then

`video.c` and `platform/sndvideo.c` are **in** the x64 build now, so
the configuration is complete and the only thing missing is this
directory's contents. The project checks for `lib/avcodec.lib` before
compiling and stops with a message naming this file, rather than
letting it surface as five unresolved externals at the link.
