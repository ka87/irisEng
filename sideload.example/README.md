# Sideloading a recovered script

Copy this folder next to the built exe and rename it `sideload`:

    <build output>\irisEng.exe
    <build output>\sideload\5E006324.ofc
    ...

On startup `SIDE_InitModule` lists the folder and notes every file whose
name is eight hex digits. From then on, loading one of those keys reads
the file instead of the `.bf` member, and runs it through the
interpreter even where the behaviour has been transcribed into C. The
debug output says what it picked up:

    SIDE: 0x5e006324 <- 5E006324.ofc
    SIDE: 4 key(s) overridden from ...\sideload\

With no folder there, nothing changes: the engine behaves exactly as it
did before.

## What is here

The five `main_logo` behaviours lifted back out of the retail 2003
exe. Four have bodies; `main_logo_ml_initnopad` (0x490289B1) is empty
in retail, so there is nothing to supply for it.

| file | behaviour | nodes |
|---|---|---|
| `3D003882.ofc` | `main_logo_ml_load` | 38 |
| `5E006324.ofc` | `main_logo_ml_init` | 244 |
| `3D003A94.ofc` | `main_logo_ml_init1` | 889 |
| `49027635.ofc` | `main_logo_ml_init2` | 2588 |

Each matches the compiled C call for call. The source they were written
from is in the decompiler's `retail/fct/`.

`extra/5E006325.ova` is the matching variable file for the `main_logo`
model, rebuilt and byte-identical to the one in the binarized world.
Subfolders are not scanned, so it does nothing where it sits; move it up
a level to sideload the model's variables too. The full game already has
this layout, so it is only worth doing to check the rebuild itself.

## Naming

The part before the first dot is the key, eight hex digits, optionally
`0x`-prefixed. The extension is ignored. A name that is not a key is
skipped, so notes and spare copies are safe to leave in the folder.

## interpret.txt

Keys listed there are interpreted from the archive's own nodes, with no
replacement file. Use it to take a transcribed behaviour out of the C
path and watch the original script run.

## A caveat

A sideloaded file is a standalone member, not a binarized one, so every
field is present. The loader turns `LOA_gb_SpeedMode` off while it
parses one and puts it back afterwards. Dropping in a chunk carved out
of a binarized world file will not work; it has to be the standalone
form.
