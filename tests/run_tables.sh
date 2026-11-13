#!/bin/sh
# The differential test for tables.c.
#
#   ./tests/run_tables.sh
#
# Standalone on purpose: tests/run.sh is where several passes would
# collide, so this script is the one to run until it is folded in.
# The frozen copy and the rewrite in one binary with BASE_/NEW_
# prefixes -- the thirty-one functions and both module globals -- over
# one logging allocator, with an arena per side so that addresses
# never reach the comparison.

set -e
cd "$(dirname "$0")/.."

CC=${CC:-gcc}
CFLAGS="-std=gnu99 -fcommon -fms-extensions -O0 -g -w -Itests/stub -Itests"
OUT=tests/build
mkdir -p "$OUT"

tab_syms="TAB_TableElemIsInvalid TAB_DLlist_CreateElem \
TAB_DLlist_CreateAndAddElem TAB_DLlist_Create TAB_DLlist_Remove \
TAB_DLlist_RemoveElem TAB_TablesManager_Init \
TAB_TablesManager_RegisterTable TAB_TablesManager_UnregisterTable \
TAB_TablesManager_Close TAB_TablesManager_Call \
TAB_TablesManager_RemoveHolesFromTablesList TAB_Ptable_Init \
TAB_Ptable_Resize TAB_Ptable_Close TAB_Ptable_RemoveHoles \
TAB_PTable_AddElemToTable TAB_ul_Ptable_GetElemIndexWithPointer \
TAB_PFtable_Init TAB_PFtable_Clear TAB_PFtable_Resize TAB_PFtable_Close \
TAB_PFtable_RemoveHoles TAB_PFtable_RemoveElem \
TAB_ul_PFtable_GetElemIndexWithPointer TAB_pst_PFtable_GetElemWithPointer \
TAB_AddElemToTablePF TAB_PFTable_AddElemToTable \
TAB_PFtable_RemoveElemWithPointer TAB_InitModule TAB_CloseModule \
gst_TablesManagerList gb_TablesManagerListInitialized"

defs() {
    prefix=$1
    shift
    for s in "$@"; do printf ' -D%s=%s%s' "$s" "$prefix" "$s"; done
}

echo "=== tables ==="
$CC $CFLAGS -c tables.c -o "$OUT/tables.new.o" $(defs NEW_ $tab_syms)
$CC $CFLAGS -c tests/baseline_tables.c -o "$OUT/tables.base.o" $(defs BASE_ $tab_syms)
$CC $CFLAGS -c tests/tables_mocks.c -o "$OUT/tables.mocks.o"
$CC $CFLAGS -c tests/test_tables.c -o "$OUT/tables.drv.o"
$CC "$OUT/tables.new.o" "$OUT/tables.base.o" "$OUT/tables.mocks.o" \
    "$OUT/tables.drv.o" -o "$OUT/tables" -lm
"$OUT/tables"
