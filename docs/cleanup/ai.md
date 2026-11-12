# ai/

The AI interpreter and the builtins the game's scripts call. 32 files;
**20 are finished**, meaning no `goto`, no decompiler-named local or
parameter, and they parse as standard C against the stub include set
with `-Werror=implicit-function-declaration`.

One of the twelve remaining is deliberately out of scope and one is
blocked; the other ten are the next batch.

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
| `ai_messagefunctions.c` | 410 to 356 | | 1 to 0 | done |
| `ai_fields.c` | 362 | | | done |
| `ai_lightfunctions.c` | 540 to 440 | | | done |
| `ai_worldfunctions.c` | 493 | | | done |
| `ai_sectorfunctions.c` | 494 | | | done |
| `ai_miscfunctions.c` | 541 to 521 | | | done |
| `ai_aifunctions.c` | 560 to 524 | | | done |
| `ai_viewfunctions.c` | 604 | | | done |
| `ai_animationfunctions.c` | 639 | | | done |
| `ai_gfxfunctions.c` | 648 | | | done |
| `ai_wayfunctions.c` | 777 | | 2 to 0 | done |
| `ai_functions.c` | 885 | | | done, table only |
| `ai_compiledscripts.c` | 118 | 11 | | out of scope |
| `ai_gridfunctions.c` | 606 | 305 | | blocked on `grid.c` |
| `ai_inofunctions.c` | 884 | 410 | | next batch |
| `ai_arrfunctions.c` | 754 | 415 | | next batch |
| `ai_materialandcolorfunctions.c` | 1,077 | 568 | 1 | next batch |
| `ai_gameobjecttextfunctions.c` | 1,249 | 588 | | next batch |
| `ai_mathfunctions.c` | 1,371 | 623 | | next batch |
| `ai_keywords.c` | 2,038 | 766 | 10 | next batch |
| `ai_soundfunctions.c` | 1,822 | 778 | 7 | next batch |
| `ai_dynfunctions.c` | 1,681 | 926 | | next batch |
| `ai_colfunctions.c` | 2,774 | 1,279 | | next batch |
| `ai_gameobjectfunctions.c` | 3,799 | 1,668 | | next batch |

"Residue" is the count of lines carrying a decompiler name — `a1`…`a9`,
`v1`…`vN` — which is the measure used throughout this pass.

`ai_compiledscripts.c` is **the user's own hand-written hook file**, not
decompiler output: its locals and its Hungarian commentary are theirs.
It is left alone.

`ai_gridfunctions.c` is almost entirely pass-through into `grid.c` —
`GRI_Tag(grid, pos, a3, a4)`, `GRID_ShortestPath` with eleven arguments —
and `grid.c` is still decompiler output, so nothing there says what those
arguments mean. Naming them from this side would be invention, so the
file waits for `grid.c`. Everything else in it (the current/dest grid
pick, the setters, the wrappers) is ready to go the moment it does.

## Verification

There are no differential tests in this directory, and that is a
decision rather than an omission. Every file here is a table of thin
wrappers over the engine: a builtin pops its arguments off the AI stack,
calls one engine function and pushes the result. Standing one up means
standing up the interpreter, the game object tree, the world and the
engine subsystem behind whichever builtin is under test — and the test
would then be measuring the mocks, not the wrapper.

What the directory gets instead is the three checks that can see a
mistake in a wrapper:

- **The argument order is read off the pops.** The AI stack is popped in
  reverse, so a wrapper's pop sequence is the builtin's argument list
  backwards. Every rewritten wrapper was checked pop by pop against the
  original, and against the `_C` call it feeds.
- **Every file parses as standard C** with
  `-Werror=implicit-function-declaration`, which catches a renamed
  function that was not renamed at its other end. 120 of the project's
  137 first-party sources now parse, against 117 before this batch —
  `ai_gfxfunctions.c` and `graphicsobjects/shadow.c` came in with the
  d3d9 stub additions below.
- **The residue count is zero per file**, so nothing was renamed halfway.

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

---

## What is still owed here

- The ten files in the next batch, and `ai_gridfunctions.c` once
  `grid.c` is named.
- The `_sub_<address>` function names across the project, as one sweep.
- `docs/cleanup/README.md` has no entry for this file yet: the geostr
  branch is still open against it and an entry here would conflict. It
  comes with the next batch.
- Nothing here has been built under MSVC.
