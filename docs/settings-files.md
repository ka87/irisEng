# engine.ini and settings.ini

Everything the game is told before it starts. Some of it used to be
compiled in — the big file name, binarized mode, and the one video the
fullscreen video function could play. The rest came out of the Windows
registry, where the settings application put it, which is a place only
Windows has and only an installer fills.

**Two files, because they belong to different people.** What the
install is is one thing and what this player has chosen is another.

```ini
; engine.ini, beside the executable
[data]
bigfile      = sally_clean.bf
binarized    = 1
specialarray = jade.spe

[render]
backend = gl

[video]
0x490294C0 = BGEMakingOf.bik
```

```ini
; settings.ini, in the player's own folder beside the saves
[display]
adapter    = 0
mode       = 0
zbuffer    = 75
mipmapping = 1
windowed   = 0
;maxfps    = 100
width      = 640
height     = 480
```

A copy of `engine.ini` ships in the repository root. **It belongs
beside the executable**, which is where the game looks for it — not
the working directory, because a game started from a shortcut has
whatever working directory the shortcut says, and not the source tree.

`settings.ini` ships nowhere, because it is not the install's to
ship. It is written by the settings application, into the folder the
saves are in:

| | |
| --- | --- |
| Windows | `Documents\irisEng\settings.ini` |
| anywhere else | `~/.local/share/irisEng/settings.ini` |

The folder is created if it is not there yet, by whichever of the game
and the settings application gets there first. Two reasons for it
being there rather than beside the executable: it is a file that gets
**written**, and an install folder under Program Files is not somewhere
either program can count on writing — the game has already had to
write to the saves folder in order to save at all. And keeping the two
apart means replacing or copying an install does not take somebody's
resolution with it, and that their choices survive one.

A `[display]` section left in `engine.ini` by an older install is
ignored, and the reader says so by name in the debug output rather
than quietly obeying it. The install's sections in `settings.ini` are
ignored the same way, and for a sharper reason: a file in the player's
folder has no business saying where the game reads its data from.

Everything the game only reads is found that way: the big file, the
special array, this file itself, and `data/<folder>/` for the
scripts, the shaders and the fonts. The two things it writes — saves
and screenshots — go somewhere else, under the player's own
directory, because the install may be read-only and the saves are
the player's rather than the game's. `datapath.h` holds both rules
and nothing else does.

## The display

`[display]` lives in `settings.ini` and is the one section the game
cannot do without. The
Direct3D renderer needs `adapter`, `mode`, `zbuffer` and
`mipmapping`, and stops with a message box naming the file if any of
the four is missing — it has no idea which video card or display mode
to ask for otherwise. The settings application is what normally
writes them.

**`zbuffer` is a surface format, not a number of bits.** It is handed
to the device as the automatic depth-stencil format, so it carries a
Direct3D `D3DFORMAT` value: 75 is D24S8, 77 is D24X8, 80 is D16, 71
is D32. A format the card will not give is a device that will not come
up, which is why the settings application lists only the ones the card
reports. The shipped file says 75.

`windowed`, `width` and `height` are optional and default to
fullscreen at 640x480. `windowed` is the engine's own flag rather
than a property of the window: it is what the renderer reads when it
decides whether to take an exclusive display mode.

`maxfps` is how many frames a second the engine will draw. Leave it
out and the engine uses 100, which is not an arbitrary round number:
the clock counts any frame shorter than a hundredth of a second as a
hundredth of a second, so a frame that arrives sooner advances the
world further than real time did and the game runs fast — a fifth
fast at 120Hz, very nearly half again at 144Hz. Both renderers
present on the vertical blank, so without a limit the frame rate is
the display's and so is the error.

The game has always counted frames that way; it simply never met a
display this fast. Holding frames back to a hundredth of a second
leaves that arithmetic untouched and makes it honest again, and a
display at 60, 75 or 100 is unaffected either way. A higher limit
lowers the clock's floor to match, so the game still runs at the
right speed — at the cost of a constant the game shipped with; set it
to your refresh rate if you would rather have the frames. `0` turns
the limit off and restores the shipped behaviour exactly, fast
display and all.

## The special array

`specialarray` names the special array file, `jade.spe` by default,
and it is looked for beside the executable like the rest of the
install. It is named here rather than compiled in so a build of that
file under another name can be tried without rebuilding the game.

## Either file is optional

If one is not there, or cannot be opened, nothing is set from it and
the game runs on what the other and the command line say. That is not
an error and nothing is written about it, which is the one case where
the reader says nothing at all: everything else it cannot make sense
of, it names in the debug output and carries on past.

A missing `settings.ini` does stop the Direct3D renderer, but not as a
reader error — it is the four settings above being absent, and the
message the player gets is the one the game has always given when they
were.

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
GFX_FS_VideoStart: no video for id 0x490294C0 (1224905920) in engine.ini
```

The first version printed only the decimal, which left whoever read
it doing the conversion by hand before they could use it.

An id with no entry, or an entry naming a file that will not open,
**plays nothing**. Both are written to the debug output and the video
simply does not start:

```
GFX_FS_VideoStart: no video for id 0x490294C0 (1224905920) in engine.ini
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

`tests/run_settings.sh`, 100 checks. New code with no earlier copy,
so it is a direct check. The driver includes `settings.c` so it can
point the reader at files it writes itself, and **one case reads the
shipped `engine.ini`** — so a typo in that file fails here rather
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
