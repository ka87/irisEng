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
#if UINTPTR_MAX > 0xFFFFFFFFu
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

/* The layout, exposed so that a test can take a handle apart. */
#define HDL_GEN_SHIFT 2
#define HDL_GEN_BITS  8
#define HDL_IDX_SHIFT 10
#define HDL_IDX_BITS  21

#define HDL_GEN_MASK  ((1u << HDL_GEN_BITS) - 1u)
#define HDL_IDX_MASK  ((1u << HDL_IDX_BITS) - 1u)
#define HDL_MAX_SLOTS (1u << HDL_IDX_BITS)
