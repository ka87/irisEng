#!/bin/sh
# The system services the engine asks for by Windows' names.
#
#   ./tests/run_syscompat.sh
#
# platform/syscompat.c is included by the test rather than linked, so
# the branch under test is the one this host compiles -- which is the
# branch that had no implementation before it, and so the one worth
# running. The Windows half is the platform's own and is not reachable
# from here.
set -e
cd "$(dirname "$0")/.."

CC=${CC:-gcc}
OUT=tests/build
mkdir -p "$OUT/syscompat.run"

# The single-instance guard puts its lock beside the saves, which the
# path module answers for. Only SDL's headers are wanted here: the
# entry points it uses are defined by the harness (tests/sdlpath.h),
# which is what lets the lock go in a directory of the test's own.
SDL3_INC="$PWD/thirdparty/SDL3/include"

echo "=== syscompat ==="
$CC -g -O0 -fsanitize=address,undefined -std=gnu11 -fms-extensions \
    -I"$SDL3_INC" -Itests/stub -Itests \
    -c tests/test_syscompat.c -o "$OUT/syscompat.o"
$CC -g -O0 -fsanitize=address,undefined -std=gnu11 -fms-extensions \
    -I"$SDL3_INC" -c datapath.c -o "$OUT/syscompat.datapath.o"
$CC -fsanitize=address,undefined "$OUT/syscompat.o" \
    "$OUT/syscompat.datapath.o" -o "$OUT/syscompat" -lm
SYSCOMPAT_DIR="$PWD/$OUT/syscompat.run" "$OUT/syscompat"
