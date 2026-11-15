# datafiles.ini

Three things used to be compiled in: the big file name, binarized
mode, and the one video the fullscreen video function could play.
They now come from `datafiles.ini` in the working directory.

```ini
[data]
bigfile   = sally_clean.bf
binarized = 1

[video]
0 = BGEMakingOf.bik
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

**Which id each video is wanted under is decided by the scripts, and
nothing in this tree says what they are.** No behaviour in
`ai/ai_bge` calls that function, so the shipped file puts the one
known video under `0` as a starting point. That is why the log names
the id it wanted: run it, read the line, move the entry to that key.

An id with no entry, or an entry naming a file that will not open,
**plays nothing**. Both are written to the debug output and the video
simply does not start:

```
GFX_FS_VideoStart: no video for id 3 in datafiles.ini
GFX_FS_VideoStart: cannot open "missing.bik" for id 0
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

A `[video]` key that is not a number is reported rather than taken as
id 0, which is what `atoi` alone would have done — `making_of =
...` silently becoming id 0 is exactly the sort of thing that looks
like it works.

## The tests

`tests/run_datafiles.sh`, 65 checks. New code with no earlier copy,
so it is a direct check. The driver includes `datafiles.c` so it can
point the reader at files it writes itself, and **one case reads the
shipped `datafiles.ini`** — so a typo in that file fails here rather
than in game.

Fifteen defects were injected one at a time and all fifteen caught: a
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
