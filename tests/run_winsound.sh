#!/bin/sh
# Differential test for win/winsound.c, as a standalone runner.
#
#   ./tests/run_winsound.sh            run the sweep
#   ./tests/run_winsound.sh --guards   print the seed-bit distribution
#
# It is separate from run.sh only because other files were being rewritten
# in parallel and run.sh is where that work would collide; the body below
# is the same shape as run.sh's run_* functions and is meant to be folded
# in.
#
# winsound.c sits in win/, so the frozen copy's four include paths are
# rewritten to reach the same headers from tests/. winsound.h and sound.h
# declare the platform layer's globals without extern, so both copies
# would otherwise share one tentative definition and the two runs would
# not be independent -- every global is renamed along with every function.
# tests/winsound_side.c is compiled twice against the same rename list,
# once per side, and carries the engine-side callbacks too, because those
# are handed pointers into the caller's own per-side globals.
#
# basic_array.c, math.c and win/adpcm.c are the engine's own, linked
# rather than mocked: the sound-buffer list is basic_array's binary
# search, the doppler shift is math.c's vector helpers and the round
# constant, and the ADPCM decode is adpcm.c's.
#
# -no-pie is load-bearing. winsound.c casts a tSndSoundBufferElem* to int
# and back at seven sites (BAS_binsert, BAS_bsearch, BAS_bdeletegetval and
# the stream handle API), which is lossless on the 32-bit target. Linked
# -no-pie the mock arena sits below 4GB, so it is lossless here too;
# test_winsound.c checks that and refuses to run otherwise.

set -e
cd "$(dirname "$0")/.."

CC=${CC:-gcc}
CFLAGS="-std=gnu99 -fcommon -fms-extensions -O0 -g -w -Itests/stub -Itests"
OUT=tests/build
mkdir -p "$OUT"

# Every symbol winsound.c defines -- functions and file-scope objects --
# plus the globals winsound.h gives it tentative definitions for, the
# sound.h and loader globals it reaches, and the engine callbacks that
# live in winsound_side.c. The first two are extracted rather than listed
# so a new one cannot be forgotten.
wsnd_syms=$(python3 - <<'EOF'
import re
src = open('win/winsound.c', encoding='latin-1').read()
hdr = open('win/winsound.h', encoding='latin-1').read()
names = []

body = re.sub(r'(?s)/\*.*?\*/', '', src)
body = re.sub(r'//[^\n]*', '', body)

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

# tentative globals in winsound.h
for line in hdr.split('\n'):
    line = line.strip()
    if not line or line.startswith('//') or line.startswith('#') or '(' in line:
        continue
    m = re.match(r'^(?:extern\s+)?[A-Za-z_][\w \*]*?(?<![\w])(\w+)\s*(\[[^\]]*\])?\s*;$', line)
    if m and m.group(1) not in ('sDSoundGlobalStruct', 'sPcSndEventRelated', 'sFXGlobalStruct'):
        names.append(m.group(1))

# globals from sound.h and the loader that winsound.c reads or writes, and
# the engine callbacks winsound_side.c defines
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
new_defs="$(defs NEW_ $wsnd_syms) -DSIDE_RUN=NEW_wsnd_side_run -DSIDE_CASES=NEW_wsnd_side_cases -DSIDE_RUN_GUARDS=NEW_wsnd_side_guards"
# shellcheck disable=SC2086
base_defs="$(defs BASE_ $wsnd_syms) -DSIDE_RUN=BASE_wsnd_side_run -DSIDE_CASES=BASE_wsnd_side_cases -DSIDE_RUN_GUARDS=BASE_wsnd_side_guards"

# shellcheck disable=SC2086
$CC $CFLAGS -c win/winsound.c            -o "$OUT/wsnd.new.o"       $new_defs
# shellcheck disable=SC2086
$CC $CFLAGS -c tests/baseline_winsound.c -o "$OUT/wsnd.base.o"      $base_defs
# shellcheck disable=SC2086
$CC $CFLAGS -c tests/winsound_side.c     -o "$OUT/wsnd.side.new.o"  $new_defs
# shellcheck disable=SC2086
$CC $CFLAGS -c tests/winsound_side.c     -o "$OUT/wsnd.side.base.o" $base_defs
$CC $CFLAGS -c math.c                    -o "$OUT/wsnd.math.o"
$CC $CFLAGS -c basic_array.c             -o "$OUT/wsnd.bas.o"
$CC $CFLAGS -c win/adpcm.c               -o "$OUT/wsnd.adpcm.o"
$CC $CFLAGS -c tests/winsound_mocks.c    -o "$OUT/wsnd.mocks.o"
$CC $CFLAGS -c tests/test_winsound.c     -o "$OUT/wsnd.drv.o"

$CC -no-pie "$OUT/wsnd.new.o" "$OUT/wsnd.base.o" "$OUT/wsnd.side.new.o" "$OUT/wsnd.side.base.o" \
    "$OUT/wsnd.math.o" "$OUT/wsnd.bas.o" "$OUT/wsnd.adpcm.o" "$OUT/wsnd.mocks.o" "$OUT/wsnd.drv.o" \
    -o "$OUT/winsound" -lm

"$OUT/winsound" "$@"
