#!/bin/sh
# The differential test for photo.c.
#
#   ./tests/run_photo.sh
#
# Standalone on purpose: tests/run.sh is where several passes would
# collide, so this script is the one to run until it is folded in.
# The frozen copy and the rewrite go into one binary with BASE_/NEW_
# prefixes, the real math.c is linked rather than mocked, and the C
# library's file calls are redirected into the harness so the lines
# IMG_Log4Debugging writes can be compared.

set -e
cd "$(dirname "$0")/.."

CC=${CC:-gcc}
CFLAGS="-std=gnu99 -fcommon -fms-extensions -O0 -g -w -Itests/stub -Itests"
OUT=tests/build
mkdir -p "$OUT"

syms="IMG_Log4Debugging IMG_LogOutput IMG_ModifierObjectAnalyser IMG_ObjectAnalyser \
IMG_Analyser IMG_ModifierObjectAnalyser_Bis IMG_ObjectAnalyser_Bis IMG_Analyser_Bis \
IMG_Log IMG_PhotoSummary IMG_Gao"

FILEDEFS="-Dfopen=PHMOCK_fopen -Dfwrite=PHMOCK_fwrite -Dfclose=PHMOCK_fclose"

defs() {
    prefix=$1
    shift
    for s in "$@"; do printf ' -D%s=%s%s' "$s" "$prefix" "$s"; done
}

echo "=== photo ==="
$CC $CFLAGS $FILEDEFS -c photo.c -o "$OUT/photo.new.o" $(defs NEW_ $syms)
$CC $CFLAGS $FILEDEFS -c tests/baseline_photo.c -o "$OUT/photo.base.o" $(defs BASE_ $syms)
$CC $CFLAGS -c math.c -o "$OUT/photo.math.o"
$CC $CFLAGS -c tests/photo_mocks.c -o "$OUT/photo.mocks.o"
$CC $CFLAGS -c tests/test_photo.c -o "$OUT/photo.drv.o"
$CC "$OUT/photo.new.o" "$OUT/photo.base.o" "$OUT/photo.math.o" \
    "$OUT/photo.mocks.o" "$OUT/photo.drv.o" -o "$OUT/photo" -lm
"$OUT/photo"
