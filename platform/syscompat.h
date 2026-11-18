#pragma once

/* The handful of system services the engine asks for by Windows' own
 * names, and the one it should not.
 *
 * Two different things live here:
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
