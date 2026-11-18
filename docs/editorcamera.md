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
back. `assignedGao`, the viewport, the flags and the display data are
left exactly as the game set them. So everything that asks the view
what camera it has — `AI_EvalFunc_VIEWGetObject` among them — gets the
same answer it would have got, and the game's own camera logic carries
on running against the object it always had.

That function is also where the game is kept out of the camera's
position, sight and field of view, which is the next section.

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

## Keeping the game out of the camera

`WOR_SetCam` is the one place a view's matrix, focale and viewport
reach the device, and it rebuilds the projection matrix from them. The
engine calls every view's follow function from the view loop in
`engine.c` and `WOR_SetCam` during the render that follows, so **what
the follow function leaves behind is what gets drawn**. That makes it
the place to enforce all three, and it is already ours.

**Position and sight** need only the matrix imposed, which the follow
function was doing anyway. They reach a view through exactly one
function — `WOR_View_SetViewPoint` — and it has exactly two callers in
the tree: `WOR_View_FollowFather`, which the camera displaced, and the
camera's own follow. `WOR_View_Init` is the only other writer and it
runs when a view is created. So there is no third path to close, and
the test's check is the one that matters: the follow puts the
camera's matrix back over whatever the view held a moment before.

**The field of view is different**, and it was the real gap. `focale`
is a plain field on the view that the game writes directly:
`AI_EvalFunc_VIEWFocaleSet_C` sets it and *then* calls the view's
follow function, which is to say ours. So the follow notes what was
asked for and puts the camera's own value back:

```c
if (view->focale != ecam_focale)
    ecam_gameFocale = view->focale;
view->focale = ecam_focale;
```

`AI_EvalFunc_VIEWFocaleSet_C` goes on to push the game's value into
`displayData->ct.cam.focale` and rebuild the projection — and that
does not matter, because `WOR_SetCam` overwrites both from
`view->focale` before anything is drawn. `WOR_View_SetFocale` writes
the field without calling the follow at all, and the next frame's
follow catches it before the render.

**Standing down gives back what the game last asked for**, not what
it had when the camera took the view. The same reasoning as the
after-effects: the camera withholds the change while it is up rather
than cancelling it, so a cinematic that narrowed the field of view
while the camera was flying gets its narrow field when the camera
hands back. With nothing asked for meanwhile, the value it took comes
back unchanged. Both are checked.

The camera's own field of view is the one it took and does not change;
**F9 still syncs position, sight and banking only.**

## The input gate

While the camera has the input, the game must not see any — and the
game reads input in two places, so it is stopped in two.

**Through INO**, the engine already has the mechanism:
`INO_l_JoystickMask` is what it clears for controls it wants ignored,
and `INO_Joystick_Move` returns nothing and the pad array is zeroed
for anything outside it. So `INO_SuspendInput(1)` takes that mask and
clears it, and `INO_SuspendInput(0)` puts back what it took rather
than the `-1` that would look close enough. A second suspend does
nothing and a spare release does nothing, so the mask it took cannot
be overwritten with the cleared one and lost.

**But the mask is not only set by the game, it is saved and
restored**, and a snapshot is not enough because of it.
`AI_EvalFunc_IoJoyFreeze_C` *returns* the mask in force so that a
script can freeze some controls and put back what it found. Taken
from the live mask while the camera holds it, what the script finds
is the suspension — and it then restores that later as though the
game had chosen it, which leaves the controls frozen after the camera
has handed back. The write is the same fault the other way round: a
script re-enabling its controls would open the mask the suspension is
holding shut, and the input would leak to the game mid-flight.

So the game reads and writes **its own copy** while the input is
suspended, through `INO_i_GameJoystickMask` and
`INO_GameJoystickMaskSet`:

```c
int INO_i_GameJoystickMask(void)
{
    return ino_inputSuspended ? ino_gameJoystickMask : INO_l_JoystickMask;
}
```

