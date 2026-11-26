#pragma once

/* _CRT_SECURE_NO_WARNINGS was defined here. It is defined for every
   configuration in irisEng.vcxproj instead -- the project file is
   the only place immune to include order, and FreeType's sources
   never reach this header at all. Defining it in both places is a
   redefinition: the command line makes it 1 and a bare #define
   makes it empty, so the two replacement lists differ and every
   translation unit says so. */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <math.h>
/* FLT_MAX and its neighbours, which six files use and none of them
   include: dynamics.c, grid.c, platform/d3drender.c, graphicsobjects/shadow.c
   and the two ai/ ones. They reached it through a chain that is on its
   way out, so it is asked for here, beside math.h, which they reach
   the same way. */
#include <float.h>
/* And the limits, which are standard C and which the engine reads --
   SHRT_MAX in math.c -- but which used to arrive only because
   windows.h dragged them in. */
#include <limits.h>
/* The same story for offsetof, which gdi.c measures a buffer with. */
#include <stddef.h>


/* A compile-time check that holds on every compiler this builds with.
 *
 * `_Static_assert` is C11. MSVC's C compiler takes it only under
 * /std:c11 or later and this project sets no standard, so on a default
 * MSVC build the keyword is unknown, the assertion parses as a
 * function declaration, and each one is five or six syntax errors in a
 * header -- which reads as a broken header rather than as a missing
 * switch. Four headers grew one of these and nothing here compiles
 * with MSVC, so both Windows configurations had been failing for three
 * days before anyone built them.
 *
 * So the checks go through this: the keyword where the compiler says
 * it has C11, and an array that cannot have its size otherwise. The
 * message stays an argument either way, so it is on the screen beside
 * the failure even where the compiler cannot print it. */
#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L
#define IRIS_STATIC_ASSERT(cond, msg) _Static_assert(cond, msg)
#else
#define IRIS_SA_JOIN2(a, b) a##b
#define IRIS_SA_JOIN(a, b) IRIS_SA_JOIN2(a, b)
#define IRIS_STATIC_ASSERT(cond, msg) \
    typedef char IRIS_SA_JOIN(iris_static_assert_, __COUNTER__)[(cond) ? 1 : -1]
#endif
/* The debugger check and the C library's POSIX spellings, which the
   engine uses everywhere and so are asked for here. */
#include "platform/syscompat.h"

/* Windows' own, where there is a Windows to have them. What the engine
   actually wants out of these is answered portably elsewhere now -- the
   POSIX spellings and the debugger check in platform/syscompat.h above,
   the wide seek and the free-space query in fileio.h, the window and the
   input in platform.h -- so what is still taken from here is the types
   and the handles the Win32-only files name, and syscompat.h declares
   those itself where there is no windows.h.

   The condition asks whether the header is reachable rather than
   whether this is Windows, and on purpose: the test harnesses stand a
   stub of it in tests/stub and are built by a compiler that does not
   define _WIN32, so asking _WIN32 would take the stub away from the
   one place that put it there. Off Windows and outside the harnesses
   there is no windows.h and the branch is not taken. */
#if defined(_WIN32) || __has_include(<windows.h>)
#include <windows.h>
#include <synchapi.h>
#include <io.h>
#endif
#include <sys/stat.h> // for _S_IWRITE on Windows, and stat everywhere

// Bink
#define RAD_NO_LOWERCASE_TYPES // to be able to compile with lzolib (u32 redef problem)

/*
#ifndef BOOL
typedef int8_t BOOL;
#define TRUE  1
#define FALSE !TRUE
#endif
*/

#define struct_member_size(type, member) sizeof(((type *)0)->member)

float ENG_gf_SpecialTime;
float ENG_gb_LastSpecialTime;
float ENG_gf_SpecialDT;
BOOL ENG_gb_AIRunning;
int ENG_gb_ExitApplication;
int ENG_gb_ForceEndEngine;
int ENG_gb_ForcePauseEngine;
int ENG_gb_FirstFrame;
int ENG_gb_MustRebuildDyn;
int ENG_gb_EngineRunning;
int ENG_IsSingleStream;
int ENG_IsDLBFEnabled;
BOOL ENG_gb_ForceAttach;

/* 256 values and one more: SPG_PackAndAddData reads two neighbours, so
   the last sprite of a batch of 256 reads [256]. It is a copy of [0]. */
float f_RandomTable[256 + 1];
int ulRandomInit;

char MAI_gst_BIGFileName[288];
char cmdLine[53856];
HMODULE ENG_bgeModuleHandle;

extern int ENG_AIProfilerEnabled;
extern int ENG_ifSingleStreamThenEq2Otherwise8;

int RND_JADErand();
/* rand() in MSVC's range, 0..0x7FFF: the engine scales it by 1/32767
   everywhere, and glibc's RAND_MAX is 2^31-1. Here rather than in
   common.c because math.c carries two of its callers and is linked
   into a dozen test harnesses, none of which link common.c. */
static inline int RND_i_Rand(void)
{
    return rand() & 0x7FFF;
}
float RND_RandFloatRange(float start, float end);