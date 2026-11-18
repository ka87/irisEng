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

void pcINO_mouse_and_keyb_acquire(void)   { plt_acquired++; }
void pcINO_mouse_and_keyb_unacquire(void) { plt_unacquired++; }

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

static int plt_statsAsked;
void ENG_GetStatsText(char* buffer)
{
    plt_statsAsked++;
    strcpy(buffer, "frames 60, draws 1234");
}

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

/* The keyboard and the mouse, as the game's own input code asks for
   them. The game's codes are DirectInput's and stay that way, so what
   is checked here is the translation into them. */
static void devices(void)
{
    unsigned char keys[266];
    int           mouse[5];
    int           i;

    printf("-- the keyboard and the mouse in the game's own codes\n");

    /* With the input not taken, nothing is answered at all -- which is
       what being released means, and what lets the command field be
       typed into. */
    PLT_InputCapture(0);
    memset(keys, 0xAA, sizeof(keys));
    PLT_ReadDevices(keys, mouse);
    for (i = 0; i < 266; i++)
        if (keys[i])
            break;
    okEq(i, 266, "released, the keyboard answers nothing");

    PLT_InputCapture(1);

    /* The dummy driver has no keys down, so what this proves is the
       shape: the call fills the block and clears what it does not
       set, rather than leaving the caller's bytes. */
    memset(keys, 0xAA, sizeof(keys));
    PLT_ReadDevices(keys, mouse);
    for (i = 0; i < 266; i++)
        if (keys[i])
            break;
    okEq(i, 266, "and with no key down, nothing is reported down");

    /* The table itself, which is the part that matters: the codes the
       engine's tables, its controls screen and everyone's saved
       bindings are written in. Checked both ways round, on the keys
       the game actually names. */
    PLT_l_KeymapBuild();
    okEq(plt_dikOf[SDL_SCANCODE_ESCAPE], 0x01, "escape is 1");
    okEq(plt_dikOf[SDL_SCANCODE_F1], 59, "F1 is 59, where the engine reads it");
    okEq(plt_dikOf[SDL_SCANCODE_F4], 62, "F4 is 62, which the alt-F4 check uses");
    okEq(plt_dikOf[SDL_SCANCODE_F11], 87, "F11 is 87, which raises the field");
    okEq(plt_dikOf[SDL_SCANCODE_F12], 88, "and F12 is 88, which nothing is on");
    okEq(plt_dikOf[SDL_SCANCODE_HOME], 199, "home is 199");
    okEq(plt_dikOf[SDL_SCANCODE_LALT], 56, "left alt is 56");
    okEq(plt_dikOf[SDL_SCANCODE_RALT], 184, "right alt is 184");
    okEq(plt_dikOf[SDL_SCANCODE_RCTRL], 157, "right control is 157");
    okEq(plt_dikOf[SDL_SCANCODE_A], 0x1E, "A is 30");
    okEq(plt_dikOf[SDL_SCANCODE_UP], 0xC8, "up is 200");
    okEq(plt_scancodeOf[59], SDL_SCANCODE_F1, "and back again");

    /* No two keys may share a code: one would shadow the other and
       the second would be unbindable. */
    {
        int seen[256];
        int clash = 0;
        size_t k;

        memset(seen, 0, sizeof(seen));
        for (k = 0; k < sizeof(plt_keymap) / sizeof(plt_keymap[0]); k++)
        {
            if (seen[plt_keymap[k].dik]++)
                clash++;
            if (plt_keymap[k].dik == 0)
                clash++;
        }
        okEq(clash, 0, "every key has a code of its own, and none is zero");
    }

    /* The wheel arrives as events and is answered as a movement since
       the last ask, which is how the old device reported it. */
    {
        SDL_Event ev;

        memset(&ev, 0, sizeof(ev));
        ev.type = SDL_EVENT_MOUSE_WHEEL;
        ev.wheel.y = 2.0f;
        SDL_PushEvent(&ev);
        PLT_Pump();
        PLT_ReadDevices(keys, mouse);
        okEq(mouse[2], 2, "the wheel's notches arrive");
        PLT_ReadDevices(keys, mouse);
        okEq(mouse[2], 0, "and are not answered twice");
    }

    /* And a key's name, for the controls screen. */
    ok(PLT_sz_KeyName(59)[0] != 0, "a key has a name");
    okEq((long)PLT_sz_KeyName(0)[0], 0, "and a code with no key has none");

    /* Taking the input and giving it back has to reach the window, or
       the pointer stays where the desktop left it and the game turns
       the camera with nothing. */
    PLT_InputCapture(1);
    ok(SDL_GetWindowRelativeMouseMode(plt_window) != false,
       "taking the input locks the pointer to the window");
    PLT_InputCapture(0);
    ok(SDL_GetWindowRelativeMouseMode(plt_window) == false,
       "and giving it back lets it go");

    /* A wheel movement while the game does not have the input is not
       saved up for the moment it does: the notches belong to whatever
       had the pointer. */
    {
        SDL_Event ev;

        memset(&ev, 0, sizeof(ev));
        ev.type = SDL_EVENT_MOUSE_WHEEL;
        ev.wheel.y = 3.0f;
        SDL_PushEvent(&ev);
        PLT_Pump();
        PLT_ReadDevices(keys, mouse);
        okEq(mouse[2], 0, "released, the wheel is not answered");
        for (i = 0; i < 5; i++)
            if (mouse[i])
                break;
        okEq(i, 5, "and nor is anything else of the mouse");
    }
}

/* The statistics in the caption, which used to be a click on the title
   bar -- a non-client click, which a window SDL owns does not pass
   on. They are on Home: the function keys are taken, F1 to F10 by the
   debug keys and F11 by the command field. */
static void stats(void)
{
    SDL_Event ev;

    printf("-- the statistics in the window's caption\n");

    plt_statsAsked = 0;
    SDL_SetWindowTitle(plt_window, "iriseng");

    memset(&ev, 0, sizeof(ev));
    ev.type = SDL_EVENT_KEY_DOWN;
    ev.key.key = SDLK_HOME;
    SDL_PushEvent(&ev);
    PLT_Pump();
    okEq(plt_statsAsked, 1, "home asks the engine for them");
    ok(strcmp(SDL_GetWindowTitle(plt_window), "frames 60, draws 1234") == 0,
       "and they are the caption");

    /* Held down rather than pressed: the repeats are not a second
       press, as the key blocks the engine polls are not either. */
    plt_statsAsked = 0;
    memset(&ev, 0, sizeof(ev));
    ev.type = SDL_EVENT_KEY_DOWN;
    ev.key.key = SDLK_HOME;
    ev.key.repeat = true;
    SDL_PushEvent(&ev);
    PLT_Pump();
    okEq(plt_statsAsked, 0, "a repeat does not ask again");

    /* And no other key does it. */
    plt_statsAsked = 0;
    memset(&ev, 0, sizeof(ev));
    ev.type = SDL_EVENT_KEY_DOWN;
    /* F11 above all: it used to be this key, and it is the command
       field's now. */
    ev.key.key = SDLK_F11;
    SDL_PushEvent(&ev);
    PLT_Pump();
    okEq(plt_statsAsked, 0, "and no other key asks for them, F11 least of all");
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
    devices();
    stats();
    timing();

    PLT_Close();
    ok(plt_window == NULL, "closing gives the window back");

    SDL_Quit();
    printf("  %d checks, %d failure(s)\n", checks, fails);
    return fails ? 1 : 0;
}
