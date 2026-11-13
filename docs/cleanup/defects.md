# Defects found in the engine, and what was done with each

Every file in `irisEng` outside `jpeg-6b/`, `minilzo/` and `ai/ai_bge/`
has now been through the decompiler-output pass described in
[README.md](README.md). This file is the register of what that turned
up: the defects in the engine's own behaviour, separate from the
rewriting itself.

## The rule that decided each one

**If the defect is in what the game does, it stays.** A wrong
threshold, a dead branch, a leak, an off-by-one in a bound — changing
any of those changes the game, and the job was a readable
transcription, not a patched engine. Each such site keeps a one-line
comment saying what the code does, which is the only exception to the
no-comments rule, so the next reader does not take it for a
transcription mistake and quietly "fix" it.

**If the defect is the decompiler being wrong about a type, it is
corrected.** In each of the four cases below the frozen copy reads or
writes memory the object does not own, which the original compiled
code did not do: a byte offset applied to a typed pointer, a cursor of
the wrong type, a struct pointer stepped through an array of pointers.
Those are artefacts of the transcription, not behaviour, and leaving
them in would have shipped a wild write.

The split is **5 corrected, 1 withdrawn, and 141 preserved** — 117
carrying a `// preserved:` line and 24 a prose note at the site.
Part 3 records the two that moved and why.

Nothing here covers defects the rewrites themselves introduced; those
were caught by the differential tests (or, in one case, by reading)
and fixed before merge, and they are recorded per directory.

---

## Part 1 — Corrected

Each carries a comment at the site, on the precedent the user set with
`explodemodifier`.

### `modifiers/explodemodifier.c:53` — chunk array subscripted by byte

The chunk array was indexed `[chunk * 32]`, `sizeof(tGeoTriangleChunks)`
being 32, which on a **typed** pointer reaches 32 elements further
rather than one and so ran about 1 KB past the array from the second
chunk onwards. The `memcpy` below it always used the element index, so
the two disagreed about which chunk they were working on. Now indexed
by element.

### `gfx/gfxtable.c:44` — vertex-colour cursor walking backwards

Each cell wrote the four indices `4n+1`, `4n`, `4n-1` and `4n-2`, so
cell zero stored two ints **in front of** the colour array and every
later cell overwrote its neighbour's colour. Each cell now writes
`4n+1` to `4n+4`, which is where its own four vertices read theirs.

The two arrays therefore cannot be compared between the copies, so
`tests/run_gfx.sh` checks the rewritten side against what the geometry
asks for, cell by cell, rather than against the frozen copy.

### `gfx/gfxflare.c` — sub-flare chain stepping by structs, not fields

The frozen copy advances a cursor of the whole effect's type, so the
nine sub-flares' fields step by `sizeof(tGfxFlareData)` and eight of
the nine come out of memory the effect does not own — the test saw
materials of `0xBF800000` and `0x56EE3E3B` where the rewrite reads
4, 5, 6. The rewrite reads four parallel runs of nine consecutive
fields.

### `gfx/gfxsun.c:116` — struct pointer walked through an array of pointers

`D3D_SetSunEffectOnOff` stepped a struct pointer through
`gSunEffectOnOffArray`. That and `gD3dSunSurfaces` are parallel
sixteen-entry tables, and both are now walked by index.

### `proceduraltextures.c:82` — the water kernel's "row above"

Read against the gold binary and corrected; Part 3 has the
disassembly and the test that replaced the side-by-side comparison.

### One prototype corrected, for completeness

`textures.h`'s loader prototype gave its third argument as a pointer
where the definition in `proceduraltextures.c` takes a byte count.
A declaration fix, not a behavioural one.

---

## Part 2 — Preserved, with a line at the site

### The ten worth looking at first

These are the ones most likely to be causing something you can see in
the running game, rather than dead code:

