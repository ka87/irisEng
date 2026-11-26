# libav (FFmpeg), 64-bit Windows, shared, LGPL

The x64 half of the split. What `video.c` links against in the two
`x64` configurations, as `../x32` is for the two `Win32` ones.

    ffmpeg-n9.0.2-982-g836df4e858-20261126
    from the n9.0-latest build of BtbN/FFmpeg-Builds

## What is here

The same five libraries as `../x32`, file for file -- **avcodec**,
**avformat**, **avutil**, **swscale**, **swresample**:

| | |
|---|---|
| `include/` | the public headers of the five |
| `lib/*.lib` | MSVC import libraries |
| `lib/*.dll.a` | the same for a MinGW build, unused by the project |
| `bin/*.dll` | the five runtime DLLs, copied next to the exe after a build |
| `LICENSE.txt` | the build's own licence text |

`avdevice` and `avfilter` ship in the same zip and are not here, as in
`../x32`: nothing in these five imports them and they are another
36 MB. Neither are the `.def` files, the `pkgconfig` directory or the
three executables.

All five DLLs report machine `0x8664`.

## It is a different commit from x32, and the same ABI

`../x32` is `n9.0.2-48-gd4070a14ce-20261107` and this is
`n9.0.2-982-g836df4e858-20261126`. The pinned 32-bit build is not in
BtbN's `latest` release any more, so the two halves are the 9.0 branch
at different points.

That is safe and was checked rather than assumed:

- **the sonames are identical** -- `avcodec-63`, `avformat-63`,
  `avutil-61`, `swresample-7`, `swscale-10` -- so the major and minor
  versions agree and the ABI is the same;
- **the API is identical.** Of the five libraries' headers, eight files
  differ between the trees. Five are a `VERSION_MICRO` of 102 against
  103, one is the `FFMPEG_VERSION` string, and the other two --
  `avcodec.h` and `avformat.h` -- differ **only in documentation
  comments**: a note about parser calls after EOF and two about
  `AV_INPUT_BUFFER_PADDING_SIZE`. No declaration, type or constant
  differs.

`tests/run_ffmpegtrees.sh` checks the first of those on every run, so
a later swap that breaks it fails rather than being discovered when the
two platforms decode differently.

Replacing this half means replacing `include/`, `lib/` and `bin/`
**together**: a header that does not match its DLL goes wrong at run
time rather than at the link.
