# modifiers/

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
