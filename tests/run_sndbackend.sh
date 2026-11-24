#!/bin/sh
# Differential test for platform/sndbackend.c, as a standalone runner.
#
#   ./tests/run_sndbackend.sh            run the sweep
#   ./tests/run_sndbackend.sh --guards   print the seed-bit distribution
#
# tests/run.sh calls this script, so there is no second copy of the
# symbol list; running it on its own is the quick way to work on it, and
# --guards is only useful from here.
#
# sndbackend.c sits in platform/, so the frozen copy's four include paths are
# rewritten to reach the same headers from tests/. sndbackend.h and sound.h
# declare the platform layer's globals without extern, so both copies
# would otherwise share one tentative definition and the two runs would
# not be independent -- every global is renamed along with every function.
# tests/sndbackend_side.c is compiled twice against the same rename list,
# once per side, and carries the engine-side callbacks too, because those
# are handed pointers into the caller's own per-side globals.
#
# basic_array.c, math.c and platform/adpcm.c are the engine's own, linked
# rather than mocked: the sound-buffer list is basic_array's binary
# search, the doppler shift is math.c's vector helpers and the round
# constant, and the ADPCM decode is adpcm.c's.
#
# -no-pie is load-bearing. sndbackend.c puts a tSndSoundBufferElem* or a
# voice into a four-byte slot and takes it back out again, which is
# lossless on the 32-bit target. Linked -no-pie the mock arena sits below
# 4GB, so it is lossless here too; test_sndbackend.c checks that and
# refuses to run otherwise.
#
# HDL_TRANSLATES=0 for the same reason. The sound-buffer list and the two
# raster lists hold handles now, and the frozen copy holds truncated
# addresses, so the two would disagree at every one of those keys by
# construction -- 24 differences on top of the 84 this test declares as
# deliberate, measured. Forcing the identity form of the translation
# layer makes the new copy store what the frozen one does, which under
# -no-pie is lossless, so the comparison and its declared counts stay
# exactly as they were.
#
# What that gives up is the translating path through those ten sites.
# tests/test_sndrefs.c, run below, is that path.

set -e
cd "$(dirname "$0")/.."

CC=${CC:-gcc}
# WINSOUND_FROZEN: sound.h gives its structs the DirectSound
# interfaces the frozen copy still uses, in place of the plain
# pointers the game build gets. Every translation unit here needs it,
# or the two would disagree about the layout of a struct they hand to
# each other.
# MEM_PTR_BYTES=4: an array of pointers takes four bytes an element in
# the 32-bit build the engine ships as, and the frozen copies here
# write that out. Five of them are compared on the figure the
# allocator was asked for -- the mock logs it -- so the rewrite has
# to ask for the same thing, and the mock's own double headroom
# covers the shortfall. The game build gets the real width.
CFLAGS="-std=gnu99 -fcommon -fms-extensions -O0 -g -w -Itests/stub -Itests -DWINSOUND_FROZEN -DHDL_TRANSLATES=0 -DMEM_PTR_BYTES=4"
# The translating build: nothing forced, so the layer is whatever this
# host's pointer width asks for.
CFLAGS_T="-std=gnu99 -fcommon -fms-extensions -O0 -g -w -Itests/stub -Itests -DWINSOUND_FROZEN"
OUT=tests/build
mkdir -p "$OUT"

