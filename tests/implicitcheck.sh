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
# The baseline below is empty: no engine source calls a function it has
# not been shown. The check fails on any file that appears, which is
# now any file at all.
#
# It sweeps the test drivers and mocks as well, which it did not until
# a driver for graphicsobjects/geomaterial.c called four loaders that
# no header declares. An implicit declaration returns int, so each of
# those materials came back truncated and the driver crashed on the
# first one -- which is exactly the fault that driver was written to
# catch, arriving through the driver instead of through the engine. The
# runners cannot see it: thirteen of them pass
# -Werror=implicit-function-declaration and all thirteen also pass -w.
#
# Fifteen were sitting in nine drivers when the sweep first reached
# here. Fourteen returned int or void and were harmless; the fifteenth
# was MEM_p_Alloc in tests/test_gfx.c, a truncated pointer that only
# that runner's -no-pie kept lossless.
#
# tests/baseline_*.c are frozen copies and not swept: they are not
# ours to change, and each is compiled only inside the runner that
# renames its symbols.
#
# strings.c was the last entry, 1 for isdigit. It asks for <ctype.h>
# itself now rather than waiting for windows.h to bring it.
#
# engine.c and sound.c were here with 1 each, both _strnicmp -- the
# CRT's name for a case-insensitive compare of a bounded length. The
# engine asks for that by a name of its own now and each platform
# answers, so the two came off together.
#
# proceduraltextures.c was here with 2, for BinkSetMemory and
# BinkSetIOSize. Its pass added them to tests/stub/bink.h, so it is
# off the list and the parse sweep is 133 of 137 rather than 132.
#
# profiler.c was here with 9, all of them GDI and system-colour calls.
# Its pass added them to tests/stub/windows.h, which also took
# platform/inputbackend.c from 10 to 9.
#
# platform/sndbackend.c was here with 13, all of them handle, thread, file and
# COM calls. Its pass added them to tests/stub/windows.h, which took
# engine.c from 2 to 1 and platform/d3drender.c from 13 to 11 as well.
#
# sideload.c was here with 1, GetModuleFileNameA. font.c wanted the
# same call and declared it in tests/stub/windows.h, which took
# sideload.c to zero with it.
#
# platform/d3drender.c went from 25 to 13 when the D3D9, D3DX and message-loop
# declarations went into the stubs; that work also took the parse
# sweep from 133 of 137 to 135, leaving only bigfiles.c and loading.c,
# which were the fpos_t question and are no longer a question at all.
#
# The four platform/ passes between them added every Win32 and Direct3D
# declaration these files were missing -- the window, cursor, module
# and directory calls, Direct3DCreate9, QueryPerformanceFrequency,
# MsgWaitForMultipleObjectsEx, the dialog and keyboard calls and the
# CRT's _hypot -- so platform/d3drender.c, platform/inputbackend.c and
# platform/sndbackend.c are all off the list. The figures below are measured
# on the merge, not carried over from any one pass: the four sets of
# declarations resolve more together than any of them does alone.

set -e
cd "$(dirname "$0")/.."

CC=${CC:-gcc}
CFLAGS="-fsyntax-only -std=gnu11 -fms-extensions -Itests/stub"
CFLAGS="$CFLAGS -Ithirdparty/ffmpeg/x32/include"
CFLAGS="$CFLAGS -Ithirdparty/SDL3/include"
# font.c needs these to parse at all, and a file that does not parse
# reports no implicit declarations -- see UNSWEEPABLE below
CFLAGS="$CFLAGS -Ithirdparty/freetype/include -Ithirdparty/freetype/iriseng"
# the drivers and mocks reach their own headers and the engine's
CFLAGS="$CFLAGS -Itests"

BASELINE=""

# Files this host cannot compile at all, for reasons that are not
# implicit declarations. They matter because the count below is a grep
# for one phrase: a file that stops with a different error produces
# none of it and scores a clean zero, so it drops out of the sweep
# silently. That is how font.c went unswept until this list existed --
# it was missing an include path and counted zero.
#
# Nothing, now. loading.c was here because it cast call_fgetpos()'s
# return to unsigned int, and fpos_t is an integer on MSVC and a
# struct in glibc -- correct where the engine was built and impossible
# here. The note said it was listed rather than fixed, because
# changing engine code to suit this host is the wrong way round, and
# that still holds: what changed is not the host's business but the
# engine's. The big file is bigger than a long and the file was
# seeking in it through a type the standard calls opaque, which
# happened to work on one library. It asks for a wide seek by name
# now, and each platform answers.
#
# bigfiles.c came off the baseline at the same time and for the same
# change: its two implicit declarations were _access and _chmod, the
# CRT's own names for asking after a file's permissions, and those are
# a question of the engine's now too.
# tests/sndbackend_side.c is one side of a differential and is compiled
# twice, with every sndbackend.c symbol and every tentative global
# renamed by -D and one of the two sound gates forced. Standalone it
# does not know which side it is, so it does not parse: the field it
# reads off a buffer element and the globals it touches are both the
# per-side names. It is the only file here in that shape.
UNSWEEPABLE="tests/sndbackend_side.c"

