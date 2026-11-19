#pragma once

/* The handful of system services the engine asks for by Windows' own
 * names, and the one it should not.
 *
 * Two different things live here:
 *
 * - **A single-instance guard.** The game refuses to run twice over,
 *   which it asked for with a named mutex -- a Windows object with no
 *   counterpart elsewhere. PLT_b_OnlyInstance is the question without
 *   the answer's platform in it.
 *
 * - **A debugger check the engine can ask anywhere.** The renderer
 *   wants to know whether it is being debugged, because taking an
 *   exclusive display mode under a debugger wedges the machine. It
 *   used to ask by loading kernel32 and looking IsDebuggerPresent up
 *   in it, which is Windows and nothing else. PLT_b_DebuggerAttached
 *   is the question without the answer's platform in it.
 *
 * - **Implementations of the Win32 clock and debug output off
 *   Windows**, in syscompat.c and declared by windows.h rather than
 *   here. QueryPerformanceCounter is called in about a hundred places
 *   across the engine, so it keeps its name and gains an
 *   implementation; the alternative is a hundred edits to say the
 *   same thing. The tests do not link this file -- they have mocks of
 *   their own for all of these -- so nothing here can collide with
 *   them.
 */

/* Whether a debugger has this process. Windows asks the kernel;
   Linux reads TracerPid out of /proc/self/status, which is how the
   same question is asked there. Anywhere else the answer is no,
   which is the safe way to be wrong: it costs an exclusive display
   mode, where the other way costs a wedged machine. */
int PLT_b_DebuggerAttached(void);

/* Whether this is the only copy of the game running. Asked once, at
   startup, and the thing that says so -- a named mutex on Windows, a
   locked file beside the saves elsewhere -- is then held for the life
   of the process and released by the system when it goes, so a crash
   does not lock the player out of their own game.

   A guard that cannot be put up at all answers yes. Being unable to
   ask is not a reason to refuse to start, and the cost of being wrong
   is two copies running rather than none. */
int PLT_b_OnlyInstance(void);

/* The C library under its POSIX name.
 *
 * Case-insensitive compare has no standard spelling: POSIX calls it
 * strcasecmp, and the prefix form strncasecmp, and Microsoft's library
 * has both under names of its own. The engine says the POSIX one, because the rest of what it
 * says -- snprintf, vsnprintf, hypot -- is standard C that both
 * libraries have, and one spelling throughout is worth more than
 * matching whichever library happens to be underneath. */
#ifdef _WIN32
#define strcasecmp  _stricmp
#define strncasecmp _strnicmp
#else
#include <strings.h>
#endif

/* The two Win32 type names the engine says in code that is not
 * Windows-only.
 *
 * `BOOL` is in the engine's own vocabulary -- its globals, its
 * function signatures and its struct fields -- and `HMODULE` holds the
 * handle the entry point is given and the handle a sound library is
 * loaded under. Both come out of windows.h where there is one, and
 * both are needed off Windows for the files that merely *mention*
 * them to compile.
 *
 * `BOOL` is `int` and nothing else. Windows' is a signed 32-bit int,
 * so it decides the size of every struct field and global declared
 * with it; a narrower stand-in -- common.h carried a commented-out
 * `int8_t` one for years -- would quietly give the two builds
 * different layouts for the same struct, which is the worst way for
 * this to be wrong.
 *
 * `HMODULE` is an opaque handle, so a void pointer carries it. Nothing
 * off Windows loads a library through one yet; this is here so that a
 * declaration naming the type compiles, not as a promise that the
 * thing behind it works. */
/* Exactly when common.h will not be including windows.h: the same
   condition it asks, so that the two cannot both define these. The
   harnesses stand a stub of windows.h in tests/stub and it declares
   all of this already. */
#if !defined(_WIN32) && !__has_include(<windows.h>)
typedef int BOOL;
typedef void* HMODULE;
/* Three more handles named in headers that are not Windows-only:
   HWND in gdi.h and the renderer interface, HANDLE in sound.h, and
   LARGE_INTEGER in profiler.h. Opaque, so a pointer carries the first
   two and the third is the 64-bit counter it is on Windows, spelled as
   the union so that the .QuadPart the engine reads still reads.

   As with HMODULE above: this is so that a declaration naming the type
   compiles. Nothing off Windows has a window handle or a wait handle
   to put in one -- the window is SDL's and is reached through
   PLT_p_Window -- and the code that would fill them is the code that
   has not left Windows yet. */
typedef void* HWND;
typedef void* HANDLE;
/* And one that is not a handle at all: CHAR, in dbg.h, which is
   Windows' name for char and nothing more. */
typedef char CHAR;
/* Two more spellings of things C already has: DWORD is a 32-bit
   unsigned and SIZE_T is size_t. syscompat.c names the first in its
   own non-Windows half, and memory.c names the second in the
   allocator's signature. */
typedef unsigned int DWORD;
typedef unsigned int UINT;
/* And LONG, which is a 32-bit signed int and not C's long: the sound
   backend holds a stereo balance in one. */
typedef int LONG;
typedef const char* LPCSTR;
typedef size_t SIZE_T;
/* The longest path the engine is willing to hold on the stack. Windows
   puts this at 260; nothing here depends on the number matching, only
   on the buffer being big enough for what the loader builds. */
#ifndef MAX_PATH
#define MAX_PATH 260

/* windows.h defines these two and the engine uses them as if they
   were the language's -- lights.c clamps a colour channel with min.
   Spelled the same way windows.h spells them, so a build on either
   side computes the same thing. */
#ifndef min
#define min(a, b) (((a) < (b)) ? (a) : (b))
#endif
#ifndef max
#define max(a, b) (((a) > (b)) ? (a) : (b))
#endif
#endif
typedef union { struct { unsigned int LowPart; int HighPart; }; long long QuadPart; } LARGE_INTEGER;

/* The millisecond timer and the two calls that ask the system for a
   finer one. Windows declares these in <timeapi.h> and syscompat.c
   answers them off Windows -- the timer from the monotonic clock, the
   other two with nothing to do, there being no system-wide timer
   resolution to set.
 *
 * Declared here because they were declared nowhere: the definitions
 * have been in syscompat.c since the move off winmm, and every caller
 * was taking them from <timeapi.h> on Windows and from the test stub's
 * windows.h off it. A real build off Windows got an implicit
 * declaration, which is a warning the sweep does not see because the
 * sweep compiles against that same stub. */
/* The high-resolution counter, which the profiler's instrumentation
   calls in about a hundred places, and the debugger's ear. All three
   are answered in syscompat.c; they are declared here for the same
   reason as the timer below, which is that windows.h is where the
   engine used to get them from. */
int   QueryPerformanceCounter(LARGE_INTEGER* out);
BOOL  QueryPerformanceFrequency(LARGE_INTEGER* out);
void  OutputDebugStringA(LPCSTR text);

DWORD timeGetTime(void);
DWORD timeBeginPeriod(UINT period);
DWORD timeEndPeriod(UINT period);
#ifndef TRUE
#define TRUE  1
#endif
#ifndef FALSE
#define FALSE 0
#endif
#endif
