# The two faces the engine draws text with

Both are **Liberation 2.1.5**, under the SIL Open Font License 1.1.
`LICENSE.txt` beside them is that licence, and it belongs with any
distribution of these files.

| file | stands in for |
|---|---|
| `LiberationSans-Regular.ttf` | Arial |
| `LiberationMono-Regular.ttf` | Courier New |

## Why Liberation and not something else

Because the engine asked for those two faces by name, and Liberation
exists to be **metric-compatible** with them -- same advance widths,
same ascender and descender. So text occupies the space it used to,
line breaks fall where they fell, and a caret measured by summing
advances lands where it landed. A font merely similar in appearance
would be a layout change wearing the clothes of a build change.

What that looks like in the files here, at 2048 units per em:
Liberation Mono advances **1229 (0.600 em) for every glyph**, which is
Courier New's pitch; Liberation Sans gives `A` 1366, `M` 1706, `W`
1933, `i` and `l` 455, space 569, which are Arial's. Those are the
numbers upstream matches deliberately, so an update is not a chance to
re-check them -- it is a reason to.

They also carry a licence that allows redistribution with a program,
which Arial and Courier New do not.

## What reads them

`font.c`, through FreeType, at a pixel size it chooses. It looks for
them beside the executable, under `data\fonts\`, and if a file is
missing it logs that once and draws no text -- nothing else in the
engine changes behaviour.
