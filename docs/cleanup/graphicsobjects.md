# graphicsobjects/

Ten files, **7,011 lines to 6,645**. Nine of the ten are finished: no
`goto`, no decompiler-named local or parameter, and they parse as
standard C under the stub include set with
`-Werror=implicit-function-declaration`.

The tenth, `geostr.c`, is finished apart from one function —
`STR_l_Effect_Treat`, 2,227 lines with 43 of the directory's 64
`goto`s. Everything else in that file is done. That function is a
pass of its own and is the only work left in the directory.

| file | lines | gotos | state |
| --- | --- | --- | --- |
| `way.c` | 10 | | done |
| `camera.c` | 50 | | done, was already source |
| `light.c` | 79 | | done |
| `staticlod.c` | 128 to 119 | | done |
| `gro_common.c` | 294 to 287 | | done |
| `geomaterial.c` | 476 to 483 | 5 to 0 | done |
| `particle.c` | 594 to 497 | 12 to 0 | done |
| `shadow.c` | 882 to 878 | | done |
| `geo.c` | 1,289 | | done, was already source |
| `geostr.c` | 3,209 to 2,953 | 47 to 43 | all but one function |

`geomaterial.c` is seven lines longer than it was. Five `goto`s became
explicit switch bodies and a few one-line statements became two; the
point of the pass is what the file says, not how short it is.

## Verification

There are no differential tests in this directory, and that is a
decision rather than an omission. Every file here is an implementation
of the graphics-object interface: a loader that walks a byte stream, a
free function, a render entry point that hands work to the display
device. Standing one up means standing up the loader, the texture
manager, the display device and the geometry buffer, and a test would
then be measuring the mocks.

What the risky changes got instead was **mechanical verification of
the thing that could break**:

- **The particle loader's read order.** A switch over twelve file
  versions, each reading a different field set from the stream. The
  reader and the target of every read, per version, were extracted
  from both copies and compared, with the old copy's jumps resolved.
  All twelve agree; the only differences are the paired reads landing
  in a named local rather than a decompiler temporary.
- **The nine-patch's store table.** `STR_AddFrame` was 274 lines of
  scrambled stores. Every store was resolved through its temporaries
  into a vertex-index-to-expression map, and the rewritten loop's
  output was checked against that map: all 36 vertices and all 36 UVs
  agree, including the z and the colour run.
- **The one-quad sprite**, the same way: four vertices, four UVs, all
  agreeing.

`shadow.c` is the one file here that does not parse against the
committed stub, and that is also pre-existing: it calls two
`IDirect3DDevice9` vtable methods the stub does not declare. It was
verified against a scratch copy of the stub with those two methods
and their four render-state constants added. The addition is not in
this branch because the gfx pass already touches `tests/stub/d3d9.h`;
it belongs in whichever of the two lands second.

---

## way.c, camera.c, light.c

Close to source already. `way.c` and `light.c` each had one `a1`
parameter; `camera.c` had none. `light.c`'s colour-table build and
`camera.c`'s loader read as what they are.

## staticlod.c

The level-of-detail switch: up to six geometries and five distance
thresholds.

Two things changed. `GEO_pst_StaticLOD_GetLOD` and
`GEO_l_StaticLOD_GetIndex` walked the same search loop, so the first
calls the second. And `GEO_l_StaticLOD_HasSomethingToRender` assigned
inside its own condition:

```c
if ((geo = lod->geo[GEO_l_StaticLOD_GetIndex(lod, lod->currDistance)]) == NULL) ret = FALSE;
*gro = (tGroGeneric*)geo;
if (geo != NULL)
    ret = (... & 8) ? TRUE : FALSE;
return ret;
```

which reads as though `ret` could be returned unset. It cannot — the
two arms cover both cases — but it takes a moment to see, so it is
two returns now.

## gro_common.c

The interface table, the object loader callback and the render entry
point. Already written out.

The look-at matrix was the one place worth touching: one `Vec3` did
three jobs in a row — the negated camera up, the axis swap, and the
scale — and the two component-wise negations are the helper call. The
axis swap gets a line saying what it is, because `v2 = v3` followed by
`v3 = -oldv2` reads as a mistake otherwise.

## geomaterial.c

Single, multi-material and multi-texture materials, their loaders and
their validity flags. All five `goto`s gone.

**The mapping-mode switch** was five arms jumping to a shared store of
a value each arm had computed differently. It is five ORs into the
validity flag. The mode itself was spelled `(uint8_t)(((flags) &
0xffff) >> 8) >> 4`, which is `(flags >> 12) & 0xF`; the user's note
about the byte shuffle is kept, since it records what the compiled
code did.

**The blend-mode switch** was two arms sharing a tail through a
boolean. Written out, the two arms say different things: one checks
the *next* element's mode, the other checks its own flag.

**The multitexture loader** keeps its chain walk, with the link cursor
named and the user's note about the next pointer doubling as the
end marker kept where it was.

**`MAT_Validate_Multi`'s last line** was

```c
a1->solidVertexColor ^= ((uint8_t)v2 ^ (uint8_t)a1->solidVertexColor) & 0x40;
```

with two `TODO FIXME ??????? ez mi` against it. It sets bit 0x40 of the
result to bit 0x40 of the running AND of the children's masks, where
every other bit is the union. It is an if/else now and the question is
answered.

Two file-local helpers named after their addresses —
`MAT_allocGRO_Single_sub_4704E0` and `MAT_allocGRO_sub_470350` — are
`static` and named after what they allocate. Neither is referenced
anywhere else in the tree.

