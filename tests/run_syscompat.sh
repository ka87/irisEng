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
mkdir -p "$OUT"

echo "=== syscompat ==="
$CC -g -O0 -fsanitize=address,undefined -std=gnu11 -fms-extensions \
    -Itests/stub \
    tests/test_syscompat.c -o "$OUT/syscompat" -lm
"$OUT/syscompat"
