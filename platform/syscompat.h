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
 * strcasecmp and Microsoft's library has the function under a name of
 * its own. The engine says the POSIX one, because the rest of what it
 * says -- snprintf, vsnprintf, hypot -- is standard C that both
 * libraries have, and one spelling throughout is worth more than
 * matching whichever library happens to be underneath. */
#ifdef _WIN32
#define strcasecmp _stricmp
#else
#include <strings.h>
#endif
