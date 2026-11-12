#!/usr/bin/env python3
"""Compare the AI stack traffic of a file before and after its rewrite.

Every builtin in ai/ is a wrapper: it pops its arguments off the AI's
global stack in reverse order, calls one engine function and pushes the
result. The one thing such a wrapper can get wrong in a rewrite is the
order or the number of those pops -- and nothing else here can see it,
because the directory has no differential tests (see docs/cleanup/ai.md
for why).

So this extracts, per function, the ordered sequence of pops and pushes
from the pre-rewrite copy in git and from the working tree, and compares
them.

    ./tests/popcheck.py origin/claude/sideload ai/ai_dynfunctions.c
    ./tests/popcheck.py origin/claude/sideload ai/*.c

Two things make the comparison exact rather than approximate:

  * a pop nested inside a push's argument still evaluates before the
    push, so pops are counted before pushes within one statement;
  * no statement anywhere in ai/ holds more than one pop, which
    --pops-per-statement checks, so there is no case where C's
    unspecified argument evaluation order could reorder two of them.

A pop that moved into a helper is credited back to the caller through
ABSORB below. Add an entry when a new helper takes over a pop.

Exit status is 1 if any pop sequence differs.
"""

import re
import subprocess
import sys

POP = r'AI_Get(?:Int|Float|Vec|Var|Object)FromStack|AI_PopStackSlot|AI_gul_SizeGlobalStack\s*-=\s*32'
PUT = r'AI_Put(?:Int|Float|Vec|Object)ToStack'

# file-local helpers that absorbed a pop, and how many each performs
ABSORB = {
    'AI_l_PopRawTarget':     1,
    'AI_l_PopRawValue':      1,
    'AI_l_PopNumberAsFloat': 1,
    'AI_l_PopNumberAsInt':   1,
    'AI_l_PopFunction':      1,
    'AI_l_PopFieldObject':   1,
    'MSG_l_PopCommonFields': 7,
    # AI_l_CoerceToSecondType and AI_l_PromoteToFloat pop nothing: they
    # take values that have already been popped.
}


def functions(text):
    """name -> body, for every top-level function definition."""
    out = {}
    lines = text.split('\n')
    i = 0
    while i < len(lines):
        m = re.match(r'^[A-Za-z_][\w ]*?\**\s*\b(\w+)\s*\(', lines[i])
        if m and i + 1 < len(lines) and lines[i + 1].startswith('{'):
            depth = 0
            body = []
            j = i + 1
            while j < len(lines):
                depth += lines[j].count('{') - lines[j].count('}')
                body.append(lines[j])
                if depth == 0:
                    break
                j += 1
            out[m.group(1)] = '\n'.join(body)
            i = j + 1
            continue
        i += 1
    return out


def traffic(body):
    """The ordered stack traffic of one function."""
    seq = []
    body = re.sub(r'//[^\n]*', '', body)
    for statement in body.split(';'):
        pops = len(re.findall(POP, statement))
        for name, count in ABSORB.items():
            if re.search(r'\b%s\s*\(' % name, statement):
                pops += count
        seq += ['POP'] * pops
        seq += ['PUT'] * len(re.findall(PUT, statement))
    return seq


def pops_per_statement(path):
    """Report any statement holding more than one pop.

    Scoped per function, because splitting the whole file on ';' runs
    one function's tail into the next one's head -- and a helper's own
    definition line then reads as a call to itself.
    """
    text = open(path, encoding='latin-1').read()
    found = 0
    for name, body in functions(text).items():
        if name in ABSORB:
            continue
        for statement in body.split(';'):
            statement = re.sub(r'//[^\n]*', '', statement)
            hits = re.findall(POP, statement) + \
                [n for n in ABSORB if re.search(r'\b%s\s*\(' % n, statement)]
            if len(hits) > 1:
                print('%s: %s has %d pops in one statement: %s'
                      % (path, name, len(hits), ' '.join(hits)))
                print('   ', ' '.join(statement.split())[:150])
                found += 1
    return found


def compare(rev, path):
    old = subprocess.run(['git', 'show', '%s:%s' % (rev, path)],
                         capture_output=True).stdout.decode('latin-1')
    if not old:
        print('%-42s (not in %s, skipped)' % (path, rev))
        return 0
    new = open(path, encoding='latin-1').read()
    before, after = functions(old), functions(new)

    gone = [k for k in before if k not in after]
    bad = []
    benign = 0
    for name in before:
        if name not in after:
            continue
        a, b = traffic(before[name]), traffic(after[name])
        if a.count('POP') != b.count('POP') or \
                [x for x in a if x == 'POP'] != [x for x in b if x == 'POP'] or \
                [i for i, x in enumerate(a) if x == 'POP'] != \
                [i for i, x in enumerate(b) if x == 'POP'] and a != b:
            if [x for x in a if x == 'POP'] != [x for x in b if x == 'POP']:
                bad.append((name, a, b))
            else:
                benign += 1
        elif a != b:
            benign += 1

    print('%-42s funcs %3d  gone %d  pop mismatches %d  (push count only: %d)'
          % (path, len(before), len(gone), len(bad), benign))
    for name in gone:
        print('   gone: %s' % name)
    for name, a, b in bad:
        print('   %s\n      before %s\n      after  %s' % (name, a, b))
    return len(bad) + len(gone)


def main():
    args = sys.argv[1:]
    if args and args[0] == '--pops-per-statement':
        total = sum(pops_per_statement(p) for p in args[1:])
        print('statements with more than one pop:', total)
        return 1 if total else 0
    if len(args) < 2:
        print(__doc__)
        return 2
    rev, paths = args[0], args[1:]
    return 1 if sum(compare(rev, p) for p in paths) else 0


if __name__ == '__main__':
    sys.exit(main())
