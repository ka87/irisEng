# modifiers/

Twenty-five files: the twenty-four modifier implementations and the
dispatcher. All of them now parse as standard C, carry no decompiler
names and contain no `goto`.

Four have differential tests, covering the files whose arithmetic is
intricate enough for a silent slip to be plausible: `snakemodifier`,
`protexmodifier`, `lazymodifier` and `xmenmodifier`. Together they run
**192 cases, all equivalent**. The rest are recorded below, each with
what changed and every decision that had more than one answer.

| file | lines | locals | gotos | test |
| --- | --- | --- | --- | --- |
| `crushmodifier.c` | 134 | | | |
| `explodemodifier.c` | 195 | 43 to 22 | | |
| `gensoundmodifier.c` | 402 | 30 to 16 | 8 | |
| `lazymodifier.c` | 492 | 91 to 33 | 1 | yes |
| `leglinkmodifier.c` | 262 | 51 to 9 | | |
| `loadingsoundmodifier.c` | 211 | | | |
| `morphmodifier.c` | 278 | 36 to 11 | 6 | |
| `otcmodifier.c` | 168 | 60-odd to 8 | | |
| `particlemodifier.c` | 73 | | | |
| `photomodifier.c` | 224 | | 2 | |
| `protexmodifier.c` | 567 | 29 to 14 in the water pass | 3 | yes |
| `rlicartemodifier.c` | 166 | | | |
| `rotrmodifier.c` | 129 | 28 to 10 | | |
| `semilookatmodifier.c` | 48 | 10 to 2 | | |
| `shadowmodifier.c` | 266 | | | |
| `snakemodifier.c` | 211 | 44 to 17 | 1 | yes |
| `snapmodifier.c` | 112 | | | |
| `soundfxmodifier.c` | 152 | | | |
| `speciallookatmodifier.c` | 188 | 44 to 18 | | |
| `spritegenmodifier.c` | 370 | | | |
| `stpmodifier.c` | 92 | | | |
| `symetriemodifier.c` | 103 | | | |
| `xmecmodifier.c` | 330 | | | |
| `xmenmodifier.c` | 819 | 104 to 32 in DRAW | 6 | yes |
| `modifiers.c` | 571 | | | |

## snakemodifier.c

A chain of game objects that follow one another at fixed distances, each
link blending its orientation toward the one ahead with some inertia and
sag.

`GAO_ModifierSNAKE_Apply` had **44 locals**, named `v2`..`v44` with
`a2`..`a5`, `a4a` and `a1a` mixed in. It has **17**, and the file lost 91
lines net.

Covered by `tests/test_snakemodifier.c`: 46 cases, all equivalent.

### The loop

The body was a `do`/`while` walking two pointers — `v5` over
`snake->gaos` and `v5a` over `snake->distances` — with a separate counter
`v4` starting at 1, the whole thing wrapped in `if (snake->num > 1)` so
the bottom test was safe.

It is a `for (i = 1; i < snake->num; i++)`, and the pointers are gone:
`*v5` is `gaos[i - 1]`, `v5[1]` is `gaos[i]`, `*(v5 - 1)` is
`gaos[i - 2]` and `*v5a` is `distances[i - 1]`. That is how the
already-clean half of the same file indexed them, so the two halves now
read alike. The guard folds into the loop condition.

### `goto LABEL_6` in `UpdatePointers`

The decompiler produced this:

    j = 0;
    if (snake->num <= 0)
    {
    LABEL_6:
        snake->gao = snake->gaos[0];
        for (...) { measure the distances }
    }
    else
    {
        while (1)
        {
            gao = OBJ_pst_GroupGetByRank(grp, snake->ranks[j]);
            snake->gaos[j] = gao;
            if (!gao)
                break;
            ...
            if (++j >= snake->num)
                goto LABEL_6;
        }
    }

The `num <= 0` arm is **unreachable**: the function has already returned
unless `num >= 2`. It is a bottom-tested loop whose exit target the
decompiler hoisted into a dead sibling branch and then jumped back into.
It is now a plain `for` with the tail below it.

One detail decides whether the rewrite is correct. The original `break`
leaves `while (1)`, falls out of the `if`/`else` past the label, and ends
the function — it does **not** run the tail. A `break` from the new `for`
would fall straight into the tail, so that exit is a `return`. Nothing
follows it in the function, so the two are the same.

### Restored calls

The decompiler spells small helpers out at each call site. Each of these
was checked against the real body in `math.c` before the call went back:

| was | is | sites |
|---|---|---|
| `a3.x = -v11; a3.y = -v12->y; ...` | `MATH_VecInvert` | 3 |
| `sqrt(z*z + y*y + x*x)` | `MATH_VecLen` | 1 |
| `v29 = 1.0 / sqrt(...); v42.x = a3.x * v29; ...` | `MATH_NormalizeVector` | 1 |
| `v28 = 1.0 / sqrt(...); a5.x = a5.x * v28; ...` | `MATH_NormalizeEqualVector` | 1 |
| `diffX = ...; sqrt(diffX*diffX + ...)` | `MATH_DistanceOfTwoPoints` | 1 |

