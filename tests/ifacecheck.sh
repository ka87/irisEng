#!/bin/sh
# Every renderer fills every entry of the interface.
#
#   ./tests/ifacecheck.sh
#
# A renderer is a row in gdi.c's table plus a function that writes
# tGdi_DisplayData, and an entry that function leaves alone is a NULL
# pointer the engine will call -- a crash, and not where the mistake
# was made. Adding an entry to the interface means filling it in every
# row, and nothing about that is enforced by the compiler.
#
# So this reads the entry names out of gdi.h and checks each fill
# function in gdi.c assigns all of them. It is a text check on
# purpose: the fill functions are static, so driving them would mean
# linking the engine, and what can go wrong here is visible in the
# source.

set -e
cd "$(dirname "$0")/.."

python3 - <<'PY'
import re
import sys

gdic = open('gdi.c', encoding='latin-1').read()
gdih = open('gdi.h', encoding='latin-1').read()
i = gdih.index('typedef struct sGdi_DisplayData')
j = gdih.index('} tGdi_DisplayData', i)
entries = [m.group(2) for m in
           re.finditer(r'^\s*([A-Za-z_][\w \*]*?)\(\*\s*(\w+)\)\s*\([^;]*\);',
                       gdih[i:j], re.M)]
if not entries:
    print('ifacecheck: found no interface entries -- has gdi.h moved?')
    sys.exit(1)

# One file per renderer that does not live in gdi.c, plus gdi.c
# itself. A renderer whose row is written elsewhere -- which is every
# renderer gdi.c cannot name the types of -- has to be listed here, or
# this check would quietly stop looking at it and still say every
# renderer fills every entry.
SOURCES = ['gdi.c', 'platform/d3drow.c']

# How many renderers there are is gdi.c's table, not this list: a file
# dropped from the list above would otherwise take its renderer out of
# the check and still report that every renderer fills every entry.
# The table says two, so two fill functions have to be found.
table = re.search(r'gdi_renderers\[\]\s*=\s*\{(.*?)\n\};', gdic, re.S)
if not table:
    print('ifacecheck: found no renderer table in gdi.c')
    sys.exit(1)
WANT = len(re.findall(r'^\s*\{\s*"', table.group(1), re.M))
if WANT < 1:
    print('ifacecheck: the renderer table in gdi.c has no rows')
    sys.exit(1)

fills = []
for path in SOURCES:
    text = open(path, encoding='latin-1').read()
    found = re.findall(r'^(?:static )?void (\w*Fill\w+)\(struct sGdi_DisplayData\* dd\)\s*\{(.*?)\n\}',
                       text, re.S | re.M)
    if not found:
        print('ifacecheck: found no fill function in %s' % path)
        sys.exit(1)
    fills += found

if len(fills) != WANT:
    print('ifacecheck: gdi.c lists %d renderer(s) and %d fill function(s) '
          'were found -- is a renderer\'s file missing from SOURCES above?'
          % (WANT, len(fills)))
    sys.exit(1)

bad = 0
for name, body in fills:
    assigned = set(re.findall(r'dd->(\w+)\s*=', body))
    missing = [e for e in entries if e not in assigned]
    print('%-22s %d of %d entries' % (name, len(entries) - len(missing), len(entries)))
    for one in missing:
        print('    MISSING  %s' % one)
        bad += 1

print()
if bad:
    print('%d entry/entries unfilled' % bad)
    sys.exit(1)
print('every renderer fills every entry (%d entries, %d renderers)'
      % (len(entries), len(fills)))
PY
