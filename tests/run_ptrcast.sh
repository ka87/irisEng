#!/bin/sh
# A ratchet on casts between a pointer and an integer of another size.
#
#   ./tests/run_ptrcast.sh
#
# The port's work so far has been to stop keeping addresses in 32-bit
# slots, and the thing that was missing throughout was anything to stop
# one coming back. A whole-build -Werror cannot go on while files remain
# that legitimately still do it, so this does it a file at a time:
#
#   tests/ptrcast-clean.txt lists the built sources that compile at
#   -m64 with no such cast. Each is compiled again here with those two
#   warnings as errors, so a new one is a build failure and not a line
#   in a report nobody runs.
#
# And it ratchets the other way too. Every built source that is NOT on
# the list is compiled as well and has to still warn. A file that has
# been converted stops warning, which fails here and says to add it --
# so the list cannot quietly fall behind the work, which is what would
# otherwise happen the first time someone converted a file and did not
# think to come here.
#
# A missed site is exactly what this would have caught: world.c kept
# registering a world by its truncated address for one commit, because
# the delete sites were converted by a sweep and the two registrations
# by hand.

set -e
cd "$(dirname "$0")/.."

CC=${CC:-gcc}
mkdir -p tests/build
# per-run, so two runs cannot read each other's diagnostics
ERR=tests/build/ptrcast.err
INC="-Itests/stub -Itests -Ithirdparty/SDL3/include -Ithirdparty/freetype/include -Ithirdparty/ffmpeg/x32/include"
CFLAGS="-m64 -std=gnu99 -fcommon -fms-extensions -fsyntax-only $INC"
# -Wint-conversion is the third of these and was missing, which left
# the ratchet blind to the fault it exists for whenever the source
# has no cast in it. `slot = ptr;` with slot an int compiles, warns
# only under -Wint-conversion, and says "makes integer from pointer
# without a cast" rather than "of different size" -- so neither the
# warning nor the grep below saw it, and a pointer truncated into a
# four-byte slot passed this file.
WARN="-Wint-to-pointer-cast -Wpointer-to-int-cast -Wint-conversion"
# What any of the three says. Matched in both directions below.
BAD="of different size|makes integer from pointer|makes pointer from integer"
LIST=tests/ptrcast-clean.txt

# The sources the build actually compiles, which is the project file's
# business and not a glob over the tree.
built=$(python3 - <<'PY'
import re
t = open('irisEng.vcxproj', encoding='utf-8-sig').read()
for m in re.findall(r'ClCompile Include="([^"]+\.c)"', t):
    m = m.replace('\\', '/')
    if not m.startswith('thirdparty/'):
        print(m)
PY
)

echo "=== files that must stay free of pointer-to-integer casts ==="

missing=''
regressed=''
clean=0
for f in $(cat "$LIST"); do
    if [ ! -f "$f" ]; then
        missing="$missing $f"
        continue
    fi
    if LC_ALL=C $CC $CFLAGS $WARN -Werror=int-to-pointer-cast \
            -Werror=pointer-to-int-cast -Werror=int-conversion \
            "$f" 2>"$ERR"; then
        clean=$((clean + 1))
    else
        regressed="$regressed $f"
        echo "  FAIL $f now converts between a pointer and a narrower integer:"
        grep -E "$BAD" "$ERR" | sed 's/^/      /' | head -4
    fi
done
echo "  $clean file(s) clean"

if [ -n "$missing" ]; then
    echo "  FAIL these are on the list and not in the build any more:"
    for f in $missing; do echo "      $f"; done
fi

echo "=== files that still have them, which the list must reflect ==="
newlyClean=''
dirty=0
for f in $built; do
    if grep -qxF "$f" "$LIST"; then
        continue
    fi
    n=$(LC_ALL=C $CC $CFLAGS $WARN "$f" 2>&1 | grep -cE "$BAD" || true)
    if [ "$n" -eq 0 ]; then
        newlyClean="$newlyClean $f"
    else
        dirty=$((dirty + 1))
    fi
done
echo "  $dirty file(s) still to convert"

if [ -n "$newlyClean" ]; then
    echo "  FAIL these no longer have any, so they belong on the list:"
    for f in $newlyClean; do echo "      $f"; done
    echo
    echo "  Add them to $LIST, keeping it sorted. The list is what stops"
    echo "  the work being undone, so it is only useful while it is current."
fi

if [ -n "$regressed" ] || [ -n "$missing" ] || [ -n "$newlyClean" ]; then
    exit 1
fi
