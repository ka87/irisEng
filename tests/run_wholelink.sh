#!/bin/sh
# Every engine source, compiled and put in one link.
#
#   ./tests/run_wholelink.sh
#
# The suite beside this compiles each file on its own and links small
# groups of them -- a differential pulls in the four or five sources it
# needs and no more. So two things nothing here could see:
#
#   - a symbol one file defines and another defines differently. The
#     engine builds with -fcommon and keeps a great many globals in its
#     headers, so a tentative definition in two places merges quietly;
#     a *strong* definition in two places does not, and nothing was
#     looking.
#   - a function the engine calls and no engine file defines. A
#     per-file syntax check is happy with a declaration, and a runner
#     that does not link that path never finds out.
#
# This compiles all of them and relocatably links the lot, which
# answers both: ld refuses a duplicate definition, and what is left
# unresolved is either genuinely outside the engine or a hole.
#
# tests/wholelink-external.txt is the list of names that really do come
# from somewhere else -- the Windows API, libm, SDL, OpenGL, FreeType,
# ffmpeg, jpeg, png, lzo, Bink and the C library. Anything unresolved
# and not on that list fails the run, and a name on the list that is no
# longer needed fails it too, so the list cannot rot.
#
# It is not a real build: this container has no ffmpeg development
# files, so CMake stops before it starts, and no 32-bit toolchain
# either. What this does establish is that the 181 translation units
# compile and coexist, which is as far as anything here can go.

set -e
cd "$(dirname "$0")/.."

CC=${CC:-gcc}
CFLAGS="-std=gnu99 -fcommon -fms-extensions -O0 -w"
INCLUDES="-Itests/stub -Ithirdparty/SDL3/include -Ithirdparty/freetype/include -Ithirdparty/freetype/iriseng -Ithirdparty/ffmpeg/include"
OUT=tests/build/wholelink
rm -rf "$OUT"
mkdir -p "$OUT"

echo "=== every engine source, compiled and put in one link ==="

# The project file is the list of sources, as it is for every other
# sweep here; thirdparty is not ours to link.
python3 - > "$OUT/srcs.txt" <<'PY'
import pathlib, re
proj = pathlib.Path('irisEng.vcxproj').read_text(encoding='latin-1')
for m in sorted(set(re.findall(r'ClCompile Include="([^"]+\.c)"', proj))):
    s = m.replace('\\', '/')
    if not s.startswith('thirdparty/'):
        print(s)
PY
num=$(wc -l < "$OUT/srcs.txt" | tr -d ' ')
echo "  $num sources"

# Compiled in parallel; one failure is enough to stop.
: > "$OUT/failed.txt"
# shellcheck disable=SC2016
xargs -a "$OUT/srcs.txt" -P "$(nproc 2>/dev/null || echo 4)" -I{} sh -c '
    o="$1/$(echo "{}" | tr "/" "_" | sed "s/\.c$/.o/")"
    if ! '"$CC"' '"$CFLAGS"' '"$INCLUDES"' -c "{}" -o "$o" 2>"$o.err"; then
        echo "{}" >> "$1/failed.txt"
    fi
' _ "$OUT"

if [ -s "$OUT/failed.txt" ]; then
    echo "  FAIL these did not compile:"
    sed 's/^/      /' "$OUT/failed.txt"
    exit 1
fi
objs=$(find "$OUT" -name '*.o' | wc -l | tr -d ' ')
echo "  $objs objects, none failed"

# The combined object goes beside the directory and not in it: the
# objects are named after their sources, the engine has an engine.c,
# and ld will not write its output over one of its inputs.
ALL=tests/build/wholelink-all.o

# ld refuses a duplicate strong definition, so this call is the check.
if ! ld -r -o "$ALL" "$OUT"/*.o 2>"$OUT/link.err"; then
    echo "  FAIL the objects would not link together:"
    sed 's/^/      /' "$OUT/link.err" | head -20
    exit 1
fi
echo "  they link; $(nm -g --defined-only "$ALL" | wc -l | tr -d ' ') defined symbols"

nm -u "$ALL" | sed 's/^ *U //' | grep -v '^_GLOBAL_OFFSET_TABLE_$' \
    | sort -u > "$OUT/unresolved.txt"
sort -u tests/wholelink-external.txt > "$OUT/expected.txt"

new=$(comm -23 "$OUT/unresolved.txt" "$OUT/expected.txt")
gone=$(comm -13 "$OUT/unresolved.txt" "$OUT/expected.txt")
rc=0

if [ -n "$new" ]; then
    echo "  FAIL the engine calls these and no engine file defines them:"
    echo "$new" | sed 's/^/      /'
    echo
    echo "  Either one is missing, or it belongs to something outside the"
    echo "  engine and goes in tests/wholelink-external.txt, sorted."
    rc=1
fi
if [ -n "$gone" ]; then
    echo "  FAIL these are on the external list and nothing needs them now:"
    echo "$gone" | sed 's/^/      /'
    echo
    echo "  Drop them from tests/wholelink-external.txt. A list that keeps"
    echo "  names nothing asks for stops being evidence of anything."
    rc=1
fi
[ "$rc" = 0 ] && echo "  $(wc -l < "$OUT/unresolved.txt" | tr -d ' ') unresolved, every one of them external"

exit $rc
