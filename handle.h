#pragma once
#include "common.h"

/* A 32-bit stand-in for an address.
 *
 * Several of the engine's stores keep an address in a 32-bit slot: the
 * key/value arrays hold one in an `int`, an AI variable's reference
 * occupies exactly four bytes of a variable block, and the functions
 * that read them back are all declared to return an integer. Where the
 * build's pointers are wider than that slot, an address cannot go in
 * it, so a handle goes in instead and the address is looked up.
 *
 * Where pointers already fit, there is nothing to translate and these
 * are the casts the engine writes today, so that build is unaffected.
 *
 * The handle's shape is forced by what the surrounding code already
 * does with these slots, and all of it has to keep working:
 *
 *   - 0 means "no object", and several readers test for it.
 *   - 0xFFFFFFFF means "not found", which is what a failed search
 *     returns.
 *   - 1 and 2 are the two main actors, substituted for by name
 *     wherever such a slot is read.
 *   - the top bit must stay clear, because the key/value search
 *     compares keys as signed integers and a key above the signed
 *     range sorts below every other one.
 *   - the low two bits are a tag: a slot whose low bits are 01 holds
 *     an unresolved key rather than a resolved address, and about
 *     forty places test for exactly that.
 *
 * So a handle is a multiple of four in the signed range, which leaves
 * the low-bit tag reading "resolved" as it should and keeps all four
 * reserved values available. Of the 29 bits that remain, 21 are a slot
 * index and 8 a generation count; the generation is what makes a
 * handle to a released object resolve to nothing rather than to
 * whatever later took its place.
 *
 * Every entry point is transparent for the reserved values and for a
 * tagged slot: what goes in comes back out unchanged, so a caller that
 * tests a slot before using it does not have to know whether the
 * translation is active.
 *
 * Not reentrant, and deliberately unlocked: these stores are reached
 * from the engine thread only -- the sound thread's own paths touch
 * none of them, and the rest are serialised against it by the command
 * handshake -- and the key/value search they sit behind keeps its
 * state in a file-scope variable, so it was never reentrant either.
 */

/* Settled by the build's own pointer width, so nothing has to be
   defined on a command line. It can be forced to 0, which is how the
   32-bit path is exercised on a host that has no 32-bit libc to link
   against; forcing it to 1 on a 32-bit build would not work and is
   not meant to. */
#ifndef HDL_TRANSLATES
#if UINTPTR_MAX > UINT32_MAX
#define HDL_TRANSLATES 1
#else
#define HDL_TRANSLATES 0
#endif
#endif

#if HDL_TRANSLATES

/* The handle for an address, the same one every time for the same
   address. Returns 0 if the table is full, which can only mean
   handles are being taken and never released. */
uint32_t HDL_ul_Handle(void* ptr);

/* The address a handle stands for: NULL if it was released, and the
   value itself if it is a reserved one or a tagged slot. */
void* HDL_p_Pointer(uint32_t handle);

/* Give up this address's handle. Resolving it afterwards gives NULL
   until the slot is reused, and the generation means the old handle
   stays NULL after that too. */
void HDL_Release(void* ptr);

/* How many addresses currently have a handle. */
int HDL_i_LiveCount(void);

void HDL_CloseModule(void);

#else

#define HDL_ul_Handle(ptr)   ((uint32_t)(uintptr_t)(ptr))
#define HDL_p_Pointer(h)     ((void*)(uintptr_t)(uint32_t)(h))
#define HDL_Release(ptr)     ((void)(ptr))
#define HDL_i_LiveCount()    (0)
#define HDL_CloseModule()    ((void)0)

#endif

/* The values a 32-bit slot may hold that are not addresses. Each one
   is read somewhere as itself, so each one has to come back out of
   this layer unchanged. */
#define HDL_NO_OBJECT         0u
#define HDL_MAIN_ACTOR_FIRST  1u
#define HDL_MAIN_ACTOR_LAST   2u
#define HDL_NOT_FOUND         UINT32_MAX

/* How wide the slot is. Not the build's word size -- the slot is
   32 bits whatever the build, which is the whole reason this file
   exists. */
#define HDL_WORD_BITS 32

/* The low bits of a slot say whether what is in it has been resolved
   to an address yet. A handle has to read as resolved, which is why it
   is a multiple of four. */
#define HDL_TAG_BITS           2
#define HDL_TAG_MASK           ((1u << HDL_TAG_BITS) - 1u)
#define HDL_TAG_RESOLVED       0u
#define HDL_TAG_UNRESOLVED_KEY 1u
/* A slot that does not hold a reference at all: a small index into an
   array somewhere, which is already narrow enough and wants no handle.
   It is tagged rather than left bare so that a slot holding one can be
   told apart from a slot holding a reference, by whoever reads it
   without knowing which it was given. */
#define HDL_TAG_INDEX          2u
IRIS_STATIC_ASSERT(HDL_TAG_INDEX <= HDL_TAG_MASK,
               "every tag has to fit in the tag bits");

/* The top bit, which has to stay clear because the key/value search
   compares keys as signed integers. */
#define HDL_SIGN_BITS 1
#define HDL_SIGN_MASK (1u << (HDL_WORD_BITS - HDL_SIGN_BITS))

/* So the rest of the word is a generation and a slot index, and only
   the generation's width is a free choice: eight bits means a slot has
   to be reused 256 times before a handle issued for one of its earlier
   occupants can match again.

   Everything else below follows, and is written as a derivation rather
   than a number so that changing one of the three widths above cannot
   leave a shift behind. The layout is exposed because the tests take a
   handle apart. */
#define HDL_GEN_BITS  8
#define HDL_GEN_SHIFT HDL_TAG_BITS
#define HDL_IDX_SHIFT (HDL_GEN_SHIFT + HDL_GEN_BITS)
#define HDL_IDX_BITS  (HDL_WORD_BITS - HDL_SIGN_BITS - HDL_IDX_SHIFT)

#define HDL_GEN_MASK  ((1u << HDL_GEN_BITS) - 1u)
#define HDL_IDX_MASK  ((1u << HDL_IDX_BITS) - 1u)
#define HDL_MAX_SLOTS (1u << HDL_IDX_BITS)

/* Index 0 is never handed out, so that no handle can be
   HDL_NO_OBJECT. */
#define HDL_FIRST_INDEX 1u

/* The three widths have to tile the word exactly, and the smallest
   handle there is -- the first index with generation zero -- has to
   clear the reserved values. Neither holds for every choice of widths,
   so both are asserted rather than reasoned about again later. */
IRIS_STATIC_ASSERT(HDL_TAG_BITS + HDL_GEN_BITS + HDL_IDX_BITS + HDL_SIGN_BITS
                   == HDL_WORD_BITS,
               "a handle's tag, generation, index and sign bit must fill "
               "the word exactly, with nothing spare and nothing overlapping");
IRIS_STATIC_ASSERT((HDL_FIRST_INDEX << HDL_IDX_SHIFT) > HDL_MAIN_ACTOR_LAST,
               "the smallest handle must be larger than every value a slot "
               "may hold that is not an address");
