# The in-game overlay

Two of the engine's debug surfaces were Win32 windows living outside the
game's own window: the **Rasters** window on F1, which the profiler drew
into a device context with GDI calls, and the **command prompt** on F12,
which was a modal dialog box. Both worked, and neither was visible in
full screen, on a second monitor without hunting for it, or in a capture
of the game.

This is a screen-space layer drawn in Direct3D on top of everything
else, with the two surfaces rebuilt on it. No widget library and no
assets: every shape is a quad and every string goes through the debug
font the rasters code already creates.

## The pieces

| file | what it holds |
|---|---|
| `win/overlay.c` | the layer, the widget primitives and the two widgets |
| `win/overlay.h` | `OVL_Draw`, and the command field's four entry points |
| `win/wingdi.c` | `D3D_OverlayBegin` / `DrawQuads` / `End`, the pass itself |
| `win/wininputoutput.c` | F12, and `pcINO_RunTweakCommand` |
| `tests/test_overlay.c` | the unit test, 160 checks |

### Where the pass sits

`D3D_Flip` draws the overlay last, after `Rasters_Update`, so it is on
top of the frame and of the profiler's own bars. The three D3D entry
points live in `win/wingdi.c` rather than in the overlay because two of
the setters the pass needs — `D3D_RenderStateSet` and
`D3D_SetFixedFunctionFVF` — are file-local to `win/wingdi.c`. The
overlay only builds geometry and strings; nothing in it touches the
device.

`D3D_OverlayBegin` saves the renderer's cached state, sets up an
orthographic projection over the viewport with an identity view, turns
off Z, culling and lighting, turns on alpha blending, and clears the
four texture stages. `D3D_OverlayEnd` restores what it saved. The
round trip is checked: see "The tests" below.

### The profiler bars are untouched

`PRO_ProfilerDisplayHandler` and the raster bars `Rasters_Update` draws
into the frame are a different thing and are left exactly as they were.
What the overlay replaces is the **Rasters window** — the GDI text
panel — and the Win32 code for it is still in `profiler.c`, no longer
called. `engine.c`'s call to `PRO_ProfilerWindowHandler` is commented
out rather than deleted, with a line saying why, so the window can be
put back by uncommenting one statement.

## The widget primitives

Three of them, and they are all that either widget needs.

`OVL_l_Quad` appends six vertices to a batch of 512 quads. A quad with
a zero or negative extent is dropped, and the batch refuses a quad once
it is full rather than running past its end.

`OVL_l_RoundRectFill` is a centre block plus one horizontal strip per
corner step, with the strip's inset taken from the circle equation
(`r - sqrt(r² - dy²)`), five steps to a corner. There is nothing to
load and no curve to tessellate. A radius larger than half the box is
clamped to half the box, and a radius under a pixel degenerates to a
plain quad.

`OVL_l_RoundRectOutline` is a fill with a smaller fill of the inner
colour over it. That keeps the corner arithmetic in one place and needs
no line primitive, which Direct3D would otherwise draw at a different
pixel centre than the quads.

### Strings are queued, not drawn

`D3D_RastersDrawText` goes through `ID3DXFont`, which draws immediately
through its own sprite. A string issued while quads are still batched
would be painted over when the batch flushes. So `OVL_l_Text` queues
the string — up to 192 of them, 95 characters each — and `OVL_Draw`
emits the whole queue after the quad batch has gone to the device.
Getting this backwards is one of the mutations the test catches.

## The rasters panel

Against the **right** edge, `y = 60`, 320 wide, with a rounded
translucent backdrop and a green **Rasters** title. Its left is a
runtime value, `viewport width - 320 - 8`, so it follows the edge at
any resolution; a viewport narrower than the panel puts it at the left
margin rather than off-screen. The backdrop is emitted *after* the
rows, so it can be sized to what the walk actually produced, and it is
clipped to the viewport.

The two pages the window had are both here, and **F1 now steps through
them and off again**: nothing, the summary, the detail page, nothing.
The original alternated between the two pages for ever once it was up,
with no way back to nothing short of restarting the game. The cycle is
`OVL_RastersCycleView`, which `win/wingdi.c`'s F1 block calls; it is
in the overlay rather than in the key handler because the key handler
spins on `GetAsyncKeyState` and cannot be driven from a test, and the
state machine is the part worth testing. Every step restarts the walk,
so a raster that was drilled into does not survive the change.

The two pages, as the two flags select them:

- `PRO_rasterWindowIsOn` alone is the **summary**: the frame time and
  the tick delta, both scaled by `gfDBGRasterSetTo60`, exactly as
  `PRO_ProfilerWindowHandler` printed them.
