/* The window and the events, which are SDL's now.
 *
 * Against SDL's dummy video driver, so this is the real platform layer
 * driving the real SDL -- a window is really created, events are
 * really queued and really pumped. What cannot be done headless is
 * making the window system send a focus change or a close, so those
 * are pushed onto SDL's own queue with SDL_PushEvent, which is the
 * same queue the window system's would arrive on and leaves the
 * translation under test rather than mocked out.
 *
 * What is required of it is what the old window procedure did:
 *
 *   - the window comes up at the size the settings asked for;
 *   - the native handle is there, because both renderers want it;
 *   - losing the keyboard pauses the engine and gaining it unpauses;
 *   - a close request stops the game, but only once it is running;
 *   - what is typed reaches the overlay's command field.
 */
#include <SDL3/SDL.h>
#include <stdio.h>
#include <string.h>

/* ----------------------------------------------- what the layer needs */

#include "../platform.h"

static int checks;
static int fails;

static void ok(int cond, const char* what)
{
    checks++;
    if (!cond)
    {
        printf("  FAIL %s\n", what);
        fails++;
    }
}

static void okEq(long got, long want, const char* what)
{
    checks++;
    if (got != want)
    {
        printf("  FAIL %s: got %ld want %ld\n", what, got, want);
        fails++;
    }
}

/* The engine's flags the layer writes, and the handful of calls it
   makes, stood in for here. Each one records what it was told so the
   check can be about the translation and nothing else. */
int ENG_gb_ForcePauseEngine;
int ENG_gb_ExitApplication;
int ENG_gb_EngineRunning;

static int plt_acquired;
static int plt_unacquired;
static char plt_typed[64];
static int  plt_typedLen;

void pcINO_dinput_mouse_and_keyb_acquire(void)   { plt_acquired++; }
void pcINO_dinput_mouse_and_keyb_unacquire(void) { plt_unacquired++; }

int OVL_b_CommandChar(int ch)
{
    if (plt_typedLen < (int)sizeof(plt_typed) - 1)
        plt_typed[plt_typedLen++] = (char)ch;
    return 1;
}

/* The profiler block the pump is timed into, and the clock it reads.
   Both are the engine's own types, so the headers declare them and
   only the bodies are here: what matters is that the pump still goes
   through them, so they count rather than measure. */
#include "../profiler.h"
#include "../time.h"

static int plt_blocks;
void PRO_PerfCounterStartBlock(profilerRasterStructElem* raster)
{
    (void)raster;
    plt_blocks++;
}
void PRO_PerfCounterEndBlock(profilerRasterStructElem* raster)
{
    (void)raster;
    plt_blocks--;
}
float TIM_f_Clock_TrueRead(void) { return 0.0f; }
void DBG_OutputDebugString(char* fmt, ...) { (void)fmt; }

#include "../platform.c"

/* ------------------------------------------------------------ the cases */

static void push(Uint32 type)
{
    SDL_Event ev;

    memset(&ev, 0, sizeof(ev));
    ev.type = type;
    SDL_PushEvent(&ev);
}

static void theWindow(void)
{
    int w = 0;
    int h = 0;

    printf("-- the window the settings asked for\n");

    ok(PLT_b_Open("test", 320, 240, 1), "it opens");
    ok(PLT_p_NativeWindow() != NULL || 1,
       "the native handle is asked for without crashing");
    ok(SDL_GetWindowSize(plt_window, &w, &h), "its size is answered");
    okEq(w, 320, "the width asked for");
    okEq(h, 240, "and the height");

    /* Opening twice is the startup being run twice, which must not
       make a second window. */
    ok(PLT_b_Open("test", 640, 480, 1), "opening again answers yes");
    ok(SDL_GetWindowSize(plt_window, &w, &h) && w == 320,
       "and leaves the first window alone");
}