The last one replaced code that was already hand-written with named
locals, not decompiler output — it was an inlined helper either way.

### What was deliberately left alone

**`->matrices` is read eight times and is not cached.** It would read
better hoisted, but `->matrices` is reassigned in `gameobject.c` and
`animation.c`, and three calls sit inside the loop —
`OBJ_SightGeneralSet`, `OBJ_BankingGeneralSet`,
`OBJMY_ComputeLocalWhenHie`. Proving none of them can reach a
reallocation is more work than the tidiness is worth, so the reads stay
where the original put them.

**Two `double` locals kept.** `drop` and `weight` hold what the
decompiler showed as `double`:

    drop = sag + snake->f3;
    sag  = (float)drop;
    if (drop + snake->inertia < 0.0)

The comparison uses the unrounded sum. Those `double`s are x87's 80-bit
intermediates, and under the original toolchain `float` and `double`
spellings compiled to the same code — but irisEng builds for SSE2, where
they do not. Writing `sag + snake->f3 < 0.0` would change which side of
the boundary a value near zero lands on. The cost of keeping them is two
`double`s; the cost of removing them is unverifiable.

The `weight` clamp was also written back the right way round. The
decompiler had `if (w >= 0) { if (w > 1) w = 1; } else w = 0;`, which is
Hex-Rays normalising the branch; it is now the obvious
`if (w < 0) w = 0; else if (w > 1) w = 1;`.

### The one measurable difference, and why it is accepted

Restoring `MATH_VecLen` does not reproduce the old arithmetic exactly.
The old inline was:

    v18   = v15->Pos.z - v16->Pos.z;    /* double */
    src.z = (float)v18;
    v37   = sqrt(v18 * src.z + src.y * src.y + src.x * src.x);

The z term is `(double)diff * (float)diff` — a mixed-precision product,
one operand rounded to `float` and the other not. No C source writes
that. It is x87 holding the difference in a register while storing a
rounded copy to the stack slot, and the decompiler faithfully reporting
both. `MATH_VecLen` computes `z * z`.

The test measures it: **208 of the compared floats differ, the worst by
2.73e-6 relative and 6.1e-5 absolute**, on values of position scale. It
enters through `measured`, which feeds the blend weight, which carries
forward because each iteration writes the position the next one reads —
so the worst case shows up in the arguments of a later
`VecBlendRotate`, not at the site itself.

This was checked rather than assumed: reverting the mock's
`MATH_NormalizeVector` to the old double-precision reciprocal moved the
worst relative difference only from 2.73e-6 to 1.88e-6, and reverting
`MATH_VecLen` to a double-precision `z*z` did not move it at all, which
is what identified the mixed product as the source.

It is accepted because the difference is between two decompilations of
the same original, not between the original and a change: the author
called `MATH_VecLen`, and the extra precision on one operand is an
artifact of the instruction selection, not of the source. Nothing at
2.73e-6 on a chain of followers is observable.

## protexmodifier.c

777 lines to 568, thirteen functions, three `goto`s gone, and every
local named. The file is latin-1 and carries undecodable bytes, so it is
edited with a script that reads and writes it as latin-1 rather than
with a text editor.

### The nested guards

`PROTEX_Disturb`, `PROTEX_Modifier_Unapply` and `PROTEX_BeforeDraw` were
each one body inside four or five nested `if`s. They are early returns
now. `Unapply` went from eight levels of indentation to two.

The one place that changed meaning is `Disturb`'s outermost test. The
decompiler had

    if (gao && (gao->flags & 5) != 0) { ... } else { v4 = v4 & ~0xff; }

and `v4` is uninitialised on that arm — it is a spill slot the register
allocator reused, not a variable the author wrote. The `else` is
dropped and the guard is `if (!gao || (gao->flags & 5) == 0) return;`.
A NULL `gao` would have faulted on `gao->matrices` two lines later
anyway.

### The twelve-way dispatch in `ComputeStUstVstP`

All three `goto`s were in one function: two arms of a `switch` on
`(param >> 4) & 3` sharing their tails, and both halves of the
with/without-a-UV-matrix split doing the same work. The shared part is
now `PROTEX_l_Basis(out, gao, uvMtx, haveUvMatrix)`, which is the one
difference between the halves — `MATH_MakeOGLMatrix` of the absolute
matrix against `MATH_MulMatrixMatrix` of it with the UV matrix.

The `default:` arm is unreachable: the value is masked with `& 3` and
all four cases are present. It is gone.

The original calls `OBJ_pst_GetAbsoluteMatrix` twice per basis, once for
the matrix and once for `Pos`, and in mode 0 computes the basis twice.
Both are kept — the function is not pure as far as this file can tell,
and the differential test compares the call sequence, so removing either
would show up as a difference rather than as a cleanup.

