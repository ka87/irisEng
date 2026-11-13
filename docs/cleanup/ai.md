# ai/

The AI interpreter and the builtins the game's scripts call. 32 files,
**31 finished**, meaning no `goto`, no decompiler-named local or
parameter, and they parse as standard C against the stub include set
with `-Werror=implicit-function-declaration`.

The one left, `ai_compiledscripts.c`, is the user's own hand-written
hook file rather than decompiler output and is deliberately out of
scope. One function elsewhere, `AI_EvalFunc_SAVEDataGet_C`, is left
byte-for-byte and is described below.

| file | lines | residue | gotos | state |
| --- | --- | --- | --- | --- |
| `ai_types.c` | 65 | | | done |
| `ai_netfunctions.c` | 93 | | | done |
| `ai_timefunctions.c` | 95 | | | done |
| `ai_categories.c` | 141 | | | done |
| `ai_debugfunctions.c` | 186 | | | done |
| `ai_pagfunctions.c` | 279 | | | done |
| `ai_eventtrackfunctions.c` | 333 to 325 | | | done |
| `ai_actionfunctions.c` | 358 | | | done |
| `ai_fields.c` | 362 | | | done |
| `ai_messagefunctions.c` | 410 to 356 | | 1 to 0 | done |
| `ai_lightfunctions.c` | 540 to 440 | | | done |
| `ai_worldfunctions.c` | 493 | | | done |
| `ai_sectorfunctions.c` | 494 | | | done |
| `ai_miscfunctions.c` | 541 to 521 | | | done |
| `ai_aifunctions.c` | 560 to 524 | | | done |
| `ai_viewfunctions.c` | 604 | | | done |
| `ai_animationfunctions.c` | 639 | | | done |
| `ai_gfxfunctions.c` | 648 | | | done |
| `ai_arrfunctions.c` | 754 to 774 | | | done |
| `ai_wayfunctions.c` | 777 | | 2 to 0 | done |
| `ai_inofunctions.c` | 884 to 877 | 125 | | done but one function |
| `ai_functions.c` | 885 | | | done, table only |
| `ai_materialandcolorfunctions.c` | 1,077 to 1,057 | | 1 to 0 | done |
| `ai_mathfunctions.c` | 1,371 to 1,201 | | | done |
| `ai_gameobjecttextfunctions.c` | 1,249 to 1,207 | | | done |
| `ai_dynfunctions.c` | 1,681 to 1,295 | | | done |
| `ai_soundfunctions.c` | 1,822 to 1,640 | | 7 to 0 | done |
| `ai_keywords.c` | 2,038 to 1,295 | | 10 to 0 | done |
| `ai_colfunctions.c` | 2,774 to 2,498 | 1,374 to 0 | | done |
| `ai_gameobjectfunctions.c` | 3,799 to 3,545 | 1,668 to 0 | | done |
| `ai_gridfunctions.c` | 606 to 505 | 305 to 0 | | done |
| `ai_compiledscripts.c` | 118 | 11 | | out of scope |

"Residue" is the count of lines carrying a decompiler name -- `a1`..`a9`,
`v1`..`vN` -- which is the measure used throughout this pass. Where a
file shows a figure it is accounted for below.

`ai_compiledscripts.c` is **the user's own hand-written hook file**, not
decompiler output: its locals and its Hungarian commentary are theirs.
It is left alone.

`ai_gridfunctions.c` was held back for a batch for a reason worth
recording: it is almost entirely pass-through into `grid.c` --
`GRI_Tag(grid, pos, a3, a4)`, `GRID_ShortestPath` with eleven arguments
-- and `grid.c` was still decompiler output, so nothing said what those
arguments meant. Naming them from this side would have been invention.
`grid.c` was done first instead; see [root.md](root.md).

`ai_inofunctions.c`'s 125 lines of residue are all inside one function
that is deliberately untouched; see its section. Its header keeps that
function's `a1`/`a2`, which is the directory's only remaining residue
line, because the definition it would be synced from is the one that is
left alone.

Three headers from earlier batches were found still carrying `a1`/`a2`
in prototypes whose definitions had been named properly --
`ai_actionfunctions.h`, `ai_timefunctions.h` and `ai_viewfunctions.h`,
fifteen prototypes between them. Step 4 of the pass had been applied to
the sources and not propagated. They are synced from the definitions
now, which is also how the three files of the third batch generate their
headers, so the two can no longer drift.

## Verification

There are no differential tests in this directory, and that is a
decision rather than an omission. Every file here is a table of thin
wrappers over the engine: a builtin pops its arguments off the AI stack,
calls one engine function and pushes the result. Standing one up means
standing up the interpreter, the game object tree, the world and
whichever engine subsystem is behind the builtin under test -- and the
test would then be measuring the mocks, not the wrapper.

What the directory gets instead is a check on the one thing a wrapper
can get wrong.

### The stack-traffic comparison

`tests/popcheck.py` extracts, per function, the ordered sequence of AI
stack pops and pushes from a file's pre-rewrite copy in git and from the
working tree, and compares them:

    ./tests/popcheck.py origin/claude/sideload ai/ai_dynfunctions.c

A pop that moved into a helper is credited back to its caller. Two
things make the comparison exact rather than approximate: a pop nested
inside a push's argument still evaluates before the push, and
`--pops-per-statement` confirms that **no statement anywhere in `ai/`
holds more than one pop**, so there is no site where C's unspecified
argument evaluation order could reorder two of them.

Across the eight files of the second batch -- 614 functions -- **no pop
sequence differs**. The dozen remaining differences are all one push
replacing two in an if/else, or one becoming several early returns.

### What else each file got

