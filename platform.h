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
int PLT_b_Open(const char* title, int width, int height);

void PLT_Close(void);

/* The window's native handle: an HWND on Windows, and what the
   renderers are handed. Null before the window is open. */
void* PLT_p_NativeWindow(void);

/* Once a frame, from the engine's loop. Everything the old window
   procedure did -- the engine paused when the window loses focus, the
   exit flag set when it is closed, the overlay's command field fed the
   keys it is typed into -- happens in here now. */
void PLT_Pump(void);

/* Start a program and do not wait for it: the arguments, the first of
   them the program, and a null to end the list. True if it started.
   
   The one caller hands off to the settings application when the
   display cannot be made and then leaves, so there is nothing to wait
   for and nothing to read back. */
int PLT_b_Launch(const char* const* args);

/* What the settings ask for. Left in one place so the rest of the
   startup does not have to know where they come from. */
void PLT_WindowSettings(int* width, int* height, int* windowed);

/* A message in front of the player, for the handful of things worth
   stopping them over: settings that cannot be made sense of, a device
   that will not come up, no memory left, a failed assertion.
   
   The game asked Windows for these directly, which is a dialog on one
   platform and nothing at all on the others. SDL puts up the system's
   own box wherever it runs, and knows about the game's window -- so a
   box raised while the game holds an exclusive display mode comes up
   in front of it rather than behind.
   
   error says which box it is: an error when something has gone wrong,
   and otherwise a plain notice. */
void PLT_Message(const char* title, const char* text, int error);

/* The keyboard and the mouse, as the game's own input code wants them:
   a 266-byte block indexed by **DirectInput's** key codes, and the
   five words the mouse device used to answer with. The game's tables,
   its controls screen and everyone's saved bindings are all written in
   those codes, so they stay -- see the table in platform.c. */
void PLT_ReadDevices(unsigned char* keys, int* mouse);

/* Two keys the engine asks after outside that block, because neither
   is a key being pressed in the game.

   Shift is a developer's modifier: held while something happens, it
   turns extra logging on. Num lock is remembered when the game takes
   the input, because it is a lamp on the keyboard and a state the
   player had before the game started, not a key at all.

   They are here rather than read out of the block above because the
   block is what the game's own input code reads, and these two are
   asked at moments when it has not been filled -- and because shift
   is one key to the player and two in that block. */
/* The window, as the library that made it knows it.
 *
 * PLT_p_NativeWindow below hands out the system's own handle, which
 * is what the Direct3D renderer wants. The OpenGL renderer wants this
 * one, because the context is made through the same library as the
 * window. Null before the window exists. */
void* PLT_p_Window(void);

int PLT_b_ShiftHeld(void);
int PLT_b_CtrlHeld(void);
int PLT_b_NumLockOn(void);

/* Whether one key is down, by the code the engine's own tables use --
   the same codes PLT_ReadDevices fills a block with.
 *
 * For the debug keys, which are asked about outside the block because
 * they are read where the block has not been filled.
 */
int PLT_b_KeyHeld(int key);

/* Waits for a key to come back up, which is what the debug keys do to
   act once per press rather than once per frame.
 *
 * Here rather than written as a loop at each site, because the loop
 * needs the key state refreshed inside it: the state the question
 * above reads is a snapshot the library updates, so a bare spin on it
 * would never see the key released and would hang the game. The call
 * it replaced asked the system afresh every time and had no such
 * problem, which is exactly the kind of difference that turns a
 * faithful transcription into a lock-up. */
void PLT_WaitKeyRelease(int key);

/* Whether the game has the input. Taking it locks the pointer to the
   window and hides it, which is what the exclusive device did; giving
   it back is what lets the overlay's command field be typed into. */
void PLT_InputCapture(int on);

/* What a key is called, for the controls screen. Empty for a code the
   keyboard has no key for. */
const char* PLT_sz_KeyName(int dik);
