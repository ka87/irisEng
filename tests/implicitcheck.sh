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
# The baseline below is what the tree carries now. Every entry is in a
# file that has not had its cleanup pass yet, except strings.c's
# isdigit, which wants <ctype.h>. The check fails on any count that
# rises and on any file that appears and is not listed.
#
# proceduraltextures.c was here with 2, for BinkSetMemory and
# BinkSetIOSize. Its pass added them to tests/stub/bink.h, so it is
# off the list and the parse sweep is 133 of 137 rather than 132.
#
# profiler.c was here with 9, all of them GDI and system-colour calls.
# Its pass added them to tests/stub/windows.h, which also took
# win/wininputoutput.c from 10 to 9.
#
# win/winsound.c was here with 13, all of them handle, thread, file and
# COM calls. Its pass added them to tests/stub/windows.h, which took
# engine.c from 2 to 1 and win/wingdi.c from 13 to 11 as well.
#
# sideload.c was here with 1, GetModuleFileNameA. font.c wanted the
# same call and declared it in tests/stub/windows.h, which took
# sideload.c to zero with it.
#
# win/wingdi.c went from 25 to 13 when the D3D9, D3DX and message-loop
# declarations went into the stubs; that work also took the parse
# sweep from 133 of 137 to 135, leaving only bigfiles.c and loading.c,
# which are the fpos_t question.
#
# The four win/ passes between them added every Win32 and Direct3D
# declaration these files were missing -- the window, cursor, module
# and directory calls, Direct3DCreate9, QueryPerformanceFrequency,
# MsgWaitForMultipleObjectsEx, the dialog and keyboard calls and the
# CRT's _hypot -- so win/wingdi.c, win/wininputoutput.c and
# win/winsound.c are all off the list. The figures below are measured
# on the merge, not carried over from any one pass: the four sets of
# declarations resolve more together than any of them does alone.

set -e
cd "$(dirname "$0")/.."

CC=${CC:-gcc}
CFLAGS="-fsyntax-only -std=gnu11 -fms-extensions -Itests/stub"
CFLAGS="$CFLAGS -Ithirdparty/ffmpeg/include"
# font.c needs these to parse at all, and a file that does not parse
# reports no implicit declarations -- see UNSWEEPABLE below
CFLAGS="$CFLAGS -Ithirdparty/freetype/include -Ithirdparty/freetype/iriseng"

BASELINE="bigfiles.c 2
engine.c 1
sound.c 1
strings.c 1"

# Files this host cannot compile at all, for reasons that are not
# implicit declarations. They matter because the count below is a grep
# for one phrase: a file that stops with a different error produces
# none of it and scores a clean zero, so it drops out of the sweep
# silently. That is how font.c went unswept until this list existed --
# it was missing an include path and counted zero.
#
# loading.c casts call_fgetpos()'s return to unsigned int. fpos_t is an
# integer on MSVC and a struct in glibc, so the cast is correct where
# the engine is built and impossible here. It is listed, not fixed:
# changing engine code to suit this host is the wrong way round.
UNSWEEPABLE="loading.c"

files=$(python3 - <<'EOF'
import re
s = open('irisEng.vcxproj', encoding='latin-1').read()
for m in re.finditer(r'ClCompile Include="([^"]+\.c)"', s):
    p = m.group(1).replace('\\', '/')
    if 'minilzo' in p or p.startswith('thirdparty/'):
        continue   # upstream source, not ours to tidy -- and FreeType's
                   # needs defines and include paths this sweep does not
                   # pass, so it was failing for other reasons and
                   # counting zero
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

# Which files could not be compiled at all. Checked separately from the
# count, because the count cannot tell "clean" from "never parsed".
unswept=$(
for f in $files; do
    out=$($CC $CFLAGS "$f" 2>&1 || true)
    case "$out" in
        *'implicit declaration of function'*) continue ;;
    esac
    if ! $CC $CFLAGS "$f" >/dev/null 2>&1; then
        printf '%s\n' "$f"
    fi
done
true
)

if [ "$1" = "--list" ]; then
    echo "$current"
    exit 0
fi

bad=0
for f in $unswept; do
    case " $UNSWEEPABLE " in
        *" $f "*) printf 'skip  %-28s cannot be compiled here, known\n' "$f" ;;
        *) printf 'UNSWEPT %-26s cannot be compiled here, so its count means nothing\n' "$f"
           bad=$((bad + 1)) ;;
    esac
done
for f in $UNSWEEPABLE; do
    case " $unswept " in
        *" $f "*) ;;
        *) printf 'FIXED %-28s compiles now -- take it off UNSWEEPABLE\n' "$f"
           bad=$((bad + 1)) ;;
    esac
done
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