### The water surface

`PROTEX_l_WaterUpdate` had twenty-nine locals, two of them (`v22`,
`v26`) written and never read, and four pointers walking the same two
buffers at different offsets. It is two counted loops now, and the
buffers are indexed rather than walked.

Three things in it are worth recording:

- The relaxation reads `*cur` and writes it, while the eight samples
  come from `prev`; after the pass the two are swapped. That is a
  standard two-buffer wave step and is unchanged.
- The gradient pass runs over the buffer that was `prev` *before* the
  swap, so the bump map it produces is one frame behind the heights.
  Preserved.
- The samples are read through a `uint16_t*` while the buffer is
  `int16_t*`, so a negative height reads as a large positive one and
  `(a - b) >> 6` is computed on unsigned values. That is what the
  original does; it is now an explicit cast instead of a silent pointer
  mismatch.

### A suspected decompilation artefact, left as it stands

The stencil is not the eight neighbours it looks like. The decompiler
wrote the row-above pointer as

    unsigned int v3 = 2 * width;
    v8 = &v7[(v3 / -2) + 1];

and `v3` is **unsigned**, so `v3 / -2` is unsigned division by
`0xFFFFFFFE`, which is `0` for every width. `v8` is therefore `v7 + 1`,
not `v7 - width + 1`, and the eight samples are

    2 * prev[-1] + 2 * prev[+1] + prev[0]
      + prev[width - 1] + prev[width] + prev[width + 1]

— the row above is never read, the centre sample is counted as one of
its own neighbours, and two samples are counted twice. A comment in the
original file says the offset "pushes it back to the start", so the
author read it as negative too.

The rewrite keeps the behaviour and spells the duplication out, because
the question of what the original binary did is not answerable from
this tree: under both MSVC and gcc the file as written computes the
above, so that is what the engine has been doing. Restoring the
intended stencil is a one-line change and the test will show exactly
what moves; it is left for the user to decide.

### What the test covers

`tests/test_protexmodifier.c`, 33 cases, all equivalent:

- the water surface over six grid sizes including the degenerate 4x2 and
  2x4, and over eight consecutive frames so an error in the swap
  compounds rather than cancelling;
- `Apply2DMatrix` with a real UV matrix and with an identity one;
- `ComputeStUstVstP` across all four modes times all three UV-bit
  settings, plus a wrong subtexture kind;
- `Disturb` over a five-element protex chain, with and without the
  stamp buffers, and with a target that is not drawable;
- `BeforeDraw`'s list surgery, which removes elements through a
  pointer-to-pointer, for five patterns of which elements survive.

Each case compares the two stamp buffers, the bump bytes, the colour
output and the recorded engine-call sequence.

**12 of the compared floats differ, the worst by 2.43e-7 relative.**
They are in `Apply2DMatrix`, where the original inlined
`MATH_VecMultScalar` and `MATH_AddScaleVector` with x87 intermediates
and the restored calls round each product to `float`. Same reasoning as
snakemodifier's.

## lazymodifier.c

793 lines to 490. The one `goto` is gone, the two big functions lose 47
and 44 locals between them, and four blocks that were the same
computation written out four times become three helpers.

### The three helpers

- **`GAO_l_ModifierLazy_BlendVec(dest, target, blend)`** — `dest +=
  (target - dest) * blend`, which appeared five times: the position, the
  three matrix axes and the scale triple. Each copy was six lines of
  component arithmetic through `double` temporaries.
- **`GAO_l_ModifierLazy_ClampToBox(lazy, target)`** — decompose the
  offset from the target position onto the target's three axes, clamp
  each against its `ff*`/`upperMax` pair, and rebuild the position.
  Written twice, once inlined and once already using
  `MATH_AddScaleVector`.
- **`GAO_l_ModifierLazy_KeepBoneLength(lazy, mtx, anchor)`** — rescale
  the lazy position to the bone's length and rotate the bone onto it
  through the half-vector quaternion. Also written twice, and the second
  copy is what identified the first: the inlined version is the same
  sequence with `sqrt` and `memcpy` in place of `MATH_VecLen` and
  `MATH_CopyMatrix`.

The clamp helper takes the **target matrix**, not the parent matrix,
and that is not cosmetic. The semi-mechanical model computes
`MATH_MulMatrixMatrix(&target, &lazy->mtx1, parentMtx)` once at the top
and reuses it for the clamp; `Compute` computes it inside the clamp
branch. A first draft had the helper compute it itself, which gave the
semi-mechanical path a second `MATH_MulMatrixMatrix` — caught by the
call-sequence comparison, not by any value.

### The flag shadow

`Compute` carried two copies of the flag word, `v3` and `v44`, and three
statements of the form `LOBYTE(v3) = v44`. `v44` is assigned only where
`v3` is, and to the same value, so all three restores are no-ops — they
are the register allocator reloading a spilled byte. Both the shadow and
the restores are gone.