- `PRO_rasterDetailPage` is the **detail page**: the group, subgroup
  and module walk out of `PRO_ProfilerDisplayHandler`, row for row,
  with the column positions the window used (`10` for a subgroup
  heading, `20` for a module, `80` when a single raster has been
  drilled into, and `x + 150` for the value).

The row function carries all five forms a raster's value can take —
int, string, nothing, float, and the default, which is a time. The
timing form has a **side effect**: reading it moves `sumTime` into
`unk10` and zeroes the accumulator, so the walk is what advances a
raster from one frame to the next. The row also clears `unk7`, which is
how the walk marks a raster drawn. Both are preserved, and both are
checked by the test; dropping either leaves the panel looking right and
the profiler's numbers wrong.

## The command field

Full width at the top of the screen, `y = 10`, a rounded green 2-pixel
outline over a translucent backdrop, `> ` and the line in white, and a
caret that blinks every 20 frames. No assets.

**The field has a font of its own.** The rasters font is Arial at ten
pixels, which was right for a small GDI window and is unreadable as a
command line, so the overlay creates a second one: Courier New, at
`viewport height / 30` clamped to 14..44, rebuilt whenever that height
changes, which is what carries it across a resolution change. The box
is then as tall as the line it holds --- `font height + 2 x 6` ---
rather than the font having to fit a box fixed at 26. The panel keeps
the small font, because its column positions were laid out for it.

**The caret is placed by measuring, not by counting.** `ID3DXFont`
measures a string with `DT_CALCRECT`, and the caret goes one pixel
past where it says the text ends. The first version multiplied the
character count by an assumed 7-pixel cell, which was wrong in two
ways at once: the cell was too narrow, and the face is proportional,
so the error compounded with every character and the caret drifted
further from the text the more was typed. One measuring call per
frame is the cost, and it is the only thing that is right for an
arbitrary face.

### What F12 does now

```
F12 pressed (edge, and the tweak keys are not disabled)
  -> pcINO_dinput_mouse_and_keyb_unacquire()
  -> OVL_CommandShow()
```

DirectInput holds the keyboard and mouse **exclusively**, so while the
field is up it has to be released or no characters reach the window at
all. That is the whole reason the release is part of raising the field
rather than something the field could do without.

With the devices released the characters arrive as window messages, and
`PC_WNDMsgHandler` offers every message to `OVL_b_CommandMessage`
before anything else looks at it. While the field is up that function
consumes `WM_CHAR`, `WM_KEYDOWN`, `WM_KEYUP` and `WM_SYSCHAR` and
leaves everything else alone; while the field is down it consumes
nothing, so the handler runs exactly as it did.

| key | what happens |
|---|---|
| a printable character, 32..126 | appended, up to 127 characters |
| backspace | drops one character; a no-op on an empty line |
| Enter | runs the line, hides the field, re-acquires the devices |
| Escape | abandons the line, hides the field, re-acquires the devices |

Enter and Escape both end with
`pcINO_dinput_mouse_and_keyb_acquire()`, which is what gives the game
its input back. The field also refuses to re-raise itself while it is
already up, so holding F12 does not clear the line being typed.

### Running the line

The old F12 dialog parsed its text and applied the tweak inside
`pcINO_HandleTweakFlags`, in among the dialog bookkeeping. That parse
is now `pcINO_RunTweakCommand(const char* line)` — a name, a value, a
`bsearch` over the tweak table and one of the three stores the table's
`type` selects — and `pcINO_HandleTweakFlags` calls it. So the dialog
and the overlay field run the same code and accept the same syntax, and
the dialog still works if it is ever put back.

## The tests

`tests/run_overlay.sh` builds `tests/test_overlay.c`, which **includes
`win/overlay.c`** so it can read the quad batch and the text queue back
after a draw. The stub `windows.h` and
`tests/wingdi_msg_sdk.h` both give the four window messages the field
consumes their **real** SDK values, rather than the sequential ones the
stub uses elsewhere: `PC_WNDMsgHandler` opens with a range test on the
message number, so a made-up ordering would change what the handler
does. 160 checks over the command field's state machine, the three shape
primitives, the caret, the panel's placement and F1 cycle, its five
value forms and the walk, and the pass itself.

The font the test stands in is **deliberately proportional**, with
four different glyph widths, because that is the only way the caret
checks can see the defect they exist for: eight narrow characters and
eight wide ones are the same length, and a caret placed by character
count puts both in the same place.

