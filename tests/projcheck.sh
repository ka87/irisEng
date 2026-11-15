#!/bin/sh
# Every engine source a test builds has to be in the Visual Studio
# project.
#
#   ./tests/projcheck.sh
#
# The tests compile their own file lists, written out in the shell
# scripts beside this one. The game is built by MSVC from
# irisEng.vcxproj. Nothing connected the two, so a new source file
# could be linked into every harness, pass every test, and not be in
# the build the user actually runs -- which is what happened twice:
# rasterstate.c and geosprite.c were added, tested, merged, and the
# game would not link.
#
# The rule is derived rather than listed: a file some test compiles is
# a file the engine needs, so it must be in the project. There is no
# list of exemptions to go stale -- a source nothing compiles is not
# claimed to be part of the build, and a dead copy left in the tree is
# ignored because no test asks for it.

set -e
cd "$(dirname "$0")/.."

echo "=== project file ==="

python3 - <<'PY'
import glob, re, sys, os

raw = open('irisEng.vcxproj', 'rb').read().decode('utf-8-sig')
listed = set(m.group(1).replace('\\', '/').lower()
             for m in re.finditer(r'<ClCompile Include="([^"]+)"', raw))

# What the harnesses build, which comes in two shapes. Most sources
# are compiled on their own with "-c <file>.c" in the shell script.
# Some are #included by their driver instead, because the thing under
# test is static to the file -- font.c and win/wingl.c are both like
# that -- and those count just as much. Looking for only the first
# shape missed font.c, which is how this was found out.
wanted = {}
for sh in sorted(glob.glob('tests/*.sh')):
    body = open(sh, encoding='latin-1').read()
    for m in re.finditer(r'-c\s+("?)([\w./\\-]+\.c)\1', body):
        p = m.group(2).replace('\\', '/')
        if p.startswith('tests/'):
            continue
        wanted.setdefault(p, set()).add(os.path.basename(sh))

for drv in sorted(glob.glob('tests/*.c')):
    body = open(drv, encoding='latin-1').read()
    for m in re.finditer(r'#\s*include\s+"((?:\.\./)+[\w./\\-]+\.c)"', body):
        p = os.path.normpath(os.path.join('tests', m.group(1))).replace('\\', '/')
        if p.startswith('tests/'):
            continue
        wanted.setdefault(p, set()).add(os.path.basename(drv))

missing = sorted(p for p in wanted if p.lower() not in listed)
gone = sorted(p for p in wanted if not os.path.exists(p))

for p in gone:
    print("  FAIL %s is compiled by %s and is not in the tree"
          % (p, ", ".join(sorted(wanted[p]))))
for p in missing:
    if p in gone:
        continue
    print("  FAIL %s is compiled by %s and is not in irisEng.vcxproj"
          % (p, ", ".join(sorted(wanted[p]))))

bad = len(missing) + len([p for p in gone if p not in missing])
if bad:
    print("\n  %d source(s) the tests build are not in the project; the"
          " game will not link" % bad)
    sys.exit(1)
print("  every engine source the tests build is in the project"
      " (%d of them)" % len(wanted))
PY