### Pick — the material-table tag test *(compiler artefact)*

`MAT_pc_GetAllUsedTextureIndex` skipped an entry whose low two bits
were `01`, written `((uint8_t)v6 & 3) != 1` — a pointer cast to a byte.
It is `((uintptr_t)mat & 3) == 1` now, which is the same two bits in
standard C, with a line saying what the tag means.

## particle.c

The particle generator: a twelve-version loader, the generator table
and the render entry point. 594 lines to 497, twelve `goto`s to none.

**The loader** is the whole story. Eleven of the twelve version arms
jumped past the end of the switch to the allocation that follows it;
two shared three lines; and the last two fell out of the switch into a
fifteen-read tail. The arms `break` now, the three lines are written
out in both places, and the late tail is a function the two versions
that want it call.

Three runs were factored, after checking the whole table rather than
reading it:

- `ffb5` to `ffb14` is the same ten reads in the same order in every
  version from 1 on — `PAG_l_ReadMotionFloats`;
- six discarded longs appear three times — `PAG_l_SkipSixLongs`;
- the fifteen-read tail versions 10 and 11 share —
  `PAG_l_ReadLateTail`.

Version 0 keeps its own motion floats, because it reads two of them
into *pairs* of fields.

The read order was then verified mechanically, as described above.

**The generator table** had the same search loop in `PAG_DelGenerator`
and `PAG_SetGeneratorFrequency`; both call one `PAG_l_FindGenerator`.
The particle-generator lookup is the visual object first and then the
modifier chain, rather than a condition with a jump out of its `else`.

## shadow.c

Shadow atlas management and the shadow render. Thirteen lines of
decompiler naming and one compiler artefact.

The artefact is worth recording. The atlas column was converted to a
double like this:

```c
v24 = SDW_RDR_AND & sdwNum;
v5 = (double)(int)v24;
if ((SDW_RDR_AND & sdwNum & 0x80000000) != 0)
    v5 = v5 + 4294967300.0;
```

That is how an unsigned-to-double conversion was emitted on a target
with only a signed one: convert through the signed path and add 2^32
when the sign bit is set. It is `(double)(unsigned int)column`, and
the column and the row have names rather than sharing one temporary.

## geo.c

The geometry object: MRM level selection, skinning, normals, the
loader and the render. **Already source** — no decompiler names, no
`goto`s, nothing to do. Three `__int16` spellings in a commented-out
struct in `geo.h` are stdint now, for consistency.

## geostr.c

The string object: fonts, letters, frames, effects and the render.
3,209 lines to 2,953, and 47 `goto`s to 43 — all 43 of them in one
function.

### STR_AddFrame, the nine-patch

274 lines of scrambled vertex and UV stores. It is three columns by
three rows of quads over four x edges and four y edges, in column
order, each quad's four vertices going top-left, bottom-left,
bottom-right, top-right.

The texture edges are the find: the u edges are
`glyph->x`, the midpoint, the midpoint, `glyph->y`, and the v edges
`glyph->h`, the midpoint, the midpoint, `glyph->w`. The middle column
and the middle row therefore sample **one line of texels each**, at
the midpoint between the glyph's two edges, which is exactly what lets
a nine-patch stretch its middle. Two nested loops over four-element
edge tables, and the file says so in three lines.

### STR_AddSprite

The same thing with one quad and the glyph's own four edges.

### STR_Render

354 lines, 65 declarations and four `goto`s, which were the loop
structure of its two passes. It reads as the two passes it is: one
counting the quads each sub-material needs, and one placing a frame
and then the letters of each string.

The jump out of the frame branch carries meaning: a string whose
frame has no font of its own draws **nothing**, its letters included.
That is a `continue` with a line saying so, because it would read as
an oversight.

#### Pick — the two pivot spellings *(binary)*

The first sprite of a letter takes its left edge from
`letter->x + STR_w_XPivot - dx` and the repeats from
`letter->x + (uint16_t)(STR_w_XPivot - dx)`. The two differ before the
parameter truncates them to sixteen bits and not after, so they are
the same value; both are written as the original has them rather than
unified, because unifying them would be a guess about which the author
wrote.

#### Pick — the per-material clear *(binary)*

`memset(&v71, 0, 96)` cleared 96 of the block's 1,024 bytes, with a
`FIXME` asking why. 96 bytes is 24 ints, which is exactly the length
of `ul_SubMatOrder` and so of the triangle counters anything reads. It
is `memset(perMat.triangleNum, 0, 24 * sizeof(int))` now — the same
bytes, saying which ones.

Note that a material with more than 24 sub-materials can have counts
written above index 23, which are never cleared and never read.
Preserved; it is a property of the sub-material order, not of this
pass.

#### STR_Init

The 23-of-24 clear of the global string table is preserved, with the
user's `ezt átnézni` kept and a line saying the count is deliberate as
written.

### What is left: STR_l_Effect_Treat

2,227 lines, 43 `goto`s, and a `LOBYTE` or two. It is the text-effect
dispatcher — a switch over effect kinds, each with its own walk over a
string's letters. It wants a pass of its own, with the same treatment
the particle loader got: extract the per-kind behaviour mechanically
first, then rewrite against that table rather than against a reading
of the control flow.

---

## What is still owed here

- `STR_l_Effect_Treat`, above.
- The two `d3d9.h` stub methods `shadow.c` needs, once the gfx pass
  and this one are both in.
- Nothing here has been built under MSVC.