- **Every file parses as standard C** with
  `-Werror=implicit-function-declaration`, which catches a function
  renamed at one end only. 120 of the project's 137 first-party sources
  parse now, against 117 before this directory: `ai/ai_gfxfunctions.c`
  and `graphicsobjects/shadow.c` came in with the d3d9 stub additions
  the full-screen video path needed. `ai_soundfunctions.c` is the one
  file here that still does not, on a pre-existing gap in the
  `win/winsound.h` stub.
- **`ai_keywords.c`'s two coercion rules** were checked against the
  originals exhaustively rather than by reading: 7 type values by 13 bit
  patterns, and the four shift float paths separately, for **9,594 cases
  with no mismatch**.
- **`ai_gameobjecttextfunctions.c`** was checked a third way, mapping
  every engine call's arguments back to the pop ordinal they came from.
- **The residue count is zero per file**, so nothing was renamed
  halfway, and the function set is compared before and after so nothing
  was dropped.

Nothing here has been built under MSVC.

---

## The shapes that run through the whole directory

Four things repeat in nearly every file, and naming them once is most of
what this pass does.

### The wrapper

A builtin is two functions. The `_C` does the work on a game object, and
the wrapper the interpreter calls saves the current "ultra" object,
points it at the current game object, pops the arguments, calls the
`_C`, pushes the result and returns the next node:

```c
tAINode* AI_EvalFunc_ACTHasKit(tAINode* node)
{
    tGameObjectMain* gaoUltra;

    gaoUltra = AI_gpst_CurrentUltra;
    AI_gpst_CurrentUltra = AI_gpst_CurrentGameObject;
    AI_gpst_CurrentInstanceUltra = AI_gpst_CurrentInstance;
    AI_PutIntToStack(AI_EvalFunc_ACTHasKit_C(gaoUltra));
    return node + 1;
}
```

`node` and `gaoUltra` are the user's own names, taken from the two
builtins they wrote by hand (`AI_EvalFunc_DbgVarsCurToInit` and
`AI_EvalFunc_DbgTraceInt`). Some wrappers pop before the ultra swap and
some after; that is preserved as written, since the pop can read a
global the swap changes.

Some builtins also have a `_C_CURRENT` or a `_C_CURGLOB`, which is the
same work on the current game object or the current global effect id.
Where those were a copy of the `_C` body, they now call the `_C`.

### The object pop

Popping an object off the AI stack was written out at 218 sites across
the directory:

```c
AI_gul_SizeGlobalStack -= 32;
--AI_gul_SizeGlobalVarStack;
gao = *(tGameObjectMain**)&AI_gac_GlobalStack[AI_gul_SizeGlobalStack];
```

`ai_common.c` now has the two functions that are:

```c
// Unwinds one element and hands back the slot it occupied, for a value
// the stack keeps inline rather than behind a pointer -- a vector, say.
char* AI_PopStackSlot(void);

// the object counterpart of AI_PutObjectToStack
void* AI_GetObjectFromStack(void);
```

The slot-returning one matters: a few builtins take a `Vec3*` pointing
*into* the stack rather than copying the vector out, and
`AI_GetObjectFromStack` would hand back the first four bytes of the
vector instead. `PAG_ShiftParticles`, `WTR_GetDif`, `WTR_GetNrm`,
`GFXSetv` and `CreateExplosion` are the sites.

### The object returned to a script

Returning an object was written through a multi-word frame buffer:

```c
int v2[6];
v2[0] = (int)AI_EvalFunc_AIMainActorGet_C(v3);
AI_PutObjectToStack(v2, AI_gaw_EnumLink[40]);
```

The buffer widths were whatever the decompiler found in the frame — 3,
5, 6, 7, 9 and 13 ints for the same operation. `AI_gast_Types` says the
object type copies **four bytes**, so it is one pointer:

```c
actor = AI_EvalFunc_AIMainActorGet_C(actorIdx);
AI_PutObjectToStack(&actor, AI_gaw_EnumLink[40]);
```

### The table-entry tag

The engine's object tables mark a free entry by setting the low two bits
of its pointer to `01`, and the test for it appears in four of these
files, written `((int)p & 3) != 1` and once `((uint8_t)p & 3) != 1`.
Both are the low two bits either way; it is `(uintptr_t)` now, and a
file-local `AI_l_ObjectInUse` where it repeats.

---

## ai_actionfunctions.c

The action-kit builtins. Nothing but wrappers, and the first file to
settle the wrapper shape the rest of the directory follows.

## ai_categories.c, ai_types.c, ai_functions.c

The three dispatch tables: node categories, value types and the 886
builtins. Already close to source; `ai_functions.c` is the table and
nothing else.

## ai_fields.c

The 27 field accessors, in two families. A field of a value already on
the stack re-points the descriptor at the field's offset, retypes it,
and copies the value down beside it — and the first field of a value
sits at offset zero, which is why three of them make no copy. The two
repeated address computations are macros:

```c
#define AI_FIELD_TOP()  (&AI_gast_GlobalVarStruct[AI_gul_SizeGlobalVarStack])
#define AI_FIELD_SLOT() (&AI_gac_GlobalStack[AI_gul_SizeGlobalStack - 32])
```

A design field is the other family: the object is popped, resolved to a
game object, and its field block handed back as a variable of the
field's own type. That is `AI_l_PopFieldObject`.

## ai_pagfunctions.c

The particle generator builtins. Eleven `_C` functions that all begin
with the same generator lookup, and eleven wrappers. The unnamed
arguments are a generator id, a frequency, two flag masks, a float index
bounded at 35, and a shift vector.

## ai_eventtrackfunctions.c

The event-track builtins: play, stop, pause and time, per track and for
all of them. `a2` is a track index at every site but two.

`TRASetEnd` puts a track on its last key frame, and the frames-to-time
conversion there — `(float)(frames * 1.0 / 60.0)` — is the same one
`ANISetEnd` does for an actor's animation.

## ai_messagefunctions.c

The message queue: five slots per game object, with senders, readers and
a by-id search. 410 lines to 356, one `goto` gone.

