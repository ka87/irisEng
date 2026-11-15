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

gdih = open('gdi.h', encoding='latin-1').read()
i = gdih.index('typedef struct sGdi_DisplayData')
j = gdih.index('} tGdi_DisplayData', i)
entries = [m.group(2) for m in
           re.finditer(r'^\s*([A-Za-z_][\w \*]*?)\(\*\s*(\w+)\)\s*\([^;]*\);',
                       gdih[i:j], re.M)]
if not entries:
    print('ifacecheck: found no interface entries -- has gdi.h moved?')
    sys.exit(1)

gdic = open('gdi.c', encoding='latin-1').read()
fills = re.findall(r'static void (GDI_l_Fill\w+)\(struct sGdi_DisplayData\* dd\)\s*\{(.*?)\n\}',
                   gdic, re.S)
if not fills:
    print('ifacecheck: found no fill functions in gdi.c')
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