| site | what it does |
|---|---|
| `collision.c:3706` | ~~`COL_b_TestFaceTag` truncates its result~~ — **withdrawn, see Part 3**: the premise was a `BOOL` typedef that is commented out |
| `grid.c:1129` | `GRID_ul_CallbackCompressedGrid` dispatches the run tag with `(tag & 0xC0) == 0xFFFFFFC0`, which can never hold because `tag` is a `char`; a short-run tag's value byte is never read and the stream desynchronises from that record on. **The identical artefact two functions away already carries your own fix**, noted `// for routing fix, it was char`. The one-line equivalent here is `((uint8_t)tag & 0xC0) == 0xC0` |
| `proceduraltextures.c:82` | ~~samples the centre cell where the row above belongs~~ — **corrected, see Part 3**: gold reads the row above |
| `proceduraltextures.c:1072` | a byte row stride and a byte indent applied to a `uint32_t` pointer, so every write lands four times too far in |
| `textures.c:1002` | the BMP header pointer is stepped by whole headers, so the pixels are read `sizeof²` bytes in |
| `textures.c:1754` | the first store lands one byte past the destination, so the whole walk is shifted and the first byte pair is never written |
| `world.c:445` | `child` cannot be NULL there, so the second arm always returns and the first leaves `result` NULL — every path out of the function yields NULL |
| `world.c:407` | one slot for a list `WOR_i_GetAllChildrenOf` writes without a bound, so a parent with more than one child writes past it |
| `win/wingdi.c:3993` | `elem` still points at the `0xC0DE` marker, so a read lands at `second[49374]` |
| `gfx/gfxprofiler.c:398` | `GFX_SetNumberOfP` computes `255 / num`, so a slice count of zero takes the process down |

### Every site, by file

117 sites use the `// preserved:` convention:

**`actions.c`** (1)

- `:232` — animPtr[0] is dereferenced here and null-tested on the next statement

**`bigfiles.c`** (4)

- `:346` — this lands before the address of the pointer variable, where the twin writes before the buffer it points at
- `:366` — the tail of this split copy goes one whole run past the dwords it belongs behind
- `:381` — the block is counted twice in this test, where the twin in BIG_SpeedMode_fread tests the sum before the increment
- `:745` — the read lands on the caller's pointer variable rather than on what it points at

**`collision.c`** (15)

- `:2643` — this scales by the hit distance of the previous triangle, which is zero until one is found, and nothing reads the result
- `:3353` — the other fifteen angles here are multiples of pi/4 and this one is not
- `:4740` — the contact normal's z is always zero -- the subtraction here is hitPosTalan->z minus itself
- `:4764` — as above -- the radial offset's z is center->z minus itself
- `:7348` — both calls ask for the rebound angle, so the slide normal is computed with it too
- `:7574` — re-tests the outer volume the enclosing branch already proved is a box, so the sphere-in-box arm below is unreachable
- `:7613` — inside the sphere branch this can never hold, so the box-in-sphere arm and COL_f_SqrLen's only call site are unreachable
- `:8837` — the rank is read without the (uint16_t) narrowing its three siblings use, so a tagged filter indexes far in front of the record array
- `:9179` — with no matching record the cursor is one past the end and this reads past it
- `:9327` — unreachable -- the walk counter only reaches 15 on a step that leaves no neighbour, and that path has already returned above
- `:9400` — the rank is written back without the null test the initialisation uses
- `:9652` — the rank is written back without the null test the initialisation uses
- `:9685` — the filter is a uint16_t, so this is always taken and the rank arm below is dead
- `:11000` — the bound admits a slot one past the end of selectedInstElemPtrs
- `:11223` — the unset-zone arm zeroes the result and then falls through into the lookup

**`gameobject.c`** (7)

- `:795` — the three guards above can leave walk null and the test below dereferences it
- `:1735` — these two have no "= 0", so unlike OBJ_GameObject_Create they are left uninitialised
- `:1872` — indexed as an array of pointers, where every other site indexes the array itself
- `:1885` — indexed as an array of pointers, where every other site indexes the array itself
- `:2291` — the second copy repeats the first 0x50 bytes the first one has already copied
- `:3269` — the getter is null-safe and all three of its results here are dereferenced unchecked
- `:3856` — OBJ_CullingAABBox writes this only when the box is inside, and it is stored either way

**`gfx.c`** (2)

- `:348` — the capacity tested is the triangle table's, the one updated is the sprite table's
- `:452` — the walk never stops early, so the value returned is always NULL

**`inputoutput.c`** (4)

- `:213` — clamped above but not below, so a negative intensity goes through
- `:440` — the saved pointer is put back and then dropped by the alloc below
- `:876` — elems[photo + 1] reads past the array for photo 99, where it lands on photoBufSize
- `:957` — first and count are bounded apart, so first + count may run past the 100 entries

**`ok3.c`** (2)

- `:459` — the bound admits boxIdx == num, one past the end of the box array
- `:636` — the first copy takes one box too many, and clearing the last slot is what makes the duplicate it leaves there harmless

