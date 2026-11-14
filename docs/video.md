# Video through libav

The engine played video with the Bink SDK: `binkw32.dll` and RAD's
`bink.h`, opened either from a file next to the exe or through a set
of custom I/O hooks that pulled a video out of the `.bf`.

`video.c` replaces the decoding half of that with libav (FFmpeg).
This file is the first of two steps: **the module and its test, with
nothing rewired yet.** The call sites still use Bink. Wiring them up,
and the DirectSound sink that the full-screen video needs, follow
separately — see "What is not here yet".

## 32-bit x86

It is the mainline case rather than an afterthought: the project's
primary platform is `Win32`, and i686 is a first-class FFmpeg target
with hand-written MMX and SSE paths.

**The build is in the repository**, under `thirdparty/ffmpeg`:
headers, MSVC import libraries and the five runtime DLLs of
`ffmpeg-n9.0.2-48-gd4070a14ce-win32-lgpl-shared-9.0`. The two `Win32`
configurations link it and copy the DLLs next to the exe after a
build; `video.c` is excluded from the two `x64` configurations,
because a 32-bit import library has nothing for an x64 link to
resolve against. `thirdparty/ffmpeg/README.md` has the details,
including the LGPL position and how to put a much smaller build in
its place.

**Bink 1 only.** libav's `bink` demuxer and its `binkvideo`,
`binkaudio_dct` and `binkaudio_rdft` decoders were reverse-engineered
and carry no RAD code, but **Bink 2 (`BK2`/`KB2`) is not supported**.
Bink 2 is from 2013 and the game is from 2003, so the game's files are
Bink 1; the first four bytes of a `.bik` say which (`BIKb`, `BIKi` and
so on are Bink 1).

## What the module is

A small API shaped by what the engine's two call sites already do,
which is the same four steps per frame the Bink API had:

| Bink | here |
|---|---|
| `BinkWait` | `VID_b_Waiting` |
| `BinkDoFrame` | `VID_b_DecodeFrame` |
| `BinkCopyToBuffer` | `VID_CopyToBuffer` |
| `BinkNextFrame` | `VID_NextFrame` |
| `BinkOpen` | `VID_pst_OpenFile` |
| `BinkSetIO` + five callbacks | `VID_pst_OpenReader`, one callback |
| `BinkSetSoundSystem`, `BinkOpenDirectSound` | *(nothing — see below)* |

Minimum libav is **5.1**, for `AVChannelLayout`.

### Reading from the .bf gets much smaller

Bink's `BINKIO` wanted a pre-buffered, push-style reader with a fixed
2048-byte I/O size and no threading, and `proceduraltextures.c` built
a whole apparatus to satisfy it: a linked list of 4 KB blocks, a
ten-slot bitmask, a `stateMachine2MeansBusy` flag per block, a
spin-wait, and a file-scope `BINK_IF_PTR_GLOBAL` because the six
callbacks have no user pointer.

