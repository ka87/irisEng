#pragma once

/* The window and the events, which are SDL's rather than Windows' own.
 *
 * The game used to register a window class, create a window, and run a
 * message loop with a procedure of its own. All three were Windows and
 * nothing else, and they were in the Direct3D renderer's file -- which
 * is not where a window belongs, since the other renderer needs the
 * same one.
 *
 * SDL owns both now: it creates the window and it is what the engine
 * pumps once a frame. What the engine's own procedure used to do with
 * the messages it cared about is done here with the events that mean
 * the same thing.
 *
 * The renderers still want the native handle -- Direct3D takes an HWND
 * and so does the OpenGL context -- so it is asked for rather than
 * assumed, and it is the only part of this that is platform-shaped.
 */

/* Opens the window. Returns zero and says why in the log if SDL or the
   window will not come up, which is fatal rather than something to
   carry on past. */
int PLT_b_Open(const char* title, int width, int height, int windowed);

void PLT_Close(void);

/* The window's native handle: an HWND on Windows, and what the
   renderers are handed. Null before the window is open. */
void* PLT_p_NativeWindow(void);

/* Once a frame, from the engine's loop. Everything the old window
   procedure did -- the engine paused when the window loses focus, the
   exit flag set when it is closed, the overlay's command field fed the
   keys it is typed into -- happens in here now. */
void PLT_Pump(void);

/* What the settings ask for, which on Windows is still the registry
   the game has always kept them in. Left in one place so the rest of
   the startup does not have to know where they come from. */
void PLT_WindowSettings(int* width, int* height, int* windowed);