**`particlegenerator.c`** (4)

- `:281` — the third axis takes cos(elevation) where subMode 2's double cone, the same shape, takes sin
- `:325` — the pitch range runs from -ffb7 to ffb8, where every other cone range here is symmetric
- `:341` — vertexIdx is only ever set by mode 6, and gro is only ever non-NULL there too
- `:544` — the sine index reaches 1024, one past the end of MATH_gf_TableSin[1024]

**`photo.c`** (5)

- `:59` — an unknown type leaves msg unwritten and logs it anyway
- `:105` — the two thresholds print without the x100 the values carry
- `:125` — the screen test prints the frame thresholds rather than its own
- `:413` — the screen test reports the frame thresholds rather than its own
- `:522` — *code is still 0 here on every path, so the ratio test can never be reached

**`proceduraltextures.c`** (13)

- `:124` — the last pixel of the buffer reads damped[1], one entry past the end
- `:188` — the lower pair of the 2x2 splash is indexed at twice its element offset
- `:611` — decimal 100, where the note beside it is the byte-1 bit, 0x100
- `:784` — the fourth quarter is written at 172, not 192, so arr[236..255] is never set and the struct is not zeroed
- `:816` — unknum is never written anywhere in the tree and the struct is not zeroed, so this branches on uninitialised memory
- `:821` — the walk starts one entry past the end of the buffer
- `:903` — both passes generate into ptr1, so the first is discarded and the two fields end up identical
- `:943` — the fourth quarter is written at 172, not 192, so values[236..255] is never set
- `:975` — a logical and where the two terms beside it are channel masks
- `:1072` — a byte row stride and a byte indent applied to a uint32_t pointer, so every write lands four times too far in
- `:1085` — span[2] is written when a vertex on the scanline is followed by a third crossing
- `:1384` — the walk starts in edge0 and advances one Vec3 per corner, so the third and fourth projections land past edge1, on viewMtx -- the matrix container->cam is pointing at while they are taken
- `:1396` — no test in the file matches 0x10000000, so the reveal this field drives never starts

**`profiler.c`** (1)

- `:101` — a byte count stepping a typed pointer, so one raster is cleared and the loop ends

**`snp.c`** (5)

- `:105` — equal indices give -1 here, which indexes the pointer in front of flagsArray
- `:350` — gao1 missing its collision structures abandons gao2's list too
- `:385` — gao1 missing its collision structures abandons gao2's list too
- `:419` — overlaps[0] is dereferenced here and only null-tested below
- `:479` — read even when the queue is empty, where it takes a stale entry

**`sound.c`** (6)

- `:999` — the scan steps by a hardcoded 56 bytes, which is sizeof(tSndSoundElem) only in the engine's own 32-bit build
- `:1607` — two of SND_i_LoadStreamedFile's failure paths return without touching wavHdr, so this reads it uninitialised
- `:2163` — the index is cleared first, so this always deletes nothing
- `:4754` — SND_i_StreamAlloc assigns SND_gast_StreamList, and which of the base and the index is evaluated first is unspecified
- `:5594` — the realloc above sizes the array for innerNum+1 pointers, so this stores one past its end and leaves the slot i4[] was filled for uninitialised
- `:6099` — both arms assign the same value, so the whole test is dead

**`soundmodifier.c`** (3)

- `:626` — the subtraction already yields an element index, so dividing by the size again collapses every instance onto 0 or 1
- `:655` — the subtraction is unsigned, so the lower clamp below it can never fire and a late step indexes before the table
- `:1132` — all three near/far channels blend i1 over nearArr[0]/farArr[0]; i2, i3 and index 1 and 2 are never read

**`strings.c`** (6)

- `:459` — the bound test truncates to a byte but the index does not
- `:1127` — (n - n % 10) % 10 is always 0, so only the lowest digit is packed
- `:1160` — a global's characters index the table as signed chars
- `:1252` — the clamped length goes into the edited string's number2, not next's
- `:2488` — the string is not checked for NULL before it is used
- `:2737` — the scratch string takes the camera width as its height and the height as its width

**`tables.c`** (1)

- `:354` — the test is on the element and not on the object it holds, so the hole arm is unreachable

**`textures.c`** (9)

