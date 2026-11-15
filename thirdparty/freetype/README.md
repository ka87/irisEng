# FreeType 2.13.3, vendored as source

What `font.c` rasterizes glyphs with. Committed so the project builds
from a fresh clone with nothing to download, and built as part of the
engine rather than linked as a DLL.

    freetype-2.13.3, from https://github.com/freetype/freetype
    tag VER-2-13-3

## No upstream file is edited

Not one. The build is steered entirely from outside the tree, which is
what makes updating it a copy rather than a merge:

| | |
|---|---|
| `FT2_BUILD_LIBRARY` | defined when compiling these sources |
| `FT_CONFIG_MODULES_H=<ftmodule.h>` | points at `iriseng/ftmodule.h` below |
| `-I iriseng -I include` | in that order |

`iriseng/ftmodule.h` is the one file here that is ours. It lists the
modules the build contains, which is what `FT_CONFIG_MODULES_H` exists
for; its own comment says which five and why.

## What is here

`src/` holds **138 files**, and the set is derived rather than chosen:
start from the thirteen `.c` files the engine compiles, follow every
`#include "..."` recursively, and that closure is what is committed. A
FreeType module is one amalgamation `.c` that includes its own
siblings, so compiling thirteen files pulls in the rest.

The thirteen, which are what a project file needs to name:

    src/base/ftsystem.c      memory and stream handling
    src/base/ftinit.c        FT_Init_FreeType, reads ftmodule.h
    src/base/ftdebug.c       the tracing hooks, inert in a release build
    src/base/ftbase.c        the core, which includes most of src/base
    src/base/ftbitmap.c      bitmap conversion
    src/base/ftglyph.c       the glyph object
    src/base/ftmm.c          multiple masters
    src/truetype/truetype.c  the TrueType driver
    src/sfnt/sfnt.c          the table reader
    src/smooth/smooth.c      the antialiased rasterizer
    src/autofit/autofit.c    the autohinter
    src/psnames/psnames.c    glyph names
    src/gzip/ftgzip.c        inflate, and FreeType's bundled zlib

`include/` is upstream's **in full**, unpruned. The sources reach it
with angle-bracket includes that resolve through FreeType's own macros,
so a closure over those is not something a text search can be trusted
to compute; a stale header is a quiet miscompile, and 1.5 MB of
headers is the cheaper mistake.

## Why ftmm.c and ftgzip.c, which look unnecessary

Neither is optional at this configuration, and both were found by the
link failing rather than by reading:

- `truetype.c` references `FT_Set_Named_Instance`, which lives in
  `ftmm.c`. The TrueType driver carries GX variation support and does
  not ask whether you wanted it.
- `sfnt.c` references `FT_Gzip_Uncompress`, for compressed tables and
  for an SVG document table, because upstream's `ftoption.h` defines
  `FT_CONFIG_OPTION_USE_ZLIB`. Turning that off would mean a modified
  copy of `ftoption.h`, which is a vendored file that then diverges
  and has to be re-merged at every update. 924 KB of self-contained
  inflate source is the better trade.

## Licensing

FreeType is dual licensed, **FTL or GPLv2**, and this project takes it
under the **FTL**: `docs/FTL.TXT`, with `LICENSE.TXT` explaining the
choice and `docs/GPLv2.TXT` present because upstream ships it. The FTL
asks that use of FreeType be credited in documentation, which
`docs/renderers.md` does.

`src/gzip` is FreeType's bundled **zlib**, which is separately licensed
under the zlib licence -- the notice is at the top of `src/gzip/zlib.h`
and `src/gzip/README.freetype` records what FreeType changed.

## Updating it

Copy a new `include/` and recompute the `src/` closure from the same
thirteen entry points. Nothing else should need touching unless
upstream moves a file between modules, which the link will say. Check
`iriseng/ftmodule.h` still names classes that exist -- a renamed module
class is a link error, not a silent change.
