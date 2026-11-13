#pragma once
#include "../gfx.h"
#include "../gdi.h"
#include "../time.h"
#include "../soft.h"
#include "../world.h"
#include "../profiler.h"
#include "../graphicsobjects/geo.h"
#include "../sprites.h"

/* The engine around gfx.c: a bump allocator whose addresses repeat
   exactly from one side of a case to the other, the twenty-odd effect
   entry points gfx.c dispatches to, the display data and the geometry
   buffer, plus an ordered record of every call either side made. */

#define GFXC_LOG_MAX 262144
#define GFXC_ARGS 6

typedef struct sGfxcCall
{
    const char* fn;
    int nargs;
    /* the display state in force when the call was made, so a render
       state set on the way into an effect is observable even though
       GFX_Render puts it back before it returns */
    int state;
    int drawMask;
    unsigned long long arg[GFXC_ARGS];
} tGfxcCall;

extern tGfxcCall GFXC_log[GFXC_LOG_MAX];
extern int GFXC_logLen;
extern int GFXC_logOverflow;

/* Every effect render entry point returns this, so a case can drive
   both the kept and the dropped path through the switch. */
extern int GFXC_renderResult;

/* What a fresh block is filled with.  The engine's allocator does not
   zero, and a zero fill hides any memset whose target is already
   clear -- one injected defect was invisible until this was poison. */
#define GFXC_POISON 0xA5

/* The arena an allocation comes out of.  Static storage, so every
   pointer fits in 32 bits under -no-pie and the frozen copy's
   (unsigned int) cast of one is lossless. */
#define GFXC_ARENA_BYTES (4 * 1024 * 1024)
extern unsigned char GFXC_arena[GFXC_ARENA_BYTES];
extern unsigned int GFXC_arenaUsed;

extern tWorld GFXC_world;

/* Puts the allocator, the log, the display data, the world and every
   gfx global back to the state a case starts in. */
void GFXC_Reset(void);

unsigned long long GFXC_F(float f);
unsigned long long GFXC_P(const void* p);
void GFXC_Call0(const char* fn);
void GFXC_Call(const char* fn, int n, ...);
