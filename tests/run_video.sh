#!/bin/sh
# The unit test for video.c.
#
#   ./tests/run_video.sh
#
# This is the one test here that links a real third-party library
# rather than mocking it: video.c exists so that libav decodes the
# picture, and a mocked libav would check nothing at all.
#
# It needs the libav development files and the ffmpeg command line,
# which builds the two fixtures. Where either is missing the test
# says so and is skipped rather than failing -- it is not part of the
# engine's own toolchain, and a machine without FFmpeg installed
# should still be able to run everything else.
#
# Override the include and library paths when pkg-config cannot find
# them, which is how this runs against headers unpacked beside a
# distribution's shared libraries:
#
#   VIDEO_CFLAGS=-I/path/to/include VIDEO_LIBS="-L/path/to/lib -lavformat ..." \
#       ./tests/run_video.sh

cd "$(dirname "$0")/.."

CC=${CC:-gcc}
OUT=tests/build
mkdir -p "$OUT"

echo "=== video ==="

if [ -z "$VIDEO_CFLAGS" ] && [ -z "$VIDEO_LIBS" ]; then
    if pkg-config --exists libavformat libavcodec libavutil libswscale libswresample 2>/dev/null; then
        VIDEO_CFLAGS=$(pkg-config --cflags libavformat libavcodec libavutil libswscale libswresample)
        VIDEO_LIBS=$(pkg-config --libs libavformat libavcodec libavutil libswscale libswresample)
    else
        echo "  skipped: no libav development files (pkg-config found no libavformat)"
        exit 0
    fi
fi

if ! command -v ffmpeg >/dev/null 2>&1; then
    echo "  skipped: no ffmpeg command line to build the fixtures"
    exit 0
fi

CLIP="$OUT/video_clip.nut"
SILENT="$OUT/video_silent.nut"
ALPHA="$OUT/video_alpha.nut"

# A clip with sound: a solid colour, 10 fps, and a stereo ramp. NUT
# carries raw-ish streams without a seek table, which is what the
# reader path needs -- it is handed bytes in order and cannot seek.
#
# The sound is a 1000-sample sawtooth rather than a tone, because the
# test checks each sample against its own index: sample k must be
# -32768 + (k mod 1000) * 65.536. One sample dropped or repeated
# anywhere in the queue shifts the phase and every sample after it
# fails, which a tone cannot show.
if [ ! -f "$CLIP" ]; then
    ffmpeg -loglevel error -y \
        -f lavfi -i "color=c=0x2040C0:s=64x48:r=10:d=2" \
        -f lavfi -i "aevalsrc='mod(n\,1000)/500-1|mod(n\,1000)/500-1':s=44100:d=2" \
        -ac 2 -c:v ffv1 -pix_fmt yuv420p -c:a pcm_s16le "$CLIP" || {
            echo "  skipped: ffmpeg could not build the fixture"
            exit 0
        }
fi
if [ ! -f "$SILENT" ]; then
    ffmpeg -loglevel error -y \
        -f lavfi -i "color=c=0x2040C0:s=64x48:r=10:d=2" \
        -c:v ffv1 -pix_fmt yuv420p "$SILENT" || {
            echo "  skipped: ffmpeg could not build the fixture"
            exit 0
        }
fi
# and one with an alpha plane that is not opaque, so the withAlpha
# argument is told apart from a file that happens to be opaque
if [ ! -f "$ALPHA" ]; then
    ffmpeg -loglevel error -y \
        -f lavfi -i "color=c=0x2040C080:s=64x48:r=10:d=1,format=yuva420p" \
        -c:v ffv1 "$ALPHA" || {
            echo "  skipped: ffmpeg could not build the fixture"
            exit 0
        }
fi

BASE="-std=gnu11 -fcommon -fms-extensions -O0 -g -w -Itests/stub -Itests"

build () {
    suffix=$1
    extra=$2
    $CC $BASE $extra $VIDEO_CFLAGS -c video.c -o "$OUT/video$suffix.o" || return 1
    $CC $BASE $extra $VIDEO_CFLAGS -c tests/test_video.c \
        -o "$OUT/video$suffix.drv.o" || return 1
    $CC $extra "$OUT/video$suffix.o" "$OUT/video$suffix.drv.o" \
        -o "$OUT/video$suffix" $VIDEO_LIBS -lm || return 1
    return 0
}

build "" "" || exit 1
LD_LIBRARY_PATH="${VIDEO_LD_LIBRARY_PATH:-$LD_LIBRARY_PATH}" \
    "$OUT/video" "$CLIP" "$SILENT" "$ALPHA" || exit 1

# A second pass under AddressSanitizer. The PCM queue is a ring, and a
# ring that steps over its own end writes outside its allocation --
# which the checks above can miss, because the bytes it then reads
# back are stale sound rather than nonsense and can pass for the real
# thing. ASan sees the write itself.
if printf 'int main(void){return 0;}\n' > "$OUT/asanprobe.c" \
   && $CC -fsanitize=address "$OUT/asanprobe.c" -o "$OUT/asanprobe" 2>/dev/null; then
    build ".asan" "-fsanitize=address" || exit 1
    echo "  and again under AddressSanitizer"
    LD_LIBRARY_PATH="${VIDEO_LD_LIBRARY_PATH:-$LD_LIBRARY_PATH}" \
        "$OUT/video.asan" "$CLIP" "$SILENT" "$ALPHA" || exit 1
else
    echo "  (no AddressSanitizer in this compiler, so the overrun pass is skipped)"
fi
