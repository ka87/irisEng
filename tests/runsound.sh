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
# MEM_PTR_BYTES=4: an array of pointers takes four bytes an element in
# the 32-bit build the engine ships as, and the frozen copies here
# write that out. Five of them are compared on the figure the
# allocator was asked for -- the mock logs it -- so the rewrite has
# to ask for the same thing, and the mock's own double headroom
# covers the shortfall. The game build gets the real width.
# IRIS_FROZEN_SIZES: the frozen copy walks the sound array by a
# hardcoded fifty-six bytes and asks the allocator for fifty-six a
# slot, which is what an element measures where a pointer is four.
# It cannot be changed, so the rewrite is told the same figure here
# and the two walk the same array. Every other build gets sizeof.
CFLAGS="-std=gnu99 -fcommon -fms-extensions -O0 -g -w -no-pie -fno-pie -DHDL_TRANSLATES=0 -Itests/stub -Itests -DMEM_PTR_BYTES=4 -DIRIS_FROZEN_SIZES"
OUT=tests/build
mkdir -p "$OUT"

# The 32-bit stand-in for an address: the engine files below
# register and release through it, and it has no engine
# dependencies of its own.
$CC $CFLAGS -c handle.c -o "$OUT/sound.handle.o"

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
# SND_FAMILY_FROZEN: the AND-family list's size is folded in the frozen
# copy, and the figure it folds is right only while the struct is 144
# bytes. Both copies take the folded form here so the heap they dump
# stays comparable; ./tests/run_sndfamily.sh --asan checks the real one.
CFLAGS="$CFLAGS -DSND_FAMILY_FROZEN=1"
new_defs="$(defs NEW_ $sound_syms) -DSIDE_RUN=NEW_snd_side_run -DSIDE_CASES=NEW_snd_side_cases"
# shellcheck disable=SC2086
# SND_WAVEINFO_FROZEN: the rewrite takes a pointer-wide position for a
# header already in memory, where the frozen copy still holds that
# address in an int. The two signatures differ, so sound.h gives the
# frozen copy its own.
base_defs="$(defs BASE_ $sound_syms) -DSIDE_RUN=BASE_snd_side_run -DSIDE_CASES=BASE_snd_side_cases -DSND_WAVEINFO_FROZEN"

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

$CC -no-pie "$OUT/snd.new.o" "$OUT/snd.base.o" "$OUT/snd.side.new.o" "$OUT/snd.side.base.o" \
    "$OUT/snd.math.o" "$OUT/snd.bas.o" "$OUT/snd.mocks.o" "$OUT/snd.drv.o" \
    -o "$OUT/sound" "$OUT/sound.handle.o" -lm

"$OUT/sound"