# Every symbol sndbackend.c defines -- functions and file-scope objects --
# plus the globals sndbackend.h gives it tentative definitions for, the
# sound.h and loader globals it reaches, and the engine callbacks that
# live in sndbackend_side.c. The first two are extracted rather than listed
# so a new one cannot be forgotten.
wsnd_syms=$(python3 - <<'EOF'
import re
src = open('platform/sndbackend.c', encoding='latin-1').read()
hdr = open('platform/sndbackend.h', encoding='latin-1').read()
names = []

# Comments, in the order they appear rather than block-then-line.
# Taking the blocks out first is wrong and was: platform/sndbackend.c carries
# a commented-out line reading "//*(double*)&dest.x = v21;", whose
# "/*" is inside a line comment and opens nothing -- but a block pass
# that runs first reads it as an opener and eats everything up to the
# next "*/", taking five function definitions with it. They were then
# missing from the rename list and both copies defined them, which the
# linker called a multiple definition.
def WSND_strip(text):
    out = []
    i = 0
    n = len(text)
    while i < n:
        if text.startswith('/*', i):
            end = text.find('*/', i + 2)
            i = n if end < 0 else end + 2
        elif text.startswith('//', i):
            end = text.find('\n', i)
            i = n if end < 0 else end
        else:
            out.append(text[i])
            i += 1
    return ''.join(out)

body = WSND_strip(src)

for m in re.finditer(r'(?m)^(?:static\s+)?[A-Za-z_][\w \*]*?(?<![\w])(\w+)\s*\([^;{}]*\)\s*\n?\{', body):
    names.append(m.group(1))

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

# tentative globals in sndbackend.h
for line in hdr.split('\n'):
    line = line.strip()
    if not line or line.startswith('//') or line.startswith('#') or '(' in line:
        continue
    m = re.match(r'^(?:extern\s+)?[A-Za-z_][\w \*]*?(?<![\w])(\w+)\s*(\[[^\]]*\])?\s*;$', line)
    if m and m.group(1) not in ('sDSoundGlobalStruct', 'sPcSndEventRelated', 'sFXGlobalStruct'):
        names.append(m.group(1))

# globals from sound.h and the loader that sndbackend.c reads or writes, and
# the engine callbacks sndbackend_side.c defines
names += [
    'SND_gst_Params', 'SND_gst_FxParam', 'SND_gst_FxDefaultParam',
    'SND_gpst_Interface', 'SND_gast_StreamList', 'SND_gi_StreamListSize',
    'SND_gax_TrackPlayingList', 'SND_gst_TrackLookUpTable',
    'rasterbas1', 'rasterbas2', 'sndRasterStat', 'SND_gi_RasterCurrentFileId',
    'SND_gc_NoSound', 'LOA_gb_SpeedMode', 'TIM_gf_dt', 'MAI_gh_MainWindow',
    'SND_l_GetAttFromVol', 'SND_f_GetVolFromAtt', 'SND_f_GetGlobalVol',
    'SND_SetInstVolume', 'SND_AutoStop', 'SND_AutoPlay',
    'SND_f_SphericComputing', 'SND_f_AxisComputing', 'SND_i_IsPlaying',
    'SND_i_StreamAlloc', 'SND_TrackDeleteInstanceFromList',
    'OBJ_pst_GetDynaStruct', 'DYN_SpeedGetVector',
]

seen = []
for n in names:
    if n not in seen and n not in ('struct', 'return', 'int', 'void', 'float', 'if', 'typedef'):
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
new_defs="$(defs NEW_ $wsnd_syms) -DSIDE_RUN=NEW_wsnd_side_run -DSIDE_CASES=NEW_wsnd_side_cases -DSIDE_RUN_GUARDS=NEW_wsnd_side_guards -DWSND_SIDE_NEW=1"
# shellcheck disable=SC2086
base_defs="$(defs BASE_ $wsnd_syms) -DSIDE_RUN=BASE_wsnd_side_run -DSIDE_CASES=BASE_wsnd_side_cases -DSIDE_RUN_GUARDS=BASE_wsnd_side_guards -DWSND_SIDE_NEW=0"

# Which copy this is. One scripted wake-up can mean different things
# to the two of them -- the device announcing a refill position to the
# frozen copy, and a tick to the rewrite, which looks for itself -- and
# the driver has to say which it is for the mock to answer in the right
# words.

# shellcheck disable=SC2086
$CC $CFLAGS -c platform/sndbackend.c            -o "$OUT/wsnd.new.o"       $new_defs
# The one function the pass renamed. It used to place a 3D buffer on a
# unit circle from the pan; it sets the pan itself now, and the name
# followed. The frozen copy keeps the old spelling and is mapped onto
# the new one rather than edited.
base_defs="$base_defs -DpcSND_3DSoundBufferSetPos=BASE_pcSND_SoundBufferSetPan"

# shellcheck disable=SC2086
$CC $CFLAGS -c tests/baseline_sndbackend.c -o "$OUT/wsnd.base.o"      $base_defs
# shellcheck disable=SC2086
$CC $CFLAGS -c tests/sndbackend_side.c     -o "$OUT/wsnd.side.new.o"  $new_defs
# shellcheck disable=SC2086
$CC $CFLAGS -c tests/sndbackend_side.c     -o "$OUT/wsnd.side.base.o" $base_defs
$CC $CFLAGS -c math.c                    -o "$OUT/wsnd.math.o"
$CC $CFLAGS -c basic_array.c             -o "$OUT/wsnd.bas.o"
$CC $CFLAGS -c platform/adpcm.c               -o "$OUT/wsnd.adpcm.o"
$CC $CFLAGS -c handle.c                  -o "$OUT/wsnd.handle.o"
$CC $CFLAGS -c tests/sndbackend_mocks.c    -o "$OUT/wsnd.mocks.o"
$CC $CFLAGS -c tests/test_sndbackend.c     -o "$OUT/wsnd.drv.o"

$CC -no-pie "$OUT/wsnd.new.o" "$OUT/wsnd.base.o" "$OUT/wsnd.side.new.o" "$OUT/wsnd.side.base.o" \
    "$OUT/wsnd.math.o" "$OUT/wsnd.bas.o" "$OUT/wsnd.adpcm.o" "$OUT/wsnd.mocks.o" "$OUT/wsnd.drv.o" \
    "$OUT/wsnd.handle.o" -o "$OUT/sndbackend" -lm

"$OUT/sndbackend" "$@"

# And the three lists with the translation layer really on, which is
# what the differential above gives up to keep its byte comparison and
# its declared counts. One copy of sndbackend.c, its own names, no
# -no-pie: a handle here is a number that is nothing like the address it
# stands for, and the first check in the driver is that it is.
$CC $CFLAGS_T -c platform/sndbackend.c     -o "$OUT/sndref.snd.o"
$CC $CFLAGS_T -DSIDE_RUN=wsnd_side_run -DSIDE_CASES=wsnd_side_cases \
    -DSIDE_RUN_GUARDS=wsnd_side_guards -DWSND_SIDE_NEW=1 \
    -c tests/sndbackend_side.c   -o "$OUT/sndref.side.o"
$CC $CFLAGS_T -c math.c                    -o "$OUT/sndref.math.o"
$CC $CFLAGS_T -c basic_array.c             -o "$OUT/sndref.bas.o"
$CC $CFLAGS_T -c platform/adpcm.c          -o "$OUT/sndref.adpcm.o"
$CC $CFLAGS_T -c tests/sndbackend_mocks.c  -o "$OUT/sndref.mocks.o"
$CC $CFLAGS_T -c handle.c                  -o "$OUT/sndref.handle.o"
$CC $CFLAGS_T -c tests/test_sndrefs.c      -o "$OUT/sndref.drv.o"
$CC "$OUT/sndref.snd.o" "$OUT/sndref.side.o" "$OUT/sndref.math.o" \
    "$OUT/sndref.bas.o" "$OUT/sndref.adpcm.o" "$OUT/sndref.mocks.o" \
    "$OUT/sndref.drv.o" "$OUT/sndref.handle.o" -o "$OUT/sndrefs" -lm

"$OUT/sndrefs"
