#!/bin/sh
# The two MSVC warnings this tree keeps producing, caught here instead
# of in the user's build log.
#
#   ./tests/warncheck.sh
#
# The game is built at /W3, where these two fire:
#
#   C4101  unreferenced local variable
#   C4244  conversion from 'double' to 'float', possible loss of data
#
# And one that is not MSVC's, added because it cost three runs of the
# game to find by hand:
#
#   -Wundef  an identifier that is not defined, used in a #if
#
# An undefined identifier is nought in a #if, so a `#if FEATURE`
# written above `#define FEATURE` compiles out whichever value the
# define carries, and nothing says so. win/winsound.c had seven of
# them above its trace switch and two below it; the two printed, the
# seven did not, and the silence was read three times over as the code
# not reaching those points. A trace that cannot print is worse than
# no trace, because it is evidence of the wrong thing.
#
# Required to be zero like the other two. The only exclusion is by
# file: minilzo's own headers test a long list of undefined platform
# macros on purpose, and they are upstream source, which this sweep
# already leaves alone -- but bigfiles.c includes them, so they are
# dropped by the path they are reported against rather than by the
# file being swept.
#
# gcc has an equivalent for each -- -Wunused-variable and
# -Wfloat-conversion -- and both have now been reported by hand more
# than once, which is what this exists to stop. Four locals in
# Rasters_Update and one narrowed scale were the last batch; every one
# of them was a leftover from code that had moved into perfbars.c.
#
# Both are required to be **zero**, with no list of exemptions. A
# warning of either kind is either dead code to delete or a narrowing
# to make explicit, and neither is worth carrying.
#
# Two things this deliberately does not check:
#
#   - **set but not used** (-Wunused-but-set-variable, MSVC's C4189).
#     That is a /W4 warning and the build is /W3, and removing one
#     means removing its assignment, which in decompiled engine code
#     may be removing a side effect. Five of them are in the tree:
#     gaoUltra in three ai/ files, v12 in ai_inofunctions.c and
#     currVecIdx in gfxexplode.c.
#   - **long double to double** (-Wfloat-conversion's other half).
#     MSVC cannot produce it, because long double is double there, so
#     all sixteen in the tree are an artefact of being compiled here.
#     They are filtered out below rather than listed.
#
# The flags carry no -w, and must not: -w disables these the way it
# disables -Werror=implicit-function-declaration, which is the hazard
# tests/implicitcheck.sh was written for. The first run of this sweep
# had -w in it and reported a clean tree with the reported warning
# still in the file.

set -e
cd "$(dirname "$0")/.."

CC=${CC:-gcc}
# The float check runs under clang where there is one: gcc is silent
# when the value converts exactly, and the game's compiler is not.
# See the note at the top of this file.
FCC=${FCC:-}
if [ -z "$FCC" ]; then
    if command -v clang >/dev/null 2>&1; then FCC="clang"
    elif command -v clang++ >/dev/null 2>&1; then FCC="clang++ -x c"
    else FCC="$CC"; fi
fi
CFLAGS="-fsyntax-only -std=gnu11 -fms-extensions -Itests/stub"
CFLAGS="$CFLAGS -Ithirdparty/ffmpeg/include"
CFLAGS="$CFLAGS -Ithirdparty/SDL3/include"
CFLAGS="$CFLAGS -Ithirdparty/freetype/include -Ithirdparty/freetype/iriseng"

# loading.c cannot be compiled here at all -- the fpos_t question in
# tests/implicitcheck.sh -- and a file that does not compile reports no
# warnings, so it is named rather than silently counting zero.
UNSWEEPABLE="loading.c"

echo "=== compiler warnings ==="

files=$(python3 - <<'PY'
import re

raw = open('irisEng.vcxproj', encoding='latin-1').read()
for m in re.finditer(r'ClCompile Include="([^"]+\.c)"', raw):
    p = m.group(1).replace('\\', '/')
    # upstream source is not ours to tidy, and the vendored libraries
    # are built with warnings off in the project too
    if 'minilzo' in p or p.startswith('thirdparty/'):
        continue
    print(p)
PY
)

swept=0
found=0
for f in $files; do
    skip=0
    for u in $UNSWEEPABLE; do
        [ "$f" = "$u" ] && skip=1
    done
    if [ "$skip" = "1" ]; then
        echo "  skip  $f cannot be compiled here, known"
        continue
    fi
    swept=$((swept + 1))
    out=$($CC $CFLAGS -Wunused-variable "$f" 2>&1 \
          | grep -E '\[-Wunused-variable\]' || true)
    # See the note at the top: upstream headers test undefined macros
    # on purpose, so they go by the path they are reported against.
    uout=$($CC $CFLAGS -Wundef "$f" 2>&1 \
           | grep -E '\[-Wundef\]' \
           | grep -v '^minilzo/' \
           | grep -v '/minilzo/' || true)
    [ -n "$uout" ] && out="$out${out:+
}$uout"
    # Two kinds are dropped: 'long double', which cannot arise where
    # the game is built because the type is double there, and _Bool,
    # which is a float used as a truth value in a condition and is not
    # a narrowing the game's compiler reports at all. Two in
    # collision.c are of that second kind.
    fout=$($FCC $CFLAGS -Wfloat-conversion "$f" 2>&1 \
           | grep -E '\[-Wfloat-conversion\]' \
           | grep -v "'long double'" \
           | grep -v "to '_Bool'" || true)
    [ -n "$fout" ] && out="$out${out:+
}$fout"
    if [ -n "$out" ]; then
        echo "$out" | sed 's/^/  FAIL /'
        found=$((found + $(echo "$out" | wc -l)))
    fi
done

if [ "$found" != "0" ] ; then
    echo
    echo "  $found warning(s): an unreferenced local to delete, a"
    echo "  narrowing to write out, or a #if above the define it reads"
    exit 1
fi
echo "  no unreferenced locals, no implicit double-to-float and no" \
     "undefined identifier in a #if ($swept files)"
