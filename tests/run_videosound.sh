#!/bin/sh
# The unit test for platform/sndvideo.c.
#
#   ./tests/run_videosound.sh
#
# The platform layer is a mock and the video stream is a stub, because
# what is checked is the arithmetic: a play cursor running round a
# ring, how much is free, how much has been heard, and what of that
# was silence rather than video. The test has to move the cursor
# itself, which a real device will not let it do; sndplatform.c is run
# for real against the library by tests/run_sndplatform.sh. libav is
# not linked -- the sink calls four functions of video.c and the stub
# supplies all four.

set -e
cd "$(dirname "$0")/.."

CC=${CC:-gcc}
# MEM_PTR_BYTES=4: an array of pointers takes four bytes an element in
# the 32-bit build the engine ships as, and the frozen copies here
# write that out. Five of them are compared on the figure the
# allocator was asked for -- the mock logs it -- so the rewrite has
# to ask for the same thing, and the mock's own double headroom
# covers the shortfall. The game build gets the real width.
CFLAGS="-std=gnu11 -fcommon -fms-extensions -O0 -g -w -Itests/stub -Itests -Ithirdparty/ffmpeg/x32/include -DMEM_PTR_BYTES=4"
OUT=tests/build
mkdir -p "$OUT"

echo "=== video sound ==="
$CC $CFLAGS -c platform/sndvideo.c -o "$OUT/videosound.o"
$CC $CFLAGS -c tests/test_videosound.c -o "$OUT/videosound.drv.o"
$CC "$OUT/videosound.o" "$OUT/videosound.drv.o" -o "$OUT/videosound" -lm
"$OUT/videosound"