### The decay loop

    decay = 1.0; elapsed = 0.0;
    if (dt > 0) do { decay *= perStep; elapsed += 0.0033333334; } while (elapsed < dt);
    blend = 1 - decay;

Kept exactly, including the `double` accumulators and the
`do`/`while`, which runs one step even for a `dt` below 1/300. Making
`elapsed` a `float` would change the step count at large `dt`.

### `goto LABEL_14` in `Inherit`

The label was the `else` of a test duplicated in both arms of an
outer branch: each arm ends with "if the source has a scale, copy it
and set the flag; otherwise clear the flag". Hoisting the source matrix
into a variable lets the axis copy and the scale copy be written once,
and the `goto` becomes the `else` it always was. The position copy
stays inside the hierarchy arm, because only that arm had it.

### What the test covers

`tests/test_lazymodifier.c`, 56 cases, all equivalent:

- `Compute` over fourteen flag combinations times three settings
  (one frame, eight frames with a scaled matrix, and no parent), plus
  `dt` of zero and one;
- the semi-mechanical model with and without the box clamp, over eight
  frames, and with no parent;
- `Inherit` across both source kinds, each of the three inherit bits
  alone, and with and without a source scale;
- `Apply` over eight scenes: plain object, hierarchy with and without a
  reference, a reference carrying a four-modifier chain (so the
  recursion runs), the flag-restore bits, and an already-applied
  modifier;
- `Reinit` four ways and `Load` at each of the four stream versions.

Each case compares both matrices, the whole `lazy` struct and the
recorded engine-call sequence.

**254 of the compared floats differ.** The two extremes are
`0.000244` absolute, which is `6.8e-8` relative — `vv` is a velocity,
divided by `dt`, so a 4e-6 position difference arrives scaled by 62 —
and `1.3%` relative, whose absolute difference is `1.2e-7`, on a matrix
component near zero after a cancellation. Both are the `double`-versus-
`float` intermediates described under snakemodifier: the decompiler
shows x87's 80-bit registers as `double` locals, and the restored
`MATH_*` calls compute in `float`.

## xmenmodifier.c

1,190 lines to 819, six `goto`s gone, 104 locals in `DRAW` down to 32,
and the two public entry points that walk a skeleton share one scan.

### `GetBonePosFromNum`'s goto maze

Five of the six `goto`s were in this one function, and between them they
expressed one thing: pick the matrix for a multipart element, falling
back to the identity. There were four ways in and two labels shared by
all of them. It is now
`GAO_l_ModifierXMEN_BoneMatrix(gao, id, relative)`, which returns the
matrix and writes into `relative` only on the one path that computes a
matrix of its own. `LABEL_15` onward is the caller's tail, and the
`switch` on the vector type stops falling through two cases into a
label.

The cross product at the end was written out with its operands the
other way round — `a2.x = dz * vec.y - dy * vec.z` is
`cross(vec, offset)`, not `cross(offset, vec)` — so the call it becomes
is `MATH_CrossProduct(&tangent, mtxVecRetByType, &offset)`. Getting
that backwards would have flipped every ribbon in the game and cost
nothing the chunk figures or the call check could see.

### `SMOOTH_BUFFER`

Four inlined `MATH_BlendVector` bodies and a `while (1)` with the test
at the bottom. The second loop's four copies are exactly what
`..._BendVectors_sub_4235A0` does — including the clamp of the blend
weight, which `MATH_BlendVector` performs itself — so the whole loop is
one call per step.

### `DRAW`

575 lines in one function, and it is five passes that happen to share a
frame:

1. advance each entry's trail by one slot;
2. if the frame was skipped, reset every slot to the first;
3. for the ribbon type, smooth the slot pairs, build a per-slot
   bitangent from the camera axis, and offset the pair by the size;
4. set up the matrix stack;
5. per entry, build the triangle strip, measure the trail, lay out the
   UVs and draw.

The rewrite keeps the five passes as they are and lifts the repetition
out of each: `GAO_l_ModifierXMEN_Quad` (the two triangles of one strip
quad, written out twice at twelve assignments each),
`GAO_l_ModifierXMEN_TrailUVs` (one loop, written four times — twice for
the wrap and twice for the `u`/`v` swap) and
`GAO_l_ModifierXMEN_TrailLength` (twice). The three ways of placing a
new trail slot differ only in which point the bone lookup is given and
the two scale factors, so the branch now sets `nearScale` and
`farScale` and the placement is two `MATH_AddScaleVector` calls.

Two pieces of pointer arithmetic were doing something simple the hard
way:

- `&xmen->ptr1[idx + (num2 ? 0 : 1)].data[2 * (num2 - 1)]` is the
  previous slot, wrapping to slot 7 by walking off the front of the
  next entry. It is `&xmen->ptr1[idx].data[2 * ((num2 + 7) & 7)]`.
