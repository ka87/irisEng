# The free camera

**F8** detaches the camera from whatever it was following and flies it
around the level while the game keeps running. F8 again gives
everything back.

It is built on two mechanisms the engine already has, rather than
beside them. That distinction is the whole design, so it is worth
setting out what each one is.

## The follow parameter

A view follows its assigned object through
`tViewElem.viewFunctionPtr`. The engine calls it once a frame, and
`WOR_View_FollowFather` fills the view's camera matrix from the
object's absolute matrix. That pointer *is* the follow parameter.

Turning the camera on swaps it for a function that fills the view from
the free camera's own matrix, and turning it off puts the original
back. Nothing else about the view is touched: `assignedGao`, the
focale, the viewport, the flags and the display data are left exactly
as the game set them. So everything that asks the view what camera it
has — `AI_EvalFunc_VIEWGetObject` among them — gets the same answer it
would have got, and the game's own camera logic carries on running
against the object it always had.

### Why the pointer is replaced and not cleared

Clearing it looks like the obvious way to unbind a camera, and the
engine itself does exactly that in `AI_EvalFunc_VIEWRemove`. But two
callers in the same file — `AI_EvalFunc_VIEWFocaleSet_C` and
`AI_EvalFunc_VIEWCulled_C` — invoke `viewFunctionPtr` **without
testing it first**. A script setting a focale or asking whether an
object is culled would dereference NULL.

That is a defect in the tree already, reachable today by removing a
view and then setting its focale. It is not touched here, but it is
the reason this camera substitutes rather than clears: a view that
follows something else is still a view that follows, and every
existing caller keeps working. The unit test calls the restored
pointer with a guard in front of it precisely so that getting this
wrong reports a failure instead of a segfault — which is what it did
before the guard was added.

A happy consequence: those AI-driven direct calls now *refresh* the
free camera rather than fighting it. Calling a view's follow function
three times in one frame gives the same matrix three times, which the
test checks.

## The input gate

While the camera has the input, the game must not see any. The engine
already has the mechanism: `INO_l_JoystickMask` is what it clears for
controls it wants ignored, and `INO_Joystick_Move` returns nothing and
the pad array is zeroed for anything outside it.

So `INO_SuspendInput(1)` saves that mask and clears it, and
`INO_SuspendInput(0)` puts back **exactly what it took** rather than
the `-1` that would look close enough. A second suspend does nothing
and a spare release does nothing, so the saved mask cannot be
overwritten with the cleared one and lost.

The keyboard needed one line of its own: `INO_Keyboard_Update` returns
early while suspended, leaving `INO_Keys` as the `memset` at the top
of it already left it.

**Not covered, deliberately:**

- `INO_v_MemoryCardManager`, which `INO_Update` calls separately, so
  **saving and loading carry on** while the camera is up.
- the control mode and submode, which stay as the game set them and
  stay visible to everything that reads them. The gate calls nothing
  in the platform layer at all, which is what the input harness
  checks.

The camera reads the device through `pcINO_Joystick_Move` and the
platform keyboard array — below the gate. That is why suppressing the
game does not suppress the camera, and it is a layering fact rather
than a trick: the gate sits between the platform and the game, and the
camera is on the platform side of it.

## Flying

There is nothing to switch off. No collision is consulted, no dynamics
run and no sector or portal is tested, because none of them are on the
path between the input and the matrix — the camera is a matrix and a
clock. Everything else in the level keeps updating meanwhile.

### Which way is forward

Not a guess. `CAM_SetObjectMatrixFromCam` fills the camera device's
`objectMtx` from the view's matrix with the rows permuted and negated,
and `gameobject.c` takes the camera's sight vector from `objectMtx.v3`.
Following that through, the view matrix's forward direction is
**`-v2`**. The other two rows are the lateral and vertical axes, and
their *signs* are the one thing here that a first run settles rather
than the code: if strafing or rising comes out inverted, it is a minus
in `ECAM_l_Axes`.

Turning is around the world's up axis, so the horizon stays level
however far the camera has been flown. Looking up and down is around
the camera's own lateral axis and is limited to a little under a
quarter turn, because a camera pointing exactly along the world's up
axis has no lateral axis left to turn around.

### The controls

| | |
|---|---|
| left stick, `W` `A` `S` `D` | forward, back, strafe |
| right stick, arrow keys | look |
| `E` / `Q` | up, down |
| `Left Shift` | five times faster |
| `Left Ctrl` | a fifth of the speed |

Speeds and rates are the constants at the top of `editorcamera.c`.
A key counts as a full stick deflection, which the test checks, so the
two can be mixed.

## Why F8

F1 to F7 and F10 are taken by `PC_EngineCheatFinal`, F12 by the
overlay's command field, and Alt+F4 quits. **F8, F9 and F11** were
unbound; F8 is the first of them.

## Standing down safely

A level change closes every view and clears its follow pointer, which
would leave the camera holding a pointer the engine has let go of.
Two things prevent it:

- `ECAM_Release` is called from `engine.c` before the world resets its
  views, so the camera gives the view back while it is still valid;
- every `ECAM_Update` checks that the world is still the one it took
  the view from and that the view's follow function is still ours. If
  either has changed the camera stands down without writing to the
  view at all.

Both are tested, including the case where the view was closed under it
— the check is that the closed view is **left** closed rather than
written to.

## The tests

`tests/run_editorcamera.sh`, 51 checks. It includes `editorcamera.c`
so it can read back which view was taken and where the camera is, and
it stands in for the engine around it.

The rotation helpers in the driver are **real, not stubs**. A stub that
did nothing would leave every check about which axis the camera moves
along passing on code that moved along the wrong one, which is the
mistake most worth catching in a file like this.

Fourteen defects were injected one at a time and every one was caught:
the follow not substituted, the follow cleared instead of restored,
the input not taken, the input not given back, not starting from the
game's camera, the world-change guard dropped, forward as `+v2`,
strafe along the vertical axis, the pitch limit dropped, yaw around
the local axis instead of the world's, the clock ignored, the speed
modifiers dropped, a view that is not being drawn taken, and a removed
view taken.

The gate has its own cases in `tests/run_io.sh` — the harness that
owns `inputoutput.c` — because that is where the real
`INO_SuspendInput` is linked, rather than the mock the camera's test
uses. Five more defects there, each caught: the mask restored as `-1`
instead of as it was, the mask not cleared, no guard against a second
suspend, no guard against a spare release, and the keyboard gate
dropped. The file's differential test is unaffected, since nothing in
it suspends input: 2418 cases, 0 differing.

**The test found one defect of its own making worth recording.** The
module's state outlives a case, so a case that left the camera on
turned it *off* with its first toggle and then measured nothing — the
strafe check read as a forward one and passed for the wrong reason.
Every case now starts from off.

## Open ends

- **The lateral and vertical signs**, as above: forward is derived,
  the other two are conventional and a first run settles them.
- **No mouse look.** The look axis is the right stick or the arrow
  keys. The mouse is read by the platform layer for the game's own
  purposes and taking it here would mean deciding what happens to the
  cursor; the arrow keys were the smaller change.
- **Nothing is drawn.** There is no indicator that the camera is
  detached and no read-out of where it is. The overlay could carry
  both.
- **The camera does not survive a level change.** It stands down
  rather than following into the new world, which is the safe
  behaviour and not necessarily the wanted one.
