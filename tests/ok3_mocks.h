#pragma once
#include "../memory.h"
#include "../gameobject.h"
#include "../graphicsobjects/geo.h"
#include "../collision.h"

/* ok3.c needs an allocator, one collision entry point and nothing
   else: the real math.c is linked rather than mocked, so the vector
   helpers are the engine's own.
 *
 * INT_PointInTriangle lives in collision.c, which would drag in the
 * whole collision mock set, so it is replaced here by a deterministic
 * function of its own arguments.  Both sides call the same one, and
 * every argument it is handed goes into the call log, so the hit
 * point and the normal the two copies compute are compared exactly
 * even though the predicate itself is not the engine's. */

#define OK3M_LOG_MAX 2000000
#define OK3M_ARGS 8

typedef struct sOk3Call
{
    const char* fn;
    int nargs;
    unsigned long long arg[OK3M_ARGS];
} tOk3Call;

extern tOk3Call OK3M_log[OK3M_LOG_MAX];
extern int OK3M_logLen;
extern int OK3M_logOverflow;

/* The allocator hands out blocks from here, poisoned rather than
   zeroed.  Over-allocation is deliberate: ok3.c computes several
   block sizes with the 32-bit sizes of its own structs written as
   the literals 8 and 32, which are short on a 64-bit host.  Both
   copies compute the same short size, so the comparison stays
   valid; the slack is what keeps the run from scribbling outside
   the arena while it does so. */
#define OK3M_ARENA_BYTES (32 * 1024 * 1024)
#define OK3M_SLACK 4
#define OK3M_POISON 0x5C

extern unsigned char OK3M_arena[OK3M_ARENA_BYTES];
extern unsigned int OK3M_arenaUsed;

void OK3M_Reset(void);
unsigned long long OK3M_F(float f);
unsigned long long OK3M_P(const void* p);
void* OK3M_Bump(unsigned int size, unsigned int align);