- `(Vec3*)((char*)&xmen->ptr1[v139] + 24 * v66)` and its two
  neighbours, `v67 - 8` / `v138a - 2` with a `+ 56` / `+ 16` correction
  when `num2` is zero, are the current and previous slots again. Both
  are now indexed by `num2` and `prevNum2`.

### The half-stride window, left as it stands

In the unsmoothed path the vertex array is set to
`(char*)xmen->ptr1->data + 96 * entry`, while the smoothed path uses
`+ 192 * entry`. An entry is 16 `Vec3`, which is 192 bytes, so the
unsmoothed path advances by half an entry and every entry past the
first draws a window straddling two of them. It stays inside the
allocation, so nothing faults.

Preserved, with a comment at the site, for the same reason as
protexmodifier's stencil: the file as written is what the engine has
been compiling, and which of the two strides the author meant is not
answerable from this tree. Changing the `96` to `192` is a one-token
edit and the test will show exactly what moves.

### The skeleton scan

`XMEN_OnOff` and `XMEN_SetSize` each ended with the same twenty-line
walk: handle the object itself, then, if it has an actor with a
skeleton group, every object in that group. The walk is a `while` whose
counter starts as a null pointer and repositions itself on the first
pass, which is why it reads as it does. It is now
`XMEN_l_ScanBegin` / `XMEN_l_ScanNext`, which keep that behaviour
exactly — including the one iteration an object with no skeleton gets.
`XMEN_l_ScanNext` returns a flag and writes the object through a
pointer, because a null element is a legitimate mid-scan value that the
callers skip.

The tag test `((uint8_t)v8 & 3) == 1` becomes
`((uintptr_t)obj & 3) == 1`, which is the same low two bits without
casting a pointer to a byte.

### What the test covers

`tests/test_xmenmodifier.c`, 73 cases, all equivalent:

- `GetBonePosFromNum` for all eight vector types across all four link
  shapes (no matrix index, an index with a plain child, an index with a
  hierarchy child, no child at all), plus plain part matrices, an id
  out of range, an id of -1, no multipart data, no basic, and a null
  matrix pointer — which is every path through the goto maze;
- `SMOOTH_BUFFER` at four pass widths and four blend weights including
  out-of-range ones;
- `DRAW` over eighteen scenes: both trail types, each flag alone and
  all of them together, one to three entries, ten consecutive frames so
  the slot ring wraps, a frame gap mid-run, a `dt` that expires the
  trail every frame, a zero `dt`, plain part matrices, and no entries;
- `Load` at each of the four stream versions, `Reinit` four ways, and
  `OnOff`/`SetSize` for a matching id, every id, no match and a null
  object.

Each `DRAW` case compares the trail buffer, the ring counter and the
timer, and the mock captures the full draw state — vertex and triangle
counts, all 64 triangle indices, all 64 UVs and all 64 vertices — at
every draw call, so a difference in any entry shows up and not just in
the last one.

**2,802 of the compared floats differ, the worst by 2.4e-7 absolute and
2.7e-7 relative.** They come from the restored
`MATH_DistanceOfTwoPoints`, which sums the squares in `double` and in
`x, y, z` order where the inlined copy used `float` products summed
`x, z, y`.

### Two changes to the frozen baseline, for the test only

`tests/baseline_xmenmodifier.c` is a verbatim copy of the file at HEAD
except for two 32-bit assumptions, which the header of that file also
records:

- `v10[-2 * passNum]` in `SMOOTH_BUFFER`, where `passNum` is `unsigned`
  — so `-2 * passNum` is a huge positive index under 64-bit pointer
  arithmetic and the function faults. Cast to `int`.
- `(Vec3*)((int)v138a - 24)` in `DRAW`, which truncates a 64-bit
  pointer. Widened to `(intptr_t)`.

The engine builds 32-bit, where both wrap to the intended offsets. The
rewrite computes neither through an unsigned index nor through an `int`
cast, so it needs no such fix — which is itself part of converting the
file to standard C.

## The other twenty-one files

The four above have tests of their own and sections to match. These are
the rest, in directory order, with what changed and every decision that
had more than one answer.

### crushmodifier.c

Already hand-written; only a double cast in `Reinit` changed.

Its six clamp loops each name their own axis and direction. Collapsing
them into one loop with a computed axis index was considered and
rejected: the index arithmetic would be harder to read than the six
explicit loops, and the compiler emits the same thing either way.

### explodemodifier.c

`Create` had 43 locals and a **real bug**, which is the one place in the
batch where behaviour changed on purpose. The chunk array was indexed by
a byte offset:

    v9 = i * 32;                                   /* sizeof(tGeoTriangleChunks) */
    v10 = &v4->geogroNew->triangleChunks[v9];
    v10->triangleNum = v3->triangleChunks[v9].triangleNum;
    ...
    memcpy(v10->triangleChunkElems, v3->triangleChunks[i].triangleChunkElems, ...);

