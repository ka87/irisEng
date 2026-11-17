#!/bin/sh
# The vendored SDL3 on its own, before anything in the engine uses it.
#
#   ./tests/run_sdl3.sh
#
# Builds the vendored source once (tests/sdl3.sh) and runs the checks
# against SDL's dummy drivers. Nothing of the engine is linked in: this
# is about the dependency, not about the engine.
set -e
cd "$(dirname "$0")/.."

CC=${CC:-gcc}
OUT=tests/build
mkdir -p "$OUT"

echo "=== SDL3 ==="
. ./tests/sdl3.sh

$CC -g -O0 -fsanitize=address,undefined -std=gnu11 \
    -I"$SDL3_INC" tests/test_sdl3.c -o "$OUT/sdl3probe" \
    "$SDL3_LIB" -lm -lpthread -ldl
"$OUT/sdl3probe"
