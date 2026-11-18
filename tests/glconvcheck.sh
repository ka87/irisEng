#!/bin/sh
# Every GL function pointer carries the platform's calling convention.
#
#   ./tests/glconvcheck.sh
#
# On Windows every GL and WGL entry point is __stdcall, and so is a
# callback the driver invokes. A pointer declared without it is called
# as though the caller cleans up a stack the callee has already
# cleaned, so the stack is wrong after every single call. MSVC's
# run-time checks report the first one as "the value of ESP was not
# properly saved across a function call" and nothing reports the rest.
#
# No test can see this. The whole GL list is reached through pointers,
# the harnesses build on Linux, and GL_APIENTRY is empty there -- so
# every case passes with the convention missing, which is exactly what
# happened: the renderer shipped with all of it wrong and the first
# call into the driver was what found out.
#
# So this is a check on the source text rather than on behaviour, and
# the rules are derived from how a pointer is used rather than listed
# by name. A pointer needs the convention when the driver is at the
# other end of it: when it supplies the function, or when it calls
# ours. That is what picks GL_pfn_DebugProc
# up and leaves GL_pfn_GetProc alone -- that one is only ever given
# our own functions, so there is no exemption list here to go stale.
#
# A first version of this check missed three of the eight ways the bug
# can be written, including the one that was actually shipped: the
# declaration it had to police sits on a continuation line, and the
# scan read only the first line of a #define. Everything below works
# on logical lines with the continuations joined, and the macro-
# generated mocks are expanded rather than waved through on the
# strength of a sibling macro being right.

set -e
cd "$(dirname "$0")/.."

echo "=== GL calling convention ==="

python3 - <<'PY'
import glob, re, sys

fails = []
checked = 0


def logical(text):
    """The file as logical lines: backslash continuations joined, each
    paired with the line number it starts on. A #define whose body is
    on the next line is one entry, which is the whole point -- reading
    only the first physical line is how this check first missed the
    bug it exists to catch."""
    out = []
    raw = text.split('\n')
    i = 0
    while i < len(raw):
        start = i
        acc = raw[i]
        while acc.endswith('\\') and i + 1 < len(raw):
            i += 1
            acc = acc[:-1] + ' ' + raw[i]
        out.append((start + 1, acc))
        i += 1
    return out


def defines(text):
    """Every #define in the file, as (line, name, params, body)."""
    for line, one in logical(text):
        m = re.match(r'\s*#\s*define\s+(\w+)\(([^)]*)\)(.*)$', one)
        if m:
            yield (line, m.group(1),
                   [p.strip() for p in m.group(2).split(',') if p.strip()],
                   m.group(3))


engine = sorted(glob.glob('platform/*.c') + glob.glob('platform/*.h'))
harness = sorted(glob.glob('tests/*.c') + glob.glob('tests/*.h'))
src = dict((p, open(p, encoding='latin-1').read()) for p in engine + harness)

core = src['platform/glcore.h']

# ---------------------------------------------------------------- 1
# The macro exists and says __stdcall on Windows and nothing elsewhere.
checked += 1
if not re.search(r'#if\s+defined\(_WIN32\)\s*\n\s*#define\s+GL_APIENTRY\s+'
                 r'__stdcall\s*\n\s*#else\s*\n\s*#define\s+GL_APIENTRY\s*\n'
                 r'\s*#endif', core):
    fails.append("platform/glcore.h does not define GL_APIENTRY as __stdcall under"
                 " _WIN32 and empty otherwise")

# ---------------------------------------------------------------- 2
# Every macro that writes a GLFN_ pointer's type. A macro that names
# both the pointer and its parameter list is declaring, defining or
# casting that type; one that names the pointer alone is only
# assigning or testing it, and needs nothing. That distinction is what
# separates GL_DECLARE, GL_DEFINE and GL_LOAD, which all need it, from
# GL_CLEAR and the harnesses' own GLFN_##name loops, which do not.
seen = 0
for path in engine + harness:
    for line, name, params, body in defines(src[path]):
        if 'GLFN_##' not in body or 'params' not in body:
            continue
        seen += 1
        checked += 1
        if 'GL_APIENTRY' not in body:
            fails.append("%s:%d: %s writes a GLFN_ pointer's type without"
                         " GL_APIENTRY" % (path, line, name))
checked += 1
if seen < 3:
    fails.append("found only %d macro(s) writing a GLFN_ pointer type, where"
                 " the declaration, the definition and the resolving cast are"
                 " each one; this check has stopped looking at something"
                 % seen)

# ---------------------------------------------------------------- 3 & 4
# The typedefs, and which of them the driver is at the other end of.
typedefs = {}
for path in engine:
    for m in re.finditer(r'typedef\s+[^;{]*?\(\s*(?:[A-Za-z_]\w*\s+)?\*\s*'
                         r'(GL_pfn_\w+)\s*\)[^;]*;', src[path]):
        typedefs[m.group(1)] = (path, src[path][:m.start()].count('\n') + 1,
                                m.group(0))

listing = re.search(r'#define\s+GL_FUNCTION_LIST\(X\)(.*?)\n\s*\n', core, re.S)
checked += 1
if not listing:
    fails.append("platform/glcore.h has no GL_FUNCTION_LIST to read")
    listed = ''
else:
    listed = listing.group(1)

