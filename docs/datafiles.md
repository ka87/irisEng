# datafiles.ini

Three things used to be compiled in: the big file name, binarized
mode, and the one video the fullscreen video function could play.
They now come from `datafiles.ini` in the working directory.

```ini
[data]
bigfile   = sally_clean.bf
binarized = 1

[video]
0x490294C0 = BGEMakingOf.bik
```

A copy ships in the repository root. It belongs next to the
executable — the working directory the game is started from, not the
source tree.

## The file is optional

If it is not there, or cannot be opened, nothing is set from it and
the game runs on its command line alone. That is not an error and
nothing is written about it, which is the one case where the reader
says nothing at all: everything else it cannot make sense of, it
names in the debug output and carries on past.

## The command line wins

The ini is applied **before** `ENG_ParseCommandLine` runs, and the
parser is what makes that enough:

- it only ever *sets* the flags it finds, so `/B` on the command line
  can add binarized mode but no parameter has ever been able to take
  it away;
- its closing `sscanf` writes `MAI_gst_BIGFileName` only when the
  line actually carries a bare file name, so a name given there
  replaces the ini's and no name leaves the ini's in place.

So the precedence falls out of the existing parser rather than being
arranged on top of it, and **no new command line parameters were
added** — which also means there is no way to turn *off* something
the ini turned on. `binarized = 0` in the file is how to not have it.

## The videos

`[video]` keys are ids and the values are files. The id is the one
the script passes to the fullscreen video function —
`AI_EvalFunc_GFX_FS_VideoStart` already took an int off the stack and
passed it on, where it had been ignored.

**The id is a 32-bit resource key, not a small index.** That is worth
being plain about because it decides how the file is written and what
the reader has to accept: an id may be given in hex with a `0x`
prefix or in decimal, the whole 32-bit range is matched, and a key
with the top bit set — which arrives as a negative `int` — matches
the key as written. `0x9E006771`, `2650826609` and `-1644140687` are
the same id and all three find the same entry.

The trailer launcher asks for `0x490294C0`, **confirmed from the
running game** rather than inferred. It chooses between two ids on a
global it tests and only this one has been seen, so if the other
branch is ever taken the log will name what it wanted. Some callers
pass a key held in a variable, so the id they ask for comes from
world data rather than from the script at all — which is the other
reason the log names the id. Run it, read the line, add that key.

**The log gives the id in hex as well as decimal**, because hex is
what gets pasted back into the file:

```
GFX_FS_VideoStart: no video for id 0x490294C0 (1224905920) in datafiles.ini
```

The first version printed only the decimal, which left whoever read
it doing the conversion by hand before they could use it.

An id with no entry, or an entry naming a file that will not open,
**plays nothing**. Both are written to the debug output and the video
simply does not start:

```
GFX_FS_VideoStart: no video for id 0x490294C0 (1224905920) in datafiles.ini
GFX_FS_VideoStart: cannot open "missing.bik" for id 0x490294C0
```

Returning without starting was already a supported path — the open
could fail before — so nothing downstream had to change for it.

## What the reader accepts

Sections and keys ignore case. Blank lines are skipped and `;` or `#`
begins a comment. Whitespace is trimmed from both the key and the
value. A value may be wrapped in double quotes, which is how to give
a name with spaces, or with a trailing space; a quote on only one end
is part of the name, and so is one in the middle.

`binarized` takes `1`, `yes`, `true`, `on`, or any non-zero number;
anything else is off.

A repeated video id takes the last value given. A setting with no
value counts as not set.

A `[video]` key that is not a valid id is reported rather than taken
as id 0, which is what `atoi` alone would have done — `making_of =
...` silently becoming id 0 is exactly the sort of thing that looks
like it works. So are an id past 32 bits in either base, a bare `0x`,
a negated hex id, and hex with anything after it.

## The tests

`tests/run_datafiles.sh`, 82 checks. New code with no earlier copy,
so it is a direct check. The driver includes `datafiles.c` so it can
point the reader at files it writes itself, and **one case reads the
shipped `datafiles.ini`** — so a typo in that file fails here rather
than in game.

Twenty-two defects were injected one at a time and twenty-one were
caught. For the ids: the 32-bit range unchecked, a `0x` prefix read
as decimal, hex not accepted at all, only the low half of the key
compared, a negative id not negated, a negated hex id accepted, and a
bare `0x` accepted.

**The range check for hex was a real gap found this way**, not just a
mutation: `strtoul` is 64 bits wide here, so the first version — which
only range-checked decimal — kept the low eight digits of a nine-digit
hex id and said nothing. The check now covers both bases.

**The twenty-second is not observable.** Narrowing the local the id is
parsed into from `unsigned` back to `int` changes nothing: the bit
pattern is the same and so is the comparison. It is a type-tidiness
point rather than a defect, and gcc does not warn about it here
either, so it is recorded rather than tested.

For the rest of the reader: a
missing file reported, a second read not starting clean, a key that
only starts with digits taken as an id, any key at all taken as an
id, the video table overrun, an over-long name not truncated, the
tail of an over-long line read as a setting, an unset big file and an
unset video returned as empty strings rather than nothing, one quote
enough to unquote, every `[data]` key taken as the big file, every
section taken as `[data]`, and comments read as settings.

**Two of those needed the case strengthening before they were
caught**, and both are worth recording because the first version
passed:

- The over-long line. The padding has to be sized so the read's cut
  falls *exactly* where a real setting begins. Any other length
  leaves the remainder starting mid-padding, which is rejected as an
  unknown key whether it was skipped or not — so the case passed
  either way.
- The unbalanced quote. The first case put the quote in the middle of
  the name, which the mutant does not touch. It takes a quote on
  exactly one end to tell the two apart.

**Not covered:** the precedence of the command line over the ini.
That lives in `WinMain` and no harness owns `main.c`. The two
properties it rests on are the parser's, and they are quoted above
from the code rather than assumed, but the ordering itself is checked
by inspection.
