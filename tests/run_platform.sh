#!/bin/sh
# The window and the event translation, against SDL's dummy video
# driver.
#
#   ./tests/run_platform.sh
#
# platform.c is included by the test rather than linked, so the window
# it owns can be read back after a pump. The engine's flags and the few
# engine calls it makes are stood in for in the test; everything SDL is
# real.
set -e
cd "$(dirname "$0")/.."

CC=${CC:-gcc}
OUT=tests/build
mkdir -p "$OUT"

echo "=== platform ==="
. ./tests/sdl3.sh

$CC -g -O0 -fsanitize=address,undefined -std=gnu11 -fms-extensions \
    -I"$SDL3_INC" -Itests/stub \
    tests/test_platform.c -o "$OUT/platform" \
    "$SDL3_LIB" -lm -lpthread -ldl
"$OUT/platform"
