#include "platform.h"
#include "settings.h"
#include <SDL3/SDL_process.h>

#include "common.h"
#include "engine.h"
#include "inputoutput.h"
#include "profiler.h"
#include "time.h"
#include "dbg.h"
#include "platform/overlay.h"
#include "platform/inputbackend.h"

#include <SDL3/SDL.h>
#include <string.h>

/* The window, and the events that used to be window messages.
 *
 * Which events stand in for which message is the whole of the
 * interesting part, so each one says so where it is handled below.
 */

static SDL_Window* plt_window;
static int         plt_up;

void PLT_WindowSettings(int* width, int* height, int* windowed)
{
    int value;

    /* The defaults the game has always started from, kept for
       anything the settings do not mention. */
    if (width)
        *width = 640;
    if (height)
        *height = 480;
    if (windowed)
        *windowed = 0;

    /* The settings come out of engine.ini beside the executable,
       where they used to come out of the registry. A value the ini
       does not carry leaves the default standing, which is how the
       registry read behaved too -- a missing value left the default
       rather than zeroing it. */
    if (windowed && SET_b_DisplayWindowed(&value) && value)
        *windowed = 1;
    if (width && SET_b_DisplayWidth(&value) && value > 0)
        *width = value;
    if (height && SET_b_DisplayHeight(&value) && value > 0)
        *height = value;
}

int PLT_b_Open(const char* title, int width, int height)
{
    SDL_WindowFlags flags = 0;

    if (plt_window)
        return 1;

    /* Video and events only. The gamepads come up with the input work
       and the audio with the sound work, each where it is used. */
    if (!SDL_InitSubSystem(SDL_INIT_VIDEO))
    {
        DBG_OutputDebugString("PLT: SDL video would not start: %s\n",
                              SDL_GetError());
        return 0;
    }
    plt_up = 1;

    /* An ordinary window with a sizing border, which is the only kind
       the game has ever made: the style it asked Windows for was
       border, dialogue frame, system menu, minimise, maximise and
       thick frame, at a size out of the settings.
       
       Deliberately not fullscreen. Going fullscreen is the renderer's
       own business and always has been -- the Direct3D device asks for
       an exclusive mode, or does not, and that decision reads the
       engine's windowed flag and whether a debugger is attached. A
       window that is itself fullscreen puts a second mode change
       underneath that one, and the two disagree in exactly the case
       the flag exists for: under a debugger the device wants a
       windowed presentation and would get it inside a fullscreen
       window.
       
       When the OpenGL renderer becomes the way the game runs off
       Windows, SDL's own fullscreen is what replaces the device's
       mode change. It is not a second route to it while Direct3D is
       still doing it. */
    flags = SDL_WINDOW_RESIZABLE;

    /* A window a GL context can go on, and the format that context
       will want: eight bits a channel, a 24-bit depth buffer and
       eight of stencil, which is what both renderers draw against.
       Asked for here and not in the renderer because a window's
       format is settled when the window is made, and the renderer is
       not chosen until afterwards -- it is chosen by asking each one
       whether it can run, and the OpenGL one answers that by making a
       context, which needs a window of some kind to exist first.

       Costing the Direct3D path nothing is the reason it is asked for
       this way round rather than refused: a format good for GL is no
       worse for a Direct3D device, which does not look at it. But a
       machine with no GL at all can refuse the window outright, and
       that must not stop a game that was going to use Direct3D, so a
       refusal is retried without it. */
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    plt_window = SDL_CreateWindow(title ? title : "IRISEngine",
                                  width > 0 ? width : 640,
                                  height > 0 ? height : 480,
                                  flags | SDL_WINDOW_OPENGL);
    if (!plt_window)
    {
        DBG_OutputDebugString("PLT: no window that can carry a GL "
                              "context (%s); trying without\n",
                              SDL_GetError());
        plt_window = SDL_CreateWindow(title ? title : "IRISEngine",
                                      width > 0 ? width : 640,
                                      height > 0 ? height : 480,
                                      flags);
    }
    if (!plt_window)
    {
        DBG_OutputDebugString("PLT: no window: %s\n", SDL_GetError());
        return 0;
    }

    /* The old procedure hid the cursor on the first mouse message it
       saw. Said once here instead, which is the same thing without
       waiting for a mouse to move. */
    SDL_HideCursor();
    SDL_ShowWindow(plt_window);
    return 1;
}

void PLT_Close(void)
{
    if (plt_window)
    {
        SDL_DestroyWindow(plt_window);
        plt_window = NULL;
    }
    if (plt_up)
    {
        SDL_QuitSubSystem(SDL_INIT_VIDEO);
        plt_up = 0;
    }
}

