#!/bin/sh
# The AND-family list, grown past its first block.
#
#   ./tests/run_sndfamily.sh
#   ./tests/run_sndfamily.sh --asan
#
# Two faults in SND_pst_GetOrArray's growth, neither of which the
# sound differential can see, because the frozen copy it compares
# against has both: the room asked for is written folded in a form
# that is right only while tSoundFamilyStruct is 144 bytes, and the
# clear afterwards runs for that whole figure from the first new
# element rather than for the sixteen the list just gained. The second
# is wrong on a 32-bit target as well.
#
# Both write into heap the allocator has not handed out and nothing
# reads those bytes back, so --asan is the mode that sees them. The
# plain mode checks that the list still answers correctly while it
# grows, which is what says the growth is really being exercised.
#
# Built without SND_FAMILY_FROZEN on purpose: that gate is what keeps
# the differential's two copies comparable, and this is the check on
# the form that ships.

set -e
cd "$(dirname "$0")/.."

CC=${CC:-gcc}
SDL3_INC=${SDL3_INC:-thirdparty/SDL3/include}
CFLAGS="-std=gnu99 -fcommon -fms-extensions -O0 -g -w -I$SDL3_INC -Itests/stub -Itests"
OUT=tests/build
mkdir -p "$OUT"

SRCS="sound.c math.c basic_array.c handle.c tests/sound_mocks.c tests/test_sndfamily.c"

# shellcheck disable=SC2086
$CC $CFLAGS $SRCS -o "$OUT/sndfamily" -lm
"$OUT/sndfamily"

if [ "$1" = "--asan" ]; then
    echo "--- the same driver under AddressSanitizer ---"
    # shellcheck disable=SC2086
    $CC $CFLAGS -fsanitize=address $SRCS -o "$OUT/sndfamily_asan" -lm
    "$OUT/sndfamily_asan"
fi