Four things were one thing written five times:

- the queue push, `if (count < 5) { memcpy(...); ++count; }` — now
  `MSG_l_Push`, which `MSGSend_C` is a null-check around;
- the by-id search, which `MSGGetByID_C` runs and pops from and
  `MSGReadByID` runs and leaves alone — the `goto` was the found arm of
  the second;
- the cleared global a reader hands back when it finds nothing;
- the seven fields `MSGSendAll` and `MSGSendAllOwner` pop in the same
  order, which differ only in where the eighth comes from.

### Pick — MSGOn's null test *(compiler artefact)*

```c
if ((gao->flags & 0x8000) == 0)
{
    if (gao) a2 = gao->flags; else a2 = 0;
    OBJ_ChangeIdentityFlags(gao, a2 | 0x8000, -1);
}
```

The null test is dead — `gao` was dereferenced on the line above it — and
it is what an inlined flags getter looks like after the inliner has run.
It is `OBJ_ChangeIdentityFlags(gao, gao->flags | 0x8000, -1)` now.

## ai_lightfunctions.c

The light builtins. 540 lines to 440.

Eight of them start with the same three-part lookup — the object exists,
carries the light flag, has a light — which is `AI_l_GetLight`.

**The near/far switch had three arms with identical bodies.** `SetNearFar`
wrote the same four lines for light kinds 0, 2 and 3, and `GetNear` and
`GetFar` each had a three-case switch returning the same field. It is
`AI_l_LightHasRange`, and the kinds it names are the ones where a range
means anything.

`LightGetAttenuationFactor` was five levels of nesting for four
outcomes; it is four early returns. Its byte-over-127 conversion keeps
the double it computes in, since the result is narrowed from it.

## ai_worldfunctions.c

The universe and world builtins. The universe table lookup —
`((tableElemStructPF*)(WOR_gst_Universe.univWorldTablePF.firstElemPtr))[i].objectPtr`
cast to `tWorld*` — appears ten times and is `AI_l_UniverseWorld`.

Two builtins return an interpreter **control code** in place of a node:
`WORReinit` returns 3 and `WORPlayAbsTime` returns 1, both cast to
`tAINode*`. That reads as a bug and is not one, so the file says so.

`WORUniverseDestroy` takes two arguments and uses the second; preserved,
with a line saying it is deliberate.

### Pick — WORCheckSetOrder's index *(open)*

The function counts **in-use** entries while walking the object table,
then indexes the raw array with that count. With a hole anywhere before
the two objects it is reordering, it swaps the wrong pair of slots.
Preserved as written; it is a property of the original, not of this pass,
and whether the table can have holes at that point is a question for the
table manager.

Its two parameters are declared `tWorld*` while the table it searches is
a world's **object** table. The comparison is pointer identity, so the
type does not reach the behaviour, and it is left as declared rather than
retyped on a hunch.

## ai_sectorfunctions.c

Sector registration, membership and the door cull.

The sector lists are one flat array in hundred-wide slices, so the
capacity is a constant now. Adding and removing an object are
`AI_l_AddToSector` and `AI_l_RemoveFromSector`, the second of which
`GRPSectUpdateMe` does twice.

**Invisibility is a count, not a flag.** `if_this_not0_wont_render_gao`
is raised and lowered rather than set, and -1 clears it outright; three
copies of that three-way are `AI_l_ApplyInvisible`.

### GRPCullDoor

The big one: it narrows the view frustum onto each door the script
supplies and raises the invisibility count of everything the narrowed
frustum culls.

The decompiler had **forty-one locals** for it, most of them pointer
aliases standing in for inlined vector helpers. Those are the calls they
are: `MATH_VecAddVecWithoutBuffer` for the corner translation,
`MATH_DotProduct` for the two dot products, `MATH_VecInvert` for the
plane flip, and a plain struct assignment for the two Vec3 copies. Each
was checked against the helper's body first — the add is commutative
per component and the dot product sums in the same order, so all three
are exact.

The eight bounding-box corners are built in one function.

**The seventeen-element array is a plane.** `Vec3 v44[17]` with the
user's `TODO FIXME ennek nem kéne hogy array legyen` against it only
ever touches `v44[0]`; it is `planeDown`, which answers the TODO.

Two oddities are preserved with a line each: a door seen more than
~72 degrees off axis `return`s out of the whole function rather than
skipping that door, and two direction vectors are computed, normalised
and never read.

## ai_miscfunctions.c

Groups, the PC audio-effect hooks, three modifiers, the water export and
the PC-version adjustments.

`GRPInvisibleSet` and `GRPInactiveSet` were the same twenty lines with
one call different, so the group walk takes the per-member setter as an
argument.

**SNAKE_SetInertia's loop was seeded with a null cursor.** The original
runs a `while (cursor <= end)` starting at `NULL`, uses the first
iteration to handle the object itself, and sets the cursor to
`firstElemPtr - 1` inside that iteration so the increment lands it on the
first group member. It is the object and then its skeleton group now,
with the per-object work in a function.

**WTR_Enable's condition was an exclusive or.**
`if ((!enabled || !a1) && (enabled || a1))` is "the state is not already
this one", and that is what the file says.

Two builtins named after their addresses —
`AI_EvalFunc_pop_one_elem_put_zero_sub_4B4280` and
`AI_EvalFunc_WTR_put_zero_no_pop_sub_4B42C0` — are
`AI_EvalFunc_PopArgPutZero` and `AI_EvalFunc_WTR_PutZero`, with their
entries in `ai_functions.c` updated. There are **thirty-odd more
`_sub_<address>` names across the project**, including
`PC_AE_MASTER_GET_ONOFF_sub_4A37D0` which this file calls; those are a
sweep of their own, since each one's definition lives in a file that has
not had its pass yet.

### Pick — WTR_SetF's bound *(binary)*

