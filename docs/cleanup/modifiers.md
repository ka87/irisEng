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
