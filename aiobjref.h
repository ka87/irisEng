#pragma once

#include "common.h"
#include "handle.h"

struct sGameObjectMain;

/* An element of an array of objects in the AI language.
 *
 * The language's arrays are thirty-two bits wide whatever the build,
 * so an element is a reference slot and not an address. Walking one as
 * `tGameObjectMain**` steps twice as far on a wide build and reads two
 * slots as one pointer.
 *
 * It is a struct rather than a bare uint32_t so that the compiler
 * objects to the old spelling. It lives in a header of its own because
 * both the language and the collision lists that fill these arrays
 * need it, and those two already include each other.
 *
 * Note that MSVC reports the mismatch as C4047, a warning, so a build
 * with warnings allowed will still produce an executable. gcc needs
 * -Werror=incompatible-pointer-types to refuse it. */
typedef struct sAIObjRef
{
    uint32_t slot;
} tAIObjRef;

IRIS_STATIC_ASSERT(sizeof(tAIObjRef) == 4,
                   "an AI array element is four bytes on every build");

static __inline struct sGameObjectMain* AI_p_ObjFromRef(tAIObjRef ref)
{
    return (struct sGameObjectMain*)HDL_p_Pointer(ref.slot);
}

static __inline tAIObjRef AI_ref_FromObj(struct sGameObjectMain* gao)
{
    tAIObjRef ref;

    ref.slot = HDL_ul_Handle(gao);
    return ref;
}

/* What the collision list builders write into.
 *
 * They fill one of these arrays for the AI and have no other caller,
 * so they write slots. Their frozen copies write addresses and cannot
 * be changed, and the array has no cap for a local one to stand in
 * for -- the script sizes it -- so the shape is gated rather than
 * converted at the boundary. A build with IRIS_FROZEN_SIZES writes
 * what the frozen copy writes, which is what makes the two
 * comparable; every other build writes slots. */
#ifdef IRIS_FROZEN_SIZES
typedef struct sGameObjectMain* tAIObjOut;
#define AI_OBJ_OUT(gao) (gao)
#else
typedef tAIObjRef tAIObjOut;
#define AI_OBJ_OUT(gao) AI_ref_FromObj(gao)
#endif