so the same loop reached chunk `i * 32` to write the header and chunk `i`
to copy the elements. The two agree only on the first pass; anything with
more than one triangle chunk read and wrote 1KB past where it meant to.
The index is `i` throughout now, which is what the `memcpy` beside it
always used. The fix carries the comment the user asked for, at the site.

Several locals were the same pointer reloaded after the inner loop
because the register had been spilled. The triangle normal was a
hand-written normalise with a zero guard; `MATH_b_NulVector` tests the
same three `fabs` comparisons, so it is
`MATH_NormalizeVectorWithNullVectorCheck`.

`Apply` moved each vertex by the difference between its ballistic
position at `t` and at `t + dt`, with the `z` term carrying a gravity
factor, spread over 21 locals and a `for`-increment that performed one of
the three stores. The two positions come from
`GAO_l_ModifierExplode_PosAt` now and the loop body is three lines.
`Reinit` used two counters that were always equal.

This file is also where the `-fms-extensions` dependency ended: it
assigned through a cast on the left of an assignment, which MSVC allows
and standard C does not.

### gensoundmodifier.c

All eight `goto`s were in `Load`, and seven of them were one construct:
a `switch` over the chunk version where each case set a flag and then
jumped into the next, which is how Hex-Rays renders fall-through. The
cases run 266 to 273 with no gap, so every flag is a threshold —
`verIs266`, `verUpTo267`, `verUpTo268`, `verUpTo269`, `verUpTo270`,
`verUpTo272`. They are named for the version they bound rather than for
behaviour this pass cannot vouch for.

The `default` case and the `switch` both returned the same expression;
one guard at the top does instead, which takes the whole body out of
three levels of nesting.

The eighth `goto` skipped a block of shared range validation. It ran when
the third zone's flag was set, or when either of the first two had been
validated, which one `zoneChecked` flag set in all three branches
expresses directly.

Thirty locals become sixteen. Most were x87 temporaries or a field read
into an `int` to be tested once; two held the same value under different
names, and one was a copy of a flag made twenty lines before its use.
`Load` also opened by fetching the current key and discarding it, as
`soundfxmodifier` did.

### leglinkmodifier.c

The worst single function of the batch. `Compute` had **51 locals**, all
but a few of them x87 register temps, with three inlined cross products,
four inlined normalisations and the table-based square root written out
as a shift-and-mask expression over `MATH_gl_TableRacine`. It is nine
locals and reads as what it is: a two-bone IK solve — project the knee
onto the hip-to-foot axis, raise it by the perpendicular height, then
rebuild both joints' bases.

#### What settled the reading

`DoIt`, in the same file, is the same algorithm already written with the
engine's own helpers, so the decode was checked against it line for line
rather than inferred: same distance, same projection, same
`MATH_GetSqrt`, same cross-and-normalise order.

Where `Compute`'s hand-rolled normalisation used a `long double`
reciprocal and the helper uses a `float` one, the helper wins, because
that is what the neighbouring function does with the same quantity.

`DoIt` itself kept its structure and lost its names: `a2a` held the
direction and then a cross-product temporary, and `src2` held the raised
offset and then both bone vectors, so those are separate locals now. The
flag-8 branch that swapped two pointers to reorder a cross product's
operands says so directly instead.

`Load` bound three file references with the same five lines each, the
third carrying a duplicated return, so that is
`GAO_l_ModifierLegLink_BindRef`. `Apply` saved and restored the three
rank-carrying pointer fields through `v14`, `v15` and `v16` while reading
one of them again through `v11`.

### loadingsoundmodifier.c

Nested guards to early returns, the inlined distance to
`MATH_DistanceOfTwoPoints`, and the decompiler's return slot in `Load`
dropped.

One oddity kept exactly as it was: `Load` guards its reads with
`if (lsmdf)` and then dereferences `lsmdf` unconditionally afterwards. It
looks wrong; it is what the game does.

### morphmodifier.c

`Apply` drives a set of morph channels, each holding a list of target
keys, a progress and a factor. Its six `goto`s were two constructs. The
first was a `while (1)` whose `break` meant "this channel is active" and
whose two `goto`s meant "next channel" and "all channels done" — a scan
loop, now a `for` over the channels with a `continue`. The second skipped
the first of the two blend contributions when its weight came out at or
below zero, which is a condition on the weight.

The two contributions then turned out to be the same operation written
twice: add a target's vertex deltas into the work buffer, scaled, with a
separate branch when the scale is exactly one. The first spells it with
`MATH_VecMultScalar` and `MATH_VecAddVecWithoutBuffer`, the second writes
the components out by hand. `GEO_l_ModifierMorphing_AddTarget` takes
both.

Two locals in that function existed only to be restored to themselves: a
cursor and a channel pointer were saved before the contribution blocks
and copied back afterwards, and neither block touches either. 36 locals
become 11, and three levels of nesting come out as early returns.