# A typedef used as a parameter type in GL_FUNCTION_LIST is a callback
# the driver invokes.
for name in sorted(typedefs):
    if not re.search(r'\b%s\b' % name, listed):
        continue
    path, line, text = typedefs[name]
    checked += 1
    if 'GL_APIENTRY' not in text:
        fails.append("%s:%d: %s is a callback the driver invokes and its"
                     " typedef does not carry GL_APIENTRY" % (path, line, name))

# A typedef cast onto a resolved entry point is a function the driver
# supplies.
supplied = set()
for path in engine:
    for m in re.finditer(r'\((GL_pfn_\w+)\)\s*(?:\\?\s*\n\s*)?'
                         r'GL_p_Resolve\(\s*"(\w+)"', src[path]):
        if re.match(r'w?gl[A-Z]', m.group(2)):
            supplied.add(m.group(1))
# There is no such cast in the tree at the moment. That is a fact
# about how the context is made rather than a hole here:
# wglCreateContextAttribsARB was the only one, and it went when the
# context started coming from SDL -- which resolves every entry point
# through GL_FUNCTION_LIST, and section 2 above is what polices that.
# So an empty set is reported rather than failed, and any cast that
# appears later is still checked below.
#
# This guard did fire when the cast went, which is what it is for: a
# check that quietly stops seeing what it used to see is worse than no
# check. It is being answered rather than deleted.
checked += 1
if not supplied:
    print("  no typedef is cast onto a resolved entry point; the one there"
          " was went with the WGL context")
for name in sorted(supplied):
    checked += 1
    if name not in typedefs:
        fails.append("%s is resolved from the driver and its typedef was not"
                     " found" % name)
        continue
    path, line, text = typedefs[name]
    if 'GL_APIENTRY' not in text:
        fails.append("%s:%d: %s is a function the driver supplies and its"
                     " typedef does not carry GL_APIENTRY"
                     % (path, line, name))

# ---------------------------------------------------------------- 5
# A mock a harness assigns into a GLFN_ pointer has to have the
# renderer's type, or the harness is checking a different contract from
# the one that ships. On Linux the two are the same type whatever is
# written, which is why nothing else notices.
#
# Mocks written out by hand are read directly. Mocks a macro writes out
# are expanded: a macro whose body pastes an identifier from one of its
# parameters generates one name per invocation, so the names are worked
# out and each is tied back to the macro that wrote it. Accepting a
# generated mock because some other macro in the same file carries the
# convention is how this check first missed a whole family of them.
for path in harness:
    body_all = src[path]
    generated = {}
    for line, name, params, body in defines(body_all):
        if 'static' not in body:
            continue
        for m in re.finditer(r'(\w+)##(\w+)', body):
            prefix, param = m.group(1), m.group(2)
            if param not in params:
                continue
            at = params.index(param)
            for call in re.finditer(r'\b%s\s*\(([^)]*)\)' % re.escape(name),
                                    body_all):
                args = [a.strip() for a in call.group(1).split(',')]
                if at < len(args) and re.match(r'^\w+$', args[at]):
                    generated[prefix + args[at]] = (line, name, body)

    for nm in sorted(set(re.findall(r'GLFN_\w+\s*=\s*([A-Za-z_]\w*)\s*;',
                                    body_all))):
        if nm in ('NULL', '0'):
            continue
        checked += 1
        hand = re.findall(r'^[ \t]*static\s+[\w ]*?\**[\w ]*?\b%s\s*\(' % nm,
                          body_all, re.M)
        # A harness may also aim a GLFN_ pointer at a saved copy of
        # itself, to put the real one back after a case has stood in
        # for it. That copy is a pointer variable rather than a
        # function, and it has to carry the convention for the same
        # reason -- so its declaration is read the same way.
        saved = re.findall(r'^[ \t]*static\s+[^;\n]*?\(\s*([A-Za-z_]\w*\s+)?'
                           r'\*\s*%s\s*\)\s*\(' % nm, body_all, re.M)
        savedDecl = re.findall(r'^[ \t]*static\s+[^;\n]*?\*\s*%s\s*\)\s*\('
                               % nm, body_all, re.M)
        if hand:
            if not any('GL_APIENTRY' in d for d in hand):
                fails.append("%s: the mock %s is assigned into a GLFN_ pointer"
                             " and is not GL_APIENTRY" % (path, nm))
        elif savedDecl:
            if not any('GL_APIENTRY' in d for d in savedDecl):
                fails.append("%s: %s holds a GL entry point and its"
                             " declaration is not GL_APIENTRY" % (path, nm))
        elif nm in generated:
            line, macro, text = generated[nm]
            if 'GL_APIENTRY' not in text:
                fails.append("%s:%d: %s writes the mock %s, which is assigned"
                             " into a GLFN_ pointer, and is not GL_APIENTRY"
                             % (path, line, macro, nm))
        else:
            fails.append("%s: the mock %s is assigned into a GLFN_ pointer and"
                         " its definition was not found" % (path, nm))

for f in fails:
    print("  FAIL %s" % f)
if fails:
    print("\n  %d problem(s); on Windows these corrupt the stack on every"
          " call" % len(fails))
    sys.exit(1)
print("  every GL pointer and every GL mock carries the calling convention"
      " (%d checked)" % checked)
PY
