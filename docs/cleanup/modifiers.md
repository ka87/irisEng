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