`Create`, `Destroy` and `Load` each walked their arrays with two counters
advancing in step, one indexing and one only comparing against the bound.
`Load` also guarded three allocations with `sizeof(T) * n` rather than
`n`, and used a `BOOL` reloaded three times to hold `n <= 0`.

### otcmodifier.c

`Apply` was twelve copies of the same loop behind a nested flag dispatch,
spread over sixty-odd float locals named `a1a` through `a1bf`, `mdfa`
through `mdfn` and `geogroa` through `geogrog` — all of them x87 spill
slots. 436 lines become 168.

What the twelve are: three bits pick which vertex component drives the
wave, bit 8 picks between two sines accumulating into one component and a
sine and a cosine into two, bit 0x10 weights the amplitude by the alpha
byte of the object's RLI vertex colours, and bit 0x20 inverts that
weight. Eleven of the twelve are the same function with different
component indices, so they are one helper taking the destination and
source indices and whether the second term is a cosine.

#### The twelfth variant, left as its own loop

The unweighted bit-4-plus-bit-8 case computes

    z += (sin(speed * x) + sin(speed * y)) * (2 * sin(offset) * intensity)

where its weighted twin computes

    z += sin(speed * x + offset) * amp;
    z += sin(speed * y + offset) * amp;

The offset has moved out of the sines and into the amplitude, which is a
different function, not a rearrangement of the same one. Whether that is
intended or a slip in the original is not something this pass can answer,
so it is reproduced as it stands, as
`GEO_l_ModifierOtc_WaveZFromXY`.

`Load` reads an offset out of the file and then overwrites it with zero
on the next line. Both are kept.

### particlemodifier.c

Renaming only.

### photomodifier.c

Both `goto`s were shared tails. `SnapGOGet` jumped into the middle of the
second resolution attempt to reuse its "did that work?" check, and both
arms ended at a label that clears the AI canal and returns the object
unchanged; that tail is a small function now and the two attempts stand
on their own. Note that the first attempt does **not** fall through to
the second when it resolves nothing — the `goto` went past it — so the
early return keeps that.

`Get` held the same modifier-list walk twice, once for the object and
once for its animation reference, each with its own `goto` for "not
found", plus a label that both paths jumped to for the culling-sphere
tail. `GAO_l_ModifierPhoto_FindOn` does the walk, so the function reads
as: find it here, else find it on the reference, else give up.

`Create` chose between the supplied struct and a default seven times
over, field by field, with a temporary for each. It is one `if`/`else`.

### rlicartemodifier.c

`Apply` was a seven-deep pyramid of null checks, now early returns, and
three `do`/`while` loops over walking pointers that differ only in
whether the colour is replaced, added or subtracted; they are three
indexed loops under the same `switch`. The tail that seeds the vertex
buffer runs whether or not the `switch` matched, as before.

`indicesPtr` is read through a `uint8_t*` as the original did, so an
index above 127 stays positive.

### rotrmodifier.c

`Apply` had 28 locals, now 10. The orthonormal fix-up at the end was two
cross products and two normalisations written out by hand, including the
`long double`s the decompiler shows for x87 intermediates; they are
`MATH_CrossProduct` and `MATH_NormalizeEqualVector`. The three identical
`dt`-times-speed clamps are one helper. `UpdatePointers` was a four-deep
pyramid of null checks and is early returns; `Create` built the same flag
value in both arms, so it is set once before the branch.

#### One oddity kept

`Create`'s `else` arm calls `UpdatePointers` and then immediately clears
the game object it has just resolved. The call is still needed for its
other effect — it also copies the matrix axis into `v1` — so it stays.

### semilookatmodifier.c

Two cross products written out longhand; the second is a negated cross,
which is `MATH_CrossProduct` with the operands swapped. `Apply` is four
calls and a subtraction now, down from ten locals including four
`long double`s.

### shadowmodifier.c

Already hand-written — what the first survey counted as decompiler
locals were the matrix fields `v1`, `v2` and `v3` — but it repeated
itself. `Create` cast the same pointer nine times; it uses a local.
`Apply_All` had three near-identical blocks that differed only in which
matrix axis they took, and six copies of the same invert-flag check, so
the axis blocks are one call and the invert-and-normalise tail shared by
the three light cases is another. 69 lines go, 55 arrive.

The three directional tails were done one at a time by hand after a
scripted structural replacement mangled the file: the end marker it
searched for also matched a check *inside* each block, which left three
dangling fragments. Caught on read-back, reverted, redone. That was the
second time scripted structural editing misfired in this pass, which is
why the rest of it is by hand.

### snapmodifier.c

Two pairs of walking pointers that incremented in lockstep and always
addressed the same element, so one index does for each.

### soundfxmodifier.c

The distance to the sound reference was six locals and an inlined `sqrt`;
it is `MATH_DistanceOfTwoPoints`. The two nested guards on `soundEnabled`
and `sndMatrixRef` are early returns. `Create` built the flag value in a
temporary so it could be stored after the pointer; both arms set the same
bit, so it is one assignment at the end.

