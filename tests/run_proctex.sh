#!/bin/sh
# The proceduraltextures.c differential test, standalone.
#
#   ./tests/run_proctex.sh
#
# Kept out of tests/run.sh because several passes are in flight at once
# and that file is where they would collide. Fold it in afterwards: the
# body below is a run_* function in the same shape as the others.

set -e
cd "$(dirname "$0")/.."

CC=${CC:-gcc}
CFLAGS="-std=gnu99 -fcommon -fms-extensions -O0 -g -w -Werror=implicit-function-declaration -Itests/stub -Itests"
OUT=tests/build
mkdir -p "$OUT"

defs() {
    prefix=$1
    shift
    for s in "$@"; do printf ' -D%s=%s%s' "$s" "$prefix" "$s"; done
}

# Defined by both copies, so prefixed on both sides and compared.
proctex_syms="TEXPRO_pv_WaterLoad TEXPRO_l_WaterUpdate TEXPRO_WaterDisrupt \
TEX_l_File_LoadProcedural TEX_Procedural_Add \
TEXPRO_MPEG_Start TEXPRO_MPEG_ReadNext TEXPRO_pv_MpegLoad TEXPRO_l_MpegUpdate \
TEXPRO_MpegFree TEXPRO_pv_FireLoad TEXPRO_l_FireUpdate \
TEXPRO_pv_PlasmaLoad TEXPRO_l_PlasmaUpdate TEXPRO_pv_PhotoLoad \
TEXPRO_l_PhotoUpdate TEXPRO_Free TEXPRO_PhotoFree TEXPRO_DummyDisrupt \
TEXPRO_DummyFree TEXPRO_pv_EmptyLoad TEXPRO_l_DummyUpdate TEX_Procedural_Init \
TEXPRO_Photo_SetMapGao \
TEX_gast_ProceduralInterface TexPro_WaterDistruptCounter \
KX_442 KY_443 ALPHA_441 \
TEX_gi_NbProcedural TEX_gdst_Procedural TEX_gst_GlobalList TEXPRO_Cos \
TEXPRO_pst_CurrentMpeg"

# Only the frozen copy has these: they are the Bink SDK's custom-I/O
# apparatus -- a linked list of 4 KB blocks, a ten-slot bitmask, a
# busy flag per block and the six callbacks that drove them -- and
# the rewrite deletes the lot, because libav pulls its input through
# one callback instead. They are still prefixed so the frozen copy
# keeps its own symbols, and they are no longer compared: there is
# nothing on the other side to compare them against.
proctex_base_only="TEXPRO_Bink_RemoveActSlotIDFromUsedSlots \
TEXPRO_Bink_GetNextFreeSlotID TEXPRO_Bink_ReadActBlock \
TEXPRO_BINK_ReadActBlockAndAdvance TEXPRO_Bink_GetAlreadyProcessedFileSize \
BINK_releaseLinkedList PC_VideoStreamRestart_Bink PC_VideoStreamSync_Bink \
BINK_GetBufferSize TEXPRO_Bink_ReadAndCopyNextBlockToReadBuffer \
TEXPRO_BINK_ReadNumBytes BINK_ReadHeader BINK_ReadFrame BINK_SetInfo \
BINK_Idle TEXPRO_BINK_Close BINK_Close BINK_AllocLinkedList BINK_IO_FUNC \
PC_VideoStreamCreate_Bink BINK_CloseAll gBinkReadUsedSlots \
BINK_IF_PTR_GLOBAL"

# Only the rewrite has these.
proctex_new_only="TEXPRO_i_ReadFromBF PC_VideoStreamCreate \
PC_VideoStreamRestart PC_VideoStreamSync PC_VideoStreamClose \
PC_VideoStreamCloseAll"

# The frozen copy names two fields of tTexProcMpegSub the way the
# header did before the pass. Mapped with -D rather than edited: a
# baseline is not changed to follow the rewrite.
BASECOMPAT="-DbinkKey=videoKey -DbinkIFPtr=videoStream"

# shellcheck disable=SC2086
$CC $CFLAGS -Ithirdparty/ffmpeg/include -c proceduraltextures.c \
    -o "$OUT/proctex.new.o" $(defs NEW_ $proctex_syms) $(defs NEW_ $proctex_new_only)
# shellcheck disable=SC2086
$CC $CFLAGS $BASECOMPAT -c tests/baseline_proceduraltextures.c \
    -o "$OUT/proctex.base.o" $(defs BASE_ $proctex_syms) $(defs BASE_ $proctex_base_only)
$CC $CFLAGS -Ithirdparty/ffmpeg/include -c tests/proctex_mocks.c \
    -o "$OUT/proctex.mocks.o"
$CC $CFLAGS -Ithirdparty/ffmpeg/include -c tests/test_proceduraltextures.c \
    -o "$OUT/proctex.drv.o"
$CC "$OUT/proctex.new.o" "$OUT/proctex.base.o" "$OUT/proctex.mocks.o" \
    "$OUT/proctex.drv.o" -o "$OUT/proctex" -lm
"$OUT/proctex"
