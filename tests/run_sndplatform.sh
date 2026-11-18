#!/bin/sh
# The audio device and the voices over it, against SDL's dummy audio
# driver.
#
#   ./tests/run_sndplatform.sh
#
# sndplatform.c is included by the test rather than linked, so a
# voice's cursor and buffer can be read back after a feed. SDL is
# real: a real device, real streams bound to it, real conversion. The
# dummy driver consumes what it is given and plays nothing, which is
# what makes this a test of what reaches the stream.
set -e
cd "$(dirname "$0")/.."

CC=${CC:-gcc}
OUT=tests/build
mkdir -p "$OUT"

echo "=== sndplatform ==="
. ./tests/sdl3.sh

$CC -g -O0 -fsanitize=address,undefined -std=gnu11 -fms-extensions \
    -I"$SDL3_INC" -Itests/stub \
    tests/test_sndplatform.c -o "$OUT/sndplatform" \
    "$SDL3_LIB" -lm -lpthread -ldl
"$OUT/sndplatform"
