#!/usr/bin/env python3
"""Check that a header sync changed parameter NAMES and not TYPES.

Step 4 of the pass propagates parameter names from each definition into
its prototypes, and the quick way to do that is to copy the definition's
whole parameter list over the prototype's. That also copies the types,
which is wrong twice over:

  * a prototype may spell a parameter `struct sFoo*` on purpose, because
    the `tFoo` typedef is not visible that early in the header. Copying
    `tFoo*` from the definition makes the header stop compiling -- and
    only for the files that include it in a particular order, so it
    looks like an unrelated breakage;
  * a type that genuinely differs between a prototype and its definition
    is a real defect worth seeing, not something to paper over.

So: run this after any header sync. It compares, per prototype, the
parameter type list before and after, and reports every one that moved.

    ./tests/hdrcheck.py origin/claude/sideload ai.h math.h dynamics.h
    ./tests/hdrcheck.py origin/claude/sideload $(ls *.h)

Exit status is 1 if any type changed.
"""

import re
import subprocess
import sys

PROTO = re.compile(r'(?m)^([A-Za-z_][^;(){}\n]*?)\b(\w+)\s*\(([^;(){}]*)\)\s*;')


def protos(text):
    return {m.group(2): m.group(3) for m in PROTO.finditer(text)}


def types(sig):
    """The parameter types of one signature, with the names dropped."""
    if not sig.strip() or sig.strip() == 'void':
        return []
    out = []
    for p in sig.split(','):
        p = re.sub(r'\b[A-Za-z_]\w*\s*(\[[^\]]*\])?$', '', p.strip()).strip()
        p = re.sub(r'\s+', ' ', p).replace(' *', '*').replace('* ', '*')
        out.append(p)
    return out


def check(rev, path):
    raw = subprocess.run(['git', 'show', '%s:%s' % (rev, path)],
                         capture_output=True).stdout
    if not raw:
        print('%-28s (not in %s, skipped)' % (path, rev))
        return 0
    old = protos(raw.decode('latin-1'))
    new = protos(open(path, encoding='latin-1').read())

    moved, gone, renamed = [], [], 0
    for name in new:
        if name not in old:
            continue
        a, b = types(old[name]), types(new[name])
        if a != b:
            moved.append((name, a, b))
        elif old[name] != new[name]:
            renamed += 1
    for name in old:
        if name not in new:
            gone.append(name)

    print('%-28s prototypes %3d  renamed %3d  types moved %d  gone %d'
          % (path, len(new), renamed, len(moved), len(gone)))
    for name, a, b in moved:
        print('   TYPE MOVED %s\n      before %s\n      after  %s' % (name, a, b))
    for name in gone:
        print('   gone: %s' % name)
    return len(moved)


def main():
    if len(sys.argv) < 3:
        print(__doc__)
        return 2
    rev, paths = sys.argv[1], sys.argv[2:]
    return 1 if sum(check(rev, p) for p in paths) else 0


if __name__ == '__main__':
    sys.exit(main())
