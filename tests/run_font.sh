#!/bin/sh
# The unit test for font.c.
#
#   ./tests/run_font.sh
#
# New code with no earlier copy: the D3DX font this replaces had no
# harness and could not have had one, because every answer came from
# inside d3dx9. So this is a direct check rather than a differential
# one.
#
# The real FreeType is compiled in from thirdparty/freetype and the
# real faces are read from data/fonts -- the driver points the
# module's idea of the executable at the repository root so it finds
# them. Glyph shapes and metrics therefore come from the same files
# the game would read, which is the only way the arithmetic is
# checked against something other than itself.

set -e
cd "$(dirname "$0")/.."

CC=${CC:-gcc}
CFLAGS="-std=gnu11 -fcommon -fms-extensions -O0 -g -w -Itests/stub -Itests"
FT="thirdparty/freetype"
FTFLAGS="-DFT2_BUILD_LIBRARY -DFT_CONFIG_MODULES_H=<ftmodule.h> -I$FT/iriseng -I$FT/include"
OUT=tests/build
mkdir -p "$OUT" "$OUT/ft"

echo "=== font ==="

# FreeType's own sources, which the engine compiles the same way. Each
# is an amalgamation that includes its own siblings, so these thirteen
# are the whole library at this configuration.
FTSRC="$FT/src/base/ftsystem.c $FT/src/base/ftinit.c $FT/src/base/ftdebug.c \
$FT/src/base/ftbase.c $FT/src/base/ftbitmap.c $FT/src/base/ftglyph.c \
$FT/src/base/ftmm.c $FT/src/truetype/truetype.c $FT/src/sfnt/sfnt.c \
$FT/src/smooth/smooth.c $FT/src/autofit/autofit.c $FT/src/psnames/psnames.c \
$FT/src/gzip/ftgzip.c"

ftobjs=""
for one in $FTSRC
do
    obj="$OUT/ft/$(basename "$one" .c).o"
    # -w and no engine includes: this is upstream source and the
    # engine's headers have no business in it
    $CC -std=gnu11 -O1 -g -w $FTFLAGS -c "$one" -o "$obj"
    ftobjs="$ftobjs $obj"
done

# The path rule lives in datapath.c now, shared with the shader loader,
# so it is compiled in rather than being part of font.c. The cases that
# drive it -- a missing file, no path at all, a path too long -- still
# drive it through font.c, which is where it is used.
$CC $CFLAGS -c datapath.c -o "$OUT/font.datapath.o"
# shellcheck disable=SC2086
$CC $CFLAGS $FTFLAGS -c tests/test_font.c -o "$OUT/font.drv.o"
# shellcheck disable=SC2086
$CC "$OUT/font.drv.o" "$OUT/font.datapath.o" $ftobjs -o "$OUT/font" -lm
"$OUT/font"