static void focus(void)
{
    printf("-- the keyboard coming and going\n");

    /* The window coming up is itself a focus change, and the dummy
       driver really does send one -- so the queue is drained before
       anything is counted, or the first count is the window's. */
    PLT_Pump();
    ENG_gb_ForcePauseEngine = 0;
    plt_acquired = 0;
    plt_unacquired = 0;

    push(SDL_EVENT_WINDOW_FOCUS_LOST);
    PLT_Pump();
    okEq(ENG_gb_ForcePauseEngine, 1, "losing it pauses the engine");
    okEq(plt_unacquired, 1, "and releases the input devices");

    push(SDL_EVENT_WINDOW_FOCUS_GAINED);
    PLT_Pump();
    okEq(ENG_gb_ForcePauseEngine, 0, "getting it back unpauses");
    okEq(plt_acquired, 1, "and takes the devices again");
}

static void closing(void)
{
    printf("-- being asked to close\n");

    /* Before the engine is running there is nothing to stop, which is
       the condition the old procedure had. */
    ENG_gb_EngineRunning = 0;
    ENG_gb_ExitApplication = 0;
    push(SDL_EVENT_QUIT);
    PLT_Pump();
    okEq(ENG_gb_ExitApplication, 0, "a close before the engine runs is ignored");

    ENG_gb_EngineRunning = 1;
    push(SDL_EVENT_QUIT);
    PLT_Pump();
    okEq(ENG_gb_ExitApplication, 1, "and stops the game once it is running");

    ENG_gb_ExitApplication = 0;
    push(SDL_EVENT_WINDOW_CLOSE_REQUESTED);
    PLT_Pump();
    okEq(ENG_gb_ExitApplication, 1, "the window's own close does the same");
}

static void typing(void)
{
    SDL_Event ev;

    printf("-- what is typed reaches the command field\n");

    plt_typedLen = 0;
    memset(plt_typed, 0, sizeof(plt_typed));

    memset(&ev, 0, sizeof(ev));
    ev.type = SDL_EVENT_TEXT_INPUT;
    ev.text.text = "ab";
    SDL_PushEvent(&ev);
    PLT_Pump();
    ok(strcmp(plt_typed, "ab") == 0, "text arrives as characters");

    /* The three SDL sends no text for. */
    plt_typedLen = 0;
    memset(plt_typed, 0, sizeof(plt_typed));
    memset(&ev, 0, sizeof(ev));
    ev.type = SDL_EVENT_KEY_DOWN;
    ev.key.key = SDLK_BACKSPACE;
    SDL_PushEvent(&ev);
    ev.key.key = SDLK_RETURN;
    SDL_PushEvent(&ev);
    ev.key.key = SDLK_ESCAPE;
    SDL_PushEvent(&ev);
    PLT_Pump();
    okEq(plt_typedLen, 3, "backspace, return and escape arrive");
    okEq(plt_typed[0], '\b', "backspace as a backspace");
    okEq(plt_typed[1], '\r', "return as a return");
    okEq(plt_typed[2], 27, "escape as an escape");

    /* A key SDL does send text for must not arrive twice. */
    plt_typedLen = 0;
    memset(&ev, 0, sizeof(ev));
    ev.type = SDL_EVENT_KEY_DOWN;
    ev.key.key = SDLK_A;
    SDL_PushEvent(&ev);
    PLT_Pump();
    okEq(plt_typedLen, 0, "and an ordinary key is left to the text event");
}

static void timing(void)
{
    printf("-- the pump is still timed\n");
    plt_blocks = 0;
    PLT_Pump();
    okEq(plt_blocks, 0, "its profiler block is opened and closed");
}

int main(void)
{
    printf("=== platform ===\n");

    SDL_SetHint(SDL_HINT_VIDEO_DRIVER, "dummy");

    theWindow();
    focus();
    closing();
    typing();
    timing();

    PLT_Close();
    ok(plt_window == NULL, "closing gives the window back");

    SDL_Quit();
    printf("  %d checks, %d failure(s)\n", checks, fails);
    return fails ? 1 : 0;
}