- `:595` — the walk stops above src[0], so the first pixel is not expanded
- `:850` — a run of 32 bit pixels is written to the same address every time
- `:914` — the 16 bit case takes the 24 bit masks, not 5-5-5 as the bmp loader does
- `:1002` — hdr is stepped by whole headers, so the pixels are read sizeof^2 bytes in
- `:1119` — a source channel narrower than the destination one leaves both outputs unwritten
- `:1729` — the second nibble is shifted signed, so a high bit sets the whole top nibble
- `:1754` — the first store lands one byte past the destination, so the whole walk is shifted by one and the first byte pair is never written
- `:2048` — the bits kept from the destination pixel come from the channel shifts and not from the channel masks
- `:2164` — the width is taken from the resized height and the height from the resized width

**`ways.c`** (2)

- `:58` — networks is indexed here and only null-tested after the loop
- `:214` — this returns from the whole call, so the waypoint's remaining links go unweighted

**`win/adpcm.c`** (1)

- `:45` — not -32768, but every use of it narrows to int16_t

**`win/wingdi.c`** (6)

- `:198` — this return skips the matching end block, so raster 38 stays entered
- `:955` — the source array is walked with no null test, unlike the one above
- `:3388` — align is masked to 0..15, so this test can never fail
- `:3993` — elem still points at the 0xC0DE marker, so this reads second[49374]
- `:4174` — the colour arg 2 backup is taken from arg 1
- `:4199` — this one of nine second-colour lookups scales its index by four

**`win/wingdi_aftereffects.c`** (3)

- `:219` — a count used where the other arms use an index, so this stops short of 254
- `:613` — the first pass's vertical offset comes from the unnarrowed product, the rest from the narrowed step
- `:3136` — every other getter's unknown slot gives zero; this one gives the motion blur

**`win/wininputoutput.c`** (4)

- `:352` — the caller passes a float here, so this reads its bits; one notch adds 1065353216 to a scroll that decays by 1 per 0.04s
- `:990` — reached with pcINO_mouseDevice NULL when either call above failed, and DINPUT_GetProperty does not test it
- `:1020` — pcINO_getKeyNames dereferences pcINO_keybDevice unconditionally, and this runs whether or not it was created
- `:1189` — 266, where the first call and the buffer DirectInput fills are 256

**`win/winsound.c`** (8)

- `:58` — the key is left open when the value cannot be read
- `:93` — the key is left open on every failing path
- `:133` — the handle array is grown from sndPlayList, not from sndEventHandles
- `:572` — the distances and the position are float bit patterns passed as integers
- `:591` — the guard tests the address of the field, not the interface in it
- `:1516` — this is the ADPCM block stride, which equals the struct size only in a 32-bit build
- `:1577` — both allocations leak when either call fails
- `:2729` — with no matching instance the index is one past the array

**`world.c`** (5)

- `:133` — the loop has no break, so i is deletedObjNum here and this test can never hold -- the list never shrinks
- `:407` — one slot for a list WOR_i_GetAllChildrenOf writes without a bound, so a parent with more than one child writes past it
- `:445` — child cannot be NULL here, so the second arm always returns and the first leaves result NULL -- every path out of this function yields NULL
- `:1283` — a hole in the GRO table is dereferenced here without the tag test every other walk in this file makes
- `:1534` — BVMask_ga_Masks holds 200 entries and neither this function nor WOR_BVMaskPreComputeAll bounds BVMask_gi_Num against that

The directories done earlier use a prose note at the site rather than
the `// preserved:` prefix. Twenty-four more:

