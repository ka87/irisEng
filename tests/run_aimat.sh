#!/bin/sh
# The AI material and colour builtins, across the four-byte AI stack.
#
#   ./tests/run_aimat.sh
#
# Nothing in ai/ had a test of any kind before this -- no frozen copy
# of any of those files and no runner building one.
#
# Two things are under test. The colour builtins write a colour through
# the address MAT_GetColorPtr hands back, and MATSetAmbient declared
# that address an int** where its own getter has always had int*: an
# eight-byte write into the four-byte ambient field, with the diffuse
# colour directly after it, so on a wide build setting the ambient
# colour destroyed the diffuse one. And the builtins that hand a script
# a reference to something the engine owns have to put a handle in the
# stack slot rather than a truncated address.
#
# Built with the handle layer in its real form, not the identity: there
# is no frozen copy here to stay byte-identical with, so a handle is a
# small number that is nothing like the address it stands for and the
# driver checks exactly that.

set -e
cd "$(dirname "$0")/.."

CC=${CC:-gcc}
CFLAGS="-std=gnu99 -fcommon -fms-extensions -O0 -g -w -Itests/stub -Itests"
OUT=tests/build
mkdir -p "$OUT"

$CC $CFLAGS -c ai/ai_materialandcolorfunctions.c -o "$OUT/aimat.sut.o"
$CC $CFLAGS -c handle.c                          -o "$OUT/aimat.hdl.o"
$CC $CFLAGS -c tests/aimat_mocks.c               -o "$OUT/aimat.mocks.o"
$CC $CFLAGS -c tests/test_aimat.c                -o "$OUT/aimat.drv.o"
$CC "$OUT/aimat.sut.o" "$OUT/aimat.hdl.o" "$OUT/aimat.mocks.o" \
    "$OUT/aimat.drv.o" -o "$OUT/aimat" -lm

"$OUT/aimat"