It is a unit test rather than a differential one, because there is no
earlier copy of this file to compare against. That makes the
mutation check the only evidence it has teeth, so twenty-four
independent defects were injected into `win/overlay.c` one at a time and every one
of them was caught:

| mutation | check that fired |
|---|---|
| the line's bounds guard dropped | the line stops one short of the buffer |
| Enter does not re-acquire | enter re-acquires the devices |
| Escape runs the line | escape runs nothing |
| the radius not clamped to the box | an oversized radius is clamped |
| the timing row keeps its accumulator | reading a timing raster clears the accumulator |
| strings flushed before the quads | the strings are issued after the quads |
| the quad batch cap dropped | the quad batch refuses one more |
| the text queue cap dropped | the text queue refuses one more |
| the corner inset flattened | the rounded fill clips its corner |
| a drawn raster not marked | a drawn raster is marked |
| the subgroup heading never takes a row | a subgroup heading takes a row of its own |
| `OVL_Draw` with no widget up still draws | an idle overlay does not touch the device |
| the caret placed by character count | the caret is at the end of the measured text |
| the caret measured without the prompt | the caret is at the end of the measured text |
| the line drawn in the panel's small font | the command line uses the overlay font |
| the box height fixed again | the field grows with the font |
| the caret height fixed again | the caret is as tall as the font |
| the panel back on the left | the panel's right edge is one margin in from the viewport |
| the narrow-viewport clamp dropped | a narrow viewport puts the panel at the left edge |
| the backdrop left at a fixed x | the panel's right edge is one margin in from the viewport |
| the panel's text not moved with it | the panel's strings move with the panel |
| F1 never hides it (the old cycle) | three presses hide the panel |
| F1 skips the detail page | two presses reach the detail page |
| a step does not restart the walk | a step clears the drilled-into raster |

**One of those mutations was missed on the first attempt, and the
reason is worth keeping.** The cap check used to fill the batch to four
times its size and then compare the counter. Without the guard, that
writes over the counter which follows the array, and after a few
hundred overruns the counter landed back on exactly the passing value
by chance, so the check passed on the mutant. Filling the batch
*exactly* and then asking for one more is decisive, and that is what it
does now.

### The overlay's D3D pass

`tests/test_wingdi.c` gained `overlayPassCase`, which drives
`D3D_OverlayBegin` / `DrawQuads` / `End` over the mock device and
requires the renderer's cached state to come back **byte for byte**,
once and then again over the state the first pass left. A render state
or a texture stage left behind would corrupt the next frame's geometry
rather than the overlay, which is the kind of defect that is
near-impossible to find from the symptom. It also requires the hash to
move *inside* the pass, so the check cannot pass by the pass doing
nothing. Emptying `D3D_OverlayEnd` is caught, with the differing bytes
reported.

### What the F12 change cost the input harness

`win/wininputoutput.c`'s differential test compares the frozen
pre-rewrite copy against the current one call for call. The F12 path
now **deliberately** differs: the frozen copy puts up the Win32 dialog,
runs its whole dialog procedure, applies a tweak and fills the dialog
buffer; the rewrite releases DirectInput and raises the field. The two
cannot be lined up field by field.

So an F12 edge is skipped in `runCase` — 585 of 77,917 cases — and
checked directly instead by `f12FieldCase`, over four situations: the
edge, the key already held, the key not pressed, and the tweak keys
disabled. It requires the field to go up only on the first, with both
devices released, and it requires the **frozen copy never to raise the
field**, which is what shows the mock is not leaking between the two
sides. Three mutations were injected and all three were caught: the
release dropped, the edge check dropped, and the `tweakFlagsDisabled`
guard dropped.

Every case that is not an F12 edge is still compared in full, and the
suite is at 0 differences.

## Open ends

- The panel's width is fixed at 320, which is enough for the column
  positions the window used but will clip a long module name. The
  window clipped it too, at a different place. Now that the panel is
  right-aligned, a wider panel would grow leftwards, so the width
  could be made to follow the viewport if it ever needs to.
- Courier New is assumed to be installed. It is on every Windows since
  3.1, and GDI substitutes a default face if it ever is not, so the
  field degrades to a different-looking font rather than to nothing.
- The overlay font is never released on a device reset. Neither is the
  rasters font, which is the existing practice here; the height check
  rebuilds it after a resolution change, which is the case that
  matters.
- The command field has no history and no completion. The dialog had
  neither either.
- `PRO_ProfilerWindowHandler` is still in `profiler.c` and still
  creates its window class if it is ever called. Nothing calls it.