int PLT_b_ShiftHeld(void)
{
    /* Either shift, which is what the one question about it means. */
    return (SDL_GetModState() & SDL_KMOD_SHIFT) != 0;
}

int PLT_b_CtrlHeld(void)
{
    /* Either control, as with shift. */
    return (SDL_GetModState() & SDL_KMOD_CTRL) != 0;
}

int PLT_b_NumLockOn(void)
{
    /* The lamp, not the key: this is the toggle's state. */
    return (SDL_GetModState() & SDL_KMOD_NUM) != 0;
}

void PLT_Message(const char* title, const char* text, int error)
{
    /* The window if there is one, so the box belongs to it. Before it
       is made -- which is when the settings and the device complain --
       there is nothing to belong to and SDL puts up a box of its own
       anyway. */
    SDL_ShowSimpleMessageBox(error ? SDL_MESSAGEBOX_ERROR
                                   : SDL_MESSAGEBOX_INFORMATION,
                             title ? title : "IRISEngine",
                             text ? text : "",
                             plt_window);
}

void* PLT_p_Window(void)
{
    return plt_window;
}

void* PLT_p_NativeWindow(void)
{
    if (!plt_window)
        return NULL;
#ifdef SDL_PLATFORM_WIN32
    return SDL_GetPointerProperty(SDL_GetWindowProperties(plt_window),
                                  SDL_PROP_WINDOW_WIN32_HWND_POINTER, NULL);
#else
    /* Nothing else needs a native handle yet: the renderers that do
       are both Windows'. */
    return NULL;
#endif
}

/* The window losing and gaining the keyboard, which the engine pauses
   on. The old procedure did this on WM_ACTIVATEAPP, and released the
   input devices with it so that another window could have them. */
static void PLT_l_Focus(int gained)
{
    ENG_gb_ForcePauseEngine = gained ? 0 : 1;
    if (gained)
        pcINO_mouse_and_keyb_acquire();
    else
        pcINO_mouse_and_keyb_unacquire();
}

/* Asked to close, from the window's button or from the system. The old
   procedure set this flag on WM_CLOSE, WM_QUIT and a close from the
   system menu, and only while the engine was running -- before that
   there is nothing to ask to stop. */
static void PLT_l_CloseAsked(void)
{
    if (ENG_gb_EngineRunning)
        ENG_gb_ExitApplication = 1;
}

/* A character typed while the overlay's command field has the
   keyboard. SDL delivers text as UTF-8 rather than as a character at a
   time, and the field takes the plain ones, so the bytes below seven
   bits are handed over and anything else is dropped rather than
   mangled. */
static void PLT_l_Text(const char* text)
{
    const unsigned char* p = (const unsigned char*)text;

    for (; p && *p; p++)
        if (*p < 0x80u)
            OVL_b_CommandChar((int)*p);
}


/* ------------------------------------------------- keyboard and mouse */

/* The game's key codes are **DirectInput's**, and they stay that way.
 *
 * Every table in the engine is written in them -- which action is on
 * which key, the names shown on the controls screen, and the bindings
 * in a saved game. Changing the engine to SDL's numbering would mean
 * rewriting all of that and invalidating everyone's saved bindings, to
 * no end: what the game needs is which physical key is down, and the
 * two numberings are two spellings of the same thing.
 *
 * So the translation lives here, and nothing above this file knows
 * that SDL has replaced anything. DirectInput's codes are PS/2 set 1
 * scan codes and SDL's are USB usage codes; this is the
 * correspondence. A key the table does not name is a key the game has
 * never had a code for.
 */