It gets back exactly what it last set, the suspension stays whole in
between, and release hands over **the game's latest intent** rather
than the value from when the camera took the input. The same shape as
the after-effects and the field of view: the change is withheld, not
cancelled. With nothing suspended both accessors are the live mask, so
every existing caller is unchanged.

`AI_EvalFunc_IoJoyFreeze_C` is those two accessors and nothing else,
and `INO_Reinit` uses the setter for the same reason.

The keyboard needed one line of its own: `INO_Keyboard_Update` returns
early while suspended, leaving `INO_Keys` as the `memset` at the top
of it already left it.

**In the platform layer**, `pcINO_Joystick_Update` stops calling the
selected control mode's handler at all while the input is taken, so
the pad the game goes on to read stays as that function's own opening
`memset` left it.

Zeroing the pad *after* letting a handler run would look equivalent
and is not. Two of the handlers **accumulate** — the wheel's notches
in `pcINO_case2_mouseScroll`, the pointer's travel in
`pcINO_case4_mouseMoveX/Y` — so the running total would sit there
through the whole time the camera was up and arrive in the game in one
lump the moment the input was given back. The test drives every one of
the eight handlers with the gate closed and checks all three
accumulators come back untouched, and then drives them with it open to
show that at least one of them really does move.

The two `pcINO_IntKeybDataLink` pad overrides that sit beside the
handler call are inside the same gate, for the same reason.

**Not covered, deliberately:**

- `INO_v_MemoryCardManager`, which `INO_Update` calls separately, so
  **saving and loading carry on** while the camera is up.
- Alt+F4 and the command field's key, which are read in the same
  function above the gate:
  quitting and the command field keep working.
- the control mode and submode, which are neither read nor written.
  They stay as the game set them and stay visible to everything that
  reads them, and nothing about the camera's own controls changes when
  the game changes them.

The camera reads the keyboard backup array and the published mouse
delta — below both gates. That is why suppressing the game does not
suppress the camera, and it is a layering fact rather than a trick:
the gates sit between the platform and the game, and the camera is on
the platform side of them.

### The mouse delta

The control modes read the mouse as `mouseScaled`, which is the raw
counts times the sensitivity and then interpreted by whichever mode is
selected. The camera cannot use that — it is exactly the
control-mode dependency this was meant to drop — so the platform layer
publishes the raw counts as `pcINO_mouseDeltaX/Y` beside where it
computes `mouseScaled`. They are counts and not pixels, and the
buttons are not among them: those go into `pcINO_keyboardDataBackup`
above the 256 keys, with the wheel at 256 and 257 and the right button
at **259**.

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

### Which way is forward, and which way is right

**All three axes are derived, none assumed.** The view's matrix is a
camera-to-world placement — `WOR_View_FollowFather` fills it from the
object's absolute matrix, and `CAM_Inverse` is what later produces the
world-to-camera one — so its three rows are the camera's own axes in
world space. Which row is which comes from
`CAM_SetObjectMatrixFromCam`, which builds the camera device's matrix
as

```
objectMtx = ( -view.v1, -view.v3, -view.v2 )
```

and in that matrix `+x` is right, `+y` is up and `+z` is the sight:
`CAM_2Dto3D` unprojects a screen point into exactly that frame, and
`gameobject.c` takes the sight from `objectMtx.v3`. So

| | |
|---|---|
| forward | `-v2` |
| right | `-v1` |
| up | `-v3` |

All three negated — which is **not** what the first version of this
file did. It had forward right and the other two positive, so
**strafing and Q/E were both inverted**, and because the pitch input
was inverted as well the camera was unpleasant to fly in three
independent ways at once.

A single wrong sign is hard to see one axis at a time, since an
inverted axis still moves along the right line. What catches it is the
triad: in this engine `right × up` is the sight, and flipping any one
of the three breaks that identity. The test checks it on a camera that
has been turned and pitched first, so it is a check on the triad and
not on where the axes happen to start.