- **`ai/ai_arrfunctions.c:645`** — `ARRSortByFlags`: a null slot advances neither the cursor nor the range, so an array with a hole in it does not terminate
- **`ai/ai_colfunctions.c:347`** — `COLNormalGet`'s second arm returns the address of a local
- **`ai/ai_colfunctions.c:1887`** — the two box getters write the null vector on each failing guard and then carry on, so an object with no collision set is dereferenced anyway
- **`ai/ai_dynfunctions.c:254`** — `DYNFlagsGet` takes the address of the flags field and then tests *that* for null, so a null `dyn` computes `&((tDyn*)0)->flags` rather than stopping
- **`ai/ai_dynfunctions.c:898`** — four setters write through a null pointer, or hand one to `AI_PutVecToStack`, when the object has no solid
- **`ai/ai_dynfunctions.c:959`** — the address is taken before the test, so the test can only fail if the field sits at offset zero of a null solid
- **`ai/ai_dynfunctions.c:1103`** — as above — the address is taken before the test, so the test cannot fire
- **`ai/ai_dynfunctions.c:1206`** — `DYNSpeedVectorHorizSet` reads the dynamics before popping the vector, where every other builtin in the file pops first
- **`ai/ai_gameobjecttextfunctions.c:396`** — `OBJ_TextColorGet` passes the string on without checking that the object has one, alone among its neighbours
- **`ai/ai_keywords.c:443`** — `Not` of an int yields 0 or 255, not 0 or 1: it computes `(value != 0) - 1` truncated to a byte
- **`ai/ai_keywords.c:689`** — `SLeft`'s float path shifts the right operand by the left one, the other way round from the int path below it
- **`ai/ai_keywords.c:715`** — `SRight`, the same
- **`ai/ai_materialandcolorfunctions.c:75`** — `MAT_GetMtxLvlPtr`'s clamp has no `-1` where `MAT_GetColorPtr`'s has one, so a `multimatId` equal to the count indexes one past the end
- **`ai/ai_materialandcolorfunctions.c:537`** — `GetFlag` with an id of 1000 or more whose level is not found falls through and looks the raw id up again, walking that many links into the chain
- **`ai/ai_mathfunctions.c:539`** — `MATHFloatNullEps` compares the magnitude as a `double` where `MATHFloatNullToler` narrows it to a `float` first, so the two disagree at the boundary
- **`ai/ai_soundfunctions.c:865`** — `SNDGetStatusModifier`'s by-id arm closes the profiler timer once per sound modifier it steps over rather than once for the call
- **`grid.c:1129`** — the run-tag dispatch above, plus eight more oddities in the same file, the sharpest being a closed list bounded at 2000 where `GRID_gast_Explored` holds 1000, and both lists written before their bound is checked
- **`gfx/gfxdesintegrate.c:93`** — every chunk past the first copies from the *first* chunk's element array indexed by the chunk number, not from its own
- **`gfx/gfxflare.c:177`** — the nine-sample depth cursor starts at the head of the table and reads one float behind it, so the first sample takes its x from the float in front of `f_9Factor_296` and the last entry is never read
- **`gfx/gfxlightning.c:111`** — the pattern spread reads the samples back out of the buffer the `memset` above has just cleared, so it copies zeros; the render also takes a double time step
- **`gfx/gfxspark.c:274`** — the four-vertex paths are unreachable as the file stands
- **`gfx/gfxtable.c:132`** — `Setv`'s bound is the array length, not its last index, and `Create` writes two of three arrays at index zero ten times over
- **`gfx/gfxprofiler.c:398`** — `GFX_SetNumberOfP` computes `255 / num`, so a slice count of zero faults
- **`modifiers/xmenmodifier.c:565`** — the entry stride is half the 192 bytes an entry really occupies, so every entry past the first draws a window that straddles two of them


---

## Part 3 — Checked against the gold binary

Two of the preserved sites were put to `BGE.exe` directly. One was a
real defect and is now corrected; the other was never a defect at all
and the claim is withdrawn.

### `proceduraltextures.c:82` — corrected

`TEXPRO_l_WaterUpdate` propagates a water height field with a
four-neighbour kernel. The transcription reads

```c
above = &previous[rowBytes / -2];
```

and `rowBytes` is `unsigned`, so `rowBytes / -2` is
`rowBytes / 4294967294`, which is **zero** for any field smaller than
4 GB. The "row above" tap therefore re-read the centre cell, and the
kernel was three neighbours plus the centre feeding back into itself.

**Gold settles it.** The function is at `0x439090`, identified by its
four taps, `sar eax,1`, the `value -= value >> 8` damping and the
±`0x2000` clamps — all of which match the transcription exactly. The
row pointers are:

```
43907a:  lea  edx,[ebx+edx*1+0x2]   ; previous = base + rowBytes + 2
43909b:  mov  ecx,edx
43909d:  sub  ecx,ebx               ; above  = previous - rowBytes
4390a0:  movsx ebp,WORD PTR [edx-0x2]     ; previous[-1]
4390a4:  movsx eax,WORD PTR [ebx+edx*1]   ; previous + rowBytes, the row below
4390aa:  movsx ebp,WORD PTR [ecx]         ; the row above
4390af:  movsx ebp,WORD PTR [edx+0x2]     ; previous[+1]
```

`ebx` is the byte stride. Retail **subtracts** it, so `above` is one
whole row back. The `/ -2` is an artefact of the pointer's type being
read as unsigned, not something the game does. Now:

