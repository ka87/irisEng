#!/bin/sh
# The four loading.h helpers, and the one rule that separates them.
#
#   ./tests/run_loarefs.sh
#
# A key sign extends and a marker zero extends, and the rule is which
# the value is compared against: -1 for a key, the literal it was
# written as for a marker. Both halves have cost something already --
# a geometry's MRM magic never matched its literal, and the actions
# differential failed 72 cases on a zero-extended -1.
#
# This checks the rule rather than any one engine source, which is
# worth having because several of the files that rest on it have no
# coverage of their own: nothing builds the frozen copy of
# spritegenmodifier.c, whose not-set sentinel is a marker chosen on
# exactly this ground.
#
# Header-only, so there is nothing to link but the driver.

set -e
cd "$(dirname "$0")/.."

CC=${CC:-gcc}
CFLAGS="-std=gnu99 -fcommon -fms-extensions -O0 -g -w -Itests/stub -Itests"
OUT=tests/build
mkdir -p "$OUT"

$CC $CFLAGS tests/test_loarefs.c -o "$OUT/loarefs" -lm
"$OUT/loarefs"