The float parameters are addressed by position from `waveHeight`, and
only the upper bound is checked — a negative index writes in front of
the export block. Preserved; the file says which end is checked.

## ai_aifunctions.c

The AI instance builtins: tracks, callbacks, the main actors, and running
one behaviour from another.

**Two argument pops are not the plain ones.** A numeric argument that may
arrive as either type is popped through `AI_GetVarFromStack`, which runs
the value's own type function, and then read as a float converting from
int for **every type but float**:

```c
AI_GetVarFromStack(dest, &a2);
if (a2.type != 2)
    dest[0] = (float)*(int32_t*)&dest[0];
```

`AI_GetFloatFromStack` converts only for the int type and skips the type
function, so the two are not interchangeable. The int direction is the
same shape with the x87 round-through-a-magic-double trick. Both are
functions now, with a comment saying why the obvious replacement is
wrong.

The rest: popping a behaviour reference, the no-nodes `-1` entry
sentinel (four sites), the control-code return (two), and the track-range
narrowing that stopping a track does (two, and the second test there
deliberately sees the value the first loop left behind).

`AIExecute_C`'s tail compared a pointer against 7, then against 1, 3, 4
and 5 in turn; it is a switch on the control code.

## ai_viewfunctions.c, ai_netfunctions.c, ai_timefunctions.c, ai_debugfunctions.c

Wrappers, done in the earlier half of this pass. `ai_debugfunctions.c`
holds the two builtins the user wrote by hand, which is where this
directory's naming comes from.

## ai_animationfunctions.c

The animation builtins. The actor behind a game object —
`((tGameObjectActorObject*)(gao->basic->actorObjectOrVisualObject))` —
appeared **thirty times**, in three guarded forms: bare, with a null
check, and with the actor flag as well. Those are three functions.

**ANIBlendFrameMaxSet keeps every narrowing step.** It shortens the blend
animation while keeping the ratio between its lead-in and its length,
through eight `float` temporaries each written as `(float)(<double
expression>)`. Each cast is part of the result, so the rewrite names the
temporaries and changes nothing else.

A file-local `ret_negative_1a()`, called once, is the `-1` it returns.

## ai_gfxfunctions.c

The effect builtins and the full-screen Bink video path.