libav's `avio_alloc_context` takes one callback, `read(user, buf,
size)`, pulls what it needs, and carries the user pointer itself.
When the call sites are rewired, all of that apparatus goes, along
with the global.

### Sound

libav decodes the sound track to 16-bit interleaved PCM and queues it;
it does not own an audio device, where Bink did. `VID_i_SoundRead`
hands PCM to whatever sink the platform has, and the sink reports back
with `VID_SoundPlayed`, which is what the frame clock then runs on —
the same thing Bink did internally when it had the sound. With no sink
the clock is the wall clock.

The queue is a ring, and **it drops the newest sound when it is full**
rather than stalling the decoder. A sink that stops asking for sound
therefore gets a gap rather than a frozen picture, which is the right
way round; it does mean a stalled sink is audible.

### Two decisions worth knowing

**The alpha byte is written, always.** `AV_PIX_FMT_BGR0` looked like
the cheaper choice for the no-alpha case, since the engine's surfaces
are already cleared — but libav documents that format's fourth byte as
*undefined*, and relying on what it happens to do today is how this
breaks silently in a future version. The scaler always targets BGRA
and the opaque byte is written explicitly where it can be relied on.
The first version of the test asserted the byte was left alone and
failed on the first run, which is how this was found.

**Reads are rounded down to whole sample frames.** A sink handed half
a stereo frame would have the channels the wrong way round for the
rest of the stream. This too came out of the test: a read size of
1006 bytes returned half a frame and shifted everything after it.

**libav allocates with its own allocator.** Bink's did not: the engine
installed `MEM_p_Alloc`/`MEM_Free` through `BinkSetMemory`, so video
memory went through the engine's accounting. libav has no equivalent
hook, so that accounting loses sight of the decoder's working set.

### It is outside the stub-only parse sweep, deliberately

The rest of the tree parses against `tests/stub/`, a hand-written
stand-in for `windows.h`, `d3d9.h` and the rest. `video.c` does not
and should not: a stubbed libav would let a wrong API call through
unnoticed, which is the one thing worth catching in a file whose
whole job is to call that API correctly. It is compiled against the
real headers by its own test instead, and a machine without them
simply skips it.

## The test

`tests/run_video.sh` builds `tests/test_video.c`, which is the only
test here that **links a real third-party library instead of mocking
it**. That is deliberate: the module exists so that libav decodes the
picture, and a mocked libav would check nothing at all. Where the
libav development files or the `ffmpeg` command line are missing, the
test says so and is skipped rather than failing — FFmpeg is not part
of the engine's own toolchain.

Before any of that it does a **compile-only pass against the headers
in `thirdparty/ffmpeg`**, which are what the engine builds against
and are usually a different version from whatever shared libraries
the machine running the test has. They cannot be linked here -- a
version mismatch shows up as every open failing rather than as a link
error -- but compiling against them catches the thing worth catching,
which is a call that is right for one version of libav and not the
other. It found one: `av_gettime_relative` needs
`<libavutil/time.h>`, which 6.1 pulled in transitively and 9.0 does
not.

69 checks over opening, both input paths, the pixel format, pitch and
clipping, the end of the stream, broken input, the sound track and the
frame clock. The fixtures are built by `ffmpeg` from `lavfi`, so they
need nothing checked in.

**The strongest check is not any expected value.** It is that the file
path and the reader path, handed the same bytes, produce byte-identical
pixels — no magic numbers, and it catches most of what can go wrong in
between.

**The sound fixture is a sawtooth, not a tone.** Every sample is
checked against its own index: sample *k* must be
`-32768 + (k mod 1000) * 65.536`. One sample dropped, repeated, or
read from the wrong side of the ring's wrap shifts the phase and
everything after it fails. A continuity check against the previous
sample — which is what this started as — misses a whole period going
astray, and did.

**And it runs a second time under AddressSanitizer.** A ring that
steps over its own end writes outside its allocation, and the checks
above can miss that: the bytes it then reads back are stale sound
rather than nonsense, and stale sound can pass for the real thing.
ASan sees the write itself.

### What the mutation check found

Eleven defects were injected one at a time; ten were caught:

| mutation | what caught it |
|---|---|
| BGRA swapped for RGBA | byte 0 is blue |
| the destHeight clamp dropped | nothing is written below the rows asked for |
| alpha forced on always | the file's own plane comes through |
| `VID_NextFrame` does not advance the clock | with no sound played the next frame waits |
| `wantSound` ignored | sound is not decoded when it was not asked for |
| the ring's read does not wrap | every sample comes out where it belongs |
| the ring's write does not wrap | a heap overrun, under ASan |
| whole-frame rounding dropped | every sample comes out where it belongs |
| the read clamped to the wrong amount | the test hangs |
| EOF reported as an I/O error | **nothing** — see below |
| the ring's overflow clamp dropped | **nothing** — see below |

Two are worth writing down rather than quietly leaving.

**EOF against I/O error at the end of the reader's data** is not
observable: avformat treats both as the end of the stream, the same
frames come out, and the stream ends the same way. The distinction is
kept in the code because it is the correct thing to report, not
because anything here can tell.

**The overflow clamp** — what stops a write running over sound that
is still queued when the sink has stalled — is not covered, and an
attempt is recorded in the test so it is not repeated. A partially
drained reader does make the clamp fire on almost every call, but the
queue drops the tail of each chunk it cannot fit, so the sound that
comes out is discontinuous *by design*: a check for steps the ramp
cannot make reports four of them on correct code. It measures the drop
policy, not the clamp. Telling the two apart needs the queue to track
where each byte came from, which is more machinery than the forty
lines it would be guarding.

## The sound: win/winvideosound.c

Bink owned an audio device — the engine handed it an `IDirectSound8`
through `BinkSetSoundSystem` and Bink ran the buffer, the mixing and
the clock. libav decodes and nothing else, so a streaming secondary
buffer lives here, fed once a frame from the stream's PCM queue.

It is also where the picture's clock comes from. Every `WVS_Update`
reads the play cursor and tells the stream how much has been heard,
and the frame pacing then follows the sound rather than the wall,
which is what Bink did internally.

Three things in it are worth knowing, because each was a defect
first:

**A full ring and an empty one look identical** from the two
positions alone — write and play are equal in both cases — so the
amount free is taken from a cumulative count of bytes written against
bytes played instead. The first draft derived it from the positions
and never primed the buffer at all: the first update computed zero
free and returned.

**The clock is not how far the cursor has run.** Some of what it ran
over is silence: poured in to cover a gap, or left over from the
clear at open. Counting that puts the clock ahead of the picture by
the length of the gap. Since the buffer plays in the order it was
written, the sound heard is the smaller of what has been played and
what real sound went in. The first draft discounted only the silence
it had written itself, and was wrong by however much of the opening
clear the cursor had crossed.

**A gap is filled with silence rather than left alone.** Stalling the
buffer would freeze the picture too, since the picture's clock is the
cursor; leaving the bytes alone would replay the previous lap, which
is heard as a stutter.

It cannot be run anywhere but Windows, so `tests/test_videosound.c`
drives it against a mock device whose play cursor the test moves
itself, and a stub video stream handing over a counted ramp. 96
checks, and eight injected defects each caught — including the two
above and the one that mattered most, a write position slipped off a
sample-frame boundary by an unaligned cursor, which would swap the
channels for the rest of the stream.

## The two call sites

**`ai/ai_gfxfunctions.c`**, the full-screen video, is the smaller
change: `VID_pst_OpenFile` in place of `BinkOpen`, the accessors in
place of the `HBINK` struct fields, and `WVS_Update` once a frame
before the pacing, so the clock is set even on the frames where
nothing is decoded.

**`proceduraltextures.c`** is where the apparatus goes. What stood
there to satisfy Bink's push-style I/O — a linked list of 4 KB
blocks, a ten-slot bitmask, a busy flag per block, a spin-wait on it,
an allocator pair installed through `BinkSetMemory`, six I/O
callbacks and the file-scope `BINK_IF_PTR_GLOBAL` they reached the
stream through, because the callbacks carried no pointer of their own
— is replaced by one function:

```c
int TEXPRO_i_ReadFromBF(void* user, uint8_t* buf, int size)
```

`tBinkIFPtr` loses six of its ten fields with it, and
`tBinkUnkStruct` goes entirely.

The frame pacing keeps the shape it had: a texture that is up to date
shows nothing new, one that has fallen behind decodes on without
showing what it has, up to three frames, and one that runs off the
end reopens the stream, which is what makes a video texture loop.
`VID_b_NextAlsoDue` exists for that middle case — Bink could be asked
whether the next frame was due *after* advancing, and here advancing
releases the picture, so the question has to be asked before.

### What it cost the procedural path's differential test

Its frozen copy calls Bink, so the video cases could not be carried
over: there is nothing on the rewrite's side to compare a slot mask
or a block ring against. `slotCases`, `binkRingCases`, `binkSdkCases`
and the two MPEG groups are **removed** rather than left comparing a
frozen copy with itself, and the new path is driven directly instead
— the same treatment the overlay's F12 edge got.

What replaces them: the reader (where it reads, how much, and that it
stops at the end of the `.bf` member rather than running on into
whatever follows), opening (a missing key, the empty 8-byte member
the archive uses for a video that was never built, and a real one),
and the pacing, with libav mocked the way the Bink SDK was. 136 cases
in that harness, eight injected defects each caught.

`tests/stub/bink.h` stays, and now says why: it exists only so the
frozen copies still compile. `tBinkIFPtr` and `tBinkUnkStruct` moved
into it from `textures.h`, and the two fields the frozen copy names
differently are mapped with `-D` rather than edited, which is the
rule the other harnesses already follow.

## What is not here yet
## What is not here yet

- **A real Bink file has not been decoded end to end.** Everything
  here is checked against fixtures `ffmpeg` builds and against
  mocks; the one thing no test can stand in for is the game's own
  `.bik` files going through libav's Bink decoder on Windows. That is
  what the first build is for.
- **`BINK_SeekAndReadFromBF` keeps its name** in `bigfiles.c`,
  although nothing Bink is left. It is a plain seek-and-read on the
  archive; renaming it means touching a frozen baseline's symbol
  list, which is not worth bundling into this change.
- **The making-of video's sound has not been heard.** The sink is
  checked arithmetically and nothing more.
- **A smaller libav.** The build committed here is 90 MB of DLLs
  because a default FFmpeg carries every codec; one configured for
  Bink alone is a few hundred KB. The configure line is in
  `thirdparty/ffmpeg/README.md`; it needs an MSYS2 toolchain.