static const struct
{
    SDL_Scancode  sdl;
    unsigned char dik;
} plt_keymap[] = {
    { SDL_SCANCODE_ESCAPE,       0x01 },
    { SDL_SCANCODE_1,            0x02 },
    { SDL_SCANCODE_2,            0x03 },
    { SDL_SCANCODE_3,            0x04 },
    { SDL_SCANCODE_4,            0x05 },
    { SDL_SCANCODE_5,            0x06 },
    { SDL_SCANCODE_6,            0x07 },
    { SDL_SCANCODE_7,            0x08 },
    { SDL_SCANCODE_8,            0x09 },
    { SDL_SCANCODE_9,            0x0A },
    { SDL_SCANCODE_0,            0x0B },
    { SDL_SCANCODE_MINUS,        0x0C },
    { SDL_SCANCODE_EQUALS,       0x0D },
    { SDL_SCANCODE_BACKSPACE,    0x0E },
    { SDL_SCANCODE_TAB,          0x0F },
    { SDL_SCANCODE_Q,            0x10 },
    { SDL_SCANCODE_W,            0x11 },
    { SDL_SCANCODE_E,            0x12 },
    { SDL_SCANCODE_R,            0x13 },
    { SDL_SCANCODE_T,            0x14 },
    { SDL_SCANCODE_Y,            0x15 },
    { SDL_SCANCODE_U,            0x16 },
    { SDL_SCANCODE_I,            0x17 },
    { SDL_SCANCODE_O,            0x18 },
    { SDL_SCANCODE_P,            0x19 },
    { SDL_SCANCODE_LEFTBRACKET,  0x1A },
    { SDL_SCANCODE_RIGHTBRACKET, 0x1B },
    { SDL_SCANCODE_RETURN,       0x1C },
    { SDL_SCANCODE_LCTRL,        0x1D },
    { SDL_SCANCODE_A,            0x1E },
    { SDL_SCANCODE_S,            0x1F },
    { SDL_SCANCODE_D,            0x20 },
    { SDL_SCANCODE_F,            0x21 },
    { SDL_SCANCODE_G,            0x22 },
    { SDL_SCANCODE_H,            0x23 },
    { SDL_SCANCODE_J,            0x24 },
    { SDL_SCANCODE_K,            0x25 },
    { SDL_SCANCODE_L,            0x26 },
    { SDL_SCANCODE_SEMICOLON,    0x27 },
    { SDL_SCANCODE_APOSTROPHE,   0x28 },
    { SDL_SCANCODE_GRAVE,        0x29 },
    { SDL_SCANCODE_LSHIFT,       0x2A },
    { SDL_SCANCODE_BACKSLASH,    0x2B },
    { SDL_SCANCODE_Z,            0x2C },
    { SDL_SCANCODE_X,            0x2D },
    { SDL_SCANCODE_C,            0x2E },
    { SDL_SCANCODE_V,            0x2F },
    { SDL_SCANCODE_B,            0x30 },
    { SDL_SCANCODE_N,            0x31 },
    { SDL_SCANCODE_M,            0x32 },
    { SDL_SCANCODE_COMMA,        0x33 },
    { SDL_SCANCODE_PERIOD,       0x34 },
    { SDL_SCANCODE_SLASH,        0x35 },
    { SDL_SCANCODE_RSHIFT,       0x36 },
    { SDL_SCANCODE_KP_MULTIPLY,  0x37 },
    { SDL_SCANCODE_LALT,         0x38 },
    { SDL_SCANCODE_SPACE,        0x39 },
    { SDL_SCANCODE_CAPSLOCK,     0x3A },
    { SDL_SCANCODE_F1,           0x3B },
    { SDL_SCANCODE_F2,           0x3C },
    { SDL_SCANCODE_F3,           0x3D },
    { SDL_SCANCODE_F4,           0x3E },
    { SDL_SCANCODE_F5,           0x3F },
    { SDL_SCANCODE_F6,           0x40 },
    { SDL_SCANCODE_F7,           0x41 },
    { SDL_SCANCODE_F8,           0x42 },
    { SDL_SCANCODE_F9,           0x43 },
    { SDL_SCANCODE_F10,          0x44 },
    { SDL_SCANCODE_NUMLOCKCLEAR, 0x45 },
    { SDL_SCANCODE_SCROLLLOCK,   0x46 },
    { SDL_SCANCODE_KP_7,         0x47 },
    { SDL_SCANCODE_KP_8,         0x48 },
    { SDL_SCANCODE_KP_9,         0x49 },
    { SDL_SCANCODE_KP_MINUS,     0x4A },
    { SDL_SCANCODE_KP_4,         0x4B },
    { SDL_SCANCODE_KP_5,         0x4C },
    { SDL_SCANCODE_KP_6,         0x4D },
    { SDL_SCANCODE_KP_PLUS,      0x4E },
    { SDL_SCANCODE_KP_1,         0x4F },
    { SDL_SCANCODE_KP_2,         0x50 },
    { SDL_SCANCODE_KP_3,         0x51 },
    { SDL_SCANCODE_KP_0,         0x52 },
    { SDL_SCANCODE_KP_PERIOD,    0x53 },
    { SDL_SCANCODE_F11,          0x57 },
    { SDL_SCANCODE_F12,          0x58 },
    { SDL_SCANCODE_KP_ENTER,     0x9C },
    { SDL_SCANCODE_RCTRL,        0x9D },
    { SDL_SCANCODE_KP_DIVIDE,    0xB5 },
    { SDL_SCANCODE_PRINTSCREEN,  0xB7 },
    { SDL_SCANCODE_RALT,         0xB8 },
    { SDL_SCANCODE_PAUSE,        0xC5 },
    { SDL_SCANCODE_HOME,         0xC7 },
    { SDL_SCANCODE_UP,           0xC8 },
    { SDL_SCANCODE_PAGEUP,       0xC9 },
    { SDL_SCANCODE_LEFT,         0xCB },
    { SDL_SCANCODE_RIGHT,        0xCD },
    { SDL_SCANCODE_END,          0xCF },
    { SDL_SCANCODE_DOWN,         0xD0 },
    { SDL_SCANCODE_PAGEDOWN,     0xD1 },
    { SDL_SCANCODE_INSERT,       0xD2 },
    { SDL_SCANCODE_DELETE,       0xD3 },
    { SDL_SCANCODE_LGUI,         0xDB },
    { SDL_SCANCODE_RGUI,         0xDC },
    { SDL_SCANCODE_APPLICATION,  0xDD },
};