Turning is around the world's up axis, so the horizon stays level
however far the camera has been flown. Looking up and down is around
the camera's own lateral axis and is limited to a little under a
quarter turn, because a camera pointing exactly along the world's up
axis has no lateral axis left to turn around.

Both rotation senses come out of `math.c` rather than from taste.
`MATH_RotateMatrix_AroundGlobalZAxis` is `(x cos − y sin, x sin + y
cos)`, counter-clockwise about `+Z`, so turning **right** is a
**negative** angle. `MATH_RotateMatrix_AroundLocalXAxis` puts
`sin(angle)` into `v2.z`, and the pitch counter is `asin` of that same
component, so a positive angle both **looks down** and raises the
counter. The mouse reports Y growing downwards, which makes the
non-inverted sense the plain `+delta`.

The driver implements both rotations for real and they were checked
against `math.c` term by term, which is what lets the test assert a
*direction* rather than a magnitude.

### The controls

| | |
|---|---|
| `W` `A` `S` `D` | forward, back, strafe |
| `E` / `Q` | up, down |
| mouse | look |
| mouse, **right button held** | move, along the same axes as `W A S D` |
| `Left Shift` | five times faster |
| `Left Ctrl` | a fifth of the speed |
| **F8** | on and off |
| **F9** | take the game camera's position, sight and banking, once |

The mouse does one of the two, never both at once: on its own it
turns the camera, and with the right button held it moves it —
across with the pointer's x, as `A`/`D` do, and forward as the
pointer goes away from you, as `W`/`S` do. Each of those is checked
against the key that ought to agree with it rather than against a
remembered sign.

**Keys are a rate and the mouse is a displacement.** A key is scaled
by the frame time, so `ECAM_SPEED` is units a second; neither looking
nor panning is, so the same sweep of the hand does the same thing at
any frame rate. Both are checked by driving one movement at two very
different frame times. The speed modifiers scale all of it.

A key and the mouse at once add up rather than one of them winning,
which is also checked.

The constants are at the top of `editorcamera.c`: `ECAM_LOOKSCALE` is
radians per mouse count and `ECAM_PANSCALE` is units per mouse count.

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

F1 to F7 and F10 are taken by `PC_EngineCheatFinal` and Alt+F4 quits.
**F8, F9, F11 and F12** were unbound when this was written; F8 toggles
and F9 syncs. F11 has since gone to the overlay's command field, which
was on F12, and the engine's statistics are on Home.

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

`tests/run_editorcamera.sh`, 106 checks. It includes `editorcamera.c`
so it can read back which view was taken and where the camera is, and
it stands in for the engine around it.

The rotation helpers in the driver are **real, not stubs**. A stub that
did nothing would leave every check about which axis the camera moves
along passing on code that moved along the wrong one, which is the
mistake most worth catching in a file like this.

### The controls

Twelve defects were injected into the reworked controls and all twelve
were caught: the lateral axis as `+v1` and the vertical as `+v3` —
the two that shipped — forward as `+v2`, the yaw sign flipped, the
pitch sign flipped, the look not guarded by the right button, the look
scaled by the frame time, the two mouse axes swapped, and each of the
three key pairs swapped.

**The button index needed two checks of its own.** The driver's look
helper presses whatever `ECAM_MOUSE_RIGHT` says, so changing that
constant moved the test with it and every look case went on passing.
A check that the constant is 259 and a case that presses the *left*
button and expects no look between them pin it down, and 258, 260 and
257 are all caught now.

### The field of view, and the pan

Thirteen more defects injected, all thirteen caught. For the field of
view: the focale not imposed at all, the game's value noted but not
replaced, the game's wish never noted, the camera's own value left
behind on release, and a fixed value imposed instead of the one the
camera took.

For the pan: each of its two axes inverted, look and pan swapped
round, each of its two terms dropped, the pan scaled by the frame
time, the modifier dropped from the pan, and the clock dropped from
the lift.

### The engine around it

