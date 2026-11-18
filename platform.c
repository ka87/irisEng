#include "platform.h"

#include "common.h"
#include "engine.h"
#include "inputoutput.h"
#include "profiler.h"
#include "time.h"
#include "dbg.h"
#include "win/overlay.h"
#include "win/wininputoutput.h"

#include <SDL3/SDL.h>

/* The window, and the events that used to be window messages.
 *
 * Which events stand in for which message is the whole of the
 * interesting part, so each one says so where it is handled below.
 */

static SDL_Window* plt_window;
static int         plt_up;

void PLT_WindowSettings(int* width, int* height, int* windowed)
{
    /* The defaults the game has always started from. */
    if (width)
        *width = 640;
    if (height)
        *height = 480;
    if (windowed)
        *windowed = 0;

#ifdef _WIN32
    {
        HKEY  key;
        DWORD value = 0;
        DWORD type = 0;
        DWORD size = sizeof(value);

        /* Still the registry, because that is where the game's
           settings are and moving them is a separate question from
           moving the window. The one platform-shaped thing in here. */
        if (!RegOpenKeyExA(HKEY_CURRENT_USER, "Software\\IRISEng\\Basic video",
                           0, KEY_READ, &key))
        {
            /* Each one a four-byte number or it is not one of ours,
               which is the same test the renderer's own reader made. */
#define PLT_REGREAD(name)                                              \
            (size = sizeof(value),                                     \
             !RegQueryValueExA(key, (name), 0, &type, (LPBYTE)&value,  \
                               &size)                                  \
             && type == REG_DWORD && size == sizeof(value))

            if (windowed && PLT_REGREAD("Windowed") && value)
                *windowed = 1;
            if (width && PLT_REGREAD("Width") && (int)value > 0)
                *width = (int)value;
            if (height && PLT_REGREAD("Height") && (int)value > 0)
                *height = (int)value;
#undef PLT_REGREAD
            RegCloseKey(key);
        }
    }
#endif
}

int PLT_b_Open(const char* title, int width, int height, int windowed)
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

    /* The window the game asked for: resizable, because the old one
       had a sizing border, and fullscreen unless the settings say
       otherwise -- which is the sense the old flag had. */
    flags = SDL_WINDOW_RESIZABLE;
    if (!windowed)
        flags |= SDL_WINDOW_FULLSCREEN;

    plt_window = SDL_CreateWindow(title ? title : "IRISEngine",
                                  width > 0 ? width : 640,
                                  height > 0 ? height : 480,
                                  flags);
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
        pcINO_dinput_mouse_and_keyb_acquire();
    else
        pcINO_dinput_mouse_and_keyb_unacquire();
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
            break;
        default:
            break;
        }
    }

    TIM_SpentTimeWithHandlingWinMessages += TIM_f_Clock_TrueRead() - startTimeStamp;
    PRO_PerfCounterEndBlock(&PRO_WinMsg);
}
