#!/bin/sh
# The differential test for win/wininputoutput.c.
#
#   ./tests/run_winio.sh
#
# tests/run.sh calls this script, so there is no second copy of the
# symbol list; running it on its own is the quick way to work on it.
#
# Every function the file defines, and every global it defines with an
# initialiser, is prefixed BASE_/NEW_: the two copies both define
# those, so they cannot be shared. Everything declared in
# win/wininputoutput.h is shared, and the harness rebuilds it between
# the two runs, which is what makes the comparison one of state.
#
# The frozen copy still spells four names the way the file did before
# the pass, so those are mapped onto the new ones with -D rather than
# edited: a baseline is not changed to follow the rewrite.
#
# fopen/fclose/fread/fwrite are redirected into the mocks for both
# copies, so the card manager's file traffic lands in the call log.

set -e
cd "$(dirname "$0")/.."

CC=${CC:-gcc}
CFLAGS="-std=gnu99 -fcommon -fms-extensions -O0 -g -w -Itests/stub -Itests"
OUT=tests/build
mkdir -p "$OUT"

fns="pcINO_JoystickPositionAlignSetFromKeyboard pcINO_case64StateMachineHandler \
pcINO_case4MouseAmountCalc pcINO_SetMouseSensitivity pcINO_InputHandlerCase1 \
pcINO_InputHandlerCase2 pcINO_InputHandlerCase4 pcINO_InputHandlerCase8 \
pcINO_InputHandlerCase32 pcINO_InputHandlerHoverCraft pcINO_InputHandlerCase100 \
pcINO_InputHandlerCase101 DINPUT_GetProperty DINPUT_InitVariables \
pcINO_getKeyNames pcINO_getKeyNamesForEverything \
pcINO_GetFourKeyNamesCattedTogether INO_SetControlSubmode_inner_sub_48A650 \
INO_SetControlMode_inner_sub_48ACB0 pcINO_Joystick_Init pcINO_Joystick_Close \
pcINO_Joystick_Move pcINO_i_IsMouseBeingUsed pcINO_i_SetCurrentPad \
pcINO_JoystickPifPafEnable pcINO_JoystickPafIntensity DbgDialogFunc \
pcINO_dinput_mouse_and_keyb_acquire pcINO_dinput_mouse_and_keyb_unacquire \
pcINO_HandleTweakFlags pcINO_Joystick_Update pcINO_SavModuleInit \
pcINO_SavModuleClose pcINO_b_AsyncInProgress pcINO_i_TestPresence \
pcINO_i_MountCard pcINO_i_UnmountCard pcINO_i_CheckCard pcINO_i_FormatCard \
pcINO_i_TestFreeSpace pcINO_i_Retry pcINO_i_Continue pcINO_i_ActiveConsole \
pcINO_i_Open pcINO_i_Close pcINO_i_ReadIndex_1 pcINO_i_WriteIndex \
pcINO_i_ReadSlot_1 pcINO_i_ReadSlot_2 pcINO_i_ReadSlot_3 pcINO_i_WriteSlot_C \
pcINO_i_WriteSlot_E pcINO_i_CheckFreeSpace pcINO_i_TestCard"

# globals the file defines with an initialiser, so each copy needs its own
glob="pcINO_SelectedControlMode pcINO_SelectedControlSubmode \
pcINO_joyPositionBeforeControlSubmodeSet prefixSpaces prefixOneSpace \
prefix_p16_2 commandWindowDlgTemplate pcINO_IntKeybDataLink \
pcINO_keyMapsCase1 pcINO_keyMapsCase2 pcINO_keyMapsCase64 pcINO_keyMapsCase32 \
pcINO_keyMapsCase8 pcINO_keyMapsCase100 pcINO_keyMapsCase101 \
pcINO_InputHandlerSubModeCaseDef pcINO_InputHandlerSubModeCase0 \
pcINO_InputHandlerSubModeCase2 pcINO_InputHandlerSubModeCase7 \
pcINO_InputHandlerSubModeCase10"

# the six card-failure flags, which the frozen copy still calls dword_*
old="dword_990164 dword_990168 dword_99016C dword_990170 dword_990174 dword_990178"
new="pcINO_savPresenceFailure pcINO_savMountFailure pcINO_savCheckFailure \
pcINO_savFormatFailure pcINO_savFreeSpaceFailure pcINO_savCardTestFailure"

# two globals and one field the pass renamed; the frozen copy keeps the
# old spelling and is mapped onto the header's new one
COMPAT="-Dmouse_if_dword_98F248=pcINO_mouseDevice \
-Dkeyb_if_dword_98F238=pcINO_keybDevice \
-DpcINO_unkKeyStateTest=pcINO_numLockAtAcquire"

STDIO="-Dfopen=WINIO_fopen -Dfclose=WINIO_fclose -Dfread=WINIO_fread \
-Dfwrite=WINIO_fwrite"

defs() {
    prefix=$1
    shift
    for s in "$@"; do printf ' -D%s=%s%s' "$s" "$prefix" "$s"; done
}

# the frozen copy's dword_* names map to the same per-side symbols the
# rewrite's new names do, pairwise
oldfail() {
    prefix=$1
    set -- $new
    for o in $old; do printf ' -D%s=%s%s' "$o" "$prefix" "$1"; shift; done
}

echo "=== wininputoutput ==="
$CC $CFLAGS $STDIO -c win/wininputoutput.c -o "$OUT/winio.new.o" \
    $(defs NEW_ $fns) $(defs NEW_ $glob) $(defs NEW_ $new)
$CC $CFLAGS $STDIO $COMPAT -c tests/baseline_winio.c -o "$OUT/winio.base.o" \
    $(defs BASE_ $fns) $(defs BASE_ $glob) $(oldfail BASE_)
$CC $CFLAGS -c tests/winio_mocks.c -o "$OUT/winio.mocks.o"
$CC $CFLAGS -c tests/test_winio.c -o "$OUT/winio.drv.o"
$CC "$OUT/winio.new.o" "$OUT/winio.base.o" "$OUT/winio.mocks.o" "$OUT/winio.drv.o" \
    -o "$OUT/winio" -lm
"$OUT/winio"
