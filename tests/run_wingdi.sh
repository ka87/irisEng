#!/bin/sh
# The differential test for win/wingdi.c.
#
#   ./tests/run_wingdi.sh
#
# Standalone on purpose: tests/run.sh is where several passes would
# collide, so this script is the one to run until it is folded in.
#
# The frozen copy and the rewrite go into one binary with BASE_/NEW_
# prefixes on every function the file defines and on the eleven globals
# it defines strongly -- everything else it declares is a tentative
# definition and is deliberately shared, so the two sides are compared
# over the same renderer state.  The Direct3D device is a logging mock
# vtable; the real math.c and profiler.c are linked rather than mocked,
# with MATH_InitModule called before the first case.

set -e
cd "$(dirname "$0")/.."

CC=${CC:-gcc}
CFLAGS="-std=gnu99 -fcommon -fms-extensions -O0 -g -w -Itests/stub -Itests"
# both sides see the real SDK's window-message numbers: the stub's are
# sequential, which inverts their order and makes most of the two
# message handlers unreachable.  See tests/wingdi_msg_sdk.h.
CFLAGS="$CFLAGS -include tests/wingdi_msg_sdk.h"
OUT=tests/build
mkdir -p "$OUT"

syms=$(python3 - <<'EOF'
import re
src = open('win/wingdi.c', encoding='latin-1').read()
names = []
for m in re.finditer(r'^[A-Za-z_][\w \*]*?[\w\*]\s+\**(\w+)\s*\(', src, re.M):
    names.append(m.group(1))
names += ['d3dProjectionMtx', 'd3dTransformMatrix', 'd3dTransformMatrix2',
          'engineFogTableModeToD3D', 'rasterNames', 'worldPtrIfMultiStream',
          'D3D_gi_Widescreen', 'D3D_gi_FlatLayerBox', 'D3D_gi_AspectTrace',
          'D3D_gf_RefYoverX', 'D3D_gf_CutsceneBand']
seen = []
for n in names:
    if n not in seen and n not in ('if', 'for', 'while', 'switch', 'return', 'sizeof'):
        seen.append(n)
print(' '.join(seen))
EOF
)

defs() {
    prefix=$1
    shift
    for s in "$@"; do printf ' -D%s=%s%s' "$s" "$prefix" "$s"; done
}

echo "=== wingdi ==="
$CC $CFLAGS -c win/wingdi.c -o "$OUT/wingdi.new.o" $(defs NEW_ $syms)
$CC $CFLAGS -c tests/baseline_wingdi.c -o "$OUT/wingdi.base.o" $(defs BASE_ $syms)
$CC $CFLAGS -c math.c -o "$OUT/wingdi.math.o"
$CC $CFLAGS -c profiler.c -o "$OUT/wingdi.profiler.o"
$CC $CFLAGS -c tests/wingdi_mocks.c -o "$OUT/wingdi.mocks.o"
$CC $CFLAGS -c tests/test_wingdi.c -o "$OUT/wingdi.drv.o"
$CC "$OUT/wingdi.new.o" "$OUT/wingdi.base.o" "$OUT/wingdi.math.o" \
    "$OUT/wingdi.profiler.o" "$OUT/wingdi.mocks.o" "$OUT/wingdi.drv.o" \
    -o "$OUT/wingdi" -lm
"$OUT/wingdi"
