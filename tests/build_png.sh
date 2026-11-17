#!/bin/sh
# Builds the vendored zlib and libpng into one archive the harnesses
# link against, and does nothing if the archive is already newer than
# the sources.
#
# Built once rather than per harness because three of them link the
# screenshot writer, and twenty-six objects compiled three times a run
# is most of a suite's time for no information.
set -e
cd "$(dirname "$0")/.."

CC=${CC:-gcc}
OUT=tests/build/pngobj
LIB=tests/build/libpng.a

newest=$(ls -t thirdparty/zlib/*.c thirdparty/zlib/*.h \
            thirdparty/libpng/*.c thirdparty/libpng/*.h 2>/dev/null | head -1)
if [ -f "$LIB" ] && [ -z "$(find "$newest" -newer "$LIB" 2>/dev/null)" ]; then
    exit 0
fi

mkdir -p "$OUT"
# -w because these are not our sources and their warnings are not ours
# to fix; -O1 because a screenshot's compression is not what a test is
# waiting for.
for src in thirdparty/zlib/*.c thirdparty/libpng/*.c; do
    obj="$OUT/$(basename "$src" .c).o"
    $CC -std=gnu11 -O1 -w -Ithirdparty/zlib -Ithirdparty/libpng \
        -c "$src" -o "$obj"
done
rm -f "$LIB"
ar rcs "$LIB" "$OUT"/*.o
