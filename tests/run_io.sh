#!/bin/sh
# The differential test for inputoutput.c.
#
#   ./tests/run_io.sh
#
# Standalone on purpose: tests/run.sh is where several passes would
# collide, so this script is the one to run until it is folded in.
#
# Only the functions carry BASE_/NEW_ prefixes. The file's globals are
# shared, because the two copies run one after the other with the whole
# state rebuilt in between, which is what makes the comparison one of
# state rather than of two parallel universes. time and localtime are
# redirected into the harness so the save index's timestamps do not
# depend on how long the run takes.

set -e
cd "$(dirname "$0")/.."

CC=${CC:-gcc}
CFLAGS="-std=gnu99 -fcommon -fms-extensions -O0 -g -w -Itests/stub -Itests"
OUT=tests/build
mkdir -p "$OUT"

syms="INO_Joystick_Init INO_SavInitModule INO_KeyboardInit INO_InitModule \
INO_i_SetCurrentPad INO_i_GetCurrentPad INO_SimulateReset INO_Joystick_Close \
INO_Joystick_Update INO_i_PifPafEnable INO_PifSet INO_PafSet \
INO_b_Joystick_IsButtonDown INO_b_Joystick_IsButtonJustDown \
INO_b_Joystick_IsButtonJustUp INO_Joystick_Move INO_JoySimulate \
INO_SetControlSubmode INO_SetControlMode INO_SetMouseSensitivity \
INO_KeyboardClose INO_SavCloseModule INO_CloseModule INO_Keyboard_Update \
INO_Reinit INO_b_LanguageIsPresent INO_i_SavGetCardStatus INO_b_IsKeyPressed \
INO_b_IsKeyJustPressed INO_b_IsKeyReleased INO_b_IsKeyJustReleased \
INO_i_GetDiscError INO_b_IoButtonPressed SAV_PhotoFirstInit SAV_PhotoInit \
SAV_b_IsMoreRecent SAV_OrderData INO_v_MemoryCardManager INO_Update \
INO_SavSetCardStatus SAV_PhotoReinit SAV_PhotoReinitValidate SAV_PhotoClose \
SAV_PhotoLastClose SAV_PhotoSet SAV_PhotoDel SAV_PhotoDecal SAV_PhotoGet \
SAV_PhotoValidate SAV_PhotoUnvalidate SAV_LoadData Demo_Geti Demo_Seti \
INO_i_IsMouseBeingUsed INO_l_ApplyJoystickDir INO_l_RunCardOp \
\
INO_ge_SavSetNextState INO_ge_SavSetNextNextState INO_ge_SavCommand INO_gi_LaunchDemo \
Photo_gi_FirstInit Photo_gi_Init INO_ge_MasterType INO_gai_PresentLanguage"

CLOCK="-Dtime=IOMOCK_time -Dlocaltime=IOMOCK_localtime"

defs() {
    prefix=$1
    shift
    for s in "$@"; do printf ' -D%s=%s%s' "$s" "$prefix" "$s"; done
}

echo "=== inputoutput ==="
$CC $CFLAGS $CLOCK -c inputoutput.c -o "$OUT/io.new.o" $(defs NEW_ $syms)
$CC $CFLAGS $CLOCK -c tests/baseline_inputoutput.c -o "$OUT/io.base.o" $(defs BASE_ $syms)
$CC $CFLAGS -c tests/io_mocks.c -o "$OUT/io.mocks.o"
$CC $CFLAGS -c tests/test_inputoutput.c -o "$OUT/io.drv.o"
$CC "$OUT/io.new.o" "$OUT/io.base.o" "$OUT/io.mocks.o" "$OUT/io.drv.o" -o "$OUT/io" -lm
"$OUT/io"
