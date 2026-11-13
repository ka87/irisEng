#!/bin/sh
# The differential test for win/adpcm.c.
#
#   ./tests/run_adpcm.sh
#
# Standalone on purpose: tests/run.sh is where several passes would
# collide, so this script is the one to run until it is folded in.
#
# adpcm.c has no engine calls and no globals, so every symbol it
# defines is a function and all three get BASE_/NEW_ prefixes. The
# file-local helper and the two constant tables the rewrite adds are
# static, so they do not collide.

set -e
cd "$(dirname "$0")/.."

CC=${CC:-gcc}
CFLAGS="-std=gnu99 -fcommon -fms-extensions -O0 -g -w -Itests/stub -Itests"
OUT=tests/build
mkdir -p "$OUT"

syms="pcINO_ApplyADPCMAaptation pcSND_ADPCMToPCM_ConvertOneBlock \
pcSND_ADPCMToPCM_Convert"

defs() {
    prefix=$1
    shift
    for s in "$@"; do printf ' -D%s=%s%s' "$s" "$prefix" "$s"; done
}

echo "=== adpcm ==="
$CC $CFLAGS -c win/adpcm.c            -o "$OUT/adpcm.new.o"  $(defs NEW_ $syms)
$CC $CFLAGS -c tests/baseline_adpcm.c -o "$OUT/adpcm.base.o" $(defs BASE_ $syms)
$CC $CFLAGS -c tests/test_adpcm.c     -o "$OUT/adpcm.drv.o"
$CC "$OUT/adpcm.new.o" "$OUT/adpcm.base.o" "$OUT/adpcm.drv.o" -o "$OUT/adpcm" -lm
"$OUT/adpcm"