/* Built once from the table above, so a frame of translation is a
   lookup per key rather than a search. */
static unsigned char plt_dikOf[SDL_SCANCODE_COUNT];
static SDL_Scancode  plt_scancodeOf[256];
static int           plt_keymapReady;

static void PLT_l_KeymapBuild(void)
{
    size_t i;

    if (plt_keymapReady)
        return;
    for (i = 0; i < sizeof(plt_keymap) / sizeof(plt_keymap[0]); i++)
    {
        plt_dikOf[plt_keymap[i].sdl] = plt_keymap[i].dik;
        plt_scancodeOf[plt_keymap[i].dik] = plt_keymap[i].sdl;
    }
    plt_keymapReady = 1;
}

/* The wheel is the one part of the mouse that arrives as events rather
   than as state: SDL reports a movement, and the game asks how far it
   has moved since it last looked. */
static int plt_wheel;

/* Whether the game has the input at all. The old device was taken
   exclusively and released while the overlay's command field had the
   keyboard; relative mouse mode does the taking now and this does the
   releasing. */
static int plt_inputHeld;

void PLT_InputCapture(int on)
{
    plt_inputHeld = on ? 1 : 0;
    if (plt_window)
    {
        SDL_SetWindowRelativeMouseMode(plt_window, plt_inputHeld ? true : false);
        /* Typed text is delivered only to a window that has asked for
           it, and a window has not asked by default -- so without
           this the command field saw the keys that arrive as keys,
           which is why its Enter worked, and none of the characters.
           The game having the input and the field having it are the
           same switch, so this is the place for it: while the game is
           being read the keyboard as game keys there is nothing that
           wants text, and while it is not, the field does. */
        if (plt_inputHeld)
            SDL_StopTextInput(plt_window);
        else
            SDL_StartTextInput(plt_window);
    }
    if (!plt_inputHeld)
        plt_wheel = 0;
}

const char* PLT_sz_KeyName(int dik)
{
    PLT_l_KeymapBuild();
    if (dik < 0 || dik > 255 || plt_scancodeOf[dik] == SDL_SCANCODE_UNKNOWN)
        return "";
    return SDL_GetScancodeName(plt_scancodeOf[dik]);
}

int PLT_b_KeyHeld(int key)
{
    const bool*  state;
    int          numKeys = 0;
    SDL_Scancode scan;

    if (key < 0 || key > 255)
        return 0;
    PLT_l_KeymapBuild();
    /* The same table the key names are read through: one map, built
       once, in both directions. */
    scan = plt_scancodeOf[key];
    if (scan == SDL_SCANCODE_UNKNOWN)
        return 0;
    state = SDL_GetKeyboardState(&numKeys);
    if (!state || (int)scan >= numKeys)
        return 0;
    return state[scan] ? 1 : 0;
}

void PLT_WaitKeyRelease(int key)
{
    while (PLT_b_KeyHeld(key))
    {
        /* The state the question above reads is only as new as the
           last time events were taken off the system, so the pump has
           to be in here or the key is held forever. A millisecond
           between looks rather than none: the call this replaced spun
           as fast as the processor would go. */
        SDL_PumpEvents();
        SDL_Delay(1);
    }
}

void PLT_SleepMs(int ms)
{
    if (ms > 0)
        SDL_Delay((Uint32)ms);
}