```c
above = previous - rowBytes / 2;
```

**How it is tested.** The two copies can no longer be compared on the
field once a ripple is in it, so the nine disrupted cases are left out
of the side-by-side comparison — the sizes with no disruption keep a
flat field, agree, and stay in. In their place
`waterSymmetryCase` checks a property the correct kernel has and the
broken one does not: a single impulse at the centre of a square field
stays mirror-symmetric about the centre row, because the row above and
the row below contribute alike. Replacing the "above" tap with the
centre cell destroys that.

```
water symmetry: rewrite 0, frozen copy 37554
```

The second number is what gives the check teeth. Two mutations
confirm it: stepping half a row instead of a whole one scores 50542,
and reinstating the original defect scores **exactly 37554**, the
frozen copy's own figure.

### `collision.c:3706` — withdrawn, there was no defect

The claim was that `COL_b_TestFaceTag` returns `BOOL`, which
`common.h` makes `int8_t`, so the bit it reads is truncated away for
every face whose index mod 32 is 8 or more, and those faces are
re-collided every frame.

**The premise is false.** `common.h`'s `typedef int8_t BOOL;` sits
inside a `/* */` block. The live typedef is `windows.h`'s
`typedef int BOOL;`, exactly as in the Win32 SDK, and a static assert
compiled in a real translation unit of `collision.c` confirms
`sizeof(BOOL) == 4`. Nothing was ever truncated.

**Gold agrees, independently.** Both `COL_b_TestFaceTag` and
`COL_SetFaceTag` are inlined into their one caller at `0x4922b1`:

```
4922b1:  mov  eax,edi          ; face
4922b3:  shr  eax,0x5
4922b6:  mov  edx,eax
4922b8:  shl  edx,0x5
4922bb:  mov  ecx,edi
4922bd:  sub  ecx,edx          ; face - 32 * (face >> 5)
4922bf:  mov  edx,0x1
4922c4:  shl  edx,cl
4922c6:  mov  ecx,DWORD PTR [esp+0x4c]
4922ca:  and  edx,DWORD PTR [ecx+eax*4]
4922cd:  test edx,edx          ; the whole word
4922cf:  jne  0x49232f
...
49232c:  or   DWORD PTR [eax+edx*4],esi   ; COL_SetFaceTag
```

`test edx,edx` is a full 32-bit test with no narrowing anywhere, and
the structure matches the source exactly: the tag array at `+0x8` of
`tCobOK3`, the skip-when-set branch of
`if (!COL_b_TestFaceTag(ok3, faceIdx))`, the collide call at
`0x492307` and the `|=` after it.

The function now returns `int` instead of `BOOL`. That is a
clarification — it says what it does without depending on which `BOOL`
is in scope — not a behaviour change.

**How the wrong claim survived.** The fixture declared the same `BOOL`
return for *both* copies, so the call truncated identically on each
side and no comparison could ever have seen a difference. It scored
clean before and after the change. Rebuilt to count rather than
compare, it now reports:

```
face tags: 192000 bits at index mod 32 >= 8, 48000 of them set;
           the frozen copy truncates 0 of those 48000 to zero
```

Zero. The "measured: 32 of 128 read back true" figure in
[root.md](root.md) was never measured; that entry is struck through
there.

The lesson is the one the rest of this work keeps relearning: **a
fixture that declares both sides the same way cannot see a difference
between them**, and a figure quoted as measured has to come from a run
that could have come out otherwise.

---

## What is deliberately not in this list

- **Transcription artefacts with no behavioural effect** — a hoisted
  temporary, a normalised branch, an invented local. Those are the
  pass's ordinary work and are not defects.
- **`AI_EvalFunc_SAVEDataGet_C`** in `ai/ai_inofunctions.c`, which
  indexes two arrays outside their bounds and is left **byte for
  byte**, locals and all, because renaming them would change what it
  does. It is a defect, but it is also the one function in the tree
  that was not rewritten at all; see [README.md](README.md) section 6.
- **Defects the rewrites introduced and the tests caught.** The
  sharpest was `D3D_l_DrawElementIndexedTriangles` losing its
  `vertexNum > 0` guard, where the loop bound underneath is about 2^30
  at zero vertices — found by reading, since the fixture never
  generates a zero-vertex mesh. Those belong to the pass, not to the
  engine, and are recorded in each directory's own file.
