#!/bin/sh
# Differential test for soundmodifier.c, as a standalone runner.
#
#   ./tests/runsoundmod.sh
#
# It is separate from run.sh only because other files were being rewritten
# in parallel and run.sh is where that work would collide; the body below
# is the same shape as run.sh's run_* functions and is meant to be folded
# in.
#
# soundmodifier.c sits at the repo root, so the frozen copy includes its
# headers through "../" for the same reason sound.c's and animation.c's do.
# sound.h declares the module's globals without extern, so both copies
# would otherwise share one tentative definition and the two runs would not
# be independent -- every global is renamed along with every function.
#
# sound.c is not linked. The twenty-odd entry points soundmodifier.c
# reaches it through are stood up in tests/soundmod_side.c, which is
# compiled twice against the same rename list, once per side, so each side
# drives its own SND_gst_Params. The four whose behaviour soundmodifier.c
# depends on follow sound.c's own bodies.
#
# Both copies are built with -ftrivial-auto-var-init=zero, and that is not
# cosmetic. MDF_SndCreateInstance and SND_SModifierExtPlayerApplyOnInstance
# each build a fade record on the stack and never write its timeMultiplier
# or elapsedTime, and FadeInit copies the whole record into the heap. gcc
# gives the record a different stack slot in the two copies, so the two
# sides read different leftovers there and nothing else in the case is
# comparable. Zeroing automatic variables makes both sides read 0 and still
# catches a dropped assignment, which leaves one side at 0 and the other
# not. The flag is applied to both sides equally.
#
# math.c is the engine's own, linked rather than mocked, and the driver
# calls MATH_InitModule before the first case: MATH_gd_magicDoubleForRound
# is what the fade table's step index is taken out of, and without the
# call every table-driven case collapses to step 0 on both sides.

set -e
cd "$(dirname "$0")/.."

CC=${CC:-gcc}
CFLAGS="-std=gnu99 -fcommon -fms-extensions -O0 -g -w -ftrivial-auto-var-init=zero -Itests/stub -Itests"
OUT=tests/build
mkdir -p "$OUT"

# Every symbol soundmodifier.c defines, the globals sound.h and
# gensoundmodifier.h give it tentative definitions for, and the sound.c
# entry points stood up per side. Extracted rather than listed so a new one
# cannot be forgotten.
sm_syms=$(python3 - <<'EOF'
import re
src = open('soundmodifier.c', encoding='latin-1').read()
hdr = open('sound.h', encoding='latin-1').read()
names = []

body = re.sub(r'(?s)/\*.*?\*/', '', src)
body = re.sub(r'//[^\n]*', '', body)

# functions defined in soundmodifier.c
for m in re.finditer(r'(?m)^(?:static\s+)?[A-Za-z_][\w \*]*?(?<![\w])(\w+)\s*\([^;{}]*\)\s*\n?\s*\{', body):
    names.append(m.group(1))

# file-scope objects defined in soundmodifier.c
depth = 0
for line in body.split('\n'):
    if depth == 0:
        m = re.match(r'^[A-Za-z_][\w \*]*?(?<![\w])(\w+)\s*(\[[^\]]*\])?\s*=', line)
        if m and '(' not in line.split('=')[0]:
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

# the sound.c entry points soundmodifier.c calls, which soundmod_side.c
# stands up once per side
names += ['SND_DeleteByIndex', 'SND_LinkToFamilly', 'SND_PlayMdF', 'SND_PlaySound',
          'SND_PlaySoundLooping', 'SND_PlaySoundLoopingOnTrack', 'SND_PlaySoundOnTrack',
          'SND_SModifierAddInInstance', 'SND_SetFlags', 'SND_SetFreq', 'SND_SetNearFar',
          'SND_SetPan', 'SND_SetPos', 'SND_SetTrack', 'SND_SetVol', 'SND_Stop',
          'SND_i_AllocSound', 'SND_i_CheckOrInitGAO', 'SND_i_InstanceSetf',
          'SND_i_IsTrackPlaying', 'SND_l_AddAmbienceNoLoad', 'SND_l_AddDialogNoLoad',
          'SND_l_AddLoadingSound', 'SND_l_AddMusicNoLoad', 'SND_l_AddSound',
          'SND_l_GetSoundIndex', 'SND_l_Request', 'SND_pst_SModifierGet',
          'sm_track_playing']

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
new_defs="$(defs NEW_ $sm_syms) -DSIDE_RUN=NEW_sm_side_run -DSIDE_CASES=NEW_sm_side_cases"
# shellcheck disable=SC2086
base_defs="$(defs BASE_ $sm_syms) -DSIDE_RUN=BASE_sm_side_run -DSIDE_CASES=BASE_sm_side_cases"

# shellcheck disable=SC2086
$CC $CFLAGS -c soundmodifier.c                 -o "$OUT/sm.new.o"       $new_defs
# shellcheck disable=SC2086
$CC $CFLAGS -c tests/baseline_soundmodifier.c  -o "$OUT/sm.base.o"      $base_defs
# shellcheck disable=SC2086
$CC $CFLAGS -c tests/soundmod_side.c           -o "$OUT/sm.side.new.o"  $new_defs
# shellcheck disable=SC2086
$CC $CFLAGS -c tests/soundmod_side.c           -o "$OUT/sm.side.base.o" $base_defs
$CC $CFLAGS -c math.c                          -o "$OUT/sm.math.o"
$CC $CFLAGS -c tests/soundmod_mocks.c          -o "$OUT/sm.mocks.o"
$CC $CFLAGS -c tests/test_soundmodifier.c      -o "$OUT/sm.drv.o"

$CC "$OUT/sm.new.o" "$OUT/sm.base.o" "$OUT/sm.side.new.o" "$OUT/sm.side.base.o" \
    "$OUT/sm.math.o" "$OUT/sm.mocks.o" "$OUT/sm.drv.o" \
    -o "$OUT/soundmodifier" -lm

"$OUT/soundmodifier"