files=$(python3 - <<'EOF'
import re
s = open('irisEng.vcxproj', encoding='latin-1').read()
for m in re.finditer(r'ClCompile Include="([^"]+\.c)"', s):
    p = m.group(1).replace('\\', '/')
    if p.startswith('thirdparty/'):
        continue   # upstream source, not ours to tidy -- and FreeType's
                   # needs defines and include paths this sweep does not
                   # pass, so it was failing for other reasons and
                   # counting zero
    print(p)

import glob
for p in sorted(glob.glob('tests/*.c')):
    if not p.startswith('tests/baseline_'):
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
# One word per line in $unswept, so the list is collapsed to spaces
# before it is searched: " $f " cannot match an entry that a newline
# follows, and with more than one file unswept only the last of them
# was ever found. That read as the first file having been fixed.
for f in $UNSWEEPABLE; do
    case " $(echo $unswept) " in
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

# Pointer types that do not match, in the same sweep and for the same
# reason: -w hides them too, and MSVC reports the mismatch as C4047,
# a warning, so a build that allows warnings still produces an exe.
#
# It is here because a conversion of the AI object arrays to a type of
# their own was announced as complete on the strength of "every source
# compiles" -- which was run with -w, and grepped for "error". Twenty
# sites were left passing a pointer array where a slot array is
# wanted, among them the collision list builders that fill those
# arrays. Nothing failed to build, and nothing said so.
#
# One engine source has one and keeps it: d3drow.c wires
# D3D_DrawSprites, declared with a void* where the renderer table
# says Vec3*, into that table. The two are the same at every call
# site. Aligning the declaration is a one-word change and it
# renames the symbol the d3drender differential compares, so the
# frozen copy stops matching -- it is left as it is and listed.
#
# The six below are drivers and mocks that
# wire a mock of a near-enough signature into a table -- a return of
# int where the field says void, that sort of thing. They predate this
# check and each is a deliberate shortcut in a harness rather than a
# fault in the engine, so they are listed with their counts and the
# check fails on anything new or worse. An entry that drops to zero is
# reported too, so the list cannot quietly go stale.
ptr_baseline='
platform/d3drow.c:1
tests/d3drender_mocks.c:2
tests/gfxcore_mocks.c:1
tests/test_glrender.c:1
tests/test_profiler.c:6
tests/test_xmenmodifier.c:1
tests/winio_mocks.c:1
'

ptrbad=0
ptrcurrent=$(
for f in $files; do
    n=$($CC $CFLAGS "$f" 2>&1 \
        | grep -c -e 'incompatible pointer type' \
                  -e 'incompatible types when assigning' \
                  -e 'incompatible type for argument' || true)
    [ "$n" != "0" ] && printf '%s:%s\n' "$f" "$n"
done
true
)

for line in $ptrcurrent; do
    f=${line%%:*}
    n=${line##*:}
    was=$(printf '%s\n' $ptr_baseline | grep "^$f:" | cut -d: -f2)
    if [ -z "$was" ]; then
        printf 'PTR   %-28s %s mismatch(es), not in the baseline\n' "$f" "$n"
        ptrbad=$((ptrbad + 1))
    elif [ "$n" -gt "$was" ]; then
        printf 'PTR   %-28s %s mismatch(es), was %s\n' "$f" "$n" "$was"
        ptrbad=$((ptrbad + 1))
    fi
done

for line in $ptr_baseline; do
    f=${line%%:*}
    printf '%s\n' $ptrcurrent | grep -q "^$f:" || \
        printf 'PTR   %-28s is clean now -- take it off the list\n' "$f"
done

if [ "$ptrbad" -ne 0 ]; then
    echo "$ptrbad file(s) pass a pointer of the wrong type"
    exit 1
fi
echo "no mismatched pointer types"