Twenty-seven defects were injected one at a time and twenty-six were
caught:
the follow not substituted, the follow cleared instead of restored,
the input not taken, the input not given back, not starting from the
game's camera, the world-change guard dropped, strafe along the
vertical axis, the pitch limit dropped, yaw around the local axis
instead of the world's, the clock ignored, the speed modifiers
dropped, a view that is not being drawn taken, and a removed view
taken. Then, for the sync: the pitch not read back, the pitch
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

Its own cases live in `tests/run_d3daftereffects.sh`, the harness that owns
`d3daftereffects.c`, because that is where `D3D_PC_AE` is linked.
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
uses. Five defects there, each caught: the mask restored as `-1`
instead of as it was, the mask not cleared, no guard against a second
suspend, no guard against a spare release, and the keyboard gate
dropped. The file's differential test is unaffected, since nothing in
it suspends input: 2429 cases, 0 differing.

The game's own mask has five more, all caught: the getter always
reading the live mask and the setter always writing it — the two
halves of the bug this fixed — the getter always reading the game's
copy and the setter always writing it, and release opening everything
instead of restoring.

**What those cases drive is the accessor pair, not
`AI_EvalFunc_IoJoyFreeze_C` itself**, in the sequence that function
performs: read, invert, set the complement. That function is those
two lines and no harness owns `ai/ai_inofunctions.c`, so it is
covered by inspection and not by a test. Worth knowing rather than
assuming.

### The platform layer's half of the gate

`tests/run_winio.sh` is a differential harness, and the gate is new
behaviour with no frozen copy to be compared against — so it is
mocked open for the 77,000-odd differential cases, which are
unaffected, and `suspendedInputCase` drives the rewrite alone with it
closed.

Eighteen cases: two on the gate itself, and the eight handlers twice
over for the accumulators. Seven injected defects, all caught — the
gate dropped, the gate inverted, the mouse delta gated along with the
rest, the two published axes swapped, one of the two pad overrides
left outside the gate, the pad never cleared at all, and the keyboard
backup gated too.

**One mutation found a redundant line rather than a missing check.**
Removing the `memset` the first draft put in the suspended branch
changed nothing, because `pcINO_Joystick_Update` already clears the
pad in its first statement and nothing on the suspended path writes to
it. The line was dropped; the gate alone is the behaviour. The check
that the pad comes back clear is still worth its place — it is what
catches a pad override being left outside the gate, and that mutation
is caught.

**The test found one defect of its own making worth recording.** The
module's state outlives a case, so a case that left the camera on
turned it *off* with its first toggle and then measured nothing — the
strafe check read as a forward one and passed for the wrong reason.
Every case now starts from off.

## Open ends

- **The pointer is not captured or hidden.** The mouse reads the delta
  the platform layer already publishes, so the cursor goes on moving
  about the screen while the view turns and can reach the edge of it.
  Capturing it would mean taking a decision about the cursor that
  nothing has asked for yet.
- **The left mouse button does nothing.** Looking needs no button
  held, so the left one is free.
- **`INO_l_JoystickDir` is not shadowed** the way the mask is.
  `AI_EvalFunc_IoJoyDir_C` saves and restores it in the same style, so
  it has the same shape of problem in principle; the suspension does
  not touch it, so nothing is being withheld from the game and there
  is nothing to put back. It would only matter if the suspension grew
  to cover it.
- **The viewport is not held** the way the position, sight and field
  of view are. `WOR_SetCam` takes it from the view alongside them and
  `AI_EvalFunc_VIEWViewportPosSet_C` and `...SizeSet_C` write it, so
  the game can still reframe the camera while it is flying. Nothing
  has asked for that to stop.
- **`CAM_2Dto3D` disagrees with its two neighbours about the sign of
  screen Y.** `CAM_2Dto3DCamera` and `CAM_2Dto3DCamera2` both
  unproject with `centerY - y`, and `CAM_2Dto3D` uses `y - centerY`.
  The camera's axes here follow the two that agree, and the triad
  check above is consistent with them. It is worth a look on its own
  account, but it is not this camera's to fix.
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
