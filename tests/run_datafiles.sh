#!/bin/sh
# The unit test for datafiles.c.
#
#   ./tests/run_datafiles.sh
#
# New code with no earlier copy, so this is a direct check and not a
# differential one. The driver includes datafiles.c so it can point
# the reader at a file it writes itself, and it reads the shipped
# datafiles.ini as one of its cases -- so a typo in that file fails
# here rather than in game.
#
# It runs from the repository root, because that case names
# datafiles.ini relative to it.

set -e
cd "$(dirname "$0")/.."

CC=${CC:-gcc}
CFLAGS="-std=gnu11 -fcommon -fms-extensions -O0 -g -w -Itests/stub -Itests"
OUT=tests/build
mkdir -p "$OUT"

echo "=== datafiles ==="
$CC $CFLAGS -c tests/test_datafiles.c -o "$OUT/datafiles.drv.o"
$CC "$OUT/datafiles.drv.o" -o "$OUT/datafiles"
"$OUT/datafiles"
