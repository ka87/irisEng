#!/bin/sh
# The console's tweak table is sorted, and every name in it is reachable.
#
#   ./tests/tweakcheck.sh
#
# The console looks a tweak up by bsearching the table in dbg.c, which
# is right only while the table is in strcmp order. A name out of place
# is a tweak that cannot be set at all -- typing it reports nothing and
# does nothing -- and nothing about that is enforced by the compiler.
#
# The console's own differential test cannot see it either: that harness
# links a table of its own, a synthetic sorted fixture, so both sides of
# the comparison search a correct table and agree. water_min_bv sat out
# of place and was unreachable for exactly that reason.
#
# So this reads the names out of dbg.c and does the search itself. It is
# a text check on purpose: the table is an initialiser and what can go
# wrong with it is visible in the source.

set -e
cd "$(dirname "$0")/.."

python3 - <<'PY'
import re
import sys

src = open('dbg.c', encoding='latin-1').read()
try:
    i = src.index('struct sTweakElem tweaks[')
    j = src.index('};', i)
except ValueError:
    print('tweakcheck: found no tweak table -- has dbg.c moved?')
    sys.exit(1)

names = [m.group(1) for m in re.finditer(r'\{\s*"([^"]+)"', src[i:j])]
if not names:
    print('tweakcheck: the tweak table looks empty')
    sys.exit(1)

bad = [(a, b) for a, b in zip(names, names[1:]) if a >= b]

def found(key, arr):
    """The search the console does: bisect on strcmp, nothing cleverer."""
    lo, hi = 0, len(arr)
    while lo < hi:
        mid = (lo + hi) // 2
        if arr[mid] == key:
            return True
        if arr[mid] < key:
            lo = mid + 1
        else:
            hi = mid
    return False

missing = [n for n in names if not found(n, names)]

print('=== console tweaks ===')
print('  %d names' % len(names))
for a, b in bad:
    print('  out of order: "%s" before "%s"' % (a, b))
for n in missing:
    print('  unreachable:  "%s" -- bsearch does not find it' % n)

if bad or missing:
    print('\nthe table must be in strcmp order, or the names above cannot be set')
    sys.exit(1)
print('  every name is in order and reachable')
PY