**An effect's id picks the list.** `id >= 1024 ? gfxCurrentList :
gfxCurrentListSortedAfterAE` was written out sixteen times, as a value in
twelve of them and as an address in four; it is two small functions.

The effect parameter setters' unnamed arguments are a **parameter index**
and a **value**: `GFX_Setf` indexes `ga_Paramf_First` and
`ga_Paramf_Number` with the first, which settles it.

The `_C_CURRENT` and `_C_CURGLOB` variants were each a copy of the `_C`
body with the id substituted; they call the `_C` now, which is also what
keeps the two Hungarian `FIXME`s about the HIWORD/LOWORD shuffle in one
place instead of three.

Three builtins are stubs that take their arguments and do nothing —
`GFXDepthBlur`, `GFXAddWorld` (which always reports failure) and three of
`GFXRequest`'s four arguments — and the file says so rather than leaving
them looking like oversights.

`GFXAddWorld_C`'s two parameters and `CreateExplosion`'s two floats are
unused or pass straight into `gfx.c`, which has not had its pass. They
are `unused1`/`unused2` and `arg1`/`arg2` with a line saying why, rather
than names invented from the call site.

## ai_wayfunctions.c

The waypoint-network builtins. Half the file was already the user's own
source — the recursive fill and nearest-of-position walks, the link
getters, the capability and design setters — and the work was in the two
decompiled functions and the wrappers.

### WAYShortWay

Dijkstra over the network, in 130 lines with six `MEM_p_Alloc`s of 4000
bytes, twenty-seven locals and two `goto`s.

The two working sets are the find: `v27`/`v15`/`v12` are the open set,
its costs and its predecessors, and `v25`/`v8`/`v7` the closed set's.
Named that way the loop reads as the algorithm, and both `goto`s are
`continue` — the first for a waypoint already closed, the second the
"not in the open set, so append" arm of a search written as an infinite
`for` with a `break`.

Two conditions were worth pulling out. The grid test is one expression
with four assignments inside it, a comma operator and a ternary picking
between the world's two grids. The link cost is a nested pick between the
two main actors. Both are functions whose names say what the parameter
bits mean.

**The allocations are counts times `sizeof` now.** `MEM_p_Alloc(4000)`
for an array of 1000 pointers is right only because the target is
32-bit; the pointer arrays are `AI_SHORTWAY_MAX * sizeof(tGameObjectMain*)`,
which is the same 4000 under MSVC and correct anywhere else.

### WAYNetNextWP

Seven ways to pick the next waypoint, with
`a3->extended->way->wayElems[i]` written out twenty times. The way
element is a local, the three direction arms use `MATH_VecInvert` and
`MATH_DotProduct`, and the one variable that did double duty as "lowest
weight index" and "best dot index" is one `bestIdx` as before.

The empty-list check stays **per case** rather than being hoisted: with
an empty link list and a negative index, case 6 reads `innerElems[-1]`,
and hoisting the check would change that.

### Pick — the missing network *(open)*

`WAYNetNextWP_C` and `WAYGetNumLinks_C` both search a waypoint's way
elements for the network they were given and then use `wayElems[i]`
whether or not they found it, reading one slot past the end when they did
not. Preserved, with a line saying so.

## ai_arrfunctions.c

The array builtins: object searches over a script's array, and a bit
array addressed by bit number. 754 lines to 774 -- longer, because
twelve bit sites stopped being one-liners and because four preserved
oddities got a line each.

**The bit index was masked before it was shifted.**
`bits[(bit & 0xFFFFFFE0) >> 5] |= 1 << (bit & 0x1F)`: the mask clears
exactly the bits the shift discards, including for a negative index,
where the arithmetic shift rounds the same way. So it is `bit >> 5`,
and the word and the mask are two macros the file's twelve bit sites
share.

The three sight-scoring searches -- pivot, bounding-volume centre and
info-photo -- are one loop shape with a named falloff that trades angle
against distance, and their inlined dot products are the call.

### Pick -- the filtering builtins' destination *(binary)*

`ARRObjCopyIdFlags` and `ARRObjCopyCB` pop a destination array and a
source array, and then filter the **source** in place, compacting it and
returning the count. The destination is never written. Preserved, with a
line saying so, because every script that calls these has been getting
that behaviour.

### Pick -- ARRSortByFlags on a hole *(open)*

The partition advances its cursor when an object passes and shrinks the
range when it fails, but a **null** slot does neither, so an array with a
hole in it does not terminate. Preserved; whether the arrays these are
given can contain nulls is a question about the callers.

`ARRBit4Test`'s above-10000 convention asks for a bit to be clear, the
bit being the index less 10000 -- and an index of exactly 10000 passes
either way, since neither arm's comparison is true for it.

## ai_inofunctions.c

Input, the save card and the save-data accessors.

The three save-variable walks -- write the flagged variables to the
buffer, write a second set that shares the buffer, read them back --
share the storage computation that the top two bits of
`variableArrayCount` encode: the number of array dimensions, whose words
come first, and then the element count. The joystick freeze and
direction builtins return the setting that was in force so a script can
put it back, and `SAVESaveSlotExt`'s first argument is a slot id when it
is zero or more and a choice of three field modes when it is negative.

### AI_EvalFunc_SAVEDataGet_C is left byte-for-byte

Its second branch (`a2 >= 100`) builds an obfuscated save code through
three bit-permutation loops and then does two things that cannot both be
right:

- it returns **`v3[a2]`** with `a2` in 100..115 and `v3` declared
  `int[87]`, so the value handed to the script is whatever sits past the
  end of that array in the frame;
- its third loop writes **`v10[15]`** where `v10` is `int[11]`.

Because both reach outside their arrays, what the function does depends
on the stack frame layout -- so renaming its locals, which is free
anywhere else, would change its result. It is left exactly as it was
with a comment saying that, and everything around it in the file is
rewritten.

The shape says the decompiler split one buffer into several locals: in
the original, the bits built up in `v10` and the code read back out of
`v3` were almost certainly the same storage, and the otherwise-dead
`v3[86] = a2` in the other branch supports that. Reconstructing it is a
guess, so it is the user's call.

## ai_materialandcolorfunctions.c

Materials, their multi-texture levels, procedural and animated textures,
and the colour helpers. 1,077 lines to 1,057, one `goto` gone.

Twenty of these builtins begin by resolving a sub-material and then a
multi-texture level, and the two helpers that do it now say what their
arguments mean: a sub-material id whose top byte makes it a big-file key
rather than an index, and a colour selector of diffuse, constant or
ambiant.

Three decodings carry the file:

- **The multi-texture enable was four maskings through a 0xFFF window**,
  and it is setting or clearing bit 11, the level's disabled bit. Two
  lines now, and the same for the multi-material variant.
- **`GetFlag`'s `goto` was a shared tail**, and written out it shows
  something the jump hid: an element id of 1000 or more whose level is
  not found falls through and looks the **raw** id up again, walking
  that many links into the chain. Preserved, with a line.
- **The packed-colour blend adds 49152 to the factor** to pin the
  float's exponent at 2^15, which makes the low byte of its bit pattern
  the factor in 256ths. Each set bit then contributes one of the two
  colours shifted right by that place, with the channel's low bits
  masked off so a channel cannot borrow from the one below it; the seven
  terms sum to the blend, and the 1/256 bit is unused. The file explains
  that where it happens, because nothing in it is legible otherwise.

### Pick -- the level lookup's clamp *(binary)*

`MAT_GetMtxLvlPtr` clamps its multi-material index to the count where
`MAT_GetColorPtr` clamps to the count less one, so an index equal to the
count reads one past the end of the material array. Preserved, with a
line noting the difference between the two.

## ai_dynfunctions.c

The dynamics builtins: speed, mass, forces, limits, constraints and the
collision response. 1,681 lines to 1,295, and ninety-three functions.

**Fifty-odd of them opened with the same four lines** to reach the
object's dynamics through its basic block, which is one function now,
and the two sites that inlined that dance inside an `if` condition --
comma operators and an assignment in a ternary -- are written out.

### DYNImpulsion

The one function here with content: a ballistic launch solver, with
twenty-eight decompiler locals, three of them duplicates of their
neighbours. It reads as what it is now -- the apex sits a given height
above the higher of the start and the target, the fall time to each end
gives the flight time, and the vertical speed that reaches the apex
gives the launch -- with the inlined negate, scale and two dot products
replaced by the math calls, each checked against the helper's body
first. Where the target is directly above, it skips the rotation frame
and takes the horizontal part straight from x and y.

It also lifts both speed limits to `3.402822e38`, which is very nearly
but not quite `FLT_MAX`; `WAYShortWay` uses the same constant.

### Pick -- the seven null dereferences *(binary)*

Four of these builtins write through a null pointer, or hand one to
`AI_PutVecToStack`, when the object has no solid or no collision:
`DYNTorqueVectorSet`, `DYNTorqueVectorGet`, `DYNRotSpeedVectorSet` and
`DYNApplyRectFactorSet`. Three more -- `DYNFlagsGet`, `DYNReboundGet`
and `DYNRotSpeedVectorGet` -- take the **address** of a field before
testing that address for null, so the test cannot fire. All seven are
preserved with a line each; they are a property of the original, and the
objects these are called on evidently have the blocks.

`DYNSlideGet` answers 1.0 rather than 0.0 when there is no collision,
which is the neutral value for a slide factor rather than a mistake.

## ai_gameobjecttextfunctions.c

The text builtins: a game object's string, its colours, its page and its
substrings. 1,249 lines to 1,207.

The payoff came from reading `strings.c`, which is still decompiler
output but whose **bodies name their own arguments**:
`STR_l_GetValidIndex` and `STR_l_GetValidNumber` appear in nearly every
one of these, which settles the convention the whole file is addressed
by -- a text builtin takes `(strIdx, firstChar, charNum, value)`.

Four readings that convention does not cover:

- `STR_ChangePage`'s last argument is a **filter on the character's
  current page**, not a second page: negative moves every character,
  0 to 255 only those already on that page, and 256 to 511 only those
  *not* on page `matchPage - 256`.
- The two `SetSubString` variants count **line breaks** and **marks**
  rather than characters, so they are `(firstLine, lineNum)` and
  `(firstMark, markNum)`.
- `STR_l_GetSubLength`'s third argument is a **line index**, with
  negative meaning the longest line.
- `STR_SetColorExt`'s sixth argument is a **mask** on the colour.

No helpers were added, deliberately: the repeating shape is "get the
string, and if it exists call one thing on it", but the callee and its
arity differ every time, and these `_C` functions already are the thin
wrapper.

### Pick -- the convert buffer *(binary)*

`get_a_string_from_globalstack`'s convert buffer was `tTextGet[6]`,
48 bytes. Eight is what a text value writes, so the original was ample;
it is the directory's standard `char converted[52]` now, which only
matters if a script ever passes a message where a string is expected.
This is the one change in the file beyond renaming.

Preserved with a line each: `OBJ_TextColorGet` alone among its dozen
siblings does not null-check the string it has just looked up, four
builtins pop arguments they discard, and the three position getters pass
a scratch vector to a function that fills it and returns it, then copy
the result onto itself.

## ai_mathfunctions.c

The maths builtins. 1,371 lines to 1,201, and eleven functions the file
defined but never declared are now in the header.

**No new helpers**, because every repeated shape here was an *inlined*
math helper: the fix was calling the real one, each checked against its
body in `math.c` first. One site that looks like `MATH_VecLenSquared` is
not -- that helper sums its terms in the other order -- so it became
`MATH_DotProduct(axis, axis)`.

- **`MATHMatrixBlend`'s eighth argument** is the callee's
  `dontUseStoredValues`, which explains the whole function: its two
  matrices are file statics because the callee can carry state between
  calls, and the flag says to refill them from the arguments. That also
  pins the orientation convention its six vector parameters are named
  from.
- **`MATHVecRotate` is Rodrigues rotation** with the axis-parallel
  component split off and added back -- and it normalises the caller's
  axis in place, after using the un-normalised one for the projection.
- **`MATHVecInCone`'s flag word** says which of its two inputs are
  already normalised, and whether to return a unit vector or one of the
  input's own length.
- `MTHFloatSign` copies the sign bit onto `1.0f` rather than branching,
  so zero reports +1, negative zero included.

**Three restructurings were reverted for changing the answer on a NaN.**
In `MTHVecNullEpsilon` and `MTHVecNullToler` the second arm is a `<=`,
which answers FALSE for a NaN component where an inverted `>` would fall
through to test z; and in `MATHVecAbsAngle` a NaN cosine has to report 0
rather than reach `acos`. All three keep the original's branch nesting.

### Pick -- the two float-is-null builtins *(binary)*

`MATHFloatNullEps` compares the magnitude as a **double** while
`MTHFloatNullToler` narrows it to a **float** first, so the two disagree
on a value right at the boundary. Both kept as written, each noting the
other.

`MATH_f_VecAbsAngle`, which a comment says `MATHVecAbsAngle` inlines,
does not exist anywhere in the tree, so that one stays written out with
a line saying why.

## ai_keywords.c

The AI language itself: 67 keyword evaluators, one per node kind. 2,038
lines to 1,295, and the ten `goto`s gone -- the most in the directory.

**All ten were the same shared tail.** The five bitwise compound
assignments each jumped to a common "is this a float" line and then past
it. None was a loop, and once the coercion became a function they
disappeared without any restructuring.

The file collapses by a third because sixteen of its evaluators are two
rules:

- **the six comparisons and the ten compound assignments** coerce the
  first-popped operand towards the second's type. The stack pops in
  reverse, so the second operand popped is the left-hand one in the
  script's source order, which is what makes the direction legible and
  is written down at the helper;
- **the nine binary operators** promote to float when either side is
  one and convert the other side in place.

Both were checked against the originals exhaustively rather than by
reading: 7 type values by 13 bit patterns is **8,918 cases**, and the
four shift float paths another **676**, compared on the result's bits.
None differed.

Three things the file now says that the original did not:

- **The four conditional jumps are two kinds.** `JumpFalse` and
  `JumpTrue` pop the condition and push it back, leaving the value of an
  `&&` or `||` chain behind; the `SP` pair consume it. That is the
  short-circuit machinery.
- **Four evaluators return an interpreter control code** rather than a
  node: 0 yields the frame, 1 stops the behaviour, 2 returns from it and
  4 returns from the track. One comment at the top rather than four.
- **The bitwise operators have no float form**: on float operands they
  round both sides through the magic double, operate, and convert back.

### Pick -- the plain shifts' float path *(open)*

`SLeft` and `SRight` shift the **right** operand by the left one on
their float path, where their own int path shifts left by right. The two
compound-assignment shifts are consistent with the int path, so this
reads as a transcription slip rather than intent -- but it is what the
engine has been compiling, so it is flagged and not fixed.

Also preserved: `Not` of an int yields 0 or 255 rather than 0 or 1,
because it computes `(value != 0) - 1` truncated to a byte, while its
float form yields 1.0f or 0.0f as expected; and the two shift
assignments mask their shift count with `& 0xff` where the plain shifts
do not.

## ai_soundfunctions.c

The sound builtins: banks, instances, families, tracks and the sound
modifiers. 1,822 lines to 1,640, and the seven `goto`s gone.

**All seven were the same jump.** One arm after another of
`SND_ExtPlay`'s mode 14 jumped to a shared "report failure" tail; each
arm returns now. All sixteen values of the mode nibble are handled, so
the `default` is unreachable -- kept as written, with a line saying so.

Four shared runs became functions:

- **the four-part guard the six sound-starting builtins apply**, with
  its short-circuit order preserved: the count test precedes the
  non-negative test, so a negative index never reaches
  `SND_i_IsPlaying`;
- the two statements that hand an instance to a game object;
- **the sound-kind decode**, which was twenty-four lines of nested
  comparisons duplicated in two builtins and is one bit of a `0x86008`
  mask: 0 is 1, 8 is 5, 0x2000 is 3, 0x4000 is 2, 0x80000 is 4, and
  anything else is -1. A switch now;
- **the modifier-chain walk**, which takes the per-modifier work as an
  argument. That collapses two near-identical loops per site across
  five builtins, because a modifier id of -1 just means every one.

Names here are grounded in the callees' bodies rather than guessed,
since `sound.c` is still decompiler output: `SND_SetPan` already names
its arguments, the `Seti` family switch on the argument that is
therefore the field index, and `SND_i_TrackSeti` passes its first to a
search of the track id table. The Familly pair turn out to take **two
masks** -- an instance takes part when it has all of the first and any
of the second -- which is also why `SND_LinkToFamilly`'s arguments are
not reversed like its siblings'. `nearDist` and `farDist` rather than
`near` and `far`, since those are MSVC macros, which is presumably why
the original said `near1`/`far1`.

### Pick -- five asymmetries between siblings *(binary)*

Each preserved with a line, because each is a difference between
functions that otherwise match:

- `SNDGetInstanceModifier` closes its profiler timer only on the
  not-found path; a match returns from inside the loop without it.
- `SNDGetStatusModifier` is deliberately **not** written with the new
  modifier-walk helper: its all-modifiers arm ORs the flags together and
  reports them only if it saw one, and its by-id arm records a profiler
  tick once per modifier it steps over rather than once per call.
- `SNDPrefetchModifier` and `SNDFlushModifier` reach through the game
  object without the null check their three Play/Pause/Stop siblings
  have.
- The instance attach is guarded by `if (gao)` in two of the six
  starters and unguarded in the other four.
- `SNDSetFlagModifier` and `SNDResetFlagModifier` have a by-id arm that
  goes through the instance getter rather than walking the chain, so it
  touches one instance where their own all-modifiers arm touches every
  match.

### The stub gap that kept this file out of the parsing set, now closed

`ai_soundfunctions.c` did not parse against the stub when it was
rewritten, and that was never this file's doing: the eight errors all
came from `win/eax/eax.h` and `win/winsound.h`, which name `IUnknown`,
`DSCAPS`, `FAR`, `PASCAL`, `CDECL`, `LPDWORD` and
`IDirectSound3DListener`, and the stub's DirectSound header was four
opaque one-field structs. The error output was byte-identical before and
after the rewrite, and with those names supplied by a throwaway prelude
the rewritten file compiled with no diagnostics at all, so it did get a
real type-check.

That gap is closed: the stub now has the DirectSound interfaces written
out with the vtable methods and struct members the engine names, which
also brought in `ai_functions.c`, `ai_compiledscripts.c`, `sound.c` and
`win/winsound.c`. See [root.md](root.md) for the whole count.

## ai_gridfunctions.c

606 lines to 505, residue 0. Unblocked by `grid.c`, and the point of
waiting was that the arguments now have names read from the callees
rather than guessed from this side.

Two helpers, `AI_l_CurrentGrid` and `AI_l_DestGrid`, replace sixteen
written-out picks of which grid a builtin works on. Three builtins whose
unused game-object parameter was typed `int` by the decompiler now take
the object, which is what the wrapper passes them.

The cell-centre getter deliberately does **not** use `grid.c`'s new
`GRI_l_CellToWorld`: that helper converts a cell of the live window and
adds the window's origin, and this builtin converts a whole-grid cell,
which has no origin to add. The two look identical and are not.

`GRID_ShortestPath`'s seventh argument is unused by the callee, and this
file is where it is built: the cell cap with its low byte replaced by
another script argument. Both sides now say so.

---

## ai_colfunctions.c

2,775 lines to 2,498, residue 0 in both the source and the header. 188
builtins, 93 `_C` implementations, and the header is generated from the
definitions, which is why it grew: it declared 68 of the 93 before.

Nine file-local helpers: the sweep-and-prune insert the three overlap
queries each open with, the register-once-then-enter raster pair, the
two routes to an object's collision instance (the plain null chain and
the `flags & 0x600` test), the zone-number lookup, the ray normalise
that also returns the length, the six-arm cast switch that was
byte-identical in the two general ray builtins, and two random helpers.
The three file-level raster flags are named and `static`; nothing
outside the file referenced them.

### What the arguments mean

- **`COL_ListCreate`'s last argument does double duty**, which is the
  find of the file. Reading its dispatch: `-1`/`-2` is ZDE against BV,
  `-3`/`-4` BV against pivot, `-5`/`-6` ZDE against pivot, `-17`/`-18`
  ZDE against BV-centre, and **anything else is the second object's
  zone number for a zone-against-zone test**. That is why
  `COLZDEZDEListGet` is the only one of the eight pairing builtins
  whose mode comes from the script: its two chars are `zone` and
  `otherZone`, and the other seven supply the sentinel themselves.
- **The ray `kind`** splits in two: the low 31 bits select the cast (0
  is the visual cast, 1 to 5 are collision-map variants differing in
  three flags) and the top bit is a separate flag passed through to the
  collision-map cast untouched. `COLRaySingleObjectDist` keeps a switch
  of its own because only kinds 0 and 0xA do anything there, and it
  never consults the top bit.
- **`COL_RayCompute`'s four ints** were named from the callee: three go
  straight into `COL_Visual_RayCast`'s already-named `flagMustHave`,
  `flagMustNothave` and `whatToTest`, and the fourth lands in
  `COL_gst_Ray.skipRayCast`.
- **`COLComputeJumpPoint`'s `useLastRayHit`** seeds the search from the
  world's last ray-cast hit. Its other int passes into the edge search
  in `collision.c`, which does not name it, so it is `edgeSearchArg`
  with a line saying so.
- **The set-and-clear pair** `flags | (~mask & current)` recurs in
  `COLCrossableSet`, `COLSetActivationSet` and `COLMapActivationSet`:
  every bit of `flags` is set, and the bits of `mask` that `flags`
  lacks are cleared.
- **`COLLODSet` takes its maximum first**, minimum second, named from
  the fields assigned.
- **A volume that is not an axis-aligned box carries one radius in
  `BVMax.x`.** Both size getters and `BVRandomPosGet` read it that way.

### Preserved rather than fixed, each with a line in the file

- **The two box getters never return from their guards.**
  `COLBoxMaxGet_C` and `COLBoxMinGet_C` write the null vector on a
  failing flag test and again on an out-of-range zone, then fall
  through and dereference the collision instance anyway.
- ~~`COLNormalGet_C`'s second arm **returns the address of a local**~~
  — **withdrawn**: the function returns `void` in the retail binary and
  the two returns were a decompiler artefact. See Part 3 of
  [defects.md](defects.md).
- `COLGMIDGet_C` runs the game-material lookup twice, once for the test
  and once for the answer.
- `BVRandomPosGet_C`'s non-box branch **never assigns `pos->z`** and
  calls `rand()` only to discard it. The user's own Hungarian `TODO
  FIXME` at that line says exactly this and is kept byte-for-byte.
- `COLBVGetFirstOverlap_C` compared two pointers through
  `unsigned int`; it is a plain pointer loop now, with a line noting
  that the two are the same on a 32-bit target.

---

## ai_gameobjectfunctions.c

The largest table in the directory: 3,799 lines to 3,545, 225
functions, residue 0 in the source and the header. The 96 header
declarations are regenerated from the definitions, which also fixed the
stale names `ai_gameobjecttextfunctions.c`'s prototypes carried and
three that said `arg0`.

Five helpers: the visual and actor block lookups, the flash matrix, the
event scale and the largest-component pick. `AI_l_Actor` is
deliberately the same shape and name as the `static` in
`ai_animationfunctions.c`. The flash matrix's `else` arms passing `0`
are unreachable -- the flag is tested immediately above -- and are
gone.

### The flags2 half-word write, and why the rewrite is valid

Six sites render a one-bit flag change as three statements: build the
new high half into a local, truncate the field to its low half, then OR
the high half back.

```c
v2 = gao->flags2 & 0xFFFF0000 | 0x200000;
gao->flags2 = (uint16_t)gao->flags2;
gao->flags2 |= v2 & 0xFFFF0000;
```

That is `gao->flags2 |= 0x200000`, and the clearing form is
`&= ~0x200000`. **It is only equivalent because the flag has no bits in
the low half**, which the truncation would otherwise destroy -- and
that holds at every one of the six sites. It was checked exhaustively
against the original rather than argued from the shape: every one of
the 65,536 high halves crossed with eight low halves, and 14.6M cases
for the `FlagsControlSet` form with arbitrary script masks. No
mismatch.

The counterexample is in the same file and confirms the rule:
`CustomBitsSet` and `CustomBitsNot` use the mask `0xFF00`, which is in
the low half, and the original already writes those two plainly. They
are untouched.

### Preserved rather than fixed, each with a line in the file

- **`OBJ_CapaTest` pops its argument only when the object has an
  extended block.** A capability test on an object without one leaves
  the argument on the AI stack.
- **`OBJModifierType` pushes `0` and then falls through and pushes a
  second result.**
- `OBJ_CapaNot_C` toggles `capa` and returns `flags2`.
- `OBJ_HierarchySet` resets the hierarchy, then calls the `_C`, which
  resets it again.
- A dead store, `fatherPos.x = 0.0`, in `OBJ_HierarchyMoveFather_C`.
- `OBJ_RegisterKill_C` ignores the object the builtin was called on.
- **`OBJMove_C`, `OBJRotateLocal_C` and `OBJRotateGlobal_C` scale the
  caller's vector in place**, so a script variable passed to one of
  them is modified while the event runs.

### Pick -- the distance-squared sum *(binary)*

`OBJ_SqrDist_C` sums its components `z + y + x`, which is the order
`MATH_DotProduct` uses, so folding it into that call is tempting. It is
left written out: the call would compute the same sum in the same order
but through a different sequence of x87 loads, and the rounding is not
guaranteed to match. The same decision as the three float-helper
substitutions elsewhere in the directory, in the other direction,
because here the helper is not a bit-for-bit match.

---

## What is still owed here

- `AI_EvalFunc_SAVEDataGet_C`, pending the user's decision.
- The `_sub_<address>` function names across the project, as one sweep.
  Two were done where this directory defines them; about thirty remain,
  each in a file that has not had its pass. `ai.h`'s
  `get_a_string_from_globalstack_sub_4D2040` is called from three files
  here, so it belongs to that sweep rather than to any one of them.
- Two arguments named from the call side only once the engine file they
  reach has had its pass: `ai_colfunctions.c`'s `edgeSearchArg`, which
  `collision.c` does not name, and `ai_gameobjectfunctions.c`'s seven
  eye-follow flags, which `gameobject.c` does not. Both are left as the
  original has them with a line saying why.
- Nothing here has been built under MSVC.
