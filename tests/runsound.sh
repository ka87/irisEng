#!/bin/sh
# Differential test for sound.c, as a standalone runner.
#
#   ./tests/runsound.sh
#
# It is separate from run.sh only because other files were being rewritten
# in parallel and run.sh is where that work would collide; the body below
# is the same shape as run.sh's run_* functions and is meant to be folded
# in.
#
# sound.c sits at the repo root, so the frozen copy includes its headers
# through "../" for the same reason animation.c's, strings.c's and
# world.c's do: -I. would put the engine's own strings.h in front of the
# system one. sound.h declares the module's globals without extern, so
# both copies would otherwise share one tentative definition and the two
# runs would not be independent -- every global is renamed along with
# every function. tests/sound_side.c is compiled twice against the same
# rename list, once per side.
#
# math.c and basic_array.c are the engine's own, linked rather than
# mocked: the volume table is pow()-driven and the attenuation search is
# the engine's binary search, and the family and track tables are
# basic_array's.

set -e
cd "$(dirname "$0")/.."

CC=${CC:-gcc}
CFLAGS="-std=gnu99 -fcommon -fms-extensions -O0 -g -w -Itests/stub -Itests"
OUT=tests/build
mkdir -p "$OUT"

# Every symbol sound.c defines -- functions and file-scope objects -- plus
# the globals sound.h and gensoundmodifier.h give it tentative definitions
# for. Extracted rather than listed so a new one cannot be forgotten.
sound_syms=$(python3 - <<'EOF'
import re
src = open('sound.c', encoding='latin-1').read()
hdr = open('sound.h', encoding='latin-1').read()
names = []

# functions defined in sound.c
for m in re.finditer(r'(?m)^(?:static\s+)?[A-Za-z_][\w \*]*?(?<![\w])(\w+)\s*\([^;{}]*\)\s*\n\{', src):
    names.append(m.group(1))

# file-scope objects defined in sound.c
body = re.sub(r'(?s)/\*.*?\*/', '', src)
depth = 0
for line in body.split('\n'):
    if depth == 0:
        m = re.match(r'^[A-Za-z_][\w \*]*?(?<![\w])(\w+)\s*(\[[^\]]*\])?\s*=', line)
        if m and '(' not in line.split('=')[0]:
            names.append(m.group(1))
        m = re.match(r'^[A-Za-z_][\w \*]*?(?<![\w])(\w+)\s*;\s*$', line)
        if m:
            names.append(m.group(1))
    depth += line.count('{') - line.count('}')

# tentative globals in sound.h, after the last typedef
tail = hdr[hdr.rindex('} tSoundMdfStruct;'):]
for line in tail.split('\n'):
    line = line.strip()
    if not line or line.startswith('//') or line.endswith(')') or '(' in line:
        continue
    m = re.match(r'^(?:extern\s+)?[A-Za-z_][\w \*]*?(?<![\w])(\w+)\s*(\[[^\]]*\])?\s*;$', line)
    if m:
        names.append(m.group(1))

names += ['sndMdfBankUser', 'SND_gst_BankMdFUserList']
seen = []
for n in names:
    if n not in seen and n not in ('struct', 'return', 'int', 'void', 'float', 'if'):
        seen.append(n)
print(' '.join(seen))
EOF
)

defs() {
    prefix=$1
    shift
    for s in "$@"; do printf ' -D%s=%s%s' "$s" "$prefix" "$s"; done
}

# shellcheck disable=SC2086
new_defs="$(defs NEW_ $sound_syms) -DSIDE_RUN=NEW_snd_side_run -DSIDE_CASES=NEW_snd_side_cases"
# shellcheck disable=SC2086
base_defs="$(defs BASE_ $sound_syms) -DSIDE_RUN=BASE_snd_side_run -DSIDE_CASES=BASE_snd_side_cases"

# shellcheck disable=SC2086
$CC $CFLAGS -c sound.c              -o "$OUT/snd.new.o"       $new_defs
# shellcheck disable=SC2086
$CC $CFLAGS -c tests/baseline_sound.c -o "$OUT/snd.base.o"    $base_defs
# shellcheck disable=SC2086
$CC $CFLAGS -c tests/sound_side.c   -o "$OUT/snd.side.new.o"  $new_defs
# shellcheck disable=SC2086
$CC $CFLAGS -c tests/sound_side.c   -o "$OUT/snd.side.base.o" $base_defs
$CC $CFLAGS -c math.c               -o "$OUT/snd.math.o"
$CC $CFLAGS -c basic_array.c        -o "$OUT/snd.bas.o"
$CC $CFLAGS -c tests/sound_mocks.c  -o "$OUT/snd.mocks.o"
$CC $CFLAGS -c tests/test_sound.c   -o "$OUT/snd.drv.o"

$CC "$OUT/snd.new.o" "$OUT/snd.base.o" "$OUT/snd.side.new.o" "$OUT/snd.side.base.o" \
    "$OUT/snd.math.o" "$OUT/snd.bas.o" "$OUT/snd.mocks.o" "$OUT/snd.drv.o" \
    -o "$OUT/sound" -lm

"$OUT/sound"