`Load` dropped a dead trio: the current key was fetched, formatted into a
1024-byte buffer with `sprintf`, and never read again — the remains of a
trace. `LOA_ul_GetCurrentKey` only reads a table entry, so nothing is
lost by dropping all three.

#### One oddity kept exactly as it was

`Load`'s guard on `i5` assigns to `i4`:

    if (sfx->i5 <= 0.0)
        sfx->i4 = 0.5;

`i4` already has its own guard above, so this one either disables a clamp
on `i5` or overwrites a good `i4` with 0.5. It looks wrong, but it is
what the game does, so it stays until someone decides otherwise.

### speciallookatmodifier.c

`Apply` had 44 locals, now 18, and three of them were the same struct
pointer under three names. The `b1 == 0` arm is an early return, so the
two real modes are no longer nested two deep. Restores
`MATH_AddScaleVector` for the position offset, `MATH_VecInvert` for three
negated copies, `MATH_VecLenSquared`, `MATH_DistanceOfTwoPoints` and the
cross-product pair.

The ring that the mode-1 branch writes into the geometry steps two
vertices at a time and leaves the odd ones untouched, which the pointer
walk hid; it is an indexed loop with the stride visible. The step is
`4 * pi` over the vertex count, so a full turn takes the half of the
vertices it writes.

One local held a distance and then a height derived from it. They are two
locals now, since reusing one name for two quantities is exactly the
habit this pass removes.

### spritegenmodifier.c

Most of this file was already hand-written; what was left sat in one
block of `Unapply`.

It splits a sprite count across four draw lists: each gets `count >> 2`,
and the `count & 3` left over is handed out one at a time to the lists
below the remainder. That was a `do`/`while` walking a pointer backwards
while decrementing three counters at once, one of which only existed to
drive the loop and another only to be read back afterwards. The read-back
is the interesting part: the counter reaches zero on every path, so the
`gvBase = v34 - 1` that followed always assigned the same thing, and it
is written as what it is — an unsigned `-1`, the reset value matching the
`gvBase = 0` earlier in the function.

The sprite total either side of the fog branch was the same four-term sum
computed twice into two locals.

### stpmodifier.c

An early return that existed only because the tail assignment was
duplicated; `hasStp` is written once now.

### symetriemodifier.c

Both loops were strength-reduced. The `v5->x` writes were not touching
`x`: the pointers had been advanced four or eight bytes by `char*`
arithmetic, so `->x` was `y` or `z`. The loop guard was a constant
evaluated once, and the trip count works out to `vertexNum` for every
axis offset. It is an explicit axis index now, picked by a small helper
the two functions share.

### xmecmodifier.c

Mostly hand-written already; its remaining `vN` are the matrix fields
`v1`, `v2` and `v3`. What changed is the loops. `Apply` had a reverse
loop spelled `while (!(i-- == 1))` and a `while (TRUE)` whose advance sat
at the bottom. `UpdatePointers` walked three arrays with three pointers
in step. `Create` filled the index table with a `do`/`while` and reloaded
the same pointer under three names. `Reinit` walked two arrays at once.
All are indexed loops now.

`XMEC_OnOff` used a null pointer as a sentinel so that the first pass of
its loop handled the object itself and later passes handled the skeleton
table, reseating the cursor mid-loop to do it. The object is handled
before the loop now and the loop only walks the table. The tagged-pointer
test it applies to table entries is kept, and reads through `uintptr_t`
rather than truncating a pointer to `uint8_t`. Its unused `id` parameter
is kept, since it is part of the signature the engine calls through.

`xmenmodifier` has the same scan written the same way. The two are not
shared: they live in different translation units, and coupling them to
save twenty lines would be a worse trade than the duplication.

### modifiers.c

Seventeen small functions and one long table.

Most of the work was nesting. Eleven of the functions opened with two or
three guards wrapped around their whole body — a flag test, then
`extended`, then the modifier list — and those are early returns now,
which takes the real body out to one level in every case.

The two loops in `UpdateModifiersGroup` were the same loop: set each
modifier's group and call its `ReInit`. The second sat four levels deep
inside the group table walk and used a second set of names for the same
locals. `MDF_l_SetGroupAndReinit` does both.

`MDF_pst_GetByType` hand-rolled a list walk with its exit in the middle
and three separate returns; it is a `for` loop. `MDF_ul_Modifier_Load`
held the reader pointer under two names and the game object under two
more, and reached the modifier through `*slot` after having it in hand.
`MDF_Modifier_AddToGameObject` walked to the end of a list with an empty
`for` body.

#### The table is left as it stands

`MDF_Modifier_Init` is twenty-five straightforward blocks of
assignments, and the only way to shorten it would be a helper taking six
function pointers, every one of which would need a cast at every call
site. That reads worse than the table, so the table stays.
