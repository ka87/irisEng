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

## The after-effects

The game's after-effects are drawn for the camera the game is looking
through, so while the free camera is up they are skipped.

**Through a flag of its own, not the `ae_no` tweak.** The tweak has to
go on holding whatever it was set to; borrowing it would mean the
camera trampling a setting the player chose and then guessing what to
put back. `ECAM_gi_NoAfterEffects` is written only by `ECAM_Toggle`
and by the stand-down path, and it sits beside the tweak in the same
test in `D3D_PC_AE`:

```c
if (tweak_ae_no || ECAM_gi_NoAfterEffects)
```

**And the game goes on being told the state it chose.** That test's
body sets `PC_AE_MASTER` to zero, which is not only a local decision:
the game reads it back through `AI_EvalFunc`'s
`PC_AE_MASTER_GET_ONOFF` and `PC_AE_MASTER_GET_P1`, so leaving it at
zero would tell every script its effects had been turned off behind
its back. So the camera's suppression lasts for the length of the
pass and the value is put back at the end of it.

The tweak is left exactly as it was: it clears the state for good,
which is what a tweak is for, and when both are on the tweak decides.

Putting the value back cannot lose anything the pass did meanwhile.
`D3D_PC_AE` writes `PC_AE_MASTER` in one other place --
`PC_AE_MASTER &= ~0x40000u`, when a fade finishes -- and that sits
inside `if ((PC_AE_MASTER & 0x40000) != 0)`, which cannot be entered
while the value is zero. Every other write in the file is in a
function the pass does not call.

One consequence worth knowing: that fade is a one-shot, and while the
camera suppresses the pass it is never consumed. A fade in progress
when the camera goes up resumes when the camera comes down, rather
than having quietly finished in the meantime. That follows from
reporting the game its own state rather than a suppressed one, and it
is the behaviour to want.

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
| **F9** | take the game camera's position, sight and banking, once |

Speeds and rates are the constants at the top of `editorcamera.c`.
A key counts as a full stick deflection, which the test checks, so the
two can be mixed.

## Snapping to the game's camera

**F9** puts the free camera where the game's camera is looking, once
per press: its position, its sight and its banking. The whole matrix
is adopted rather than an angle pair read out of it, which is what
carries the banking across.

The matrix comes from the view's assigned object, through the same
`OBJ_MakeAbsoluteMatrix` call `WOR_View_FollowFather` makes, so what
is adopted is exactly what the game's camera would be this frame.
Calling the saved follow function instead would also work and would
be shorter, but it writes the view and its previous viewpoint on the
way past, and this has to be a read.

**The pitch is read back out of the adopted matrix**, not reset. Reset
to zero, a sync to a camera already pitched down by a radian could
then be pitched a further whole limit on top of it, and the look limit
is about the world's horizon rather than about wherever the last sync
happened. Forward is `-v2` and the up axis is the world's Z, so the
sine of the pitch is `v2.z`.

That last step is the one place in this file where a sign could be
wrong without anything complaining, so the test does not check the
module's own pitch counter. It pitches a synced camera hard against
the limit and then measures **the matrix** -- a wrong sign sends the
camera a whole radian past vertical while leaving the counter looking
tidy, and the two injected sign defects are caught that way.

## Why F8 and F9

F1 to F7 and F10 are taken by `PC_EngineCheatFinal`, F12 by the
overlay's command field, and Alt+F4 quits. **F8, F9 and F11** were
unbound; F8 toggles and F9 syncs, leaving F11.

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

`tests/run_editorcamera.sh`, 76 checks. It includes `editorcamera.c`
so it can read back which view was taken and where the camera is, and
it stands in for the engine around it.

The rotation helpers in the driver are **real, not stubs**. A stub that
did nothing would leave every check about which axis the camera moves
along passing on code that moved along the wrong one, which is the
mistake most worth catching in a file like this.

Twenty-seven defects were injected one at a time and twenty-six were
caught:
the follow not substituted, the follow cleared instead of restored,
the input not taken, the input not given back, not starting from the
game's camera, the world-change guard dropped, forward as `+v2`,
strafe along the vertical axis, the pitch limit dropped, yaw around
the local axis instead of the world's, the clock ignored, the speed
modifiers dropped, a view that is not being drawn taken, and a removed
view taken. Then, for the sync: the pitch not read back, the pitch
sign flipped, only the position adopted, only the orientation
adopted, a sync with no object assigned, and a sync left running
every frame instead of once.

**The twenty-first is not observable, and the reason is the point.**
Removing the `!ecam_active` half of the sync's guard changes nothing:
releasing the camera clears `ecam_view` too, so the other half catches
it alone, and the module never reaches a state where one is set and
the other is not. The term is defence rather than a live requirement.
It is noted in the test beside the case that looks like it covers it,
so the next reader does not take that case for proof.

### The after-effects suppression

Its own cases live in `tests/run_wingdi_ae.sh`, the harness that owns
`wingdi_aftereffects.c`, because that is where `D3D_PC_AE` is linked.
Four situations: the pass as it runs normally, the camera's flag, the
tweak, and both at once. Two things are checked each time -- that the
effects really are skipped, measured as device writes (738 down to
59), and that `PC_AE_MASTER` comes back exactly as it went in for the
camera and stays cleared for the tweak.

Six more injected defects, all caught: the flag left out of the test,
the state never put back, put back even when the tweak had cleared
it, put back as zero rather than as it was, the flag never set on the
toggle, and the flag never cleared on standing down.

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
- **Only the after-effects are suppressed.** Anything else drawn for
  the game's own camera rather than for the view -- a HUD, a
  cinematic letterbox -- is untouched, because nothing has asked for
  it yet.
- **The camera does not survive a level change.** It stands down
  rather than following into the new world, which is the safe
  behaviour and not necessarily the wanted one.