void PLT_ReadDevices(unsigned char* keys, int* mouse)
{
    const bool* state;
    int         numKeys = 0;
    int         i;
    float       dx = 0.0f;
    float       dy = 0.0f;
    SDL_MouseButtonFlags buttons;

    if (keys)
        memset(keys, 0, 266);
    if (mouse)
        memset(mouse, 0, 5 * sizeof(int));
    if (!plt_window || !plt_inputHeld)
        return;

    PLT_l_KeymapBuild();

    /* The keyboard as a block, which is how the game reads it: what is
       down now, rather than what has been pressed since last time. */
    state = SDL_GetKeyboardState(&numKeys);
    if (state && keys)
    {
        if (numKeys > SDL_SCANCODE_COUNT)
            numKeys = SDL_SCANCODE_COUNT;
        for (i = 0; i < numKeys; i++)
            if (state[i] && plt_dikOf[i])
                keys[plt_dikOf[i]] = 0x80;
    }

    /* The mouse: how far it has moved since the last frame, which is
       what relative mode answers, and which buttons are down. The five
       slots are the ones the old device filled -- the pointer, the
       wheel, and two words of buttons. */
    buttons = SDL_GetRelativeMouseState(&dx, &dy);
    if (mouse)
    {
        mouse[0] = (int)dx;
        mouse[1] = (int)dy;
        mouse[2] = plt_wheel;
        mouse[3] = (int)(((buttons & SDL_BUTTON_LMASK) ? 0x80u : 0u)
                       | ((buttons & SDL_BUTTON_RMASK) ? 0x8000u : 0u)
                       | ((buttons & SDL_BUTTON_MMASK) ? 0x800000u : 0u)
                       | ((buttons & SDL_BUTTON_X1MASK) ? 0x80000000u : 0u));
        mouse[4] = (buttons & SDL_BUTTON_X2MASK) ? 0x80 : 0;
    }
    plt_wheel = 0;
}

/* The engine's statistics in the window's caption, on Home.
 *
 * They used to arrive by clicking the title bar, which was a
 * non-client click and is not something a window SDL owns passes on.
 * The function keys are taken -- F1 to F10 are the debug keys and F11
 * raises the command field -- so these are on Home, which nothing in
 * the game is bound to. */
static void PLT_l_ShowStats(void)
{
    char stats[512];

    if (!plt_window)
        return;
    stats[0] = 0;
    ENG_GetStatsText(stats);
    SDL_SetWindowTitle(plt_window, stats);
}

int PLT_b_Launch(const char* const* args)
{
    SDL_Process* child;

    if (!args || !args[0])
        return 0;
    child = SDL_CreateProcess(args, false);
    if (!child)
    {
        DBG_OutputDebugString("PLT: could not start %s: %s\n",
                              args[0], SDL_GetError());
        return 0;
    }
    /* Let go of it rather than waiting: destroying the handle does not
       stop what was started. */
    SDL_DestroyProcess(child);
    return 1;
}

void PLT_Pump(void)
{
    SDL_Event ev;
    float     startTimeStamp;

    /* Timed into the raster the message loop was timed into, so the
       performance display goes on meaning what it meant. */
    PRO_PerfCounterStartBlock(&PRO_WinMsg);
    startTimeStamp = TIM_f_Clock_TrueRead();

    while (SDL_PollEvent(&ev))
    {
        switch (ev.type)
        {
        case SDL_EVENT_QUIT:
        case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
            PLT_l_CloseAsked();
            break;
        case SDL_EVENT_WINDOW_FOCUS_GAINED:
            PLT_l_Focus(1);
            break;
        case SDL_EVENT_WINDOW_FOCUS_LOST:
            PLT_l_Focus(0);
            break;
        case SDL_EVENT_MOUSE_WHEEL:
            /* Whole notches, kept until the game asks. The old device
               reported the wheel the same way: a movement since the
               last read rather than a position. */
            plt_wheel += (int)ev.wheel.y;
            break;
        case SDL_EVENT_TEXT_INPUT:
            PLT_l_Text(ev.text.text);
            break;
        case SDL_EVENT_KEY_DOWN:
            /* The three the field reads as keys rather than as text:
               SDL sends no text for them. Return and escape put the
               line away, backspace takes a character off it. */
            if (ev.key.key == SDLK_RETURN || ev.key.key == SDLK_KP_ENTER)
                OVL_b_CommandChar('\r');
            else if (ev.key.key == SDLK_BACKSPACE)
                OVL_b_CommandChar('\b');
            else if (ev.key.key == SDLK_ESCAPE)
                OVL_b_CommandChar(27);
            else if (ev.key.key == SDLK_HOME && !ev.key.repeat)
                PLT_l_ShowStats();
            break;
        default:
            break;
        }
    }

    TIM_SpentTimeWithHandlingWinMessages += TIM_f_Clock_TrueRead() - startTimeStamp;
    PRO_PerfCounterEndBlock(&PRO_WinMsg);
}
