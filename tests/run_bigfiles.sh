#!/bin/sh
# The differential test for bigfiles.c.
#
#   ./tests/run_bigfiles.sh
#
# Standalone on purpose: tests/run.sh is where several passes would
# collide, so this script is the one to run until it is folded in.
#
# bigfiles.c does not compile on this host -- the fpos_t arithmetic in
# BIG_fseek and three MSVC CRT names -- so both copies are compiled
# with tests/bigfiles_compat.h included ahead of them.  That header
# supplies the target's fpos_t and sends the C library's file calls to
# the harness; it changes nothing in the file itself.

set -e
cd "$(dirname "$0")/.."

CC=${CC:-gcc}
CFLAGS="-std=gnu99 -fcommon -fms-extensions -O0 -g -w -Itests/stub -Itests \
-include tests/bigfiles_compat.h"
OUT=tests/build
mkdir -p "$OUT"

big_syms="BIG_InitModule BIG_CloseModule BIG_ul_SearchKeyToPos \
BIG_FreeGlobalBuffer BIG_fseek BIGcomp_ui_Read32 BIGcomp_CheckBlock \
BIGcomp_CheckBuffer BIGcomp_i_CompressedSize BIGcomp_i_CompressedSize_v2 \
BIGcomp_DecodeBlocks BIG_fseekAndRead BIG_fread BIG_SpeedMode_fread \
BIG_InsertKeyToPos BIG_ReadHeader BIG_ReadFatFile BIG_ReadAllFats BIG_Open \
BIG_p_RequestBuffer BIG_SeekAndRead BIG_ReadNoSeek BIG_ResetFat BIG_Close \
BIG_ul_GetLengthFile BIG_pc_ReadFileTmp BINK_SeekAndReadFromBF \
PreLoadMoveBlocks \
LOA_gb_DoReadAllFats BIGcomp_M_CompressedSize BIGcomp_M_BlockSize gi_TestBin \
gi_CurBin BIG_gi_SpecialRead BIG_gst BIG_gi_ReadMode BIG_gb_CanCache \
BIG_gp_ReadBuffer BIG_gi_ReadSeek BIG_si_DecompressedSize BIG_gi_ReadSize \
BIG_sp_CompressedNextBlock BIG_si_CompressedSize BIG_gp_CompressedBuffer \
BIG_gi_RealSeek BIG_si_BinFileSize BIG_gp_CompressedBufferPtrBackup \
BIG_si_BinFileReadSize BIG_gul_ReadPos BIG_ul_CachePos gai_CurSize \
gap_FirstPointer gai_CurBinFileSize gaui_CurBinFilePos BIG_gp_GlobalBuffer \
BIG_gi_GlobalBufferSize BIG_gp_GlobalSpecialBuffer \
BIG_gi_GlobalSpecialBufferSize"

defs() {
    prefix=$1
    shift
    for s in "$@"; do printf ' -D%s=%s%s' "$s" "$prefix" "$s"; done
}

echo "=== bigfiles ==="
$CC $CFLAGS -c bigfiles.c -o "$OUT/bigfiles.new.o" $(defs NEW_ $big_syms)
$CC $CFLAGS -c tests/baseline_bigfiles.c -o "$OUT/bigfiles.base.o" $(defs BASE_ $big_syms)
$CC $CFLAGS -c tests/bigfiles_mocks.c -o "$OUT/bigfiles.mocks.o"
$CC $CFLAGS -c tests/test_bigfiles.c -o "$OUT/bigfiles.drv.o"
$CC "$OUT/bigfiles.new.o" "$OUT/bigfiles.base.o" "$OUT/bigfiles.mocks.o" \
    "$OUT/bigfiles.drv.o" -o "$OUT/bigfiles" -lm
"$OUT/bigfiles"
