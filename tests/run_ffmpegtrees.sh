#!/bin/sh
# The two libav trees, against each other.
#
#   ./tests/run_ffmpegtrees.sh
#
# thirdparty/ffmpeg is split by pointer width, and the two halves are
# the 9.0 branch at different commits -- the 32-bit one is the build
# that was pinned, the 64-bit one the branch's latest, because the
# pinned build is no longer published for win64.
#
# That is safe only while they stay ABI- and API-compatible, and the
# project's own README calls a mismatched pair "the one way this goes
# wrong quietly": it compiles, it links, and it misbehaves at run time
# on one platform only. Nothing else here would notice, because the
# differentials run on one width and the Windows link is not done here
# at all.
#
# So this compares the halves:
#
#   - the same files, name for name, so neither half is missing a
#     library or carrying something the other does not;
#   - the same soname for each DLL, which is the major version and so
#     the ABI;
#   - the same VERSION_MAJOR and VERSION_MINOR in each library's
#     version.h, which is the API;
#   - and each half's DLLs reporting the machine its name claims.
#
# What it deliberately does not compare is the whole of the headers.
# They differ -- five micro versions, the version string and two
# documentation comments -- and that is allowed; a declaration, a type
# or a constant differing is not, which is what the major and minor
# cover.

set -e
cd "$(dirname "$0")/.."

A=thirdparty/ffmpeg/x32
B=thirdparty/ffmpeg/x64
LIBS="avcodec avformat avutil swscale swresample"
fails=0

echo "=== the two libav trees, against each other ==="

for d in "$A" "$B"; do
    if [ ! -d "$d/lib" ]; then
        echo "  skipped: $d is not populated"
        exit 0
    fi
done

# the same files, name for name
( cd "$A" && find . -type f | sort ) > tests/build/ffmpeg.a.txt
( cd "$B" && find . -type f | sort ) > tests/build/ffmpeg.b.txt
if ! diff -q tests/build/ffmpeg.a.txt tests/build/ffmpeg.b.txt >/dev/null; then
    echo "  FAIL the two trees do not hold the same files:"
    diff tests/build/ffmpeg.a.txt tests/build/ffmpeg.b.txt | sed 's/^/      /' | head -12
    fails=$((fails + 1))
else
    echo "  $(wc -l < tests/build/ffmpeg.a.txt | tr -d ' ') files, the same in both"
fi

# the same soname for every DLL, which is the ABI
for f in "$A"/bin/*.dll; do
    n=$(basename "$f")
    if [ ! -f "$B/bin/$n" ]; then
        echo "  FAIL $n is in x32 and not in x64, so the sonames differ"
        fails=$((fails + 1))
    fi
done
[ "$fails" -eq 0 ] && echo "  every DLL has the same soname in both, so the same major version"

# the same API: major and minor of each library
for l in $LIBS; do
    u=$(echo "$l" | tr '[:lower:]' '[:upper:]')
    for part in MAJOR MINOR; do
        va=$(grep -hE "define LIB${u}_VERSION_${part}" "$A/include/lib$l/version.h" | awk '{print $3}')
        vb=$(grep -hE "define LIB${u}_VERSION_${part}" "$B/include/lib$l/version.h" | awk '{print $3}')
        if [ "$va" != "$vb" ]; then
            echo "  FAIL lib$l ${part} is $va in x32 and $vb in x64"
            fails=$((fails + 1))
        fi
    done
done
[ "$fails" -eq 0 ] && echo "  every library's major and minor agree, so the same API"

# and each half is the architecture its name claims
python3 - "$A" "$B" <<'PY' || fails=$((fails + 1))
import sys, struct, pathlib
want = {sys.argv[1]: (0x14c, 'x86'), sys.argv[2]: (0x8664, 'x64')}
bad = 0
for d, (machine, label) in want.items():
    for f in sorted(pathlib.Path(d, 'bin').glob('*.dll')):
        b = f.read_bytes()
        m = struct.unpack_from('<H', b, struct.unpack_from('<I', b, 0x3c)[0] + 4)[0]
        if m != machine:
            print(f"  FAIL {f} reports machine 0x{m:04x} and {d} is the {label} half")
            bad += 1
print(f"  every DLL reports the machine its half claims" if not bad else "", end="" if bad else "\n")
sys.exit(1 if bad else 0)
PY

if [ "$fails" -ne 0 ]; then
    echo "  $fails failure(s)"
    exit 1
fi
echo "  the halves agree"
