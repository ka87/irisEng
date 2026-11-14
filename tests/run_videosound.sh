#!/bin/sh
# The unit test for win/winvideosound.c.
#
#   ./tests/run_videosound.sh
#
# The sink cannot be run for real anywhere but Windows, so the device
# is a mock and the video stream is a stub. What is checked is the
# arithmetic: a play cursor running round a ring, how much is free,
# how much has been heard, and what of that was silence rather than
# video. libav is not linked -- the sink calls four functions of
# video.c and the stub supplies all four.

set -e
cd "$(dirname "$0")/.."

CC=${CC:-gcc}
CFLAGS="-std=gnu11 -fcommon -fms-extensions -O0 -g -w -Itests/stub -Itests -Ithirdparty/ffmpeg/include"
OUT=tests/build
mkdir -p "$OUT"

echo "=== video sound ==="
$CC $CFLAGS -c win/winvideosound.c -o "$OUT/videosound.o"
$CC $CFLAGS -c tests/test_videosound.c -o "$OUT/videosound.drv.o"
$CC "$OUT/videosound.o" "$OUT/videosound.drv.o" -o "$OUT/videosound" -lm
"$OUT/videosound"
