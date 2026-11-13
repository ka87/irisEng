#!/bin/sh
# Implicit function declarations, per file.
#
#   ./tests/implicitcheck.sh          report and compare against the baseline
#   ./tests/implicitcheck.sh --list   print the current counts in baseline form
#
# This exists because the parse command used everywhere else carries
# -w, and on gcc 13 -w disables -Werror=implicit-function-declaration
# whichever order the two are given in. The parse check still does what
# its name says -- a file that does not parse still fails it -- but the
# implicit-declaration half of it has never fired. This runs that half
# on its own, without -w, and reads only the one diagnostic out of the
# noise.
#
# The baseline below is what the tree carried when the check was first
# run. Every entry is in a file that has not had its cleanup pass yet,
# except strings.c's isdigit, which wants <ctype.h>. The check fails on
# any count that rises and on any file that appears and is not listed.

set -e
cd "$(dirname "$0")/.."

CC=${CC:-gcc}
CFLAGS="-fsyntax-only -std=gnu11 -fms-extensions -Itests/stub"

BASELINE="bigfiles.c 2
engine.c 2
proceduraltextures.c 2
profiler.c 9
sideload.c 1
sound.c 1
strings.c 1
win/wingdi.c 25
win/wininputoutput.c 10
win/winsound.c 13"

files=$(python3 - <<'EOF'
import re
s = open('irisEng.vcxproj', encoding='latin-1').read()
for m in re.finditer(r'ClCompile Include="([^"]+\.c)"', s):
    p = m.group(1).replace('\\', '/')
    if 'jpeg-6b' in p or 'minilzo' in p:
        continue
    print(p)
EOF
)

current=$(
for f in $files; do
    n=$($CC $CFLAGS "$f" 2>&1 | grep -c 'implicit declaration of function' || true)
    [ "$n" != "0" ] && printf '%s %s\n' "$f" "$n"
done
true
)

if [ "$1" = "--list" ]; then
    echo "$current"
    exit 0
fi

bad=0
for line in $(echo "$current" | tr ' ' ':'); do
    [ -z "$line" ] && continue
    f=${line%%:*}
    n=${line##*:}
    want=$(echo "$BASELINE" | awk -v f="$f" '$1 == f { print $2 }')
    if [ -z "$want" ]; then
        printf 'NEW   %-28s %s implicit declaration(s)\n' "$f" "$n"
        bad=$((bad + 1))
    elif [ "$n" -gt "$want" ]; then
        printf 'WORSE %-28s %s, was %s\n' "$f" "$n" "$want"
        bad=$((bad + 1))
    elif [ "$n" -lt "$want" ]; then
        printf 'BETTER %-27s %s, was %s -- update the baseline\n' "$f" "$n" "$want"
    else
        printf 'ok    %-28s %s\n' "$f" "$n"
    fi
done

for line in $(echo "$BASELINE" | tr ' ' ':'); do
    f=${line%%:*}
    echo "$current" | grep -q "^$f " || \
        printf 'GONE  %-28s was %s -- update the baseline\n' "$f" "${line##*:}"
done

if [ "$bad" -ne 0 ]; then
    echo "$bad file(s) worse than the baseline"
    exit 1
fi
echo "no new implicit declarations"
